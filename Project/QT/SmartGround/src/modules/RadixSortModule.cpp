#include "RadixSortModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kRdSrc = QColor(252, 252, 254);
static const QColor kRdScan = QColor(255, 210, 145);
static const QColor kRdHead = QColor(206, 226, 250);
static const QColor kRdSlot = QColor(245, 247, 251);
static const QColor kRdFilled = QColor(255, 196, 150);
static const QColor kRdOut = QColor(150, 214, 170);

static const double kRdBucketW = 84.0;
static const double kRdBucketGap = 8.0;
static const double kRdSlotH = 38.0;

RadixSortModule::RadixSortModule() {}

QString RadixSortModule::getModuleName() const { return "基数排序"; }
QString RadixSortModule::getDefaultInput() const { return "6\n329 457 657 839 436 720"; }
QString RadixSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n <= 8）\n"
           "第二行输入 n 个非负整数（均不超过 999）\n"
           "算法按 LSD（低位优先）逐位处理：个位 → 十位 → 百位，每轮按当前位分进 10 个桶再收集";
}

ArrayBlock* RadixSortModule::bucketSlot(int digit, int slot) const {
    if (digit < 0 || digit >= static_cast<int>(m_bucketSlots.size())) return nullptr;
    if (slot < 0 || slot >= static_cast<int>(m_bucketSlots[digit].size())) return nullptr;
    return m_bucketSlots[digit][slot];
}

void RadixSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!tokens.isEmpty()) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 8 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                const int value = tokens[i + 1].toInt(&ok);
                if (!ok || value < 0 || value > 999) {
                    m_valid = false;
                    break;
                }
                m_data.push_back(value);
            }
        }
    }

    m_mathText = createScriptText(scene, m_valid ? "基数排序 LSD：个位 → 十位 → 百位，每轮按当前位分桶再收集"
                                        : "输入格式：n，随后输入 n 个 0~999 的整数；n≤8");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(120, 18);
    if (!m_valid) return;

    m_maxDigit = 1;
    for (int v : m_data) {
        int digits = 1;
        int t = v;
        while (t >= 10) { t /= 10; ++digits; }
        m_maxDigit = std::max(m_maxDigit, digits);
    }

    const double cell = 58.0;
    const double srcX = std::max(200.0, 470.0 - static_cast<double>(m_data.size()) * cell / 2.0);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i], QString::number(i));
        block->setCellSize(cell, 40);
        block->color = kRdSrc;
        block->setPos(srcX + i * cell, 104);
        scene->addItem(block);
        m_srcBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }

    const double totalW = kDigitBuckets * kRdBucketW + (kDigitBuckets - 1) * kRdBucketGap;
    const double startX = std::max(30.0, 470.0 - totalW / 2.0);
    for (int d = 0; d < kDigitBuckets; ++d) {
        const double x = startX + d * (kRdBucketW + kRdBucketGap);

        ArrayBlock* head = new ArrayBlock(d, 0, QString("digit"));
        head->setCellSize(kRdBucketW, 34);
        head->setShowLabel(false);
        head->setCustomValue(QString::number(d));
        head->setValueFontSize(13);
        head->color = kRdHead;
        head->setPos(x, 240);
        scene->addItem(head);
        m_bucketHeads.push_back(head);

        std::vector<ArrayBlock*> bucketRow;
        for (int s = 0; s < static_cast<int>(m_data.size()); ++s) {
            ArrayBlock* cell2 = new ArrayBlock(d * 100 + s, 0, QString::number(s));
            cell2->setCellSize(kRdBucketW, kRdSlotH);
            cell2->setShowLabel(false);
            cell2->showValue = false;
            cell2->color = kRdSlot;
            cell2->setPos(x, 278 + s * kRdSlotH);
            scene->addItem(cell2);
            bucketRow.push_back(cell2);
        }
        m_bucketSlots.push_back(bucketRow);
    }

    const double dstY = 278 + static_cast<double>(m_data.size()) * kRdSlotH + 46.0;
    const double dstX = std::max(200.0, 470.0 - static_cast<double>(m_data.size()) * cell / 2.0);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(1000 + i, 0, QString::number(i));
        block->setCellSize(cell, 40);
        block->showValue = false;
        block->setPos(dstX + i * cell, dstY);
        scene->addItem(block);
        m_dstBlocks.push_back(block);
    }

    QGraphicsTextItem* srcLabel = scene->addText("a[] 当前序列");
    srcLabel->setFont(UiFonts::cjk(11, true));
    srcLabel->setDefaultTextColor(QColor(60, 75, 95));
    srcLabel->setPos(40, 90);

    QGraphicsTextItem* bucketLabel = scene->addText("按当前位分的 10 个桶");
    bucketLabel->setFont(UiFonts::cjk(11, true));
    bucketLabel->setDefaultTextColor(QColor(60, 75, 95));
    bucketLabel->setPos(40, 154);

    QGraphicsTextItem* dstLabel = scene->addText("收集结果");
    dstLabel->setFont(UiFonts::cjk(11, true));
    dstLabel->setDefaultTextColor(QColor(60, 75, 95));
    dstLabel->setPos(40, dstY - 14);

    m_noteText = createScriptText(scene, "");
    m_noteText->setFont(UiFonts::mono(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(120, dstY + 56);

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Down);
    m_ptrI->setShaftLength(24);
    m_ptrI->setAnchorOffset(PointerGeometry::aboveCell(58.0, 40.0));
    m_ptrI->setLabelAtTail(8.0);
    scene->addItem(m_ptrI);

    m_ptrD = new PointerBlock("d", QColor(45, 130, 210));
    m_ptrD->setOpacity(0);
    m_ptrD->setDirection(PointerBlock::Down);
    m_ptrD->setShaftLength(24);
    m_ptrD->setAnchorOffset(PointerGeometry::aboveCell(kRdBucketW, 34.0));
    m_ptrD->setLabelAtTail(8.0);
    scene->addItem(m_ptrD);
}

