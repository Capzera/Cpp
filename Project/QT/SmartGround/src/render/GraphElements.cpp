#include "GraphElements.h"
#include "SceneGrid.h"
#include <QFont>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
#include <QTextCursor>
#include <QFontMetrics>
#include <QEvent>
#include <QKeyEvent>
#include <cmath>

namespace {
class EditorWatcher : public QObject {
public:
    explicit EditorWatcher(QObject* parent) : QObject(parent) {}
    std::function<void(bool)> onDone;
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
                if (onDone) onDone(true);
                return true;
            }
            if (ke->key() == Qt::Key_Escape) {
                if (onDone) onDone(false);
                return true;
            }
        }
        if (event->type() == QEvent::FocusOut) {
            if (onDone) onDone(true);
            return true;
        }
        return QObject::eventFilter(obj, event);
    }
};

void installEditorWatcher(QGraphicsTextItem* editor, std::function<void(bool)> onDone) {
    static QObject* keeper = new QObject();
    EditorWatcher* watcher = new EditorWatcher(keeper);
    watcher->onDone = onDone;
    editor->installEventFilter(watcher);
    QObject::connect(editor, &QObject::destroyed, keeper, [watcher]() { watcher->deleteLater(); });
}

void destroyEditor(QPointer<QGraphicsTextItem>& slot) {
    if (slot.isNull()) return;
    QGraphicsTextItem* editor = slot.data();
    slot = nullptr;
    editor->setParentItem(nullptr);
    if (editor->scene()) editor->scene()->removeItem(editor);
    editor->deleteLater();
}
}

