#include "parametersacceleration.h"
#include "ui_parametersacceleration.h"

ParametersAcceleration::ParametersAcceleration(QWidget *parent) : QDialog(parent), ui(new Ui::ParametersAcceleration)
{
    ui->setupUi(this);
}

ParametersAcceleration::~ParametersAcceleration()
{
    delete ui;
}

void ParametersAcceleration::init()
{
    connect(ui->pushButton,&QPushButton::clicked,this,&ParametersAcceleration::set);

    set();
}

void ParametersAcceleration::set()
{
    parameters.clear();

    parameters.push_back(ui->lineEdit->text().toDouble());
    parameters.push_back(ui->lineEdit_2->text().toDouble());
    parameters.push_back(ui->lineEdit_3->text().toDouble());
    parameters.push_back(ui->lineEdit_4->text().toDouble());
    parameters.push_back(ui->lineEdit_5->text().toDouble());
    parameters.push_back(ui->lineEdit_6->text().toDouble());
    parameters.push_back(ui->lineEdit_7->text().toDouble());
    parameters.push_back(ui->lineEdit_8->text().toDouble());

    emit setParameters();
}
