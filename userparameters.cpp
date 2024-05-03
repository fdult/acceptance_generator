#include "userparameters.h"
#include "ui_userparameters.h"

UserParameters::UserParameters(QWidget *parent) : QDialog(parent), ui(new Ui::UserParameters)
{
    ui->setupUi(this);

    setWindowTitle("Общие параметры");

    connect(ui->pushButton,&QPushButton::clicked,this,&UserParameters::set);
    connect(ui->lineEdit_5,&QLineEdit::editingFinished,this,&UserParameters::set);
    connect(ui->lineEdit_6,&QLineEdit::editingFinished,this,&UserParameters::set);
    connect(ui->lineEdit_7,&QLineEdit::editingFinished,this,&UserParameters::set);

    connect(ui->lineEdit,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);
    connect(ui->lineEdit_2,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);

    connect(ui->lineEdit_3,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);
    connect(ui->lineEdit_4,&QLineEdit::editingFinished,this,&UserParameters::changeParameter);

    set();

}

UserParameters::~UserParameters()
{
    delete ui;
}

void UserParameters::setParameter(int index,double value)
{
    switch (index)
    {
    case 0:
        if (ui->checkBox->isChecked())
            ui->lineEdit->setText(QString::number(value));
        break;
    case 1:
        if (ui->checkBox_2->isChecked())
            ui->lineEdit_2->setText(QString::number(value));
        break;
    case 2:
        if (ui->checkBox_3->isChecked())
            ui->lineEdit_3->setText(QString::number(value));
        break;
    case 3:
        if (ui->checkBox_4->isChecked())
            ui->lineEdit_4->setText(QString::number(value));
        break;
    default:
        break;
    }

    set();
}

void UserParameters::setLabelAcceptance(double value)
{
    ui->label_e->setText("ε = "+QString::number(value));
}

void UserParameters::set()
{
    parameters.clear();

    parameters.push_back(ui->lineEdit->text().toDouble()); // разброс по импульсу 0
    parameters.push_back(ui->lineEdit_2->text().toDouble()); // разброс по энергии 1
    parameters.push_back(ui->lineEdit_3->text().toDouble()); // поле инжекции 2
    parameters.push_back(ui->lineEdit_4->text().toDouble()); // энергия инжекции 3
    parameters.push_back(ui->lineEdit_5->text().toDouble()); // максимальный темп роста 4
    parameters.push_back(ui->lineEdit_6->text().toDouble()); // максимальное поле 5
    parameters.push_back(ui->lineEdit_7->text().toDouble()); // напряжение инжекции 6

    emit setUserParameters();
}

void UserParameters::changeParameter()
{
    QLineEdit *line=qobject_cast <QLineEdit*> (QObject::sender());

    if (line->objectName()=="lineEdit") // разброс по импульсу
    {
        emit change(0);
    }
    if (line->objectName()=="lineEdit_2") // разброс по энергии
    {
        emit change(1);
    }

    if (line->objectName()=="lineEdit_3") // поле инжекции
    {
        emit change(2);
    }
    if (line->objectName()=="lineEdit_4") // энергия инжекции
    {
        emit change(3);
    }
}
