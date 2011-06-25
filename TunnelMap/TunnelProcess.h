//----------------------------------------------------------------
// Class: TunnelProcess
// Description: This is the main class that runs the ssh process and
// connects it to the GUI using signals/slots.
// ----------------------------------------------------------------

#ifndef __TUNNELPROCESS_H
#define __TUNNELPROCESS_H

#include <QProcess>
#include <QIcon>
#include <QAction>
#include <QTime>

class TunnelProcess : public QProcess
{
  Q_OBJECT

 public:
  TunnelProcess(QObject* parent=0);
  TunnelProcess(const TunnelProcess &);
  ~TunnelProcess();

  const TunnelProcess & operator=(const TunnelProcess &);

  QString get_name() const {return name_;}
  QString get_ssh_cmd() const {return ssh_cmd_;}
  QString get_ssh_args() const {return ssh_args_;}
  QString get_args() const {return args_;}
  bool is_auto_connect() const {return auto_connect_;}
  bool is_dependent() const {return dependent_;}

  void set_name(QString s) {name_ = s;}
  void set_ssh_cmd(QString s) {ssh_cmd_ = s;}
  void set_ssh_args(QString s) {ssh_args_ = s;}
  void set_args(QString s) {args_ = s;}
  void set_auto_connect(bool b) {auto_connect_ = b; if(b) open();}
  void set_dependent(bool b) {dependent_ = b;}

  QString get_cmd() const 
  {return (ssh_cmd_ + QString(" ") + ssh_args_ + QString(" ") + args_);}

  QAction* get_action() const {action_->setText(name_); return action_;}

 signals:
  void connected(QString, QString);
  void disconnected(QString);

 public slots:
  void open();
  void close();
  void network_up();

 protected slots:
  void when_connected();
  void when_disconnected(int, QProcess::ExitStatus);
  void toggle_state();

 protected:
  void init();

  QString name_;
  QString ssh_cmd_;
  QString ssh_args_;
  QString args_;
  bool auto_connect_;
  bool close_initiated_;
  bool dependent_;

  QTime timer_;
  QAction* action_;
  static QIcon red_icon_;
  static QIcon green_icon_;
};

#endif
