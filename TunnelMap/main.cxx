
#include <QApplication>
#include <QSettings>

#include "ProcessList.h"
#include "MainWindow.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);

  QCoreApplication::setOrganizationName("WysockiSpace");
  QCoreApplication::setApplicationName("Tunnel Watcher");
  QSettings::setDefaultFormat(QSettings::IniFormat);

  ProcessList proc_list;
  proc_list.load();

  MainWindow main_window(&proc_list);
  return app.exec();
}
