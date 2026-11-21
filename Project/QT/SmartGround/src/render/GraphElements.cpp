#include "GraphElements.h"
#include "SceneGrid.h"
#include "core/PointerGeometry.h"
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
#include "ui/UiFonts.h"

namespace {
class EditorTextItem : public QGraphicsTextItem {
public:
    explicit EditorTextItem(std::shared_ptr<bool> flag) : m_flag(std::move(flag)) {}
    ~EditorTextItem() override { *m_flag = true; }
private:
    std::shared_ptr<bool> m_flag;
};

class EditorWatcher : public QObject {
public:
    EditorWatcher(QObject* parent, std::shared_ptr<bool> flag) : QObject(parent), m_flag(std::move(flag)) {}
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
            if (m_flag && *m_flag) return true;
            if (onDone) onDone(true);
            return true;
        }
        return QObject::eventFilter(obj, event);
    }
private:
    std::shared_ptr<bool> m_flag;
};

void installEditorWatcher(QGraphicsTextItem* editor, std::shared_ptr<bool> flag, std::function<void(bool)> onDone) {
    static QObject* keeper = new QObject();
    EditorWatcher* watcher = new EditorWatcher(keeper, flag);
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
    if (m_guard) *m_guard->tearingDown = true;
    if (!m_editor.isNull()) {
        QGraphicsTextItem* ed = m_editor.data();
        m_editor = nullptr;
        ed->setParentItem(nullptr);
        delete ed;
    }
    std::vector<QPointer<GraphEdge>> copy = edgeList;
    edgeList.clear();
    for (QPointer<GraphEdge>& edge : copy) {
        if (edge) edge->detach();
    }
    std::vector<QPointer<PointerBlock>> ptrCopy = pointerList;
    pointerList.clear();
    for (QPointer<PointerBlock>& ptr : ptrCopy) {
        if (ptr) ptr->clearAnchor();
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
    QFontMetrics subFm(UiFonts::mono(10, true));
    if (!subLabel.isEmpty()) {
        double w = subFm.horizontalAdvance(subLabel) + 16.0;
        left = qMin(left, -w / 2.0);
        right = qMax(right, w / 2.0);
    }
    QFontMetrics valFm(UiFonts::mono(10, true));
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
    if (!qFuzzyCompare(m_scale, 1.0)) {
        painter->scale(m_scale, m_scale);
    }
    painter->setBrush(color);
    if (highlightColor != Qt::transparent) {
        painter->setPen(QPen(highlightColor, 4));
    } else {
        painter->setPen(QPen(in_stack ? Qt::red : Qt::black, in_stack ? 3 : 2));
    }
    painter->drawEllipse(-20, -20, 40, 40);
    if (!m_editor) {
        if (!centerValueText.isEmpty()) {
            painter->setPen(Qt::black);
            painter->setFont(UiFonts::mono(12, true));
            painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, centerValueText);
        } else if (!customValueText.isEmpty()) {
            painter->setPen(Qt::black);
            painter->setFont(UiFonts::mono(12, true));
            painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, customValueText);
        } else {
            painter->setPen(Qt::black);
            painter->setFont(UiFonts::mono(12, true));
            painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, QString::number(id));
        }
    }
    if (value > 0 && customValueText.isEmpty() && valueText.isEmpty() && centerValueText.isEmpty()) {
        painter->setFont(UiFonts::mono(9, true));
        painter->setPen(QColor(100, 100, 100));
        painter->drawText(QRectF(-20, 15, 40, 20), Qt::AlignCenter, QString("v:%1").arg(value));
    }
    if (d > 0) {
        painter->setFont(UiFonts::mono(10, true));
        painter->setPen(QColor(20, 80, 150));
        painter->drawText(QRectF(-60, 25, 120, 20), Qt::AlignCenter, QString("dfn:%1 low:%2").arg(d).arg(l));
    }
    if (!subLabel.isEmpty()) {
        QFont f = UiFonts::mono(10, true);
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
    QString current = !centerValueText.isEmpty() ? centerValueText
                   : (!customValueText.isEmpty() ? customValueText
                   : (!valueText.isEmpty() ? valueText : QString::number(value)));
    m_editor = new EditorTextItem(m_guard->tearingDown);
    sc->addItem(m_editor);
    m_editor->setPlainText(current);
    m_editor->setFont(UiFonts::mono(12, true));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    QFontMetrics fm(m_editor->font());
    m_editor->setPos(-fm.horizontalAdvance(current) / 2.0, -fm.height() / 2.0);
    m_editor->setFocus();
    QPointer<GraphNode> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installEditorWatcher(m_editor, m_guard->tearingDown, [self, editor](bool accepted) {
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
    const QString shown = ok ? QString::number(parsed) : text;
    if (ok) value = parsed;
    if (!customValueText.isEmpty()) {
        setCustomValueText(shown);
        setCenterText(shown);
    } else {
        setCenterText(shown);
        if (!valueText.isEmpty() && valueText != text) setValueText(QString());
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
    if (m_guard) *m_guard->tearingDown = true;
    if (!m_editor.isNull()) {
        QGraphicsTextItem* ed = m_editor.data();
        m_editor = nullptr;
        ed->setParentItem(nullptr);
        delete ed;
    }
    if (source) source->removeEdge(this);
    if (dest) dest->removeEdge(this);
}void GraphEdge::setWeight(int w) {
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
QPointF GraphEdge::quadPoint(const QPointF& p0, const QPointF& c, const QPointF& p1, qreal t) {
    const qreal mt = 1.0 - t;
    return p0 * (mt * mt) + c * (2 * mt * t) + p1 * (t * t);
}
void GraphEdge::splitQuad(const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal s,
                          QPointF& outL0, QPointF& outL1, QPointF& outL2,
                          QPointF& outR0, QPointF& outR1, QPointF& outR2) {
    const QPointF l1 = p0 + (p1 - p0) * s;
    const QPointF l2 = p1 + (p2 - p1) * s;
    const QPointF m = l1 + (l2 - l1) * s;
    outL0 = p0; outL1 = l1; outL2 = m;
    outR0 = m; outR1 = l2; outR2 = p2;
}
void GraphEdge::quadSubPath(const QPointF& p0, const QPointF& p1, const QPointF& p2,
                            qreal from, qreal to, QPainterPath& out) {
    const qreal a = qBound<qreal>(0.0, from, 1.0);
    const qreal b = qBound<qreal>(a, to, 1.0);
    QPointF a0, a1, a2, b0, b1, b2;
    splitQuad(p0, p1, p2, a, a0, a1, a2, b0, b1, b2);
    const qreal span = 1.0 - a;
    if (span < 1e-6) {
        out.moveTo(b0);
        out.lineTo(b2);
        return;
    }
    const qreal u = qBound<qreal>(0.0, (b - a) / span, 1.0);
    QPointF c0, c1, c2, d0, d1, d2;
    splitQuad(b0, b1, b2, u, c0, c1, c2, d0, d1, d2);
    out.moveTo(c0);
    out.quadTo(c1, c2);
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
QPointF GraphEdge::pathLabelAnchor() const {
    if (!source || !dest) return QPointF();
    return (source->pos() + dest->pos()) / 2.0;
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
QPointF GraphEdge::scenePointAt(qreal t) const {
    if (!source || !dest) return QPointF();
    const qreal radius = 22.0;
    const QPointF p0 = trimmedStart(radius);
    const QPointF p1 = trimmedEnd(radius);
    const QPointF c = controlPoint();
    const qreal mt = 1.0 - t;
    return p0 * (mt * mt) + c * (2 * mt * t) + p1 * (t * t);
}
QPointF GraphEdge::sceneTangentAt(qreal t) const {
    if (!source || !dest) return QPointF(1, 0);
    const qreal radius = 22.0;
    const QPointF p0 = trimmedStart(radius);
    const QPointF p1 = trimmedEnd(radius);
    const QPointF c = controlPoint();
    return (c - p0) * (2 * (1 - t)) + (p1 - c) * (2 * t);
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
    QPainterPath stroked = stroker.createStroke(path);
    if (has_weight) {
        const QPointF anchor = weightAnchor() - pos();
        stroked.addEllipse(anchor, 20.0, 13.0);
    }
    return stroked;
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
void GraphEdge::repointDest(GraphNode* newDest) {
    if (!newDest || dest == newDest) return;
    if (dest) dest->removeEdge(this);
    dest = newDest;
    if (dest) dest->addEdge(this);
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
    setCursor(m_editable && has_weight ? QCursor(Qt::IBeamCursor) : QCursor(Qt::PointingHandCursor));
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
    if (on) {
        setAcceptHoverEvents(true);
        setToolTip("双击可修改边权");
    } else {
        setToolTip("");
    }
    update();
}
void GraphEdge::beginEdit() {
    if (!m_editable || m_editor) return;
    QGraphicsScene* sc = scene();
    if (!sc) return;
    QString current = QString::number(weight);
    m_editor = new EditorTextItem(m_guard->tearingDown);
    sc->addItem(m_editor);
    m_editor->setPlainText(current);
    m_editor->setFont(UiFonts::mono(11, true));
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
    installEditorWatcher(m_editor, m_guard->tearingDown, [self, editor](bool accepted) {
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
    if (!has_weight) {
        has_weight = true;
    }
    setWeight(parsed);
    emit weightEdited(parsed);
}
void GraphEdge::cancelEdit() {
    if (m_editor.isNull()) return;
    destroyEditor(m_editor);
    update();
}
void GraphEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (m_editable) {
        beginEdit();
        event->accept();
        return;
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}
void GraphEdge::setBeamProgress(qreal p) {
    if (qFuzzyCompare(m_beamProgress, p)) return;
    prepareGeometryChange();
    m_beamProgress = p;
    update();
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest) return;
    painter->setRenderHint(QPainter::Antialiasing);
    const qreal nodeRadius = 22.0;
    const qreal progress = qBound<qreal>(0.0, m_drawProgress, 1.0);
    QPointF p0 = trimmedStart(nodeRadius) - pos();
    QPointF p1 = trimmedEnd(nodeRadius) - pos();
    QPointF c = controlPoint() - pos();
    QPainterPath path;
    if (progress >= 1.0) {
        path.moveTo(p0);
        path.quadTo(c, p1);
    } else if (progress > 0.0) {
        const QPointF leftCtrl = p0 + (c - p0) * progress;
        const QPointF rightCtrl = c + (p1 - c) * progress;
        const QPointF mid = leftCtrl + (rightCtrl - leftCtrl) * progress;
        path.moveTo(p0);
        path.quadTo(leftCtrl, mid);
    }
    QPen pen(QColor(color.red(), color.green(), color.blue(), m_hovered ? 255 : 220), m_hovered ? 3.5 : m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(path);
    if (m_beamProgress >= 0.0 && m_beamProgress <= 1.0 && progress >= 1.0) {
        const qreal head = qBound<qreal>(0.0, m_beamProgress, 1.0);
        const qreal tail = qMax(0.0, head - 0.34);
        if (head - tail > 0.004) {
            QPainterPath beamPath;
            if (m_beamReversed) {
                quadSubPath(p0, c, p1, 1.0 - head, 1.0 - tail, beamPath);
            } else {
                quadSubPath(p0, c, p1, tail, head, beamPath);
            }
            QColor glow(color.red(), color.green(), color.blue());
            QLinearGradient grad = QLinearGradient(beamPath.pointAtPercent(0.0), beamPath.pointAtPercent(1.0));
            grad.setColorAt(0.0, QColor(glow.red(), glow.green(), glow.blue(), 0));
            grad.setColorAt(0.45, QColor(glow.red(), glow.green(), glow.blue(), 70));
            grad.setColorAt(1.0, QColor(glow.red(), glow.green(), glow.blue(), 235));
            painter->setPen(QPen(grad, 9.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPath(beamPath);
            QColor core = color.lighter(170);
            QLinearGradient coreGrad = QLinearGradient(beamPath.pointAtPercent(0.0), beamPath.pointAtPercent(1.0));
            coreGrad.setColorAt(0.0, QColor(core.red(), core.green(), core.blue(), 0));
            coreGrad.setColorAt(1.0, QColor(core.red(), core.green(), core.blue(), 245));
            painter->setPen(QPen(coreGrad, 3.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPath(beamPath);
            core.setAlpha(250);
            painter->setPen(QPen(core, 4.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPoint(beamPath.pointAtPercent(1.0));
        }
    }
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
    if (progress >= 1.0 && (is_directed || is_bidirectional)) {
        QPointF tip = trimmedEnd(nodeRadius);
        drawArrow(tip - pos(), dest->pos() - tip);
    }
    if (progress >= 1.0 && is_bidirectional) {
        QPointF tip = trimmedStart(nodeRadius);
        drawArrow(tip - pos(), source->pos() - tip);
    }
    if (has_weight) {
        QPointF anchor = weightAnchor();
        const QRectF badge(anchor.x() - 30, anchor.y() - 12, 60, 24);
        if (m_editable && (m_hovered || !m_editor.isNull())) {
            painter->setBrush(QColor(255, 255, 255, 225));
            painter->setPen(QPen(color.darker(140), 1.0));
            painter->drawRoundedRect(badge, 6, 6);
        }
        if (m_editor.isNull()) {
            painter->setPen(color.darker(120));
            painter->setFont(UiFonts::mono(11, true));
            painter->drawText(badge.adjusted(0, 1, 0, -1), Qt::AlignCenter, QString::number(weight));
        }
    }
    if (!pathLabel.isEmpty()) {
        const QPointF anchor = pathLabelAnchor();
        painter->setBrush(QColor(255, 255, 255, 235));
        painter->setPen(QPen(pathLabelColor, 1.2));
        painter->drawRoundedRect(QRectF(anchor.x() - 10, anchor.y() - 9, 20, 18), 5, 5);
        painter->setPen(pathLabelColor);
        painter->setFont(UiFonts::mono(12, true));
        painter->drawText(QRectF(anchor.x() - 10, anchor.y() - 9, 20, 18), Qt::AlignCenter, pathLabel);
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
    painter->setFont(UiFonts::mono(11, true));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(id));
}
ArrayBlock::ArrayBlock(int _id, int _val, QString _label) : id(_id), value(_val), showValue(true), color(Qt::white), customLabel(_label), showLabel(true) {
    setZValue(1);
}
ArrayBlock::~ArrayBlock() {
    if (m_guard) *m_guard->tearingDown = true;
    if (!m_editor.isNull()) {
        QGraphicsTextItem* ed = m_editor.data();
        m_editor = nullptr;
        ed->setParentItem(nullptr);
        delete ed;
    }
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
    m_editor = new EditorTextItem(m_guard->tearingDown);
    sc->addItem(m_editor);
    m_editor->setPlainText(current);
    m_editor->setFont(UiFonts::mono(14, true));
    m_editor->setDefaultTextColor(QColor(20, 30, 45));
    m_editor->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_editor->setZValue(20);
    m_editor->setParentItem(this);
    QFontMetrics fm(m_editor->font());
    m_editor->setPos(m_cellW / 2.0 - fm.horizontalAdvance(current) / 2.0, m_cellH / 2.0 - fm.height() / 2.0);
    m_editor->setFocus();
    QPointer<ArrayBlock> self(this);
    QGraphicsTextItem* editor = m_editor.data();
    installEditorWatcher(m_editor, m_guard->tearingDown, [self, editor](bool accepted) {
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
    const double top = showLabel ? -(kIndexGap + kIndexHeight) : 0.0;
    return QRectF(-1.0, top - 1.0, m_cellW + 2.0, m_cellH - top + 2.0);
}

void ArrayBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(color);
    QPen pen(Qt::black, 2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawRect(QRectF(0.0, 0.0, m_cellW, m_cellH));

    const bool hasIndex = showLabel;
    if (hasIndex) {
        const QString topText = customLabel.isEmpty() ? QString::number(id) : customLabel;
        painter->setFont(UiFonts::mono(11));
        painter->setPen(QColor(96, 108, 126));
        painter->drawText(QRectF(0.0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight),
                          Qt::AlignHCenter | Qt::AlignBottom, topText);
    }

    if (!showValue || !m_editor.isNull()) return;
    const QString text = customValue.isEmpty() ? QString::number(value) : customValue;
    painter->setFont(UiFonts::mono(m_valueFontSize, true));
    if (m_struck) {
        painter->setPen(QColor(122, 128, 138));
    } else {
        painter->setPen(Qt::black);
    }
    painter->drawText(QRectF(0.0, 0.0, m_cellW, m_cellH), Qt::AlignCenter, text);
}

FloatingText::FloatingText(const QString& t) : text(t) { setZValue(3); }
QRectF FloatingText::boundingRect() const {
    QFontMetrics fm(UiFonts::mono(fontSize, true));
    double w = qMax(60.0, fm.horizontalAdvance(text) + 10.0);
    return QRectF(0, 0, w, 40);
}

void FloatingText::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setFont(UiFonts::mono(fontSize, true));
    painter->setPen(textColor);
    QFontMetrics fm(painter->font());
    double w = qMax(60.0, fm.horizontalAdvance(text) + 10.0);
    const QRectF box = QRectF(0, 0, w, 40);
    if (centered) {
        painter->drawText(box, Qt::AlignCenter, text);
        return;
    }
    painter->drawText(QRectF(0, 0, w, 40), Qt::AlignCenter, text);
}
PointerBlock::PointerBlock(const QString& _label, QColor _color) : label(_label), color(_color) {
    setZValue(3);
    m_labelOffset = defaultLabelOffset();
    connect(this, &QGraphicsObject::xChanged, this, [this]() { m_placed = true; });
    connect(this, &QGraphicsObject::yChanged, this, [this]() { m_placed = true; });
    connect(this, &QGraphicsObject::opacityChanged, this, [this]() {
        prepareGeometryChange();
        update();
    });
}
void PointerBlock::setLabelRow(int row) {
    if (m_labelRow == row) return;
    prepareGeometryChange();
    m_labelRow = row;
    update();
}
QPointF PointerBlock::defaultLabelOffset() const {
    const double w = labelWidth();
    if (m_direction == Left) return QPointF(m_shaftLen + 8.0 + w / 2.0, -10.0);
    if (m_direction == Right) return QPointF(-(m_shaftLen + 8.0 + w / 2.0), -10.0);
    if (m_direction == Down) return QPointF(0.0, -(m_shaftLen + 9.0));
    return QPointF(0.0, m_shaftLen + 9.0);
}
QPointF PointerBlock::tailLabelOffset(double gap) const {
    const double w = labelWidth();
    if (m_direction == Left) return QPointF(m_shaftLen + gap + w / 2.0, -10.0);
    if (m_direction == Right) return QPointF(-(m_shaftLen + gap + w / 2.0), -10.0);
    if (m_direction == Down) return QPointF(0.0, -(m_shaftLen + gap));
    return QPointF(0.0, m_shaftLen + gap);
}
double PointerBlock::labelWidth() const {
    QFontMetrics fm(UiFonts::mono(14, true));
    double adv = fm.horizontalAdvance(label);
    if (adv <= 0.0) {
        adv = label.size() * 9.0;
    }
    return qMax(28.0, adv + 14.0);
}
QRectF PointerBlock::boundingRect() const {
    if (opacity() < 0.02) return QRectF();
    const double w = labelWidth();
    const double len = m_shaftLen;
    QRectF shaft;
    if (m_direction == Left || m_direction == Right) {
        const double sign = (m_direction == Right) ? 1.0 : -1.0;
        shaft = QRectF(std::min(-len * sign, 0.0), -10, len, 20);
    } else if (m_direction == Down) {
        shaft = QRectF(-10, -len, 20, len);
    } else {
        shaft = QRectF(-10, 0, 20, len);
    }
    QRectF label(effectiveLabelX() - w / 2.0, effectiveLabelY(), w, 20);
    return shaft.united(label);
}
void PointerBlock::setAnchorNode(GraphNode* node, bool syncNow) {
    if (m_anchor == node) return;
    if (m_anchor) m_anchor->removePointer(this);
    m_anchor = node;
    m_anchorObj = node;
    m_anchorObjSet = (node != nullptr);
    m_anchorItem = nullptr;
    if (node) {
        QObject::connect(node, &QObject::destroyed, this, [this]() {
            m_anchorItem = nullptr;
            m_anchor = nullptr;
            m_anchorObj = nullptr;
            m_anchorObjSet = false;
        }, Qt::UniqueConnection);
    }
    if (m_anchor) {
        m_anchor->addPointer(this);
        refreshLabelOffset();
        if (syncNow) syncToAnchor();
    }
}
void PointerBlock::setAnchorItem(QGraphicsItem* item, bool syncNow) {
    if (m_anchor) m_anchor->removePointer(this);
    m_anchor = nullptr;
    m_anchorItem = item;
    m_anchorObj = dynamic_cast<QObject*>(item);
    m_anchorObjSet = (item != nullptr);
    if (m_anchorObj) {
        QObject::connect(m_anchorObj, &QObject::destroyed, this, [this]() {
            m_anchorItem = nullptr;
            m_anchor = nullptr;
            m_anchorObj = nullptr;
            m_anchorObjSet = false;
        }, Qt::UniqueConnection);
    }
    refreshLabelOffset();
    if (syncNow) syncToAnchor();
}
void PointerBlock::refreshLabelOffset() {
    if (m_labelCustom) return;
    prepareGeometryChange();
    m_labelOffset = defaultLabelOffset();
    update();
}
void PointerBlock::syncToAnchor() {
    setPos(anchoredPos());
}

void PointerBlock::setTipTarget(QGraphicsItem* item, QPointF sceneTip) {
    m_tipTarget = item;
    m_tipTargetObj = dynamic_cast<QObject*>(item);
    m_tipTargetObjSet = (item != nullptr);
    if (item) m_tipLocal = item->mapFromScene(sceneTip);
    else m_tipLocal = sceneTip;
}

QPointF PointerBlock::anchoredPos() const {
    QGraphicsItem* target = anchorItem();
    if (!target) return pos();
    if (auto* cell = qgraphicsitem_cast<ArrayBlock*>(target)) {
        const QPointF c = cell->centerPos();
        if (m_direction == Down) return QPointF(c.x(), cell->pos().y() - PointerGeometry::kGap);
        if (m_direction == Up) return QPointF(c.x(), cell->pos().y() + cell->cellHeight() + PointerGeometry::kGap);
        if (m_direction == Left) return QPointF(cell->pos().x() + cell->cellWidth() + PointerGeometry::kGap, c.y());
        return QPointF(cell->pos().x() - PointerGeometry::kGap, c.y());
    }
    if (auto* node = qgraphicsitem_cast<GraphNode*>(target)) {
        const QPointF c = node->pos();
        if (m_direction == Down) return QPointF(c.x(), c.y() - GraphNode::kRadius);
        if (m_direction == Up) return QPointF(c.x(), c.y() + GraphNode::kRadius);
        if (m_direction == Left) return QPointF(c.x() + GraphNode::kRadius, c.y());
        return QPointF(c.x() - GraphNode::kRadius, c.y());
    }
    return target->pos() + m_anchorOffset;
}

void PointerBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    double w = labelWidth();
    double len = m_shaftLen;
    const QRectF labelRect(effectiveLabelX() - w / 2.0, effectiveLabelY(), w, 20);
    painter->setFont(UiFonts::mono(14, true));
    painter->setPen(color.darker(125));
    if (m_direction == Left || m_direction == Right) {
        const double sign = (m_direction == Right) ? 1.0 : -1.0;
        painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(-len * sign, 0, 0, 0);
        painter->drawLine(0, 0, 9 * sign, -7);
        painter->drawLine(0, 0, 9 * sign, 7);
        painter->setPen(color.darker(125));
        painter->drawText(labelRect, Qt::AlignCenter, label);
        return;
    }
    if (m_direction == Down) {
        painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(0, -len, 0, 0);
        painter->drawLine(0, 0, -7, -10);
        painter->drawLine(0, 0, 7, -10);
        painter->setPen(color.darker(125));
        painter->drawText(labelRect, Qt::AlignCenter, label);
        return;
    }
    painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(0, len, 0, 0);
    painter->drawLine(0, 0, -7, 10);
    painter->drawLine(0, 0, 7, 10);
    painter->setPen(color.darker(125));
    painter->drawText(labelRect, Qt::AlignCenter, label);
}

LinkLine::LinkLine(QPointF s, QPointF e) : start(s), end(e) {
    setZValue(5);
}

void LinkLine::setPoints(QPointF s, QPointF e) {
    if (start == s && end == e) return;
    prepareGeometryChange();
    start = s;
    end = e;
    update();
}

void LinkLine::setStyle(const QColor& c, qreal w, bool dash, bool hasArrow) {
    prepareGeometryChange();
    color = c;
    width = w;
    dashed = dash;
    arrow = hasArrow;
    update();
}

QPointF LinkLine::control() const {
    QPointF mid = (start + end) / 2.0;
    QPointF d = end - start;
    qreal len = std::hypot(d.x(), d.y());
    if (len < 1e-6) return mid;
    QPointF normal(-d.y() / len, d.x() / len);
    return mid + normal * bend;
}

QRectF LinkLine::boundingRect() const {
    QRectF rect = QRectF(start, end).normalized();
    rect = rect.united(QRectF(control(), QPointF(0, 0)));
    return rect.adjusted(-14, -14, 14, 14);
}

void LinkLine::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.moveTo(start);
    path.quadTo(control(), end);
    QPen pen(color, width, dashed ? Qt::DashLine : Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setDashPattern(dashed ? QVector<qreal>() << 6.0 << 5.0 : QVector<qreal>());
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
    if (!arrow) return;
    qreal t = 0.92;
    QPointF p0 = start;
    QPointF p1 = control();
    QPointF p2 = end;
    QPointF tip = p0 * (1 - t) * (1 - t) + p1 * 2 * (1 - t) * t + p2 * t * t;
    QPointF tangent = (p1 - p0) * 2 * (1 - t) + (p2 - p1) * 2 * t;
    qreal len = std::hypot(tangent.x(), tangent.y());
    if (len < 1e-6) return;
    QPointF dir = tangent / len;
    qreal ang = std::atan2(dir.y(), dir.x());
    qreal size = 12.0;
    qreal half = 5.5;
    QPointF base = tip - dir * size;
    QPolygonF head;
    head << tip
         << base + QPointF(half * std::cos(ang + M_PI / 2.0), half * std::sin(ang + M_PI / 2.0))
         << base + QPointF(half * std::cos(ang - M_PI / 2.0), half * std::sin(ang - M_PI / 2.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawPolygon(head);
}

Ripple::Ripple(QPointF center, QColor c) : m_center(center), color(c) {
    setZValue(4);
}

void Ripple::setRadius(qreal r) {
    if (qFuzzyCompare(m_radius, r)) return;
    prepareGeometryChange();
    m_radius = r;
    update();
}

QRectF Ripple::boundingRect() const {
    return QRectF(m_center.x() - m_radius - 3, m_center.y() - m_radius - 3,
                  (m_radius + 3) * 2, (m_radius + 3) * 2);
}

void Ripple::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color, 2.5));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(m_center, m_radius, m_radius);
}

RectRipple::RectRipple(QPointF center, QSizeF size, QColor c) : m_center(center), m_base(size), color(c) {
    setZValue(4);
    setAcceptedMouseButtons(Qt::NoButton);
}

void RectRipple::setGrow(qreal g) {
    if (qFuzzyCompare(m_grow, g)) return;
    prepareGeometryChange();
    m_grow = g;
    update();
}

void RectRipple::setBaseSize(QSizeF s) {
    if (m_base == s) return;
    prepareGeometryChange();
    m_base = s;
    update();
}

QRectF RectRipple::boundingRect() const {
    const qreal halfW = m_base.width() / 2.0 + m_grow + 4.0;
    const qreal halfH = m_base.height() / 2.0 + m_grow + 4.0;
    return QRectF(m_center.x() - halfW, m_center.y() - halfH, halfW * 2.0, halfH * 2.0);
}

void RectRipple::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    const qreal halfW = m_base.width() / 2.0 + m_grow;
    const qreal halfH = m_base.height() / 2.0 + m_grow;
    const QRectF box(m_center.x() - halfW, m_center.y() - halfH, halfW * 2.0, halfH * 2.0);
    QColor frame = color;
    frame.setAlpha(70);
    painter->setPen(QPen(frame, 5.0, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(box, 4.0, 4.0);
    painter->setPen(QPen(color, 2.4, Qt::SolidLine, Qt::RoundCap));
    painter->drawRoundedRect(box, 4.0, 4.0);
}

TracerDot::TracerDot(QColor c) : color(c) {
    setZValue(28);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF TracerDot::boundingRect() const {
    const qreal r = m_radius * 2.25 + 3.0;
    return QRectF(-r, -r, r * 2.0, r * 2.0);
}

void TracerDot::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    QColor halo = color;
    halo.setAlpha(70);
    painter->setBrush(halo);
    painter->drawEllipse(QPointF(0, 0), m_radius * 2.1, m_radius * 2.1);
    QColor core = color;
    core.setAlpha(235);
    painter->setBrush(core);
    painter->drawEllipse(QPointF(0, 0), m_radius, m_radius);
    painter->setPen(QPen(QColor(255, 255, 255, 220), 1.4));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPointF(0, 0), m_radius * 0.52, m_radius * 0.52);
}

PathTrail::PathTrail(QColor c) : m_color(c) {
    setZValue(26);
    setAcceptedMouseButtons(Qt::NoButton);
}

void PathTrail::setPoints(const QVector<QPointF>& pts) {
    prepareGeometryChange();
    m_points = pts;
    update();
}

void PathTrail::appendPoint(const QPointF& p) {
    prepareGeometryChange();
    m_points.append(p);
    update();
}

void PathTrail::clearPoints() {
    prepareGeometryChange();
    m_points.clear();
    m_progress = 0.0;
    update();
}

void PathTrail::setProgress(qreal t) {
    qreal clamped = qBound(0.0, t, 1.0);
    if (qFuzzyCompare(m_progress, clamped)) return;
    m_progress = clamped;
    update();
}

QRectF PathTrail::boundingRect() const {
    if (m_points.isEmpty()) return QRectF();
    const qreal pad = qMax<qreal>(m_width * 1.25, m_headRadius * 2.05) + 3.0;
    QRectF r;
    for (const QPointF& p : m_points) {
        const QPointF padPt(p.x() - pad, p.y() - pad);
        const QPointF size(pad * 2.0, pad * 2.0);
        r = r.isNull() ? QRectF(padPt, size) : r.united(QRectF(padPt, size));
    }
    return r;
}

void PathTrail::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (m_points.size() < 2) return;
    painter->setRenderHint(QPainter::Antialiasing);

    const int segCount = m_points.size() - 1;
    const double full = static_cast<double>(segCount);
    double drawn = full * m_progress;

    QPainterPath path;
    path.moveTo(m_points.first());
    QPointF head = m_points.first();
    bool started = false;
    for (int i = 0; i < segCount; ++i) {
        const double segStart = static_cast<double>(i);
        const double segEnd = segStart + 1.0;
        if (drawn >= segEnd) {
            if (!started) { path.moveTo(m_points[i]); started = true; }
            path.lineTo(m_points[i + 1]);
            head = m_points[i + 1];
        } else if (drawn > segStart) {
            const double t = drawn - segStart;
            const QPointF p = m_points[i] + (m_points[i + 1] - m_points[i]) * t;
            if (!started) { path.moveTo(m_points[i]); started = true; }
            path.lineTo(p);
            head = p;
            break;
        }
    }

    QColor glow = m_color;
    glow.setAlpha(60);
    painter->setPen(QPen(glow, m_width * 2.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);

    QColor core = m_color;
    core.setAlpha(235);
    painter->setPen(QPen(core, m_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(path);

    painter->setPen(Qt::NoPen);
    QColor halo = m_color;
    halo.setAlpha(85);
    painter->setBrush(halo);
    painter->drawEllipse(head, m_headRadius * 2.0, m_headRadius * 2.0);
    painter->setBrush(core);
    painter->drawEllipse(head, m_headRadius, m_headRadius);
    painter->setPen(QPen(QColor(255, 255, 255, 220), 1.4));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(head, m_headRadius * 0.5, m_headRadius * 0.5);
}