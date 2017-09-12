#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QFile>
#include <QGraphicsRectItem>
#include <graphicsmatrixitem.h>
#include <QDropEvent>
#include <QMimeData>
#include <QSizePolicy>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QtXml/QtXml>

#define SQUARE_A 20

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->layout()->setSizeConstraint(QLayout::SetMaximumSize);
    //ui->gridLayout->setSizeConstraint(QLayout::SetMaximumSize);

    model = new Model(this);
    img = model->movementSweden();
    original = new QImage(*img);
    ui->checkBox->setChecked(true);
    //ui->stackedWidget->setAcceptDrops(true);

    myLabel = new QLabel(this);
    myLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //myLabel->setStyleSheet("background-color: #333");
    myLabel->setPixmap(QPixmap::fromImage(*img));
    myLabel->show();

    //ui->gridLayout->addWidget(myLabel,2,2,Qt::AlignRight);
    QHBoxLayout *layoutForImage = new QHBoxLayout();
    layoutForImage->addWidget(myLabel);
    myLabel->setAlignment(Qt::AlignRight);
    ui->ImagePlaceHolder->setLayout(layoutForImage);

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

    view = new QWebEngineView(parent);

    scene = new QGraphicsScene(parent);
    graph = new QGraphicsView(scene,parent);

    QPushButton *button = new QPushButton("     &Proceed     ", parent);
    button->setStyleSheet("background-color: #fff; color: #111; border: 2px solid #555; font-size: 24px;");
    button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    QObject::connect( button, SIGNAL(clicked()) , this, SLOT(completedTableClicked()) );
    graph->addScrollBarWidget(button,Qt::AlignRight);

    //graph->setSceneRect(0,0,graph->frameSize().width(),graph->frameSize().height());

    z = new Graphics_view_zoom(graph);
    z->set_modifiers(Qt::NoModifier);

    setAcceptDrops(true);
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls(); //take basic paths and urls from MIME
    QUrl url = urls.at(0);
    delete img;

    if(original != nullptr) delete original;

    img = new QImage(url.toLocalFile());
    original = new QImage(*img);

    QPixmap p = QPixmap::fromImage(*img); // load pixmap
    int h = ui->ImagePlaceHolder->height();
    int w = ui->ImagePlaceHolder->width();
    // get label
    if (w > 1200){
        w = 1200;

    }
    if (h > 1000){
        h = 1000;   // set a scaled pixmap to a w x h window keeping its aspect ratio
    }

    if((h > 1000)||(w > 1200)){
    }
    myLabel->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
    myLabel->show();
}
void MainWindow::resizeEvent(QResizeEvent *event){
    QPixmap p = QPixmap::fromImage(*img); // load pixmap
    int h = ui->ImagePlaceHolder->height();
    int w = ui->ImagePlaceHolder->width();
    if(h > p.height()) h = p.height();
    if(w > p.width()) w = p.width();
    myLabel->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
    myLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    delete graph;

    foreach(QImage* img , history){
        delete img;
    }

}

