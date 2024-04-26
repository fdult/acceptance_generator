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

private:
    Ui::UserParameters *ui;


    void set();
    void changeParameter();

signals:
    void change(int index);
};

#endif // USERPARAMETERS_H
