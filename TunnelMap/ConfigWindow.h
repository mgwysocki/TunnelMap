//----------------------------------------------------------------
// Class: ConfigWindow 
// Description: This class is where the list of tunnels and the
// configuration are managed.
// ----------------------------------------------------------------

#ifndef __CONFIG_WINDOW_H
#define __CONFIG_WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
class QVBoxLayout;
class QLineEdit;
class ProcessList;
class SysTrayMenu;
class QListView;

class ConfigWindow : public QMainWindow
{
 Q_OBJECT

 public:
  ConfigWindow(ProcessList*);
  ~ConfigWindow();

 private slots:
  void add_new_tunnel_();
  void sys_tray_activate_(QSystemTrayIcon::ActivationReason reason);
  void notify_connected(QString, QString);
  void show();

 private:
  QVBoxLayout* vlayout_;
  QLineEdit* ssh_cmd_edit_;
  QLineEdit* ssh_args_edit_;
  QListView* proc_view_;

  ProcessList* proc_list_;

  void closeEvent(QCloseEvent *);

  SysTrayMenu* sys_tray_menu_;
  QSystemTrayIcon* sys_tray_icon_;
};

#endif
