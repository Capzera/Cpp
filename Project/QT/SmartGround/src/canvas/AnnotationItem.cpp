#include "AnnotationItem.h"
#include <QPainter>
#include <QJsonValue>
#include <QJsonObject>
#include <QCursor>
#include <QUuid>
#include <cmath>

AnnotationItem::AnnotationItem(QGraphicsItem* parent) : QGraphicsObject(parent) {
    m_uid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    setFlag(ItemIsMovable, true);
    setFlag(ItemSendsGeometryChanges, true);
    setFlag(ItemIsSelectable, true);
    setZValue(10);
    setCursor(QCursor(Qt::OpenHandCursor));
}

QJsonObject AnnotationItem::toJson() const {
    QJsonObject obj;
    obj["uid"] = m_uid;
    obj["type"] = type();
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["penColor"] = colorToJson(m_penColor);
    obj["penWidth"] = m_penWidth;
    obj["fillOpacity"] = m_fillOpacity;
    obj["locked"] = m_locked;
    obj["scale"] = scale();
    return obj;
}

void AnnotationItem::fromJson(const QJsonObject& obj) {
    if (obj.contains("uid")) m_uid = obj.value("uid").toString();
    setPenColor(colorFromJson(obj.value("penColor"), m_penColor));
    setPenWidth(obj.value("penWidth").toDouble(m_penWidth));
    setFillOpacity(obj.value("fillOpacity").toDouble(m_fillOpacity));
    setLocked(obj.value("locked").toBool(false));
    setPos(obj.value("x").toDouble(), obj.value("y").toDouble());
    const qreal s = obj.value("scale").toDouble(1.0);
    if (s > 0.001 && !qFuzzyCompare(s, 1.0)) {
        setTransformOriginPoint(QPointF(0, 0));
        setScale(s);
    }
}

void AnnotationItem::setPenColor(const QColor& c) {
    if (m_penColor == c) return;
    m_penColor = c;
    applyStyle();
    update();
}

void AnnotationItem::setPenWidth(qreal w) {
    if (qFuzzyCompare(m_penWidth, w)) return;
    m_penWidth = w;
    applyStyle();
    update();
}

void AnnotationItem::setFillOpacity(qreal v) {
    if (qFuzzyCompare(m_fillOpacity, v)) return;
    m_fillOpacity = v;
    applyStyle();
    update();
}

void AnnotationItem::setLocked(bool on) {
    if (m_locked == on) return;
    m_locked = on;
    setFlag(ItemIsMovable, !on);
    setCursor(on ? QCursor(Qt::ArrowCursor) : QCursor(Qt::OpenHandCursor));
    update();
}

void AnnotationItem::scaleBy(qreal factor, const QPointF& pivot) {
    scaleAbout(pivot, factor, factor);
}

AnnotationItem::Frame AnnotationItem::captureFrame() const {
    const QTransform t = transform();
    Frame frame;
    frame.origin = pos();
    frame.basisX = t.map(QPointF(1, 0)) - t.map(QPointF(0, 0));
    frame.basisY = t.map(QPointF(0, 1)) - t.map(QPointF(0, 0));
    return frame;
}

void AnnotationItem::applyFrame(const Frame& frame) {
    prepareGeometryChange();
    setTransformOriginPoint(QPointF(0, 0));
    const qreal sx = std::hypot(frame.basisX.x(), frame.basisX.y());
    const qreal sy = std::hypot(frame.basisY.x(), frame.basisY.y());
    const qreal uniform = qMax(sx, sy);
    if (uniform > 0.0001 && qFuzzyCompare(sx / uniform, sy / uniform)) {
        setScale(uniform);
    } else {
        QTransform t;
        t.setMatrix(frame.basisX.x(), frame.basisY.x(), 0.0,
                    frame.basisX.y(), frame.basisY.y(), 0.0,
                    0.0, 0.0, 1.0);
        setTransform(t);
    }
    setPos(frame.origin);
}

void AnnotationItem::scaleFrameAbout(const Frame& start, const QPointF& scenePivot, qreal sx, qreal sy) {
    if (sx <= 0.0001 || sy <= 0.0001) return;
    Frame frame = start;
    frame.basisX = frame.basisX * sx;
    frame.basisY = frame.basisY * sy;
    frame.origin = QPointF(scenePivot.x() + (start.origin.x() - scenePivot.x()) * sx,
                           scenePivot.y() + (start.origin.y() - scenePivot.y()) * sy);
    applyFrame(frame);
    notifyContentChanged();
}

void AnnotationItem::scaleAbout(const QPointF& scenePivot, qreal sx, qreal sy) {
    if (sx <= 0.0001 || sy <= 0.0001) return;
    scaleFrameAbout(captureFrame(), scenePivot, sx, sy);
}

void AnnotationItem::notifyContentChanged() {
    update();
    emit contentChanged();
}

bool AnnotationItem::hitTest(const QPointF& scenePos, qreal radius) const {
    const QPointF local = mapFromScene(scenePos);
    if (shape().contains(local)) return true;
    const QRectF b = boundingRect();
    return b.adjusted(-radius, -radius, radius, radius).contains(local);
}
void AnnotationItem::drawSelectionFrame(QPainter* painter) const {
    if (!m_selected) return;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(59, 125, 221), 1.4, Qt::DashLine);
    pen.setDashPattern({4.0, 3.0});
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(contentBounds().adjusted(-5.0, -5.0, 5.0, 5.0));
    painter->restore();
}

QVariant AnnotationItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemSelectedHasChanged) {
        m_selected = value.toBool();
        update();
    }
    return QGraphicsObject::itemChange(change, value);
}

QJsonObject AnnotationItem::colorToJson(const QColor& c) {
    QJsonObject obj;
    obj["r"] = c.red();
    obj["g"] = c.green();
    obj["b"] = c.blue();
    obj["a"] = c.alpha();
    return obj;
}

QColor AnnotationItem::colorFromJson(const QJsonValue& v, const QColor& fallback) {
    if (!v.isObject()) return fallback;
    QJsonObject obj = v.toObject();
    return QColor(obj.value("r").toInt(fallback.red()),
                  obj.value("g").toInt(fallback.green()),
                  obj.value("b").toInt(fallback.blue()),
                  obj.value("a").toInt(fallback.alpha()));
}
