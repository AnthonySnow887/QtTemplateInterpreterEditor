QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTemplateInterpreterEditor
TEMPLATE = app

include(QtTemplateInterpreter/QtTemplateInterpreter.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Build dirs
OBJECTS_DIR = build
MOC_DIR     = build
UI_DIR      = build
RCC_DIR     = build

# Build flags
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14 \
                  -fopenmp \
                  -Wall \
                  -Wextra \
                  -Wcast-align \
                  -Wcast-qual \
                  -Wlogical-op \
                  -Woverloaded-virtual

CONFIG(debug, debug|release): QMAKE_CXXFLAGS += -O0
CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O3

#win32 {
#    RC_FILE += LaurusPostProcessingTool.rc
#    OTHER_FILES += LaurusPostProcessingTool.rc
#}

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    widgets/CodeEditor.cpp \
    widgets/LineNumberArea.cpp \
    widgets/SyntaxHighlighter.cpp

HEADERS += \
    MainWindow.h \
    QtTemplateInterpreterEditorVersion.h \
    widgets/CodeEditor.h \
    widgets/LineNumberArea.h \
    widgets/SyntaxHighlighter.h

FORMS += \
    MainWindow.ui

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Sourses.qrc
