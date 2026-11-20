#include "ShapeItem.h"
#include <QPainter>
#include <QJsonArray>
#include <QBuffer>
#include <cmath>

ShapeItem::ShapeItem(Kind kind, QGraphicsItem* parent) : AnnotationItem(parent) {
    m_kind = kind;
    setZValue(10);
    applyStyle();
}

void ShapeItem::setKind(Kind k) {
    if (m_kind == k) return;
    m_kind = k;
    prepareGeometryChange();
    applyStyle();
    notifyContentChanged();
}

void ShapeItem::setGeometry(const QPointF& start, const QPointF& end) {
    m_start = start;
    m_end = end;
    prepareGeometryChange();
    notifyContentChanged();
}

void ShapeItem::setMosaicCells(const QVector<QPolygonF>& cells) {
    m_cells = cells;
    prepareGeometryChange();
    notifyContentChanged();
}

void ShapeItem::setMosaicImage(const QImage& image, const QRectF& region) {
    m_mosaicImage = image;
    m_mosaicRegion = region;
    prepareGeometryChange();
    notifyContentChanged();
}

QPainterPath ShapeItem::buildOutline() const {
    QPainterPath path;
    const QPointF a = m_start;
    const QPointF b = m_end;
    switch (m_kind) {
    case Line:
        path.moveTo(a);
        path.lineTo(b);
        break;
    case Rectangle: {
        const QRectF r = QRectF(a, b).normalized();
        path.addRect(r);
        break;
    }
    case Ellipse: {
        const QRectF r = QRectF(a, b).normalized();
        path.addEllipse(r);
        break;
    }
    case Triangle: {
        const QRectF r = QRectF(a, b).normalized();
        QPolygonF poly;
        poly << QPointF(r.center().x(), r.top())
             << QPointF(r.right(), r.bottom())
             << QPointF(r.left(), r.bottom());
        path.addPolygon(poly);
        path.closeSubpath();
        break;
    }
    case Arrow: {
        path.moveTo(a);
        path.lineTo(b);
        break;
    }
    case Mosaic: {
        for (const QPolygonF& cell : m_cells) {
            path.addPolygon(cell);
        }
        break;
    }
    }
    return path;
}

QPolygonF ShapeItem::arrowPolygon(const QPointF& from, const QPointF& to) const {
    QPolygonF poly;
    const qreal dx = to.x() - from.x();
    const qreal dy = to.y() - from.y();
    const qreal len = std::hypot(dx, dy);
    if (len < 0.001) return poly;
    const qreal ux = dx / len;
    const qreal uy = dy / len;
    const qreal head = qMax(12.0, m_penWidth * 3.6);
    const qreal wing = head * 0.36;
    const QPointF base = to - QPointF(ux * head, uy * head);
    const QPointF normal(-uy, ux);
    poly << to
         << base + normal * wing
         << base - normal * wing;
    return poly;
}

bool ShapeItem::hitTest(const QPointF& scenePos, qreal radius) const {
    const QPointF local = mapFromScene(scenePos);
    if (shape().contains(local)) return true;
    const QRectF b = boundingRect();
    return b.adjusted(-radius, -radius, radius, radius).contains(local);
}

QRectF ShapeItem::contentBounds() const {
    if (m_kind == Mosaic && !m_cells.isEmpty()) {
        QRectF r;
        for (const QPolygonF& cell : m_cells) {
            r = r.isNull() ? cell.boundingRect() : r.united(cell.boundingRect());
        }
        return r;
    }
    return QRectF(m_start, m_end).normalized();
}

QRectF ShapeItem::boundingRect() const {
    const QRectF r = contentBounds();
    const qreal pad = m_penWidth + 4.0;
    return r.adjusted(-pad, -pad, pad, pad);
}

QPainterPath ShapeItem::shape() const {
    QPainterPath p = buildOutline();
    QPainterPathStroker stroker;
    stroker.setWidth(m_penWidth);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(p);
}

void ShapeItem::applyStyle() {
    setAcceptedMouseButtons(Qt::NoButton);
}

void ShapeItem::setFilled(bool on) {
    if (m_filled == on) return;
    m_filled = on;
    prepareGeometryChange();
    update();
    notifyContentChanged();
}

void ShapeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const QPainterPath outline = buildOutline();

    if (m_kind == Mosaic) {
        if (!m_mosaicImage.isNull() && !m_mosaicRegion.isEmpty()) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawImage(m_mosaicRegion, m_mosaicImage);
        } else {
            painter->setPen(Qt::NoPen);
            painter->setBrush(m_penColor);
            for (const QPolygonF& cell : m_cells) {
                painter->drawPolygon(cell);
            }
        }
        painter->restore();
        drawSelectionFrame(painter);
        return;
    }

    QPen pen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    QBrush fillBrush(QColor(255, 255, 255, 0));
    if (m_filled) {
        QColor fillColor = m_penColor;
        fillColor.setAlphaF(m_fillOpacity);
        fillBrush = QBrush(fillColor);
    }

    switch (m_kind) {
    case Line:
        painter->setBrush(Qt::NoBrush);
        painter->drawLine(m_start, m_end);
        break;
    case Rectangle: {
        painter->setBrush(fillBrush);
        painter->drawRect(QRectF(m_start, m_end).normalized());
        break;
    }
    case Ellipse: {
        painter->setBrush(fillBrush);
        painter->drawEllipse(QRectF(m_start, m_end).normalized());
        break;
    }
    case Triangle: {
        const QRectF r = QRectF(m_start, m_end).normalized();
        QPolygonF poly;
        poly << QPointF(r.center().x(), r.top())
             << QPointF(r.right(), r.bottom())
             << QPointF(r.left(), r.bottom());
        painter->setBrush(fillBrush);
        painter->drawPolygon(poly);
        break;
    }
    case Arrow: {
        painter->setBrush(Qt::NoBrush);
        const qreal dx = m_end.x() - m_start.x();
        const qreal dy = m_end.y() - m_start.y();
        const qreal len = std::hypot(dx, dy);
        if (len > 0.001) {
            const qreal ux = dx / len;
            const qreal uy = dy / len;
            const qreal head = qMax(12.0, m_penWidth * 3.6);
            const QPointF base = m_end - QPointF(ux * head, uy * head);
            painter->drawLine(m_start, base);
            const QPolygonF headPoly = arrowPolygon(m_start, m_end);
            if (!headPoly.isEmpty()) {
                painter->setPen(Qt::NoPen);
                painter->setBrush(m_penColor);
                painter->drawPolygon(headPoly);
            }
        } else {
            painter->drawLine(m_start, m_end);
        }
        break;
    }
    case Mosaic:
        break;
    }
    Q_UNUSED(outline);
    painter->restore();
    drawSelectionFrame(painter);
}

AnnotationItem* ShapeItem::cloneItem() const {
    auto* copy = new ShapeItem(m_kind);
    copy->setGeometry(m_start, m_end);
    copy->setMosaicCells(m_cells);
    if (!m_mosaicImage.isNull()) copy->setMosaicImage(m_mosaicImage, m_mosaicRegion);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setFilled(m_filled);
    copy->setLocked(m_locked);
    return copy;
}

QJsonObject ShapeItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["kind"] = static_cast<int>(m_kind);
    obj["sx"] = m_start.x();
    obj["sy"] = m_start.y();
    obj["ex"] = m_end.x();
    obj["ey"] = m_end.y();
    obj["filled"] = m_filled;
    QJsonArray cells;
    for (const QPolygonF& cell : m_cells) {
        QJsonArray pts;
        for (const QPointF& p : cell) {
            QJsonObject o;
            o["x"] = p.x();
            o["y"] = p.y();
            pts.push_back(o);
        }
        QJsonObject c;
        c["pts"] = pts;
        cells.push_back(c);
    }
    obj["cells"] = cells;
    if (!m_mosaicImage.isNull()) {
        QByteArray png;
        QBuffer buffer(&png);
        buffer.open(QIODevice::WriteOnly);
        m_mosaicImage.save(&buffer, "PNG");
        obj["mosaicPng"] = QString::fromLatin1(png.toBase64());
        obj["mosaicRx"] = m_mosaicRegion.x();
        obj["mosaicRy"] = m_mosaicRegion.y();
        obj["mosaicRw"] = m_mosaicRegion.width();
        obj["mosaicRh"] = m_mosaicRegion.height();
    }
    return obj;
}

void ShapeItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_kind = static_cast<Kind>(obj.value("kind").toInt(m_kind));
    m_start = QPointF(obj.value("sx").toDouble(), obj.value("sy").toDouble());
    m_end = QPointF(obj.value("ex").toDouble(), obj.value("ey").toDouble());
    m_filled = obj.value("filled").toBool(m_filled);
    QVector<QPolygonF> cells;
    const QJsonArray arr = obj.value("cells").toArray();
    for (const QJsonValue& v : arr) {
        QJsonObject c = v.toObject();
        QPolygonF poly;
        const QJsonArray pts = c.value("pts").toArray();
        for (const QJsonValue& pv : pts) {
            QJsonObject o = pv.toObject();
            poly << QPointF(o.value("x").toDouble(), o.value("y").toDouble());
        }
        cells.push_back(poly);
    }
    m_cells = cells;
    const QString png = obj.value("mosaicPng").toString();
    if (!png.isEmpty()) {
        const QByteArray raw = QByteArray::fromBase64(png.toLatin1());
        QImage image;
        if (image.loadFromData(raw, "PNG")) {
            setMosaicImage(image, QRectF(obj.value("mosaicRx").toDouble(),
                                         obj.value("mosaicRy").toDouble(),
                                         obj.value("mosaicRw").toDouble(),
                                         obj.value("mosaicRh").toDouble()));
        }
    }
    prepareGeometryChange();
    applyStyle();
    update();
}
