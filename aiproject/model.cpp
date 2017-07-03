#include "model.h"
#include <QCoreApplication>
#include <QDebug>
#include <QColor>
#include <QFile>

Model::Model(QObject *parent) : QObject(parent)
{
    currentImageIndex = -1;
}
QImage* Model::movementSweden()
{
    QString desktopPath = QCoreApplication::applicationDirPath() + "/DATA/";
    privImage = new QImage(desktopPath + "350x150.png");
    return privImage;
}
void Model::incrementCurrentImageIndex()
{
    currentImageIndex++;
    HelperOperationsWithin helper;
    operationsHistory.push_front(helper);
}
void Model::decrementCurrentImageIndex()
{
    if(!operationsHistory.isEmpty())
    {
        currentImageIndex--;
        operationsHistory.pop_front();
    }
}
void Model::collectItemPoint(int r, int c)
{
    if(currentImageIndex >= 0){
        QString cords = "\t<point>\n\t\t<x dsc=\"matrix column\">"+QString::number(c)+"</x>\n\t\t<y dsc=\"matrix row\">"+QString::number(r)+"</y>\n\t</point>";
        if( !operationsHistory[currentImageIndex].chosenCordsPairs.contains(cords) ){
            operationsHistory[currentImageIndex].chosenCordsPairs.append(cords);
        }else{
            operationsHistory[currentImageIndex].chosenCordsPairs.removeOne(cords);
        }
    }else{
        qDebug()<<"Image history has not been initialized!";
    }
}
void Model::saveOperations(QString time_arg, QStringList& image_operations_history_arg)
{
    QStringList doc;
    QString path = QCoreApplication::applicationDirPath() + "/RESULTS/";
    path += time_arg + ".xml";

    doc.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    doc.append("<results>");
    foreach( QString item, operationsHistory.first().chosenCordsPairs)
    {
       doc.append(item);
    }
    doc.append("</results>");

    doc.append("<operations>");
    doc.append(image_operations_history_arg);
    doc.append("</operations>");

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for(int i=0;i<doc.size();i++)
        {
            stream<<doc.at(i)<<endl;
        }
        file.close();
    }else{
        qDebug()<<file.errorString();
    }
}
Model::~Model()
{
}
QImage* Model::test(QImage* img)
{
    QImage* grayed = new QImage(*img);
    QSize rect = grayed->size();
    for(int y = 0; y < rect.height(); y++)
    {
        for(int x = 0; x < rect.width(); x++)
        {
            QRgb pix = grayed->pixel(x,y);
            int g = qGray(pix);
            grayed->setPixelColor(x,y,QColor(g, g, g).rgb());
        }
    }

    /*OpenNN::Matrix<double>* retMatrix;
    retMatrix = normalizeImageIntoNormMatrix(grayed);
    saveMatrixIntoHTMLTable(retMatrix,QCoreApplication::applicationDirPath() + "/DATA/normal");
    delete retMatrix;*/

    return grayed;
}
OpenNN::Matrix<double>* Model::normalizeImageIntoNormMatrix(const QImage* img_arg)
{
    OpenNN::Matrix<double>* matrix = new OpenNN::Matrix<double>(img_arg->height(),img_arg->width());
    for(int y = 0; y < img_arg->height(); y++)
    {
        OpenNN::Vector<double> row;
        for(int x = 0; x < img_arg->width(); x++)
        {
            double grayScale = (double) qGray(img_arg->pixel(x,y));
            grayScale /= 255.0; //max value division, 0-255 -> 0.0 - 1.0
            row = row.insert_element(x,grayScale);
        }
        matrix->set_row(y,row);
    }
    return matrix;
}
void Model::saveMatrix(OpenNN::Matrix<double>* matrix_arg, QString path_arg)
{
    QString path = QCoreApplication::applicationDirPath() + "/RESULTS/";
    QFile file(path + path_arg + ".dat");
    file.resize(0);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for(int y = 0; y < matrix_arg->get_rows_number(); y++)
        {
            QString row;
            for(int x = 0; x < matrix_arg->get_columns_number(); x++)
            {
               row += QString::number( matrix_arg->operator ()(y,x) ,'f',2) + " ";
            }
            row.left(row.length()-2);
            stream<<row<<endl;
        }
        file.close();
    }
}
QImage* Model::removeDuplicatePixelsVertically(const QImage* img_arg, const double tolerance_arg)
{
    QImage* imgResult = nullptr;

    QList<QRgb*> rows;
    for(int y = 0; y < img_arg->height(); y++)
    {   QRgb* row = new QRgb[img_arg->width()];
        for(int x = 0; x < img_arg->width(); x++)
        {
            row[x] = img_arg->pixel(x,y);
        }
        rows.append(row);
    }

    if(rows.size() > 1)
    {
        int* rowVal = new int[rows.size()];
        for(int i = 0; i < rows.size(); i++) rowVal[i] = 0;

        for(int y = 0; y < rows.size(); y++)
        {
            for(int x = 0; x < img_arg->width(); x++)
            {
                rowVal[y] += qGray(rows.at(y)[x]);
            }
            rowVal[y] /= img_arg->width();
        }

        QList<int> toDelete;
        for(int y = 1; y < rows.size(); y++)
        {
            if(qAbs( (double) rowVal[y] - (double) rowVal[y-1]) < (tolerance_arg*255.0))
            {
                toDelete.append(y);
            }
        }

        for(int i = 0; i < toDelete.size(); i++)
        {
            delete rows.at(toDelete.at(i));
            rows[toDelete.at(i)] = nullptr;
        }

        rows.removeAll(nullptr);
        imgResult = new QImage(img_arg->width(),rows.size(),QImage::Format::Format_RGB32);
        for(int y = 0; y < imgResult->height(); y++)
        {
            for(int x = 0; x < imgResult->width(); x++)
            {
                imgResult->setPixel(x,y,rows.at(y)[x]);
            }
        }
    }

    return imgResult;
}
QImage* Model::removeDuplicatePixelsHorizontally(const QImage* img_arg, const double tolerance_arg)
{
    QImage* imgResult = nullptr;

    QList<QRgb*> columns;
    for(int x = 0; x < img_arg->width(); x++)
    {   QRgb* column = new QRgb[img_arg->height()];
        for(int y = 0; y < img_arg->height(); y++)
        {
            column[y] = img_arg->pixel(x,y);
        }
        columns.append(column);
    }

    if(columns.size() > 1)
    {
        int* colVal = new int[columns.size()];
        for(int i = 0; i < columns.size(); i++) colVal[i] = 0;

        for(int x = 0; x < columns.size(); x++)
        {
            for(int y = 0; y < img_arg->height(); y++)
            {
                colVal[x] += qGray(columns.at(x)[y]);
            }
            colVal[x] /= img_arg->height();
        }

        QList<int> toDelete;
        for(int x = 1; x < columns.size(); x++)
        {
            if(qAbs( (double) colVal[x] - (double) colVal[x-1]) < (tolerance_arg*255.0))
            {
                toDelete.append(x);
            }
        }

        for(int i = 0; i < toDelete.size(); i++)
        {
            delete columns.at(toDelete.at(i));
            columns[toDelete.at(i)] = nullptr;
        }

        columns.removeAll(nullptr);
        imgResult = new QImage(columns.size(),img_arg->height(),QImage::Format::Format_RGB32);
        for(int x = 0; x < imgResult->width(); x++)
        {
            for(int y = 0; y < imgResult->height(); y++)
            {
                imgResult->setPixel(x,y,columns.at(x)[y]);
            }
        }
    }

    return imgResult;
}
QImage* Model::simplifyImageWithFactorSharp(const QImage* img_arg, double factor_arg)
{
    int heightFactored = qRound((double) img_arg->height()*factor_arg);
    int widthFactored = qRound((double) img_arg->width()*factor_arg);
    QImage* imgSimplified = new QImage(widthFactored,heightFactored,QImage::Format::Format_RGB32);

    double iterator = static_cast<double>(static_cast<int>(100/factor_arg+0.5))/100.0;
    for(int y = 0; y < imgSimplified->height(); y++)
    {
        for(int x = 0; x < imgSimplified->width(); x++)
        {
            double secondX = x*iterator;
            double secondY = y*iterator;
            (secondX >= img_arg->width()) ? secondX = img_arg->width()-1 : secondX = secondX;
            (secondY >= img_arg->height()) ? secondY = img_arg->height()-1 : secondY = secondY;

            int singleValPixel = qRed(img_arg->pixel(secondX,secondY));
            imgSimplified->setPixel(x,y,QColor(singleValPixel, singleValPixel, singleValPixel).rgb());
        }
    }

    return imgSimplified;
}
QImage* Model::simplifyImageWithFactorSoft(const QImage* img_arg, double factor_arg)
{
    int heightFactored = qRound((double) img_arg->height()*factor_arg);
    int widthFactored = qRound((double) img_arg->width()*factor_arg);
    QImage* imgSimplified = new QImage(widthFactored,heightFactored,QImage::Format::Format_RGB32);

    double iterator = static_cast<double>(static_cast<int>(100/factor_arg+0.5))/100.0;
    for(int y = 0; y < imgSimplified->height(); y++)
    {
        for(int x = 0; x < imgSimplified->width(); x++)
        {
            double halfIterator = iterator*0.5;
            double secondX = x*iterator;
            double secondY = y*iterator;

            int pixelCounter = 0;
            int pixelSum = 0;
            int pixelSumG = 0;
            int pixelSumB = 0;
            for(int ycorner = (secondY-halfIterator);ycorner<(secondY+halfIterator);ycorner++)
            {
                for(int xcorner = (secondX-halfIterator);xcorner<(secondX+halfIterator);xcorner++)
                {
                    if(xcorner >= img_arg->width()) xcorner = img_arg->width()-1;
                    else if(xcorner < 0) xcorner = 0;
                    if(ycorner >= img_arg->height()) ycorner = img_arg->height()-1;
                    else if(ycorner < 0) ycorner = 0;

                    pixelSum += qRed(img_arg->pixel(xcorner,ycorner));
                    pixelSumG += qGreen(img_arg->pixel(xcorner,ycorner));
                    pixelSumB += qBlue(img_arg->pixel(xcorner,ycorner));
                    pixelCounter++;
                }
            }
            if(pixelCounter == 0)
            {
                (secondX >= img_arg->width()) ? secondX = img_arg->width()-1 : secondX = secondX;
                (secondY >= img_arg->height()) ? secondY = img_arg->height()-1 : secondY = secondY;
                pixelSum = qRed(img_arg->pixel(secondX,secondY));
                pixelSumG = qGreen(img_arg->pixel(secondX,secondY));
                pixelSumB = qBlue(img_arg->pixel(secondX,secondY));
            }else
            {
                pixelSum /= pixelCounter;
                pixelSumG /= pixelCounter;
                pixelSumB /= pixelCounter;
            }

            imgSimplified->setPixel(x,y,QColor(pixelSum, pixelSumG, pixelSumB).rgb());
        }
    }

    return imgSimplified;
}
QImage* Model::hardenImageContrast(const QImage* img_arg, const double factor_arg)
{
    QImage* imgHarden = new QImage(*img_arg);

    for(int y = 0; y < imgHarden->height(); y++)
    {
        for(int x = 0; x < imgHarden->width(); x++)
        {
            QRgb pixel = imgHarden->pixel(x,y);

            int absRed = qRed(pixel);
            if(qRed(pixel)-127.5 < 0){
                double red = qRed(pixel);
                red *= factor_arg;
                absRed -= red;
            }else{
                double red = 255.0 - qRed(pixel);
                red *= factor_arg;
                absRed += red;
            }

            int absGreen = qGreen(pixel);
            if(qGreen(pixel)-127.5 < 0){
                double green = qGreen(pixel);
                green *= factor_arg;
                absGreen -= green;
            }else{
                double green = 255.0 - qGreen(pixel);
                green *= factor_arg;
                absGreen += green;
            }

            int absBlue = qBlue(pixel);
            if(qBlue(pixel)-127.5 < 0){
                double blue = qBlue(pixel);
                blue *= factor_arg;
                absBlue -= blue;
            }else{
                double blue = 255.0 - qBlue(pixel);
                blue *= factor_arg;
                absBlue += blue;
            }

            imgHarden->setPixel(x,y,QColor(absRed, absGreen, absBlue).rgb());
        }
    }

    return imgHarden;
}
void Model::saveImageAsInGreyscaleHTML(const QImage* img_arg, const QString path_arg, Normalization normalize_arg)
{
    QStringList doc;
    doc.append(QString("<!DOCTYPE html>"));
    doc.append(QString("<head>"));
    doc.append(QString("<style>"));
    doc.append(QString("td{ border: 1px solid #333; }"));
    doc.append(QString(".highlighted{ background-color: #AED1E6; }"));
    doc.append(QString("</style>"));
    doc.append(QString("</head>"));

    doc.append(QString("<body>"));
    doc.append(QString("<table style=\"border-collapse: collapse; text-align: center;\">"));

    for(int y = 0; y < img_arg->height(); y++)
    {
        QString row = "<tr>\n";
        for(int x = 0; x < img_arg->width(); x++)
        {
            QRgb pixel = img_arg->pixel(x,y);
            int grey = qGray(pixel);
            row += "<td>";
            switch(normalize_arg)
            {
                case Normalization::HIGHLIGHTED:
                {
                    double realGrey = (double) grey;
                    double limit = 255.0;
                    realGrey /= limit;
                    if(realGrey < 0.51)
                    {
                        row = row.left(row.length()-1);
                        row += " class=\"highlighted\">";
                    }
                    row += QString::number(realGrey,'f',2);
                }
                    break;
                case Normalization::STANDARD:
                {
                    double realGrey = (double) grey;
                    double limit = 255.0;
                    row += QString::number(realGrey/limit,'f',2);
                }
                    break;
                case Normalization::NO:
                {
                    row += QString::number(grey);
                }
                    break;
                default:
                    break;
            }
            row += "</td>\n";
        }
        doc.append(row += "</tr>");
    }

    doc.append(QString("</table>"));
    doc.append(QString("</body>"));

    QFile file(path_arg + ".html");
    file.resize(0);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        for(int i=0;i<doc.size();i++)
        {
            stream<<doc.at(i)<<endl;
        }
    }

}
void Model::saveMatrixIntoHTMLTable(const OpenNN::Matrix<double>* matrix_arg, const QString path_arg)
{
    QStringList doc;
    doc.append(QString("<!DOCTYPE html>"));
    doc.append(QString("<head>"));
    doc.append(QString("<style>"));
    doc.append(QString("td{ border: 1px solid #333; }"));
    doc.append(QString(".highlighted{ background-color: #AED1E6; }"));
    doc.append(QString("</style>"));
    doc.append(QString("</head>"));

    doc.append(QString("<body>"));
    doc.append(QString("<table style=\"border-collapse: collapse; text-align: center;\">"));

    for(int y = 0; y < matrix_arg->get_rows_number(); y++)
    {
        QString row = "<tr>\n";
        for(int x = 0; x < matrix_arg->get_columns_number(); x++)
        {
            double pixel = matrix_arg->operator ()(y,x);
            row += "<td>";
            if(pixel < 0.51)
            {
                row = row.left(row.length()-1);
                row += " class=\"highlighted\">";
            }
            row += QString::number(pixel,'f',2);
            row += "</td>\n";
        }
        doc.append(row += "</tr>");
    }

    doc.append(QString("</table>"));
    doc.append(QString("</body>"));

    QFile file(path_arg + ".html");
    file.resize(0);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        for(int i=0;i<doc.size();i++)
        {
            stream<<doc.at(i)<<endl;
        }
    }

}
