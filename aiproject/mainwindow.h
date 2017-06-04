#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// System includes
/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
*/
#include <time.h>
#include <algorithm>

#include <stdint.h>
#include <limits.h>

// OpenNN includes

#include "../opennn/opennn.h"

#include "model.h"
#include <QLabel>

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

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    Model* model;

    QImage* img;
    QLabel* myLabel;
};

#endif // MAINWINDOW_H
