#ifndef SHAPEITEM_H
#define SHAPEITEM_H
#include "AnnotationItem.h"
#include <QPolygonF>
#include <QVector>

class ShapeItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 22 };
    enum Kind { Line, Rectangle, Ellipse, Arrow, Triangle, Mosaic };
    Q_ENUM(Kind)

    int type() const override { return Type; }

    ShapeItem(Kind kind, QGraphicsItem* parent = nullptr);

    Kind kind() const { return m_kind; }
    void setKind(Kind k);

    void setGeometry(const QPointF& start, const QPointF& end);
    QPointF startPoint() const { return m_start; }
    QPointF endPoint() const { return m_end; }

    void setMosaicCells(const QVector<QPolygonF>& cells);
    const QVector<QPolygonF>& mosaicCells() const { return m_cells; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

private:
    QPainterPath buildOutline() const;
    QPolygonF arrowPolygon(const QPointF& from, const QPointF& to) const;

    Kind m_kind = Line;
    QPointF m_start;
    QPointF m_end;
    QVector<QPolygonF> m_cells;
};

#endif
