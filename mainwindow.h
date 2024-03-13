#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

struct V
{
    QVector <double> x,y;
};

struct Interpolation
{
    QVector <double> x,y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    V V;
    Interpolation Interpolation;

    void initPlot();

    void interpolation();

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
