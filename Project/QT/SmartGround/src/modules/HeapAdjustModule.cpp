#include "HeapAdjustModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kHgCompare = QColor(255, 214, 140);
static const QColor kHgSwap = QColor(255, 170, 120);
static const QColor kHgDone = QColor(140, 205, 150);
static const QColor kHgPale = QColor(250, 250, 252);
static const QColor kHgUp = QColor(150, 185, 235);
static const QColor kHgDown = QColor(250, 200, 150);

HeapAdjustModule::HeapAdjustModule() {}

QString HeapAdjustModule::getModuleName() const { return "堆调整 up/down"; }
QString HeapAdjustModule::getDefaultInput() const { return "5\n10 5 8 2 9"; }

QString HeapAdjustModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示堆中元素的个数\n"
           "第二行输入 n 个整数，即完全二叉树按层序展开后的数组存储\n"
           "模块先演示 up()：从指定位置向上与父节点比较并交换\n"
           "再演示 down()：从指定位置向下与孩子比较并交换";
}

void HeapAdjustModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (tokens.isEmpty()) return;
    int n = tokens[0].toInt();
    if (n < 1 || n > 16) n = 0;
    if (n > static_cast<int>(tokens.size()) - 1) n = tokens.size() - 1;
    m_valid = n > 0;

    m_mathText = scene->addText(m_valid ? "堆调整：up() 向上冒泡，down() 向下沉降"
                                         : "输入格式：n，随后 n 个整数；限制 n≤16");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    if (!m_valid) return;

    std::vector<int> heap(n + 1, 0);
    for (int i = 1; i <= n; ++i) heap[i] = tokens[i].toInt();
    layoutHeap(n, heap);
    syncTreeValues(heap);
}

std::vector<AnimFrame> HeapAdjustModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    m_frames.push_back({ActionType::HG_RESET, 0, 0, 0, 0, {},
                        "准备演示堆的调整过程",
                        "up(i)：与父节点 i/2 比较并交换　　down(i)：与孩子 2i、2i+1 比较并交换"});

    std::vector<int> heap = m_heap;

    int target = m_n;
    m_frames.push_back({ActionType::HG_COMPARE, target, target / 2, 0, 0, {},
                        QString("演示 up(%1)：从下标 %1 向上调整").arg(target),
                        QString("up(%1)：父节点为 %1/2 = %2").arg(target).arg(target / 2)});

    int i = target;
    int guard = 0;
    while (i > 1 && guard++ < 64) {
        int parent = i / 2;
        m_frames.push_back({ActionType::HG_COMPARE, i, parent, heap[i], heap[parent], {},
                            QString("比较 heap[%1]=%2 与父节点 heap[%3]=%4").arg(i).arg(heap[i]).arg(parent).arg(heap[parent]),
                            QString("heap[%1]=%2  vs  heap[%3]=%4").arg(i).arg(heap[i]).arg(parent).arg(heap[parent])});
        if (heap[i] > heap[parent]) {
            m_frames.push_back({ActionType::HG_SWAP, i, parent, heap[i], heap[parent], {},
                                QString("父节点更小，交换 heap[%1] 与 heap[%2]").arg(parent).arg(i),
                                QString("swap(heap[%1], heap[%2])").arg(parent).arg(i)});
            std::swap(heap[i], heap[parent]);
            i = parent;
            m_frames.push_back({ActionType::HG_COMPARE, i, i / 2, heap[i], 0, {},
                                QString("继续向上，%1 的父节点是 %2").arg(i).arg(i / 2),
                                QString("i = %1，继续检查父节点 %2").arg(i).arg(i / 2)});
        } else {
            m_frames.push_back({ActionType::HG_DONE, i, 0, heap[i], 0, {},
                                QString("heap[%1] 不大于父节点，up() 结束").arg(i),
                                QString("heap[%1] ≤ heap[%2]，停止上浮").arg(i).arg(parent)});
            break;
        }
    }
    if (guard >= 64) {
        m_frames.push_back({ActionType::HG_DONE, 1, 0, heap[1], 0, {},
                            QString("up() 已到达根节点"), QString("到达根节点，停止上浮")});
    }
    m_frames.push_back({ActionType::HG_DONE, 1, 0, heap[1], 1, {},
                        QString("up() 完成，堆顶为 %1").arg(heap[1]),
                        QString("up() 完成")});

    int downStart = 1;
    m_frames.push_back({ActionType::HG_COMPARE, downStart, 0, heap[downStart], 0, {},
                        QString("演示 down(%1)：从根节点向下调整").arg(downStart),
                        QString("down(%1)：孩子为 %2 与 %3").arg(downStart).arg(downStart * 2).arg(downStart * 2 + 1)});

    i = downStart;
    guard = 0;
    while (guard++ < 64) {
        int left = i * 2;
        int right = left + 1;
        int best = 0;
        if (left <= m_n && right <= m_n) {
            m_frames.push_back({ActionType::HG_COMPARE, i, left, heap[i], heap[left], {},
                                QString("比较左右孩子 heap[%1]=%2 与 heap[%3]=%4").arg(left).arg(heap[left]).arg(right).arg(heap[right]),
                                QString("heap[%1]=%2  vs  heap[%3]=%4").arg(left).arg(heap[left]).arg(right).arg(heap[right])});
            best = (heap[left] >= heap[right]) ? left : right;
        } else if (left <= m_n) {
            best = left;
        } else {
            break;
        }
        if (heap[best] > heap[i]) {
            m_frames.push_back({ActionType::HG_SWAP, i, best, heap[i], heap[best], {},
                                QString("孩子 heap[%1] 更大，交换并下沉").arg(best),
                                QString("swap(heap[%1], heap[%2])").arg(i).arg(best)});
            std::swap(heap[i], heap[best]);
            i = best;
            m_frames.push_back({ActionType::HG_COMPARE, i, 0, heap[i], 0, {},
                                QString("下沉到位置 %1，继续检查孩子").arg(i),
                                QString("i = %1，继续检查孩子").arg(i)});
        } else {
            m_frames.push_back({ActionType::HG_DONE, i, 0, heap[i], 0, {},
                                QString("heap[%1] 已不小于孩子，down() 结束").arg(i),
                                QString("heap[%1] ≥ heap[%2]，停止下沉").arg(i).arg(best)});
            break;
        }
    }
    m_frames.push_back({ActionType::HG_DONE, 1, 0, heap[1], 2, {},
                        QString("down() 完成，堆顶为最大值 %1").arg(heap[1]),
                        QString("down() 完成，堆顶 = %1").arg(heap[1])});
    return m_frames;
}

void HeapAdjustModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::HG_RESET) {
        for (auto& kv : m_arrayBlocks) { kv.second->color = kHgPale; kv.second->update(); }
        for (auto& kv : m_treeNodes) {
            kv.second->color = kHgPale;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_treeEdges) { kv.second->color = QColor(170, 180, 195); kv.second->update(); }
    } else if (frame.type == ActionType::HG_COMPARE) {
        auto paint = [&](int index, QColor c) {
            auto it = m_arrayBlocks.find(index);
            if (it != m_arrayBlocks.end()) { it->second->color = c; it->second->update(); }
            auto tn = m_treeNodes.find(index);
            if (tn != m_treeNodes.end()) {
                tn->second->color = c;
                tn->second->setHighlightColor(c);
                tn->second->update();
                playScalePulse(tn->second, 1.2, 240);
            }
        };
        paint(frame.u, kHgCompare);
        if (frame.v > 0) paint(frame.v, kHgUp);
        auto edge = m_treeEdges.find({frame.v, frame.u});
        if (edge != m_treeEdges.end() && frame.v > 0) {
            edge->second->color = kHgUp;
            edge->second->update();
        }
        int current = frame.u;
        int partner = frame.v;
        QString caption;
        if (partner == current / 2 && partner > 0) {
            caption = QString("父节点关系：%1 的父亲是 %1/2 = %2").arg(current).arg(partner);
        } else if (partner > 0 && (partner == current * 2 || partner == current * 2 + 1)) {
            caption = QString("孩子关系：%1 的孩子是 %1*2 = %2 与 %1*2+1 = %3")
                          .arg(current).arg(current * 2).arg(current * 2 + 1);
        } else {
            caption = QString("下标 %1　父 %2　孩子 %3 与 %4")
                          .arg(current).arg(current / 2).arg(current * 2).arg(current * 2 + 1);
        }
        highlightRelation(current, partner, kHgCompare, caption);
    } else if (frame.type == ActionType::HG_SWAP) {
        int a = frame.u;
        int b = frame.v;
        ArrayBlock* ba = m_arrayBlocks.count(a) ? m_arrayBlocks[a] : nullptr;
        ArrayBlock* bb = m_arrayBlocks.count(b) ? m_arrayBlocks[b] : nullptr;
        if (ba && bb) {
            playSwapAnim(ba, bb, frame.val1, frame.val2);
            ba->color = kHgSwap;
            bb->color = kHgSwap;
            ba->update();
            bb->update();
        }
        swapHeapValues(a, b, frame.val1, frame.val2);
        GraphNode* na = m_treeNodes.count(a) ? m_treeNodes[a] : nullptr;
        GraphNode* nb = m_treeNodes.count(b) ? m_treeNodes[b] : nullptr;
        if (na) { na->color = kHgSwap; na->setHighlightColor(kHgSwap); na->update(); }
        if (nb) { nb->color = kHgSwap; nb->setHighlightColor(kHgSwap); nb->update(); }
        auto edge = m_treeEdges.find({b, a});
        if (edge != m_treeEdges.end()) {
            edge->second->color = kHgSwap;
            edge->second->setPenWidth(4.0);
            edge->second->update();
        }
        if (m_relationText) {
            m_relationText->setPlainText(QString("swap(heap[%1], heap[%2])：值真正互换").arg(b).arg(a));
            m_relationText->setDefaultTextColor(kHgSwap.darker(120));
        }
    } else if (frame.type == ActionType::HG_DONE) {
        for (auto& kv : m_arrayBlocks) {
            if (kv.second->color != kHgSwap) { kv.second->color = kHgDone; }
            kv.second->update();
        }
        for (auto& kv : m_treeNodes) {
            kv.second->setHighlightColor(Qt::transparent);
            if (kv.second->color != kHgSwap) kv.second->color = kHgDone;
            kv.second->update();
        }
    }
}

void HeapAdjustModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "heap" && index >= 1 && index < static_cast<int>(m_heap.size())) m_heap[index] = value;
}

void HeapAdjustModule::reset() {
    m_frames.clear();
    m_heap.clear();
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
    m_n = 0;
    m_valid = false;
}

void HeapAdjustModule::releaseSceneItems() {
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
}
