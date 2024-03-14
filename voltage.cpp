#include "voltage.h"
#include "ui_voltage.h"

Voltage::Voltage(QWidget *parent) : QDialog(parent), ui(new Ui::Voltage)
{
    ui->setupUi(this);
}

Voltage::~Voltage()
{
    delete ui;
}
