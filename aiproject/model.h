#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>

#include "../opennn/opennn.h"

enum class Normalization{ NO, STANDARD, HIGHLIGHTED };

class Model : public QObject
{
    Q_OBJECT
public:
    QImage* pubImage;
    Model(QObject *parent = 0);
    ~Model();
    QImage* movementSweden();
    QImage* test(QImage* img);

    void saveImageAsInGreyscaleHTML(const QImage* img_arg, const QString path_arg, Normalization normalize_arg = Normalization::NO);
    QImage* simplifyImageWithFactorSharp(const QImage* img_arg, double factor_arg);
    QImage* simplifyImageWithFactorSoft(const QImage* img_arg, double factor_arg);
    QImage* hardenImageContrast(const QImage* img_arg, const double factor_arg);
    QImage* removeDuplicatePixelsVertically(const QImage* img_arg, const double tolerance_arg);
    QImage* removeDuplicatePixelsHorizontally(const QImage* img_arg, const double tolerance_arg);
    OpenNN::Matrix<double>* normalizeImageIntoNormMatrix(const QImage* img_arg);
    void saveMatrixIntoHTMLTable(const OpenNN::Matrix<double>* matrix_arg, const QString path_arg);

private:
    QImage* privImage;
};

#endif // MODEL_H
