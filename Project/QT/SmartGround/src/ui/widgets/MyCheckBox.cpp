#include "MyCheckBox.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

MyCheckBox::MyCheckBox(const QString& text, QWidget* parent) : QAbstractButton(parent), m_text(text) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setCheckable(true);
    setFont(UiTheme::uiFont(10));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void MyCheckBox::setText(const QString& text) {
    if (m_text == text) return;
    m_text = text;
    updateGeometry();
    update();
}

void MyCheckBox::setChecked(bool on) {
    if (m_checked == on) return;
    m_checked = on;
    update();
    emit toggled(on);
}

QSize MyCheckBox::sizeHint() const {
    QFontMetrics fm(font());
    return QSize(fm.horizontalAdvance(m_text) + 28, 26);
}

QSize MyCheckBox::minimumSizeHint() const {
    return QSize(80, 26);
}

void MyCheckBox::setRatio(qreal v) {
    if (qFuzzyCompare(m_ratio, v)) return;
    m_ratio = v;
    setEnabled(v > 0.5);
    update();
}

void MyCheckBox::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(0.3 + 0.7 * m_ratio);
    int s = 17;
    int cy = height() / 2;
    QRectF box(1, cy - s / 2.0, s, s);

    QPainterPath path;
    path.addRoundedRect(box, 4, 4);

    if (m_checked) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::accent());
        painter.drawPath(path);
        painter.setPen(QPen(UiTheme::textOnAccent(), 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QPainterPath tick;
        tick.moveTo(box.left() + 4.0, box.top() + s / 2.0);
        tick.lineTo(box.left() + s * 0.44, box.bottom() - 4.5);
        tick.lineTo(box.right() - 3.5, box.top() + 4.0);
        painter.drawPath(tick);
    } else {
        painter.setPen(QPen(m_hover ? UiTheme::accent() : UiTheme::borderStrong(), 1.5));
        painter.setBrush(m_hover ? UiTheme::accentSoft() : UiTheme::surface());
        painter.drawPath(path);
    }

    painter.setPen(m_checked ? UiTheme::textPrimary() : UiTheme::textSecondary());
    painter.setFont(font());
    painter.drawText(QRectF(s + 8, 0, width() - s - 8, height()), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void MyCheckBox::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void MyCheckBox::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void MyCheckBox::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked);
        event->accept();
        return;
    }
    QAbstractButton::mousePressEvent(event);
}
