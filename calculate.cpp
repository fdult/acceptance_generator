#include "calculate.h"
#include "qdebug.h"

Calculate::Calculate(QObject *parent) : QObject{parent}
{



}

QPair <QVector <double>,QVector <double>> Calculate::setVoltage(QPair <QVector <double>,QVector <double>> Voltage)
{
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

        double W=tempVoltageY[2]/7.6;

        qDebug()<<Voltage;

        Voltage.first.clear();
        Voltage.second.clear();

        int diskret=10;
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
                Voltage.second.push_back(10*W);
            }
        }
        return Voltage;
    }
    throw QString("Voltage calculate error");
}

