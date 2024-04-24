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

void Calculate::setFullPeriod(QPair <QVector <double>,QVector <double>> &VoltageFrequency,
                                                                  QVector <double> &p,QVector <double> &p2)
{

    time.clear();
    Voltage.clear();
    Energy.clear();
    BField.clear();
    dBField.clear();
    phase.clear();

    int max_time=30,diskret=1e3;

    double t_ad=0,Z=0,A_m=0,E_m=0,E=0,P=0,r=0,dE=0,freq=0,v=0,t=0,sin_phi=0,acc=0,V_rf=0,V_inj=0,gamma=0,eta=0;

    double dB=0,B=0,dB_MAX=0,B_max=0;

    t_ad=p2[0];
    acc=p2[1];
    V_inj=p2[2]/1e3;

    V_rf=V_inj;

    Z=p[3];
    A_m=p[2];
    E=p[6];
    E_m=p[4];
    P=p[1];
    r=p[0];

    dB_MAX=1;
    B_max=1.6;

    time.push_back(0);
    Voltage.push_back(V_inj);
    Energy.push_back(E/1e9);
    BField.push_back(B);
    dBField.push_back(dB);
    phase.push_back(sin_phi);

    int i=0;
    while (i<max_time*diskret && freq/1e3<1210)
    {
        while (t<t_ad)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);
            t+=100*P/v;

            gamma=(A_m*(E_m+E))/(A_m*E_m);
            eta=(1./100)-(1./pow(gamma,2));
            sin_phi=findNextX(sin_phi,V_rf*1e3,gamma,eta,acc,p);
            E+=(Z/A_m)*100*V_rf*1e3*sin_phi;

            V_rf=V_inj+((VoltageFrequency.second[f_inj(p)/1e3]-V_inj)/2) * (1-cos(M_PI*t/t_ad));

            dB=V_rf*1e3*sin_phi/(P*r);

            B=(A_m/Z)*sqrt((2*E_m*E+E*E)/pow(r*_CLight,2));

            time.push_back(t);
            Voltage.push_back(V_rf*sin_phi/0.3718);
            Energy.push_back(E/1e9);
            BField.push_back(B);
            dBField.push_back(dB);
            phase.push_back(sin_phi);

            temp.push_back(freq/1e6);

            i++;
        }

        while (t>=t_ad && dB<dB_MAX)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);
            t+=100*P/v;

            freq=v/P;

            gamma=(A_m*(E_m+E))/(A_m*E_m);
            eta=(1./100)-(1./pow(gamma,2));
            sin_phi=findNextX(sin_phi,V_rf*1e3,gamma,eta,acc,p);
            E+=(Z/A_m)*100*V_rf*1e3*sin_phi;

            V_rf=VoltageFrequency.second[floor(freq/1e3)];

            dB=V_rf*1e3*sin_phi/(P*r);

            B=(A_m/Z)*sqrt((2*E_m*E+E*E)/pow(r*_CLight,2));

            time.push_back(t);
            Voltage.push_back(V_rf*sin_phi/0.3718);
            Energy.push_back(E/1e9);
            BField.push_back(B);
            dBField.push_back(dB);
            phase.push_back(sin_phi);

            temp.push_back(freq/1e6);
            i++;
        }

        while (B<B_max-0.5*dB_MAX*t_ad)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);

            B+=dB_MAX/diskret;

            dE=E;
            E=sqrt(pow((Z/A_m)*_CLight*B*r,2)+E_m*E_m)-E_m;
            dE=E-dE;

            t+=1./diskret;
            freq=v/P;
            V_rf=VoltageFrequency.second[floor(freq/1e3)];
            sin_phi=dE*A_m/((1./diskret)*freq*Z*V_rf*1e3);
            dB=V_rf*1e3*sin_phi/(r*P);

            time.push_back(t);
            Voltage.push_back(V_rf*sin_phi/0.3718);
            Energy.push_back(E/1e9);
            BField.push_back(B);
            dBField.push_back(dB);
            phase.push_back(sin_phi);

            temp.push_back(freq/1e6);
            i++;

        }

        int temp1=0;
        temp1=i+floor(t_ad*diskret);

        while (i<max_time*diskret && freq/1e3<1210)
        {
            v=_CLight*sqrt(pow(E_m+E,2)-E_m*E_m)/(E_m+E);

            B=B_max-(0.5*dB_MAX*t_ad)*pow((i-temp1+1)/floor(t_ad*diskret),2);
            dE=E;
            E=sqrt(pow((Z/A_m)*_CLight*B*r,2)+E_m*E_m)-E_m;
            dE=E-dE;

            t+=1./diskret;
            freq=v/P;
            V_rf=VoltageFrequency.second[floor(freq/1e3)];
            sin_phi=dE*A_m/((1./diskret)*freq*Z*V_rf*1e3);
            dB=V_rf*1e3*sin_phi/(r*P);

            time.push_back(t);
            Voltage.push_back(V_rf*sin_phi/0.3718);
            Energy.push_back(E/1e9);
            BField.push_back(B);
            dBField.push_back(dB);
            phase.push_back(sin_phi);

            temp.push_back(freq/1e6);
            i++;
        }

        i++;
    }
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
    return ((8*p[4]*p[1])/(M_PI*_CLight)) * ((1-sin_phi)/pow(1+sin_phi/2,2)) * sqrt((p[3]*V*gamma)/(2*M_PI*p[2]*p[5]*fabs(eta)*p[4]));
}

