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
    adiabaticity.cpp \
    calculate.cpp \
    main.cpp \
    mainwindow.cpp \
    parametersacceleration.cpp \
    qcustomplot.cpp \
    userparameters.cpp \
    voltage.cpp

HEADERS += \
    adiabaticity.h \
    calculate.h \
    mainwindow.h \
    parametersacceleration.h \
    qcustomplot.h \
    userparameters.h \
    voltage.h

FORMS += \
    adiabaticity.ui \
    mainwindow.ui \
    parametersacceleration.ui \
    userparameters.ui \
    voltage.ui

RC_ICONS = icons/icon_app.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


win32: LIBS += -L$$PWD/../../gsl_2.7/gsl_bin/lib/ -lgsl

INCLUDEPATH += $$PWD/../../gsl_2.7/gsl_bin/include/gsl
DEPENDPATH += $$PWD/../../gsl_2.7/gsl_bin/include/gsl
