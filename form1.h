#ifndef FORM1_H
#define FORM1_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <QLabel>
#include "qcustomplot.h"

#define NUM_SAMPLES_z 5
#define NUM_SAMPLES_y 2
#define NUM_SAMPLES_x 8

namespace Ui {
class Form1;
}

class Form1 : public QWidget
{
    Q_OBJECT

public:
    explicit Form1(QWidget *parent = 0);
    ~Form1();

private:
    Ui::Form1 *ui;
    QLabel *label;
    QCustomPlot *cP;
    int v_sl;

    int16_t B[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];

public slots:
    void onMouseMoveOverHeatmap(QMouseEvent *event);
    void getValueSlider(int);
    void getA(int16_t *p);
    void showGraph(int16_t);
};

#endif // FORM1_H
