#include "form1.h"
#include "ui_form1.h"
#include "mainwindow.h"

Form1::Form1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form1)
{
    ui->setupUi(this);

    v_sl = 0;
    max_A = 0;

    cP = new QCustomPlot();
    cP->resize(80, 60);
    ui->verticalLayout_2->addWidget(cP);
    setLayout(ui->verticalLayout_2);

    //cP->xAxis->setLabel("X");
    //cP->yAxis->setLabel("Y");

    QCPColorMap *heatmap = new QCPColorMap(cP->xAxis, cP->yAxis);
    cP->addPlottable(heatmap);

    connect(cP, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(onMouseMoveOverHeatmap(QMouseEvent*)));
}

Form1::~Form1()
{
    delete ui;
}

void Form1::onMouseMoveOverHeatmap(QMouseEvent *event)
{
    // Конвертируем пиксели в координаты axes
    double xCoord = cP->xAxis->pixelToCoord(event->pos().x());
    double yCoord = cP->yAxis->pixelToCoord(event->pos().y());

    QCPColorMap *colorMap = qobject_cast<QCPColorMap*>(cP->plottable(0));
    if (!colorMap) return;

    QCPColorMapData *data = colorMap->data();
    if (!data) return;

    // Получаем границы диапазона осей цветовой карты
    double keyMin = data->keyRange().lower;
    double keyMax = data->keyRange().upper;
    double valueMin = data->valueRange().lower;
    double valueMax = data->valueRange().upper;

    int nx = data->keySize();   // количество ячеек по оси X
    int ny = data->valueSize(); // количество ячеек по оси Y

    // Проверка: деления на ноль, если сетка вырождена
    if (nx <= 1 || ny <= 1) return;

    // Вычисляем индексы ячейки
    double cellWidth = (keyMax - keyMin) / nx;
    double cellHeight = (valueMax - valueMin) / ny;

    // Индекс по X: округление вниз, с проверкой выхода за границы
    int ix = static_cast<int>((xCoord - keyMin) / cellWidth);
    ix = qBound(0, ix, nx - 1);

    // Индекс по Y: аналогично
    int iy = static_cast<int>((yCoord - valueMin) / cellHeight);
    iy = qBound(0, iy, ny - 1);

    // Получаем значение в ячейке
    int cellValue = (int)(data->cell(ix, iy));

    emit Signal_form1_sendx(ix);
    emit Signal_form1_sendsamples(iy);
    emit Signal_form1_sendA(cellValue);
}

void Form1::getValueSlider(int value)
{
    v_sl = value;

    showGraph(value);

    setWindowTitle("B-scan viewer индекс курсора = " + QString::number(value));
}

void Form1::getA(int32_t *p)
{
    max_A = 0;
    for(int i = 0; i < NUM_SAMPLES_z; ++i)
    {
        for(int j = 0; j < NUM_SAMPLES_y; ++j)
        {
            for(int k = 0; k < NUM_SAMPLES_x; ++k)
            {
                B[i][j][k] = *p;
                p++;

                if(B[i][j][k] > max_A)
                {
                    max_A = B[i][j][k];
                }
            }
        }
    }

    showGraph(0);
}

void Form1::showGraph(int16_t v_sl)
{
    //QCPColorMap *heatmap = new QCPColorMap(cP->xAxis, cP->yAxis);
    //cP->addPlottable(heatmap);
    QCPColorMap *heatmap = qobject_cast<QCPColorMap*>(cP->plottable(0));

    // Настраиваем размер данных
    int nx = NUM_SAMPLES_x;
    int ny = NUM_SAMPLES_z;
    heatmap->data()->setSize(nx, ny);
    heatmap->data()->setRange(QCPRange(0, NUM_SAMPLES_x), QCPRange(0, NUM_SAMPLES_z));

    cP->xAxis->setRange(0, NUM_SAMPLES_x);
    cP->xAxis->setAutoTickStep(true);
    //cP->xAxis->setAutoTickStep(false);
    //cP->xAxis->setTickStep(1.0);

    cP->yAxis->setRange(0, NUM_SAMPLES_z);
    cP->yAxis->setAutoTickStep(true);
    //cP->yAxis->setAutoTickStep(false);
    //cP->yAxis->setTickStep(1.0);

    // Заполняем данными
    for (int x = 0; x < nx; ++x)
    {
        for (int y = 0; y < ny; ++y)
        {
            //double v = x*100 + y * 1000;
            double value = B[y][v_sl][x];

            heatmap->data()->setCell(x, y, value);
        }
    }

    // Добавляем цветовую шкалу
    QCPColorScale *colorScale = new QCPColorScale(cP);
    cP->plotLayout()->addElement(0, 1, colorScale);

    colorScale->setDataRange(QCPRange(0,max_A));

    /*colorScale->axis()->setAutoTicks(false);
    colorScale->axis()->setAutoTickStep(false);
    colorScale->axis()->setTickStep(1000.0);
    colorScale->axis()->setAutoSubTicks(false);
    colorScale->axis()->setSubTickCount(0);

    colorScale->axis()->setAutoTickLabels(false);

    QVector<QString> labels;
    labels << "0";   // подпись слева/снизу
    labels << "1000";  // подпись справа/сверху
    labels << "2000";  // подпись справа/сверху
    labels << "3000";  // подпись справа/сверху
    labels << "4000";  // подпись справа/сверху
    labels << "5000";  // подпись справа/сверху
    labels << "6000";  // подпись справа/сверху
    labels << "7000";  // подпись справа/сверху
    colorScale->axis()->setTickVectorLabels(labels);*/

    QCPColorGradient rainbowGradient;
    rainbowGradient.clearColorStops();
    rainbowGradient.setColorStopAt(0.00, QColor(75, 0, 130));    //(Indigo)
    rainbowGradient.setColorStopAt(0.17, QColor(0, 0, 255));     //(Blue)
    rainbowGradient.setColorStopAt(0.33, QColor(0, 255, 0));     //(Green)
    rainbowGradient.setColorStopAt(0.50, QColor(255, 255, 0));   //(Yellow)
    rainbowGradient.setColorStopAt(0.67, QColor(255, 165, 0));   //(Orange)
    rainbowGradient.setColorStopAt(0.83, QColor(255, 0, 0));     //(Red)
    rainbowGradient.setColorStopAt(1.00, QColor(153, 0, 0));     //(DarkRed)
    rainbowGradient.setColorInterpolation(QCPColorGradient::ciHSV);
    colorScale->setGradient(rainbowGradient);

    heatmap->setColorScale(colorScale);

    QCPMarginGroup *marginGroup = new QCPMarginGroup(cP);
    // Добавляем axisRect (область графика) в группу по нижнему краю
    cP->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    // Добавляем цветовую шкалу в ту же группу
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // Перерисовываем график
    cP->rescaleAxes();
    cP->replot();
}