GraphNode::GraphNode(int _id) : id(_id), d(0), l(0), value(0), in_stack(false), color(Qt::white), highlightColor(Qt::transparent) {
    setFlags(ItemIsMovable | ItemSendsGeometryChanges);
    setZValue(1);
}
GraphNode::GraphNode(int _id, int _val) : id(_id), d(0), l(0), value(_val), in_stack(false), color(Qt::white), highlightColor(Qt::transparent) {
    setFlags(ItemIsMovable | ItemSendsGeometryChanges);
    setZValue(1);
}void GraphNode::addEdge(GraphEdge *edge) { edgeList.push_back(edge); }
GraphNode::~GraphNode() {
    std::vector<GraphEdge*> copy = edgeList;
    edgeList.clear();
    for (GraphEdge* edge : copy) {
        if (edge) edge->detach();
    }
}
void GraphNode::removeEdge(GraphEdge *edge) {
    for (int i = 0; i < static_cast<int>(edgeList.size()); ++i) {
        if (edgeList[i] == edge) {
            edgeList.erase(edgeList.begin() + i);
            return;
        }
    }
}
void GraphNode::removeAllEdges() { edgeList.clear(); }
void GraphNode::addPointer(PointerBlock *pointer) {
    for (PointerBlock* p : pointerList) if (p == pointer) return;
    pointerList.push_back(pointer);
}
void GraphNode::removePointer(PointerBlock *pointer) {
    for (int i = 0; i < static_cast<int>(pointerList.size()); ++i) {
        if (pointerList[i] == pointer) {
            pointerList.erase(pointerList.begin() + i);
            return;
        }
    }
}
QRectF GraphNode::boundingRect() const {
    double left = -84.0, top = -48.0, right = 88.0, bottom = 60.0;
    QFontMetrics subFm(QFont("Consolas", 10, QFont::Bold));
    if (!subLabel.isEmpty()) {
        double w = subFm.horizontalAdvance(subLabel) + 16.0;
        left = qMin(left, -w / 2.0);
        right = qMax(right, w / 2.0);
    }
    QFontMetrics valFm(QFont("Consolas", 10, QFont::Bold));
    if (!valueText.isEmpty()) {
        double w = valFm.horizontalAdvance(valueText) + 12.0;
        left = qMin(left, -w / 2.0);
        right = qMax(right, w / 2.0);
    }
    if (!tagText.isEmpty()) {
        double tw = valFm.horizontalAdvance(tagText) + 14.0;
        right = qMax(right, 24.0 + tw);
        top = qMin(top, -10.0);
        bottom = qMax(bottom, 10.0);
    }
    if (d > 0) {
        left = qMin(left, -60.0);
        right = qMax(right, 60.0);
        bottom = qMax(bottom, 45.0);
    }
    return QRectF(left, top, right - left, bottom - top);
}
void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(color);
    if (highlightColor != Qt::transparent) {
        painter->setPen(QPen(highlightColor, 4));
    } else {
        painter->setPen(QPen(in_stack ? Qt::red : Qt::black, in_stack ? 3 : 2));
    }
    painter->drawEllipse(-20, -20, 40, 40);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    if (!m_editor) {
        if (!customValueText.isEmpty()) {
            painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, customValueText);
        } else {
            painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, QString::number(id));
        }
    }
    if (value > 0) {
        painter->setFont(QFont("Arial", 9, QFont::Bold));
        painter->setPen(QColor(100, 100, 100));
        painter->drawText(QRectF(-20, 15, 40, 20), Qt::AlignCenter, QString("v:%1").arg(value));
    }
    if (d > 0) {
        painter->setFont(QFont("Arial", 10, QFont::Bold));
        painter->setPen(QColor(20, 80, 150));
        painter->drawText(QRectF(-60, 25, 120, 20), Qt::AlignCenter, QString("dfn:%1 low:%2").arg(d).arg(l));
    }
    if (!subLabel.isEmpty()) {
        QFont f("Consolas", 10, QFont::Bold);
        f.setPointSizeF(10.0);
        painter->setFont(f);
        QFontMetrics fm(f);
        int w = fm.horizontalAdvance(subLabel) + 16;
        painter->setPen(QColor(70, 80, 95));
        painter->drawText(QRectF(-w / 2.0, -44, w, 18), Qt::AlignCenter, subLabel);
    }
    if (!valueText.isEmpty()) {
        QFont f("Consolas", 10, QFont::Bold);
        painter->setFont(f);
        QFontMetrics fm(f);
        int w = fm.horizontalAdvance(valueText) + 12;
        painter->setPen(QColor(35, 90, 60));
        painter->drawText(QRectF(-w / 2.0, 22, w, 18), Qt::AlignCenter, valueText);
    }
    if (!tagText.isEmpty()) {
        QFont f("Consolas", 10, QFont::Bold);
        painter->setFont(f);
        QFontMetrics fm(f);
        int tw = fm.horizontalAdvance(tagText) + 14;
        QRectF tag(24, -10, tw, 20);
        painter->setBrush(tagColor.isValid() ? tagColor : QColor(255, 214, 140));
        painter->setPen(QPen(QColor(150, 110, 30), 1.4));
        painter->drawRoundedRect(tag, 5, 5);
        painter->setPen(QColor(60, 45, 10));
        painter->drawText(tag, Qt::AlignCenter, tagText);
    }
}
void GraphNode::updateData(int _d, int _l) { d = _d; l = _l; update(); }
void GraphNode::setEditable(bool on) {
    m_editable = on;
    setCursor(on ? Qt::IBeamCursor : Qt::ArrowCursor);
    setToolTip(on ? "双击可修改该结点的值" : "");
    update();
}
void GraphNode::beginEdit() {
    if (!m_editable || !m_editor.isNull()) return;
    QGraphicsScene* sc = scene();
    if (!sc) return;
    QString current = valueText.isEmpty() ? QString::number(value) : valueText;
    m_editor = sc->addText(current);
    m_editor->setFont(QFont("Consolas", 12, QFont::Bold));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    QFontMetrics fm(m_editor->font());
    m_editor->setPos(-fm.horizontalAdvance(current) / 2.0, -fm.height() / 2.0);
    m_editor->setFocus();
    QPointer<GraphNode> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installEditorWatcher(m_editor, [self, editor](bool accepted) {
        if (self.isNull()) return;
        if (self->m_editor.data() != editor) return;
        if (accepted) self->commitEdit(); else self->cancelEdit();
    });
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::Document);
    m_editor->setTextCursor(cursor);
    update();
}
void GraphNode::commitEdit() {
    if (m_editor.isNull()) return;
    QString text = m_editor->toPlainText().trimmed();
    destroyEditor(m_editor);
    update();
    if (text.isEmpty()) return;
    bool ok = false;
    int parsed = text.toInt(&ok);
    if (ok) {
        value = parsed;
        setValueText(QString::number(parsed));
    } else {
        setValueText(text);
    }
    update();
    emit valueEdited(id, text);
}
void GraphNode::cancelEdit() {
    if (m_editor.isNull()) return;
    destroyEditor(m_editor);
    update();
}
QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (GraphEdge *edge : edgeList) edge->adjust();
        for (PointerBlock *pointer : pointerList) pointer->syncToAnchor();
    }
    return QGraphicsObject::itemChange(change, value);
}
void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragging = true;
    m_dragOrigin = pos();
    m_pressScenePos = event->scenePos();
    QGraphicsObject::mousePressEvent(event);
}
void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_dragging && scene()) {
        SceneGrid::Config cfg = SceneGrid::currentConfig();
        bool freeMove = event->modifiers().testFlag(Qt::ControlModifier);
        if (cfg.enabled && !freeMove) {
            QPointF proposed = m_dragOrigin + (event->scenePos() - m_pressScenePos);
            setPos(SceneGrid::magneticAlign(scene(), this, proposed, cfg));
            event->accept();
            return;
        }
    }
    QGraphicsObject::mouseMoveEvent(event);
}
void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_dragging = false;
    QGraphicsObject::mouseReleaseEvent(event);
}
void GraphNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (m_editable) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}
GraphEdge::GraphEdge(GraphNode *s, GraphNode *d, bool dir, bool bi) : source(s), dest(d), is_directed(dir), is_bidirectional(bi), weight(0), has_weight(false), color(Qt::black), m_bendOffset(0, 0), m_dragging(false), m_hovered(false) {
    setZValue(-1);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
    setCursor(QCursor(Qt::OpenHandCursor));
}
void GraphEdge::detach() {
    if (source) source->removeEdge(this);
    if (dest) dest->removeEdge(this);
    source = nullptr;
    dest = nullptr;
}
GraphEdge::~GraphEdge() {
    if (source) source->removeEdge(this);
    if (dest) dest->removeEdge(this);
}
void GraphEdge::setWeight(int w) {
    weight = w;
    has_weight = true;
    prepareGeometryChange();
    update();
}
void GraphEdge::removeWeight() {
    has_weight = false;
    prepareGeometryChange();
    update();
}
QPointF GraphEdge::controlPoint() const {
    if (!source || !dest) return QPointF();
    return (source->pos() + dest->pos()) / 2 + m_bendOffset;
}
QPointF GraphEdge::curvePoint(qreal t) const {
    if (!source || !dest) return QPointF();
    QPointF p0 = source->pos() - pos();
    QPointF p1 = dest->pos() - pos();
    QPointF c = controlPoint() - pos();
    qreal mt = 1.0 - t;
    return p0 * (mt * mt) + c * (2 * mt * t) + p1 * (t * t);
}
QPointF GraphEdge::curveTangent(qreal t) const {
    if (!source || !dest) return QPointF(1, 0);
    QPointF p0 = source->pos() - pos();
    QPointF p1 = dest->pos() - pos();
    QPointF c = controlPoint() - pos();
    return (c - p0) * (2 * (1 - t)) + (p1 - c) * (2 * t);
}
QPointF GraphEdge::weightAnchor() const {
    if (!source || !dest) return QPointF();
    QPointF mid = curvePoint(0.5);
    QPointF tangent = curveTangent(0.5);
    qreal length = std::max<qreal>(1.0, std::hypot(tangent.x(), tangent.y()));
    QPointF normal(-tangent.y() / length, tangent.x() / length);
    return mid + normal * 16.0;
}
QPointF GraphEdge::trimmedStart(qreal radius) const {
    if (!source || !dest) return QPointF();
    QPointF p0 = source->pos();
    QPointF p1 = dest->pos();
    QPointF c = controlPoint();
    QPointF chord = p1 - p0;
    qreal len = std::hypot(chord.x(), chord.y());
    if (len < 1e-6) return p0;
    qreal r = std::min<qreal>(radius, len * 0.4);
    return p0 + chord * (r / len);
}
QPointF GraphEdge::trimmedEnd(qreal radius) const {
    if (!source || !dest) return QPointF();
    QPointF p0 = source->pos();
    QPointF p1 = dest->pos();
    QPointF chord = p1 - p0;
    qreal len = std::hypot(chord.x(), chord.y());
    if (len < 1e-6) return p1;
    qreal r = std::min<qreal>(radius, len * 0.4);
    return p1 - chord * (r / len);
}
QVariant GraphEdge::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemPositionHasChanged) {
        prepareGeometryChange();
    }
    return QGraphicsObject::itemChange(change, value);
}
QPainterPath GraphEdge::shape() const {
    QPainterPath path;
    if (!source || !dest) return path;
    path.moveTo(trimmedStart(22.0) - pos());
    path.quadTo(controlPoint() - pos(), trimmedEnd(22.0) - pos());
    QPainterPathStroker stroker;
    stroker.setWidth(12);
    return stroker.createStroke(path);
}
QRectF GraphEdge::boundingRect() const {
    if (!source || !dest) return QRectF();
    QRectF rect = QRectF(source->pos(), dest->pos()).normalized().adjusted(-30, -30, 30, 30);
    rect = rect.united(QRectF(controlPoint(), QPointF(0, 0)).adjusted(-30, -30, 30, 30));
    if (has_weight) rect = rect.united(QRectF(weightAnchor() - QPointF(30, 11), QPointF(60, 22)));
    return rect;
}
void GraphEdge::adjust() {
    if (!source || !dest) return;
    prepareGeometryChange();
    update();
}
void GraphEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastScenePos = event->scenePos();
        setCursor(QCursor(Qt::ClosedHandCursor));
        event->accept();
    } else {
        QGraphicsItem::mousePressEvent(event);
    }
}
void GraphEdge::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_dragging) {
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }
    QPointF delta = event->scenePos() - m_lastScenePos;
    m_lastScenePos = event->scenePos();
    m_bendOffset += delta;
    prepareGeometryChange();
    update();
    event->accept();
}
void GraphEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_dragging) {
        m_dragging = false;
        setCursor(QCursor(Qt::OpenHandCursor));
        event->accept();
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}
void GraphEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_hovered = true;
    if (!has_weight) setCursor(QCursor(Qt::PointingHandCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void GraphEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_hovered = false;
    setCursor(QCursor(Qt::OpenHandCursor));
    QGraphicsItem::hoverLeaveEvent(event);
}
int GraphEdge::key() const {
    if (!source || !dest) return -1;
    int u = source->id, v = dest->id;
    if (is_directed) return u * 1000 + v;
    return (std::min(u, v)) * 1000 + (std::max(u, v));
}

void GraphEdge::setEditable(bool on) {
    m_editable = on;
    if (on && has_weight) {
        setAcceptHoverEvents(true);
        setToolTip("双击可修改边权");
    } else {
        setToolTip("");
    }
    update();
}
void GraphEdge::beginEdit() {
    if (!m_editable || m_editor || !has_weight) return;
    QGraphicsScene* sc = scene();
    if (!sc) return;
    QString current = QString::number(weight);
    m_editor = sc->addText(current);
    m_editor->setFont(QFont("Consolas", 11, QFont::Bold));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    QFontMetrics fm(m_editor->font());
    QPointF anchor = weightAnchor() - pos();
    m_editor->setPos(anchor.x() - fm.horizontalAdvance(current) / 2.0, anchor.y() - fm.height() / 2.0);
    m_editor->setFocus();
    QPointer<GraphEdge> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installEditorWatcher(m_editor, [self, editor](bool accepted) {
        if (self.isNull()) return;
        if (self->m_editor.data() != editor) return;
        if (accepted) self->commitEdit(); else self->cancelEdit();
    });
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::Document);
    m_editor->setTextCursor(cursor);
    update();
}
void GraphEdge::commitEdit() {
    if (m_editor.isNull()) return;
    QString text = m_editor->toPlainText().trimmed();
    destroyEditor(m_editor);
    update();
    if (text.isEmpty()) return;
    bool ok = false;
    int parsed = text.toInt(&ok);
    if (!ok) return;
    setWeight(parsed);
    emit weightEdited(parsed);
}
void GraphEdge::cancelEdit() {
    if (m_editor.isNull()) return;
    destroyEditor(m_editor);
    update();
}
void GraphEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (m_editable && has_weight) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}
void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest) return;
    painter->setRenderHint(QPainter::Antialiasing);
    const qreal nodeRadius = 22.0;
    QPointF p0 = trimmedStart(nodeRadius) - pos();
    QPointF p1 = trimmedEnd(nodeRadius) - pos();
    QPointF c = controlPoint() - pos();
    QPainterPath path;
    path.moveTo(p0);
    path.quadTo(c, p1);
    QPen pen(QColor(color.red(), color.green(), color.blue(), m_hovered ? 255 : 220), m_hovered ? 3.5 : m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(path);
    auto drawArrow = [&](QPointF tip, QPointF dir) {
        qreal length = std::hypot(dir.x(), dir.y());
        if (length < 1e-6) return;
        QPointF tangent = dir / length;
        qreal arrowSize = 15.0;
        qreal halfWidth = 7.0;
        QPointF base = tip - tangent * arrowSize;
        qreal ang = std::atan2(tangent.y(), tangent.x());
        QPointF p1a = base + QPointF(halfWidth * std::cos(ang + M_PI / 2.0), halfWidth * std::sin(ang + M_PI / 2.0));
        QPointF p2a = base + QPointF(halfWidth * std::cos(ang - M_PI / 2.0), halfWidth * std::sin(ang - M_PI / 2.0));
        QPolygonF head;
        head << tip << p1a << p2a;
        QPainterPath headPath;
        headPath.addPolygon(head);
        painter->setBrush(color);
        painter->setPen(QPen(QColor(255, 255, 255, 210), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(headPath);
    };
    if (is_directed || is_bidirectional) {
        QPointF tip = trimmedEnd(nodeRadius);
        drawArrow(tip - pos(), dest->pos() - tip);
    }
    if (is_bidirectional) {
        QPointF tip = trimmedStart(nodeRadius);
        drawArrow(tip - pos(), source->pos() - tip);
    }
    if (has_weight && !m_editor) {
        QPointF anchor = weightAnchor();
        painter->setPen(color.darker(120));
        painter->setFont(QFont("Consolas", 11, QFont::Bold));
        painter->drawText(QRectF(anchor.x() - 30, anchor.y() - 11, 60, 22), Qt::AlignCenter, QString::number(weight));
    }
}
StackBlock::StackBlock(int _id) : id(_id) {
    setOpacity(0);
    setZValue(2);
}
QRectF StackBlock::boundingRect() const { return QRectF(-30, -15, 60, 30); }
void StackBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(QColor(135, 206, 235));
    painter->setPen(QPen(Qt::black, 2));
    painter->drawRect(-30, -15, 60, 30);
    painter->setFont(QFont("Arial", 11, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(id));
}
ArrayBlock::ArrayBlock(int _id, int _val, QString _label) : id(_id), value(_val), showValue(true), color(Qt::white), customLabel(_label), showLabel(true) {
    setZValue(1);
}
void ArrayBlock::setEditable(bool on) {
    m_editable = on;
    setCursor(on ? Qt::IBeamCursor : Qt::ArrowCursor);
    setToolTip(on ? "双击可修改该格的值" : "");
    update();
}
QString ArrayBlock::displayText() const {
    return customValue.isEmpty() ? QString::number(value) : customValue;
}
void ArrayBlock::beginEdit() {
    if (!m_editable || !m_editor.isNull()) return;
    QGraphicsScene* sc = scene();
    if (!sc) return;
    QString current = displayText();
    m_editor = sc->addText(current);
    m_editor->setFont(QFont("Consolas", 14, QFont::Bold));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    QFontMetrics fm(m_editor->font());
    m_editor->setPos(m_cellW / 2.0 - fm.horizontalAdvance(current) / 2.0, m_cellH / 2.0 - fm.height() / 2.0);
    m_editor->setFocus();
    QPointer<ArrayBlock> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installEditorWatcher(m_editor, [self, editor](bool accepted) {
        if (self.isNull()) return;
        if (self->m_editor.data() != editor) return;
        if (accepted) self->commitEdit(); else self->cancelEdit();
    });
    QTextCursor cursor = m_editor->textCursor();
    cursor.select(QTextCursor::Document);
    m_editor->setTextCursor(cursor);
    update();
}
void ArrayBlock::commitEdit() {
    if (m_editor.isNull()) return;
    QString text = m_editor->toPlainText().trimmed();
    destroyEditor(m_editor);
    update();
    if (text.isEmpty()) return;
    bool ok = false;
    int parsed = text.toInt(&ok);
    if (ok) {
        value = parsed;
        customValue.clear();
    } else {
        setCustomValue(text);
    }
    showValue = true;
    update();
    emit valueEdited(id, text);
}
void ArrayBlock::cancelEdit() {
    if (m_editor.isNull()) return;
    destroyEditor(m_editor);
    update();
}
void ArrayBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (m_editable) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}
QRectF ArrayBlock::boundingRect() const {
    return QRectF(-1.0, -27.0, m_cellW + 2.0, m_cellH + 28.0);
}

void ArrayBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(color);
    QPen pen(Qt::black, 2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawRect(QRectF(0.0, 0.0, m_cellW, m_cellH));

    painter->setFont(QFont("Consolas", 14, QFont::Bold));
    if (showValue && m_editor.isNull()) {
        QString text = customValue.isEmpty() ? QString::number(value) : customValue;
        painter->drawText(QRectF(0, 0, m_cellW, m_cellH), Qt::AlignCenter, text);
    }

    if (!showLabel) return;
    painter->setFont(QFont("Consolas", 12));
    QString topText = customLabel.isEmpty() ? QString::number(id) : customLabel;
    painter->drawText(QRectF(0, -25, m_cellW, 20), Qt::AlignHCenter | Qt::AlignBottom, topText);
}
FloatingText::FloatingText(const QString& t) : text(t) { setZValue(3); }
QRectF FloatingText::boundingRect() const {
    QFontMetrics fm(QFont("Consolas", 14, QFont::Bold));
    double w = qMax(60.0, fm.horizontalAdvance(text) + 10.0);
    return QRectF(0, 0, w, 40);
}

void FloatingText::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setFont(QFont("Consolas", 14, QFont::Bold));
    painter->setPen(Qt::black);
    QFontMetrics fm(painter->font());
    double w = qMax(60.0, fm.horizontalAdvance(text) + 10.0);
    painter->drawText(QRectF(0, 0, w, 40), Qt::AlignCenter, text);
}
PointerBlock::PointerBlock(const QString& _label, QColor _color) : label(_label), color(_color) {
    setZValue(3);
}
double PointerBlock::labelWidth() const {
    QFontMetrics fm(QFont("Consolas", 14, QFont::Bold));
    double adv = fm.horizontalAdvance(label);
    if (adv <= 0.0) {
        adv = label.size() * 9.0;
    }
    return qMax(20.0, adv + 8.0);
}
QRectF PointerBlock::boundingRect() const {
    double w = labelWidth();
    return QRectF(-w / 2.0, 0, w, 47);
}
void PointerBlock::setAnchorNode(GraphNode* node, bool syncNow) {
    if (m_anchor == node) return;
    if (m_anchor) m_anchor->removePointer(this);
    m_anchor = node;
    if (m_anchor) {
        m_anchor->addPointer(this);
        if (syncNow) syncToAnchor();
    }
}
void PointerBlock::syncToAnchor() {
    if (!m_anchor) return;
    int index = 0;
    int total = 0;
    for (PointerBlock* p : m_anchor->pointerList) {
        if (!p || p->opacity() < 0.05) continue;
        ++total;
        if (p == this) index = total - 1;
    }
    if (total <= 1) {
        setPos(m_anchor->pos() + m_anchorOffset);
        return;
    }
    double spread = 16.0;
    double delta = (index - (total - 1) / 2.0) * spread;
    setPos(m_anchor->pos() + m_anchorOffset + QPointF(delta, 0));
}void PointerBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(0, 25, 0, 3);
    painter->drawLine(0, 3, -7, 13);
    painter->drawLine(0, 3, 7, 13);
    QFont f("Consolas", 14, QFont::Bold);
    painter->setFont(f);
    double w = labelWidth();
    painter->drawText(QRectF(-w / 2.0, 25, w, 20), Qt::AlignCenter, label);
}