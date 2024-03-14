#ifndef PARAMETERSACCELERATION_H
#define PARAMETERSACCELERATION_H

#include <QDialog>

namespace Ui
{
    class parametersAcceleration;
}

class parametersAcceleration : public QDialog
{
    Q_OBJECT

public:
    explicit parametersAcceleration(QWidget *parent = nullptr);
    ~parametersAcceleration();

private:
    Ui::parametersAcceleration *ui;
};

#endif // PARAMETERSACCELERATION_H
