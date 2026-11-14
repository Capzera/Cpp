#ifndef STROKEITEM_H
#define STROKEITEM_H
#include "AnnotationItem.h"
#include <QPainterPath>
#include <QVector>
#include <QPolygonF>

class StrokeItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 21 };
    int type() const override { return Type; }

    enum Mode { Ink, Highlighter };
    Q_ENUM(Mode)

    explicit StrokeItem(QGraphicsItem* parent = nullptr);

    void addPoint(const QPointF& p);
    void setPoints(const QVector<QPointF>& pts);
    QVector<QPointF> points() const { return m_points; }

    Mode mode() const { return m_mode; }
    void setMode(Mode m);

    bool isHighlighter() const override { return m_mode == Highlighter; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    bool hitTest(const QPointF& scenePos, qreal radius) const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    QPainterPath buildPath() const;
    QPainterPath buildCenterPath() const;
    QVector<QPointF> smoothPoints() const;
    void rebuild();

    QVector<QPointF> m_points;
    QVector<QPointF> m_smooth;
    QPainterPath m_path;
    QPainterPath m_center;
    Mode m_mode = Ink;
};

#endif
