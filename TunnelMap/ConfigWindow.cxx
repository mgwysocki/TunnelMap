#include <QtGui>
#include "ConfigWindow.h"
#include "ProcessList.h"
#include "SysTrayMenu.h"
#include "EditDialog.h"
#include <iostream>
using namespace std;

ConfigWindow::ConfigWindow(ProcessList* pl) :
  QMainWindow::QMainWindow(),
  proc_view_(new QListView(this)),
  proc_list_(pl)
{
  QIcon window_icon(":/icons/systray.png");
  setWindowIcon(window_icon);
  setWindowTitle(tr("TunnelMap"));

  QIcon icon(":/icons/systray.png");
  sys_tray_icon_ = new QSystemTrayIcon(icon, this);
  sys_tray_icon_->show();

  proc_view_->setModel(proc_list_);
  connect(proc_list_, SIGNAL( connected(QString, QString)),
	  this,         SLOT( notify_connected(QString, QString)));

  QWidget* w = new QWidget;
  QGridLayout* layout = new QGridLayout;

  QString ssh_cmd = proc_list_->get_ssh_cmd();
  QString ssh_args = proc_list_->get_ssh_args();
  if( ssh_cmd.isNull() ){  // assume it's the first time this is run
    ssh_cmd = tr("/usr/bin/ssh");
    ssh_args = tr("-CN");
  }

  QLabel* ssh_cmd_label = new QLabel("ssh Command:");
  ssh_cmd_edit_ = new QLineEdit("");
  ssh_cmd_edit_->setText(ssh_cmd);

  QLabel* ssh_args_label = new QLabel("ssh Standard Arguments:");
  ssh_args_edit_ = new QLineEdit("");
  ssh_args_edit_->setText(ssh_args);

  QPushButton* new_tunnel_btn = new QPushButton("New Tunnel");
  connect(new_tunnel_btn, SIGNAL(clicked()),
	  this,             SLOT(add_new_tunnel_()));

  QPushButton* apply_btn = new QPushButton("Apply");
  connect(apply_btn, SIGNAL(clicked()),
	  proc_list_,  SLOT(save()));

  layout->addWidget(ssh_cmd_label, 0, 0, 1, 1);
  layout->addWidget(ssh_cmd_edit_, 0, 1, 1, 1);
  layout->addWidget(ssh_args_label, 1, 0, 1, 1);
  layout->addWidget(ssh_args_edit_, 1, 1, 1, 1);
  layout->addWidget(proc_view_, 2, 0, 4, 2);
  layout->addWidget(new_tunnel_btn, 6, 0, 1, 1);
  layout->addWidget(apply_btn, 6, 1, 1, 1);

  w->setLayout(layout);
  this->setCentralWidget(w);

  // Set the maximum window size to the desktop size minus a 50 pixel border
  QSize maxsize(QApplication::desktop()->size());
  maxsize += QSize(-50, -50);
  setMaximumSize(maxsize);

  sys_tray_menu_ = new SysTrayMenu(proc_list_, this);
  connect( proc_list_,     SIGNAL( modified() ),
	   sys_tray_menu_,   SLOT( refresh()  ));
  connect(sys_tray_menu_->get_config_action(), SIGNAL(triggered()),
	  this,                                  SLOT(show()));

  sys_tray_icon_->setContextMenu(sys_tray_menu_);
  connect(sys_tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	  this, SLOT(sys_tray_activate_(QSystemTrayIcon::ActivationReason)));
}

ConfigWindow::~ConfigWindow()
{
  std::cout << "ConfigWindow::~ConfigWindow()" << std::endl;
  proc_list_->disconnect_all();
}

void ConfigWindow::closeEvent(QCloseEvent *event)
{
  proc_list_->save();
  event->accept();
  hide();
  return;
}

void ConfigWindow::add_new_tunnel_()
{
  TunnelProcess* new_proc = new TunnelProcess;
  new_proc->set_ssh_cmd( ssh_cmd_edit_->text() );
  new_proc->set_ssh_args( ssh_args_edit_->text() );

  EditDialog ed(new_proc, this);
  if(ed.exec() == QDialog::Accepted) {
    cout << "here1" << endl;
    proc_list_->append( new_proc );
    cout << "here2" << endl;
  } else {
    delete new_proc;
  }
}

void ConfigWindow::sys_tray_activate_(QSystemTrayIcon::ActivationReason reason)
{
  if( reason == QSystemTrayIcon::DoubleClick){
    this->show();
  }
  return;
}

void ConfigWindow::notify_connected(QString tunnel_name, QString tunnel_cmd)
{
  QString title = tr("The tunnel '") + tunnel_name + tr("' has been connected!");
  QString message = tunnel_cmd.replace("\"", "'");
  std::cout << "ConfigWindow::notify_connected()" << std::endl;
  sys_tray_icon_->showMessage(title, message, QSystemTrayIcon::NoIcon);
  return;
}

void ConfigWindow::show()
{
  raise();
  activateWindow();
  QMainWindow::show();
}
