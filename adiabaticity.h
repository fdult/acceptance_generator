#ifndef ADIABATICITY_H
#define ADIABATICITY_H

#include <QDialog>

namespace Ui
{
    class Adiabaticity;
}

class Adiabaticity : public QDialog
{
    Q_OBJECT

public:
    explicit Adiabaticity(QWidget *parent = nullptr);
    ~Adiabaticity();

    QVector <double> parameters;
    void set();
    void changeAcceptance(double value);

private:
    Ui::Adiabaticity *ui;

signals:
    void setAdiabaticityParameters();
    void setAcceptance(int index);
};

#endif // ADIABATICITY_H
