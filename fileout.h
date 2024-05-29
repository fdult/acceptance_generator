#ifndef FILEOUT_H
#define FILEOUT_H

#include <QFile>
#include <QDataStream>
#include <QTextStream>


class FileOut
{
public:
    FileOut();

    void writeToFile(QPair <QVector <double>,QVector <double>> Voltage);

    void out_V(QVector <double> &time,QVector <double> &V);
    void out_dBField(QVector <double> &time,QVector <double> &dB);
    void out_BField(QVector <double> &time,QVector <double> &B);
};

#endif // FILEOUT_H
