#include "CanvasView.h"
#include "AnnotationScene.h"
#include "ToolController.h"
#include "SelectionOverlay.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <cmath>

CanvasView::CanvasView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    setObjectName("canvasView");
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setFocusPolicy(Qt::StrongFocus);
    viewport()->setFocusPolicy(Qt::StrongFocus);
    viewport()->installEventFilter(this);
}

void CanvasView::focusInEvent(QFocusEvent* event) {
    viewport()->setFocus();
    QGraphicsView::focusInEvent(event);
}

void CanvasView::setAnnotationScene(AnnotationScene* scene) {
    m_annotationScene = scene;
    if (m_annotationScene) m_annotationScene->setHostView(this);
    refreshOverlay();
}

void CanvasView::setToolController(ToolController* controller) {
    m_toolController = controller;
    syncDragMode();
}

void CanvasView::syncDragMode() {
    if (!m_toolController) {
        setDragMode(QGraphicsView::ScrollHandDrag);
        return;
    }
    if (m_toolController->currentTool() == ToolController::Select) {
        setDragMode(QGraphicsView::ScrollHandDrag);
    } else {
        setDragMode(QGraphicsView::NoDrag);
    }
    viewport()->setCursor(m_toolController->currentTool() == ToolController::Select
                             ? Qt::OpenHandCursor
                             : Qt::CrossCursor);
}

void CanvasView::refreshOverlay() {
    if (m_annotationScene) viewport()->update();
}

QPointF CanvasView::mapToSceneF(const QPointF& viewportPos) const {
    bool invertible = false;
    const QTransform inverse = viewportTransform().inverted(&invertible);
    if (!invertible) return mapToScene(viewportPos.toPoint());
    return inverse.map(viewportPos);
}

QPixmap CanvasView::captureSceneRegion(const QRectF& region) const {
    if (region.isEmpty()) return QPixmap();
    const QPointF topLeft = mapToScene(QPoint(0, 0));
    const QPointF bottomRight = mapToScene(QPoint(width(), height()));
    const QRectF viewRect{topLeft, bottomRight};
    const QRectF clipped = region.intersected(viewRect.normalized());
    if (clipped.isEmpty()) return QPixmap();

    const QSize size = clipped.size().toSize();
    if (size.isEmpty()) return QPixmap();

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    if (scene()) scene()->render(&painter, clipped, clipped, Qt::KeepAspectRatio);
    painter.end();
    return pixmap;
}

void CanvasView::drawForeground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawForeground(painter, rect);
    if (!m_annotationScene) return;
    const QRectF items = m_annotationScene->itemsBoundingRect();
    if (!items.isEmpty()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setRenderHint(QPainter::TextAntialiasing);
        const QRectF src = items.adjusted(-40, -40, 40, 40);
        m_annotationScene->render(painter, src, src, Qt::IgnoreAspectRatio);
        painter->restore();
    }
    if (m_toolController) {
        const QRectF marquee = m_toolController->marqueeRect();
        if (!marquee.isNull() && marquee.width() > 0.5 && marquee.height() > 0.5) {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QPen pen(QColor(59, 125, 221), 1.3, Qt::DashLine);
            pen.setDashPattern({5.0, 4.0});
            pen.setCosmetic(true);
            painter->setPen(pen);
            painter->setBrush(QColor(59, 125, 221, 28));
            painter->drawRect(marquee);
            painter->restore();
        }
        if (m_toolController->eraserCursorActive()) {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            const QPointF c = m_toolController->eraserCursorPos();
            const qreal r = m_toolController->eraserRadius();
            QPen ring(QColor(229, 72, 77), 1.6);
            ring.setCosmetic(true);
            painter->setPen(ring);
            painter->setBrush(QColor(229, 72, 77, 36));
            painter->drawEllipse(c, r, r);
            painter->restore();
        }
        if (m_toolController) {
            SelectionOverlay* overlay = m_toolController->selectionOverlay();
            if (overlay && overlay->isActive()) {
                painter->save();
                overlay->paint(painter);
                painter->restore();
            }
        }
    }
}

bool CanvasView::eventFilter(QObject* obj, QEvent* event) {
    if (obj != viewport()) return QGraphicsView::eventFilter(obj, event);

    if (event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        const double angle = wheelEvent->angleDelta().y();
        const double factor = std::pow(1.0015, angle);
        const double currentScale = transform().m11();
        if ((currentScale < 0.2 && factor < 1.0) || (currentScale > 5.0 && factor > 1.0)) return true;
        scale(factor, factor);
        return true;
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (m_toolController && m_toolController->eventFilter(obj, event)) return true;
        if (keyEvent->key() == Qt::Key_Space && !keyEvent->isAutoRepeat()) {
            m_panning = true;
            setCursor(Qt::ClosedHandCursor);
            return true;
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            m_panning = false;
            syncDragMode();
            return true;
        }
    }

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::MiddleButton) {
            m_panning = true;
            m_panOrigin = mouseEvent->position().toPoint();
            return true;
        }
    }

    if (event->type() == QEvent::MouseMove && m_panning) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        const QPoint delta = mouseEvent->position().toPoint() - m_panOrigin;
        m_panOrigin = mouseEvent->position().toPoint();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        return true;
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (m_panning && (mouseEvent->button() == Qt::MiddleButton)) {
            m_panning = false;
            syncDragMode();
            return true;
        }
    }

    return QGraphicsView::eventFilter(obj, event);
}

void CanvasView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    refreshOverlay();
}
