#include "TableIconButton.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <cmath>

namespace {

void guide(QPainter& painter, const QRectF& box, const QColor& color) {
    QPen pen(color, 1.0, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    const double w = box.width();
    const double h = box.height();
    for (int i = 0; i < 4; ++i) {
        const double y = box.top() + h * (0.22 + i * 0.19);
        const double len = (i % 2 == 0) ? w * 0.74 : w * 0.52;
        painter.drawLine(QPointF(box.left() + (w - len) / 2.0, y), QPointF(box.left() + (w + len) / 2.0, y));
    }
}

void alignIcon(QPainter& painter, TableIconButton::Icon icon, const QRectF& box, const QColor& color) {
    QPen pen(color, 1.6, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    const double x = box.left();
    const double w = box.width();
    const double top = box.top();
    const double h = box.height();

    switch (icon) {
    case TableIconButton::AlignLeft: {
        painter.drawLine(x, top, x, top + h);
        painter.drawLine(x + 3, top + h * 0.20, x + w * 0.82, top + h * 0.20);
        painter.drawLine(x + 3, top + h * 0.48, x + w * 0.58, top + h * 0.48);
        painter.drawLine(x + 3, top + h * 0.76, x + w * 0.82, top + h * 0.76);
        break;
    }
    case TableIconButton::AlignCenter: {
        const double cx = x + w / 2.0;
        painter.drawLine(cx, top, cx, top + h);
        painter.drawLine(x + w * 0.09, top + h * 0.20, x + w * 0.91, top + h * 0.20);
        painter.drawLine(x + w * 0.21, top + h * 0.48, x + w * 0.79, top + h * 0.48);
        painter.drawLine(x + w * 0.09, top + h * 0.76, x + w * 0.91, top + h * 0.76);
        break;
    }
    case TableIconButton::AlignRight: {
        painter.drawLine(x + w, top, x + w, top + h);
        painter.drawLine(x + w * 0.18, top + h * 0.20, x + w - 3, top + h * 0.20);
        painter.drawLine(x + w * 0.42, top + h * 0.48, x + w - 3, top + h * 0.48);
        painter.drawLine(x + w * 0.18, top + h * 0.76, x + w - 3, top + h * 0.76);
        break;
    }
    case TableIconButton::AlignTop: {
        painter.drawLine(x, top, x + w, top);
        painter.drawLine(x + w * 0.20, top + 3, x + w * 0.20, top + h * 0.80);
        painter.drawLine(x + w * 0.50, top + 3, x + w * 0.50, top + h * 0.56);
        painter.drawLine(x + w * 0.80, top + 3, x + w * 0.80, top + h * 0.80);
        break;
    }
    default:
        break;
    }
}

void cellIcon(QPainter& painter, TableIconButton::Icon icon, const QRectF& box, const QColor& color) {
    const double w = box.width();
    const double h = box.height();
    const QRectF grid(box.left() + w * 0.10, box.top() + h * 0.14, w * 0.80, h * 0.72);

    QPen pen(color, 1.1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(grid);

    painter.setPen(QPen(color, 0.9));
    painter.drawLine(QPointF(grid.center().x(), grid.top()), QPointF(grid.center().x(), grid.bottom()));
    painter.drawLine(QPointF(grid.left(), grid.center().y()), QPointF(grid.right(), grid.center().y()));

    QColor fill = color;
    fill.setAlpha(90);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);

    switch (icon) {
    case TableIconButton::SelectCell:
        painter.drawRect(QRectF(grid.left(), grid.top(), grid.width() / 2.0, grid.height() / 2.0));
        break;
    case TableIconButton::SelectRow:
        painter.drawRect(QRectF(grid.left(), grid.center().y() - grid.height() / 4.0,
                               grid.width(), grid.height() / 2.0));
        break;
    case TableIconButton::SelectColumn:
        painter.drawRect(QRectF(grid.center().x() - grid.width() / 4.0, grid.top(),
                               grid.width() / 2.0, grid.height()));
        break;
    default:
        break;
    }
}

void insertIcon(QPainter& painter, TableIconButton::Icon icon, const QRectF& box, const QColor& color) {
    const double w = box.width();
    const double h = box.height();
    const bool row = (icon == TableIconButton::InsertRow);

    QPen pen(color, 1.1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    if (row) {
        const QRectF body(box.left() + w * 0.12, box.top() + h * 0.14, w * 0.76, h * 0.44);
        painter.drawRect(body);
        painter.setPen(QPen(color, 1.0));
        painter.drawLine(QPointF(body.left(), body.bottom()), QPointF(body.right(), body.bottom()));
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        QPolygonF arrow;
        const double cx = box.center().x();
        arrow << QPointF(cx, box.top() + h * 0.92)
              << QPointF(cx - w * 0.13, box.top() + h * 0.68)
              << QPointF(cx + w * 0.13, box.top() + h * 0.68);
        painter.drawPolygon(arrow);
    } else {
        const QRectF body(box.left() + w * 0.12, box.top() + h * 0.14, w * 0.44, h * 0.76);
        painter.drawRect(body);
        painter.setPen(QPen(color, 1.0));
        painter.drawLine(QPointF(body.right(), body.top()), QPointF(body.right(), body.bottom()));
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        QPolygonF arrow;
        const double cy = box.center().y();
        arrow << QPointF(box.left() + w * 0.92, cy)
              << QPointF(box.left() + w * 0.68, cy - h * 0.16)
              << QPointF(box.left() + w * 0.68, cy + h * 0.16);
        painter.drawPolygon(arrow);
    }
}

void deleteIcon(QPainter& painter, TableIconButton::Icon icon, const QRectF& box, const QColor& color) {
    const double w = box.width();
    const double h = box.height();
    const bool row = (icon == TableIconButton::DeleteRow);
    const QColor warn = QColor(214, 74, 78);

    QPen pen(color, 1.1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    if (row) {
        const QRectF body(box.left() + w * 0.12, box.top() + h * 0.14, w * 0.76, h * 0.44);
        painter.drawRect(body);
        painter.setPen(QPen(warn, 1.6, Qt::SolidLine, Qt::RoundCap));
        const double cx = body.center().x();
        const double cy = body.center().y();
        painter.drawLine(QPointF(cx - w * 0.10, cy - h * 0.11), QPointF(cx + w * 0.10, cy + h * 0.11));
        painter.drawLine(QPointF(cx + w * 0.10, cy - h * 0.11), QPointF(cx - w * 0.10, cy + h * 0.11));
    } else {
        const QRectF body(box.left() + w * 0.12, box.top() + h * 0.14, w * 0.44, h * 0.76);
        painter.drawRect(body);
        painter.setPen(QPen(warn, 1.6, Qt::SolidLine, Qt::RoundCap));
        const double cx = body.center().x();
        const double cy = body.center().y();
        painter.drawLine(QPointF(cx - w * 0.06, cy - h * 0.13), QPointF(cx + w * 0.06, cy + h * 0.13));
        painter.drawLine(QPointF(cx + w * 0.06, cy - h * 0.13), QPointF(cx - w * 0.06, cy + h * 0.13));
    }
}

}  // namespace

TableIconButton::TableIconButton(Icon icon, const QString& tip, QWidget* parent)
    : QAbstractButton(parent), m_icon(icon) {
    setToolTip(tip);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFixedSize(26, 26);
    setFocusPolicy(Qt::StrongFocus);
}

void TableIconButton::paintIcon(QPainter& painter, Icon icon, const QRectF& box, const QColor& color) {
    switch (icon) {
    case AlignLeft:
    case AlignCenter:
    case AlignRight:
    case AlignTop:
        alignIcon(painter, icon, box, color);
        break;
    case SelectCell:
    case SelectRow:
    case SelectColumn:
        cellIcon(painter, icon, box, color);
        break;
    case InsertRow:
    case InsertColumn:
        insertIcon(painter, icon, box, color);
        break;
    case DeleteRow:
    case DeleteColumn:
        deleteIcon(painter, icon, box, color);
        break;
    }
}

void TableIconButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(5.5, 5.5, -5.5, -5.5);
    QColor iconColor = UiTheme::textSecondary();

    if (m_hover && isEnabled()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::surfaceHover());
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 7, 7);
        iconColor = UiTheme::textPrimary();
    }
    if (!isEnabled()) iconColor = UiTheme::textMuted().lighter(140);

    paintIcon(painter, m_icon, box, iconColor);
}

void TableIconButton::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void TableIconButton::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QAbstractButton::leaveEvent(event);
}
