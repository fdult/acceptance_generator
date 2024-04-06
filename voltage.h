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

    QPair <QVector <double>,QVector <double>> V_time;

    void replot();

private:
    Ui::Voltage *ui;

    void init();
    void initPlot();
    void setVolatge();

    void changeItem(int row,int col);

private slots:
    // plot
    //=========================================
    void limitAxisRange(QCPAxis *axis,const QCPRange &newRange,const QCPRange &limitRange);
    void xAxisChanged(const QCPRange &newRange);
    void xAxis2Changed(const QCPRange &newRange);
    void yAxisChanged(const QCPRange &newRange);
    //=========================================

    void setRng(const QCPRange &newRange);

signals:
    void setVoltageSignal();
};

#endif // VOLTAGE_H