std::vector<AnimFrame> RadixSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);

    std::vector<int> arr = m_data;
    m_frames.push_back({ActionType::RD_RESET, 0, 0, 0, 0, {},
                        QString("初始化：准备按低位优先逐位排序"),
                        QString("LSD 基数排序")});

    int place = 1;
    for (int round = 0; round < m_maxDigit; ++round) {
        std::vector<std::vector<int>> buckets(kDigitBuckets);
        for (int i = 0; i < n; ++i) {
            const int value = arr[i];
            const int digit = (value / place) % 10;
            buckets[digit].push_back(value);
            m_frames.push_back({ActionType::RD_DIGIT, round, i, digit, value, {},
                                QString("第 %1 轮：a[%2] = %3 的当前位是 %4")
                                    .arg(round + 1).arg(i).arg(value).arg(digit),
                                QString("d = (%3 / %4) %% 10 = %5").arg(i).arg(value).arg(place).arg(digit)});
            m_frames.push_back({ActionType::RD_BUCKET, round, digit, value,
                                static_cast<int>(buckets[digit].size()) - 1, {},
                                QString("a[%1] = %2 按当前位 %3 落入桶 %3")
                                    .arg(i).arg(value).arg(digit),
                                QString("bucket[%1].push(%2)").arg(digit).arg(value)});
        }
        int outIndex = 0;
        for (int d = 0; d < kDigitBuckets; ++d) {
            for (int value : buckets[d]) {
                m_frames.push_back({ActionType::RD_COLLECT, round, outIndex, value, d, {},
                                    QString("桶 %1 的 %2 收集到结果第 %3 位").arg(d).arg(value).arg(outIndex),
                                    QString("收集顺序：桶 0 → 9")});
                ++outIndex;
            }
        }
        arr.clear();
        for (int d = 0; d < kDigitBuckets; ++d) {
            for (int value : buckets[d]) arr.push_back(value);
        }
        place *= 10;
    }

    m_frames.push_back({ActionType::RD_DONE, n, m_maxDigit, 0, 0, {},
                        QString("基数排序完成，共 %1 轮").arg(m_maxDigit),
                        QString("基数排序复杂度 O(d(n+k))")});
    return m_frames;
}

void RadixSortModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::RD_RESET) {
        for (auto* b : m_srcBlocks) { b->color = kRdSrc; b->update(); }
        for (auto* b : m_dstBlocks) { b->showValue = false; b->color = Qt::white; b->update(); }
        for (auto* b : m_bucketHeads) { b->color = kRdHead; b->update(); }
        for (auto& row : m_bucketSlots) {
            for (auto* b : row) { b->showValue = false; b->color = kRdSlot; b->update(); }
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrD) m_ptrD->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, "");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
    } else if (frame.type == ActionType::RD_DIGIT) {
        for (auto* b : m_srcBlocks) {
            if (b->color == kRdScan) { b->color = kRdSrc; b->update(); }
        }
        const int index = frame.v;
        if (index >= 0 && index < static_cast<int>(m_srcBlocks.size())) {
            m_srcBlocks[index]->color = kRdScan;
            m_srcBlocks[index]->update();
            if (m_ptrI) {
                m_ptrI->setOpacity(1);
                retargetPointerTo(m_ptrI, m_srcBlocks[index], PointerGeometry::aboveCell(58.0, 40.0));
            }
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("第 %1 轮：取 a[%2] = %3 的当前位 = %4")
                                     .arg(frame.u + 1).arg(index).arg(frame.val2).arg(frame.val1));
        }
    } else if (frame.type == ActionType::RD_BUCKET) {
        const int digit = frame.v;
        const int value = frame.val1;
        const int slot = frame.val2;
        if (digit >= 0 && digit < static_cast<int>(m_bucketHeads.size())) {
            m_bucketHeads[digit]->color = kRdFilled;
            m_bucketHeads[digit]->update();
            if (m_ptrD) {
                m_ptrD->setOpacity(1);
                retargetPointerTo(m_ptrD, m_bucketHeads[digit], PointerGeometry::aboveCell(kRdBucketW, 34.0));
            }
        }
        ArrayBlock* target = bucketSlot(digit, slot);
        if (target) {
            target->setCustomValue(QString::number(value));
            target->color = kRdFilled;
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.14, 220);
                playCellLanding(target, kRdFilled, 380);
            }
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("第 %1 轮：%2 进入桶 %3").arg(frame.u + 1).arg(value).arg(digit));
        }
    } else if (frame.type == ActionType::RD_COLLECT) {
        const int outIndex = frame.v;
        const int value = frame.val1;
        const int digit = frame.val2;
        if (outIndex >= 0 && outIndex < static_cast<int>(m_dstBlocks.size())) {
            ArrayBlock* src = bucketSlot(digit, 0);
            QPointF from = src ? src->pos() : QPointF(200, 278);
            if (digit >= 0 && digit < static_cast<int>(m_bucketSlots.size())) {
                for (ArrayBlock* b : m_bucketSlots[digit]) {
                    if (b->showValue && b->customValue == QString::number(value)) {
                        from = b->pos();
                        break;
                    }
                }
            }
            playValueFlight(from, m_dstBlocks[outIndex], value, kRdOut, true);
            if (m_animated) playCellLanding(m_dstBlocks[outIndex], kRdOut, 380);
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("第 %1 轮收集：桶 %2 的 %3 → 第 %4 位")
                                     .arg(frame.u + 1).arg(digit).arg(value).arg(outIndex));
        }
    } else if (frame.type == ActionType::RD_DONE) {
        for (auto* b : m_dstBlocks) {
            if (b->showValue) { b->color = kRdOut; b->update(); }
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrD) m_ptrD->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, QString("基数排序完成，共 %1 轮").arg(frame.v));
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void RadixSortModule::reset() {
    m_frames.clear();
    m_data.clear();
    m_srcBlocks.clear();
    m_dstBlocks.clear();
    m_bucketHeads.clear();
    m_bucketSlots.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrI = nullptr;
    m_ptrD = nullptr;
    m_maxDigit = 0;
    m_valid = false;
}

void RadixSortModule::releaseSceneItems() {
    m_srcBlocks.clear();
    m_dstBlocks.clear();
    m_bucketHeads.clear();
    m_bucketSlots.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrI = nullptr;
    m_ptrD = nullptr;
}

void RadixSortModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}
