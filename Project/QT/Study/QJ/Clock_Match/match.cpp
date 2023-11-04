#include "match.h"
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <qdebug.h>

MyItem::MyItem() {
    brushColor = Qt::red;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}
void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (hasFocus()) {
        painter->setPen(QPen(QColor(255, 255, 255, 200)));
    }
    else {
        painter->setPen(QPen(QColor(100, 100, 100, 100)));
    }
    painter->setBrush(brushColor);
    //painter->drawRect(-10, -10, 20, 20);

    // 定义四个点
    static const QPointF points[4] = {
    QPointF(200.0, 80.0),
    QPointF(220.0, 10.0),
    QPointF(270.0, 30.0),
    QPointF(310.0, 70.0)
    };
    // 使用四个点绘制多边形
    painter->drawPolygon(points, 4);

}

QRectF MyItem::boundingRect() const {
    qreal adjust = 0.5;
    return QRectF(-10 - adjust, -10 - adjust,
                  20 + adjust, 20 + adjust);
}
