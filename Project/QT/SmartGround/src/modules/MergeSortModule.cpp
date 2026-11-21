#include "MergeSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QPointer>
#include <algorithm>
#include <set>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kMsIdle = QColor(252, 252, 254);
static const QColor kMsLeft = QColor(198, 218, 248);
static const QColor kMsRight = QColor(224, 210, 244);
static const QColor kMsSorted = QColor(178, 222, 190);
static const QColor kMsPick = QColor(255, 214, 138);
static const QColor kMsOutput = QColor(158, 200, 240);
static const QColor kMsDone = QColor(150, 220, 175);
static const QColor kMsConsumed = QColor(238, 240, 245);
static const QColor kMsTempEmpty = QColor(248, 249, 252);

static const double kMsCellW = 56.0;
static const double kMsCellH = 40.0;
static const double kMsGap = 20.0;
static const double kMsMainY = 165.0;
static const double kMsTempY = 350.0;

MergeSortModule::MergeSortModule() {}

QString MergeSortModule::getModuleName() const {
    return "归并排序";
}

QString MergeSortModule::getDefaultInput() const {
    return "8\n3 5 1 9 2 7 4 6";
}

QString MergeSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组\n"
           "归并排序先把区间一分为二（数组真的裂开成两段），\n"
           "再把两段有序序列合并回一段（格子拼回一起）";
}

double MergeSortModule::cellX(int index) const {
    double x = m_startX + index * kMsCellW;
    for (int boundary : m_openBounds) {
        if (boundary < index) x += kMsGap;
    }
    return x;
}

void MergeSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) {
        m_data.push_back(items[i].toInt());
    }
    n = m_data.size();
    m_n = n;
    if (n <= 0) return;

    m_startX = static_cast<int>(400 - (n * kMsCellW) / 2);
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        block->setCellSize(kMsCellW, kMsCellH);
        m_scene->addItem(block);
        block->setPos(m_startX + i * kMsCellW, kMsMainY);
        block->color = kMsIdle;
        m_blocks[i] = block;
        registerArraySlot(block, "a", i);
    }
    for (int i = 0; i < n; ++i) {
        ArrayBlock* auxBlock = new ArrayBlock(i, 0);
        auxBlock->setCellSize(kMsCellW, kMsCellH);
        auxBlock->setShowLabel(false);
        auxBlock->showValue = false;
        m_scene->addItem(auxBlock);
        auxBlock->setPos(m_startX + i * kMsCellW, kMsTempY);
        auxBlock->color = kMsTempEmpty;
        m_auxBlocks[i] = auxBlock;
    }

    m_mathText = createScriptText(m_scene, "");
    m_mathText->setPos(60, 30);
    m_mathText->setFont(UiFonts::mono(14, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));

    m_levelText = createScriptText(m_scene, "");
    m_levelText->setPos(60, 74);
    m_levelText->setFont(UiFonts::cjk(12, true));
    m_levelText->setDefaultTextColor(QColor(90, 105, 125));

    m_mergeText = createScriptText(m_scene, "");
    m_mergeText->setPos(60, 620);
    m_mergeText->setFont(UiFonts::cjk(13, true));
    m_mergeText->setDefaultTextColor(QColor(60, 80, 110));

    QGraphicsTextItem* auxTitle = scene->addText("temp[] 合并输出缓冲区");
    auxTitle->setFont(UiFonts::cjk(11, true));
    auxTitle->setDefaultTextColor(QColor(120, 130, 145));
    auxTitle->setPos(m_startX, kMsTempY - 34);
}

void MergeSortModule::resetRowColors() {
    for (auto& kv : m_blocks) {
        kv.second->color = kMsIdle;
        kv.second->setScale(1.0);
        kv.second->update();
    }
    for (auto& kv : m_auxBlocks) {
        kv.second->color = kMsTempEmpty;
        kv.second->showValue = false;
        kv.second->setScale(1.0);
        kv.second->update();
    }
}

