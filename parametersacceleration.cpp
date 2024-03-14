#include "parametersacceleration.h"
#include "ui_parametersacceleration.h"

parametersAcceleration::parametersAcceleration(QWidget *parent) : QDialog(parent), ui(new Ui::parametersAcceleration)
{
    ui->setupUi(this);
}

parametersAcceleration::~parametersAcceleration()
{
    delete ui;
}
