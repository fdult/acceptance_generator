#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <QDialog>

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

private:
    Ui::Voltage *ui;

    void init();
    void setVolatge();

signals:
    void setVoltageSignal();
};

#endif // VOLTAGE_H