void MainWindow::on_pushButton_clicked()
{
    QImage* test = model->test(img);
    imageOperationsHistory.append("\t\t<greyscale></greyscale>");
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString line = ui->lineEdit_4->text();
    imageOperationsHistory.append("\t\t<contrast>"+line+"</contrast>");
    QImage* test = model->hardenImageContrast(img,line.toDouble());
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_3_clicked()
{
    model->saveImageAsInGreyscaleHTML(img,QCoreApplication::applicationDirPath() + "/DATA/capture",Normalization::HIGHLIGHTED);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString line = ui->lineEdit_3->text();
    if(line.toDouble() < 1.00){
        imageOperationsHistory.append("\t\t<simplify dsc=\"standard\">"+line+"</simplify>");
        QImage* test = model->simplifyImageWithFactorSoft(img,line.toDouble());
        myLabel->setPixmap(QPixmap::fromImage(*test));
        myLabel->show();

        history.append(img);
        img = test;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString line = ui->lineEdit->text();
    imageOperationsHistory.append("\t\t<pixels-removed-vertically>"+line+"</pixels-removed-vertically>");
    QImage* test = model->removeDuplicatePixelsVertically(img,line.toDouble());
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_6_clicked()
{
    QString line = ui->lineEdit_2->text();
    imageOperationsHistory.append("\t\t<pixels-removed-horizontally>"+line+"</pixels-removed-horizontally>");
    QImage* test = model->removeDuplicatePixelsHorizontally(img,line.toDouble());
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    model->incrementCurrentImageIndex();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString urlPath = QCoreApplication::applicationDirPath() + "/DATA/normal";


    OpenNN::Matrix<double>* retMatrix;
    retMatrix = model->normalizeImageIntoNormMatrix(img);
    model->saveMatrixIntoHTMLTable(retMatrix,urlPath);

    graph->setMouseTracking(true);

    for(int y = 0; y < retMatrix->get_rows_number(); y++)
    {
        for(int x = 0; x < retMatrix->get_columns_number(); x++)
        {
            GraphicsMatrixItem* item = new GraphicsMatrixItem();
            item->setModel(model);
            item->setRect( QRectF(x*30, y*30, 30, 30) );
            item->setData(0, x);
            item->setData(1, y);
            item->setToolTip("Matrix -> row: "+QString::number(y)+" column: "+QString::number(x)+" ");


            double pixel = retMatrix->operator ()(y,x);

            if(ui->checkBox->isChecked()){
                QGraphicsTextItem* fillText = new QGraphicsTextItem(QString::number(pixel,'f',2),item);
                fillText->setScale(0.7);
                fillText->setPos(x*30+2.5,y*30+7.5);
                fillText->setTextWidth(50);
                if(pixel < 0.5) fillText->setDefaultTextColor( QColor(255,255,255) );
            }

            int color = pixel*255.0;
            item->setBrush( QBrush( QColor(color,color,color) ) );

            scene->addItem(item);
        }
    }

    graph->show();

    delete retMatrix;
}
void MainWindow::on_pushButton_9_clicked()
{
    QString path = QCoreApplication::applicationDirPath() + "/DATA/";
    img->save(path+"output.png","PNG",100);
}

void MainWindow::on_pushButton_10_clicked()
{
    if(!history.isEmpty()){
        myLabel->setPixmap(QPixmap::fromImage(*history.last()));
        myLabel->show();
        imageOperationsHistory.pop_back();

        delete img;
        img = history.last();
        history.pop_back();
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
    model->decrementCurrentImageIndex();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

}

void MainWindow::on_pushButton_LearnTheOpenNN_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
    this->setFocus();
}

void MainWindow::on_pushButton_ConvertToExcel_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_GoBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void MainWindow::on_pushButton_GoBack_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}
void MainWindow::completedTableClicked()
{
    int response = QMessageBox::question(graph,"Confirmation",
                                         "Are you sure to save and proceed?",
                                         QMessageBox::Cancel,QMessageBox::Yes);
    if(response == 16384){
        QDateTime curr = QDateTime::currentDateTime();
        QString time = curr.toString().replace(":","_");

        if(!QDir("RESULTS/result-"+time).exists()){
            QDir().mkdir("RESULTS/result-"+time);
        }

        original->save(QCoreApplication::applicationDirPath()+"/RESULTS/"+"/result-"+time+"/__original_image.png","PNG",100);
        OpenNN::Matrix<double>* matrix = model->normalizeImageIntoNormMatrix(original);
        model->saveMatrix(matrix,"/result-"+time+"/__original_matrix");
        delete matrix;

        img->save(QCoreApplication::applicationDirPath()+"/RESULTS/"+"/result-"+time+"/__processed_image.png","PNG",100);
        matrix = model->normalizeImageIntoNormMatrix(img);
        model->saveMatrix(matrix,"/result-"+time+"/__processed_matrix");
        delete matrix;

        model->saveOperations("/result-"+time+"/__relevant_data",imageOperationsHistory);

        graph->close();
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    reworkDataAndTargetFiles();
    /*QString dataFile = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QCoreApplication::applicationDirPath()
                                                   ,"All files (*.*);; Open file (*.dat)");
    if(!dataFile.isEmpty())
    {
        model->initiateNeuralNetworkData(dataFile);
    }*/
}
void MainWindow::reworkDataAndTargetFiles()
{

    QString dataFile = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QCoreApplication::applicationDirPath()
                                                   ,"All files (*.*);; Open file (*.dat)");
    QString targetFile = QFileDialog::getOpenFileName(this,
                                                            tr("Open File"),
                                                            QCoreApplication::applicationDirPath()
                                                           ,"All files (*.*);; Open file (*.dat)");
    //if(!dataFile.isEmpty())
    //{
        //if(!targetFile.isEmpty())
        //{
            /*QFile dataFileHandler(dataFile);
            QFile targetFileHandler(targetFile);

            if(!dataFileHandler.open(QIODevice::ReadOnly))
                return;

            QString resultFile;
            /*while(
            (resultFile = QFileDialog::getSaveFileName(this,
                                                        tr("Save File"),
                                                        QCoreApplication::applicationDirPath()
                                                       ,"Save file (*.dat)")).isEmpty())
            {}

            QFile resultFileHandler(resultFile);*/
            /*if(!resultFileHandler.open(QIODevice::WriteOnly))
                return;

            QTextStream dataStream(&dataFileHandler);

            QTextStream resultStream(&resultFileHandler);

            QString dataLine;
            while(!dataStream.atEnd())
            {
                dataLine = dataStream.readLine();
                dataLine.remove('\n');
                if(dataLine.right(1) != " " && !dataStream.atEnd())
                    dataLine.append(' ');
                resultStream<<dataLine;
            }*/

            QFile targetFileHandler(targetFile);
            QDomDocument targetDoc;
            targetDoc.setContent(&targetFileHandler);
            targetFileHandler.close();

            QDomElement root = targetDoc.firstChildElement();
            QDomNodeList points = root.elementsByTagName("point");

            QList<int> pointsList;
            for(int i=0; i<points.count(); i++)
            {
                QDomNode node = points.at(i);
                int x = node.toElement().elementsByTagName("x").at(0).toElement().text().toInt();
                pointsList.append(x);
                int y = node.toElement().elementsByTagName("y").at(0).toElement().text().toInt();
                pointsList.append(y);
            }


            OpenNN::Matrix<double>* dataMatrix = model->createMatrixFromDataFile(dataFile);
            QList< OpenNN::Matrix<double>* > sqList = model->createSquaresFromMatrix( dataMatrix, SQUARE_A, pointsList);



            //qDebug()<<numberOfOutputs;
           // QMessageBox::information(this, "Output","Number of outputs - " + QString::number(numberOfOutputs));
       // }else
          //  QMessageBox::warning(this, "Deny!","No target file selected!");
   // }else
     //   QMessageBox::warning(this, "Deny!","No data file selected!");
}
