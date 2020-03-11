#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T14:35:10
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = TB12Ctrl
TARGET = TBAccess
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += __WINDOWS_MM__=1
LIBS += -lwinmm

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        dialog.cpp \
        qbank2widget.cpp \
        qbtnsetupwidget.cpp \
        qbuttonswidget.cpp \
        qcccwidget.cpp \
        qccmwidget.cpp \
        qcctwidget.cpp \
        qcmnsetupwidget.cpp \
        qhomewidget.cpp \
        qindicator.cpp \
        qmidi/qmidiin.cpp \
        qmidi/qmidiout.cpp \
        qmidi/qmidisysexmsgs.cpp \
        qmidi/rtmidi-4.0.0/RtMidi.cpp \
        qnotewidget.cpp \
        qnrpncwidget.cpp \
        qnrpntwidget.cpp \
        qpresetchangewidget.cpp \
        qsyssetupwidget.cpp \
        qtbbutton.cpp

HEADERS += \
        dialog.h \
        qbank2widget.h \
        qbtnsetupwidget.h \
        qbuttonswidget.h \
        qcccwidget.h \
        qccmwidget.h \
        qcctwidget.h \
        qcmnsetupwidget.h \
        qhomewidget.h \
        qindicator.h \
        qmidi/qmidiin.h \
        qmidi/qmidiout.h \
        qmidi/qmidisysexmsgs.h \
        qmidi/rtmidi-4.0.0/RtMidi.h \
        qnotewidget.h \
        qnrpncwidget.h \
        qnrpntwidget.h \
        qpresetchangewidget.h \
        qsyssetupwidget.h \
        qtbbutton.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    tb12ctrl.qrc

RC_ICONS = images/logo.ico

