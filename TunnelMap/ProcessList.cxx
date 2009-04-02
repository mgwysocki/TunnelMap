#include "ProcessList.h"
#include <QSettings.h>
#include <iostream>
using namespace std;

ProcessList::ProcessList() :
  QAbstractListModel::QAbstractListModel()
{}

ProcessList::~ProcessList()
{
  for( int i=tunnel_list_.size()-1; i>=0; --i ){
    delete tunnel_list_.takeAt(i);
  }
}

QVariant ProcessList::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  if( row < 0 || row >= tunnel_list_.size() ) return QVariant();

  if( role == Qt::DisplayRole ){
    QString d = tunnel_list_.at(row)->get_name();
    return QVariant(d);
  }
  return QVariant();
}

void ProcessList::append(TunnelProcess* tp)
{
  beginInsertRows(QModelIndex(), size(), size());
  tunnel_list_.append(tp);
  endInsertRows();
  emit modified();
  return;
}

TunnelProcess* ProcessList::insert( int row )
{
  if( row < 0 || row > tunnel_list_.size() ) return 0;

  beginInsertRows(QModelIndex(), row, row);
  tunnel_list_.insert(row, new TunnelProcess);
  endInsertRows();
  emit modified();
  return tunnel_list_[row];
}

void ProcessList::remove( int row )
{
  if( row < 0 || row >= tunnel_list_.size() ) return;

  beginRemoveRows(QModelIndex(), row, row);
  TunnelProcess* tp = tunnel_list_.takeAt(row);
  delete tp;
  endRemoveRows();
  emit modified();
  return;
}

void ProcessList::load()
{
  QSettings settings;
  ssh_cmd_ = settings.value("command").toString();
  ssh_args_ = settings.value("arguments").toString();

  int size = settings.beginReadArray("tunnels");
  for( int i=0; i<size; ++i ){
    settings.setArrayIndex(i);
    insert(i);
    TunnelProcess* tproc = tunnel_list_[i];
    tproc->set_ssh_cmd( ssh_cmd_ );
    tproc->set_ssh_args( ssh_args_ );
    tproc->set_name( settings.value("name").toString() );
    tproc->set_args(settings.value("args").toString() );
    tproc->set_auto_connect( settings.value("enabled").toBool() );
    tproc->set_dependent( settings.value("dependent").toBool() );
    connect(tproc, SIGNAL(connected(QString, QString)),
	    this, SLOT(notify_connected(QString, QString)));
  }

  settings.endArray();
  emit modified();
  return;
}

void ProcessList::save()
{
  std::cout << "Saving tunnels...";
  QSettings settings;
  settings.setValue("command", ssh_cmd_);
  settings.setValue("arguments", ssh_args_);

  settings.beginWriteArray("tunnels");
  for( int i=0; i<tunnel_list_.size(); ++i ){
    settings.setArrayIndex(i);
    settings.setValue("name", tunnel_list_.at(i)->get_name());
    settings.setValue("args", tunnel_list_.at(i)->get_args());
    settings.setValue("enabled", tunnel_list_.at(i)->is_auto_connect());
    settings.setValue("dependent", tunnel_list_.at(i)->is_dependent());
  }
  settings.endArray();
  std::cout << "done" << std::endl;
  return;
}

void ProcessList::connect_all()
{
  for( int i=0; i<tunnel_list_.size(); ++i ){
    tunnel_list_[i]->open();
  }  
  return;
}

void ProcessList::disconnect_all()
{
  for( int i=0; i<tunnel_list_.size(); ++i ){
    tunnel_list_[i]->close();
  }  
  return;
}
