#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QString str="platform windows:darkmode=1";
    QCommandLineOption mode("platform windows:darkmode","Sets the dark mode for platform windows","num","0");
    QCommandLineParser parser;
    parser.addOption(mode);
    parser.process(a);

    MainWindow w;
    w.show();
    return a.exec();
}
