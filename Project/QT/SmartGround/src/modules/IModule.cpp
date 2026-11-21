#include "IModule.h"
#include "render/EditableModel.h"
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QVariantAnimation>
#include <QEasingCurve>
#include <QGraphicsTextItem>
#include <QFontMetrics>
#include <QColor>
#include <QTimer>
#include <algorithm>
#include <cmath>

bool IModule::s_scriptTextVisible = false;

const int IModule::kScriptTextMarker = 0x5347;

void IModule::setScriptTextVisible(bool on) {
    s_scriptTextVisible = on;
}

bool IModule::scriptTextVisible() {
    return s_scriptTextVisible;
}

int IModule::scriptTextMarker() {
    return kScriptTextMarker;
}

QGraphicsTextItem* IModule::createScriptText(QGraphicsScene* scene, const QString& text) {
    if (!scene) return nullptr;
    QGraphicsTextItem* item = scene->addText(text);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    item->setData(kScriptTextMarker, true);
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
    }
    return item;
}

QGraphicsTextItem* IModule::createStatusText(QGraphicsScene* scene, const QString& text) {
    if (!scene) return nullptr;
    QGraphicsTextItem* item = scene->addText(text);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return item;
}

void IModule::setScriptText(QGraphicsTextItem* item, const QString& text) {
    if (!item) return;
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
        return;
    }
    item->setVisible(true);
    item->setPlainText(text);
}

void IModule::setScriptText(QGraphicsTextItem* item, const QString& text, const QColor& color) {
    if (!item) return;
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
        return;
    }
    item->setVisible(true);
    item->setDefaultTextColor(color);
    item->setPlainText(text);
}

void IModule::clearScriptText(QGraphicsTextItem* item) {
    if (!item) return;
    item->setPlainText(QString());
}

void IModule::setStatusText(QGraphicsTextItem* item, const QString& text) {
    if (!item) return;
    item->setVisible(true);
    item->setPlainText(text);
}

