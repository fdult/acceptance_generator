#include "calculate.h"
#include "qdebug.h"
#include <cmath>

Calculate::Calculate(QObject *parent) : QObject{parent}
{



}

QPair <QVector <double>,QVector <double>> Calculate::setVoltage(QPair <QVector <double>,QVector <double>> Voltage)
{
    /*
    int N=Voltage.first.size();
    if (N)
    {
        double tempVoltageX[N];
        double tempVoltageY[N];

        for (int i=0;i<N;i++)
        {
            tempVoltageX[i]=Voltage.first[i];
            tempVoltageY[i]=Voltage.second[i];
            // qDebug()<<tempVoltageX[i]<<tempVoltageY[i];
        }

        Voltage.first.clear();
        Voltage.second.clear();

        int diskret=1; // 1 = 1 кГц
        for (int i=0;i<diskret*10e3;i++)
        {
            Voltage.first.push_back((double)i/diskret);

            if (i>=0 && i<tempVoltageX[0]*diskret)
            {   // +
                double dx=0,dy=0;

                dx=tempVoltageX[0]*diskret;
                dy=tempVoltageY[0];

                Voltage.second.push_back(dy*Voltage.first[i]/dx);
            }

            if (i>=tempVoltageX[0]*diskret && i<tempVoltageX[1]*diskret)
            {   // +
                double dx=0,xa=0,dy=0,ya=0;

                dx=(tempVoltageX[1]-tempVoltageX[0])*diskret;
                xa=tempVoltageX[0]*diskret;
                dy=tempVoltageY[1]-tempVoltageY[0];
                ya=tempVoltageY[0];

                Voltage.second.push_back(ya+dy*(i-xa)/dx);
            }

            if (i>=tempVoltageX[1]*diskret && i<tempVoltageX[2]*diskret)
            {   // +
                double dx=0,xa=0,dy=0,ya=0;

                dx=(tempVoltageX[2]-tempVoltageX[1])*diskret;
                xa=tempVoltageX[1]*diskret;
                dy=tempVoltageY[2]-tempVoltageY[1];
                ya=tempVoltageY[1];

                Voltage.second.push_back(ya+dy*(i-xa)/dx);
            }

            if (i>=tempVoltageX[2]*diskret && i<tempVoltageX[3]*diskret)
            {
                double dx=0,xa=0,dy=0,ya=0;

                dx=(tempVoltageX[3]-tempVoltageX[2])*diskret;
                xa=tempVoltageX[2]*diskret;
                dy=tempVoltageY[3]-tempVoltageY[2];
                ya=tempVoltageY[2];

                Voltage.second.push_back(ya+dy*(i-xa)/dx);
            }

            if (i>=tempVoltageX[3]*diskret && i<tempVoltageX[4]*diskret)
            {
                double dx=0,xa=0,dy=0,ya=0;

                dx=(tempVoltageX[4]-tempVoltageX[3])*diskret;
                xa=tempVoltageX[3]*diskret;
                dy=tempVoltageY[4]-tempVoltageY[3];
                ya=tempVoltageY[3];

                Voltage.second.push_back(ya+dy*(i-xa)/dx);
            }

            if (i>=tempVoltageX[4]*diskret && i<tempVoltageX[5]*diskret)
            {
                double dx=0,xa=0,dy=0,ya=0;

                dx=(tempVoltageX[5]-tempVoltageX[4])*diskret;
                xa=tempVoltageX[4]*diskret;
                dy=tempVoltageY[5]-tempVoltageY[4];
                ya=tempVoltageY[4];

                Voltage.second.push_back(ya+dy*(i-xa)/dx);
            }

            if (i>=tempVoltageX[5]*diskret && i<10e3*diskret)
            {
                Voltage.second.push_back(tempVoltageY[5]);
            }
        }
        return Voltage;
    }
    throw QString("Voltage calculate error");
    */

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
    y[7]=10;

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

double Calculate::B_inj(QVector<double> p)
{
    return (p[2]/p[3])*std::sqrt((2*p[6]*p[4]+p[6])/(p[0]*_CLight*p[0]*_CLight));
}

double Calculate::B_0(QVector<double> p)
{
    return (p[2]*p[4])/(p[3]*p[0]*_CLight);
}

double Calculate::f_inj(QVector<double> p)
{
    return (p[5]*_CLight*B_inj(p))/(p[1]*std::sqrt(B_inj(p)*B_inj(p)+B_0(p)*B_0(p)));
}