void MergeSortModule::markSegment(int l, int r, const QColor& color) {
    for (int i = l; i <= r; ++i) {
        auto it = m_blocks.find(i);
        if (it == m_blocks.end()) continue;
        it->second->color = color;
        it->second->update();
    }
}

void MergeSortModule::applyLayout(int duration) {
    std::vector<QGraphicsObject*> items;
    std::vector<QPointF> targets;
    for (int i = 0; i < m_n; ++i) {
        auto it = m_blocks.find(i);
        if (it == m_blocks.end()) continue;
        items.push_back(it->second);
        targets.push_back(QPointF(cellX(i), kMsMainY));
    }
    for (int i = 0; i < m_n; ++i) {
        auto it = m_auxBlocks.find(i);
        if (it == m_auxBlocks.end()) continue;
        items.push_back(it->second);
        targets.push_back(QPointF(cellX(i), kMsTempY));
    }
    playMoveTo(items, targets, duration);
}

std::vector<AnimFrame> MergeSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    if (n <= 0) return m_frames;
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;
    mergeSort(arr, 0, n - 1, 0);
    m_frames.push_back({ActionType::MS_DONE, 0, n - 1, 0, 0, {},
                        QString("归并排序完成"),
                        QString("log n 层递归，每层合并总代价 O(n)")});
    return m_frames;
}

void MergeSortModule::mergeSort(std::vector<int>& arr, int l, int r, int depth) {
    if (l >= r) {
        m_frames.push_back({ActionType::MS_REC, l, r, 0, depth, {},
                            QString("区间 [%1, %2] 只剩一个元素，已天然有序").arg(l + 1).arg(r + 1),
                            QString("单元素区间天然有序")});
        return;
    }
    int mid = (l + r) / 2;
    m_frames.push_back({ActionType::MS_SPLIT, l, r, mid, depth, {},
                        QString("把区间 [%1, %2] 从中点 %3 一分为二，数组裂开").arg(l + 1).arg(r + 1).arg(mid + 1),
                        QString("mid = (%1+%2)/2 = %3").arg(l + 1).arg(r + 1).arg(mid + 1)});
    mergeSort(arr, l, mid, depth + 1);
    mergeSort(arr, mid + 1, r, depth + 1);
    merge(arr, l, mid, r);
    m_frames.push_back({ActionType::MS_REC, l, r, 1, depth, {},
                        QString("区间 [%1, %2] 合并完成，整段已有序").arg(l + 1).arg(r + 1),
                        QString("mergeSort(%1, %2) 返回").arg(l + 1).arg(r + 1)});
}

