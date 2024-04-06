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

    /////////////////////////////////////////
    /// Dark mode
    ui->plot->xAxis->setBasePen(QPen(Qt::white,1));
    ui->plot->xAxis2->setBasePen(QPen(Qt::white,1));
    ui->plot->yAxis->setBasePen(QPen(Qt::white,1));

    ui->plot->xAxis->setTickPen(QPen(Qt::white,1));
    ui->plot->xAxis2->setTickPen(QPen(Qt::white,1));
    ui->plot->yAxis->setTickPen(QPen(Qt::white,1));

    ui->plot->xAxis->setSubTickPen(QPen(Qt::white,1));
    ui->plot->xAxis2->setSubTickPen(QPen(Qt::white,1));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::white,1));

    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->xAxis2->setTickLabelColor(Qt::white);
    ui->plot->yAxis->setTickLabelColor(Qt::white);

    ui->plot->xAxis->grid()->setPen(QPen(QColor(140,140,140),1,Qt::DotLine));
    ui->plot->xAxis2->grid()->setPen(QPen(QColor(140,140,140),1,Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140,140,140),1,Qt::DotLine));

    ui->plot->xAxis->grid()->setSubGridPen(QPen(QColor(80,80,80),1,Qt::DotLine));
    ui->plot->xAxis2->grid()->setSubGridPen(QPen(QColor(80,80,80),1,Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(80,80,80),1,Qt::DotLine));

    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis2->grid()->setSubGridVisible(true);
    ui->plot->yAxis->grid()->setSubGridVisible(true);

    ui->plot->xAxis->grid()->setZeroLinePen(QPen(Qt::white,2));
    ui->plot->xAxis2->grid()->setZeroLinePen(QPen(Qt::white,2));
    ui->plot->yAxis->grid()->setZeroLinePen(QPen(Qt::white,2));

    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->xAxis2->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);


    QLinearGradient plotGradient;
    plotGradient.setStart(0,0);
    plotGradient.setFinalStop(0,350);
    plotGradient.setColorAt(0,QColor(80,80,80));
    plotGradient.setColorAt(1, QColor(50,50,50));
    ui->plot->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0,0);
    axisRectGradient.setFinalStop(0,350);
    axisRectGradient.setColorAt(0,QColor(80,80,80));
    axisRectGradient.setColorAt(1,QColor(30,30,30));
    ui->plot->axisRect()->setBackground(axisRectGradient);
    /////////////////////////////////////////


    QFont pfont("Newyork",11);
    pfont.setStyleHint(QFont::SansSerif);
    pfont.setPointSize(11);

    ui->plot->xAxis->setLabel("Частота, кГц");
    ui->plot->xAxis2->setLabel("Время, с");
    ui->plot->yAxis->setLabel("Напряжение, кВ");

    ui->plot->xAxis->setLabelColor(Qt::white);
    ui->plot->xAxis2->setLabelColor(Qt::white);
    ui->plot->yAxis->setLabelColor(Qt::white);

    ui->plot->xAxis->setLabelFont(pfont);
    ui->plot->xAxis2->setLabelFont(pfont);
    ui->plot->yAxis->setLabelFont(pfont);

    ui->plot->xAxis->setTickLabelFont(pfont);
    ui->plot->xAxis2->setTickLabelFont(pfont);
    ui->plot->yAxis->setTickLabelFont(pfont);

    ui->plot->xAxis->setRange(-15,1.3e3);
    ui->plot->xAxis2->setRange(-0.15,13);
    ui->plot->yAxis->setRange(-0.4,10.4);

    ui->plot->xAxis2->setVisible(true);


    // QList <QCPAxis*> draggableAxes={ui->plot->xAxis,ui->plot->yAxis,ui->plot->xAxis2};

    // ui->plot->axisRect()->setRangeDragAxes(draggableAxes);
    // ui->plot->axisRect()->setRangeZoomAxes(draggableAxes);


    // QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    // fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples );
    // fixedTicker->setTickStep(100.0);

    // ui->plot->xAxis->setTicker(fixedTicker);

    QList <QCPAxis*> axises;
    axises<<ui->plot->xAxis<<ui->plot->xAxis2<<ui->plot->yAxis;

    ui->plot->axisRect()->setRangeDragAxes(axises);

    ui->plot->axisRect()->setRangeZoomAxes(axises);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::green));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));

    ui->plot->addGraph(ui->plot->xAxis2,ui->plot->yAxis);
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));

    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(setRng(QCPRange)));
    connect(ui->plot->xAxis2,SIGNAL(rangeChanged(QCPRange)),this,SLOT(setRng(QCPRange)));

    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->xAxis2,SIGNAL(rangeChanged(QCPRange)),this,SLOT(xAxis2Changed(QCPRange)));
    connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(yAxisChanged(QCPRange)));
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

    emit setVoltageSignal();

    replot();
}

void Voltage::changeItem(int row,int col)
{
    if (col==2)
    {
        disconnect(ui->tableWidget,&QTableWidget::cellChanged,this,&Voltage::changeItem);
        bool isNum=false;
        double num=ui->tableWidget->item(row,col)->text().toDouble(&isNum);
        if (isNum && num<=100)
        {
            ui->tableWidget->item(row,col)->setBackground(QColor(25,25,25));
            setVolatge();
        }
        else if (num>100)
        {
            ui->tableWidget->item(row,col)->setText(QString::number(100));
            ui->tableWidget->item(row,col)->setBackground(QColor(25,25,25));
            setVolatge();
        }
        else
        {
            ui->tableWidget->item(row,col)->setBackground(Qt::red);
        }
        connect(ui->tableWidget,&QTableWidget::cellChanged,this,&Voltage::changeItem);
    }

}

void Voltage::replot()
{
    ui->plot->xAxis->setRange(-15,1.3e3);
    ui->plot->xAxis2->setRange(-0.15,13);
    ui->plot->yAxis->setRange(-0.4,10.4);

    ui->plot->graph(0)->setData(V_interpol.first,V_interpol.second);
    ui->plot->graph(1)->setData(V_time.first,V_time.second);

    ui->plot->replot();
    ui->plot->update();
}

void Voltage::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-15,1.3e3);
    limitAxisRange(axis,newRange,limitRange);
}

void Voltage::xAxis2Changed(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.15,13);
    limitAxisRange(axis,newRange,limitRange);
}

void Voltage::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.4,10.4);
    limitAxisRange(axis,newRange,limitRange);
}

void Voltage::setRng(const QCPRange &newRange)
{
    double factor=125;
    if (QObject::sender()==ui->plot->xAxis)
    {
        ui->plot->xAxis2->setRange(newRange/factor);
    }

    if (QObject::sender()==ui->plot->xAxis2)
    {
        ui->plot->xAxis->setRange(newRange*factor);
    }
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
