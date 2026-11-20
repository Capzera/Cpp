#include "BucketSortModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kBkSrc = QColor(252, 252, 254);
static const QColor kBkScan = QColor(255, 210, 145);
static const QColor kBkBucket = QColor(196, 226, 250);
static const QColor kBkSlot = QColor(244, 246, 250);
static const QColor kBkFilled = QColor(255, 196, 150);
static const QColor kBkOut = QColor(150, 214, 170);

static const double kBkBucketW = 120.0;
static const double kBkBucketGap = 24.0;
static const double kBkSlotH = 40.0;

BucketSortModule::BucketSortModule() {}

QString BucketSortModule::getModuleName() const { return "桶排序"; }
QString BucketSortModule::getDefaultInput() const { return "8\n37 22 11 45 13 30 26 8"; }
QString BucketSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n <= 10）\n"
           "第二行输入 n 个整数（取值范围 0~99）\n"
           "算法把数值按大小分进 5 个桶，每个桶内再排序，最后按桶序拼接";
}

ArrayBlock* BucketSortModule::bucketSlot(int bucket, int slot) const {
    if (bucket < 0 || bucket >= static_cast<int>(m_bucketSlots.size())) return nullptr;
    if (slot < 0 || slot >= static_cast<int>(m_bucketSlots[bucket].size())) return nullptr;
    return m_bucketSlots[bucket][slot];
}

void BucketSortModule::layoutBuckets() {
    const double cell = 58.0;
    const double srcX = std::max(210.0, 470.0 - static_cast<double>(m_data.size()) * cell / 2.0);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i], QString::number(i));
        block->setCellSize(cell, 40);
        block->color = kBkSrc;
        block->setPos(srcX + i * cell, 110);
        m_scene->addItem(block);
        m_srcBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }

    const double totalW = kBucketCount * kBkBucketW + (kBucketCount - 1) * kBkBucketGap;
    const double startX = std::max(60.0, 470.0 - totalW / 2.0);

    for (int b = 0; b < kBucketCount; ++b) {
        const double x = startX + b * (kBkBucketW + kBkBucketGap);
        const int lo = b * 20;
        const int hi = lo + 19;

        ArrayBlock* head = new ArrayBlock(b, 0, QString("head"));
        head->setCellSize(kBkBucketW, 38);
        head->setShowLabel(false);
        head->setCustomValue(QString("桶 %1 [%2, %3]").arg(b).arg(lo).arg(hi));
        head->setValueFontSize(11);
        head->color = kBkBucket;
        head->setPos(x, 250);
        m_scene->addItem(head);
        m_bucketHeads.push_back(head);

        std::vector<ArrayBlock*> bucketRow;
        for (int s = 0; s < kBucketSlots; ++s) {
            ArrayBlock* cell2 = new ArrayBlock(b * kBucketSlots + s, 0, QString::number(s));
            cell2->setCellSize(kBkBucketW, kBkSlotH);
            cell2->setShowLabel(false);
            cell2->showValue = false;
            cell2->color = kBkSlot;
            cell2->setPos(x, 292 + s * kBkSlotH);
            m_scene->addItem(cell2);
            bucketRow.push_back(cell2);
        }
        m_bucketSlots.push_back(bucketRow);
    }

    const double outX = std::max(210.0, 470.0 - static_cast<double>(m_data.size()) * cell / 2.0);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(1000 + i, 0, QString::number(i));
        block->setCellSize(cell, 40);
        block->showValue = false;
        block->setPos(outX + i * cell, 506);
        m_scene->addItem(block);
        m_outBlocks.push_back(block);
    }
}

void BucketSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!tokens.isEmpty()) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 10 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                const int value = tokens[i + 1].toInt(&ok);
                if (!ok || value < 0 || value > 99) {
                    m_valid = false;
                    break;
                }
                m_data.push_back(value);
            }
        }
    }

    m_mathText = createScriptText(scene, m_valid ? "桶排序：按数值范围分桶 → 桶内排序 → 按桶序拼接"
                                        : "输入格式：n，随后输入 n 个 0~99 的整数；n≤10");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(140, 18);
    if (!m_valid) return;

    m_bucketContent.assign(kBucketCount, {});
    layoutBuckets();

    QGraphicsTextItem* srcLabel = m_scene->addText("a[] 原数组");
    srcLabel->setFont(UiFonts::cjk(11, true));
    srcLabel->setDefaultTextColor(QColor(60, 75, 95));
    srcLabel->setPos(40, 96);

    QGraphicsTextItem* bucketLabel = m_scene->addText("5 个桶（每桶容量 4，桶内竖排）");
    bucketLabel->setFont(UiFonts::cjk(11, true));
    bucketLabel->setDefaultTextColor(QColor(60, 75, 95));
    bucketLabel->setPos(40, 180);

    QGraphicsTextItem* outLabel = m_scene->addText("out[] 输出");
    outLabel->setFont(UiFonts::cjk(11, true));
    outLabel->setDefaultTextColor(QColor(60, 75, 95));
    outLabel->setPos(40, 484);

    m_noteText = createScriptText(m_scene, "");
    m_noteText->setFont(UiFonts::mono(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(140, 578);

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Down);
    m_ptrI->setShaftLength(40);
    m_ptrI->setAnchorOffset(PointerGeometry::aboveCell(58.0, 40.0));
    m_ptrI->setLabelAtTail(8.0);
    scene->addItem(m_ptrI);

    m_ptrB = new PointerBlock("b", QColor(45, 130, 210));
    m_ptrB->setOpacity(0);
    m_ptrB->setDirection(PointerBlock::Down);
    m_ptrB->setShaftLength(24);
    m_ptrB->setAnchorOffset(PointerGeometry::aboveCell(kBkBucketW, 38.0));
    m_ptrB->setLabelAtTail(8.0);
    scene->addItem(m_ptrB);
}

std::vector<AnimFrame> BucketSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);

    m_frames.push_back({ActionType::BK_RESET, 0, 0, 0, 0, {},
                        QString("初始化 %1 个空桶").arg(kBucketCount),
                        QString("桶 %1 的范围是 [%2, %3]").arg(0).arg(0).arg(19)});

    std::vector<std::vector<int>> buckets(kBucketCount);
    for (int i = 0; i < n; ++i) {
        const int value = m_data[i];
        const int b = value / kBucketWidth;
        buckets[b].push_back(value);
        m_frames.push_back({ActionType::BK_BUCKET, i, b, value, static_cast<int>(buckets[b].size()) - 1, {},
                            QString("a[%1] = %2 落入桶 %3（范围 [%4, %5]）")
                                .arg(i).arg(value).arg(b).arg(b * kBucketWidth).arg(b * kBucketWidth + 19),
                            QString("b = %1 / %2 = %3").arg(value).arg(kBucketWidth).arg(b)});
    }

    for (int b = 0; b < kBucketCount; ++b) {
        if (buckets[b].empty()) continue;
        std::sort(buckets[b].begin(), buckets[b].end());
        for (int s = 0; s < static_cast<int>(buckets[b].size()); ++s) {
            m_frames.push_back({ActionType::BK_SORT, b, s, buckets[b][s], 0, {},
                                QString("桶 %1 内部排序后第 %2 个元素是 %3")
                                    .arg(b).arg(s + 1).arg(buckets[b][s]),
                                QString("桶内升序")});
        }
    }

    int slot = 0;
    for (int b = 0; b < kBucketCount; ++b) {
        for (int s = 0; s < static_cast<int>(buckets[b].size()); ++s) {
            m_frames.push_back({ActionType::BK_COLLECT, b, s, buckets[b][s], slot, {},
                                QString("桶 %1 的元素 %2 写入 out[%3]")
                                    .arg(b).arg(buckets[b][s]).arg(slot),
                                QString("out[%1] = %2").arg(slot).arg(buckets[b][s])});
            ++slot;
        }
    }

    m_frames.push_back({ActionType::BK_DONE, n, kBucketCount, 0, 0, {},
                        QString("桶排序完成，共 %1 个元素").arg(n),
                        QString("桶排序复杂度 O(n + k)")});
    return m_frames;
}

void BucketSortModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::BK_RESET) {
        for (auto* b : m_srcBlocks) { b->color = kBkSrc; b->update(); }
        for (auto* b : m_bucketHeads) { b->color = kBkBucket; b->update(); }
        for (auto& row : m_bucketSlots) {
            for (auto* b : row) { b->showValue = false; b->color = kBkSlot; b->update(); }
        }
        for (auto* b : m_outBlocks) { b->showValue = false; b->color = Qt::white; b->update(); }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) setScriptText(m_noteText, "");
    } else if (frame.type == ActionType::BK_BUCKET) {
        for (auto* b : m_srcBlocks) {
            if (b->color == kBkScan) { b->color = kBkSrc; b->update(); }
        }
        const int index = frame.u;
        const int bucket = frame.v;
        const int value = frame.val1;
        const int slot = frame.val2;
        if (index >= 0 && index < static_cast<int>(m_srcBlocks.size())) {
            m_srcBlocks[index]->color = kBkScan;
            m_srcBlocks[index]->update();
            if (m_ptrI) {
                m_ptrI->setOpacity(1);
                retargetPointerTo(m_ptrI, m_srcBlocks[index], PointerGeometry::aboveCell(58.0, 40.0));
            }
        }
        if (bucket >= 0 && bucket < static_cast<int>(m_bucketHeads.size())) {
            m_bucketHeads[bucket]->color = kBkFilled;
            m_bucketHeads[bucket]->update();
            if (m_ptrB) {
                m_ptrB->setOpacity(1);
                retargetPointerTo(m_ptrB, m_bucketHeads[bucket], PointerGeometry::aboveCell(kBkBucketW, 38.0));
            }
        }
        ArrayBlock* target = bucketSlot(bucket, slot);
        if (target) {
            target->setCustomValue(QString::number(value));
            target->color = kBkFilled;
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.16, 240);
                playCellLanding(target, kBkFilled, 380);
            }
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("a[%1] = %2 → 桶 %3 第 %4 格")
                                     .arg(index).arg(value).arg(bucket).arg(slot + 1));
        }
    } else if (frame.type == ActionType::BK_SORT) {
        const int bucket = frame.u;
        const int slot = frame.v;
        const int value = frame.val1;
        ArrayBlock* target = bucketSlot(bucket, slot);
        if (target) {
            target->setCustomValue(QString::number(value));
            target->color = QColor(176, 220, 250);
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.12, 220);
                playCellLanding(target, QColor(176, 220, 250), 340);
            }
        }
        if (m_ptrB && bucket >= 0 && bucket < static_cast<int>(m_bucketHeads.size())) {
            m_ptrB->setOpacity(1);
            retargetPointerTo(m_ptrB, m_bucketHeads[bucket], PointerGeometry::aboveCell(kBkBucketW, 38.0));
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("桶 %1 内部升序：第 %2 格 = %3").arg(bucket).arg(slot + 1).arg(value));
        }
    } else if (frame.type == ActionType::BK_COLLECT) {
        const int bucket = frame.u;
        const int slot = frame.v;
        const int value = frame.val1;
        const int outIndex = frame.val2;
        ArrayBlock* src = bucketSlot(bucket, slot);
        if (outIndex >= 0 && outIndex < static_cast<int>(m_outBlocks.size())) {
            const QPointF from = src ? src->pos() : QPointF(200, 292);
            playValueFlight(from, m_outBlocks[outIndex], value, kBkOut, true);
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("桶 %1 → out[%2] = %3").arg(bucket).arg(outIndex).arg(value));
        }
    } else if (frame.type == ActionType::BK_DONE) {
        for (auto* b : m_outBlocks) {
            if (b->showValue) { b->color = kBkOut; b->update(); }
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, QString("桶排序完成：%1 个元素").arg(frame.u));
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void BucketSortModule::reset() {
    m_frames.clear();
    m_data.clear();
    m_srcBlocks.clear();
    m_bucketHeads.clear();
    m_bucketSlots.clear();
    m_bucketContent.clear();
    m_outBlocks.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrI = nullptr;
    m_ptrB = nullptr;
    m_valid = false;
}

void BucketSortModule::releaseSceneItems() {
    m_srcBlocks.clear();
    m_bucketHeads.clear();
    m_bucketSlots.clear();
    m_bucketContent.clear();
    m_outBlocks.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrI = nullptr;
    m_ptrB = nullptr;
}

void BucketSortModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}
