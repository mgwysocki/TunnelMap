
#include <QPushButton>
#include <QLineEdit>
#include <QSizePolicy>
#include <QLabel>
#include <QHBoxLayout>
#include <QSettings>

#include "TunnelWidget.h"

#include <iostream>
using namespace std;

QIcon TunnelWidget::red_icon_;
QIcon TunnelWidget::green_icon_;

TunnelWidget::TunnelWidget(QWidget* parent) :
  QWidget::QWidget(parent),
  action_(new QAction(this)),
  close_initiated_(false)
{
  if(red_icon_.isNull()) {
    red_icon_.addFile(":/icons/redcircle.png");
    green_icon_.addFile(":/icons/greencircle.png");
  }

  QSizePolicy size_policy(QSizePolicy::Preferred, 
			  QSizePolicy::Preferred);
  setSizePolicy( size_policy );

  QHBoxLayout* hlayout = new QHBoxLayout;
  connect_button_ = new QPushButton("Connect", this);
  disconnect_button_ = new QPushButton("Disconnect", this);
  disconnect_button_->setDisabled(true);
  hlayout->addWidget(connect_button_);
  hlayout->addWidget(disconnect_button_);

  name_label_ = new QLabel("Name:");
  name_line_edit_ = new QLineEdit("");
  args_label_ = new QLabel("Arguments:");
  args_line_edit_ = new QLineEdit("");
  hlayout->addWidget(name_label_);
  hlayout->addWidget(name_line_edit_);
  hlayout->addWidget(args_label_);
  hlayout->addWidget(args_line_edit_);

  auto_connect_ = new QCheckBox("Auto-connect");
  hlayout->addWidget(auto_connect_);

  process_.setProcessChannelMode(QProcess::MergedChannels);
  process_.setStandardOutputFile("tunnel1.log");

  action_->setIcon(red_icon_);
  connect(name_line_edit_, SIGNAL(textChanged(QString)),
	  this, SLOT(change_name(QString)));

  // Active/Inactive slots
  connect( &process_, SIGNAL(started()),
	   this, SLOT(when_connected()) );
  connect( &process_, SIGNAL(finished(int, QProcess::ExitStatus)),
	   this, SLOT(when_disconnected()) );
  connect( action_, SIGNAL(triggered()),
	   this, SLOT(toggle_state()));

  // Action slots
  connect( connect_button_, SIGNAL(clicked()),
	   this, SLOT(open()) );
  connect( disconnect_button_, SIGNAL(clicked()),
	   this, SLOT(close()) );

  this->setLayout(hlayout);
}

TunnelWidget::~TunnelWidget()
{
  delete action_;
}

const QSize TunnelWidget::sizeHint()
{
  return QSize(200, 800);
}

void TunnelWidget::open()
{
  QString command = build_cmd_();
  cout << qPrintable(command) << endl;
  process_.start( command );
  return;
}

void TunnelWidget::close()
{
  close_initiated_ = true;
  process_.terminate();
  process_.waitForFinished();
  return;
}

void TunnelWidget::when_connected()
{
  connect_button_->setDisabled(true);
  disconnect_button_->setEnabled(true);
  action_->setIcon(green_icon_);
  timer_.restart();
  emit connected(get_name(), build_cmd_());
  return;
}

void TunnelWidget::when_disconnected()
{
  connect_button_->setEnabled(true);
  disconnect_button_->setDisabled(true);
  action_->setIcon(red_icon_);
  emit disconnected(get_name());

  if( !close_initiated_ && timer_.elapsed() > 5*60*1000)
    open();
  close_initiated_ = false;
  return;
}

void TunnelWidget::toggle_state()
{
  if( process_.state() == QProcess::NotRunning )  open();
  else                                            close();
  return;
}

void TunnelWidget::change_name(QString newname)
{
  action_->setText(newname);
  return;
}

QString TunnelWidget::build_cmd_()
{
  QSettings settings;
  QString cmd = settings.value("command").toString();
  QString args = settings.value("arguments").toString();
  QString command = cmd + tr(" ") + args + tr(" ") + args_line_edit_->text();
  return command;
}
