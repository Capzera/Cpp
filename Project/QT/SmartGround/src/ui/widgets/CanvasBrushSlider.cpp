#include "CanvasBrushSlider.h"
#include "ui/UiTheme.h"
#include "canvas/ToolController.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>

namespace {
constexpr qreal kWidgetWidth = 34.0;
constexpr qreal kWidgetHeight = 190.0;
constexpr qreal kTopPadding = 26.0;
constexpr qreal kBottomPadding = 26.0;
constexpr qreal kTrackHeight = 4.0;
constexpr qreal kHandleRadius = 7.0;
constexpr int kMinWidth = 1;
constexpr int kMaxWidth = 40;
constexpr int kMinRadius = 4;
constexpr int kMaxRadius = 60;
}

CanvasBrushSlider::CanvasBrushSlider(ToolController* controller, QWidget* parent)
    : QWidget(parent), m_controller(controller) {
    setObjectName("canvasBrushSlider");
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(static_cast<int>(kWidgetWidth), static_cast<int>(kWidgetHeight));
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setToolTip("拖动调整笔刷粗细（滚轮同样有效）");
    if (m_controller) {
        connect(m_controller, &ToolController::toolChanged, this, [this](ToolController::Tool) {
            syncFromController();
        });
        connect(m_controller, &ToolController::styleChanged, this, &CanvasBrushSlider::syncFromController);
    }
    hide();
}

void CanvasBrushSlider::setHostWidget(QWidget* host) {
    m_host = host;
    reposition();
}

int CanvasBrushSlider::currentTool() const {
    if (!m_controller) return -1;
    return static_cast<int>(m_controller->currentTool());
}

bool CanvasBrushSlider::isVisibleForTool() const {
    const int tool = currentTool();
    return tool == static_cast<int>(ToolController::Pen)
        || tool == static_cast<int>(ToolController::Highlighter)
        || tool == static_cast<int>(ToolController::Eraser);
}

void CanvasBrushSlider::syncFromController() {
    if (!m_controller) return;
    if (isVisibleForTool()) {
        show();
        raise();
    } else {
        hide();
        return;
    }
    reposition();
    update();
}

void CanvasBrushSlider::reposition() {
    if (!m_host) return;
    const int x = 16;
    const int y = (m_host->height() - static_cast<int>(kWidgetHeight)) / 2;
    setGeometry(x, qMax(8, y), static_cast<int>(kWidgetWidth), static_cast<int>(kWidgetHeight));
}

qreal CanvasBrushSlider::valueFromY(qreal y) const {
    const qreal top = kTopPadding;
    const qreal bottom = height() - kBottomPadding;
    if (bottom <= top) return 1.0;
    qreal t = (bottom - y) / (bottom - top);
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    return t;
}

void CanvasBrushSlider::applyValue(int value) {
    if (!m_controller) return;
    const int tool = currentTool();
    ToolController::ToolStyle style = m_controller->styleFor(static_cast<ToolController::Tool>(tool));
    if (tool == static_cast<int>(ToolController::Eraser)) {
        const int radius = qBound(kMinRadius, value, kMaxRadius);
        style.eraserRadius = radius;
        m_controller->setStyleFor(static_cast<ToolController::Tool>(tool), style);
        m_controller->setEraserRadius(radius);
        return;
    }
    const int width = qBound(kMinWidth, value, kMaxWidth);
    style.width = width;
    m_controller->setStyleFor(static_cast<ToolController::Tool>(tool), style);
    m_controller->setPenWidth(width);
}

