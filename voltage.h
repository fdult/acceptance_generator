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

private:
    Ui::Voltage *ui;
};

#endif // VOLTAGE_H
