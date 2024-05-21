#ifndef FILEOUT_H
#define FILEOUT_H

#include <QFile>
#include <QDataStream>

class FileOut
{
public:
    FileOut();

    void writeToFile(QPair <QVector <double>,QVector <double>> Voltage);
};

#endif // FILEOUT_H
