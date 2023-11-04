#ifndef MATCH_H
#define MATCH_H

#include <QGraphicsItem>

class MyItem : public QGraphicsItem {
public:
    MyItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setColor(const QColor &color) { brushColor = color; }
private:
    QColor brushColor;
};

#endif // MATCH_H