void CanvasBrushSlider::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, 14, 14);
    painter.setPen(QPen(QColor(0, 0, 0, 26), 1));
    painter.setBrush(QColor(255, 255, 255, 246));
    painter.drawPath(path);
    QPainterPath inner;
    inner.addRoundedRect(box.adjusted(1, 1, -1, -1), 13, 13);
    painter.setPen(QPen(QColor(255, 255, 255, 190), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(inner);

    const qreal top = kTopPadding;
    const qreal bottom = height() - kBottomPadding;
    const QRectF track(kWidgetWidth / 2.0 - kTrackHeight / 2.0, top, kTrackHeight, bottom - top);
    painter.setPen(Qt::NoPen);
    painter.setBrush(UiTheme::surfaceSunken());
    painter.drawRoundedRect(track, kTrackHeight / 2.0, kTrackHeight / 2.0);

    const int tool = currentTool();
    int current = kMinWidth;
    if (tool == static_cast<int>(ToolController::Eraser)) {
        current = m_controller ? static_cast<int>(m_controller->eraserRadius()) : kMinRadius;
    } else if (m_controller) {
        current = static_cast<int>(m_controller->penWidth());
    }
    const int lo = (tool == static_cast<int>(ToolController::Eraser)) ? kMinRadius : kMinWidth;
    const int hi = (tool == static_cast<int>(ToolController::Eraser)) ? kMaxRadius : kMaxWidth;
    int span = hi - lo;
    qreal t = span > 0 ? static_cast<qreal>(current - lo) / span : 0.0;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    const qreal handleY = bottom - (bottom - top) * t;
    QRectF filled(track);
    filled.setY(handleY);
    filled.setHeight(bottom - handleY);
    painter.setBrush(UiTheme::accent());
    painter.drawRoundedRect(filled, kTrackHeight / 2.0, kTrackHeight / 2.0);

    if (m_hover > 0.01 || m_dragging) {
        QColor halo = UiTheme::accent();
        halo.setAlpha(static_cast<int>(72 * (m_dragging ? 1.0 : m_hover)));
        painter.setPen(Qt::NoPen);
        painter.setBrush(halo);
        painter.drawEllipse(QPointF(kWidgetWidth / 2.0, handleY), kHandleRadius + 5.0, kHandleRadius + 5.0);
    }

    painter.setBrush(UiTheme::surface());
    painter.setPen(QPen(m_dragging ? UiTheme::accent() : UiTheme::borderStrong(), m_dragging ? 2.0 : 1.4));
    painter.drawEllipse(QPointF(kWidgetWidth / 2.0, handleY), kHandleRadius, kHandleRadius);

    painter.setPen(UiTheme::textSecondary());
    painter.setFont(UiTheme::monoFont(9, true));
    painter.drawText(QRectF(0, height() - 22, kWidgetWidth, 16), Qt::AlignCenter,
                     QString::number(current));
    painter.setFont(UiTheme::uiFont(8));
    painter.setPen(UiTheme::textMuted());
    painter.drawText(QRectF(0, 6, kWidgetWidth, 16), Qt::AlignCenter, "粗细");
}

void CanvasBrushSlider::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    m_dragging = true;
    const int tool = currentTool();
    const int lo = (tool == static_cast<int>(ToolController::Eraser)) ? kMinRadius : kMinWidth;
    const int hi = (tool == static_cast<int>(ToolController::Eraser)) ? kMaxRadius : kMaxWidth;
    const qreal t = valueFromY(event->position().y());
    applyValue(lo + static_cast<int>(t * (hi - lo) + 0.5));
    update();
    event->accept();
}

void CanvasBrushSlider::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragging) {
        QWidget::mouseMoveEvent(event);
        return;
    }
    const int tool = currentTool();
    const int lo = (tool == static_cast<int>(ToolController::Eraser)) ? kMinRadius : kMinWidth;
    const int hi = (tool == static_cast<int>(ToolController::Eraser)) ? kMaxRadius : kMaxWidth;
    const qreal t = valueFromY(event->position().y());
    applyValue(lo + static_cast<int>(t * (hi - lo) + 0.5));
    update();
    event->accept();
}

void CanvasBrushSlider::mouseReleaseEvent(QMouseEvent* event) {
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        update();
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void CanvasBrushSlider::wheelEvent(QWheelEvent* event) {
    const int delta = event->angleDelta().y();
    if (delta == 0) return;
    const int tool = currentTool();
    const int lo = (tool == static_cast<int>(ToolController::Eraser)) ? kMinRadius : kMinWidth;
    const int hi = (tool == static_cast<int>(ToolController::Eraser)) ? kMaxRadius : kMaxWidth;
    const int step = (tool == static_cast<int>(ToolController::Eraser)) ? 2 : 1;
    int current = (tool == static_cast<int>(ToolController::Eraser))
        ? static_cast<int>(m_controller->eraserRadius())
        : static_cast<int>(m_controller->penWidth());
    applyValue(qBound(lo, current + (delta > 0 ? step : -step), hi));
    update();
    event->accept();
}

void CanvasBrushSlider::enterEvent(QEnterEvent* event) {
    m_hover = 1.0;
    update();
    QWidget::enterEvent(event);
}

void CanvasBrushSlider::leaveEvent(QEvent* event) {
    m_hover = 0.0;
    update();
    QWidget::leaveEvent(event);
}
