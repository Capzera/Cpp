#include "LiveEditArrayBase.h"
#include <QRegularExpression>

QString LiveEditArrayBase::makeKey(const QString& row, int index) const {
    return QString("%1#%2").arg(row).arg(index);
}

void LiveEditArrayBase::bindBlock(ArrayBlock* block, const QString& row, int index, int fallback) {
    if (!block) return;
    QString key = makeKey(row, index);
    EditableModel::instance().declare(key, fallback);
    SlotBinding binding;
    binding.key = key;
    binding.block = block;
    binding.fallback = fallback;
    m_bindings[key] = binding;
    block->setEditable(true);
    connect(block, &ArrayBlock::valueEdited, this, [this](int, const QString& text) {
        if (!m_scene) return;
        for (auto& kv : m_bindings) {
            if (kv.second.block == sender()) {
                applyLiveEdit(kv.first, text);
                return;
            }
        }
    });
}

void LiveEditArrayBase::bindNode(GraphNode* node, const QString& row, int index, int fallback) {
    if (!node) return;
    QString key = makeKey(row, index);
    EditableModel::instance().declare(key, fallback);
    SlotBinding binding;
    binding.key = key;
    binding.node = node;
    binding.fallback = fallback;
    m_bindings[key] = binding;
    node->setEditable(true);
}

void LiveEditArrayBase::bindEdge(GraphEdge* edge, const QString& row, int index, int fallback) {
    if (!edge) return;
    QString key = makeKey(row, index);
    EditableModel::instance().declare(key, fallback);
    SlotBinding binding;
    binding.key = key;
    binding.edge = edge;
    binding.fallback = fallback;
    m_bindings[key] = binding;
    edge->setEditable(true);
}

QString LiveEditArrayBase::slotKeyFor(ArrayBlock* block) const {
    if (!block) return QString();
    for (auto& kv : m_bindings) {
        if (kv.second.block == block) return kv.first;
    }
    return QString();
}

QString LiveEditArrayBase::slotKeyFor(GraphNode* node) const {
    if (!node) return QString();
    for (auto& kv : m_bindings) {
        if (kv.second.node == node) return kv.first;
    }
    return QString();
}

QString LiveEditArrayBase::slotKeyFor(GraphEdge* edge) const {
    if (!edge) return QString();
    for (auto& kv : m_bindings) {
        if (kv.second.edge == edge) return kv.first;
    }
    return QString();
}

int LiveEditArrayBase::slotValue(const QString& row, int index, int fallback) const {
    QString key = makeKey(row, index);
    return EditableModel::instance().value(key, fallback);
}

std::vector<QString> LiveEditArrayBase::keysForRow(const QString& row) const {
    std::vector<QString> out;
    QString prefix = row + "#";
    for (auto& kv : m_bindings) {
        if (kv.first.startsWith(prefix)) out.push_back(kv.first);
    }
    return out;
}

void LiveEditArrayBase::refreshSlot(const QString& key) {
    auto it = m_bindings.find(key);
    if (it == m_bindings.end()) return;
    int value = EditableModel::instance().value(key, it->second.fallback);
    if (it->second.block) {
        it->second.block->value = value;
        it->second.block->customValue.clear();
        it->second.block->showValue = true;
        it->second.block->update();
    }
    if (it->second.node) {
        it->second.node->value = value;
        it->second.node->valueText = QString::number(value);
        it->second.node->update();
    }
    if (it->second.edge) {
        it->second.edge->weight = value;
        it->second.edge->update();
    }
}

void LiveEditArrayBase::refreshAllSlots() {
    for (auto& kv : m_bindings) refreshSlot(kv.first);
}

bool LiveEditArrayBase::onSlotEdited(const QString& key, const QString& text) {
    if (key.isEmpty()) return false;
    bool ok = false;
    text.toInt(&ok);
    if (!ok) return false;
    refreshSlot(key);
    return true;
}
