#include "MyRadioButton.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QMouseEvent>

MyRadioButton::MyRadioButton(const QString& text, QWidget* parent) : QAbstractButton(parent), m_text(text) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFont(UiTheme::uiFont(10));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void MyRadioButton::setText(const QString& text) {
    if (m_text == text) return;
    m_text = text;
    updateGeometry();
    update();
}

void MyRadioButton::setChecked(bool on) {
    if (m_checked == on) return;
    m_checked = on;
    update();
    emit toggled(on);
}

QSize MyRadioButton::sizeHint() const {
    QFontMetrics fm(font());
    return QSize(fm.horizontalAdvance(m_text) + 28, 26);
}

QSize MyRadioButton::minimumSizeHint() const {
    return QSize(80, 26);
}

void MyRadioButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int d = 17;
    int cy = height() / 2;
    QRectF dot(d / 2.0 + 1, cy - d / 2.0, d, d);

    painter.setPen(QPen(m_checked ? UiTheme::accent() : UiTheme::borderStrong(), m_checked ? 2 : 1.5));
    painter.setBrush(m_hover && !m_checked ? UiTheme::surfaceHover() : UiTheme::surface());
    painter.drawEllipse(dot);

    if (m_checked) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::accent());
        painter.drawEllipse(dot.adjusted(4, 4, -4, -4));
    }

    painter.setPen(m_checked ? UiTheme::textPrimary() : UiTheme::textSecondary());
    painter.setFont(font());
    painter.drawText(QRectF(d + 8, 0, width() - d - 8, height()), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void MyRadioButton::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void MyRadioButton::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void MyRadioButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setChecked(true);
        event->accept();
        return;
    }
    QAbstractButton::mousePressEvent(event);
}
