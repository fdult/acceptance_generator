QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

VERSION = 1.0
QMAKE_TARGET_COMPANY = JINR
QMAKE_TARGET_PRODUCT = Acceptance app
QMAKE_TARGET_COPYRIGHT = JINR

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculate.cpp \
    main.cpp \
    mainwindow.cpp \
    parametersacceleration.cpp \
    qcustomplot.cpp \
    voltage.cpp

HEADERS += \
    calculate.h \
    mainwindow.h \
    parametersacceleration.h \
    qcustomplot.h \
    voltage.h

FORMS += \
    mainwindow.ui \
    parametersacceleration.ui \
    voltage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -LC:/Users/fedor/Documents/gsl_2.7/gsl_bin/bin -lgsl

INCLUDEPATH += C:/Users/fedor/Documents/gsl_2.7/gsl_src/include/gsl
DEPENDPATH += C:/Users/fedor/Documents/gsl_2.7/gsl_src/include/gsl

RESOURCES += \
    resource.qrc
