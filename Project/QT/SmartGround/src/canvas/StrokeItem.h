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
    QPainterPath inkPath() const override { return m_center; }
    AnnotationItem* cloneItem() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    void appendSegment(QPainterPath& path, int index) const;
    void rebuildTail();
    void resampleAll();
    void resampleSegment(const QPointF& from, const QPointF& to);

    QVector<QPointF> m_points;
    QVector<QPointF> m_resampled;
    QPainterPath m_prefix;
    QPainterPath m_center;
    int m_cachedSegments = 0;
    Mode m_mode = Ink;
};

#endif
