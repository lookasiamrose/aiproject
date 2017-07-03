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
    img = new QImage(url.toLocalFile());

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
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString line = ui->lineEdit_4->text();
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
    QImage* test = model->removeDuplicatePixelsVertically(img,line.toDouble());
    myLabel->setPixmap(QPixmap::fromImage(*test));
    myLabel->show();

    history.append(img);
    img = test;
}

void MainWindow::on_pushButton_6_clicked()
{
    QString line = ui->lineEdit_2->text();
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
void MainWindow::completedTableClicked()
{
    qDebug()<<"completed";
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

        delete img;
        img = history.last();
        history.pop_back();
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
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
