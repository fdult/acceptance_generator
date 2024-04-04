#ifndef ADIABATICITY_H
#define ADIABATICITY_H

#include <QDialog>

namespace Ui { class Adiabaticity; }

class Adiabaticity : public QDialog
{
    Q_OBJECT

public:
    explicit Adiabaticity(QWidget *parent = nullptr);
    ~Adiabaticity();

    QVector <double> p;
    void set();

private:
    Ui::Adiabaticity *ui;

signals:
    void setAdiabaticityParameters();
};

#endif // ADIABATICITY_H
