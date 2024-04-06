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
    explicit Calculate(QObject *parent=nullptr);

    QPair <QVector <double>,QVector<double>> setVoltageFrequency(QPair <QVector <double>, QVector <double>> Voltage);
    QPair <QVector <double>,QVector<double>> setVoltageAdiabaticity(QVector<double> &p, QVector<double> &p2);
    QPair <QVector <double>,QVector<double>> setVoltageTime(QPair <QVector <double>,QVector <double>> &VoltageFrequency,QVector <double> &p,QVector <double> &p2);

    double B_inj(QVector <double> &p);
    double B_0(QVector <double> &p);
    double f_inj(QVector <double> &p);
    double e_inj(QVector <double> &p);
    double Velocity_inj(QVector <double> &p);
    double V_adiabaticity(QVector <double> &p, QVector <double> &p2, int numV);
    double E_total(QVector <double> &p);
    double t_adiabaticity(QVector <double> &p,QVector <double> &p2);
    double freqrequency(QVector <double> &p);

    // для метода Ньютона-Рафсона
    double derivative(double x,double V,double E,QVector <double> &p);
    double setNextX(double x,double V,double E,double y,QVector <double> &p);
    double function(double sin_phi,double V,double E,QVector <double> &p);
    ///////////////////////////////////
};

#endif // CALCULATE_H
