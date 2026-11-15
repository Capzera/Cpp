#ifndef SELECTIONOVERLAY_H
#define SELECTIONOVERLAY_H
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QVector>
#include <QHash>

#include "AnnotationItem.h"

class ToolController;
class QPainter;

class SelectionOverlay : public QObject {
    Q_OBJECT
public:
    enum Handle {
        None,
        TopLeft,
        TopCenter,
        TopRight,
        MiddleRight,
        BottomRight,
        BottomCenter,
        BottomLeft,
        MiddleLeft
    };

    explicit SelectionOverlay(ToolController* controller, QObject* parent = nullptr);

    void setItems(const QVector<AnnotationItem*>& items);
    void clear();
    bool isActive() const { return !m_items.isEmpty(); }

    QRectF sceneBounds() const;
    Handle handleAt(const QPointF& scenePos, qreal tolerance) const;
    QPointF handlePoint(Handle handle) const;

    void beginScale(const QPointF& scenePos, Handle handle);
    void updateScale(const QPointF& scenePos);
    void finishScale();
    bool isScaling() const { return m_scaling; }

    void paint(QPainter* painter) const;

private:
    ToolController* m_controller = nullptr;
    QVector<AnnotationItem*> m_items;
    bool m_scaling = false;
    Handle m_handle = None;
    QPointF m_scaleAnchor = QPointF();
    QRectF m_startBounds;
    QHash<const AnnotationItem*, AnnotationFrame> m_startFrames;
};

#endif
