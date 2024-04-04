#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <voltage.h>
#include <calculate.h>
#include <parametersacceleration.h>
#include <adiabaticity.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

struct points
{    //               x                y
    QPair <QVector <double>,QVector <double>> V;
    QPair <QVector <double>,QVector <double>> B;
    QPair <QVector <double>,QVector <double>> dB;
    QPair <QVector <double>,QVector <double>> f;
    QPair <QVector <double>,QVector <double>> E;
    QPair <QVector <double>,QVector <double>> Acceptance;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Voltage *voltage;
    Calculate *calculate;
    ParametersAcceleration *parametersAcceleration;
    Adiabaticity *adiabaticity;

    points points;

    void initWindow();
    void initPlot();
    void setCycleParameters();
    void clearPoints();

    void replot();

private slots:
    // plot
    //=========================================
    void limitAxisRange(QCPAxis *axis,const QCPRange &newRange,const QCPRange &limitRange);
    void xAxisChanged(const QCPRange &newRange);
    void yAxisChanged(const QCPRange &newRange);
    //=========================================

};
#endif // MAINWINDOW_H
