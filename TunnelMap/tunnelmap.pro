######################################################################
# qmake configuration file for datathief project


TEMPLATE = app
TARGET = TunnelMap
# DEPENDPATH += .
# INCLUDEPATH += .

win32 {
  OBJECTS_DIR = build_windows
}
macx {
  OBJECTS_DIR = build_mac
}
#unix {
#  OBJECTS_DIR = build_linux
#}

MOC_DIR = $$OBJECTS_DIR

# Input
HEADERS += TunnelWidget.h \
           MainWindow.h
           
SOURCES += TunnelWidget.cxx \
           MainWindow.cxx \
           main.cxx

RESOURCES += icons.qrc
DEFINES += DEBUG
