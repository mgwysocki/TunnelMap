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
HEADERS += TunnelProcess.h \
           ProcessList.h \
           SysTrayMenu.h \
           EditDialog.h \
           ConfigWindow.h
           
SOURCES += TunnelProcess.cxx \
           ProcessList.cxx \
           SysTrayMenu.cxx \
           EditDialog.cxx \
           ConfigWindow.cxx \
           main.cxx

RESOURCES += icons.qrc
DEFINES += DEBUG
