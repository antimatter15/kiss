include(../../config.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(c_plugin)
DESTDIR = ../
INCLUDEPATH += ../../editor/src

HEADERS += CLexerSpecProvider.h
SOURCES += CLexerSpecProvider.cpp

#############################
# OS X Specific dylib stuff #
#############################

macx:{
install_lib_names.extra = sh ../../scripts/osx_install_names.sh ../../$${INSTALL_BASE}/lexers/libc_plugin.dylib QtCore.framework/Versions/4/QtCore QtGui.framework/Versions/4/QtGui
install_lib_names.path = ../../$${INSTALL_BASE}

INSTALLS += install_lib_names
}