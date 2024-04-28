#ifndef USERPARAMETERS_H
#define USERPARAMETERS_H

#include <QDialog>

namespace Ui
{
    class UserParameters;
}

class UserParameters : public QDialog
{
    Q_OBJECT

public:
    explicit UserParameters(QWidget *parent = nullptr);
    ~UserParameters();

    QVector <double> parameters;

    void setParameter(int index,double value);
    void setLabelAcceptance(double value);

private:
    Ui::UserParameters *ui;


    void set();
    void changeParameter();

signals:
    void change(int index);
    void setUserParameters();
};

#endif // USERPARAMETERS_H
