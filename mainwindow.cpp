#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    form1 = new Form1();
    /*form1->setWindowTitle("B-scan viewer");
    form1->show();*/

    FileBin = new QFile();
    ui->label->setText("Загружен файл: ---");

    ui->centralWidget->setLayout(ui->verticalLayout);

    connect(this, SIGNAL(sendValueSlider(int)), form1, SLOT(getValueSlider(int)));

    connect(this, SIGNAL(Signal_sendA(int32_t *)), form1, SLOT(getA(int32_t *)));

    connect(form1, SIGNAL(Signal_form1_sendx(int)), this, SLOT(Slot_get_x(int)));
    connect(form1, SIGNAL(Signal_form1_sendsamples(int)), this, SLOT(Slot_get_sample(int)));
    connect(form1, SIGNAL(Signal_form1_sendA(int)), this, SLOT(Slot_get_A(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete form1;
    delete FileBin;
}

void MainWindow::on_pushButton_clicked()
{
    FileDialog1 = new QFileDialog();
    PathName.clear();
    //PathName.append(FileDialog1->getOpenFileName(0, "Open Dialog", "", "*.bin *.binary"));
    PathName.append("C:/2023-04-12-10-38-29 (1).binary");
    if (PathName!="")
    {
        ui->label->setText("Загружен файл: "+ PathName);
        FileBin->setFileName(PathName);

        if(!(FileBin->open(QIODevice::ReadOnly)))
        {
            QMessageBox::information(this, tr("Ошибка!"),
                                     tr("Невозможно открыть файл"),
                                     QMessageBox::Ok);
        }
        else
        {
            array1.clear();
            int32_t rd_byte_from_file = 0;

            memset(&Part0,0,sizeof(Part0));
            memset(&Part1,0,sizeof(Part1));
            memset(&A,0,sizeof(A));

            while(!(FileBin->atEnd()))
            {
                for(int i = 0; i < NUM_SAMPLES_z; ++i)
                {
                    array1.clear();
                    array1.append(FileBin->read(sizeof(int16_t)*8*2));
                    memcpy(&Part0[i],array1,sizeof(int16_t)*8*2);

                    rd_byte_from_file += sizeof(int16_t)*8*2;
                }

                for (int i = 0; i < NUM_SAMPLES_z; ++i)
                {
                    array1.clear();
                    array1.append(FileBin->read(sizeof(int16_t)*8*2));
                    memcpy(&Part1[i],array1,sizeof(int16_t)*8*2);

                    rd_byte_from_file += sizeof(int16_t)*8*2;
                }

                break;//
            }

            int16_t A0, A1;
            for(int i = 0; i < NUM_SAMPLES_z; ++i)
            {
                for(int j = 0; j < NUM_SAMPLES_y; ++j)
                {
                    for(int k = 0; k < NUM_SAMPLES_x; ++k)
                    {
                        A0 = Part0[i][j][k];
                        A1 = Part1[i][j][k];
                        A[i][j][k] = sqrt(pow(A0,2) + pow(A1,2));
                    }
                }
            }

            int32_t *ptr = **A;
            emit Signal_sendA(ptr);

            FileBin->close();
        }
    }
    else QMessageBox::information(this, tr("Предупреждение"),
                                  tr("Не выбран Файл"),
                                  QMessageBox::Ok);

    delete FileDialog1;

    showGraph();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_slider->setText(QString::number(value));
    emit sendValueSlider(value);
}

void MainWindow::Slot_get_x(int x)
{
    ui->label_x->setText("x = " + QString::number(x));
}

void MainWindow::Slot_get_sample(int s)
{
    ui->label_sample->setText("sample = " + QString::number(s));
}

void MainWindow::Slot_get_A(int A)
{
    ui->label_A->setText("A = " + QString::number(A));
}

void MainWindow::showGraph(void)
{
    form1->setWindowTitle("B-scan viewer индекс курсора = 0");

    QRect mainRect = this->frameGeometry();
    int newX = mainRect.x() + mainRect.width() + 20;
    int newY = mainRect.y();
    form1->move(newX, newY);

    form1->show();
}


