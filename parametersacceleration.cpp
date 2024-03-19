#include "parametersacceleration.h"
#include "ui_parametersacceleration.h"

ParametersAcceleration::ParametersAcceleration(QWidget *parent) : QDialog(parent), ui(new Ui::ParametersAcceleration)
{
    ui->setupUi(this);

    init();

    set();
}

ParametersAcceleration::~ParametersAcceleration()
{
    delete ui;
}

void ParametersAcceleration::init()
{
    setWindowTitle("Параметры ускорителя и пучка");

    connect(ui->pushButton,&QPushButton::clicked,this,&ParametersAcceleration::set);

    set();
}

void ParametersAcceleration::set()
{
    parameters.clear();

    parameters.push_back(ui->lineEdit->text().toDouble()); // радиус поворота 0
    parameters.push_back(ui->lineEdit_2->text().toDouble()); // периметр 1
    parameters.push_back(ui->lineEdit_3->text().toDouble()); // массовое число 2
    parameters.push_back(ui->lineEdit_4->text().toDouble()); // заряд 3
    parameters.push_back(ui->lineEdit_5->text().toDouble()); // масса покоя протона 4
    parameters.push_back(ui->lineEdit_6->text().toDouble()); // кратность 5
    parameters.push_back(ui->lineEdit_7->text().toDouble()); // энергия инжекции 6
    parameters.push_back(ui->lineEdit_8->text().toDouble()); // разброс по импульсу 7

    emit setParameters();
}
