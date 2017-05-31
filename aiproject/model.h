#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>

class Model : public QObject
{
    Q_OBJECT
    enum class Normalization{ NO, STANDARD, HIGHLIGHTED };
public:
    QImage* pubImage;
    Model(QObject *parent = 0);
    ~Model();
    QImage* movementSweden();
    QImage* test(QImage* img);

    void saveImageAsInGreyscaleHTML(const QImage* img_arg, const QString path_arg, Normalization normalize_arg = Normalization::NO);

private:
    QImage* privImage;
};

#endif // MODEL_H
