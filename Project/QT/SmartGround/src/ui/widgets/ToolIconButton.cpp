#include "ToolIconButton.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

namespace {

QPen iconPen(QPainter& painter, const QColor& color, qreal width = 1.6) {
    QPen pen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    return pen;
}

void drawCursor(QPainter& painter, const QRectF& box, const QColor& color) {
    QPainterPath path;
    const double w = box.width() * 0.42;
    const double h = box.height() * 0.56;
    const double x = box.center().x() - w * 0.45;
    const double y = box.center().y() - h * 0.5;
    path.moveTo(x, y);
    path.lineTo(x, y + h);
    path.lineTo(x + w * 0.28, y + h * 0.72);
    path.lineTo(x + w * 0.52, y + h * 1.06);
    path.lineTo(x + w * 0.68, y + h * 0.96);
    path.lineTo(x + w * 0.44, y + h * 0.62);
    path.lineTo(x + w * 0.78, y + h * 0.56);
    path.closeSubpath();
    iconPen(painter, color, 1.5);
    painter.drawPath(path);
}

void drawFrame(QPainter& painter, const QRectF& box, const QColor& color) {
    QRectF r = box.adjusted(box.width() * 0.18, box.height() * 0.18,
                            -box.width() * 0.18, -box.height() * 0.18);
    QPen pen = iconPen(painter, color, 1.5);
    pen.setStyle(Qt::DashLine);
    pen.setDashPattern({3.0, 2.4});
    painter.setPen(pen);
    painter.drawRect(r);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    const double d = 3.0;
    painter.drawRoundedRect(QRectF(r.left() - d / 2, r.top() - d / 2, d, d), 1, 1);
    painter.drawRoundedRect(QRectF(r.right() - d / 2, r.top() - d / 2, d, d), 1, 1);
    painter.drawRoundedRect(QRectF(r.left() - d / 2, r.bottom() - d / 2, d, d), 1, 1);
    painter.drawRoundedRect(QRectF(r.right() - d / 2, r.bottom() - d / 2, d, d), 1, 1);
}

void drawPen(QPainter& painter, const QRectF& box, const QColor& color) {
    QPainterPath path;
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    path.moveTo(c.x() - s * 0.42, c.y() + s * 0.46);
    path.lineTo(c.x() + s * 0.26, c.y() - s * 0.44);
    path.lineTo(c.x() + s * 0.50, c.y() - s * 0.18);
    path.lineTo(c.x() - s * 0.18, c.y() + s * 0.72);
    path.closeSubpath();
    iconPen(painter, color, 1.5);
    painter.drawPath(path);
    painter.drawLine(QPointF(c.x() - s * 0.30, c.y() + s * 0.58),
                     QPointF(c.x() + s * 0.14, c.y() - s * 0.28));
}

void drawHighlighter(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    QPainterPath body;
    body.moveTo(c.x() - s * 0.40, c.y() + s * 0.52);
    body.lineTo(c.x() - s * 0.40, c.y() - s * 0.10);
    body.lineTo(c.x() - s * 0.02, c.y() - s * 0.52);
    body.lineTo(c.x() + s * 0.40, c.y() - s * 0.06);
    body.lineTo(c.x() + s * 0.02, c.y() + s * 0.38);
    body.closeSubpath();
    iconPen(painter, color, 1.4);
    painter.drawPath(body);

    QPainterPath tip;
    tip.moveTo(c.x() - s * 0.40, c.y() + s * 0.52);
    tip.lineTo(c.x() - s * 0.24, c.y() + s * 0.74);
    tip.lineTo(c.x() - s * 0.06, c.y() + s * 0.44);
    tip.closeSubpath();
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawPath(tip);

    painter.setPen(QPen(color, 1.6, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(c.x() - s * 0.58, c.y() + s * 0.70),
                     QPointF(c.x() + s * 0.58, c.y() + s * 0.70));
}

void drawEraser(QPainter& painter, const QRectF& box, const QColor& color) {
    QPainterPath path;
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    path.moveTo(c.x() - s * 0.62, c.y() + s * 0.36);
    path.lineTo(c.x() - s * 0.02, c.y() - s * 0.38);
    path.lineTo(c.x() + s * 0.62, c.y() - s * 0.10);
    path.lineTo(c.x() + s * 0.34, c.y() + s * 0.44);
    path.closeSubpath();
    iconPen(painter, color, 1.5);
    painter.drawPath(path);
    painter.drawLine(QPointF(c.x() - s * 0.20, c.y() + s * 0.02),
                     QPointF(c.x() + s * 0.44, c.y() + s * 0.14));
}

void drawLine(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF a = box.center() + QPointF(-box.width() * 0.34, box.height() * 0.30);
    const QPointF b = box.center() + QPointF(box.width() * 0.34, -box.height() * 0.30);
    iconPen(painter, color, 1.6);
    painter.drawLine(a, b);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawEllipse(a, 2.0, 2.0);
    painter.drawEllipse(b, 2.0, 2.0);
}

void drawRect(QPainter& painter, const QRectF& box, const QColor& color) {
    const QRectF r = box.adjusted(box.width() * 0.20, box.height() * 0.22,
                                  -box.width() * 0.20, -box.height() * 0.22);
    iconPen(painter, color, 1.6);
    painter.drawRect(r);
}

void drawCircle(QPainter& painter, const QRectF& box, const QColor& color) {
    const QRectF r = box.adjusted(box.width() * 0.20, box.height() * 0.20,
                                  -box.width() * 0.20, -box.height() * 0.20);
    iconPen(painter, color, 1.6);
    painter.drawEllipse(r);
}

void drawArrow(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF a = box.center() + QPointF(-box.width() * 0.34, box.height() * 0.28);
    const QPointF b = box.center() + QPointF(box.width() * 0.34, -box.height() * 0.28);
    iconPen(painter, color, 1.6);
    painter.drawLine(a, b);
    const QPointF dir = (b - a);
    const double len = std::hypot(dir.x(), dir.y());
    if (len > 0.1) {
        const double ang = std::atan2(dir.y(), dir.x());
        const double head = qMin(box.width(), box.height()) * 0.22;
        QPainterPath headPath;
        headPath.moveTo(b);
        headPath.lineTo(b.x() - head * std::cos(ang - 0.42), b.y() - head * std::sin(ang - 0.42));
        headPath.lineTo(b.x() - head * std::cos(ang + 0.42), b.y() - head * std::sin(ang + 0.42));
        headPath.closeSubpath();
        painter.setBrush(color);
        painter.drawPath(headPath);
    }
}

void drawTriangle(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    QPainterPath path;
    path.moveTo(c.x(), c.y() - s * 0.62);
    path.lineTo(c.x() + s * 0.58, c.y() + s * 0.44);
    path.lineTo(c.x() - s * 0.58, c.y() + s * 0.44);
    path.closeSubpath();
    iconPen(painter, color, 1.6);
    painter.drawPath(path);
}

void drawMosaic(QPainter& painter, const QRectF& box, const QColor& color) {
    const double cell = qMin(box.width(), box.height()) * 0.26;
    const double ox = box.center().x() - cell * 1.5;
    const double oy = box.center().y() - cell * 1.5;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            const bool filled = (r + c) % 2 == 0;
            const QRectF cellRect(ox + c * cell, oy + r * cell, cell, cell);
            if (filled) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(color);
                painter.drawRect(cellRect);
            } else {
                iconPen(painter, color, 1.0);
                painter.drawRect(cellRect);
            }
        }
    }
}

