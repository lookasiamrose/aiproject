#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "../../opennn/multilayer_perceptron.h"
//#include "../../OpenNN/dlib/dlib/dnn.h"
//#include "../../OpenNN/dlib/dlib/data_io.h"
//#include "../../OpenNN/dlib/dlib/gui_widgets.h"

//using namespace dlib;

using namespace OpenNN;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Vector<unsigned> structure(5);
    structure[0] = 1;
    structure[1] = 2;
    structure[2] = 4;
    structure[3] = 2;
    structure[4] = 1;
    NeuralNetwork* myNN = new NeuralNetwork(2,2,2);
    /*MultilayerPerceptronPointer* mlpp = myNN.get_multilayer_perceptron_pointer();
    qDebug()<<"inputs_number " + mlpp.count_inputs_number();
    qDebug()<<"outputs_number " + mlpp.count_outputs_number();*/
    qDebug()<<myNN->count_parameters_number();

    Vector<double> in(2);
    in[0] = 0.5;
    in[1] = 0.25;

    Vector<double> out = myNN->calculate_outputs(in);
    qDebug()<<out;
    Matrix<double> jacobian = myNN->calculate_Jacobian(in);
    qDebug()<<jacobian;

    myNN->save("C:/Qt/neural_network.xml");

}

MainWindow::~MainWindow()
{
    delete ui;
}
