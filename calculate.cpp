#include "calculate.h"
#include "qdebug.h"
#include <cmath>

Calculate::Calculate()
{

}

QPair <QVector <double>,QVector <double>> Calculate::setVoltageFrequency(QPair <QVector <double>,QVector <double>> Voltage)
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

    for (double i=0;i<=100;i+=1./12.5) // если ставить на каждый кГц, то должно быть x_max/1000 (где 1000 это значение до которого идёт цикл * 10),
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

QPair <QVector <double>,QVector <double>> Calculate::setVoltageTime(QPair <QVector <double>,QVector <double>> &VoltageFrequency,
                                                                  QVector <double> &p,QVector <double> &p2)
{
    QPair <QVector <double>,QVector <double>> Voltage;

    int max_time=30,diskret=1e3;

    double t_ad=0,Z=0,A_m=0,E_m=0,E=0,P=0,r=0,freq=0,v=0,t=0,sin_phi=0,acc=0,V_rf=0,V_inj=0,gamma=0,eta=0;

    double dB=0,B=0;


    t_ad=p2[0];
    acc=p2[1];
    V_inj=p2[2]/1e3;

    Z=p[3];
    A_m=p[2];
    E=p[6];
    E_m=p[4];
    P=p[1];
    r=p[0];

    Voltage.second.push_back(V_inj);

    for (int i=0;i<diskret*max_time;i++)
    {
        if (t<t_ad)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);
            t+=100*P/v;

            gamma=(A_m*(E_m+E))/(A_m*E_m);
            eta=(1./100)-(1./pow(gamma,2));
            sin_phi=findNextX(sin_phi,Voltage.second[i]*1e3,gamma,eta,acc,p);
            E+=(Z/A_m)*100*Voltage.second[i]*1e3*sin_phi;

            V_rf=V_inj+((VoltageFrequency.second[f_inj(p)/1e3]-V_inj)/2) * (1-cos(M_PI*t/t_ad));
            Voltage.second.push_back(V_rf);

            dB=V_rf*1e3*sin_phi/(P*r);

            B=(A_m/Z)*sqrt((2*E_m*E+E*E)/pow(r*_CLight,2));

            qDebug()<<dB<<t;
        }
        if (t>=t_ad && dB<1)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);
            t+=100*P/v;

            freq=v/P;

            gamma=(A_m*(E_m+E))/(A_m*E_m);
            eta=(1./100)-(1./pow(gamma,2));
            sin_phi=findNextX(sin_phi,Voltage.second[i]*1e3,gamma,eta,acc,p);
            E+=(Z/A_m)*100*Voltage.second[i]*1e3*sin_phi;

            V_rf=VoltageFrequency.second[floor(freq/1e3)];
            Voltage.second.push_back(V_rf);

            dB=V_rf*1e3*sin_phi/(P*r);

            B=(A_m/Z)*sqrt((2*E_m*E+E*E)/pow(r*_CLight,2));

            qDebug()<<i<<dB<<t;
        }

        Voltage.first.push_back(t);
    }


    return Voltage;
}

QPair <QVector <double>,QVector <double>> Calculate::setBField(QPair <QVector <double>,QVector <double>> &Energy,QVector <double> &p)
{
    QPair <QVector <double>,QVector <double>> BField;

    int N=Energy.first.size();
    double Btemp=0,Z=0,A_m=0,E_m=0,p_bs=0;

    Z=p[3];
    A_m=p[2];
    E_m=p[4];
    p_bs=p[0];

    for (int i=0;i<N;i++)
    {
        Btemp=(A_m/Z)*sqrt((2*E_m*Energy.second[i]+Energy.second[i])/pow(p_bs*_CLight,2));

        BField.first.push_back(Energy.first[i]);
        BField.second.push_back(Btemp*1e5);
    }

    return BField;
}

QPair <QVector<double>,QVector<double>> Calculate::setEnergy(QPair <QVector <double>,QVector <double>> &Voltage,QVector <double> &p,
                                                             QVector <double> &p2)
{
    QPair <QVector <double>,QVector  <double>> Energy;

    int N=Voltage.first.size();

    double Z=0,A_m=0,E=0,dE=0,sin_phi=0,epsilon=0;

    epsilon=p2[3];
    Z=p[3];
    A_m=p[2];
    E=p[6];

    for (int i=0;i<N;i++)
    {
        Energy.first.push_back(Voltage.first[i]);
        Energy.second.push_back(E/1e9);

        // sin_phi=findNextX(sin_phi,Voltage.second[i]*1e3,E,epsilon,p);

        dE=(Z/A_m)*100*Voltage.second[i]*1e3*sin_phi;
        E+=dE;
    }

    return Energy;
}

double Calculate::B_inj(QVector <double> &p)
{
    return (p[2]/p[3])*sqrt((2*p[6]*p[4]+p[6])/(p[0]*_CLight*p[0]*_CLight));
}

double Calculate::B_0(QVector <double> &p)
{
    return (p[2]*p[4])/(p[3]*p[0]*_CLight);
}

double Calculate::f_inj(QVector <double> &p)
{
    return (p[5]*_CLight*B_inj(p))/(p[1]*sqrt(B_inj(p)*B_inj(p)+B_0(p)*B_0(p)));
}


double Calculate::E_total(QVector <double> &p)
{
    return p[2]*(p[6]+p[4]);
}

double Calculate::freqrequency(QVector <double> &p)
{
    return p[5]*_CLight*B_inj(p)/(p[1]*sqrt(pow((p[2]*p[4])/(p[3]*p[0]*_CLight),2)+B_inj(p)*B_inj(p)));
}

double Calculate::derivative(double x,double V,double gamma, double eta,QVector <double> &p)
{
    double h=1e-9;

    return (function(x+h,V,gamma,eta,p)-function(x-h,V,gamma,eta,p))/(2*h);
}

double Calculate::findNextX(double x,double V,double gamma, double eta,double y,QVector <double> &p)
{
    double eps=1e-9;

    while (abs(function(x,V,gamma,eta,p)-y)>eps)
    {
        x=x-(function(x,V,gamma,eta,p)-y)/derivative(x,V,gamma,eta,p);
    }

    return x;
}

double Calculate::function(double sin_phi, double V, double gamma, double eta, QVector <double> &p)
{
    // return ((1-sin_phi)/pow(1+sin_phi/2,2)) * (8*p[4]*p[1]*sqrt(p[3]*V*(p[4]+E+((double)p[3]/p[2])*100*V*sin_phi)/p[4])) /
    //         (M_PI*_CLight*sqrt(2*M_PI*p[2]*p[4]*p[5]*(1./pow((p[4]+E+((double)p[3]/p[2])*100*1000*sin_phi) / p[4] ,2)-1./100)));

    return ((8*p[4]*p[1])/(M_PI*_CLight)) * ((1-sin_phi)/pow(1+sin_phi/2,2)) * sqrt((p[3]*V*gamma)/(2*M_PI*p[2]*p[5]*fabs(eta)*p[4]));
}

