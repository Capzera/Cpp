#include "StrokeItem.h"
#include <QPainter>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include <limits>
#include <cmath>

namespace {

constexpr qreal kMinSampleDistance = 0.4;
constexpr qreal kResampleSpacing = 3.0;
constexpr qreal kCentripetalAlpha = 0.5;

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
    m_center = buildCenterPath();
    update();
}

QVector<QPointF> StrokeItem::resampledPoints() const {
    QVector<QPointF> out;
    if (m_points.isEmpty()) return out;
    out << m_points.first();
    for (int i = 1; i < m_points.size(); ++i) {
        const QPointF a = m_points.at(i - 1);
        const QPointF b = m_points.at(i);
        const qreal len = std::hypot(b.x() - a.x(), b.y() - a.y());
        if (len < 1e-6) continue;
        const int steps = qMax(1, static_cast<int>(std::ceil(len / kResampleSpacing)));
        for (int s = 1; s <= steps; ++s) {
            const qreal t = static_cast<qreal>(s) / steps;
            out << (a + (b - a) * t);
        }
    }
    return out;
}

QVector<QPointF> StrokeItem::smoothPoints() const {
    QVector<QPointF> out;
    const QVector<QPointF> pts = resampledPoints();
    if (pts.isEmpty()) return out;
    if (pts.size() == 1) {
        out << pts.first();
        return out;
    }
    if (pts.size() == 2) {
        out << pts.at(0) << pts.at(1);
        return out;
    }
    for (int i = 0; i < pts.size() - 1; ++i) {
        const QPointF p0 = pts.at(i > 0 ? i - 1 : 0);
        const QPointF p1 = pts.at(i);
        const QPointF p2 = pts.at(i + 1);
        const QPointF p3 = pts.at(i + 2 < pts.size() ? i + 2 : i + 1);

        const QPointF c1 = p1 + (p2 - p0) / 6.0;
        const QPointF c2 = p2 - (p3 - p1) / 6.0;

        for (int s = 0; s < 8; ++s) {
            const qreal t = static_cast<qreal>(s) / 8.0;
            const qreal mt = 1.0 - t;
            const qreal a = mt * mt * mt;
            const qreal b = 3.0 * mt * mt * t;
            const qreal c = 3.0 * mt * t * t;
            const qreal d = t * t * t;
            out << QPointF(a * p1.x() + b * c1.x() + c * c2.x() + d * p2.x(),
                           a * p1.y() + b * c1.y() + c * c2.y() + d * p2.y());
        }
    }
    out << pts.last();
    return out;
}

QPainterPath StrokeItem::buildCenterPath() const {
    QPainterPath path;
    const QVector<QPointF> pts = resampledPoints();
    if (pts.isEmpty()) return path;
    path.moveTo(pts.first());
    if (pts.size() == 1) {
        path.lineTo(pts.first() + QPointF(0.01, 0.0));
        return path;
    }
    if (pts.size() == 2) {
        path.lineTo(pts.at(1));
        return path;
    }

    const int n = pts.size();
    for (int i = 0; i < n - 1; ++i) {
        const QPointF p1 = pts.at(i);
        const QPointF p2 = pts.at(i + 1);
        const QPointF p0 = (i > 0) ? pts.at(i - 1) : (p1 * 2.0 - p2);
        const QPointF p3 = (i + 2 < n) ? pts.at(i + 2) : (p2 * 2.0 - p1);

        const qreal d1 = std::pow(std::hypot(p1.x() - p0.x(), p1.y() - p0.y()), kCentripetalAlpha);
        const qreal d2 = std::pow(std::hypot(p2.x() - p1.x(), p2.y() - p1.y()), kCentripetalAlpha);
        const qreal d3 = std::pow(std::hypot(p3.x() - p2.x(), p3.y() - p2.y()), kCentripetalAlpha);
        const qreal e1 = qMax(d1, 1e-4);
        const qreal e2 = qMax(d2, 1e-4);
        const qreal e3 = qMax(d3, 1e-4);

        const qreal k1 = 3.0 * e1 * (e1 + e2);
        const qreal k2 = 3.0 * e3 * (e3 + e2);

        const QPointF c1(
            (e1 * e1 * p2.x() - e2 * e2 * p0.x() + (2.0 * e1 * e1 + 3.0 * e1 * e2 + e2 * e2) * p1.x()) / k1,
            (e1 * e1 * p2.y() - e2 * e2 * p0.y() + (2.0 * e1 * e1 + 3.0 * e1 * e2 + e2 * e2) * p1.y()) / k1);
        const QPointF c2(
            (e3 * e3 * p1.x() - e2 * e2 * p3.x() + (2.0 * e3 * e3 + 3.0 * e3 * e2 + e2 * e2) * p2.x()) / k2,
            (e3 * e3 * p1.y() - e2 * e2 * p3.y() + (2.0 * e3 * e3 + 3.0 * e3 * e2 + e2 * e2) * p2.y()) / k2);

        path.cubicTo(c1, c2, p2);
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
    painter->setBrush(Qt::NoBrush);

    const qreal width = qMax(0.6, m_penWidth);
    if (m_mode == Highlighter) {
        QColor ink = m_penColor;
        ink.setAlphaF(0.36 * qBound(0.0, m_fillOpacity, 1.0));
        QPen pen(ink, width * 2.4, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(m_center);
    } else {
        QColor ink = m_penColor;
        ink.setAlphaF(qBound(0.0, m_fillOpacity, 1.0));
        QPen pen(ink, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(m_center);
    }
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

AnnotationItem* StrokeItem::cloneItem() const {
    auto* copy = new StrokeItem();
    copy->setMode(m_mode);
    copy->setPoints(m_points);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setLocked(m_locked);
    return copy;
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