void MergeSortModule::merge(std::vector<int>& arr, int l, int mid, int r) {
    m_frames.push_back({ActionType::MS_MERGE_OPEN, l, mid, r, 0, {},
                        QString("准备合并两段有序序列 [%1, %2] 与 [%3, %4]").arg(l + 1).arg(mid + 1).arg(mid + 2).arg(r + 1),
                        QString("两段各自有序，逐个取较小者")});
    std::vector<int> left(arr.begin() + l, arr.begin() + mid + 1);
    std::vector<int> right(arr.begin() + mid + 1, arr.begin() + r + 1);
    std::vector<int> merged;
    int i = 0, j = 0, k = l;
    while (i < static_cast<int>(left.size()) && j < static_cast<int>(right.size())) {
        const int leftIndex = l + i;
        const int rightIndex = mid + 1 + j;
        m_frames.push_back({ActionType::MS_CMP, leftIndex, rightIndex, left[i], right[j], {},
                            QString("比较两段队首：左段 %1 与右段 %2").arg(left[i]).arg(right[j]),
                            QString("a[%1]=%2　对比　a[%3]=%4").arg(leftIndex + 1).arg(left[i]).arg(rightIndex + 1).arg(right[j])});
        if (left[i] <= right[j]) {
            m_frames.push_back({ActionType::MS_PICK, leftIndex, k, left[i], 0, {0},
                                QString("左段的 %1 更小，取出放入 temp[%2]").arg(left[i]).arg(k + 1),
                                QString("temp[%1] = %2").arg(k + 1).arg(left[i])});
            merged.push_back(left[i]);
            arr[k++] = left[i++];
        } else {
            m_frames.push_back({ActionType::MS_PICK, rightIndex, k, right[j], 1, {1},
                                QString("右段的 %1 更小，取出放入 temp[%2]").arg(right[j]).arg(k + 1),
                                QString("temp[%1] = %2").arg(k + 1).arg(right[j])});
            merged.push_back(right[j]);
            arr[k++] = right[j++];
        }
    }
    while (i < static_cast<int>(left.size())) {
        m_frames.push_back({ActionType::MS_APPEND, l + i, k, left[i], 0, {0},
                            QString("左段已取完，剩余的 %1 直接搬入 temp[%2]").arg(left[i]).arg(k + 1),
                            QString("temp[%1] = %2").arg(k + 1).arg(left[i])});
        merged.push_back(left[i]);
        arr[k++] = left[i++];
    }
    while (j < static_cast<int>(right.size())) {
        m_frames.push_back({ActionType::MS_APPEND, mid + 1 + j, k, right[j], 1, {1},
                            QString("右段已取完，剩余的 %1 直接搬入 temp[%2]").arg(right[j]).arg(k + 1),
                            QString("temp[%1] = %2").arg(k + 1).arg(right[j])});
        merged.push_back(right[j]);
        arr[k++] = right[j++];
    }
    m_frames.push_back({ActionType::MS_COPY, l, r, mid, 0, merged,
                        QString("把 temp[%1..%2] 整体写回原数组，两段拼回一起").arg(l + 1).arg(r + 1),
                        QString("temp 覆盖原区间，合并结果落位")});
}

void MergeSortModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (m_mergeText) {
        setScriptText(m_mergeText, frame.explanation);
        m_mergeText->setDefaultTextColor(QColor(60, 80, 110));
    }

    if (frame.type == ActionType::MS_SPLIT) {
        const int l = frame.u, r = frame.v, mid = frame.val1;
        const int depth = frame.val2;
        m_openBounds.insert(mid);
        resetRowColors();
        markSegment(l, mid, kMsLeft);
        markSegment(mid + 1, r, kMsRight);
        applyLayout(420);
        if (m_levelText) {
            setScriptText(m_levelText, QString("第 %1 层：区间 [%2, %3] 裂成 [%4, %5] 与 [%6, %7]")
                               .arg(depth + 1).arg(l + 1).arg(r + 1).arg(l + 1).arg(mid + 1).arg(mid + 2).arg(r + 1));
        }
        std::vector<ArrayBlock*> cells;
        for (int i = l; i <= r; ++i) {
            auto it = m_blocks.find(i);
            if (it != m_blocks.end()) cells.push_back(it->second);
        }
        if (!cells.empty()) playCellSweep(cells, kMsLeft, 45, 260);
    } else if (frame.type == ActionType::MS_REC) {
        const int l = frame.u, r = frame.v;
        const bool baseCase = (frame.val1 == 0);
        markSegment(l, r, baseCase ? kMsSorted : kMsDone);
        std::vector<ArrayBlock*> cells;
        for (int i = l; i <= r; ++i) {
            auto it = m_blocks.find(i);
            if (it != m_blocks.end()) cells.push_back(it->second);
        }
        if (cells.size() == 1) playCellLanding(cells[0], baseCase ? kMsSorted : kMsDone, 380);
        else if (cells.size() > 1) playCellSweep(cells, baseCase ? kMsSorted : kMsDone, 45, 260);
    } else if (frame.type == ActionType::MS_MERGE_OPEN) {
        const int l = frame.u, mid = frame.v, r = frame.val1;
        markSegment(l, mid, kMsLeft);
        markSegment(mid + 1, r, kMsRight);
        for (int i = l; i <= r; ++i) {
            auto it = m_auxBlocks.find(i);
            if (it == m_auxBlocks.end()) continue;
            it->second->color = QColor(240, 246, 255);
            it->second->showValue = false;
            it->second->update();
        }
        if (m_levelText) {
            setScriptText(m_levelText, QString("合并阶段：把 [%1, %2] 与 [%3, %4] 合成一段有序序列")
                               .arg(l + 1).arg(mid + 1).arg(mid + 2).arg(r + 1));
        }
    } else if (frame.type == ActionType::MS_CMP) {
        auto left = m_blocks.find(frame.u);
        auto right = m_blocks.find(frame.v);
        if (left != m_blocks.end()) {
            left->second->color = kMsPick;
            left->second->update();
            playNodePop(left->second, 1.14, 240);
            playCellRipple(left->second, QColor(255, 190, 90), 0.85, 460);
        }
        if (right != m_blocks.end()) {
            right->second->color = kMsPick;
            right->second->update();
            playNodePop(right->second, 1.14, 240);
            playCellRipple(right->second, QColor(255, 190, 90), 0.85, 460);
        }
    } else if (frame.type == ActionType::MS_PICK || frame.type == ActionType::MS_APPEND) {
        const int srcIndex = frame.u;
        const int dstIndex = frame.v;
        const int value = frame.val1;
        auto src = m_blocks.find(srcIndex);
        auto dst = m_auxBlocks.find(dstIndex);
        if (src != m_blocks.end()) {
            src->second->color = kMsConsumed;
            src->second->update();
        }
        if (dst != m_auxBlocks.end()) {
            const QPointF from = src != m_blocks.end() ? src->second->centerPos() : QPointF();
            playValueFlight(from, dst->second, value, kMsOutput, true);
            dst->second->color = kMsOutput;
            dst->second->update();
        }
    } else if (frame.type == ActionType::MS_COPY) {
        const int l = frame.u, r = frame.v;
        const int mid = frame.val1;
        m_openBounds.erase(mid);
        std::vector<ArrayBlock*> cells;
        for (int i = l; i <= r; ++i) {
            auto aux = m_auxBlocks.find(i);
            auto dst = m_blocks.find(i);
            if (aux == m_auxBlocks.end() || dst == m_blocks.end()) continue;
            const int offset = i - l;
            const int value = offset < static_cast<int>(frame.group.size())
                                  ? frame.group[offset]
                                  : (aux->second->showValue ? aux->second->value : 0);
            dst->second->value = value;
            dst->second->customValue.clear();
            dst->second->showValue = true;
            dst->second->color = kMsDone;
            dst->second->update();
            aux->second->color = kMsTempEmpty;
            aux->second->showValue = false;
            aux->second->update();
            cells.push_back(dst->second);
        }
        applyLayout(460);
        if (cells.size() == 1) playCellLanding(cells[0], kMsDone, 400);
        else if (cells.size() > 1) playCellSweep(cells, kMsDone, 45, 280);
    } else if (frame.type == ActionType::MS_DONE) {
        for (auto& kv : m_blocks) {
            kv.second->color = kMsDone;
            kv.second->update();
        }
        for (auto& kv : m_auxBlocks) {
            kv.second->color = kMsTempEmpty;
            kv.second->showValue = false;
            kv.second->update();
        }
        if (m_mergeText) {
            setScriptText(m_mergeText, "归并排序完成：log n 层递归，每层合并总代价 O(n)");
            m_mergeText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void MergeSortModule::reset() {
    m_data.clear();
    m_blocks.clear();
    m_auxBlocks.clear();
    m_frames.clear();
    m_openBounds.clear();
    m_mathText = nullptr;
    m_mergeText = nullptr;
    m_levelText = nullptr;
    m_startX = 0;
    m_n = 0;
}

void MergeSortModule::releaseSceneItems() {
    m_blocks.clear();
    m_auxBlocks.clear();
    m_mathText = nullptr;
    m_mergeText = nullptr;
    m_levelText = nullptr;
}
