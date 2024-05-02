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
    }
    else return;
}
