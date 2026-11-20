#include "IModule.h"
#include "render/EditableModel.h"
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QVariantAnimation>
#include <QEasingCurve>
#include <algorithm>
#include <cmath>

QPropertyAnimation* IModule::acquireAnim(QObject* target, const char* property, int duration, const QEasingCurve& curve) {
    if (!target) return nullptr;
    purgeDeadAnims();
    AnimKey key;
    key.target = QPointer<QObject>(target);
    key.property = QString(property);
    auto it = m_animRegistry.find(key);
    QPropertyAnimation* anim = nullptr;
    if (it != m_animRegistry.end() && it->second) {
        anim = it->second.data();
    } else {
        anim = new QPropertyAnimation(target, QByteArray(property));
        m_animRegistry[key] = QPointer<QPropertyAnimation>(anim);
    }
    anim->stop();
    anim->setDuration(duration);
    anim->setEasingCurve(curve);
    return anim;
}

IModule::~IModule() {
    for (auto& kv : m_animRegistry) {
        if (!kv.second) continue;
        kv.second->stop();
        delete kv.second.data();
    }
    m_animRegistry.clear();
}

void IModule::purgeDeadAnims() {
    std::vector<AnimKey> dead;
    for (auto& kv : m_animRegistry) {
        if (kv.first.target.isNull()) dead.push_back(kv.first);
    }
    for (auto& key : dead) {
        auto it = m_animRegistry.find(key);
        if (it == m_animRegistry.end()) continue;
        if (it->second) delete it->second.data();
        m_animRegistry.erase(it);
    }
}

void IModule::releaseAnim(QObject* target, const char* property) {
    AnimKey key;
    key.target = QPointer<QObject>(target);
    key.property = QString(property);
    auto it = m_animRegistry.find(key);
    if (it == m_animRegistry.end()) return;
    if (it->second) {
        it->second->stop();
        delete it->second.data();
    }
    m_animRegistry.erase(it);
}

void IModule::stopAnimFor(QObject* target) {
    if (!target) return;
    std::vector<AnimKey> dead;
    for (auto& kv : m_animRegistry) {
        if (kv.first.target.data() != target) continue;
        if (kv.second) kv.second->stop();
        dead.push_back(kv.first);
    }
    for (auto& key : dead) {
        auto it = m_animRegistry.find(key);
        if (it == m_animRegistry.end()) continue;
        if (it->second) delete it->second.data();
        m_animRegistry.erase(it);
    }
}
void IModule::stopAllAnims() {
    for (auto& kv : m_animRegistry) {
        if (kv.second) kv.second->stop();
    }
}

