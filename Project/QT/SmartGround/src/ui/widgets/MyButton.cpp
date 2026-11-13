#include "MyButton.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>

MyButton::MyButton(const QString& text, QWidget* parent) : QAbstractButton(parent), m_text(text), m_accent(UiTheme::accent()) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFont(UiTheme::uiFont(10, true));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void MyButton::setText(const QString& text) {
    if (m_text == text) return;
    m_text = text;
    update();
}

void MyButton::setAccentColor(const QColor& c) {
    if (m_accent == c) return;
    m_accent = c;
    update();
}

void MyButton::setDangerMode(bool on) {
    if (m_danger == on) return;
    m_danger = on;
    update();
}

void MyButton::setVariant(Variant v) {
    if (m_variant == v) return;
    m_variant = v;
    update();
}

void MyButton::setIconGlyph(const QString& glyph) {
    if (m_glyph == glyph) return;
    m_glyph = glyph;
    update();
}

void MyButton::setCompact(bool on) {
    if (m_compact == on) return;
    m_compact = on;
    updateGeometry();
}

QSize MyButton::sizeHint() const {
    QFontMetrics fm(font());
    int w = fm.horizontalAdvance(m_text) + (m_compact ? 22 : 34);
    int h = m_compact ? 30 : 38;
    if (!m_glyph.isEmpty()) w += fm.horizontalAdvance(m_glyph) + 8;
    return QSize(w, h);
}

QSize MyButton::minimumSizeHint() const {
    return QSize(m_compact ? 56 : 64, m_compact ? 28 : 34);
}

void MyButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF box = QRectF(this->rect()).adjusted(0.5, 0.5, -0.5, -0.5);

    QColor fill;
    QColor borderColor;
    QColor textColor;

    if (m_danger) {
        if (m_variant == Solid) {
            fill = m_pressed ? UiTheme::danger().darker(112) : (m_hover ? UiTheme::danger() : UiTheme::danger());
            borderColor = fill.darker(110);
            textColor = UiTheme::textOnAccent();
        } else {
            fill = m_hover ? QColor(253, 236, 236) : Qt::transparent;
            borderColor = UiTheme::danger().lighter(150);
            textColor = UiTheme::danger();
        }
    } else if (m_variant == Solid) {
        if (m_pressed) fill = UiTheme::accentPressed();
        else if (m_hover) fill = UiTheme::accentHover();
        else fill = m_accent;
        borderColor = fill.darker(112);
        textColor = UiTheme::textOnAccent();
    } else if (m_variant == Soft) {
        if (m_pressed) fill = UiTheme::accentSoftDeep();
        else if (m_hover) fill = UiTheme::accentSoft();
        else fill = UiTheme::accentSoft();
        borderColor = UiTheme::accentSoftDeep();
        textColor = m_accent.darker(112);
    } else {
        fill = m_pressed ? UiTheme::surfaceSunken() : (m_hover ? UiTheme::surfaceHover() : Qt::transparent);
        borderColor = m_hover ? UiTheme::borderStrong() : UiTheme::border();
        textColor = UiTheme::textPrimary();
    }

    if (hasFocus() && m_variant != Solid) {
        borderColor = UiTheme::borderFocus();
    }

    QPainterPath path;
    path.addRoundedRect(box, UiTheme::radiusMd(), UiTheme::radiusMd());
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);
    painter.drawPath(path);
    if (m_variant != Solid || m_danger) {
        painter.setPen(QPen(borderColor, 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }

    paintContent(painter, box);
}

void MyButton::paintContent(QPainter& painter, const QRectF& rect) const {
    QColor textColor;
    if (m_danger && m_variant != Solid) textColor = UiTheme::danger();
    else if (m_variant == Solid) textColor = UiTheme::textOnAccent();
    else if (m_variant == Soft) textColor = m_accent.darker(112);
    else textColor = UiTheme::textPrimary();

    QFont f = font();
    QFontMetrics fm(f);
    painter.setFont(f);
    int textWidth = fm.horizontalAdvance(m_text);
    int glyphWidth = m_glyph.isEmpty() ? 0 : fm.horizontalAdvance(m_glyph) + 8;
    int totalWidth = textWidth + glyphWidth;

    QPointF start(rect.center().x() - totalWidth / 2.0, rect.center().y());
    painter.setPen(textColor);
    if (!m_glyph.isEmpty()) {
        painter.drawText(QRectF(start.x(), rect.top(), fm.horizontalAdvance(m_glyph) + 4, rect.height()), Qt::AlignVCenter | Qt::AlignLeft, m_glyph);
        start.setX(start.x() + glyphWidth);
    }
    painter.drawText(QRectF(start.x(), rect.top(), textWidth + 2, rect.height()), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void MyButton::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void MyButton::leaveEvent(QEvent* event) {
    m_hover = false;
    m_pressed = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void MyButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
    }
    QAbstractButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent* event) {
    m_pressed = false;
    update();
    QAbstractButton::mouseReleaseEvent(event);
}

void MyButton::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        setDown(!isDown());
        emit clicked();
        return;
    }
    QAbstractButton::keyPressEvent(event);
}
