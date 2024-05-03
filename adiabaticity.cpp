#include "adiabaticity.h"
#include "ui_adiabaticity.h"

Adiabaticity::Adiabaticity(QWidget *parent) : QDialog(parent), ui(new Ui::Adiabaticity)
{
    ui->setupUi(this);

    setWindowTitle("Параметры адиабатичности");

    connect(ui->pushButton,&QPushButton::clicked,this,&Adiabaticity::set);
    connect(ui->lineEdit,&QLineEdit::editingFinished,this,&Adiabaticity::set);
    connect(ui->lineEdit_2,&QLineEdit::editingFinished,this,&Adiabaticity::set);

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

    emit setAcceptance(4);
    emit setAdiabaticityParameters();
}

void Adiabaticity::changeAcceptance(double value)
{
    if (ui->checkBox->isChecked())
    {
        ui->lineEdit_2->setText(QString::number(value));
        set();
    }
}
