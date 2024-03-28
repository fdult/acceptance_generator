#include "calculate.h"
#include "qdebug.h"
#include <cmath>

Calculate::Calculate(QObject *parent) : QObject{parent}
{



}

QPair <QVector <double>,QVector <double>> Calculate::setVoltage(QPair <QVector<double>,QVector<double>> Voltage)
{
    int N=Voltage.first.size()+2;
    double xi=0,yi=0;

    double *x=new double[N]{0};
    double *y=new double[N]{0};

    for (int i=0;i<N-2;i++)
    {
        x[i+1]=Voltage.first[i];
        y[i+1]=Voltage.second[i];
    }

    x[7]=1250;
    y[7]=Voltage.second[5];

    Voltage.first.clear();
    Voltage.second.clear();

    gsl_interp_accel *acc=gsl_interp_accel_alloc();
    gsl_spline *spline=gsl_spline_alloc(gsl_interp_steffen,N);
    gsl_spline_init(spline,x,y,N);

    for (double i=0;i<=100;i+=1./12.5) // если ставить на каждый кГц, то должно быть  x_max/1000 (где 1000 это значение до которого идёт цикл * 10),
    // то есть 12500/1000=12.5
    {
        xi=(1-i/100.0)*x[0]+(i/100.0)*x[N-1];
        yi=gsl_spline_eval(spline,xi,acc);

        Voltage.second.push_back(yi);
        Voltage.first.push_back(xi);
    }

    return Voltage;

    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);
}

double Calculate::B_inj(QVector <double> &p)
{
    return (p[2]/p[3])*std::sqrt((2*p[6]*p[4]+p[6])/(p[0]*_CLight*p[0]*_CLight));
}

double Calculate::B_0(QVector <double> &p)
{
    return (p[2]*p[4])/(p[3]*p[0]*_CLight);
}

double Calculate::f_inj(QVector <double> &p)
{
    return (p[5]*_CLight*B_inj(p))/(p[1]*std::sqrt(B_inj(p)*B_inj(p)+B_0(p)*B_0(p)));
}

double Calculate::e_inj(QVector <double> &p)
{
    double beta=0;
    double dp=0;
    beta=Velocity_inj(p)/_CLight;
    dp=p[7]/(beta*beta*(p[4]+p[6]));
    return 2*p[1]*dp*std::sqrt(2*p[6]*p[4]+p[6]*p[6])/_CLight;
}

double Calculate::Velocity_inj(QVector <double> &p)
{
    return _CLight*std::sqrt(2*p[4]*p[6]-p[6]*p[6])/(p[4]+p[6]);
}

