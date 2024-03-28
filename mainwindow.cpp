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
    setWindowIcon(QIcon(":/icons/1.ico"));

    voltage=new Voltage(this);
    calculate=new Calculate();
    parametersAcceleration=new ParametersAcceleration(this);

    // menu
    connect(ui->action_Voltage,&QAction::triggered,voltage,&Voltage::show);
    connect(ui->action_parametersAcceleration,&QAction::triggered,parametersAcceleration,&ParametersAcceleration::show);

    // voltage dialog
    connect(voltage,&Voltage::setVoltageSignal,this,&MainWindow::setCycleParameters);

    // parameters dialog
    connect(parametersAcceleration,&ParametersAcceleration::setParameters,this,&MainWindow::setCycleParameters);
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

    ui->plot->xAxis->setRange(-0.2,10e3);
    ui->plot->yAxis->setRange(-0.4,10.4);

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::green));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(0)->setName("Напряжение");

    // ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    // ui->plot->graph(1)->setPen(QPen(Qt::red));
    // ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    // ui->plot->graph(1)->setName("Интреполяция");

    ui->plot->legend->setVisible(true);

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


}

void MainWindow::setCycleParameters()
{
    clearPoints();

    // Voltage
    voltage->V_interpol=calculate->setVoltage(voltage->V);
    points.V=voltage->V_interpol;

    voltage->replot();


    qDebug()<<"Поле инжекции, Тл ="<<calculate->B_inj(parametersAcceleration->parameters);
    qDebug()<<"B_0 ="<<calculate->B_0(parametersAcceleration->parameters);
    qDebug()<<"Частота инжекции, Гц ="<<calculate->f_inj(parametersAcceleration->parameters);
    qDebug()<<"Акцептанс инжекции ="<<calculate->e_inj(parametersAcceleration->parameters);
    qDebug()<<"Скорость пучка на инжекции, м/с ="<<calculate->Velocity_inj(parametersAcceleration->parameters);
    qDebug()<<"======================================";
    // B field

    // dB

    // frequency

    // Energy

    // Acceptance

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

/*
void MainWindow::interpolation(QVector <double> x,QVector <double> y)
{

    for (int i=0;i<V.x.size()-2;i++)
    {
        double h=V.x[i+1]-V.x[i];
        double s=(V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i]);
        double d=V.y[i];
        double c=der(h,V.y[i],V.y[i+1]);
        double b=(3*s-2*der(h,V.y[i],V.y[i+1])-der(h,V.y[i+1],V.y[i+2]))/h;
        double a=(der(h,V.y[i],V.y[i+1])+der(h,V.y[i+1],V.y[i+2])-2*s)/(h*h);

        for (int ii=i*100;ii<100*(i+1);ii++)
        {
            double temp=0;

            Interpolation.x.push_back(ii*0.01);
            temp=a*pow(Interpolation.x[ii]-V.x[i],3)+b*pow(Interpolation.x[ii]-V.x[i],2)+c*(Interpolation.x[ii]-V.x[i])+d;
            Interpolation.y.push_back(temp);
            // qDebug()<<a<<b<<c<<d<<s<<der(h,V.y[i],V.y[i+1]);

        }
    }
    //


    // this
    for (int i=0;i<x.size()-2;i++)
    {
        double dx=0.01;

        double h=x[i+1]-x[i];

        double der=derivative(h,y[i],y[i+1]);

        double s=(y[i+1]-y[i])/(x[i+1]-x[i]);

        double a=(derivative(h,y[i],y[i+1])+derivative(h,y[i+1],y[i+2])-2*s)/(h*h);

        double p=(s*(x[i+2]-x[i+1])+(x[i+2]-x[i+1])*h)/(h+x[i+2]-x[i+1]+(y[i+2]-y[i+1]));

        if ((y[i+1]-y[i])/(x[i+1]-x[i])*(y[i+2]-y[i+1])/(x[i+2]-x[i+1])<=0) der=0;
        else if (fabs(p)>2*fabs(s)) der=2*a*std::min(s,x[i+2]-x[i+1]);
        else if (fabs(p)>2*fabs(x[i+2]-x[i+1])) der=a;
        else der=p;

        a=(der+der-2*s)/(h*h);

        double d=y[i];
        double c=der;
        double b=(3*s-2*derivative(h,y[i],y[i+1])-derivative(h,y[i+1],y[i+2]))/h;

        for (int ii=i*100;ii<100*(i+1);ii++)
        {
            double temp=0;

            Interpolation.x.push_back(ii*dx);
            temp=a*pow(Interpolation.x[ii]-x[i],3)+b*pow(Interpolation.x[ii]-x[i],2)+c*(Interpolation.x[ii]-x[i])+d;
            Interpolation.y.push_back(temp);
            // qDebug()<<a<<b<<c<<d<<s<<der(h,y[i],y[i+1]);

        }
    }


    // int N=x.size();

    // for (int i=0;i<N;i++)
    // {

    // }


    for (int i=0;i<V.x.size()-2;i++)
    {
        double h=V.x[i+1]-V.x[i];

        double der=derivative(h,V.y[i+1],V.y[i+2]);

        double s=(V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i]);

        double a=(derivative(h,V.y[i],V.y[i+1])+derivative(h,V.y[i+1],V.y[i+2])-2*s)/(h*h);

        double p=(s*(V.x[i+2]-V.x[i+1])+(V.x[i+2]-V.x[i+1])*h)/(h+V.x[i+2]-V.x[i+1]+(V.y[i+2]-V.y[i+1]));

        // if ((V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i])*(V.y[i+2]-V.y[i+1])/(V.x[i+2]-V.x[i+1])<=0) der=0;
        // else if (fabs(p)>2*fabs(s)) der=2*a*std::min(s,V.x[i+2]-V.x[i+1]);
        // else if (fabs(p)>2*fabs(V.x[i+2]-V.x[i+1])) der=a;
        // else der=p;

        der=(sign(1,V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i])+sign(1,V.y[i+2]-V.y[i+1])/(V.x[i+2]-V.x[i+1]))*min_c(fabs((V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i])),fabs((V.y[i+2]-V.y[i+1])/(V.x[i+2]-V.x[i+1])),0.5*fabs(p));

        double d=V.y[i];
        double c=der;
        double b=(3*s-2*der-derivative(h,V.y[i+1],V.y[i+2]))/h;

        for (int ii=i*100;ii<100*(i+1);ii++)
        {
            double temp=0;

            Interpolation.x.push_back(ii*0.01);
            temp=a*pow(Interpolation.x[ii]-V.x[i],3)+b*pow(Interpolation.x[ii]-V.x[i],2)+c*(Interpolation.x[ii]-V.x[i])+d;
            Interpolation.y.push_back(temp);
            // qDebug()<<a<<b<<c<<d<<s<<der(h,V.y[i],V.y[i+1]);

        }
    }
//

}

double MainWindow::derivative(double h,double y1,double y2)
{
    return (y2-y1)/h;
}

int MainWindow::factorial(int x)
{
    int res=1;
    for (int i=1;i<=x;i++)
    {
        res*=i;
    }
    if (x>0) return res;
    else return 1;

}

double MainWindow::sign(double a, double s)
{
    if (s>0) return a;
    if (s<0) return -a;
    if (s==0) return 0;
}

double MainWindow::min_c(double a, double b, double c)
{
    if (a<b && a<c) return a;
    else if (b<a && b<c) return b;
    else if (c<a && c<b) return c;
}
*/

void MainWindow::replot()
{
    // ui->plot->graph(0)->setData(points.V.first,points.V.second);
    // ui->plot->graph(1)->setData(Interpolation.x,Interpolation.y);

    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.2,10e3);
    limitAxisRange(axis,newRange,limitRange);
}

void MainWindow::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.4,10.4);
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


