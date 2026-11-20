#include "HashTableModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kHtEmpty = QColor(252, 252, 254);
static const QColor kHtFilled = QColor(160, 205, 245);
static const QColor kHtProbe = QColor(255, 210, 145);
static const QColor kHtKey = QColor(140, 205, 165);
static const QColor kHtConflict = QColor(245, 150, 135);

HashTableModule::HashTableModule() {}

QString HashTableModule::getModuleName() const { return "哈希表 (开放寻址)"; }
QString HashTableModule::getDefaultInput() const { return "5\n3 10 17 24 31"; }
QString HashTableModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示待插入的关键字个数（n <= 6）\n"
           "第二行输入 n 个非负整数作为关键字\n"
           "哈希函数为 h(k) = k % 7，冲突时向后线性探测找空槽";
}

void HashTableModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!tokens.isEmpty()) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 6 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                const int v = tokens[i + 1].toInt(&ok);
                if (!ok || v < 0 || v > 999) {
                    m_valid = false;
                    break;
                }
                m_keys.push_back(v);
            }
        }
    }

    m_mathText = scene->addText(m_valid ? "哈希表：h(k) = k % 7，冲突则线性探测下一个空槽"
                                        : "输入格式：n，随后输入 n 个非负整数；n≤6");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;

    const double cellW = kCellWidth;
    const double cellH = kCellHeight;
    const double left = 300.0;

    auto addLabel = [scene](const QString& text, double x, double y) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(UiFonts::cjk(11, true));
        item->setDefaultTextColor(QColor(60, 75, 95));
        item->setPos(x, y);
    };
    addLabel("槽位 table[]", 40, 110);
    addLabel("存放的关键字", 40, 214);

    for (int i = 0; i < m_capacity; ++i) {
        ArrayBlock* slot = new ArrayBlock(i, 0, QString::number(i));
        slot->setCellSize(cellW, cellH);
        slot->setShowLabel(false);
        slot->setCustomValue("—");
        slot->setPos(left + i * cellW, 118);
        scene->addItem(slot);
        m_slotBlocks.push_back(slot);

        ArrayBlock* key = new ArrayBlock(i, 0, QString::number(i));
        key->setCellSize(cellW, cellH);
        key->setShowLabel(false);
        key->showValue = false;
        key->setPos(left + i * cellW, 222);
        scene->addItem(key);
        m_keyBlocks.push_back(key);
    }

    m_probePointer = new PointerBlock("k", QColor(210, 80, 55));
    m_probePointer->setOpacity(0);
    m_probePointer->setDirection(PointerBlock::Down);
    m_probePointer->setShaftLength(26);
    m_probePointer->setAnchorOffset(PointerGeometry::aboveCell(cellW, cellH));
    m_probePointer->setLabelAtTail(10.0);
    scene->addItem(m_probePointer);

    m_probeText = scene->addText("");
    m_probeText->setFont(UiFonts::mono(14, true));
    m_probeText->setDefaultTextColor(QColor(200, 110, 40));
    m_probeText->setPos(left, 320);
}

std::vector<AnimFrame> HashTableModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<int> table(m_capacity, -1);

    m_frames.push_back({ActionType::HT_RESET, 0, 0, 0, 0, {},
                        QString("初始化 %1 个空槽位").arg(m_capacity),
                        QString("h(k) = k % %1").arg(m_capacity)});

    for (int i = 0; i < static_cast<int>(m_keys.size()); ++i) {
        const int key = m_keys[i];
        int slot = key % m_capacity;
        int probe = 0;
        m_frames.push_back({ActionType::HT_HASH, key, slot, i, 0, {},
                            QString("关键字 %1 计算哈希位置 %2").arg(key).arg(slot),
                            QString("h(%1) = %1 %% %2 = %3").arg(key).arg(m_capacity).arg(slot)});
        while (table[slot] != -1) {
            ++probe;
            const int next = (slot + 1) % m_capacity;
            m_frames.push_back({ActionType::HT_PROBE, key, slot, next, probe, {},
                                QString("冲突！槽 %1 已被 %2 占用，线性探测到槽 %3").arg(slot).arg(table[slot]).arg(next),
                                QString("槽 %1 冲突 → 探测槽 %2").arg(slot).arg(next)});
            slot = next;
        }
        table[slot] = key;
        m_frames.push_back({ActionType::HT_INSERT, key, slot, i, probe, {},
                            QString("关键字 %1 放入空槽 %2").arg(key).arg(slot),
                            QString("table[%1] = %2").arg(slot).arg(key)});
    }

    m_frames.push_back({ActionType::HT_DONE, static_cast<int>(m_keys.size()), m_capacity, 0, 0, {},
                        QString("插入完成，共 %1 个关键字").arg(m_keys.size()),
                        QString("线性探测平均代价低于链地址法")});
    return m_frames;
}

void HashTableModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::HT_RESET) {
        for (size_t i = 0; i < m_slotBlocks.size(); ++i) {
            m_slotBlocks[i]->setCustomValue("—");
            m_slotBlocks[i]->color = kHtEmpty;
            m_slotBlocks[i]->update();
            m_keyBlocks[i]->showValue = false;
            m_keyBlocks[i]->color = kHtEmpty;
            m_keyBlocks[i]->update();
        }
        m_probePointer->setOpacity(0);
        if (m_probeText) {
            m_probeText->setPlainText("");
            m_probeText->setDefaultTextColor(QColor(200, 110, 40));
        }
    } else if (frame.type == ActionType::HT_HASH) {
        const int slot = frame.v;
        if (slot >= 0 && slot < static_cast<int>(m_slotBlocks.size())) {
            m_slotBlocks[slot]->color = kHtProbe;
            m_slotBlocks[slot]->update();
            m_probePointer->setOpacity(1);
            retargetPointerTo(m_probePointer, m_slotBlocks[slot], PointerGeometry::aboveCell(cellWidth(), cellHeight()));
        }
        if (m_probeText) {
            m_probeText->setPlainText(QString("h(%1) = %1 %% %2 = %3").arg(frame.u).arg(m_capacity).arg(slot));
            m_probeText->setDefaultTextColor(QColor(200, 110, 40));
        }
    } else if (frame.type == ActionType::HT_PROBE) {
        const int from = frame.u;
        const int to = frame.v;
        if (from >= 0 && from < static_cast<int>(m_slotBlocks.size())) {
            m_slotBlocks[from]->color = kHtConflict;
            m_slotBlocks[from]->update();
        }
        if (to >= 0 && to < static_cast<int>(m_slotBlocks.size())) {
            m_slotBlocks[to]->color = kHtProbe;
            m_slotBlocks[to]->update();
            m_probePointer->setOpacity(1);
            retargetPointerTo(m_probePointer, m_slotBlocks[to], PointerGeometry::aboveCell(cellWidth(), cellHeight()));
            if (m_animated) playScalePulse(m_slotBlocks[to], 1.14, 220);
        }
        if (m_probeText) {
            m_probeText->setPlainText(QString("槽 %1 冲突（已存 %2），去探测槽 %3").arg(from).arg(frame.u).arg(to));
            m_probeText->setDefaultTextColor(kHtConflict.darker(110));
        }
    } else if (frame.type == ActionType::HT_INSERT) {
        const int slot = frame.v;
        if (slot >= 0 && slot < static_cast<int>(m_slotBlocks.size())) {
            m_slotBlocks[slot]->setCustomValue(QString::number(frame.u));
            m_slotBlocks[slot]->color = kHtFilled;
            m_slotBlocks[slot]->update();
            if (m_animated) playScalePulse(m_slotBlocks[slot], 1.18, 260);
        }
        if (slot >= 0 && slot < static_cast<int>(m_keyBlocks.size())) {
            m_keyBlocks[slot]->showValue = true;
            m_keyBlocks[slot]->value = frame.u;
            m_keyBlocks[slot]->color = kHtKey;
            m_keyBlocks[slot]->update();
        }
        if (m_probeText) {
            m_probeText->setPlainText(QString("关键字 %1 → 槽 %2（探测 %3 次）").arg(frame.u).arg(slot).arg(frame.val2));
            m_probeText->setDefaultTextColor(QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::HT_DONE) {
        for (auto* b : m_slotBlocks) {
            if (b->customValue != "—") { b->color = kHtFilled; b->update(); }
        }
        for (auto* b : m_keyBlocks) {
            if (b->showValue) { b->color = kHtKey; b->update(); }
        }
        m_probePointer->setOpacity(0);
    }
}

void HashTableModule::reset() {
    m_frames.clear();
    m_slotBlocks.clear();
    m_keyBlocks.clear();
    m_keys.clear();
    m_mathText = nullptr;
    m_probeText = nullptr;
    m_probePointer = nullptr;
    m_capacity = 7;
    m_valid = false;
}

void HashTableModule::releaseSceneItems() {
    m_slotBlocks.clear();
    m_keyBlocks.clear();
    m_mathText = nullptr;
    m_probeText = nullptr;
    m_probePointer = nullptr;
}
