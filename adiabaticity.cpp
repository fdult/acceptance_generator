#include "adiabaticity.h"
#include "ui_adiabaticity.h"

Adiabaticity::Adiabaticity(QWidget *parent) : QDialog(parent), ui(new Ui::Adiabaticity)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&Adiabaticity::set);

    set();
}

Adiabaticity::~Adiabaticity()
{
    delete ui;
}

void Adiabaticity::set()
{
    parameters.clear();

    parameters.push_back(ui->lineEdit->text().toDouble());
    parameters.push_back(ui->lineEdit_2->text().toDouble());
    parameters.push_back(ui->lineEdit_3->text().toDouble());
    parameters.push_back(ui->lineEdit_4->text().toDouble());

    emit setAdiabaticityParameters();
}