void IModule::playSwapAnim(ArrayBlock* b1, ArrayBlock* b2, int val1, int val2) {
    if (!b1 || !b2 || !m_scene) return;
    if (m_animated) {
        b1->showValue = false;
        b1->update();
        b2->showValue = false;
        b2->update();
        QPointer<ArrayBlock> safeB1 = b1;
        QPointer<ArrayBlock> safeB2 = b2;
        QPointer<FloatingText> t1 = new FloatingText(QString::number(val1));
        QPointer<FloatingText> t2 = new FloatingText(QString::number(val2));
        t1->setPos(b1->pos());
        t2->setPos(b2->pos());
        m_scene->addItem(t1.data());
        m_scene->addItem(t2.data());
        QVariantAnimation* anim = new QVariantAnimation();
        anim->setDuration(500);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QPointF p1 = b1->pos();
        QPointF p2 = b2->pos();
        QObject::connect(anim, &QVariantAnimation::valueChanged, [t1, t2, p1, p2](const QVariant& value) {
            qreal t = value.toReal();
            qreal arc = std::sin(t * M_PI) * 40.0;
            if (t1) t1->setPos(p1.x() + (p2.x() - p1.x()) * t, p1.y() - arc);
            if (t2) t2->setPos(p2.x() + (p1.x() - p2.x()) * t, p2.y() + arc);
        });
        QObject::connect(anim, &QVariantAnimation::finished, [this, safeB1, safeB2, t1, t2, val1, val2]() {
            if (t1) {
                if (t1->scene()) t1->scene()->removeItem(t1.data());
                delete t1.data();
            }
            if (t2) {
                if (t2->scene()) t2->scene()->removeItem(t2.data());
                delete t2.data();
            }
            if (safeB1) {
                safeB1->value = val2;
                safeB1->showValue = true;
                safeB1->update();
            }
            if (safeB2) {
                safeB2->value = val1;
                safeB2->showValue = true;
                safeB2->update();
            }
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        b1->value = val2;
        b2->value = val1;
        b1->showValue = true;
        b1->update();
        b2->showValue = true;
        b2->update();
    }
}

void IModule::playShiftAnim(ArrayBlock* src, ArrayBlock* dst, int val, QColor targetColor, bool pulseDst) {
    if (!src || !dst || !m_scene) return;
    if (m_animated) {
        src->showValue = false;
        src->color = QColor(200, 200, 200);
        src->update();
        QPointer<FloatingText> ft = new FloatingText(QString::number(val));
        ft->setPos(src->pos());
        m_scene->addItem(ft.data());
        m_transientItems.push_back(ft.data());
        QPointer<ArrayBlock> safeDst = dst;
        QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
        anim->setEndValue(dst->pos());
        anim->setDuration(300);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QObject::connect(anim, &QPropertyAnimation::finished, [this, safeDst, ft, val, targetColor, pulseDst]() {
            if (ft) {
                if (ft->scene()) ft->scene()->removeItem(ft.data());
                delete ft.data();
            }
            if (!safeDst) return;
            safeDst->value = val;
            safeDst->showValue = true;
            safeDst->color = targetColor;
            safeDst->update();
            if (pulseDst) playScaleAnim(safeDst);
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        src->showValue = false;
        src->color = QColor(200, 200, 200);
        src->update();
        dst->value = val;
        dst->showValue = true;
        dst->color = targetColor;
        dst->update();
    }
}

void IModule::playExtractAnim(ArrayBlock* src, ArrayBlock* dst, int val) {
    if (!src || !dst || !m_scene) return;
    if (m_animated) {
        src->showValue = false;
        src->color = QColor(200, 200, 200);
        src->update();
        QPointer<FloatingText> ft = new FloatingText(QString::number(val));
        ft->setPos(src->pos());
        m_scene->addItem(ft.data());
        m_transientItems.push_back(ft.data());
        QPointer<ArrayBlock> safeDst = dst;
        QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
        anim->setEndValue(dst->pos());
        anim->setDuration(400);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QObject::connect(anim, &QPropertyAnimation::finished, [this, safeDst, ft, val]() {
            if (ft) {
                if (ft->scene()) ft->scene()->removeItem(ft.data());
                delete ft.data();
            }
            if (!safeDst) return;
            safeDst->value = val;
            safeDst->showValue = true;
            safeDst->update();
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        src->showValue = false;
        src->color = QColor(200, 200, 200);
        src->update();
        dst->value = val;
        dst->showValue = true;
        dst->update();
    }
}

void IModule::playPointerMove(PointerBlock* ptr, QPointF targetPos) {
    playPointerMove(ptr, targetPos, 400, QEasingCurve::InOutQuad);
}

void IModule::playPointerMove(PointerBlock* ptr, QPointF targetPos, int duration, const QEasingCurve& curve) {
    if (!ptr) return;
    if (!m_animated) {
        stopAnimFor(ptr);
        ptr->setPos(targetPos);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(ptr, "pos", duration, curve);
    if (!anim) return;
    anim->setStartValue(ptr->pos());
    anim->setEndValue(targetPos);
    anim->start();
}

void IModule::playScaleAnim(ArrayBlock* block) {
    if (!block) return;
    if (!m_animated) {
        stopAnimFor(block);
        block->setScale(1.0);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(block, "scale", 300, QEasingCurve::InOutQuad);
    if (!anim) return;
    anim->setStartValue(block->scale());
    anim->setKeyValueAt(0.0, 1.0);
    anim->setKeyValueAt(0.5, 1.2);
    anim->setKeyValueAt(1.0, 1.0);
    anim->start();
}

namespace {
struct PointerSlot {
    double x = 0.0;
    double width = 0.0;
};
void resolvePointerCollisions(std::vector<PointerSlot>& layout) {
    if (layout.size() < 2) return;
    std::vector<int> order;
    for (int i = 0; i < static_cast<int>(layout.size()); ++i) order.push_back(i);
    std::sort(order.begin(), order.end(), [&layout](int a, int b) {
        return layout[a].x < layout[b].x;
    });
    for (int pass = 0; pass < 12; ++pass) {
        bool moved = false;
        for (int k = 0; k + 1 < static_cast<int>(order.size()); ++k) {
            int a = order[k];
            int b = order[k + 1];
            double rightA = layout[a].x + layout[a].width / 2.0;
            double leftB = layout[b].x - layout[b].width / 2.0;
            if (rightA <= leftB) continue;
            double push = (rightA - leftB) / 2.0 + 0.5;
            layout[a].x -= push;
            layout[b].x += push;
            moved = true;
        }
        if (!moved) break;
    }
}
}

void IModule::playPointersSync(std::vector<PointerBlock*> ptrs, int startX, int yPos, int spacing) {
    std::map<int, std::vector<PointerBlock*>> groups;
    for (auto* p : ptrs) {
        if (p && p->opacity() > 0) {
            int idx = p->property("logical_pos").toInt();
            if (idx >= 0) {
                groups[idx].push_back(p);
            }
        }
    }
    std::vector<PointerSlot> layout;
    std::vector<PointerBlock*> owners;
    std::vector<double> targetXs;
    for (auto& pair : groups) {
        int idx = pair.first;
        auto& group = pair.second;
        double base_x = startX + idx * spacing + spacing / 2.0;
        double totalWidth = 0.0;
        for (PointerBlock* p : group) totalWidth += p->labelWidth();
        double gap = 6.0;
        double span = totalWidth + gap * (group.size() - 1);
        double cursor = base_x - span / 2.0;
        for (PointerBlock* p : group) {
            PointerSlot s;
            s.width = p->labelWidth();
            s.x = cursor + s.width / 2.0;
            cursor += s.width + gap;
            layout.push_back(s);
            owners.push_back(p);
            targetXs.push_back(s.x);
        }
    }
    resolvePointerCollisions(layout);
    for (int i = 0; i < static_cast<int>(owners.size()); ++i) {
        PointerBlock* p = owners[i];
        QPointF targetPos(layout[i].x, yPos);
        if (m_animated) {
            QPropertyAnimation* anim = acquireAnim(p, "pos", 400, QEasingCurve::InOutQuad);
            if (!anim) continue;
            anim->setStartValue(p->pos());
            anim->setEndValue(targetPos);
            anim->start();
        } else {
            stopAnimFor(p);
            p->setPos(targetPos);
        }
    }
}

void IModule::layoutPointerSlots(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset) {
    std::map<QGraphicsItem*, std::vector<PointerBlock*>> groups;
    std::vector<PointerBlock*> owners;
    std::vector<PointerSlot> layout;
    std::vector<double> targetYs;

    for (int i = 0; i < static_cast<int>(ptrs.size()) && i < static_cast<int>(anchors.size()); ++i) {
        PointerBlock* p = ptrs[i];
        QGraphicsItem* anchor = anchors[i];
        if (!p || !anchor || p->opacity() < 0.05) continue;
        groups[anchor].push_back(p);
    }

    for (auto& pair : groups) {
        QGraphicsItem* anchor = pair.first;
        auto& group = pair.second;
        double baseX = anchor->pos().x() + 30.0;
        double totalWidth = 0.0;
        for (PointerBlock* p : group) totalWidth += p->labelWidth();
        double gap = 6.0;
        double span = totalWidth + gap * (group.size() - 1);
        double cursor = baseX - span / 2.0;
        for (PointerBlock* p : group) {
            PointerSlot s;
            s.width = p->labelWidth();
            s.x = cursor + s.width / 2.0;
            cursor += s.width + gap;
            layout.push_back(s);
            owners.push_back(p);
            targetYs.push_back(anchor->pos().y() + yOffset);
        }
    }

    resolvePointerCollisions(layout);

    for (int i = 0; i < static_cast<int>(owners.size()); ++i) {
        PointerBlock* p = owners[i];
        QPointF targetPos(layout[i].x, targetYs[i]);
        if (m_animated) {
            QPropertyAnimation* anim = acquireAnim(p, "pos", 380, QEasingCurve::InOutQuad);
            if (!anim) continue;
            anim->setStartValue(p->pos());
            anim->setEndValue(targetPos);
            anim->start();
        } else {
            stopAnimFor(p);
            p->setPos(targetPos);
        }
    }
}

void IModule::playPointerArrange(std::vector<PointerBlock*> ptrs, std::vector<QGraphicsItem*> anchors, double yOffset) {
    layoutPointerSlots(ptrs, anchors, yOffset);
}

void IModule::registerArraySlot(ArrayBlock* block, const QString& row, int dataIndex) {
    if (!block) return;
    m_arraySlotRow[block] = row;
    m_arraySlotIndex[block] = dataIndex;
}

void IModule::registerNodeSlot(int nodeId, const QString& row, int dataIndex) {
    m_nodeSlotRow[nodeId] = row;
    m_nodeSlotIndex[nodeId] = dataIndex;
}

void IModule::registerEdgeSlot(int edgeIndex, const QString& row, int dataIndex) {
    m_edgeSlotRow[edgeIndex] = row;
    m_edgeSlotIndex[edgeIndex] = dataIndex;
}

int IModule::liveIndexOfArray(const ArrayBlock* block) const {
    auto it = m_arraySlotIndex.find(block);
    return it == m_arraySlotIndex.end() ? block->id : it->second;
}

int IModule::liveIndexOfNode(int nodeId) const {
    auto it = m_nodeSlotIndex.find(nodeId);
    return it == m_nodeSlotIndex.end() ? nodeId : it->second;
}

int IModule::liveIndexOfEdge(int edgeIndex) const {
    auto it = m_edgeSlotIndex.find(edgeIndex);
    return it == m_edgeSlotIndex.end() ? edgeIndex : it->second;
}

QString IModule::editableRowFor(ArrayBlock* block) const {
    if (!block) return QString();
    auto it = m_arraySlotRow.find(block);
    return it == m_arraySlotRow.end() ? QString() : it->second;
}

QString IModule::editableRowFor(GraphNode* node) const {
    if (!node) return QString();
    auto it = m_nodeSlotRow.find(node->id);
    return it == m_nodeSlotRow.end() ? QString() : it->second;
}

QString IModule::editableRowFor(GraphEdge* edge) const {
    if (!edge) return QString();
    auto it = m_edgeSlotRow.find(edge->key());
    return it == m_edgeSlotRow.end() ? QString() : it->second;
}

void IModule::enableLiveEditing() {
    if (!m_scene) return;
    m_liveSlots.clear();
    for (QGraphicsItem* item : m_scene->items()) {
        ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
        if (block) {
            QString row = editableRowFor(block);
            if (row.isEmpty()) continue;
            int index = liveIndexOfArray(block);
            m_liveSlots[makeLiveKey(row, index)] = block->id;
            block->setEditable(true);
            QObject::connect(block, &ArrayBlock::valueEdited, m_scene, [this, block](int, const QString& text) {
                QString r = editableRowFor(block);
                if (r.isEmpty()) return;
                applyLiveEdit(makeLiveKey(r, liveIndexOfArray(block)), text);
            });
            continue;
        }
        GraphNode* node = qgraphicsitem_cast<GraphNode*>(item);
        if (node) {
            QString row = editableRowFor(node);
            if (row.isEmpty()) continue;
            int index = liveIndexOfNode(node->id);
            m_liveSlots[makeLiveKey(row, index)] = node->id;
            node->setEditable(true);
            QObject::connect(node, &GraphNode::valueEdited, m_scene, [this, node](int, const QString& text) {
                QString r = editableRowFor(node);
                if (r.isEmpty()) return;
                applyLiveEdit(makeLiveKey(r, liveIndexOfNode(node->id)), text);
            });
            continue;
        }
        GraphEdge* edge = qgraphicsitem_cast<GraphEdge*>(item);
        if (edge) {
            QString row = editableRowFor(edge);
            if (row.isEmpty()) continue;
            int index = liveIndexOfEdge(edge->key());
            m_liveSlots[makeLiveKey(row, index)] = edge->key();
            edge->setEditable(true);
            QObject::connect(edge, &GraphEdge::weightEdited, m_scene, [this, edge](int w) {
                QString r = editableRowFor(edge);
                if (r.isEmpty()) return;
                applyLiveEdit(makeLiveKey(r, liveIndexOfEdge(edge->key())), QString::number(w));
            });
        }
    }
}

void IModule::clearLiveSlots() {
    m_liveSlots.clear();
    m_arraySlotRow.clear();
    m_arraySlotIndex.clear();
    m_nodeSlotRow.clear();
    m_nodeSlotIndex.clear();
    m_edgeSlotRow.clear();
    m_edgeSlotIndex.clear();
}

void IModule::refreshLiveSlots() {
    if (!m_scene) return;
    for (QGraphicsItem* item : m_scene->items()) {
        ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
        if (block) {
            QString row = editableRowFor(block);
            if (row.isEmpty()) continue;
            int value = block->value;
            if (EditableModel::instance().getValue(liveScope(), row, liveIndexOfArray(block), value)) {
                block->value = value;
                block->customValue.clear();
                block->showValue = true;
            }
            block->update();
            continue;
        }
        GraphNode* node = qgraphicsitem_cast<GraphNode*>(item);
        if (node) {
            QString row = editableRowFor(node);
            if (row.isEmpty()) continue;
            int value = node->value;
            if (EditableModel::instance().getValue(liveScope(), row, liveIndexOfNode(node->id), value)) {
                node->value = value;
                node->setValueText(QString::number(value));
            }
            node->update();
            continue;
        }
        GraphEdge* edge = qgraphicsitem_cast<GraphEdge*>(item);
        if (edge) {
            QString row = editableRowFor(edge);
            if (row.isEmpty()) continue;
            int value = edge->weight;
            if (EditableModel::instance().getValue(liveScope(), row, liveIndexOfEdge(edge->key()), value)) {
                edge->setWeight(value);
            }
            edge->update();
        }
    }
}

QString IModule::liveScope() const {
    return getModuleName();
}

int IModule::liveValue(const QString& row, int index, int computed) const {
    int out = computed;
    EditableModel::instance().getValue(liveScope(), row, index, out);
    return out;
}

QString IModule::makeLiveKey(const QString& row, int index) const {
    return QString("%1#%2").arg(row).arg(index);
}

void IModule::applyLiveEdit(const QString& key, const QString& text) {
    if (key.isEmpty()) return;
    int sep = key.lastIndexOf('#');
    if (sep < 0) return;
    QString row = key.left(sep);
    int index = key.mid(sep + 1).toInt();
    bool ok = false;
    int parsed = text.toInt(&ok);
    if (!ok) return;
    EditableModel::instance().setValue(liveScope(), row, index, parsed);
    m_liveEditCount++;
    onLiveDataChanged(row, index, parsed);
    onSlotEdited(key, text);
    if (m_liveEditCallback) m_liveEditCallback();
}

void IModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool IModule::onSlotEdited(const QString& key, const QString& text) {
    Q_UNUSED(key);
    Q_UNUSED(text);
    return false;
}

void IModule::purgeTransientItems() {
    for (auto& commit : m_pendingCommits) {
        if (commit) commit();
    }
    m_pendingCommits.clear();
    for (auto& item : m_transientItems) {
        if (!item) continue;
        QGraphicsItem* g = item.data();
        if (!g) continue;
        if (g->scene()) g->scene()->removeItem(g);
        delete g;
    }
    m_transientItems.clear();
}

void IModule::playValueFlight(QPointF startPos, ArrayBlock* target, int value, QColor targetColor, bool pulseTarget) {
    if (!target || !m_scene) return;
    int generation = target->property("value_flight_generation").toInt() + 1;
    target->setProperty("value_flight_generation", generation);
    if (!m_animated) {
        target->value = value;
        target->showValue = true;
        target->color = targetColor;
        target->update();
        return;
    }
    QPointer<ArrayBlock> safeTarget = target;
    QPointer<FloatingText> text = new FloatingText(QString::number(value));
    text->setPos(startPos);
    m_scene->addItem(text.data());
    m_transientItems.push_back(text.data());
    QVariantAnimation* anim = new QVariantAnimation(target);
    anim->setDuration(360);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointF endPos = target->pos();
    QObject::connect(anim, &QVariantAnimation::valueChanged, [text, startPos, endPos](const QVariant& progress) {
        qreal t = progress.toReal();
        qreal arc = std::sin(t * M_PI) * 32.0;
        if (text) text->setPos(startPos.x() + (endPos.x() - startPos.x()) * t, startPos.y() + (endPos.y() - startPos.y()) * t - arc);
    });
    target->showValue = false;
    target->update();
    auto commit = [this, safeTarget, value, targetColor, pulseTarget, generation]() {
        if (!safeTarget) return;
        if (safeTarget->property("value_flight_generation").toInt() != generation) return;
        safeTarget->value = value;
        safeTarget->showValue = true;
        safeTarget->color = targetColor;
        safeTarget->update();
        if (pulseTarget) playScaleAnim(safeTarget);
    };
    m_pendingCommits.push_back(commit);
    QObject::connect(anim, &QVariantAnimation::finished, [this, safeTarget, text, commit]() {
        if (text) {
            if (text->scene()) text->scene()->removeItem(text);
            delete text.data();
        }
        if (!safeTarget) return;
        commit();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playDualValueFlight(QPointF startA, QPointF startB, ArrayBlock* target, int valueA, int valueB, QColor targetColor) {
    if (!target || !m_scene) return;
    int generation = target->property("value_flight_generation").toInt() + 1;
    target->setProperty("value_flight_generation", generation);
    if (!m_animated) {
        target->value = valueA + valueB;
        target->showValue = true;
        target->color = targetColor;
        target->update();
        return;
    }
    QPointer<ArrayBlock> safeTarget = target;
    QPointer<FloatingText> textA = new FloatingText(QString::number(valueA));
    QPointer<FloatingText> textB = new FloatingText(QString::number(valueB));
    textA->setPos(startA);
    textB->setPos(startB);
    m_scene->addItem(textA.data());
    m_scene->addItem(textB.data());
    m_transientItems.push_back(textA.data());
    m_transientItems.push_back(textB.data());
    QVariantAnimation* anim = new QVariantAnimation(target);
    anim->setDuration(420);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointF endPos = target->pos();
    QObject::connect(anim, &QVariantAnimation::valueChanged, [textA, textB, startA, startB, endPos](const QVariant& progress) {
        qreal t = progress.toReal();
        qreal arcA = std::sin(t * M_PI) * 34.0;
        qreal arcB = std::sin(t * M_PI) * 34.0;
        if (textA) textA->setPos(startA.x() + (endPos.x() - startA.x()) * t, startA.y() + (endPos.y() - startA.y()) * t - arcA);
        if (textB) textB->setPos(startB.x() + (endPos.x() - startB.x()) * t, startB.y() + (endPos.y() - startB.y()) * t - arcB);
    });
    target->showValue = false;
    target->update();
    auto commit = [this, safeTarget, valueA, valueB, targetColor, generation]() {
        if (!safeTarget) return;
        if (safeTarget->property("value_flight_generation").toInt() != generation) return;
        safeTarget->value = valueA + valueB;
        safeTarget->showValue = true;
        safeTarget->color = targetColor;
        safeTarget->update();
    };
    m_pendingCommits.push_back(commit);
    QObject::connect(anim, &QVariantAnimation::finished, [this, safeTarget, textA, textB, commit]() {
        if (textA) {
            if (textA->scene()) textA->scene()->removeItem(textA);
            delete textA.data();
        }
        if (textB) {
            if (textB->scene()) textB->scene()->removeItem(textB);
            delete textB.data();
        }
        if (!safeTarget) return;
        commit();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playMultiValueFlight(const std::vector<QPointF>& sources, const std::vector<int>& values,
                                   ArrayBlock* target, int total, QColor targetColor) {
    if (!target || !m_scene) return;
    int generation = target->property("value_flight_generation").toInt() + 1;
    target->setProperty("value_flight_generation", generation);
    if (!m_animated) {
        target->value = total;
        target->showValue = true;
        target->color = targetColor;
        target->update();
        return;
    }
    QPointer<ArrayBlock> safeTarget = target;
    std::vector<QPointer<FloatingText>> texts;
    for (size_t i = 0; i < sources.size() && i < values.size(); ++i) {
        QPointer<FloatingText> ft = new FloatingText(QString::number(values[i]));
        ft->setPos(sources[i]);
        m_scene->addItem(ft.data());
        m_transientItems.push_back(ft.data());
        texts.push_back(ft);
    }
    const QPointF endPos = target->pos();
    QVariantAnimation* anim = new QVariantAnimation(target);
    anim->setDuration(420);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [texts, sources, endPos](const QVariant& progress) {
        qreal t = progress.toReal();
        for (size_t i = 0; i < texts.size(); ++i) {
            if (!texts[i]) continue;
            const qreal arc = std::sin(t * M_PI) * (26.0 + 6.0 * static_cast<qreal>(i));
            texts[i]->setPos(sources[i].x() + (endPos.x() - sources[i].x()) * t,
                             sources[i].y() + (endPos.y() - sources[i].y()) * t - arc);
        }
    });
    target->showValue = false;
    target->update();
    auto commit = [this, safeTarget, total, targetColor, generation]() {
        if (!safeTarget) return;
        if (safeTarget->property("value_flight_generation").toInt() != generation) return;
        safeTarget->value = total;
        safeTarget->showValue = true;
        safeTarget->color = targetColor;
        safeTarget->update();
    };
    m_pendingCommits.push_back(commit);
    QObject::connect(anim, &QVariantAnimation::finished, [this, safeTarget, texts, commit]() {
        for (auto& t : texts) {
            if (!t) continue;
            if (t->scene()) t->scene()->removeItem(t.data());
            delete t.data();
        }
        if (!safeTarget) return;
        commit();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playTagDrop(QPointF startPos, QPointF endPos, const QString& text, QColor color, int duration) {
    if (!m_scene) return;
    QPointer<FloatingText> ft = new FloatingText(text);
    ft->setPos(startPos);
    ft->setZValue(5);
    m_scene->addItem(ft.data());
    m_transientItems.push_back(ft.data());
    QVariantAnimation* anim = new QVariantAnimation(ft.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ft, startPos, endPos](const QVariant& value) {
        qreal t = value.toReal();
        if (ft) ft->setPos(startPos.x() + (endPos.x() - startPos.x()) * t, startPos.y() + (endPos.y() - startPos.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [ft]() {
        if (ft) {
            if (ft->scene()) ft->scene()->removeItem(ft.data());
            delete ft.data();
        }
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playScalePulse(QObject* item, qreal peak, int duration) {
    if (!item) return;
    if (!m_animated) {
        item->setProperty("scale", 1.0);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(item, "scale", duration, QEasingCurve::InOutQuad);
    if (!anim) return;
    anim->setStartValue(item->property("scale"));
    anim->setKeyValueAt(0.0, 1.0);
    anim->setKeyValueAt(0.5, peak);
    anim->setKeyValueAt(1.0, 1.0);
    anim->start();
}

void IModule::playRowShift(std::vector<QGraphicsItem*> items, QPointF delta, int duration) {
    if (items.empty()) return;
    std::vector<QPointF> starts;
    for (QGraphicsItem* item : items) {
        if (!item) continue;
        starts.push_back(item->pos());
    }
    if (!m_animated) {
        for (QGraphicsItem* item : items) {
            if (!item) continue;
            item->setPos(item->pos() + delta);
        }
        return;
    }
    for (QGraphicsItem* item : items) {
        if (!item) continue;
        auto it = m_rowShiftAnims.find(item);
        if (it != m_rowShiftAnims.end() && it->second) {
            it->second->stop();
            it->second->deleteLater();
            m_rowShiftAnims.erase(it);
        }
    }
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [items, starts, delta](const QVariant& progress) {
        qreal t = progress.toReal();
        for (size_t i = 0; i < items.size(); ++i) {
            if (!items[i]) continue;
            QPointF s = starts[i];
            items[i]->setPos(s.x() + delta.x() * t, s.y() + delta.y() * t);
        }
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, items]() {
        for (QGraphicsItem* item : items) {
            if (!item) continue;
            m_rowShiftAnims.erase(item);
        }
    });
    for (QGraphicsItem* item : items) {
        if (!item) continue;
        m_rowShiftAnims[item] = QPointer<QVariantAnimation>(anim);
    }
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
