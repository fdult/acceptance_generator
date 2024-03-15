#ifndef PARAMETERSACCELERATION_H
#define PARAMETERSACCELERATION_H

#include <QDialog>

namespace Ui
{
    class ParametersAcceleration;
}

class ParametersAcceleration : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersAcceleration(QWidget *parent = nullptr);
    ~ParametersAcceleration();

    QVector <double> parameters;

private:
    Ui::ParametersAcceleration *ui;

    void init();
    void set();

signals:
    void setParameters();
};

#endif // PARAMETERSACCELERATION_H
