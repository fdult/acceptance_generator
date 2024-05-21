QT += core gui printsupport

greaterThan (QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS_CXX11    = -std=c++0x

# MSVC2019 Qt 5.15.2 и 6.4.3

SOURCES += \
    adiabaticity.cpp \
    calculate.cpp \
    fileout.cpp \
    main.cpp \
    mainwindow.cpp \
    parametersacceleration.cpp \
    qcustomplot.cpp \
    userparameters.cpp \
    voltage.cpp

HEADERS += \
    adiabaticity.h \
    calculate.h \
    fileout.h \
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

RESOURCES += \
    resource.qrc

win32 {

        INCLUDEPATH += $$(GSL_QT)/gsl_bin/include
        DEPENDPATH += $$(GSL_QT)/gsl_bin/include

        LIBS += -L$$PWD/../../gsl_2.7/gsl_bin/lib/ -lgsl

        INCLUDEPATH += $$(TANGO_ROOT_QT)/win64/include/vc16
        DEPENDPATH += $$(TANGO_ROOT_QT)/win64/include/vc16

        INCLUDEPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/omniORB4
        DEPENDPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/omniORB4

        INCLUDEPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/COS
        DEPENDPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/COS

        INCLUDEPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/omnithread
        DEPENDPATH += $$(TANGO_ROOT_QT)/win64/include/vc16/omnithread

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -ltango
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -ltangod

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lzmq
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lzmqd

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomniORB4_rt
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomniORB4_rtd

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomniDynamic4_rt
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomniDynamic4_rtd

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomnithread_rt
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lomnithread_rtd

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lCOS4_rt
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -lCOS4_rtd

        CONFIG(release, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -llog4tango
        else:CONFIG(debug, debug|release): LIBS += -L$$(TANGO_ROOT_QT)/win64/lib/vc16_dll/ -llog4tangod

        # LIBS += -lws2_32 -lIphlpapi

}

