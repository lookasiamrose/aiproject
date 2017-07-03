#ifndef GRAPHICSMATRIXITEM_H
#define GRAPHICSMATRIXITEM_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "model.h"

class GraphicsMatrixItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Model* model;
public:
    GraphicsMatrixItem(QGraphicsItem * parent = 0);
    QBrush myBrush;
    QBrush defaultBrush;
    void setModel(Model* model_arg);
public slots:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
signals:
    void pushPointToModel(int,int);
};

class GraphicsMatrixItemSingleton
{
    GraphicsMatrixItemSingleton();
    GraphicsMatrixItemSingleton( const GraphicsMatrixItemSingleton& );
public:
    static GraphicsMatrixItemSingleton& getSingleton()
    {
        static GraphicsMatrixItemSingleton singleton;
        return singleton;
    }

    void pushResult() {}
};

#endif // GRAPHICSMATRIXITEM_H
