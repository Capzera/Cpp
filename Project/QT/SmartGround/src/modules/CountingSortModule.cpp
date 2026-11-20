#include "CountingSortModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kCsSrc = QColor(252, 252, 254);
static const QColor kCsScan = QColor(255, 210, 145);
static const QColor kCsCount = QColor(165, 215, 245);
static const QColor kCsPrefix = QColor(185, 220, 250);
static const QColor kCsOut = QColor(150, 220, 175);
static const QColor kCsPlace = QColor(255, 180, 150);

CountingSortModule::CountingSortModule() {}

QString CountingSortModule::getModuleName() const { return "计数排序"; }
QString CountingSortModule::getDefaultInput() const { return "8\n5 3 8 1 5 9 2 6"; }
QString CountingSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n <= 10）\n"
           "第二行输入 n 个非负整数（均不超过 20）\n"
           "算法分三步：统计每个数的出现次数 → 求前缀和 → 从后往前按桶的位置回填";
}

void CountingSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
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
                if (!ok || value < 0 || value > 20) {
                    m_valid = false;
                    break;
                }
                m_data.push_back(value);
            }
        }
    }

    m_mathText = createScriptText(scene, m_valid ? "计数排序：计数 → 前缀和 → 稳定回填"
                                        : "输入格式：n，随后输入 n 个 0~20 的整数；n≤10");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(160, 18);
    if (!m_valid) return;

    const int count = static_cast<int>(m_data.size());
    m_maxValue = *std::max_element(m_data.begin(), m_data.end());
    const int buckets = m_maxValue + 1;

    auto addRowLabel = [scene](const QString& text, double x, double y) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(UiFonts::cjk(11, true));
        item->setDefaultTextColor(QColor(60, 75, 95));
        item->setPos(x, y);
        return item;
    };

    m_labelA = addRowLabel("a[] 原数组", 30, 96);
    m_labelC = addRowLabel("cnt[] 计数", 30, 216);
    m_labelP = addRowLabel("cnt[] 前缀和", 30, 336);
    m_labelO = addRowLabel("out[] 输出", 30, 456);

    const double cell = 56.0;
    const double srcX = std::max(210.0, 380.0 - count * cell / 2.0);
    for (int i = 0; i < count; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i], QString::number(i));
        block->setCellSize(cell, 40);
        block->color = kCsSrc;
        block->setPos(srcX + i * cell, 115);
        scene->addItem(block);
        m_srcBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }

    const double bucketX = std::max(210.0, 380.0 - buckets * cell / 2.0);
    for (int i = 0; i < buckets; ++i) {
        ArrayBlock* cnt = new ArrayBlock(i, 0, QString::number(i));
        cnt->setCellSize(cell, 40);
        cnt->setCustomValue("0");
        cnt->setPos(bucketX + i * cell, 235);
        scene->addItem(cnt);
        m_countBlocks.push_back(cnt);

        ArrayBlock* pre = new ArrayBlock(i, 0, QString::number(i));
        pre->setCellSize(cell, 40);
        pre->setCustomValue("0");
        pre->setPos(bucketX + i * cell, 355);
        scene->addItem(pre);
        m_prefixBlocks.push_back(pre);
    }

    const double outX = std::max(210.0, 380.0 - count * cell / 2.0);
    for (int i = 0; i < count; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(cell, 40);
        block->showValue = false;
        block->setPos(outX + i * cell, 475);
        scene->addItem(block);
        m_outBlocks.push_back(block);
    }

    m_scanPointer = new PointerBlock("i", QColor(210, 80, 55));
    m_scanPointer->setOpacity(0);
    scene->addItem(m_scanPointer);
}

std::vector<AnimFrame> CountingSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    const int buckets = *std::max_element(m_data.begin(), m_data.end()) + 1;

    std::vector<int> cnt(buckets, 0);
    std::vector<int> prefix(buckets, 0);

    m_frames.push_back({ActionType::CS_RESET, 0, 0, 0, 0, {},
                        QString("初始化：统计每个值的出现次数"), QString("cnt[] = 0")});

    for (int i = 0; i < n; ++i) {
        const int value = m_data[i];
        ++cnt[value];
        m_frames.push_back({ActionType::CS_COUNT, i, value, cnt[value], 0, {},
                            QString("a[%1] = %2，cnt[%3] 加一变为 %4").arg(i).arg(value).arg(value).arg(cnt[value]),
                            QString("cnt[%1]++ = %2").arg(value).arg(cnt[value])});
    }

    for (int i = 1; i < buckets; ++i) {
        prefix[i] = prefix[i - 1] + cnt[i];
        m_frames.push_back({ActionType::CS_PREFIX, i, prefix[i - 1], prefix[i], cnt[i], {},
                            QString("cnt[%1] 与 prefix[%2] 合并成 %3，表示 ≤%4 的元素个数")
                                .arg(i).arg(i - 1).arg(prefix[i]).arg(i),
                            QString("prefix[%1] = prefix[%2] + cnt[%3] = %4 + %5 = %6")
                                .arg(i).arg(i - 1).arg(i).arg(prefix[i - 1]).arg(cnt[i]).arg(prefix[i])});
    }

    std::vector<int> out(n, 0);
    std::vector<int> pos = prefix;
    for (int i = n - 1; i >= 0; --i) {
        const int value = m_data[i];
        const int slot = --pos[value];
        out[slot] = value;
        m_frames.push_back({ActionType::CS_PLACE, i, value, slot, 0, {},
                            QString("a[%1] = %2 放入 out[%3]").arg(i).arg(value).arg(slot),
                            QString("out[--cnt[%1]] = %2").arg(value).arg(value)});
    }

    m_frames.push_back({ActionType::CS_DONE, n, 0, 0, 0, {},
                        QString("计数排序完成，输出序列已是升序"),
                        QString("计数排序复杂度 O(n + k)")});
    return m_frames;
}

void CountingSortModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::CS_RESET) {
        for (auto* b : m_srcBlocks) { b->color = kCsSrc; b->update(); }
        for (auto* b : m_countBlocks) { b->setCustomValue("0"); b->color = Qt::white; b->update(); }
        for (auto* b : m_prefixBlocks) { b->setCustomValue("0"); b->color = Qt::white; b->update(); }
        for (auto* b : m_outBlocks) { b->showValue = false; b->color = Qt::white; b->update(); }
        m_scanPointer->setOpacity(0);
    } else if (frame.type == ActionType::CS_COUNT) {
        for (auto* b : m_srcBlocks) {
            if (b->color == kCsScan) { b->color = kCsSrc; b->update(); }
        }
        const int index = frame.u;
        if (index >= 0 && index < static_cast<int>(m_srcBlocks.size())) {
            m_srcBlocks[index]->color = kCsScan;
            m_srcBlocks[index]->update();
            m_scanPointer->setOpacity(1);
            playPointerArrange({m_scanPointer}, {m_srcBlocks[index]}, 40);
        }
        const int value = frame.v;
        if (value >= 0 && value < static_cast<int>(m_countBlocks.size())) {
            m_countBlocks[value]->setCustomValue(QString::number(frame.val1));
            m_countBlocks[value]->color = kCsCount;
            m_countBlocks[value]->update();
            if (m_animated) {
                playScalePulse(m_countBlocks[value], 1.2, 240);
                playCellRipple(m_countBlocks[value], kCsCount, 1.0, 520);
            }
        }
    } else if (frame.type == ActionType::CS_PREFIX) {
        const int index = frame.u;
        if (index < 0 || index >= static_cast<int>(m_prefixBlocks.size())) return;
        const int prevSum = frame.v;
        const int cntValue = frame.val2;
        const int total = frame.val1;

        if (index - 1 >= 0) {
            m_prefixBlocks[index - 1]->color = QColor(225, 235, 248);
            m_prefixBlocks[index - 1]->update();
        }
        if (m_countBlocks.size() > static_cast<size_t>(index)) {
            m_countBlocks[index]->color = kCsCount;
            m_countBlocks[index]->update();
        }
        m_prefixBlocks[index]->color = kCsPrefix;
        m_prefixBlocks[index]->update();

        if (m_animated) {
            std::vector<QPointF> sources;
            std::vector<int> values;
            if (index - 1 >= 0 && index - 1 < static_cast<int>(m_prefixBlocks.size())) {
                sources.push_back(m_prefixBlocks[index - 1]->pos());
                values.push_back(prevSum);
            }
            if (index < static_cast<int>(m_countBlocks.size())) {
                sources.push_back(m_countBlocks[index]->pos());
                values.push_back(cntValue);
            }
            playMultiValueFlight(sources, values, m_prefixBlocks[index], total, kCsPrefix);
        } else {
            m_prefixBlocks[index]->setCustomValue(QString::number(total));
            m_prefixBlocks[index]->update();
        }
    } else if (frame.type == ActionType::CS_PLACE) {
        const int index = frame.u;
        const int value = frame.v;
        const int slot = frame.val1;
        if (index >= 0 && index < static_cast<int>(m_srcBlocks.size())) {
            m_srcBlocks[index]->color = kCsPlace;
            m_srcBlocks[index]->update();
        }
        if (value >= 0 && value < static_cast<int>(m_countBlocks.size())) {
            m_countBlocks[value]->setCustomValue(QString::number(frame.val2));
            m_countBlocks[value]->color = kCsPlace;
            m_countBlocks[value]->update();
        }
        if (slot >= 0 && slot < static_cast<int>(m_outBlocks.size())) {
            const QPointF from = (index >= 0 && index < static_cast<int>(m_srcBlocks.size()))
                ? m_srcBlocks[index]->pos() : QPointF(330, 115);
            playValueFlight(from, m_outBlocks[slot], value, kCsOut, true);
            playCellLanding(m_outBlocks[slot], kCsOut, 420);
        }
    } else if (frame.type == ActionType::CS_DONE) {
        for (auto* b : m_srcBlocks) { b->color = kCsSrc; b->update(); }
        for (auto* b : m_outBlocks) {
            if (b->showValue) { b->color = kCsOut; b->update(); playNodePop(b, 1.12, 240); }
        }
        m_scanPointer->setOpacity(0);
    }
}

void CountingSortModule::reset() {
    m_frames.clear();
    m_data.clear();
    m_srcBlocks.clear();
    m_countBlocks.clear();
    m_prefixBlocks.clear();
    m_outBlocks.clear();
    m_scanPointer = nullptr;
    m_mathText = nullptr;
    m_labelA = nullptr;
    m_labelC = nullptr;
    m_labelP = nullptr;
    m_labelO = nullptr;
    m_maxValue = 0;
    m_valid = false;
}

void CountingSortModule::releaseSceneItems() {
    m_srcBlocks.clear();
    m_countBlocks.clear();
    m_prefixBlocks.clear();
    m_outBlocks.clear();
    m_scanPointer = nullptr;
    m_mathText = nullptr;
    m_labelA = nullptr;
    m_labelC = nullptr;
    m_labelP = nullptr;
    m_labelO = nullptr;
}
