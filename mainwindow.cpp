#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initPlot()
{
    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(0)->setName("Функция");

    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis);
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,3));
    ui->plot->graph(1)->setName("Интреполяция");

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    for (int i=0;i<100;i++)
    {
        V.x.push_back(i*0.1);
        V.y.push_back(i);
    }
    for (int i=100;i<200;i++)
    {
        V.x.push_back(i*0.1);
        V.y.push_back(200-i);
    }

    // interpolation();

    double *a=new double [2];
    a[0]=4;
    a[1]=9;
    a[2]=16;
    a[3]=25;
    qDebug()<<der(4,1,a);

    replot();

}

void MainWindow::interpolation()
{
    for (int i=0;i<198;i++)
    {
        double h=V.x[i+1]-V.x[i];
        double s=(V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i]);
        double d=V.y[i];
        double c=0;
        double b=(3*s-2*(V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i])-(V.y[i+2]-V.y[i+1])/(V.x[i+2]-V.x[i+1]))/h;
        double a=((V.y[i+1]-V.y[i])/(V.x[i+1]-V.x[i])+(V.y[i+2]-V.y[i+1])/(V.x[i+2]-V.x[i+1])-2*s)/(h*h);
        c=s;

        double temp=0;

        Interpolation.x.push_back(i*0.01);
        temp=a*pow(Interpolation.x[i]-V.x[i],3)+b*pow(Interpolation.x[i]-V.x[i],2)+c*(Interpolation.x[i]-V.x[i])+d;
        Interpolation.y.push_back(temp);
    }
}

double MainWindow::der(int n,double h,double *y)
{
    double der=0;
    for (int i=0;i<n-1;i++)
    {
        der+=y[i];
    }
    qDebug()<<der;
    der/=n-1;
    return der;
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

void MainWindow::xAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast<QCPAxis*> (QObject::sender());
    QCPRange limitRange(-0.5,10.5);
    limitAxisRange(axis,newRange,limitRange);
}

void MainWindow::yAxisChanged(const QCPRange &newRange)
{
    QCPAxis *axis=qobject_cast <QCPAxis*> (QObject::sender());
    if (QObject::sender()==ui->plot->yAxis2)
    {
        QCPRange limitRange(-10.5,10.5);
        limitAxisRange(axis,newRange,limitRange);
    }

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

void MainWindow::replot()
{
    ui->plot->graph(0)->setData(V.x,V.y);
    ui->plot->graph(1)->setData(Interpolation.x,Interpolation.y);

    ui->plot->replot();
    ui->plot->update();
}
