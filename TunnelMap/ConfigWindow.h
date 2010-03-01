//----------------------------------------------------------------
// Class: ConfigWindow 
// Description: This class is where the list of tunnels and the
// configuration are managed.
// ----------------------------------------------------------------

#ifndef __CONFIG_WINDOW_H
#define __CONFIG_WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QModelIndex>
class QVBoxLayout;
class QLineEdit;
class QPlainTextEdit;
class QCheckBox;
class ProcessList;
class SysTrayMenu;
class QListView;
class TunnelProcess;

class ConfigWindow : public QMainWindow
{
 Q_OBJECT

 public:
  ConfigWindow(ProcessList*);
  ~ConfigWindow();

 private slots:
  void add_new_tunnel_();
  void load_info(QModelIndex);
  void save_info();
  void sys_tray_activate_(QSystemTrayIcon::ActivationReason reason);
  void notify_connected(QString, QString);
  void show();

 private:
  QVBoxLayout* vlayout_;
  QLineEdit* ssh_cmd_edit_;
  QLineEdit* ssh_args_edit_;
  QListView* proc_view_;

  QLineEdit* name_edit_;
  QPlainTextEdit* args_edit_;
  QCheckBox* auto_connect_box_;
  QCheckBox* dependent_box_;

  ProcessList* proc_list_;
  TunnelProcess* tp_;

  void closeEvent(QCloseEvent *);

  SysTrayMenu* sys_tray_menu_;
  QSystemTrayIcon* sys_tray_icon_;
};

#endif
