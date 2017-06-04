#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new Model(this);
    img = model->movementSweden();

    myLabel = new QLabel(this);
    myLabel->setPixmap(QPixmap::fromImage(*img));
    myLabel->show();

    ui->gridLayout->addWidget(myLabel);
    myLabel->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(15);
    effect->setColor(QColor("#000"));
    myLabel->setGraphicsEffect(effect);

    try
    {
        srand((unsigned int)time(NULL));
    }
    catch(std::exception& e)
    {
        qDebug() << e.what();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QImage* test = model->test(img);
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    delete img;
    img = test;


    QString path = QCoreApplication::applicationDirPath() + "/DATA/";
    test->save(path+"output.png","PNG",100);
}

void MainWindow::on_pushButton_2_clicked()
{
    QImage* test = model->hardenImageContrast(img,0.34);
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    delete img;
    img = test;
}

void MainWindow::on_pushButton_3_clicked()
{
    model->saveImageAsInGreyscaleHTML(img,QCoreApplication::applicationDirPath() + "/DATA/capture",Normalization::HIGHLIGHTED);
}

void MainWindow::on_pushButton_4_clicked()
{
    QImage* test = model->simplifyImageWithFactorSoft(img,0.75);
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    delete img;
    img = test;
}

void MainWindow::on_pushButton_5_clicked()
{
    QImage* test = model->removeDuplicatePixelsVertically(img,0.01);
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    delete img;
    img = test;
}
