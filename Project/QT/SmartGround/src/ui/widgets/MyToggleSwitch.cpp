#include "MyToggleSwitch.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QEasingCurve>

MyToggleSwitch::MyToggleSwitch(QWidget* parent)
    : QAbstractButton(parent),
      m_offText(QString()),
      m_onText(QString()),
      m_onColor(UiTheme::success()) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setCheckable(true);
    setFont(UiTheme::uiFont(9, true));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_anim = new QPropertyAnimation(this, "position", this);
    m_anim->setDuration(190);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_anim, &QPropertyAnimation::valueChanged, this, [this](const QVariant& v) {
        m_position = v.toReal();
        update();
    });
}

void MyToggleSwitch::setChecked(bool on) {
    if (m_checked == on) {
        m_anim->stop();
        m_position = on ? 1.0 : 0.0;
        update();
        return;
    }
    m_checked = on;
    m_anim->stop();
    m_anim->setStartValue(m_position);
    m_anim->setEndValue(on ? 1.0 : 0.0);
    m_anim->start();
    emit toggled(on);
}

void MyToggleSwitch::setPosition(qreal v) {
    m_position = v;
    update();
}

void MyToggleSwitch::setRatio(qreal v) {
    m_ratio = v;
    update();
}

void MyToggleSwitch::setOffText(const QString& text) {
    if (m_offText == text) return;
    m_offText = text;
    updateGeometry();
    update();
}

void MyToggleSwitch::setOnText(const QString& text) {
    if (m_onText == text) return;
    m_onText = text;
    updateGeometry();
    update();
}

void MyToggleSwitch::setOnColor(const QColor& c) {
    if (m_onColor == c) return;
    m_onColor = c;
    update();
}

QSize MyToggleSwitch::sizeHint() const {
    QFontMetrics fm(font());
    int offW = m_offText.isEmpty() ? 0 : fm.horizontalAdvance(m_offText);
    int onW = m_onText.isEmpty() ? 0 : fm.horizontalAdvance(m_onText);
    int w = 50 + offW + onW + 24;
    return QSize(w, 26);
}

QSize MyToggleSwitch::minimumSizeHint() const {
    return sizeHint();
}

QRectF MyToggleSwitch::trackRect() const {
    qreal trackW = 50.0;
    qreal trackH = 24.0;
    qreal left = 0;
    if (!m_offText.isEmpty()) {
        QFontMetrics fm(font());
        left = fm.horizontalAdvance(m_offText) + 10;
    }
    return QRectF(left, (height() - trackH) / 2.0, trackW, trackH);
}

QRectF MyToggleSwitch::knobRect(qreal t) const {
    QRectF track = trackRect();
    qreal knob = 18.0;
    qreal travel = track.width() - knob - 6.0;
    qreal x = track.left() + 3.0 + travel * t;
    return QRectF(x, track.top() + 3.0, knob, knob);
}

void MyToggleSwitch::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(0.25 + 0.75 * m_ratio);
    QRectF track = trackRect();
    QRectF knob = knobRect(m_position);

    QColor trackColor;
    if (m_position > 0.5) {
        QColor on = m_onColor;
        trackColor = on.lighter(100 + 8 * (1.0 - m_position));
    } else {
        trackColor = UiTheme::trackOff().lighter(100 + 8 * m_position);
    }
    if (m_hover) trackColor = trackColor.lighter(106);

    painter.setPen(Qt::NoPen);
    painter.setBrush(trackColor);
    painter.drawRoundedRect(track, 12, 12);

    QColor knobColor = m_position > 0.5 ? UiTheme::surface() : QColor(252, 252, 253);
    painter.setBrush(knobColor);
    painter.setPen(QPen(QColor(0, 0, 0, 24), 1));
    painter.drawEllipse(knob);

    painter.setFont(font());
    bool onActive = m_position > 0.5;
    QRectF leftRect(0, 0, track.left() - 8, height());
    QRectF rightRect(track.right() + 8, 0, width() - track.right() - 8, height());

    painter.setPen(onActive ? UiTheme::textMuted() : UiTheme::textPrimary());
    painter.drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, m_offText);
    painter.setPen(onActive ? UiTheme::textPrimary() : UiTheme::textMuted());
    painter.drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, m_onText);
}

void MyToggleSwitch::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void MyToggleSwitch::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void MyToggleSwitch::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked);
        event->accept();
        return;
    }
    QAbstractButton::mousePressEvent(event);
}
