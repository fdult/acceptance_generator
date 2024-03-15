#include "voltageplot.h"
#include "ui_voltageplot.h"

VoltagePlot::VoltagePlot(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VoltagePlot)
{
    ui->setupUi(this);
}

VoltagePlot::~VoltagePlot()
{
    delete ui;
}
