#include "calculate.h"
#include "qdebug.h"
#include <cmath>

Calculate::Calculate(QObject *parent) : QObject{parent}
{



}

QPair <QVector <double>,QVector <double>> Calculate::setVoltageFrequency(QPair <QVector<double>,QVector<double>> Voltage)
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

QPair <QVector <double>,QVector <double>> Calculate::setVoltageAdiabaticity(QVector <double> &p,QVector <double> &p2)
{
    QPair <QVector <double>,QVector <double>> voltageTime;
    double V_0=0,V_1=0,t_ad=0;

    V_0=V_adiabaticity(p,p2,0);
    V_1=V_adiabaticity(p,p2,1);
    t_ad=t_adiabaticity(p,p2);

    int diskret=1e4;

    for (int i=0;i<diskret*t_ad;i++)
    {
        double v_temp=0;
        voltageTime.first.push_back((double)i/diskret);
        v_temp=V_0/pow(1-(1-sqrt(V_0/V_1))*voltageTime.first[i]/t_ad,2);
        voltageTime.second.push_back(v_temp/1000);
    }
    return voltageTime;
}

QPair <QVector <double>,QVector <double>> Calculate::setVoltageTime(QPair <QVector <double>,QVector <double>> &VoltageFrequency,
                                                                  QVector <double> &p,QVector <double> &p2)
{
    QPair <QVector <double>,QVector <double>> Voltage;
    QPair <QVector <double>,QVector <double>> tempVoltage;
    QPair <QVector <double>,QVector <double>> VoltageAdiabaticity;

    int max_time=30,diskret=1e3;

    double t_ad=0,Z=0,A_m=0,E_m=0,E=0,P=0,dE=0,freq=0,v=0,t=0,sin_phi=0,epsilon=0;

    double helpFreq=0;

    t_ad=t_adiabaticity(p,p2);
    VoltageAdiabaticity=setVoltageAdiabaticity(p,p2);
    epsilon=p2[3];

    Z=p[3];
    A_m=p[2];
    E=p[6];
    E_m=p[4];
    P=p[1];

/////////////////
    for (int i=0;i<diskret*max_time;i++)
    {
        v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);
        t+=100*P/v;
        freq=v/P;

        Voltage.first.push_back(t);

        if (t<t_ad)
        {
            if (VoltageAdiabaticity.second[floor(t*1e4)]<VoltageFrequency.second[floor(freq/1e3)])
            {
                Voltage.second.push_back(VoltageAdiabaticity.second[floor(t*1e4)]);
            }
            else
            {
                Voltage.second.push_back(VoltageFrequency.second[freq/1e3]);
            }
        }
        else
        {
            if (freq/1e3<1250)
            {
                Voltage.second.push_back(VoltageFrequency.second[freq/1e3]);
                helpFreq=VoltageFrequency.second[freq/1e3];
            }
            else if (t<diskret*max_time)
            {
                Voltage.second.push_back(helpFreq);
            }
        }

        sin_phi=setNextX(sin_phi,Voltage.second[i]*1e3,E,epsilon,p);

        dE=(Z/A_m)*100*Voltage.second[i]*1e3*sin_phi;
        E+=dE;
    }

    double temp=0;
    temp=t;

    while (temp<10)
    {
        temp+=1./1e3;
        Voltage.second.push_back(helpFreq);
        Voltage.first.push_back(temp);
    }

/////////////////////

    VoltageAdiabaticity.first.clear();
    VoltageAdiabaticity.second.clear();

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

        sin_phi=setNextX(sin_phi,Voltage.second[i]*1e3,E,epsilon,p);

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

double Calculate::e_inj(QVector <double> &p)
{
    double beta=0;
    double dp=0;
    beta=Velocity_inj(p)/_CLight;
    dp=p[7]/(beta*beta*(p[2]*(p[4]+p[6])));

    return 2*p[1]*dp*sqrt(2*p[6]*p[4]+p[6]*p[6])/_CLight;
}

double Calculate::Velocity_inj(QVector <double> &p)
{
    return _CLight*sqrt(2*p[4]*p[6]+p[6]*p[6])/(p[4]+p[6]);
}

double Calculate::V_adiabaticity(QVector <double> &p,QVector <double> &p2,int numV)
{
    double gamma=0,eta=0,beta=0,dE=0;

    gamma=(p[4]+p[6])/p[4];
    eta=1./(gamma*gamma)-1./100;
    dE=p[7];
    beta=Velocity_inj(p)/_CLight;

    if (numV==0)
        return (M_PI*p[5]*eta*p2[0]*dE*dE)/(2*p[3]*beta*beta*E_total(p));
    else if (numV==1)
        return (M_PI*M_PI*M_PI*p[5]*eta*p2[1]*p2[1]*dE*dE)/(8*p[3]*beta*beta*E_total(p));
    else return 0;
}

double Calculate::E_total(QVector <double> &p)
{
    return p[2]*(p[6]+p[4]);
}

double Calculate::t_adiabaticity(QVector <double> &p,QVector <double> &p2)
{
    double omega=0,gamma=0,eta=0,beta=0;

    gamma=(p[4]+p[6])/p[4];
    eta=1./(gamma*gamma)-1./100;
    beta=Velocity_inj(p)/_CLight;

    omega=2*M_PI*freqrequency(p)*sqrt((p[5]*eta*p[3]*V_adiabaticity(p,p2,0)/(2*M_PI*beta*beta*E_total(p))));

    return p2[2]*(1-sqrt(V_adiabaticity(p,p2,0)/V_adiabaticity(p,p2,1)))/omega;
}

double Calculate::freqrequency(QVector <double> &p)
{
    return p[5]*_CLight*B_inj(p)/(p[1]*sqrt(pow((p[2]*p[4])/(p[3]*p[0]*_CLight),2)+B_inj(p)*B_inj(p)));
}

double Calculate::derivative(double x,double V,double E,QVector <double> &p)
{
    double h=1e-6;

    return (function(x+h,V,E,p)-function(x-h,V,E,p))/(2*h);
}

double Calculate::setNextX(double x,double V,double E,double y,QVector <double> &p)
{
    double eps=1e-9;

    while (abs(function(x,V,E,p)-y)>eps)
    {
        x=x-(function(x,V,E,p)-y)/derivative(x,V,E,p);
    }

    return x;
}

double Calculate::function(double sin_phi,double V,double E,QVector <double> &p)
{
    return ((1-sin_phi)/pow(1+sin_phi/2,2)) * (8*p[4]*p[1]*sqrt(p[3]*V*(p[4]+E+((double)p[3]/p[2])*100*V*sin_phi)/p[4])) /
            (M_PI*_CLight*sqrt(2*M_PI*p[2]*p[4]*p[5]*(1./pow((p[4]+E+((double)p[3]/p[2])*100*1000*sin_phi) / p[4] ,2)-1./100)));
}

