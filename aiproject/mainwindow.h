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
#include <QtWebEngineWidgets/QWebEngineView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "graphics_view_zoom.h"

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

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_LearnTheOpenNN_clicked();

    void on_pushButton_ConvertToExcel_clicked();

    void on_pushButton_GoBack_clicked();

    void on_pushButton_GoBack_2_clicked();
    void completedTableClicked();

private:
    Ui::MainWindow *ui;
    Model* model;

    QImage* img;
    QLabel* myLabel;
    QList<QImage*> history;
    QWebEngineView *view;
    QGraphicsScene* scene;
    QGraphicsView* graph;
    Graphics_view_zoom* z;
};

#endif // MAINWINDOW_H
