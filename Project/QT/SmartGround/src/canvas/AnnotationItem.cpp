#include "AnnotationItem.h"
#include <QPainter>
#include <QJsonValue>
#include <QJsonObject>
#include <QCursor>
#include <QUuid>

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
    if (s > 0.001 && !qFuzzyCompare(s, 1.0)) setScale(s);
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
    if (factor <= 0.001) return;
    const QPointF localPivot = mapFromScene(pivot);
    prepareGeometryChange();
    setTransformOriginPoint(localPivot);
    setScale(scale() * factor);
    notifyContentChanged();
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
