#ifndef VOLTAGEPLOT_H
#define VOLTAGEPLOT_H

#include <QDialog>

namespace Ui {
class VoltagePlot;
}

class VoltagePlot : public QDialog
{
    Q_OBJECT

public:
    explicit VoltagePlot(QWidget *parent = nullptr);
    ~VoltagePlot();

private:
    Ui::VoltagePlot *ui;
};

#endif // VOLTAGEPLOT_H
