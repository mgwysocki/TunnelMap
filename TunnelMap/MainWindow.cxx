#include <QtGui>
#include "MainWindow.h"
#include "TunnelWidget.h"
//#include <ApplicationServices/ApplicationServices.h>

#include <iostream>

MainWindow::MainWindow() :
  QMainWindow::QMainWindow()
{
  QIcon window_icon(":/icons/systray.png");
  setWindowIcon(window_icon);
  setWindowTitle(tr("TunnelMap"));

  QIcon icon(":/icons/systray.png");
  systray_icon_ = new QSystemTrayIcon(icon, this);
  systray_icon_->show();

  QScrollArea* sa = new QScrollArea;
  QWidget* w = new QWidget;
  vlayout_ = new QVBoxLayout;

  QSettings settings;
  QString ssh_cmd = settings.value("command").toString();
  QString ssh_args = settings.value("arguments").toString();
  if( ssh_cmd.isNull() ){  // assume it's the first time this is run
    ssh_cmd = tr("/usr/bin/ssh");
    ssh_args = tr("-CN");
  }

  QHBoxLayout* hlayout = new QHBoxLayout;
  QPushButton* new_tunnel_btn = new QPushButton("New Tunnel");
  QPushButton* connect_all_btn = new QPushButton("Connect All");
  QPushButton* disconnect_all_btn = new QPushButton("Disconnect All");
  hlayout->addWidget(new_tunnel_btn);
  hlayout->addStretch(1);
  hlayout->addWidget(connect_all_btn);
  hlayout->addWidget(disconnect_all_btn);
  vlayout_->addLayout(hlayout);

  connect(new_tunnel_btn, SIGNAL(clicked()),
	  this, SLOT(add_new_tunnel_()));
  connect(connect_all_btn, SIGNAL(clicked()),
	  this, SLOT(connect_all_()));
  connect(disconnect_all_btn, SIGNAL(clicked()),
	  this, SLOT(disconnect_all_()));

  hlayout = new QHBoxLayout;
  QLabel* ssh_cmd_label = new QLabel("ssh Command:");
  ssh_cmd_edit_ = new QLineEdit("");
  ssh_cmd_edit_->setText(ssh_cmd);
  QLabel* ssh_args_label = new QLabel("ssh Standard Arguments:");
  ssh_args_edit_ = new QLineEdit("");
  ssh_args_edit_->setText(ssh_args);
  QPushButton* apply_btn = new QPushButton("Apply");
  hlayout->addWidget(ssh_cmd_label);
  hlayout->addWidget(ssh_cmd_edit_);
  hlayout->addWidget(ssh_args_label);
  hlayout->addWidget(ssh_args_edit_);
  hlayout->addWidget(apply_btn);
  vlayout_->addLayout(hlayout);
  connect(apply_btn, SIGNAL(clicked()),
	  this, SLOT(save_all_settings()));

  load_tunnels_();
  for( int i=0; i<tunnels_.size(); i++ ){
    vlayout_->addWidget(tunnels_.at(i));
  }

  vlayout_->addStretch(1);
  vlayout_->setContentsMargins(0,0,0,0);
  w->setLayout(vlayout_);
  vlayout_->setSizeConstraint(QLayout::SetMinimumSize);

  sa->setWidget(w);
  this->setCentralWidget(sa);

  createActions();
  createMenus();

  // Set the maximum window size to the desktop size minus a 50 pixel border
  QSize maxsize(QApplication::desktop()->size());
  maxsize += QSize(-50, -50);
  setMaximumSize(maxsize);

  QMenu* new_menu = new QMenu("Tunnels", this);
  QAction* a = new_menu->addAction("Configure...");
  connect(a, SIGNAL(triggered()),  this, SLOT(show()));
  new_menu->addSeparator();
  a = new_menu->addAction("Connect All");
  connect(a, SIGNAL(triggered()),  this, SLOT(connect_all_()));
  a = new_menu->addAction("Disconnect All");
  connect(a, SIGNAL(triggered()),  this, SLOT(disconnect_all_()));
  new_menu->addSeparator();

  for( int i=0; i<tunnels_.size(); ++i ){
    if( tunnels_.at(i)->get_args().trimmed().isEmpty() ) continue;
    new_menu->addAction( tunnels_.at(i)->get_action() );
  }
  new_menu->addSeparator();
  a = new_menu->addAction("Quit");
  connect(a, SIGNAL(triggered()),  qApp, SLOT(quit()));

  systray_icon_->setContextMenu(new_menu);

  connect(systray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	  this, SLOT(systray_activate_(QSystemTrayIcon::ActivationReason)));


  // Set it to some default size
  //resize(800, 600);

  // Testing...
  std::cout << "Scrollarea: " << sa->width() << "," << sa->height() << std::endl;
  std::cout << "Widget: " << w->width() << "," << w->height() << std::endl;
  std::cout << "Tunnel: " << tunnels_[0]->width() << "," << tunnels_[0]->height() << std::endl;
}