void drawText(QPainter& painter, const QRectF& box, const QColor& color) {
    QFont f("Segoe UI");
    f.setPointSize(11);
    f.setBold(true);
    painter.setFont(f);
    painter.setPen(color);
    painter.drawText(box, Qt::AlignCenter, QString("T"));
}

void drawMarkdown(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    QPainterPath path;
    path.moveTo(c.x() - s * 0.62, c.y() - s * 0.44);
    path.lineTo(c.x() - s * 0.30, c.y() - s * 0.44);
    path.lineTo(c.x() - s * 0.30, c.y() + s * 0.44);
    path.moveTo(c.x() - s * 0.62, c.y() - s * 0.44);
    path.lineTo(c.x() - s * 0.62, c.y() + s * 0.44);
    path.moveTo(c.x() - s * 0.30, c.y() - s * 0.10);
    path.lineTo(c.x() - s * 0.08, c.y() + s * 0.44);
    path.moveTo(c.x() - s * 0.08, c.y() + s * 0.44);
    path.lineTo(c.x() - s * 0.08, c.y() - s * 0.44);
    path.lineTo(c.x() + s * 0.26, c.y() - s * 0.44);
    path.lineTo(c.x() + s * 0.26, c.y() + s * 0.44);
    path.lineTo(c.x() + s * 0.26, c.y() - s * 0.44);
    path.lineTo(c.x() + s * 0.62, c.y() - s * 0.44);
    path.lineTo(c.x() + s * 0.62, c.y() + s * 0.44);
    path.lineTo(c.x() + s * 0.62, c.y() - s * 0.44);
    iconPen(painter, color, 1.5);
    painter.drawPath(path);
}

void drawLatex(QPainter& painter, const QRectF& box, const QColor& color) {
    QFont f("Cambria Math");
    if (QFontMetrics(f).horizontalAdvance("∑") <= 0) f = QFont("Segoe UI");
    f.setPointSize(13);
    painter.setFont(f);
    painter.setPen(color);
    painter.drawText(box, Qt::AlignCenter, QString("∑"));
}

void drawTable(QPainter& painter, const QRectF& box, const QColor& color) {
    const QRectF r = box.adjusted(box.width() * 0.20, box.height() * 0.24,
                                  -box.width() * 0.20, -box.height() * 0.24);
    iconPen(painter, color, 1.4);
    painter.drawRect(r);
    painter.setPen(QPen(color, 1.0));
    painter.drawLine(QPointF(r.left(), r.top() + r.height() / 3), QPointF(r.right(), r.top() + r.height() / 3));
    painter.drawLine(QPointF(r.left(), r.top() + r.height() * 2 / 3), QPointF(r.right(), r.top() + r.height() * 2 / 3));
    painter.drawLine(QPointF(r.center().x(), r.top()), QPointF(r.center().x(), r.bottom()));
}

