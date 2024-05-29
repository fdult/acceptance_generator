#include "fileout.h"

FileOut::FileOut()
{


}

void FileOut::writeToFile(QPair <QVector <double>,QVector <double>> Voltage)
{
    QByteArray arr;
    QDataStream stream(&arr,QIODevice::WriteOnly);

    int N=Voltage.first.size();

    for (int i=0;i<N;i++)
    {
        stream<<Voltage.first[i];
        stream<<Voltage.second[i];
    }

    QFile file("Data.txt");
    // QDataStream out(&file);
    file.resize(0);

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(arr);
        file.close();
    }

    else return;
}

void FileOut::out_V(QVector <double> &time, QVector <double> &V)
{
    QByteArray arr;
    QTextStream stream(&arr,QIODevice::WriteOnly);

    int N=time.size();

    for (int i=0;i<N;i++)
    {
        stream<<QString::number(time[i])<<" "<<QString::number(V[i])<<"\n";
    }
    QFile file_V("V_rf.txt");

    if (file_V.open(QIODevice::WriteOnly))
    {
        file_V.write(arr);
        file_V.close();
    }
}

void FileOut::out_dBField(QVector <double> &time, QVector <double> &dB)
{
    QByteArray arr;
    QTextStream stream(&arr,QIODevice::WriteOnly);

    int N=time.size();

    for (int i=0;i<N;i++)
    {
        stream<<QString::number(time[i])<<" "<<QString::number(dB[i])<<"\n";
    }
    QFile file_V("dBField.txt");

    if (file_V.open(QIODevice::WriteOnly))
    {
        file_V.write(arr);
        file_V.close();
    }
}

void FileOut::out_BField(QVector <double> &time, QVector <double> &B)
{
    QByteArray arr;
    QTextStream stream(&arr,QIODevice::WriteOnly);

    int N=time.size();

    for (int i=0;i<N;i++)
    {
        stream<<QString::number(time[i])<<" "<<QString::number(B[i])<<"\n";
    }
    QFile file_V("B_Field.txt");

    if (file_V.open(QIODevice::WriteOnly))
    {
        file_V.write(arr);
        file_V.close();
    }
}
