#include "userparameters.h"
#include "ui_userparameters.h"

UserParameters::UserParameters(QWidget *parent) : QDialog(parent), ui(new Ui::UserParameters)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&UserParameters::set);

    connect(ui->lineEdit,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);
    connect(ui->lineEdit_2,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);

    connect(ui->lineEdit_4,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);
    connect(ui->lineEdit_3,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);

    set();

}

UserParameters::~UserParameters()
{
    delete ui;
}

void UserParameters::set()
{
    parameters.clear();

    parameters.push_back(ui->lineEdit->text().toDouble()); // разброс по импульсу
    parameters.push_back(ui->lineEdit_2->text().toDouble()); // разброс по энергии
    parameters.push_back(ui->lineEdit_4->text().toDouble()); // поле инжекции
    parameters.push_back(ui->lineEdit_3->text().toDouble()); // энергия инжекции
    parameters.push_back(ui->lineEdit_5->text().toDouble()); // напряжение инжекции
}

void UserParameters::changeParameter()
{
    if (QObject::sender()==ui->lineEdit) // разброс по импульсу
        emit change(0);

    if (QObject::sender()==ui->lineEdit_2) // разброс по энергии
        emit change(1);

    if (QObject::sender()==ui->lineEdit_4) // поле инжекции
        emit change(2);

    if (QObject::sender()==ui->lineEdit_3) // энергия инжекции
        emit change(3);
}
