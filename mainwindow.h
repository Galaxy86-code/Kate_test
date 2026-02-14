#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "form1.h"
#include "common.h"


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

    void Slot_get_x(int);
    void Slot_get_sample(int);
    void Slot_get_A(int);

private:
    Ui::MainWindow *ui;

    Form1 *form1;

    QFileDialog *FileDialog1;
    QFile *FileBin;
    QString PathName;
    QByteArray array1;

    static int16_t Part0[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];
    static int16_t Part1[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];
    static int32_t A[NUM_SAMPLES_z][NUM_SAMPLES_y][NUM_SAMPLES_x];

    void showGraph(void);
signals:
    void sendValueSlider(int);
    void Signal_sendA(int32_t *);
};

#endif // MAINWINDOW_H
