//----------------------------------------------------------------
// Class: SysTrayMenu
// Description: 
// A simple class to manage the system tray menu, it inherits
// from QMenu.
//----------------------------------------------------------------

#ifndef SYSTRAYMENU_H__
#define SYSTRAYMENU_H__

#include <QMenu.h>
class ProcessList;
class QAction;

class SysTrayMenu : public QMenu
{
Q_OBJECT
 public:
  SysTrayMenu(ProcessList* pl, QWidget* parent = 0);
  QAction* get_config_action() {return config_action_;}

 public slots:
  void refresh();

 protected:
  ProcessList* proc_list_;
  QList<QAction*> tunnel_actions_;

  QAction* config_action_;
  QAction* connect_action_;
  QAction* reconnect_action_;
  QAction* disconnect_action_;
  QAction* quit_action_;
  QAction* sep_;
};

#endif // SYSTRAYMENU_H__
