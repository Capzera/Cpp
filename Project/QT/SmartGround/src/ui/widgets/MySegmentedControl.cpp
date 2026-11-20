#include "MySegmentedControl.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QEasingCurve>

MySegmentedControl::MySegmentedControl(const QStringList& options, QWidget* parent)
    : QAbstractButton(parent), m_options(options), m_accent(UiTheme::accent()) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFont(UiTheme::uiFont(9));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setFixedHeight(26);
}

void MySegmentedControl::setCurrentIndex(int index) {
    if (index < 0 || index >= m_options.size()) return;
    if (m_current == index) return;
    m_current = index;
    animateTo(index);
    update();
    emit currentIndexChanged(index);
}

void MySegmentedControl::setAccentColor(const QColor& color) {
    if (m_accent == color) return;
    m_accent = color;
    update();
}

QPainterPath MySegmentedControl::buildIconPath(int icon, const QRectF& box) {
    QPainterPath path;
    switch (icon) {
    case RectIcon:
        path.addRect(box);
        break;
    case CircleIcon:
        path.addEllipse(box);
        break;
    case TriangleIcon: {
        QPolygonF poly;
        poly << QPointF(box.center().x(), box.top())
             << QPointF(box.right(), box.bottom())
             << QPointF(box.left(), box.bottom());
        path.addPolygon(poly);
        break;
    }
    default:
        break;
    }
    return path;
}

void MySegmentedControl::paintShapeIcon(QPainter* painter, int icon, const QRectF& box,
                                         const QColor& color, bool solid) {
    if (!painter) return;
    const QPainterPath path = buildIconPath(icon, box);
    if (path.isEmpty()) return;
    if (solid) {
        painter->setPen(QPen(color, 1.3));
        painter->setBrush(color);
    } else {
        QPen dash(color, 1.3);
        dash.setStyle(Qt::DashLine);
        dash.setDashPattern({3.0, 2.4});
        painter->setPen(dash);
        painter->setBrush(Qt::NoBrush);
    }
    painter->drawPath(path);
}

void MySegmentedControl::setIndicatorProgress(qreal v) {
    if (qFuzzyCompare(m_indicatorProgress, v)) return;
    m_indicatorProgress = v;
    update();
}

void MySegmentedControl::animateTo(int index) {
    if (!m_indicatorAnim) {
        m_indicatorAnim = new QPropertyAnimation(this, "indicatorProgress", this);
        m_indicatorAnim->setDuration(180);
        m_indicatorAnim->setEasingCurve(QEasingCurve::OutCubic);
    }
    m_indicatorAnim->stop();
    m_indicatorAnim->setStartValue(m_indicatorProgress);
    m_indicatorAnim->setEndValue(static_cast<qreal>(index));
    m_indicatorAnim->start();
}

QSize MySegmentedControl::sizeHint() const {
    QFontMetrics fm(font());
    int w = 0;
    for (const QString& opt : m_options) {
        w += fm.horizontalAdvance(opt) + 22;
    }
    return QSize(qMax(w + 8, 96), 26);
}

QSize MySegmentedControl::minimumSizeHint() const {
    return QSize(96, 26);
}

int MySegmentedControl::indexAt(const QPointF& pos) const {
    if (m_options.isEmpty()) return -1;
    const int count = m_options.size();
    const qreal w = static_cast<qreal>(width()) / count;
    if (w <= 0.0) return -1;
    const int idx = static_cast<int>(pos.x() / w);
    if (idx < 0 || idx >= count) return -1;
    return idx;
}

void MySegmentedControl::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);

    QPainterPath track;
    track.addRoundedRect(box, 8.0, 8.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(238, 242, 248));
    painter.drawPath(track);
    painter.setPen(QPen(QColor(219, 226, 236), 1.0));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(track);

    const int count = m_options.size();
    if (count == 0) return;
    const qreal segW = static_cast<qreal>(width()) / count;
    const qreal progress = qBound(0.0, m_indicatorProgress, static_cast<qreal>(count - 1));

    const QRectF indicator(segW * progress + 2.0, 2.0, segW - 4.0, height() - 4.0);
    QPainterPath ipath;
    ipath.addRoundedRect(indicator, 6.5, 6.5);
    QLinearGradient grad(indicator.topLeft(), indicator.bottomLeft());
    grad.setColorAt(0.0, m_accent.lighter(118));
    grad.setColorAt(1.0, m_accent);
    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawPath(ipath);

    painter.setFont(font());
    const bool hasIcons = m_shapeIcons.size() == m_options.size();
    for (int i = 0; i < count; ++i) {
        const QRectF cell(segW * i, 0, segW, height());
        const bool active = i == m_current;
        QColor color = active ? QColor(255, 255, 255) : UiTheme::textSecondary();
        if (!active && i == m_hoverIndex) color = UiTheme::textPrimary();
        if (hasIcons) {
            const int icon = m_shapeIcons[i];
            QFontMetrics fm(font());
            const int textW = fm.horizontalAdvance(m_options[i]);
            const double iconSize = 13.0;
            const double totalW = iconSize + 5.0 + textW;
            const double startX = cell.center().x() - totalW / 2.0;
            const QRectF iconBox(startX, cell.center().y() - iconSize / 2.0, iconSize, iconSize);
            const QPainterPath ip = buildIconPath(icon, iconBox);
            if (!ip.isEmpty()) {
                paintShapeIcon(&painter, icon, iconBox, color, i == 1);
            }
            const QRectF textRect(startX + iconSize + 5.0, cell.top(), textW, cell.height());
            painter.setPen(color);
            painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_options[i]);
            continue;
        }
        painter.setPen(color);
        painter.drawText(cell, Qt::AlignCenter, m_options[i]);
    }
}

void MySegmentedControl::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        const int idx = indexAt(event->position());
        if (idx >= 0) setCurrentIndex(idx);
        event->accept();
        return;
    }
    QAbstractButton::mousePressEvent(event);
}

void MySegmentedControl::mouseMoveEvent(QMouseEvent* event) {
    const int idx = indexAt(event->position());
    if (idx != m_hoverIndex) {
        m_hoverIndex = idx;
        update();
    }
    QAbstractButton::mouseMoveEvent(event);
}

void MySegmentedControl::leaveEvent(QEvent* event) {
    if (m_hoverIndex != -1) {
        m_hoverIndex = -1;
        update();
    }
    QAbstractButton::leaveEvent(event);
}
