#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();
    initPlot();

    setCycleParameters();

    // Tango::DeviceProxy *device = new Tango::DeviceProxy("training/course/pse1");
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
    userParameters=new UserParameters(this);
    fileOut=new FileOut();

    // check graphs
    connect(ui->action_E,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_B,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_dB,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_phase,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_freq,&QAction::changed,this,&MainWindow::replot);
    connect(ui->action_Vrf,&QAction::changed,this,&MainWindow::replot);

    // menu
    connect(ui->action_Voltage,&QAction::triggered,voltage,&Voltage::show);
    connect(ui->action_parametersAcceleration,&QAction::triggered,parametersAcceleration,&ParametersAcceleration::show);
    connect(ui->action_adiabaticity,&QAction::triggered,adiabaticity,&Adiabaticity::show);
    connect(ui->action_usual,&QAction::triggered,userParameters,&UserParameters::show);

    // voltage dialog
    connect(voltage,&Voltage::setVoltageSignal,this,&MainWindow::setCycleParameters);

    // parameters dialog
    connect(parametersAcceleration,&ParametersAcceleration::setParameters,this,&MainWindow::setCycleParameters);

    // parameters adiabaticity dialog
    connect(adiabaticity,&Adiabaticity::setAdiabaticityParameters,this,&MainWindow::setCycleParameters);
    connect(adiabaticity,&Adiabaticity::setAcceptance,this,&MainWindow::changeUserParameters);

    // user parameters dialog
    connect(userParameters,&UserParameters::setUserParameters,this,&MainWindow::setCycleParameters);
    connect(userParameters,&UserParameters::change,this,&MainWindow::changeUserParameters);
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

    ui->plot->xAxis->setRange(-0.05,10);
    ui->plot->yAxis->setRange(-0.05,10.1);

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::yellow));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(0)->setName("Энергия, ГэВ/нуклон");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(1)->setPen(QPen(Qt::cyan));
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(1)->setName("Магнитное поле, Тл");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(2)->setPen(QPen(Qt::red));
    ui->plot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(2)->setName("Производная магн. поля, Тл/с");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(3)->setPen(QPen(QColor(255,0,255)));
    ui->plot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(3)->setName("Фаза");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(4)->setPen(QPen(Qt::blue));
    ui->plot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(4)->setName("Частота, МГц");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(5)->setPen(QPen(Qt::green));
    ui->plot->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(5)->setName("Напряжение, кВ");

    ui->plot->legend->setVisible(true);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::setCycleParameters()
{
    clearPoints();

    voltage->V_interpol=calculate->setVoltageFrequency(voltage->V);

    calculate->setFullPeriod(voltage->V_interpol,parametersAcceleration->parameters,adiabaticity->parameters,userParameters->parameters);

    points.time=calculate->time;
    points.BField=calculate->BField;
    points.dBField=calculate->dBField;
    points.Energy=calculate->Energy;
    points.phase=calculate->phase;
    points.freq=calculate->freq;

    fileOut->out_V(points.time,calculate->Voltage);
    fileOut->out_dBField(points.time,points.dBField);
    fileOut->out_BField(points.time,points.BField);

    // to TANGO dx=1e-5
    points.B_out=calculate->B_out;
    points.dB_out=calculate->dB_out;

    voltage->V_time=QPair <QVector <double>,QVector <double>> (points.time,calculate->Voltage);

    fileOut->writeToFile(voltage->V_time);

    voltage->replot();
    replot();
}

void MainWindow::changeUserParameters(int index)
{
    switch (index)
    {
    case 0:
        userParameters->setParameter(1,calculate->dE(parametersAcceleration->parameters,userParameters->parameters));
        adiabaticity->changeAcceptance(calculate->acceptance(parametersAcceleration->parameters,userParameters->parameters));
        userParameters->setLabelAcceptance(adiabaticity->parameters[1]);
        break;
    case 1:
        userParameters->setParameter(0,calculate->dp(parametersAcceleration->parameters,userParameters->parameters));
        adiabaticity->changeAcceptance(calculate->acceptance(parametersAcceleration->parameters,userParameters->parameters));
        userParameters->setLabelAcceptance(adiabaticity->parameters[1]);
        break;
    case 2:
        userParameters->setParameter(3,calculate->E_inj(parametersAcceleration->parameters,userParameters->parameters));
        break;
    case 3:
        userParameters->setParameter(2,calculate->B_inj(parametersAcceleration->parameters,userParameters->parameters));
        break;
    case 4:
        userParameters->setLabelAcceptance(adiabaticity->parameters[1]);
        break;
    default:
        break;
    }
}

void MainWindow::clearPoints()
{
    points.Energy.clear();
    points.BField.clear();
    points.dBField.clear();
    points.phase.clear();
    points.time.clear();
    points.B_out.clear();
    points.dB_out.clear();
}

void MainWindow::replot()
{
    if (ui->action_E->isChecked())
    {
        ui->plot->graph(0)->setData(points.time,points.Energy);
        ui->plot->graph(0)->addToLegend();
    }
    else if (!ui->action_E->isChecked())
        {
            ui->plot->graph(0)->data()->clear();
            ui->plot->graph(0)->removeFromLegend();
        }

    if (ui->action_B->isChecked())
    {
        ui->plot->graph(1)->setData(points.time,points.BField);
        ui->plot->graph(1)->addToLegend();
    }
    else if (!ui->action_B->isChecked())
        {
            ui->plot->graph(1)->data()->clear();
            ui->plot->graph(1)->removeFromLegend();
        }
    if (ui->action_dB->isChecked())
    {
        ui->plot->graph(2)->setData(points.time,points.dBField);
        ui->plot->graph(2)->addToLegend();
    }
    else if (!ui->action_dB->isChecked())
        {
            ui->plot->graph(2)->data()->clear();
            ui->plot->graph(2)->removeFromLegend();
        }

    if (ui->action_phase->isChecked())
    {
        ui->plot->graph(3)->setData(points.time,points.phase);
        ui->plot->graph(3)->addToLegend();
    }
    else if (!ui->action_phase->isChecked())
        {
            ui->plot->graph(3)->data()->clear();
            ui->plot->graph(3)->removeFromLegend();
        }

    if (ui->action_freq->isChecked())
    {
        ui->plot->graph(4)->setData(points.time,points.freq);
        ui->plot->graph(4)->addToLegend();
    }
    else if (!ui->action_freq->isChecked())
        {
            ui->plot->graph(4)->data()->clear();
            ui->plot->graph(4)->removeFromLegend();
        }

    if (ui->action_Vrf->isChecked())
    {
        ui->plot->graph(5)->setData(points.time,calculate->Voltage);
        ui->plot->graph(5)->addToLegend();
    }
    else if (!ui->action_Vrf->isChecked())
        {
            ui->plot->graph(5)->data()->clear();
            ui->plot->graph(5)->removeFromLegend();
        }

    if (ui->action_autoreplot->isChecked())
    {
        ui->plot->xAxis->setRangeUpper(points.time.last()+(5*points.time.last()/100));
        ui->plot->yAxis->setRange(-0.05,10.1);
    }

    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.05,10);
    limitAxisRange(axis,newRange,limitRange);
}

void MainWindow::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.05,10.1);
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


