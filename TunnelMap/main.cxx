
#include <QApplication>
#include <QSettings>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);

  QCoreApplication::setOrganizationName("WysockiSpace");
  QCoreApplication::setApplicationName("Tunnel Watcher");
  QSettings::setDefaultFormat(QSettings::IniFormat);

  MainWindow main_window;
  return app.exec();
}
