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

    QVector <double> Energy,BField,dBField,Voltage,phase,freq,time;
    QVector <double> dB_out,B_out,t_out;

    QPair <QVector <double>,QVector<double>> setVoltageFrequency(QPair <QVector <double>, QVector <double>> Voltage);

    void setFullPeriod(QPair <QVector <double>,QVector <double>> &VoltageFrequency,
                                                           QVector <double> &p,QVector <double> &p2,QVector <double> &p3);

    double acceptance(QVector <double> &p,QVector <double> &p2);
    double B_inj(QVector <double> &p,QVector <double> &p2); // поле инжекции
    double E_inj(QVector <double> &p,QVector <double> &p2); // энергия инжекции
    double dp(QVector <double> &p,QVector <double> &p2); // разброс по импульсу
    double dE(QVector <double> &p,QVector <double> &p2); // разброс по энергии
    double B_0(QVector <double> &p); // коэффициент
    double f_inj(QVector <double> &p,QVector <double> &p2); // частота инжекции
    double E_total(QVector <double> &p,QVector <double> &p2); // полная энергия
    double freqrequency(QVector <double> &p,QVector <double> &p2); //

    // метод Ньютона-Рафсона
    double derivative(double x, double V, double gamma, double eta, QVector <double> &p);
    double findNextX(double x, double V, double gamma, double eta, double y, QVector <double> &p);
    double function(double sin_phi, double V, double gamma,double eta, QVector <double> &p);
    ///////////////////////////////////

};

#endif // CALCULATE_H
