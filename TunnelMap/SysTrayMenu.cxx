
#include "SysTrayMenu.h"
#include "ProcessList.h"
#include <QAction.h>
#include <QApplication.h>

#include <iostream>
using namespace std;

SysTrayMenu::SysTrayMenu(ProcessList* pl, QWidget* parent) :
  QMenu::QMenu(parent),
  proc_list_(pl),
  config_action_(0),
  sep_(0)
{
  config_action_ = this->addAction("Configure...");
  this->addSeparator();

  connect_action_ = this->addAction("Connect All");
  disconnect_action_ = this->addAction("Disconnect All");
  this->addSeparator();

  for( int i=0; i<proc_list_->size(); ++i ){
    if( proc_list_->at(i)->get_name().trimmed().isEmpty() ) continue;
    this->addAction( proc_list_->at(i)->get_action() );
    tunnel_actions_.append( proc_list_->at(i)->get_action() );
  }
  sep_ = this->addSeparator();
  quit_action_ = this->addAction("Quit");

  connect(connect_action_, SIGNAL(triggered()),
	  proc_list_, SLOT(connect_all()));
  connect(disconnect_action_, SIGNAL(triggered()),
	  proc_list_, SLOT(disconnect_all()));
  connect(quit_action_, SIGNAL(triggered()),
	  qApp, SLOT(quit()));
}


void SysTrayMenu::refresh()
{
  for( int i=0; i<tunnel_actions_.size(); i++) {
    removeAction(tunnel_actions_[i]);
  }
  tunnel_actions_.clear();

  for( int i=0; i<proc_list_->size(); ++i ){
    if( proc_list_->at(i)->get_name().trimmed().isEmpty() ) continue;
    tunnel_actions_.append( proc_list_->at(i)->get_action() );
  }

  insertActions( sep_, tunnel_actions_ );
  return;
}
