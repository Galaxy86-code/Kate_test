#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "form1.h"

#define NUM_SAMPLES_z 5
#define NUM_SAMPLES_y 2
#define NUM_SAMPLES_x 8

typedef struct
{
    int16_t TX[64];//(X)
    int16_t RX[64];//(Z)
    int16_t Samples[8192];//(Y)
} part;



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    Form1 *form1;

    QFileDialog *FileDialog1;
    QFile *FileBin;
    QString PathName;
    QByteArray array1;

    //part Part0;
    //part Part1;

    /*int16_t mxy0[3];
    int16_t mxy1[3];
    int16_t mxy2[3];*/

    int16_t Part0[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];
    int16_t Part1[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];

    int16_t A[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];
signals:
    void sendValueSlider(int);
    //void Signal_sendA(int16_t[NUM_SAMPLES_y][NUM_SAMPLES_x]);
    void Signal_sendA(int16_t *);

};

#endif // MAINWINDOW_H
