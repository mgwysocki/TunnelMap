// This is the main window of the application; it talks to all the sub-widgets,
// and connects the buttons of the program to the actual guts.

#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
class QVBoxLayout;
class QLineEdit;
class TunnelWidget;

class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:
  MainWindow();
  ~MainWindow();

 private slots:
  void about();
  void add_new_tunnel_();
  void connect_all_();
  void disconnect_all_();
  void load_tunnels_();
  void save_tunnels_();
  void save_all_settings();
  void systray_activate_(QSystemTrayIcon::ActivationReason reason);
  void notify_connected(QString, QString);

 private:
  QVBoxLayout* vlayout_;
  QLineEdit* ssh_cmd_edit_;
  QLineEdit* ssh_args_edit_;
  QList<TunnelWidget*> tunnels_;

  void closeEvent(QCloseEvent *);
  void createActions();
  void createMenus();

  QAction* about_act_;
  QAction* about_qt_act_;

  QSystemTrayIcon* systray_icon_;
  QMenu* help_menu_;
};

#endif
