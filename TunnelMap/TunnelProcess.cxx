
#include "TunnelProcess.h"

#include <iostream>
using namespace std;

QIcon TunnelProcess::red_icon_;
QIcon TunnelProcess::green_icon_;

TunnelProcess::TunnelProcess(QObject* parent) :
  QProcess::QProcess(parent)
{
  init();
}


TunnelProcess::TunnelProcess(const TunnelProcess &tp) :
  QProcess::QProcess(tp.parent())
{
  init();
  *this = tp;
}

void TunnelProcess::init()
{
  cout << "TunnelProcess::init()" << endl;
  setProcessChannelMode(QProcess::MergedChannels);
  setStandardOutputFile("tunnel1.log");
  if(red_icon_.isNull()) {
    red_icon_.addFile(":/icons/redcircle.png");
    green_icon_.addFile(":/icons/greencircle.png");
  }

  auto_connect_ = false;
  dependent_ = false;
  close_initiated_ = false;
  action_ = new QAction(red_icon_, name_, 0);

  // Active/Inactive slots
  connect( this, SIGNAL(started()),
	   this, SLOT(when_connected()) );
  connect( this, SIGNAL(finished(int, QProcess::ExitStatus)),
	   this, SLOT(when_disconnected(int, QProcess::ExitStatus)) );
  connect( action_, SIGNAL( triggered()     ),
	   this,      SLOT( toggle_state()  ));
}

TunnelProcess::~TunnelProcess()
{
  delete action_;
}

const TunnelProcess & TunnelProcess::operator=(const TunnelProcess &tp)
{
  name_ = tp.get_name();
  ssh_cmd_ = tp.get_ssh_cmd();
  ssh_args_ = tp.get_ssh_args();
  args_ = tp.get_args();
  auto_connect_ = tp.is_auto_connect();
  dependent_ = tp.is_dependent();
  return *this;
}

void TunnelProcess::open()
{
  QString command = get_cmd();
  cout << qPrintable(command) << endl;
  start( command );
  return;
}

void TunnelProcess::close()
{
  close_initiated_ = true;
  terminate();
  waitForFinished();
  return;
}

void TunnelProcess::when_connected()
{
  action_->setIcon(green_icon_);
  return;
}

void TunnelProcess::when_disconnected(int i, QProcess::ExitStatus status)
{
  action_->setIcon(red_icon_);

  if( status == QProcess::NormalExit ){
    cout << "Process exited normally with code " << i << endl;
  } else {
    cout << "Process crashed with code " << i << endl;
  }

  if( !close_initiated_ && auto_connect_)
    open();
  close_initiated_ = false;
  return;
}

void TunnelProcess::toggle_state()
{
  if( state() == QProcess::NotRunning )  open();
  else                                   close();
  return;
}
