#ifndef ANNOTATIONITEM_H
#define ANNOTATIONITEM_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QColor>
#include <QString>
#include <QPainterPath>
#include <QPointF>

class QKeyEvent;

struct AnnotationFrame {
    QPointF origin;
    QPointF basisX;
    QPointF basisY;
};

class AnnotationItem : public QGraphicsObject {
    Q_OBJECT
public:
    enum { Type = UserType + 20 };
    int type() const override { return Type; }

    explicit AnnotationItem(QGraphicsItem* parent = nullptr);

    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject& obj);

    QString uid() const { return m_uid; }
    QColor penColor() const { return m_penColor; }
    void setPenColor(const QColor& c);
    qreal penWidth() const { return m_penWidth; }
    void setPenWidth(qreal w);
    qreal fillOpacity() const { return m_fillOpacity; }
    void setFillOpacity(qreal v);

    bool isLocked() const { return m_locked; }
    void setLocked(bool on);
    bool isItemSelected() const { return m_selected; }
    void setSelected(bool on) { m_selected = on; }

    virtual QRectF contentBounds() const = 0;
    virtual void applyStyle() = 0;
    virtual AnnotationItem* cloneItem() const = 0;

    virtual bool supportsInlineEdit() const { return false; }
    virtual bool beginInlineEdit() { return false; }
    virtual bool isInlineEditing() const { return false; }
    virtual void commitInlineEdit() {}
    virtual void cancelInlineEdit() {}
    virtual bool handleEditKey(QKeyEvent* event) { return false; }
    virtual bool placeCaretAt(const QPointF& scenePos) { Q_UNUSED(scenePos); return false; }
    virtual bool prefersCaretPlacement(const QPointF& scenePos) const { Q_UNUSED(scenePos); return false; }

    using Frame = AnnotationFrame;

    void scaleBy(qreal factor, const QPointF& pivot);
    void scaleAbout(const QPointF& scenePivot, qreal sx, qreal sy);
    Frame captureFrame() const;
    void applyFrame(const Frame& frame);
    void scaleFrameAbout(const Frame& start, const QPointF& scenePivot, qreal sx, qreal sy);

    void notifyContentChanged();

    virtual bool isHighlighter() const { return false; }
    virtual QPainterPath inkPath() const { return QPainterPath(); }
    QPainterPath sceneShape() const { return mapToScene(shape()); }
    virtual bool hitTest(const QPointF& scenePos, qreal radius) const;
    virtual bool borderHitTest(const QPointF& scenePos, qreal tolerance) const;
    void drawSelectionFrame(QPainter* painter) const;

    static QJsonObject colorToJson(const QColor& c);
    static QColor colorFromJson(const QJsonValue& v, const QColor& fallback);

signals:
    void contentChanged();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    QString m_uid;
    QColor m_penColor = QColor(31, 39, 51);
    qreal m_penWidth = 3.0;
    qreal m_fillOpacity = 1.0;
    bool m_locked = false;
    bool m_selected = false;
};

#endif