MainWindow::~MainWindow()
{
  std::cout << "MainWindow::~MainWindow()" << std::endl;
  disconnect_all_();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  save_all_settings();
  event->accept();
  hide();
// #ifdef Q_WS_MACX
//   ProcessSerialNumber psn;
//   if( GetCurrentProcess(&psn) == noErr ){
//     TransformProcessType(&psn,
// 			 kProcessTransformToBackgroundApplication);
//   }
// #endif
  return;
}

void MainWindow::add_new_tunnel_()
{
  TunnelWidget* tw = new TunnelWidget;
  tunnels_.append(tw);
  vlayout_->addWidget(tw);
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Image Viewer"),
		     tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
			"and QScrollArea to display an image. QLabel is typically used "
			"for displaying a text, but it can also display an image. "
			"QScrollArea provides a scrolling view around another widget. "
			"If the child widget exceeds the size of the frame, QScrollArea "
			"automatically provides scroll bars. </p><p>The example "
			"demonstrates how QLabel's ability to scale its contents "
			"(QLabel::scaledContents), and QScrollArea's ability to "
			"automatically resize its contents "
			"(QScrollArea::widgetResizable), can be used to implement "
			"zooming and scaling features. </p><p>In addition the example "
			"shows how to use QPainter to print an image.</p>"));
}

void MainWindow::createActions()
{
  about_act_ = new QAction(tr("&About"), this);
  connect(about_act_, SIGNAL(triggered()), this, SLOT(about()));

  about_qt_act_ = new QAction(tr("About &Qt"), this);
  connect(about_qt_act_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  help_menu_ = new QMenu(tr("&Help"), this);
  help_menu_->addAction(about_act_);
  help_menu_->addAction(about_qt_act_);

  menuBar()->addMenu(help_menu_);
}

void MainWindow::connect_all_()
{
  for(int i=0; i<tunnels_.size(); i++) {
    tunnels_.at(i)->open();
  }
  return;
}

void MainWindow::disconnect_all_()
{
  for(int i=0; i<tunnels_.size(); i++) {
    tunnels_.at(i)->close();
  }
  return;
}

void MainWindow::load_tunnels_()
{
  QSettings settings;
  int size = settings.beginReadArray("tunnels");
  for( int i=0; i<size; ++i ){
    settings.setArrayIndex(i);
    TunnelWidget* tw = new TunnelWidget;
    tw->set_name(settings.value("name").toString() );
    tw->set_args(settings.value("args").toString() );
    tw->set_enabled( settings.value("enabled").toBool() );
    connect(tw, SIGNAL(connected(QString, QString)),
	    this, SLOT(notify_connected(QString, QString)));
    tunnels_.append(tw);
  }
  settings.endArray();
  return;
}

void MainWindow::save_tunnels_()
{
  std::cout << "Saving tunnels...";
  QSettings settings;
  settings.beginWriteArray("tunnels");
  int j=0;
  for( int i=0; i<tunnels_.size(); ++i ){
    if( tunnels_.at(i)->get_args().trimmed().isEmpty() ) continue;

    settings.setArrayIndex(j);
    settings.setValue("name", tunnels_.at(i)->get_name());
    settings.setValue("args", tunnels_.at(i)->get_args());
    settings.setValue("enabled", tunnels_.at(i)->get_enabled());
    ++j;
  }
  settings.endArray();
  std::cout << "done" << std::endl;
  return;
}

void MainWindow::systray_activate_(QSystemTrayIcon::ActivationReason reason)
{
  if( reason == QSystemTrayIcon::DoubleClick){
    this->show();
  }
  return;
}

void MainWindow::save_all_settings()
{
  QSettings settings;
  settings.setValue("command", ssh_cmd_edit_->text());
  settings.setValue("arguments", ssh_args_edit_->text());
  save_tunnels_();
  settings.sync();
  return;
}

void MainWindow::notify_connected(QString tunnel_name, QString tunnel_cmd)
{
  QString title = tr("The tunnel '") + tunnel_name + tr("' has been connected!");
  QString message = tunnel_cmd.replace("\"", "'");
  std::cout << "MainWindow::notify_connected()" << std::endl;
  systray_icon_->showMessage(title, message, QSystemTrayIcon::NoIcon);
  return;
}
