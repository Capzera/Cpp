#include "MySlider.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QEvent>

namespace {
constexpr qreal kHandleRadius = 6.0;
constexpr qreal kTrackHeight = 4.0;
constexpr qreal kSidePadding = 14.0;
}

MySlider::MySlider(QWidget* parent) : QAbstractSlider(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFont(UiTheme::monoFont(9));
}

void MySlider::setRange(int min, int max) {
    m_min = min;
    m_max = max;
    if (m_value < min) m_value = min;
    if (m_value > max) m_value = max;
    update();
}

void MySlider::setValue(int value) {
    if (value < m_min) value = m_min;
    if (value > m_max) value = m_max;
    if (value == m_value) return;
    m_value = value;
    update();
}

void MySlider::setGlow(qreal v) {
    if (qFuzzyCompare(m_glow, v)) return;
    m_glow = v;
    update();
}

QSize MySlider::sizeHint() const {
    return QSize(180, 22);
}

QSize MySlider::minimumSizeHint() const {
    return QSize(90, 20);
}

QRectF MySlider::trackRect() const {
    qreal h = kTrackHeight;
    qreal w = qMax<qreal>(kHandleRadius * 2.0, width() - kSidePadding * 2.0);
    return QRectF(kSidePadding, (height() - h) / 2.0, w, h);
}

QRectF MySlider::handleRect() const {
    qreal d = kHandleRadius * 2.0;
    qreal t = 0.0;
    int span = m_max - m_min;
    if (span > 0) t = static_cast<qreal>(m_value - m_min) / span;
    qreal centerX = handleCenterX(t);
    return QRectF(centerX - d / 2.0, (height() - d) / 2.0, d, d);
}

qreal MySlider::handleCenterX(qreal t) const {
    qreal left = kSidePadding;
    qreal right = width() - kSidePadding;
    if (right < left) right = left;
    return left + (right - left) * t;
}

int MySlider::valueAtX(qreal x) const {
    qreal left = kSidePadding;
    qreal right = width() - kSidePadding;
    qreal travel = right - left;
    if (travel <= 0.0) return m_min;
    qreal t = (x - left) / travel;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    int span = m_max - m_min;
    return m_min + static_cast<int>(t * span + 0.5);
}

void MySlider::emitIfChanged(int value) {
    if (value == m_value) return;
    m_value = value;
    update();
    emit valueChanged(m_value);
}

void MySlider::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF track = trackRect();
    qreal radius = track.height() / 2.0;

    QColor trackColor = UiTheme::surfaceSunken();
    painter.setPen(Qt::NoPen);
    painter.setBrush(trackColor);
    painter.drawRoundedRect(track, radius, radius);

    int span = m_max - m_min;
    if (span > 0 && m_value > m_min) {
        qreal t = static_cast<qreal>(m_value - m_min) / span;
        QRectF filled = track;
        filled.setWidth(track.width() * t);
        QColor fill = UiTheme::accent().lighter(100 + 8 * m_glow);
        painter.setBrush(fill);
        painter.drawRoundedRect(filled, radius, radius);
    }

    QRectF handle = handleRect();
    qreal handleRadius = handle.width() / 2.0;
    qreal maxGlow = qMin(handle.center().x(), width() - handle.center().x());
    if (m_glow > 0.01) {
        QColor halo = UiTheme::accent();
        halo.setAlpha(static_cast<int>(70 * m_glow));
        for (int i = 3; i >= 1; --i) {
            qreal ring = handleRadius + i * 2.0;
            if (ring > maxGlow) continue;
            QColor ringColor = UiTheme::accent();
            ringColor.setAlpha(static_cast<int>(26 * m_glow / i));
            painter.setPen(Qt::NoPen);
            painter.setBrush(ringColor);
            painter.drawEllipse(handle.center(), ring, ring);
        }
        qreal haloRadius = handleRadius + 4.0;
        if (haloRadius <= maxGlow) {
            painter.setBrush(halo);
            painter.drawEllipse(handle.center(), haloRadius, haloRadius);
        }
    }

    QColor handleFill = m_hover > 0.01 || m_dragging
        ? UiTheme::surface().lighter(100 - 2 * m_hover)
        : UiTheme::surface();
    painter.setBrush(handleFill);
    painter.setPen(QPen(m_dragging ? UiTheme::accent() : UiTheme::borderStrong(), m_dragging ? 2.0 : 1.4));
    painter.drawEllipse(handle);
}

void MySlider::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        setGlow(1.0);
        emitIfChanged(valueAtX(event->position().x()));
        event->accept();
        return;
    }
    QAbstractSlider::mousePressEvent(event);
}

void MySlider::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging) {
        emitIfChanged(valueAtX(event->position().x()));
        event->accept();
        return;
    }
    QAbstractSlider::mouseMoveEvent(event);
}

void MySlider::mouseReleaseEvent(QMouseEvent* event) {
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        setGlow(m_hover);
        emit sliderReleased(m_value);
        event->accept();
        return;
    }
    QAbstractSlider::mouseReleaseEvent(event);
}

void MySlider::wheelEvent(QWheelEvent* event) {
    int step = event->angleDelta().y() > 0 ? 1 : -1;
    emitIfChanged(m_value + step);
    event->accept();
}

void MySlider::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        emitIfChanged(m_value - 1);
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Right) {
        emitIfChanged(m_value + 1);
        event->accept();
        return;
    }
    QAbstractSlider::keyPressEvent(event);
}

void MySlider::enterEvent(QEnterEvent* event) {
    m_hover = 1.0;
    setGlow(m_dragging ? 1.0 : 0.45);
    QAbstractSlider::enterEvent(event);
}

void MySlider::leaveEvent(QEvent* event) {
    m_hover = 0.0;
    setGlow(m_dragging ? 1.0 : 0.0);
    QAbstractSlider::leaveEvent(event);
}
