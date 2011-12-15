//----------------------------------------------------------------
// Class: ProcessList
// Description: This class is the model that is used in the ListView
// of the ConfigWindow.  It is also where the internal list of tunnels
// is stored.
// ----------------------------------------------------------------

#ifndef PROCESSLIST_H__
#define PROCESSLIST_H__

#include <QAbstractListModel>
#include <QList>
#include <QTimer>
#include "TunnelProcess.h"

class ProcessList : public QAbstractListModel
{
  Q_OBJECT

 public:
  ProcessList();
  virtual ~ProcessList();

  QVariant data( const QModelIndex &, int ) const;
  int rowCount( const QModelIndex & ) const {return tunnel_list_.size();}

  int size() {return tunnel_list_.size();}
  TunnelProcess* at(int i) {return tunnel_list_.at(i);}

  QString get_ssh_cmd() const {return ssh_cmd_;}
  QString get_ssh_args() const {return ssh_args_;}
  QList<QString> get_valid_nics() const {return valid_nics_;}

  void set_ssh_cmd(QString s);
  void set_ssh_args(QString s);
  void set_valid_nics(QList<QString> vn) {valid_nics_ = vn;}

  void append(TunnelProcess*);
  TunnelProcess* insert( int row );
  void remove( int row );

 signals:
  void connected(QString, QString);
  void modified();
  void network_up();

 public slots:
  void connect_all();
  void reconnect_all();
  void disconnect_all();
  void load();
  void save();
  void check_network_interfaces();

 protected slots:
  void notify_connected(QString a, QString b)
  { emit connected(a,b); }

 protected:
  QList<TunnelProcess*> tunnel_list_;
  QList<QString> valid_nics_;
  QString ssh_cmd_;
  QString ssh_args_;

  QTimer net_check_timer_;
};

#endif // PROCESSLIST_H__
