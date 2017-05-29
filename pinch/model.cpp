#include "model.h"
#include <QDebug>
#include <QColor>
#include <QFile>

Model::Model(QObject *parent) : QObject(parent)
{

}
QImage* Model::movementSweden()
{
    QString desktopPath = "C:/Users/lukasz.jamroz/Desktop/";
    privImage = new QImage(desktopPath + "colorsample.jpg");
    QImage::Format format = privImage->format();

    return privImage;
}
Model::~Model()
{
}
QImage* Model::test(QImage* img)
{
    if(img != nullptr) delete img;

    QString desktopPath = "C:/Users/lukasz.jamroz/Desktop/";
    img = new QImage(desktopPath + "threeblackcross.jpg");

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

    this->saveImageAsInGreyscaleHTML(img,"C:/Users/lukasz.jamroz/Desktop/img",Normalization::HIGHLIGHTED);

    return img;
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
