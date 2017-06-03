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
    privImage = new QImage(desktopPath + "colorsample.jpg");
    QImage::Format format = privImage->format();

    return privImage;
}
Model::~Model()
{
}
QImage* Model::test(QImage* img)
{
    //if(img != nullptr) delete img;

    QString desktopPath = QCoreApplication::applicationDirPath() + "/DATA/";
    //img = new QImage(desktopPath + "threeblackcross.jpg");

    QSize rect = img->size();
    for(int y = 0; y < rect.height(); y++)
    {
        for(int x = 0; x < rect.width(); x++)
        {
            QRgb pix = img->pixel(x,y);
            int g = qGray(pix);
            img->setPixelColor(x,y,QColor(g, g, g).rgb());
        }
    }
    QImage* newImg = this->simplifyImageWithFactorSharp(img,0.75);
    delete img;

    this->saveImageAsInGreyscaleHTML(newImg,QCoreApplication::applicationDirPath() + "/DATA/capture",Normalization::HIGHLIGHTED);

    return newImg;
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
