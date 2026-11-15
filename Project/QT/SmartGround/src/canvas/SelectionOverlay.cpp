#include "SelectionOverlay.h"
#include "AnnotationItem.h"
#include "ToolController.h"
#include <QPainter>
#include <cmath>

SelectionOverlay::SelectionOverlay(ToolController* controller, QObject* parent)
    : QObject(parent), m_controller(controller) {}

void SelectionOverlay::setItems(const QVector<AnnotationItem*>& items) {
    m_items = items;
}

void SelectionOverlay::clear() {
    m_items.clear();
    m_scaling = false;
    m_handle = None;
}

QRectF SelectionOverlay::sceneBounds() const {
    QRectF bounds;
    for (AnnotationItem* item : m_items) {
        if (!item) continue;
        const QRectF r = item->mapRectToScene(item->contentBounds());
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
    return bounds;
}

SelectionOverlay::Handle SelectionOverlay::handleAt(const QPointF& scenePos, qreal tolerance) const {
    if (m_items.isEmpty()) return None;
    const QRectF b = sceneBounds();
    if (b.isEmpty()) return None;
    static const Handle handles[] = {
        TopLeft, TopCenter, TopRight,
        MiddleRight, BottomRight, BottomCenter,
        BottomLeft, MiddleLeft
    };
    for (Handle h : handles) {
        const QPointF p = handlePoint(h);
        const qreal dx = scenePos.x() - p.x();
        const qreal dy = scenePos.y() - p.y();
        if (std::hypot(dx, dy) <= tolerance) return h;
    }
    return None;
}

QPointF SelectionOverlay::handlePoint(Handle handle) const {
    const QRectF b = sceneBounds();
    const qreal cx = b.center().x();
    const qreal cy = b.center().y();
    switch (handle) {
    case TopLeft: return b.topLeft();
    case TopCenter: return QPointF(cx, b.top());
    case TopRight: return b.topRight();
    case MiddleRight: return QPointF(b.right(), cy);
    case BottomRight: return b.bottomRight();
    case BottomCenter: return QPointF(cx, b.bottom());
    case BottomLeft: return b.bottomLeft();
    case MiddleLeft: return QPointF(b.left(), cy);
    default: return QPointF();
    }
}

void SelectionOverlay::beginScale(const QPointF& scenePos, Handle handle) {
    if (m_items.isEmpty() || handle == None) return;
    m_scaling = true;
    m_handle = handle;
    m_scaleAnchor = scenePos;
    m_startBounds = sceneBounds();
    m_startFrames.clear();
    for (AnnotationItem* item : m_items) {
        if (item) m_startFrames.insert(item, item->captureFrame());
    }
}

void SelectionOverlay::updateScale(const QPointF& scenePos) {
    if (!m_scaling || !m_controller) return;
    const QRectF b = m_startBounds;
    if (b.width() < 1.0 || b.height() < 1.0) return;

    qreal sx = 1.0;
    qreal sy = 1.0;
    QPointF anchor = b.topLeft();

    switch (m_handle) {
    case TopLeft:
        sx = (b.right() - scenePos.x()) / b.width();
        sy = (b.bottom() - scenePos.y()) / b.height();
        anchor = b.bottomRight();
        break;
    case TopCenter:
        sy = (b.bottom() - scenePos.y()) / b.height();
        anchor = QPointF(b.center().x(), b.bottom());
        break;
    case TopRight:
        sx = (scenePos.x() - b.left()) / b.width();
        sy = (b.bottom() - scenePos.y()) / b.height();
        anchor = b.bottomLeft();
        break;
    case MiddleRight:
        sx = (scenePos.x() - b.left()) / b.width();
        anchor = QPointF(b.left(), b.center().y());
        break;
    case BottomRight:
        sx = (scenePos.x() - b.left()) / b.width();
        sy = (scenePos.y() - b.top()) / b.height();
        anchor = b.topLeft();
        break;
    case BottomCenter:
        sy = (scenePos.y() - b.top()) / b.height();
        anchor = QPointF(b.center().x(), b.top());
        break;
    case BottomLeft:
        sx = (b.right() - scenePos.x()) / b.width();
        sy = (scenePos.y() - b.top()) / b.height();
        anchor = b.topRight();
        break;
    case MiddleLeft:
        sx = (b.right() - scenePos.x()) / b.width();
        anchor = QPointF(b.right(), b.center().y());
        break;
    default:
        return;
    }

    if (sx < 0.05 || sy < 0.05) return;
    m_controller->scaleSelectionFramesAbout(m_startFrames, anchor, sx, sy);
}

void SelectionOverlay::finishScale() {
    m_scaling = false;
    m_handle = None;
    m_startFrames.clear();
}

void SelectionOverlay::paint(QPainter* painter) const {
    if (m_items.isEmpty()) return;
    const QRectF b = sceneBounds();
    if (b.isEmpty()) return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QPen frame(QColor(59, 125, 221), 1.2);
    frame.setCosmetic(true);
    painter->setPen(frame);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(b);

    QPen handlePen(QColor(59, 125, 221), 1.4);
    handlePen.setCosmetic(true);
    painter->setPen(handlePen);
    painter->setBrush(QColor(255, 255, 255));

    static const Handle handles[] = {
        TopLeft, TopCenter, TopRight,
        MiddleRight, BottomRight, BottomCenter,
        BottomLeft, MiddleLeft
    };
    for (Handle h : handles) {
        const QPointF p = handlePoint(h);
        painter->drawRect(QRectF(p.x() - 3.5, p.y() - 3.5, 7, 7));
    }
    painter->restore();
}
