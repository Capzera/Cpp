#include "SegmentBar.h"
#include <QPainter>
#include <QFontMetrics>
#include "ui/UiFonts.h"

SegmentBar::SegmentBar(double x1, double x2, double height) : m_x1(x1), m_x2(x2), m_height(height) {
    setZValue(1);
}

void SegmentBar::setBar(const QString& text) {
    if (m_text == text) return;
    prepareGeometryChange();
    m_text = text;
    update();
}

void SegmentBar::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    update();
}

void SegmentBar::setMuted(bool on) {
    if (m_muted == on) return;
    m_muted = on;
    update();
}

void SegmentBar::setRadius(qreal r) {
    if (qFuzzyCompare(m_radius, r)) return;
    prepareGeometryChange();
    m_radius = r;
    update();
}

void SegmentBar::setFontSize(int size) {
    if (m_fontSize == size) return;
    prepareGeometryChange();
    m_fontSize = size;
    update();
}

QRectF SegmentBar::boundingRect() const {
    return QRectF(-2, -2, barWidth() + 4, m_height + 4);
}

void SegmentBar::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    QColor fill = m_color;
    QColor border = QColor(150, 165, 185);
    if (m_muted) {
        fill = QColor(fill.red(), fill.green(), fill.blue(), 60);
        border = QColor(200, 206, 216);
    }
    painter->setPen(QPen(border, 1.2));
    painter->setBrush(fill);
    painter->drawRoundedRect(QRectF(0, 0, barWidth(), m_height), m_radius, m_radius);

    if (!m_text.isEmpty()) {
        painter->setFont(UiFonts::cjk(m_fontSize, true));
        painter->setPen(m_muted ? QColor(160, 166, 176) : QColor(38, 50, 68));
        painter->drawText(QRectF(0, 0, barWidth(), m_height), Qt::AlignCenter, m_text);
    }
}
