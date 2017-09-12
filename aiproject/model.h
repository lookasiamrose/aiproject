#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>

#include <math.h>
#include <time.h>
#include "../opennn/opennn.h"

using namespace OpenNN;

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
    void incrementCurrentImageIndex();
    void saveOperations(QString time_arg, QStringList& image_operations_history_arg);
    void saveMatrix(OpenNN::Matrix<double>* matrix_arg, QString path_arg);
    void decrementCurrentImageIndex();
    OpenNN::Matrix<double>* createMatrixFromDataFile(QString path);
    QList< OpenNN::Matrix<double>* > createSquaresFromMatrix(OpenNN::Matrix<double>* matrix, int squareA);

    class HelperOperationsWithin
    {
    public:
        QList<QString> chosenCordsPairs;
        QList<QString> chosenImageOperations;
    };

    void initiateNeuralNetworkData(QString dataFileNameArg);

private:
    QImage* privImage;
    int currentImageIndex;
    QList<HelperOperationsWithin> operationsHistory;

    //Neural network
    DataSet* dataSet;
    NeuralNetwork* neuralNetwork;
    LossIndex* lossIndex;
    TrainingStrategy* trainingStrategy;
public slots:
    void collectItemPoint(int, int);
};

#endif // MODEL_H
