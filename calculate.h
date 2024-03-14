#ifndef CALCULATE_H
#define CALCULATE_H

#include <QObject>

class calculate : public QObject
{
    Q_OBJECT
public:
    explicit calculate(QObject *parent = nullptr);

signals:
};

#endif // CALCULATE_H
