#include "graphicsmatrixitem.h"
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QObject>

GraphicsMatrixItem::GraphicsMatrixItem(QGraphicsItem * parent) : QObject(), QGraphicsRectItem(parent)
{
    setAcceptHoverEvents(true);
    defaultBrush = this->brush();
}
void GraphicsMatrixItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    myBrush = this->brush();
    this->setBrush( QBrush(Qt::blue) );
}
void GraphicsMatrixItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    this->setBrush( myBrush );
}
void GraphicsMatrixItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    emit pushPointToModel(data(1).toInt(),data(0).toInt());
    if( myBrush != QBrush(Qt::green) )
    {
        myBrush = QBrush(Qt::green);
    }else{
        myBrush = defaultBrush;
    }
    this->setBrush( myBrush ); //REPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIR
}
void GraphicsMatrixItem::setModel(Model* model_arg)
{
    model = model_arg;
    QObject::connect( this, SIGNAL(pushPointToModel(int,int)) , model, SLOT(collectItemPoint(int,int)) );
}
