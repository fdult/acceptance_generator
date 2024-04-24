#ifndef CALCULATE_H
#define CALCULATE_H

#define _CLight 299792458

#include <QtCore>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>


class Calculate
{

public:
    Calculate();

    QVector <double> Energy,BField,dBField,Voltage,phase,time,temp;

    QPair <QVector <double>,QVector<double>> setVoltageFrequency(QPair <QVector <double>, QVector <double>> Voltage);

    void setFullPeriod(QPair <QVector <double>,QVector <double>> &VoltageFrequency,
                                                           QVector <double> &p,QVector <double> &p2);

    double B_inj(QVector <double> &p);
    double B_0(QVector <double> &p);
    double f_inj(QVector <double> &p);
    double E_total(QVector <double> &p);
    double freqrequency(QVector <double> &p);

    // метод Ньютона-Рафсона
    double derivative(double x, double V, double gamma, double eta, QVector <double> &p);
    double findNextX(double x, double V, double gamma, double eta, double y, QVector <double> &p);
    double function(double sin_phi, double V, double gamma,double eta, QVector <double> &p);
    ///////////////////////////////////

};

#endif // CALCULATE_H
