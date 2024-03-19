#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <QDialog>
#include <qcustomplot.h>

namespace Ui
{
    class Voltage;
}

class Voltage : public QDialog
{
    Q_OBJECT

public:
    explicit Voltage(QWidget *parent = nullptr);
    ~Voltage();

    QPair <QVector <double>,QVector <double>> V;

    QPair <QVector <double>,QVector <double>> V_interpol;

    void replot();

    void setInterpolation(QPair <QVector <double>,QVector <double>> V);

private:
    Ui::Voltage *ui;

    void init();
    void initPlot();
    void setVolatge();


private slots:
    // plot
    //=========================================
    void limitAxisRange(QCPAxis *axis,const QCPRange &newRange,const QCPRange &limitRange);
    void xAxisChanged(const QCPRange &newRange);
    void yAxisChanged(const QCPRange &newRange);
    //=========================================

signals:
    void setVoltageSignal();
};

#endif // VOLTAGE_H
