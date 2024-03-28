#ifndef CALCULATE_H
#define CALCULATE_H

#define _CLight 299792458

#include <QObject>
#include <gsl/interpolation/gsl_spline.h>
#include <gsl/interpolation/gsl_interp.h>

class Calculate : public QObject
{
    Q_OBJECT
public:
    explicit Calculate(QObject *parent = nullptr);

    QPair <QVector <double>,QVector<double>> setVoltage(QPair <QVector <double>, QVector <double>> Voltage);

    double B_inj(QVector <double> &p);
    double B_0(QVector <double> &p);
    double f_inj(QVector <double> &p);
    double e_inj(QVector <double> &p);
    double Velocity_inj(QVector <double> &p);

};

#endif // CALCULATE_H
