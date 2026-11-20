#include "ValueBar.h"
#include <QPainter>
#include <QFontMetrics>
#include "ui/UiFonts.h"

ValueBar::ValueBar(double maxWidth) : m_maxWidth(maxWidth) {
    setZValue(1);
}

void ValueBar::setValue(double v) {
    if (qFuzzyCompare(m_value + 1.0, v + 1.0)) return;
    prepareGeometryChange();
    m_value = v;
    update();
}

void ValueBar::setMaxValue(double v) {
    if (qFuzzyCompare(m_maxValue + 1.0, v + 1.0)) return;
    prepareGeometryChange();
    m_maxValue = v;
    update();
}

void ValueBar::setMaxWidth(double w) {
    if (qFuzzyCompare(m_maxWidth + 1.0, w + 1.0)) return;
    prepareGeometryChange();
    m_maxWidth = w;
    update();
}

void ValueBar::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    update();
}

void ValueBar::setLabel(const QString& text) {
    if (m_label == text) return;
    prepareGeometryChange();
    m_label = text;
    update();
}

void ValueBar::setSubLabel(const QString& text) {
    if (m_subLabel == text) return;
    prepareGeometryChange();
    m_subLabel = text;
    update();
}

void ValueBar::setValueFontSize(int size) {
    if (m_valueFontSize == size) return;
    prepareGeometryChange();
    m_valueFontSize = size;
    update();
}

void ValueBar::setGhost(bool on) {
    if (m_ghost == on) return;
    prepareGeometryChange();
    m_ghost = on;
    update();
}

void ValueBar::setDimmed(bool on) {
    if (m_dimmed == on) return;
    m_dimmed = on;
    update();
}

void ValueBar::setFlip(bool on) {
    if (m_flip == on) return;
    prepareGeometryChange();
    m_flip = on;
    update();
}

double ValueBar::barWidth() const {
    if (m_maxValue <= 0.0) return 0.0;
    double ratio = m_value / m_maxValue;
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;
    return m_maxWidth * ratio;
}

QRectF ValueBar::boundingRect() const {
    const double labelH = m_label.isEmpty() ? 0.0 : 22.0;
    const double subH = m_subLabel.isEmpty() ? 0.0 : 18.0;
    const double w = qMax(m_maxWidth, 120.0);
    return QRectF(-2, -2 - labelH, w + 4, m_height + labelH + subH + 4);
}

void ValueBar::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    const double labelH = m_label.isEmpty() ? 0.0 : 22.0;
    const double subH = m_subLabel.isEmpty() ? 0.0 : 18.0;

    if (!m_label.isEmpty()) {
        painter->setFont(UiFonts::cjk(11, true));
        painter->setPen(QColor(60, 75, 95));
        painter->drawText(QRectF(0, -labelH, m_maxWidth, labelH), Qt::AlignLeft | Qt::AlignVCenter, m_label);
    }

    const double bw = barWidth();
    QColor fill = m_color;
    if (m_dimmed) fill = QColor(fill.red(), fill.green(), fill.blue(), 70);
    if (m_ghost) {
        painter->setBrush(QColor(246, 248, 251));
        painter->setPen(QPen(QColor(196, 208, 224), 1.4, Qt::DashLine));
        painter->drawRect(QRectF(0, 0, m_maxWidth, m_height));
        painter->setPen(QColor(150, 162, 180));
        painter->setFont(UiFonts::mono(m_valueFontSize, true));
        painter->drawText(QRectF(0, 0, m_maxWidth, m_height), Qt::AlignCenter,
                         QString::number(static_cast<int>(m_value)));
        return;
    }

    painter->setBrush(fill);
    painter->setPen(Qt::NoPen);
    const double x = m_flip ? m_maxWidth - bw : 0.0;
    painter->drawRoundedRect(QRectF(x, 0, qMax(bw, 1.0), m_height), 8, 8);

    painter->setPen(QColor(196, 208, 224));
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(210, 220, 234), 1.2));
    painter->drawRoundedRect(QRectF(0, 0, m_maxWidth, m_height), 8, 8);

    painter->setPen(QColor(28, 38, 54));
    painter->setFont(UiFonts::mono(m_valueFontSize, true));
    painter->drawText(QRectF(0, 0, m_maxWidth, m_height), Qt::AlignCenter,
                     QString::number(static_cast<int>(m_value)));

    if (!m_subLabel.isEmpty()) {
        painter->setFont(UiFonts::mono(10, true));
        painter->setPen(QColor(120, 132, 150));
        painter->drawText(QRectF(0, m_height, m_maxWidth, subH), Qt::AlignLeft | Qt::AlignTop, m_subLabel);
    }
}