void drawStyle(QPainter& painter, const QRectF& box, const QColor& color) {
    const double s = qMin(box.width(), box.height()) * 0.5;
    const QPointF c = box.center();
    for (int i = 0; i < 3; ++i) {
        const double y = c.y() - s * 0.44 + i * s * 0.44;
        iconPen(painter, color, 1.4);
        painter.drawLine(QPointF(c.x() - s * 0.62, y), QPointF(c.x() + s * 0.62, y));
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        const double hx = c.x() - s * 0.20 + i * s * 0.34;
        painter.drawEllipse(QPointF(hx, y), 2.6, 2.6);
    }
}

void drawMore(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF c = box.center();
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    const double d = qMin(box.width(), box.height()) * 0.11;
    for (int i = -1; i <= 1; ++i) {
        painter.drawEllipse(QPointF(c.x() + i * d * 2.0, c.y()), d, d);
    }
}

void drawUndo(QPainter& painter, const QRectF& box, const QColor& color) {
    const QPointF c = box.center();
    const double s = qMin(box.width(), box.height()) * 0.5;
    QPainterPath path;
    const QRectF arcRect(c.x() - s * 0.55, c.y() - s * 0.45, s * 1.1, s * 0.9);
    path.arcTo(arcRect, 40.0, 270.0);
    iconPen(painter, color, 1.5);
    painter.drawPath(path);
    QPainterPath head;
    head.moveTo(c.x() - s * 0.58, c.y() - s * 0.12);
    head.lineTo(c.x() - s * 0.58, c.y() - s * 0.52);
    head.lineTo(c.x() - s * 0.22, c.y() - s * 0.26);
    head.closeSubpath();
    painter.setBrush(color);
    painter.drawPath(head);
}

void drawRedo(QPainter& painter, const QRectF& box, const QColor& color) {
    painter.save();
    QTransform t;
    t.translate(box.center().x() * 2, 0);
    t.scale(-1, 1);
    painter.setTransform(t, true);
    drawUndo(painter, QRectF(box.center().x() - box.width() / 2, box.top(), box.width(), box.height()), color);
    painter.restore();
}

}  // namespace

ToolIconButton::ToolIconButton(Kind kind, const QString& tip, QWidget* parent)
    : QAbstractButton(parent), m_kind(kind) {
    setToolTip(tip);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFixedSize(34, 34);
    setFocusPolicy(Qt::StrongFocus);
}

void ToolIconButton::setActive(bool on) {
    if (m_active == on) return;
    m_active = on;
    update();
}

void ToolIconButton::paintIcon(QPainter& painter, Kind kind, const QRectF& box, const QColor& color) {
    switch (kind) {
    case Select: drawCursor(painter, box, color); break;
    case Marquee: drawFrame(painter, box, color); break;
    case Pen: drawPen(painter, box, color); break;
    case Highlighter: drawHighlighter(painter, box, color); break;
    case Eraser: drawEraser(painter, box, color); break;
    case Line: drawLine(painter, box, color); break;
    case Rect: drawRect(painter, box, color); break;
    case Circle: drawCircle(painter, box, color); break;
    case Arrow: drawArrow(painter, box, color); break;
    case Triangle: drawTriangle(painter, box, color); break;
    case Mosaic: drawMosaic(painter, box, color); break;
    case Text: drawText(painter, box, color); break;
    case Markdown: drawMarkdown(painter, box, color); break;
    case Latex: drawLatex(painter, box, color); break;
    case Table: drawTable(painter, box, color); break;
    case Style: drawStyle(painter, box, color); break;
    case More: drawMore(painter, box, color); break;
    case Undo: drawUndo(painter, box, color); break;
    case Redo: drawRedo(painter, box, color); break;
    default: break;
    }
}

void ToolIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    const QRectF box = QRectF(rect()).adjusted(7.5, 7.5, -7.5, -7.5);
    QColor iconColor = m_active ? UiTheme::textOnAccent() : UiTheme::textSecondary();

    if (m_active) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::accent());
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 9, 9);
    } else if (m_hover) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::surfaceHover());
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 9, 9);
        iconColor = UiTheme::textPrimary();
    }

    paintIcon(painter, m_kind, box, iconColor);
}

void ToolIconButton::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void ToolIconButton::leaveEvent(QEvent* event) {
    m_hover = false;
    m_pressed = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void ToolIconButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
        event->accept();
        return;
    }
    QAbstractButton::mousePressEvent(event);
}

void ToolIconButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        const bool wasPressed = m_pressed;
        m_pressed = false;
        update();
        if (wasPressed) {
            const QPoint local = mapFromGlobal(event->globalPosition().toPoint());
            if (rect().contains(local)) {
                emit clicked();
                event->accept();
                return;
            }
        }
    }
    QAbstractButton::mouseReleaseEvent(event);
}
