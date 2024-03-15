#ifndef CALCULATE_H
#define CALCULATE_H

#include <QObject>


class Calculate : public QObject
{
    Q_OBJECT
public:
    explicit Calculate(QObject *parent = nullptr);

    QPair <QVector <double>,QVector<double>> setVoltage(QPair <QVector <double>, QVector <double>> Voltage);

};

#endif // CALCULATE_H
