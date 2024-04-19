#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();
    initPlot();

    setCycleParameters();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
    setWindowTitle("Расчет магнитного поля");

    setWindowIcon(QIcon(":/icons/1.ico"));

    voltage=new Voltage(this);
    calculate=new Calculate();
    parametersAcceleration=new ParametersAcceleration(this);
    adiabaticity=new Adiabaticity(this);

    // check graphs
    connect(ui->action_E,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_B,&QAction::changed,this,&MainWindow::replot);

    // menu
    connect(ui->action_Voltage,&QAction::triggered,voltage,&Voltage::show);
    connect(ui->action_parametersAcceleration,&QAction::triggered,parametersAcceleration,&ParametersAcceleration::show);
    connect(ui->action_adiabaticity,&QAction::triggered,adiabaticity,&Adiabaticity::show);

    // voltage dialog
    connect(voltage,&Voltage::setVoltageSignal,this,&MainWindow::setCycleParameters);

    // parameters dialog
    connect(parametersAcceleration,&ParametersAcceleration::setParameters,this,&MainWindow::setCycleParameters);

    // parameters adiabaticity
    connect(adiabaticity,&Adiabaticity::setAdiabaticityParameters,this,&MainWindow::setCycleParameters);
}

void MainWindow::initPlot()
{
    // plot
    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(yAxisChanged(QCPRange)));

    /////////////////////////////////////////
    /// Dark mode
    ui->plot->xAxis->setBasePen(QPen(Qt::white,1));
    ui->plot->yAxis->setBasePen(QPen(Qt::white,1));
    ui->plot->xAxis->setTickPen(QPen(Qt::white,1));
    ui->plot->yAxis->setTickPen(QPen(Qt::white,1));
    ui->plot->xAxis->setSubTickPen(QPen(Qt::white,1));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::white,1));
    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->yAxis->setTickLabelColor(Qt::white);
    ui->plot->xAxis->grid()->setPen(QPen(QColor(140,140,140),1,Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140,140,140),1,Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridPen(QPen(QColor(80,80,80),1,Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(80,80,80),1,Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis->grid()->setZeroLinePen(QPen(Qt::white,2));
    ui->plot->yAxis->grid()->setZeroLinePen(QPen(Qt::white,2));
    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
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

    QFont pfont=font();
    pfont.setPointSize(11);
    pfont.setStyleHint(QFont::SansSerif);
    ui->plot->legend->setFont(pfont);
    ui->plot->legend->setSelectedFont(pfont);

    ui->plot->xAxis->setTickLabelFont(pfont);
    ui->plot->yAxis->setTickLabelFont(pfont);

    ui->plot->xAxis->setRange(-0.15,10.2);
    ui->plot->yAxis->setRange(-0.1,15.4);

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(0)->setName("Энергия, ГэВ/нукон");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(1)->setPen(QPen(Qt::yellow));
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(1)->setName("Магнитное поле, Тл");

    ui->plot->legend->setVisible(true);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::setCycleParameters()
{
    clearPoints();

    // Voltage
    voltage->V_interpol=calculate->setVoltageFrequency(voltage->V);
    voltage->V_time=calculate->setVoltageTime(voltage->V_interpol,parametersAcceleration->parameters,adiabaticity->parameters);

    // Energy
    // points.E=calculate->setEnergy(voltage->V_time,parametersAcceleration->parameters,adiabaticity->parameters);

    // B field
    // points.B=calculate->setBField(points.E,parametersAcceleration->parameters);

    // dB

    // Frequency


    // qDebug()<<"jhfgjhgfj ="<<calculate->findNextX(0,00,1.00341,0.983212,0.12,parametersAcceleration->parameters);

    // Acceptance



    // qDebug()<<"Поле инжекции, Тл ="<<calculate->B_inj(parametersAcceleration->parameters);
    // qDebug()<<"B_0 ="<<calculate->B_0(parametersAcceleration->parameters);
    // qDebug()<<"Частота инжекции, кГц ="<<calculate->f_inj(parametersAcceleration->parameters)/1e3;
    // qDebug()<<"Акцептанс инжекции ="<<calculate->e_inj(parametersAcceleration->parameters);
    // qDebug()<<"Скорость пучка на инжекции, м/с ="<<calculate->Velocity_inj(parametersAcceleration->parameters);
    // qDebug()<<"V_0 ="<<calculate->V_adiabaticity(parametersAcceleration->parameters,adiabaticity->parameters,0);
    // qDebug()<<"V_1 ="<<calculate->V_adiabaticity(parametersAcceleration->parameters,adiabaticity->parameters,1);
    // qDebug()<<"t_ad ="<<calculate->t_adiabaticity(parametersAcceleration->parameters,adiabaticity->parameters);
    // qDebug()<<"======================================";

    voltage->replot();
    replot();
}

void MainWindow::clearPoints()
{
    points.V.first.clear();
    points.V.second.clear();

    points.B.first.clear();
    points.B.second.clear();

    points.dB.first.clear();
    points.dB.second.clear();

    points.f.first.clear();
    points.f.second.clear();

    points.E.first.clear();
    points.E.second.clear();

    points.Acceptance.first.clear();
    points.Acceptance.second.clear();
}


void MainWindow::replot()
{
    if (ui->action_E->isChecked())
        ui->plot->graph(0)->setData(points.E.first,points.E.second);
    else if (!ui->action_E->isChecked())
        ui->plot->graph(0)->data()->clear();

    if (ui->action_B->isChecked())
        ui->plot->graph(1)->setData(points.B.first,points.B.second);
    else if (!ui->action_B->isChecked())
        ui->plot->graph(1)->data()->clear();

    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.15,10.2);
    limitAxisRange(axis,newRange,limitRange);
}

void MainWindow::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.1,15.4);
    limitAxisRange(axis,newRange,limitRange);
}

void MainWindow::limitAxisRange(QCPAxis *axis,const QCPRange &newRange,const QCPRange &limitRange)
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


