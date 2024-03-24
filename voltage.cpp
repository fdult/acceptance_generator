#include "voltage.h"
#include "ui_voltage.h"

Voltage::Voltage(QWidget *parent) : QDialog(parent), ui(new Ui::Voltage)
{
    ui->setupUi(this);

    init();

    initPlot();

    setVolatge();
}

Voltage::~Voltage()
{
    delete ui;
}

void Voltage::init()
{
    setWindowTitle("Параметры напряжения");

    connect(ui->pushButton,&QPushButton::clicked,this,&Voltage::setVolatge);
    connect(ui->btn_refresh,&QPushButton::clicked,this,[=]()
    {
        for (int i=1;i<=ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->item(i-1,2)->setText(QString::number(100));
        }

        setVolatge();
    });

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

    ui->tableWidget->item(0,0)->setText(QString::number(0));
    ui->tableWidget->item(1,0)->setText(QString::number(5));
    ui->tableWidget->item(2,0)->setText(QString::number(7.6));
    ui->tableWidget->item(3,0)->setText(QString::number(8.6));
    ui->tableWidget->item(4,0)->setText(QString::number(9.2));
    ui->tableWidget->item(5,0)->setText(QString::number(10));

    for (int i=1;i<=ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->item(i-1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i-1,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i-1,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i-1,3)->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->item(i-1,1)->setText(QString::number(i*100));

        ui->tableWidget->item(i-1,2)->setText(QString::number(100));

        ui->tableWidget->item(i-1,3)->setText(QString::number(ui->tableWidget->item(i-1,0)->text().toDouble()
                                                                *ui->tableWidget->item(i-1,2)->text().toDouble()/100));
    }

    connect(ui->tableWidget,&QTableWidget::cellChanged,this,&Voltage::changeItem);
}

void Voltage::initPlot()
{
    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(yAxisChanged(QCPRange)));

    QFont pfont("Newyork",11);
    pfont.setStyleHint(QFont::SansSerif);
    pfont.setPointSize(11);

    ui->plot->xAxis->setLabel("Частота, кГц");
    ui->plot->yAxis->setLabel("Напряжение, кВ");

    ui->plot->xAxis->setLabelFont(pfont);
    ui->plot->yAxis->setLabelFont(pfont);

    ui->plot->xAxis->setTickLabelFont(pfont);
    ui->plot->yAxis->setTickLabelFont(pfont);

    ui->plot->xAxis->setRange(-1.5,1.3e3);
    ui->plot->yAxis->setRange(-0.4,10.4);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::green));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(0)->setName("Напряжение");
}

void Voltage::setVolatge()
{
    V.first.clear();
    V.second.clear();

    V_interpol.first.clear();
    V_interpol.second.clear();

    for (int i=0;i<ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->item(i,3)->setText(QString::number(ui->tableWidget->item(i,0)->text().toDouble()
                                                                 *ui->tableWidget->item(i,2)->text().toDouble()/100));

        V.second.push_back(ui->tableWidget->item(i,0)->text().toDouble()*ui->tableWidget->item(i,2)->text().toDouble()/100);
        V.first.push_back(ui->tableWidget->item(i,1)->text().toDouble());
    }

    qDebug()<<ui->tableWidget->rowCount();

    emit setVoltageSignal();

    replot();
}

void Voltage::changeItem(int row,int col)
{
    if (col==2)
    {
        bool isNum=false;
        double num=ui->tableWidget->item(row,col)->text().toDouble(&isNum);
        if (isNum)
        {
            ui->tableWidget->item(row,col)->setBackground(Qt::white);
            setVolatge();
            qDebug()<<1;
        }
        else
        {
            ui->tableWidget->item(row,col)->setBackground(Qt::red);
        }
    }

}

void Voltage::replot()
{
    ui->plot->xAxis->setRange(-1.5,1.3e3);
    ui->plot->yAxis->setRange(-0.4,10.4);

    ui->plot->graph(0)->setData(V_interpol.first,V_interpol.second);

    ui->plot->replot();
    ui->plot->update();
}

void Voltage::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-1.5,1.3e3);
    limitAxisRange(axis,newRange,limitRange);
}

void Voltage::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.4,10.4);
    limitAxisRange(axis,newRange,limitRange);
}

void Voltage::limitAxisRange(QCPAxis *axis,const QCPRange &newRange,const QCPRange &limitRange)
{
    double lowerBound=limitRange.lower;
    double upperBound=limitRange.upper;
    QCPRange fixedRange(newRange);
    if (fixedRange.lower<lowerBound)
    {
        fixedRange.lower=lowerBound;
        fixedRange.upper=lowerBound+newRange.size();
        if (fixedRange.upper>upperBound || qFuzzyCompare(newRange.size(),upperBound-lowerBound)) fixedRange.upper=upperBound;
        axis->setRange(fixedRange);
    }
    else if (fixedRange.upper>upperBound)
    {
        fixedRange.upper=upperBound;
        fixedRange.lower=upperBound-newRange.size();
        if (fixedRange.lower<lowerBound || qFuzzyCompare(newRange.size(),upperBound-lowerBound)) fixedRange.lower=lowerBound;
        axis->setRange(fixedRange);
    }
}