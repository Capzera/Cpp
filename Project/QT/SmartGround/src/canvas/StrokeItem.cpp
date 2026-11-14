#include "StrokeItem.h"
#include <QPainter>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include <limits>
#include <cmath>

namespace {

constexpr qreal kMinSampleDistance = 0.9;

}  // namespace

StrokeItem::StrokeItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    setZValue(10);
    applyStyle();
}

void StrokeItem::setMode(Mode m) {
    if (m_mode == m) return;
    m_mode = m;
    applyStyle();
    notifyContentChanged();
}

void StrokeItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void StrokeItem::addPoint(const QPointF& p) {
    if (!m_points.isEmpty()) {
        const QPointF last = m_points.last();
        const qreal dx = p.x() - last.x();
        const qreal dy = p.y() - last.y();
        if (dx * dx + dy * dy < kMinSampleDistance * kMinSampleDistance) return;
    }
    m_points.push_back(p);
    rebuild();
    notifyContentChanged();
}

void StrokeItem::setPoints(const QVector<QPointF>& pts) {
    m_points = pts;
    rebuild();
    notifyContentChanged();
}

void StrokeItem::rebuild() {
    prepareGeometryChange();
    m_smooth = smoothPoints();
    m_center = buildCenterPath();
    m_path = buildPath();
    update();
}

QVector<QPointF> StrokeItem::smoothPoints() const {
    QVector<QPointF> out;
    if (m_points.isEmpty()) return out;
    if (m_points.size() == 1) {
        out << m_points.first();
        return out;
    }
    if (m_points.size() == 2) {
        out << m_points.at(0);
        out << m_points.at(1);
        return out;
    }
    for (int i = 0; i < m_points.size() - 1; ++i) {
        const QPointF p0 = m_points.at(i > 0 ? i - 1 : 0);
        const QPointF p1 = m_points.at(i);
        const QPointF p2 = m_points.at(i + 1);
        const QPointF p3 = m_points.at(i + 2 < m_points.size() ? i + 2 : i + 1);

        const QPointF c1 = p1 + (p2 - p0) / 6.0;
        const QPointF c2 = p2 - (p3 - p1) / 6.0;

        const int steps = 12;
        for (int s = 0; s < steps; ++s) {
            const qreal t = static_cast<qreal>(s) / steps;
            const qreal mt = 1.0 - t;
            const qreal a = mt * mt * mt;
            const qreal b = 3.0 * mt * mt * t;
            const qreal c = 3.0 * mt * t * t;
            const qreal d = t * t * t;
            out << QPointF(a * p1.x() + b * c1.x() + c * c2.x() + d * p2.x(),
                           a * p1.y() + b * c1.y() + c * c2.y() + d * p2.y());
        }
    }
    out << m_points.last();
    return out;
}

QPainterPath StrokeItem::buildCenterPath() const {
    QPainterPath path;
    if (m_smooth.isEmpty()) return path;
    path.moveTo(m_smooth.first());
    if (m_smooth.size() == 1) {
        path.lineTo(m_smooth.first() + QPointF(0.01, 0.0));
        return path;
    }
    for (int i = 1; i < m_smooth.size(); ++i) {
        path.lineTo(m_smooth.at(i));
    }
    return path;
}

QPainterPath StrokeItem::buildPath() const {
    return m_center;
}

QRectF StrokeItem::boundingRect() const {
    if (m_points.isEmpty()) return QRectF(-2, -2, 4, 4);
    const QRectF r = m_center.boundingRect();
    const qreal pad = m_penWidth + 3.0;
    return r.adjusted(-pad, -pad, pad, pad);
}

QPainterPath StrokeItem::shape() const {
    QPainterPath p = m_center;
    QPainterPathStroker stroker;
    stroker.setWidth(m_penWidth);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(p);
}

void StrokeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (m_points.isEmpty()) return;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setBrush(Qt::NoBrush);

    if (m_mode == Highlighter) {
        QColor ink = m_penColor;
        ink.setAlphaF(0.38 * qBound(0.0, m_fillOpacity, 1.0));
        QPen pen(ink, m_penWidth * 2.4, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(m_center);
        painter->restore();
        drawSelectionFrame(painter);
        return;
    }

    const qreal width = qMax(0.6, m_penWidth);
    if (m_smooth.size() < 2 || width < 1.2) {
        QPen pen(m_penColor, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(m_center);
        painter->restore();
        drawSelectionFrame(painter);
        return;
    }

    const qreal half = width * 0.5;
    QPainterPath left;
    QPainterPath right;
    const QVector<QPointF>& pts = m_smooth;
    left.moveTo(pts.first());
    right.moveTo(pts.first());
    for (int i = 1; i < pts.size(); ++i) {
        const QPointF& a = pts.at(i - 1);
        const QPointF& b = pts.at(i);
        QPointF tangent = b - a;
        const qreal len = std::hypot(tangent.x(), tangent.y());
        if (len < 0.0001) continue;
        tangent /= len;
        const QPointF normal(-tangent.y(), tangent.x());
        const QPointF off = normal * half;
        left.lineTo(b + off);
        right.lineTo(b - off);
    }
    QPainterPath ribbon = left;
    for (int i = pts.size() - 1; i >= 0; --i) {
        const QPointF& a = pts.at(i);
        const QPointF& b = (i > 0) ? pts.at(i - 1) : a;
        QPointF tangent = a - b;
        const qreal len = std::hypot(tangent.x(), tangent.y());
        if (len < 0.0001) continue;
        tangent /= len;
        const QPointF normal(-tangent.y(), tangent.x());
        ribbon.lineTo(a - normal * half);
    }
    ribbon.closeSubpath();

    QColor ink = m_penColor;
    ink.setAlphaF(qBound(0.0, m_fillOpacity, 1.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(ink);
    painter->drawPath(ribbon);
    painter->restore();
    drawSelectionFrame(painter);
}

QVariant StrokeItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    const QVariant result = AnnotationItem::itemChange(change, value);
    if (change == ItemPositionHasChanged || change == ItemTransformHasChanged) {
        notifyContentChanged();
    }
    return result;
}

QRectF StrokeItem::contentBounds() const {
    if (m_points.isEmpty()) return QRectF();
    return m_center.boundingRect();
}

bool StrokeItem::hitTest(const QPointF& scenePos, qreal radius) const {
    const QPointF local = mapFromScene(scenePos);
    if (shape().contains(local)) return true;
    qreal best = std::numeric_limits<qreal>::max();
    for (const QPointF& p : m_points) {
        const qreal dx = p.x() - local.x();
        const qreal dy = p.y() - local.y();
        best = qMin(best, std::hypot(dx, dy));
    }
    return best <= radius + m_penWidth * 0.5;
}

QJsonObject StrokeItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["mode"] = static_cast<int>(m_mode);
    QJsonArray pts;
    for (const QPointF& p : m_points) {
        QJsonObject o;
        o["x"] = p.x();
        o["y"] = p.y();
        pts.push_back(o);
    }
    obj["points"] = pts;
    return obj;
}

void StrokeItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_mode = static_cast<Mode>(obj.value("mode").toInt(static_cast<int>(m_mode)));
    QVector<QPointF> pts;
    const QJsonArray arr = obj.value("points").toArray();
    for (const QJsonValue& v : arr) {
        QJsonObject o = v.toObject();
        pts.push_back(QPointF(o.value("x").toDouble(), o.value("y").toDouble()));
    }
    setPoints(pts);
}
