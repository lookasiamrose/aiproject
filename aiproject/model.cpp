#include "model.h"
#include <QCoreApplication>
#include <QDebug>
#include <QColor>
#include <QFile>

Model::Model(QObject *parent) : QObject(parent)
{

}
QImage* Model::movementSweden()
{
    QString desktopPath = QCoreApplication::applicationDirPath() + "/DATA/";
    privImage = new QImage(desktopPath + "threeblackcross.jpg");
    //QImage::Format format = privImage->format();

    return privImage;
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
    return grayed;
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
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        for(int i=0;i<doc.size();i++)
        {
            stream<<doc.at(i)<<endl;
        }
    }

}