void IModule::playTextFlightTo(QGraphicsTextItem* target, const QString& text, QPointF start,
                               int duration, std::function<void()> onArrive) {
    if (!target || !m_scene) {
        if (onArrive) onArrive();
        return;
    }
    const QFont font = target->font();
    const QPointF dst = target->pos();
    if (!m_animated) {
        target->setPlainText(text);
        target->setPos(dst);
        if (onArrive) onArrive();
        return;
    }
    QPointer<QGraphicsTextItem> safeTarget = target;
    QGraphicsTextItem* clone = m_scene->addText(text);
    clone->setFont(font);
    clone->setDefaultTextColor(target->defaultTextColor());
    clone->setPos(start);
    clone->setZValue(target->zValue() + 1.0);
    QVariantAnimation* anim = new QVariantAnimation(clone);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [clone, start, dst](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!clone) return;
        clone->setPos(start.x() + (dst.x() - start.x()) * t,
                      start.y() + (dst.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, clone, safeTarget, text, dst, onArrive]() {
        if (clone) {
            discardTransient(clone);
        }
        if (safeTarget) {
            safeTarget->setPlainText(text);
            safeTarget->setPos(dst);
        }
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playTextFlightFragment(QGraphicsTextItem* target, const QString& prefix, const QString& fragment,
                                     QPointF start, int duration, std::function<void()> onArrive) {
    if (!target || !m_scene) {
        if (onArrive) onArrive();
        return;
    }
    const QFont font = target->font();
    const QPointF dst = target->pos();
    const QFontMetrics fm(font);
    const double prefixWidth = fm.horizontalAdvance(prefix);
    if (!m_animated) {
        if (onArrive) onArrive();
        return;
    }
    QPointer<QGraphicsTextItem> safeTarget = target;
    QGraphicsTextItem* clone = m_scene->addText(fragment);
    clone->setFont(font);
    clone->setDefaultTextColor(target->defaultTextColor());
    clone->setPos(start);
    clone->setZValue(target->zValue() + 1.0);
    QVariantAnimation* anim = new QVariantAnimation(clone);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    const QPointF fragDst = dst + QPointF(prefixWidth, 0.0);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [clone, start, fragDst](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!clone) return;
        clone->setPos(start.x() + (fragDst.x() - start.x()) * t,
                      start.y() + (fragDst.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, clone, safeTarget, dst, onArrive]() {
        if (clone) {
            discardTransient(clone);
        }
        if (safeTarget) safeTarget->setPos(dst);
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

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
    for (auto& kv : m_beamAnims) {
        if (!kv.second) continue;
        kv.second->stop();
        delete kv.second.data();
    }
    m_beamAnims.clear();
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
        const QPointF glyphOffset(t1->boundingRect().width() / 2.0, t1->boundingRect().height() / 2.0);
        t1->setPos(b1->centerPos() - glyphOffset);
        t2->setPos(b2->centerPos() - glyphOffset);
        m_scene->addItem(t1.data());
        m_scene->addItem(t2.data());
        QVariantAnimation* anim = new QVariantAnimation();
        anim->setDuration(500);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QPointF p1 = b1->centerPos() - glyphOffset;
        QPointF p2 = b2->centerPos() - glyphOffset;
        QObject::connect(anim, &QVariantAnimation::valueChanged, [t1, t2, p1, p2](const QVariant& value) {
            qreal t = value.toReal();
            qreal arc = std::sin(t * M_PI) * 40.0;
            if (t1) t1->setPos(p1.x() + (p2.x() - p1.x()) * t, p1.y() - arc);
            if (t2) t2->setPos(p2.x() + (p1.x() - p2.x()) * t, p2.y() + arc);
        });
        QObject::connect(anim, &QVariantAnimation::finished, [this, safeB1, safeB2, t1, t2, val1, val2]() {
            if (t1) {
                discardTransient(t1.data());
            }
            if (t2) {
                discardTransient(t2.data());
            }
            if (safeB1) {
                safeB1->value = val2;
                safeB1->showValue = true;
                safeB1->update();
                playCellRipple(safeB1, QColor(120, 200, 245), 0.85, 460);
            }
            if (safeB2) {
                safeB2->value = val1;
                safeB2->showValue = true;
                safeB2->update();
                playCellRipple(safeB2, QColor(120, 200, 245), 0.85, 460);
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
        const QPointF glyphOffset(ft->boundingRect().width() / 2.0, ft->boundingRect().height() / 2.0);
        ft->setPos(src->centerPos() - glyphOffset);
        m_scene->addItem(ft.data());
        m_transientItems.push_back(ft.data());
        QPointer<ArrayBlock> safeDst = dst;
        QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
        anim->setEndValue(dst->centerPos() - glyphOffset);
        anim->setDuration(300);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QObject::connect(anim, &QPropertyAnimation::finished, [this, safeDst, ft, val, targetColor, pulseDst]() {
            if (ft) {
                discardTransient(ft.data());
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
        const QPointF glyphOffset(ft->boundingRect().width() / 2.0, ft->boundingRect().height() / 2.0);
        ft->setPos(src->centerPos() - glyphOffset);
        m_scene->addItem(ft.data());
        m_transientItems.push_back(ft.data());
        QPointer<ArrayBlock> safeDst = dst;
        QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
        anim->setEndValue(dst->centerPos() - glyphOffset);
        anim->setDuration(400);
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        QObject::connect(anim, &QPropertyAnimation::finished, [this, safeDst, ft, val]() {
            if (ft) {
                discardTransient(ft.data());
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
    const QPointF from = ptr->pos();
    if (!ptr->isPlaced() && from.isNull()) {
        ptr->setPos(targetPos);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(ptr, "pos", duration, curve);
    if (!anim) return;
    anim->setStartValue(from);
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
    std::vector<PointerBlock*> owners;
    std::vector<QGraphicsItem*> ownerAnchors;
    std::vector<QPointF> tips;

    for (size_t i = 0; i < ptrs.size() && i < anchors.size(); ++i) {
        PointerBlock* p = ptrs[i];
        QGraphicsItem* anchor = anchors[i];
        if (!p || !anchor || p->opacity() < 0.05) continue;
        owners.push_back(p);
        ownerAnchors.push_back(anchor);
        tips.push_back(pointerTipFor(p, anchor));
    }
    if (owners.empty()) return;

    std::map<QGraphicsItem*, std::vector<int>> groups;
    for (size_t i = 0; i < owners.size(); ++i) groups[ownerAnchors[i]].push_back(static_cast<int>(i));

    for (auto& pair : groups) {
        std::vector<int>& members = pair.second;
        if (members.size() <= 1) {
            owners[members[0]]->setLabelRow(0);
            continue;
        }
        double widest = 0.0;
        for (int idx : members) widest = qMax(widest, owners[idx]->labelWidth());
        for (int idx : members) owners[idx]->setLabelRow(0);
    }

    const bool vertical = owners.front()->direction() == PointerBlock::Down
                       || owners.front()->direction() == PointerBlock::Up;
    if (vertical) {
        std::vector<int> order(owners.size());
        for (size_t i = 0; i < order.size(); ++i) order[i] = static_cast<int>(i);
        std::sort(order.begin(), order.end(), [&tips](int a, int b) {
            return tips[a].x() < tips[b].x();
        });
        std::vector<double> rowCursors;
        for (size_t k = 0; k < order.size(); ++k) {
            const int idx = order[k];
            const double w = owners[idx]->labelWidth();
            const double left = tips[idx].x() + owners[idx]->labelBaseX() - w / 2.0;
            int row = 0;
            while (row < static_cast<int>(rowCursors.size()) && left < rowCursors[row]) ++row;
            if (row == static_cast<int>(rowCursors.size())) rowCursors.push_back(left + w);
            else rowCursors[row] = left + w;
            owners[idx]->setLabelRow(row);
        }
    } else {
        std::vector<int> order(owners.size());
        for (size_t i = 0; i < order.size(); ++i) order[i] = static_cast<int>(i);
        std::sort(order.begin(), order.end(), [&tips](int a, int b) {
            return tips[a].y() < tips[b].y();
        });
        std::vector<double> rowCursors;
        for (size_t k = 0; k < order.size(); ++k) {
            const int idx = order[k];
            const double w = owners[idx]->labelWidth();
            const double top = tips[idx].y() + owners[idx]->labelBaseY();
            int row = 0;
            while (row < static_cast<int>(rowCursors.size()) && top < rowCursors[row]) ++row;
            if (row == static_cast<int>(rowCursors.size())) rowCursors.push_back(top + 20.0);
            else rowCursors[row] = top + 20.0;
            owners[idx]->setLabelRow(row);
        }
    }
    for (size_t i = 0; i < owners.size(); ++i) {
        owners[i]->setTipTarget(ownerAnchors[i], tips[i]);
        animatePointerTo(owners[i], QPointF(tips[i].x(), tips[i].y()));
    }
    separatePointerLabels(owners);
}

QPointF IModule::pointerTipFor(PointerBlock* ptr, QGraphicsItem* anchor) {
    return pointerTipFor(ptr, anchor, ptr ? ptr->direction() : PointerBlock::Up);
}

QPointF IModule::pointerTipFor(PointerBlock* ptr, QGraphicsItem* anchor, PointerBlock::Direction dir) {
    if (!ptr || !anchor) return QPointF();
    if (auto* cell = qgraphicsitem_cast<ArrayBlock*>(anchor)) {
        const QPointF c = cell->centerPos();
        if (dir == PointerBlock::Down) return QPointF(c.x(), cell->pos().y() - PointerGeometry::kGap);
        if (dir == PointerBlock::Up) return QPointF(c.x(), cell->pos().y() + cell->cellHeight() + PointerGeometry::kGap);
        if (dir == PointerBlock::Left) return QPointF(cell->pos().x() + cell->cellWidth() + PointerGeometry::kGap, c.y());
        return QPointF(cell->pos().x() - PointerGeometry::kGap, c.y());
    }
    if (auto* node = qgraphicsitem_cast<GraphNode*>(anchor)) {
        const QPointF c = node->pos();
        if (dir == PointerBlock::Down) return QPointF(c.x(), c.y() - GraphNode::kRadius);
        if (dir == PointerBlock::Up) return QPointF(c.x(), c.y() + GraphNode::kRadius);
        if (dir == PointerBlock::Left) return QPointF(c.x() + GraphNode::kRadius, c.y());
        return QPointF(c.x() - GraphNode::kRadius, c.y());
    }
    if (auto* rect = qgraphicsitem_cast<QGraphicsRectItem*>(anchor)) {
        const QRectF r = rect->rect();
        const QPointF c = rect->pos() + r.center();
        if (dir == PointerBlock::Down) return QPointF(c.x(), rect->pos().y() - PointerGeometry::kGap);
        if (dir == PointerBlock::Up) return QPointF(c.x(), rect->pos().y() + r.height() + PointerGeometry::kGap);
        if (dir == PointerBlock::Left) return QPointF(rect->pos().x() + r.width() + PointerGeometry::kGap, c.y());
        return QPointF(rect->pos().x() - PointerGeometry::kGap, c.y());
    }
    return anchor->pos();
}

void IModule::animatePointerTo(PointerBlock* p, const QPointF& target) {
    if (!p) return;
    if (m_animated) {
        QPropertyAnimation* anim = acquireAnim(p, "pos", 380, QEasingCurve::InOutQuad);
        if (!anim) return;
        anim->setStartValue(p->pos());
        anim->setEndValue(target);
        anim->start();
    } else {
        stopAnimFor(p);
        p->setPos(target);
    }
}

void IModule::syncPointerTips() {
    if (!m_scene) return;
    for (auto* item : m_scene->items()) {
        PointerBlock* ptr = qgraphicsitem_cast<PointerBlock*>(item);
        if (!ptr) continue;
        if (ptr->opacity() < 0.05) continue;
        QGraphicsItem* anchor = ptr->anchorItem();
        if (!anchor) continue;
        const QPointF tip = pointerTipFor(ptr, anchor);
        if (qFuzzyCompare(tip.x(), ptr->pos().x()) && qFuzzyCompare(tip.y(), ptr->pos().y())) continue;
        bool posAnimRunning = false;
        bool posAnimTargetsTip = false;
        for (auto& pair : m_animRegistry) {
            if (pair.first.target.data() != ptr) continue;
            if (pair.first.property != "pos") continue;
            QPropertyAnimation* anim = pair.second.data();
            if (!anim) continue;
            if (anim->state() != QAbstractAnimation::Running) continue;
            posAnimRunning = true;
            const QVariant end = anim->endValue();
            if (end.canConvert<QPointF>()) {
                const QPointF endPos = end.toPointF();
                posAnimTargetsTip = qFuzzyCompare(endPos.x(), tip.x()) && qFuzzyCompare(endPos.y(), tip.y());
            }
            break;
        }
        if (posAnimRunning && posAnimTargetsTip) continue;
        stopAnimFor(ptr);
        ptr->setPos(tip);
        ptr->setTipTarget(anchor, tip);
    }
}

void IModule::resolveVisualCollisions() {
    if (!m_scene) return;
    std::vector<PointerBlock*> pointers;
    for (auto* item : m_scene->items()) {
        if (item->opacity() < 0.05) continue;
        if (!item->isVisible()) continue;
        if (auto* p = qgraphicsitem_cast<PointerBlock*>(item)) pointers.push_back(p);
    }
    if (pointers.empty()) return;
    for (PointerBlock* p : pointers) { p->resetLabelShift(); p->setLabelRow(0); }

    struct Obstacle { QRectF rect; };
    std::vector<Obstacle> obstacles;
    for (auto* item : m_scene->items()) {
        if (item->opacity() < 0.05) continue;
        if (!item->isVisible()) continue;
        if (qgraphicsitem_cast<PointerBlock*>(item)) continue;
        if (auto* c = qgraphicsitem_cast<ArrayBlock*>(item)) {
            obstacles.push_back({c->mapRectToScene(QRectF(0, 0, c->cellWidth(), c->cellHeight()))});
            if (c->showLabel) {
                obstacles.push_back({c->mapRectToScene(c->indexRect())});
            }
            continue;
        }
        if (auto* n = qgraphicsitem_cast<GraphNode*>(item)) {
            obstacles.push_back({n->mapRectToScene(QRectF(-20, -20, 40, 40))});
            QFontMetrics fm(UiFonts::mono(10, true));
            if (!n->subLabel.isEmpty()) {
                const double tw = fm.horizontalAdvance(n->subLabel) + 16.0;
                obstacles.push_back({n->mapRectToScene(QRectF(-tw / 2.0, -46, tw, 18))});
            }
            if (!n->valueText.isEmpty()) {
                const double tw = fm.horizontalAdvance(n->valueText) + 12.0;
                obstacles.push_back({n->mapRectToScene(QRectF(-tw / 2.0, 22, tw, 18))});
            }
            continue;
        }
        if (auto* t = qgraphicsitem_cast<QGraphicsTextItem*>(item)) {
            if (t->toPlainText().trimmed().isEmpty()) continue;
            obstacles.push_back({t->sceneBoundingRect()});
        }
    }

    struct Entry {
        PointerBlock* ptr;
        QPointF base;
        double width;
        double rowGap;
    };
    std::vector<Entry> entries;
    entries.reserve(pointers.size());
    for (PointerBlock* p : pointers) {
        Entry e;
        e.ptr = p;
        e.base = p->pos();
        for (auto& pair : m_animRegistry) {
            if (pair.first.target.data() != p) continue;
            if (pair.first.property != "pos") continue;
            QPropertyAnimation* anim = pair.second.data();
            if (!anim) continue;
            if (anim->state() != QAbstractAnimation::Running) continue;
            const QVariant end = anim->endValue();
            if (end.canConvert<QPointF>()) e.base = end.toPointF();
        }
        e.width = p->labelWidth();
        e.rowGap = 22.0;
        entries.push_back(e);
    }

    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        if (!qFuzzyCompare(a.base.y(), b.base.y())) return a.base.y() < b.base.y();
        return a.base.x() < b.base.x();
    });

    std::vector<QRectF> accepted;
    for (Entry& e : entries) {
        PointerBlock* p = e.ptr;
        const double w = e.width;
        const double bx = e.base.x() + p->labelBaseX();
        const double by0 = e.base.y() + p->labelBaseY();
        const bool vertical = (p->direction() == PointerBlock::Up || p->direction() == PointerBlock::Down);
        const double dir = (p->direction() == PointerBlock::Down) ? -1.0 : 1.0;
        const double lateral = w + 6.0;

        struct Cand { double dx; double dy; int row; double cost; };
        std::vector<Cand> cands;
        if (vertical) {
            for (int row = 0; row <= 8; ++row) {
                cands.push_back({0.0, dir * row * e.rowGap, row, row * 100.0});
            }
        } else {
            for (int lat = 0; lat <= 4; ++lat) {
                const double dx = lat == 0 ? 0.0
                                : (lat == 1 ? lateral
                                : (lat == 2 ? -lateral
                                : (lat == 3 ? 2 * lateral : -2 * lateral)));
                cands.push_back({dx, 0.0, 0, std::abs(dx) * 10.0});
            }
        }

        QRectF best;
        double bestArea = 1e18;
        double bestCost = 1e18;
        QPointF bestShift(0, 0);
        int bestRow = 0;
        bool found = false;
        for (const Cand& c : cands) {
            const QRectF rect(bx + c.dx - w / 2.0, by0 + c.dy, w, 20.0);
            double area = 0.0;
            for (const Obstacle& ob : obstacles) {
                const QRectF inter = rect.intersected(ob.rect);
                if (!inter.isEmpty()) area += inter.width() * inter.height();
            }
            for (const QRectF& acc : accepted) {
                const QRectF inter = rect.intersected(acc);
                if (!inter.isEmpty()) area += inter.width() * inter.height();
            }
            if (area <= 0.5) {
                if (!found || c.cost < bestCost) {
                    bestShift = QPointF(c.dx, c.dy);
                    best = rect;
                    bestRow = c.row;
                    bestCost = c.cost;
                    found = true;
                }
                continue;
            }
            if (!found && area < bestArea) {
                bestArea = area;
                bestShift = QPointF(c.dx, c.dy);
                best = rect;
                bestRow = c.row;
            }
        }
        p->setLabelRow(vertical ? bestRow : 0);
        p->setLabelShift(vertical ? QPointF(0, 0) : bestShift);
        accepted.push_back(best);
    }
}
void IModule::resolveScriptTextOverlaps() {
    if (!m_scene) return;
    if (!s_scriptTextVisible) return;
    std::vector<QGraphicsTextItem*> texts;
    for (auto* item : m_scene->items()) {
        if (item->opacity() < 0.05) continue;
        if (!item->isVisible()) continue;
        auto* t = qgraphicsitem_cast<QGraphicsTextItem*>(item);
        if (!t) continue;
        if (t->data(kScriptTextMarker).toBool() != true) continue;
        if (t->toPlainText().trimmed().isEmpty()) continue;
        texts.push_back(t);
    }
    if (texts.empty()) return;

    struct Solid { QRectF rect; };
    std::vector<Solid> solids;
    for (auto* item : m_scene->items()) {
        if (item->opacity() < 0.05) continue;
        if (!item->isVisible()) continue;
        if (qgraphicsitem_cast<QGraphicsTextItem*>(item)) continue;
        if (qgraphicsitem_cast<PointerBlock*>(item)) continue;
        if (auto* c = qgraphicsitem_cast<ArrayBlock*>(item)) {
            solids.push_back({c->mapRectToScene(QRectF(0, 0, c->cellWidth(), c->cellHeight()))});
            continue;
        }
        if (auto* n = qgraphicsitem_cast<GraphNode*>(item)) {
            solids.push_back({n->mapRectToScene(QRectF(-24, -24, 48, 48))});
            QFontMetrics fm(UiFonts::mono(10, true));
            if (!n->subLabel.isEmpty()) {
                const double w = fm.horizontalAdvance(n->subLabel) + 16.0;
                solids.push_back({n->mapRectToScene(QRectF(-w / 2.0, -46, w, 18))});
            }
            if (!n->valueText.isEmpty()) {
                const double w = fm.horizontalAdvance(n->valueText) + 12.0;
                solids.push_back({n->mapRectToScene(QRectF(-w / 2.0, 22, w, 18))});
            }
            continue;
        }
        if (auto* r = qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
            solids.push_back({r->mapRectToScene(r->rect())});
            continue;
        }
    }

    std::vector<Solid> placed;
    auto overlaps = [&](const QRectF& a) {
        for (const Solid& s : solids) {
            const QRectF inter = a.intersected(s.rect);
            if (!inter.isEmpty() && inter.width() * inter.height() > 4.0) return true;
        }
        for (const Solid& s : placed) {
            const QRectF inter = a.intersected(s.rect);
            if (!inter.isEmpty() && inter.width() * inter.height() > 4.0) return true;
        }
        return false;
    };

    for (QGraphicsTextItem* t : texts) {
        const QRectF orig = t->sceneBoundingRect();
        if (!overlaps(orig)) {
            placed.push_back({orig});
            continue;
        }
        const double origY = t->pos().y();
        const double origX = t->pos().x();
        bool moved = false;
        for (int k = 1; k <= 20 && !moved; ++k) {
            for (int dir = 0; dir < 2 && !moved; ++dir) {
                const double dy = (dir == 0 ? -1.0 : 1.0) * 34.0 * k;
                t->setPos(origX, origY + dy);
                if (!overlaps(t->sceneBoundingRect())) {
                    moved = true;
                    break;
                }
            }
        }
        if (!moved) {
            t->setPos(origX, origY);
            for (int dir = 0; dir < 2 && !moved; ++dir) {
                for (int k = 1; k <= 8 && !moved; ++k) {
                    const double dx = (dir == 0 ? -1.0 : 1.0) * 90.0 * k;
                    t->setPos(origX + dx, origY);
                    if (!overlaps(t->sceneBoundingRect())) {
                        moved = true;
                        break;
                    }
                }
            }
            if (!moved) t->setPos(origX, origY);
        }
        placed.push_back({t->sceneBoundingRect()});
    }
}

void IModule::separatePointerLabels(const std::vector<PointerBlock*>& ptrs) {
    std::vector<PointerBlock*> visible;
    for (PointerBlock* p : ptrs) {
        if (p && p->opacity() > 0.05) visible.push_back(p);
    }
    if (visible.size() < 2) return;
    for (PointerBlock* p : visible) p->resetLabelShift();

    const bool vertical = visible.front()->direction() == PointerBlock::Down
                       || visible.front()->direction() == PointerBlock::Up;
    for (int pass = 0; pass < 30; ++pass) {
        bool moved = false;
        for (size_t i = 0; i + 1 < visible.size(); ++i) {
            for (size_t j = i + 1; j < visible.size(); ++j) {
                PointerBlock* a = visible[i];
                PointerBlock* b = visible[j];
                const double aLeft = a->pos().x() + a->labelLeft();
                const double aRight = a->pos().x() + a->labelRight();
                const double bLeft = b->pos().x() + b->labelLeft();
                const double bRight = b->pos().x() + b->labelRight();
                const double aTop = a->pos().y() + a->labelTop();
                const double aBottom = a->pos().y() + a->labelBottom();
                const double bTop = b->pos().y() + b->labelTop();
                const double bBottom = b->pos().y() + b->labelBottom();
                const bool xOverlap = aRight > bLeft + 0.5 && bRight > aLeft + 0.5;
                const bool yOverlap = aBottom > bTop + 0.5 && bBottom > aTop + 0.5;
                if (!xOverlap || !yOverlap) continue;
                if (vertical) {
                    const double aRow = a->labelRow();
                    const double bRow = b->labelRow();
                    const double aCur = a->pos().y() + a->labelBaseY() + a->labelRowOffsetFor(static_cast<int>(aRow));
                    const double bCur = b->pos().y() + b->labelBaseY() + b->labelRowOffsetFor(static_cast<int>(bRow));
                    const double step = 22.0;
                    double aNew = aCur;
                    double bNew = bCur;
                    if (aCur <= bCur) {
                        aNew = aCur - step;
                        bNew = bCur + step;
                    } else {
                        aNew = aCur + step;
                        bNew = bCur - step;
                    }
                    a->setLabelShift(QPointF(0, aNew - aCur));
                    b->setLabelShift(QPointF(0, bNew - bCur));
                } else {
                    const double aShift = a->labelShift().x();
                    const double bShift = b->labelShift().x();
                    const double aCur = a->pos().x() + a->labelBaseX() + aShift;
                    const double bCur = b->pos().x() + b->labelBaseX() + bShift;
                    double aNew = aCur;
                    double bNew = bCur;
                    if (aCur <= bCur) {
                        aNew = aCur - 1.5;
                        bNew = bCur + 1.5;
                    } else {
                        aNew = aCur + 1.5;
                        bNew = bCur - 1.5;
                    }
                    a->setLabelShift(QPointF(aNew - aCur, 0));
                    b->setLabelShift(QPointF(bNew - bCur, 0));
                }
                moved = true;
            }
        }
        if (!moved) break;
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
    for (auto& kv : m_beamAnims) {
        if (kv.second) kv.second->stop();
        if (kv.first) kv.first->clearBeam();
    }
    m_beamAnims.clear();
    for (auto& item : m_transientItems) {
        if (!item) continue;
        QGraphicsItem* g = item.data();
        if (!g) continue;
        if (g->scene()) g->scene()->removeItem(g);
        delete g;
    }
    m_transientItems.clear();
    for (auto& dot : m_transientDots) {
        if (!dot) continue;
        TracerDot* d = dot.data();
        if (!d) continue;
        if (d->scene()) d->scene()->removeItem(d);
        delete d;
    }
    m_transientDots.clear();
    for (auto& ripple : m_transientRipples) {
        if (!ripple) continue;
        Ripple* r = ripple.data();
        if (!r) continue;
        if (r->scene()) r->scene()->removeItem(r);
        delete r;
    }
    m_transientRipples.clear();
    for (auto& rect : m_transientRectRipples) {
        if (!rect) continue;
        RectRipple* r = rect.data();
        if (!r) continue;
        if (r->scene()) r->scene()->removeItem(r);
        delete r;
    }
    m_transientRectRipples.clear();
    if (m_scene) m_scene->update(m_scene->itemsBoundingRect());
}

void IModule::discardTransient(QGraphicsItem* item) {
    if (!item) return;
    QGraphicsScene* sc = item->scene();
    const QRectF rect = item->sceneBoundingRect();
    if (sc) {
        sc->removeItem(item);
        sc->update(rect);
    }
    delete item;
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
    const QPointF glyphOffset(text->boundingRect().width() / 2.0, text->boundingRect().height() / 2.0);
    const QPointF startCentered = startPos - glyphOffset;
    const QPointF endPos = target->centerPos() - glyphOffset;
    QObject::connect(anim, &QVariantAnimation::valueChanged, [text, startCentered, endPos](const QVariant& progress) {
        qreal t = progress.toReal();
        qreal arc = std::sin(t * M_PI) * 32.0;
        if (text) text->setPos(startCentered.x() + (endPos.x() - startCentered.x()) * t,
                               startCentered.y() + (endPos.y() - startCentered.y()) * t - arc);
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
            discardTransient(text.data());
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
    const QPointF glyphOffset(textA->boundingRect().width() / 2.0, textA->boundingRect().height() / 2.0);
    const QPointF startACentered = startA - glyphOffset;
    const QPointF startBCentered = startB - glyphOffset;
    const QPointF endPos = target->centerPos() - glyphOffset;
    QObject::connect(anim, &QVariantAnimation::valueChanged, [textA, textB, startACentered, startBCentered, endPos](const QVariant& progress) {
        qreal t = progress.toReal();
        qreal arcA = std::sin(t * M_PI) * 34.0;
        qreal arcB = std::sin(t * M_PI) * 34.0;
        if (textA) textA->setPos(startACentered.x() + (endPos.x() - startACentered.x()) * t,
                                 startACentered.y() + (endPos.y() - startACentered.y()) * t - arcA);
        if (textB) textB->setPos(startBCentered.x() + (endPos.x() - startBCentered.x()) * t,
                                 startBCentered.y() + (endPos.y() - startBCentered.y()) * t - arcB);
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
            discardTransient(textA.data());
        }
        if (textB) {
            discardTransient(textB.data());
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
    const QPointF glyphOffset(texts.empty() ? QPointF(30.0, 20.0)
                                             : QPointF(texts.front()->boundingRect().width() / 2.0,
                                                       texts.front()->boundingRect().height() / 2.0));
    const QPointF endPos = target->centerPos() - glyphOffset;
    QVariantAnimation* anim = new QVariantAnimation(target);
    anim->setDuration(420);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [texts, sources, endPos, glyphOffset](const QVariant& progress) {
        qreal t = progress.toReal();
        for (size_t i = 0; i < texts.size(); ++i) {
            if (!texts[i]) continue;
            const qreal arc = std::sin(t * M_PI) * (26.0 + 6.0 * static_cast<qreal>(i));
            const QPointF s = sources[i] - glyphOffset;
            texts[i]->setPos(s.x() + (endPos.x() - s.x()) * t,
                             s.y() + (endPos.y() - s.y()) * t - arc);
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
            discardTransient(t.data());
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

void IModule::playNodeSwapAnim(GraphNode* n1, GraphNode* n2, int val1, int val2, int duration) {
    if (!n1 || !n2 || !m_scene) return;
    if (!m_animated) {
        n1->setCustomValueText(QString::number(val2));
        n2->setCustomValueText(QString::number(val1));
        return;
    }
    QPointer<GraphNode> safeN1 = n1;
    QPointer<GraphNode> safeN2 = n2;
    QPointer<FloatingText> t1 = new FloatingText(QString::number(val1));
    QPointer<FloatingText> t2 = new FloatingText(QString::number(val2));
    const QPointF p1 = n1->pos();
    const QPointF p2 = n2->pos();
    t1->setPos(p1);
    t2->setPos(p2);
    m_scene->addItem(t1.data());
    m_scene->addItem(t2.data());
    m_transientItems.push_back(t1.data());
    m_transientItems.push_back(t2.data());
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [t1, t2, p1, p2](const QVariant& value) {
        const qreal t = value.toReal();
        const qreal arc = std::sin(t * M_PI) * 34.0;
        if (t1) t1->setPos(p1.x() + (p2.x() - p1.x()) * t, p1.y() - arc);
        if (t2) t2->setPos(p2.x() + (p1.x() - p2.x()) * t, p2.y() + arc);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [safeN1, safeN2, val1, val2]() {
        if (safeN1) safeN1->setCustomValueText(QString::number(val2));
        if (safeN2) safeN2->setCustomValueText(QString::number(val1));
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playDualSwapAnim(ArrayBlock* b1, ArrayBlock* b2, GraphNode* n1, GraphNode* n2,
                               int val1, int val2, int duration) {
    playSwapAnim(b1, b2, val1, val2);
    playNodeSwapAnim(n1, n2, val1, val2, duration);
}
void IModule::aimPointerOutside(PointerBlock* ptr, ArrayBlock* cell, bool above) {
    if (!ptr || !cell) return;
    ptr->setDirection(above ? PointerBlock::Down : PointerBlock::Up);
    ptr->setOpacity(1);
    const double w = cell->cellWidth();
    const double h = cell->cellHeight();
    const QPointF offset = above ? PointerGeometry::aboveCell(w, h) : PointerGeometry::belowCell(w, h);
    retargetPointerTo(ptr, cell, offset);
}
void IModule::aimPointerOutsideNode(PointerBlock* ptr, GraphNode* node, bool above) {
    if (!ptr || !node) return;
    ptr->setDirection(above ? PointerBlock::Down : PointerBlock::Up);
    ptr->setOpacity(1);
    const QPointF offset = PointerGeometry::outsideNode(20.0, above);
    retargetPointerToNode(ptr, node);
    ptr->setAnchorOffset(offset);
    playPointerMove(ptr, ptr->anchoredPos());
}
void IModule::retargetPointer(PointerBlock* ptr, QGraphicsItem* anchor) {
    if (!ptr) return;
    if (anchor) {
        ptr->setAnchorItem(anchor, false);
    } else {
        ptr->clearAnchor();
    }
    retargetPointerTo(ptr, anchor, ptr->anchorOffset());
}
void IModule::retargetPointerToNode(PointerBlock* ptr, GraphNode* anchor) {
    if (!ptr) return;
    if (anchor) {
        ptr->setAnchorNode(anchor, false);
    } else {
        ptr->clearAnchor();
    }
    retargetPointerTo(ptr, anchor, ptr->anchorOffset());
}
void IModule::retargetPointerTo(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset) {
    if (!ptr) return;
    ptr->setAnchorOffset(offset);
    if (!anchor) {
        stopAnimFor(ptr);
        return;
    }
    if (ptr->anchorItem() != anchor) {
        ptr->setAnchorItem(anchor, false);
    }
    const QPointF tip = pointerTipFor(ptr, anchor);
    ptr->setTipTarget(anchor, tip);
    playPointerMove(ptr, tip);
}

void IModule::retargetPointerAcrossCells(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset) {
    if (!ptr) return;
    ptr->setAnchorOffset(offset);
    if (!anchor) {
        stopAnimFor(ptr);
        return;
    }
    if (ptr->anchorItem() != anchor) {
        ptr->setAnchorItem(anchor, false);
    }
    const QPointF tip = pointerTipFor(ptr, anchor);
    ptr->setTipTarget(anchor, tip);
    const QPointF from = ptr->pos();
    if (!m_animated) {
        stopAnimFor(ptr);
        ptr->setPos(tip);
        return;
    }
    const QRectF anchorRect = anchor->sceneBoundingRect();
    const bool fromInsideRow = std::abs(from.y() - anchorRect.center().y()) < 90.0;
    const bool toInsideRow = std::abs(tip.y() - anchorRect.center().y()) < 90.0;
    if (!fromInsideRow || !toInsideRow || std::abs(from.y() - tip.y()) < 1.0) {
        playPointerMove(ptr, tip, 420, QEasingCurve::InOutQuad);
        return;
    }
    const double detourY = qMin(from.y(), tip.y()) - 34.0;
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(520);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointer<PointerBlock> safePtr(ptr);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [safePtr, from, tip, detourY](const QVariant& v) {
        if (!safePtr) return;
        const qreal t = v.toReal();
        if (t < 0.5) {
            const qreal k = t / 0.5;
            safePtr->setPos(from.x() + (tip.x() - from.x()) * k * 0.25,
                            from.y() + (detourY - from.y()) * k);
        } else {
            const qreal k = (t - 0.5) / 0.5;
            safePtr->setPos(from.x() + (tip.x() - from.x()) * (0.25 + 0.75 * k),
                            detourY + (tip.y() - detourY) * k);
        }
    });
    QObject::connect(anim, &QVariantAnimation::finished, [safePtr, tip]() {
        if (!safePtr) return;
        safePtr->setPos(tip);
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::retargetPointerWithDirection(PointerBlock* ptr, QGraphicsItem* anchor, QPointF offset,
                                           PointerBlock::Direction dir) {
    if (!ptr) return;
    if (!anchor) {
        stopAnimFor(ptr);
        ptr->clearAnchor();
        return;
    }
    const bool flip = (ptr->direction() != dir);
    if (!flip) {
        retargetPointerAcrossCells(ptr, anchor, offset);
        return;
    }
    ptr->setAnchorOffset(offset);
    if (ptr->anchorItem() != anchor) {
        ptr->setAnchorItem(anchor, false);
    }
    const PointerBlock::Direction oldDir = ptr->direction();
    const QPointF oldTip = pointerTipFor(ptr, anchor);
    const QPointF newTip = pointerTipFor(ptr, anchor, dir);
    ptr->setTipTarget(anchor, newTip);
    const QPointF from = ptr->pos();
    if (!m_animated) {
        stopAnimFor(ptr);
        ptr->setDirection(dir);
        ptr->setPos(newTip);
        return;
    }
    const QRectF anchorRect = anchor->sceneBoundingRect();
    const double aboveY = anchorRect.top() - 34.0;
    const double belowY = anchorRect.bottom() + 34.0;
    const bool oldBelow = oldTip.y() > anchorRect.center().y();
    const QPointF stage1(from.x(), oldBelow ? belowY : aboveY);
    const QPointF stage2(newTip.x(), oldBelow ? aboveY : belowY);
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(560);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointer<PointerBlock> safePtr(ptr);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [safePtr, from, stage1, stage2, newTip](const QVariant& v) {
        if (!safePtr) return;
        const qreal t = v.toReal();
        QPointF p;
        if (t < 0.34) {
            const qreal k = t / 0.34;
            p = from + (stage1 - from) * k;
        } else if (t < 0.68) {
            const qreal k = (t - 0.34) / 0.34;
            p = stage1 + (stage2 - stage1) * k;
        } else {
            const qreal k = (t - 0.68) / 0.32;
            p = stage2 + (newTip - stage2) * k;
        }
        safePtr->setPos(p);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [safePtr, newTip, dir, oldDir]() {
        if (!safePtr) return;
        safePtr->setDirection(dir);
        safePtr->setPos(newTip);
        Q_UNUSED(oldDir);
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
int IModule::frameDelayMsForText(const QString& text) {
    const int length = text.size();
    if (length <= 0) return 900;
    int delay = 620 + length * 42;
    if (delay < 900) delay = 900;
    if (delay > 3000) delay = 3000;
    return delay;
}
int IModule::frameDelayMs(const AnimFrame& frame) {
    const int length = frame.explanation.size() + frame.mathText.size();
    if (length <= 0) return 900;
    int delay = 620 + length * 42;
    if (delay < 900) delay = 900;
    if (delay > 3000) delay = 3000;
    return delay;
}
void IModule::playNodeAppear(QObject* item, int duration) {
    if (!item) return;
    if (!m_animated) {
        item->setProperty("scale", 1.0);
        item->setProperty("opacity", 1.0);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(item, "scale", duration, QEasingCurve::OutBack);
    if (!anim) return;
    anim->setStartValue(0.15);
    anim->setKeyValueAt(0.65, 1.14);
    anim->setEndValue(1.0);
    anim->start();
    playFadeIn(item, duration);
}
void IModule::playEdgeTravel(GraphEdge* edge, QColor color, int duration) {
    playEdgeTravelFrom(edge, color, -1, duration);
}

void IModule::playEdgeTravelFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration) {
    if (!edge || !m_scene) return;
    if (!m_animated) {
        edge->setDrawProgress(1.0);
        edge->setColor(color);
        edge->clearBeam();
        edge->setBeamReversed(false);
        return;
    }
    edge->setColor(color);
    edge->setDrawProgress(1.0);
    const bool reversed = edge->source && edge->dest
                          && fromNodeId > 0 && edge->dest->id == fromNodeId
                          && edge->source->id != fromNodeId;
    edge->setBeamReversed(reversed);
    const QPointF start = edge->scenePointAt(0.0);
    const QPointF end = edge->scenePointAt(1.0);
    if ((end - start).manhattanLength() < 1.0) {
        edge->clearBeam();
        edge->setBeamReversed(false);
        return;
    }
    startBeam(edge, color, duration);
}

void IModule::startBeam(GraphEdge* edge, QColor color, int duration) {
    if (!edge || !m_scene) return;
    edge->setColor(color);
    edge->setDrawProgress(1.0);
    edge->clearBeam();

    auto it = m_beamAnims.find(edge);
    if (it != m_beamAnims.end()) {
        if (it->second) it->second->stop();
        m_beamAnims.erase(it);
    }

    QPointer<GraphEdge> guard(edge);
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [guard](const QVariant& v) {
        if (!guard) return;
        guard->setBeamProgress(v.toReal());
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, guard]() {
        if (!guard) return;
        guard->clearBeam();
        guard->setBeamReversed(false);
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    m_beamAnims[edge] = QPointer<QVariantAnimation>(anim);
}

void IModule::startBeamFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration) {
    if (!edge) return;
    const bool reversed = edge->source && edge->dest
                          && fromNodeId > 0 && edge->dest->id == fromNodeId
                          && edge->source->id != fromNodeId;
    startBeam(edge, color, duration);
    edge->setBeamReversed(reversed);
}

void IModule::playDiscover(QObject* node, QColor color, qreal maxRadius, int duration) {
    if (!node) return;
    playNodeRipple(node, color, maxRadius, duration);
    playNodePop(node, 1.24, 340);
}

void IModule::playEdgeDiscover(GraphEdge* edge, QObject* node, QColor color, int duration) {
    if (!edge) return;
    playEdgeTravel(edge, color, duration);
    if (node) playNodeRipple(node, color, 46.0, duration + 120);
}

void IModule::playNodeWake(QObject* node, QColor color, qreal maxRadius, int duration) {
    if (!node) return;
    playNodeRipple(node, color, maxRadius, duration);
}

void IModule::playFocusRing(QObject* node, QColor color, int duration) {
    if (!node) return;
    QGraphicsItem* g = dynamic_cast<QGraphicsItem*>(node);
    if (!g) return;
    if (!m_animated) {
        g->setOpacity(1.0);
        return;
    }
    playFadeItem(node, 0.55, 1.0, duration);
}
void IModule::playValueArcFlight(QPointF start, QPointF end, int value, QColor color, int duration) {
    if (!m_scene) return;
    if (!m_animated) return;
    QPointer<FloatingText> text = new FloatingText(QString::number(value));
    text->setFontSize(15);
    text->setTextColor(color);
    text->setPos(start);
    m_scene->addItem(text.data());
    m_transientItems.push_back(text.data());
    QVariantAnimation* anim = new QVariantAnimation(text.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [text, start, end](const QVariant& value) {
        if (!text) return;
        const qreal t = value.toReal();
        const qreal arc = std::sin(t * M_PI) * 46.0;
        text->setPos(start.x() + (end.x() - start.x()) * t,
                     start.y() + (end.y() - start.y()) * t - arc);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, text]() {
        if (!text) return;
        discardTransient(text.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playCellSweep(const std::vector<ArrayBlock*>& cells, QColor color, int stagger, int duration) {
    if (!m_animated) return;
    int index = 0;
    for (ArrayBlock* cell : cells) {
        if (!cell) continue;
        const int delay = index * stagger;
        ++index;
        const QColor base = cell->color;
        cell->color = color;
        cell->update();
        QPointer<ArrayBlock> safe = cell;
        QVariantAnimation* anim = new QVariantAnimation(safe.data());
        anim->setDuration(duration);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        QPointer<QVariantAnimation> animGuard(anim);
        QTimer::singleShot(delay, [animGuard]() {
            if (!animGuard) return;
            animGuard->start(QAbstractAnimation::DeleteWhenStopped);
        });
        QObject::connect(anim, &QVariantAnimation::valueChanged, [safe](const QVariant& value) {
            if (!safe) return;
            safe->setScale(1.0 + 0.12 * (1.0 - std::abs(value.toReal() * 2.0 - 1.0)));
        });
        QObject::connect(anim, &QVariantAnimation::finished, [safe, base]() {
            if (!safe) return;
            safe->color = base;
            safe->setScale(1.0);
            safe->update();
        });
    }
}

void IModule::playCellLanding(ArrayBlock* cell, QColor color, int duration) {
    if (!cell) return;
    if (!m_animated) {
        cell->color = color;
        cell->update();
        return;
    }
    cell->color = color;
    cell->update();
    playScalePulse(cell, 1.26, duration);
    playCellRipple(cell, color, 1.1, duration + 160);
}
void IModule::playEdgeGrow(GraphEdge* edge, int duration) {
    if (!edge) return;
    if (!m_animated) {
        edge->setDrawProgress(1.0);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(edge, "drawProgress", duration, QEasingCurve::OutCubic);
    if (!anim) return;
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start();
}
void IModule::completeEdgeGrowth(std::vector<GraphEdge*>& edges) {
    for (GraphEdge* edge : edges) {
        if (!edge) continue;
        if (edge->opacity() < 0.02) continue;
        if (edge->drawProgress() < 1.0) edge->setDrawProgress(1.0);
    }
}
void IModule::completeVisibleEdges() {
    if (!m_scene) return;
    for (QGraphicsItem* item : m_scene->items()) {
        GraphEdge* edge = qgraphicsitem_cast<GraphEdge*>(item);
        if (!edge) continue;
        if (edge->opacity() < 0.02) continue;
        if (edge->drawProgress() < 1.0) edge->setDrawProgress(1.0);
        if (edge->beamProgress() >= 0.0) edge->clearBeam();
    }
}
void IModule::playEdgeSweep(GraphEdge* edge, QColor color, int duration) {
    if (!edge) return;
    if (!m_animated) {
        edge->setDrawProgress(1.0);
        edge->setColor(color);
        return;
    }
    edge->setDrawProgress(0.0);
    edge->setColor(color);
    QPropertyAnimation* anim = acquireAnim(edge, "drawProgress", duration, QEasingCurve::OutCubic);
    if (!anim) return;
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start();
}
void IModule::playNodeRipple(QObject* item, QColor color, qreal maxRadius, int duration) {
    if (!item || !m_scene) return;
    if (!m_animated) return;
    QGraphicsItem* g = dynamic_cast<QGraphicsItem*>(item);
    if (!g) return;
    ArrayBlock* cell = qgraphicsitem_cast<ArrayBlock*>(g);
    if (cell) {
        playCellRipple(cell, color, 1.0, duration);
        return;
    }
    playRipple(g->pos(), color, maxRadius, duration);
}
void IModule::playNodePop(QObject* item, qreal peak, int duration) {
    if (!item) return;
    if (!m_animated) return;
    playScalePulse(item, peak, duration);
}
void IModule::playEdgeTrace(GraphEdge* edge, QColor color, int duration) {
    if (!edge || !m_scene) return;
    if (!m_animated) {
        edge->setDrawProgress(1.0);
        edge->setColor(color);
        return;
    }
    edge->setColor(color);
    edge->setDrawProgress(1.0);
    QPointF start = edge->scenePointAt(0.0);
    QPointF end = edge->scenePointAt(1.0);
    if ((end - start).manhattanLength() < 1.0) return;

    QPointer<TracerDot> dot = new TracerDot(color);
    dot->setPos(start);
    m_scene->addItem(dot.data());
    m_transientDots.push_back(dot.data());

    QVariantAnimation* anim = new QVariantAnimation(dot.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [dot, edge](const QVariant& value) {
        if (!dot || !edge) return;
        const qreal t = value.toReal();
        dot->setPos(edge->scenePointAt(t));
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, dot]() {
        if (!dot) return;
        discardTransient(dot.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playEdgeTraceFrom(GraphEdge* edge, QColor color, int fromNodeId, int duration) {
    if (!edge) return;
    const bool reversed = edge->source && edge->dest
                          && fromNodeId > 0 && edge->dest->id == fromNodeId
                          && edge->source->id != fromNodeId;
    if (!m_animated) {
        edge->setDrawProgress(1.0);
        edge->setColor(color);
        return;
    }
    edge->setColor(color);
    edge->setDrawProgress(1.0);
    QPointF start = edge->scenePointAt(0.0);
    QPointF end = edge->scenePointAt(1.0);
    if ((end - start).manhattanLength() < 1.0) return;

    QPointer<TracerDot> dot = new TracerDot(color);
    dot->setPos(reversed ? end : start);
    m_scene->addItem(dot.data());
    m_transientDots.push_back(dot.data());

    QVariantAnimation* anim = new QVariantAnimation(dot.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [dot, edge, reversed](const QVariant& value) {
        if (!dot || !edge) return;
        const qreal t = value.toReal();
        dot->setPos(edge->scenePointAt(reversed ? 1.0 - t : t));
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, dot]() {
        if (!dot) return;
        discardTransient(dot.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playFadeIn(QObject* item, int duration) {
    if (!item) return;
    if (!m_animated) {
        item->setProperty("opacity", 1.0);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(item, "opacity", duration, QEasingCurve::InOutQuad);
    if (!anim) return;
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start();
}
void IModule::playFadeItem(QObject* item, qreal from, qreal to, int duration) {
    if (!item) return;
    if (!m_animated) {
        stopAnimFor(item);
        if (QGraphicsItem* g = qobject_cast<QGraphicsItem*>(item)) g->setOpacity(to);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(item, "opacity", duration, QEasingCurve::InOutQuad);
    if (!anim) return;
    anim->setStartValue(from);
    anim->setEndValue(to);
    anim->start();
}
GraphEdge* IModule::createEdge(GraphNode* u, GraphNode* v, const GraphModuleUtils::GraphOptions& options, const GraphModuleUtils::EdgeInput& edge, bool hasWeight) {
    if (!u || !v || !m_scene) return nullptr;
    bool directed = options.directed;
    bool bidirectional = !options.directed && options.bidirectional;
    GraphEdge* item = new GraphEdge(u, v, directed, bidirectional);
    if (hasWeight) item->setWeight(edge.w);
    m_scene->addItem(item);
    u->addEdge(item);
    v->addEdge(item);
    return item;
}
void IModule::drawLabel(const QString& text, QPointF pos, QColor color, int size) {
    if (!m_scene) return;
    QGraphicsTextItem* item = new QGraphicsTextItem(text);
    item->setDefaultTextColor(color);
    QFont f("Microsoft YaHei UI");
    if (f.family().isEmpty() || f.family().contains("Sans")) f = QFont("Segoe UI");
    f.setPointSize(size);
    f.setBold(true);
    item->setFont(f);
    item->setPos(pos);
    item->setZValue(4);
    m_scene->addItem(item);
}
void IModule::drawCaption(const QString& text, QPointF pos, QColor color, int size) {
    if (!m_scene) return;
    QGraphicsTextItem* item = new QGraphicsTextItem(text);
    item->setDefaultTextColor(color);
    QFont f("Microsoft YaHei UI");
    if (f.family().isEmpty() || f.family().contains("Sans")) f = QFont("Segoe UI");
    f.setPointSize(size);
    item->setFont(f);
    item->setPos(pos);
    item->setZValue(4);
    m_scene->addItem(item);
}
void IModule::applyNodeHighlight(GraphNode* node, QColor color) {
    if (!node) return;
    node->setHighlightColor(color);
}
void IModule::clearNodeHighlights() {
    if (!m_scene) return;
    for (QGraphicsItem* item : m_scene->items()) {
        GraphNode* node = dynamic_cast<GraphNode*>(item);
        if (node) node->setHighlightColor(Qt::transparent);
    }
}
void IModule::attachEdge(GraphNode* node, GraphEdge* edge) {
    if (!node || !edge) return;
    node->addEdge(edge);
}
void IModule::detachAllEdges(std::vector<GraphNode*>& nodes, std::vector<GraphEdge*>& edges) {
    for (GraphEdge* edge : edges) {
        if (edge) edge->detach();
    }
    edges.clear();
    for (GraphNode* node : nodes) {
        if (node) node->removeAllEdges();
    }
    nodes.clear();
}
void IModule::playMoveTo(std::vector<QGraphicsObject*> items, std::vector<QPointF> targets, int duration) {
    if (items.empty() || items.size() != targets.size()) return;
    std::vector<QPointF> starts;
    for (size_t i = 0; i < items.size(); ++i) {
        if (!items[i]) continue;
        starts.push_back(items[i]->pos());
    }
    if (!m_animated) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (!items[i]) continue;
            items[i]->setPos(targets[i]);
        }
        return;
    }
    for (QGraphicsObject* item : items) {
        if (!item) continue;
        auto it = m_rowShiftAnims.find(item);
        if (it != m_rowShiftAnims.end() && it->second) {
            it->second->stop();
            it->second->deleteLater();
            m_rowShiftAnims.erase(it);
        }
    }
    std::vector<QPointer<QGraphicsObject>> safeItems;
    for (QGraphicsObject* item : items) safeItems.push_back(QPointer<QGraphicsObject>(item));
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [safeItems, starts, targets](const QVariant& progress) {
        const qreal t = progress.toReal();
        for (size_t i = 0; i < safeItems.size(); ++i) {
            if (!safeItems[i]) continue;
            const QPointF s = starts[i];
            const QPointF e = targets[i];
            safeItems[i]->setPos(s.x() + (e.x() - s.x()) * t, s.y() + (e.y() - s.y()) * t);
        }
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, safeItems]() {
        for (const QPointer<QGraphicsObject>& item : safeItems) {
            if (!item) continue;
            m_rowShiftAnims.erase(item.data());
        }
    });
    for (QGraphicsItem* item : items) {
        if (!item) continue;
        m_rowShiftAnims[item] = QPointer<QVariantAnimation>(anim);
    }
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playValueDropInto(QPointF start, ArrayBlock* target, int value, QColor color, int duration) {
    if (!target || !m_scene) return;
    const int generation = target->property("value_flight_generation").toInt() + 1;
    target->setProperty("value_flight_generation", generation);
    const QPointF end = target->centerPos();
    if (!m_animated) {
        target->value = value;
        target->showValue = true;
        target->update();
        return;
    }
    QPointer<ArrayBlock> safeTarget = target;
    QPointer<FloatingText> text = new FloatingText(QString::number(value));
    text->setFontSize(15);
    text->setTextColor(color);
    const QRectF box = text->boundingRect();
    const QPointF offset(box.width() / 2.0, box.height() / 2.0);
    text->setPos(start - offset);
    m_scene->addItem(text.data());
    m_transientItems.push_back(text.data());
    QVariantAnimation* anim = new QVariantAnimation(target);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [text, start, end, offset](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!text) return;
        text->setPos(start.x() + (end.x() - start.x()) * t - offset.x(),
                     start.y() + (end.y() - start.y()) * t - offset.y());
    });
    auto commit = [safeTarget, value]() {
        if (!safeTarget) return;
        safeTarget->value = value;
        safeTarget->showValue = true;
        safeTarget->update();
    };
    m_pendingCommits.push_back(commit);
    QObject::connect(anim, &QVariantAnimation::finished, [this, safeTarget, text, commit]() {
        if (text) {
            discardTransient(text.data());
        }
        if (!safeTarget) return;
        commit();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playValueFlyOut(QPointF start, int value, QColor color, int duration) {
    if (!m_scene) return;
    if (!m_animated) return;
    QPointer<FloatingText> text = new FloatingText(QString::number(value));
    text->setFontSize(15);
    text->setTextColor(color);
    const QRectF box = text->boundingRect();
    const QPointF offset(box.width() / 2.0, box.height() / 2.0);
    const QPointF begin = start - offset;
    const QPointF end = begin + QPointF(0.0, -46.0);
    text->setPos(begin);
    m_scene->addItem(text.data());
    m_transientItems.push_back(text.data());
    QVariantAnimation* anim = new QVariantAnimation(text.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [text, begin, end](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!text) return;
        text->setPos(begin.x() + (end.x() - begin.x()) * t,
                     begin.y() + (end.y() - begin.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, text]() {
        if (text) {
            if (text->scene()) text->scene()->removeItem(text);
            delete text.data();
        }
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playTextFlight(const QString& text, QPointF start, QPointF end,
                             int fontSize, QColor color, int duration,
                             std::function<void()> onArrive) {
    if (!m_scene) return;
    if (!m_animated) {
        if (onArrive) onArrive();
        return;
    }
    QPointer<FloatingText> ft = new FloatingText(text);
    ft->setFontSize(fontSize);
    ft->setTextColor(color);
    ft->setPos(start);
    ft->setZValue(6);
    m_scene->addItem(ft.data());
    m_transientItems.push_back(ft.data());
    QVariantAnimation* anim = new QVariantAnimation(ft.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    const double halfW = ft->boundingRect().width() / 2.0;
    const double startOffset = -halfW;
    const double endOffset = -halfW;
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ft, start, end, startOffset, endOffset](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!ft) return;
        ft->setPos(start.x() + (end.x() - start.x()) * t + startOffset + (endOffset - startOffset) * t,
                   start.y() + (end.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, ft, onArrive]() {
        if (ft) {
            discardTransient(ft.data());
        }
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playTextFlightAligned(const QString& text, QPointF start, QPointF endLeft,
                                    int fontSize, QColor color, int duration,
                                    std::function<void()> onArrive) {
    if (!m_scene) {
        if (onArrive) onArrive();
        return;
    }
    if (!m_animated) {
        if (onArrive) onArrive();
        return;
    }
    QPointer<FloatingText> ft = new FloatingText(text);
    ft->setFontSize(fontSize);
    ft->setTextColor(color);
    ft->setZValue(6);
    m_scene->addItem(ft.data());
    m_transientItems.push_back(ft.data());
    QVariantAnimation* anim = new QVariantAnimation(ft.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ft, start, endLeft](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!ft) return;
        ft->setPos(start.x() + (endLeft.x() - start.x()) * t,
                   start.y() + (endLeft.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, ft, onArrive]() {
        if (ft) {
            discardTransient(ft.data());
        }
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playRipple(QPointF center, QColor color, qreal maxRadius, int duration) {
    if (!m_scene) return;
    if (!m_animated) return;
    QPointer<Ripple> ripple = new Ripple(center, color);
    m_scene->addItem(ripple.data());
    m_transientRipples.push_back(ripple.data());
    QVariantAnimation* anim = new QVariantAnimation(ripple.data());
    anim->setDuration(duration);
    anim->setStartValue(6.0);
    anim->setEndValue(maxRadius);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ripple](const QVariant& value) {
        if (ripple) ripple->setRadius(value.toReal());
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, ripple]() {
        if (!ripple) return;
        discardTransient(ripple.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playCellRipple(ArrayBlock* cell, QColor color, qreal expandRatio, int duration) {
    if (!cell || !m_scene) return;
    if (!m_animated) return;
    const QSizeF size(cell->cellWidth(), cell->cellHeight());
    playCellRippleAt(cell->centerPos(), size, color,
                     expandRatio * cell->cellHeight() * 0.5, duration);
}

void IModule::playCellRippleAt(QPointF center, QSizeF size, QColor color, qreal expand, int duration) {
    if (!m_scene) return;
    if (!m_animated) return;
    QPointer<RectRipple> ripple = new RectRipple(center, size, color);
    m_scene->addItem(ripple.data());
    m_transientRectRipples.push_back(ripple.data());
    QVariantAnimation* anim = new QVariantAnimation(ripple.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(qMax(3.0, expand));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ripple](const QVariant& value) {
        if (ripple) ripple->setGrow(value.toReal());
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, ripple]() {
        if (!ripple) return;
        discardTransient(ripple.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playPathTrail(PathTrail* path, QVector<QPointF> points, QColor color, int duration) {
    if (!path) return;
    if (points.size() < 2) return;
    path->setPoints(points);
    path->setColor(color);
    if (!m_animated) {
        path->setProgress(1.0);
        return;
    }
    path->setProgress(0.0);
    QVariantAnimation* anim = new QVariantAnimation(path);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointer<PathTrail> safePath(path);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [safePath](const QVariant& value) {
        if (safePath) safePath->setProgress(value.toReal());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playPathTrailAppend(PathTrail* path, QPointF point, QColor color, int duration) {
    if (!path) return;
    path->appendPoint(point);
    path->setColor(color);
    if (!m_animated) {
        path->setProgress(1.0);
        return;
    }
    const int total = path->pointCount();
    if (total < 2) return;
    QVariantAnimation* anim = new QVariantAnimation(path);
    anim->setDuration(duration);
    anim->setStartValue((total - 2) / static_cast<double>(total - 1));
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QPointer<PathTrail> safePath(path);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [safePath](const QVariant& value) {
        if (safePath) safePath->setProgress(value.toReal());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playLinkRedirect(LinkLine* line, QPointF start, QPointF end, int duration) {
    if (!line) return;
    if (!m_animated) {
        line->setPoints(start, end);
        return;
    }
    QPropertyAnimation* anim = acquireAnim(line, "pos", duration, QEasingCurve::InOutCubic);
    if (!anim) return;
    anim->setStartValue(line->pos());
    anim->setEndValue(QPointF(0, 0));
    anim->start();
    QVariantAnimation* morph = new QVariantAnimation(line);
    morph->setDuration(duration);
    morph->setStartValue(0.0);
    morph->setEndValue(1.0);
    morph->setEasingCurve(QEasingCurve::InOutCubic);
    QPointF s0 = line->start;
    QPointF e0 = line->end;
    QObject::connect(morph, &QVariantAnimation::valueChanged, [line, s0, e0, start, end](const QVariant& progress) {
        qreal t = progress.toReal();
        if (!line) return;
        line->setPoints(s0 + (start - s0) * t, e0 + (end - e0) * t);
    });
    morph->start(QAbstractAnimation::DeleteWhenStopped);
}
void IModule::playBubbleFlight(QPointF start, QPointF end, int value, int duration) {
    if (!m_scene) return;
    if (!m_animated) return;
    QPointer<FloatingText> ft = new FloatingText(QString::number(value));
    ft->setPos(start);
    ft->setZValue(5);
    m_scene->addItem(ft.data());
    m_transientItems.push_back(ft.data());
    QVariantAnimation* anim = new QVariantAnimation(ft.data());
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [ft, start, end](const QVariant& progress) {
        qreal t = progress.toReal();
        if (!ft) return;
        qreal arc = std::sin(t * M_PI) * 38.0;
        ft->setPos(start.x() + (end.x() - start.x()) * t,
                   start.y() + (end.y() - start.y()) * t - arc);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [this, ft]() {
        if (!ft) return;
        discardTransient(ft.data());
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}