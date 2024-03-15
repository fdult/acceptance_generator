#include "voltage.h"
#include "ui_voltage.h"

Voltage::Voltage(QWidget *parent) : QDialog(parent), ui(new Ui::Voltage)
{
    ui->setupUi(this);

    init();
}

Voltage::~Voltage()
{
    delete ui;
}

void Voltage::init()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Напряжение, кВ"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Частота, кГц"));

    ui->tableWidget->setRowCount(6);

    connect(ui->pushButton,&QPushButton::clicked,this,&Voltage::setVolatge);

    for (int i=1;i<7;i++)
    {
        ui->tableWidget->setItem(i-1,0,new QTableWidgetItem());
        ui->tableWidget->setItem(i-1,1,new QTableWidgetItem());

        ui->tableWidget->item(i-1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i-1,1)->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->item(i-1,1)->setText(QString::number(i*100));
    }

    ui->tableWidget->item(0,0)->setText(QString::number(0));
    ui->tableWidget->item(1,0)->setText(QString::number(5));
    ui->tableWidget->item(2,0)->setText(QString::number(7.6));
    ui->tableWidget->item(3,0)->setText(QString::number(8.6));
    ui->tableWidget->item(4,0)->setText(QString::number(9.2));
    ui->tableWidget->item(5,0)->setText(QString::number(10));

    setVolatge();
}

void Voltage::setVolatge()
{
    V.first.clear();
    V.second.clear();

    double W=ui->lineEdit->text().toDouble();

    for (int i=0;i<ui->tableWidget->rowCount();i++)
    {
        V.second.push_back(ui->tableWidget->item(i,0)->text().toDouble()*W/100);
        V.first.push_back(ui->tableWidget->item(i,1)->text().toDouble());
    }

    emit setVoltageSignal();
}


