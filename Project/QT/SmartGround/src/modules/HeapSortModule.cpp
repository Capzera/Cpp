#include "HeapSortModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kHsCompare = QColor(255, 214, 140);
static const QColor kHsSwap = QColor(255, 170, 120);
static const QColor kHsSorted = QColor(140, 205, 150);
static const QColor kHsHeap = QColor(150, 185, 235);
static const QColor kHsPale = QColor(250, 250, 252);

HeapSortModule::HeapSortModule() {}

QString HeapSortModule::getModuleName() const { return "堆排序"; }
QString HeapSortModule::getDefaultInput() const { return "6\n10 5 8 2 9 3"; }

QString HeapSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示元素个数\n"
           "第二行输入 n 个整数，即待排序数组\n"
           "先自底向上建堆，再反复把堆顶与堆尾交换并缩小堆区间";
}

void HeapSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (tokens.isEmpty()) return;
    int n = tokens[0].toInt();
    if (n < 1 || n > 16) n = 0;
    if (n > static_cast<int>(tokens.size()) - 1) n = tokens.size() - 1;
    m_valid = n > 0;

    m_mathText = createScriptText(scene, m_valid ? "堆排序：自底向上建堆 → 反复取堆顶放到末尾"
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

std::vector<AnimFrame> HeapSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<int> heap = m_heap;
    int n = m_n;

    m_frames.push_back({ActionType::HS_RESET, 0, 0, 0, 0, {},
                        "第一步：自底向上建堆，对每个非叶节点执行 down()",
                        "for i = n/2 downto 1: down(i)"});

    for (int i = n / 2; i >= 1; --i) {
        m_frames.push_back({ActionType::HS_COMPARE, i, 0, heap[i], 0, {},
                            QString("对节点 %1 执行 down()").arg(i),
                            QString("down(%1)：调整以 %1 为根的子树").arg(i)});
        int node = i;
        int guard = 0;
        while (guard++ < 64) {
            int left = node * 2;
            int right = left + 1;
            int best = 0;
            if (left <= n && right <= n) {
                m_frames.push_back({ActionType::HS_COMPARE, node, left, heap[node], heap[left], {},
                                    QString("比较孩子 heap[%1]=%2 与 heap[%3]=%4").arg(left).arg(heap[left]).arg(right).arg(heap[right]),
                                    QString("heap[%1]=%2  vs  heap[%3]=%4").arg(left).arg(heap[left]).arg(right).arg(heap[right])});
                best = (heap[left] >= heap[right]) ? left : right;
            } else if (left <= n) {
                best = left;
            } else {
                break;
            }
            if (heap[best] > heap[node]) {
                m_frames.push_back({ActionType::HS_SWAP, node, best, heap[node], heap[best], {},
                                    QString("交换 heap[%1] 与 heap[%2]").arg(node).arg(best),
                                    QString("swap(heap[%1], heap[%2])").arg(node).arg(best)});
                std::swap(heap[node], heap[best]);
                node = best;
            } else {
                break;
            }
        }
    }
    m_frames.push_back({ActionType::HS_DONE, 0, 0, 0, 1, {},
                        QString("建堆完成，堆顶为最大值 %1").arg(heap[1]),
                        QString("建堆完成，堆顶 = %1").arg(heap[1])});

    int heapSize = n;
    for (int end = n; end >= 2; --end) {
        m_frames.push_back({ActionType::HS_COMPARE, 1, end, heap[1], heap[end], {},
                            QString("把堆顶 %1 与堆尾 heap[%2]=%3 交换").arg(heap[1]).arg(end).arg(heap[end]),
                            QString("swap(heap[1], heap[%2])　当前堆区间 [1, %2]").arg(end)});
        m_frames.push_back({ActionType::HS_SWAP, 1, end, heap[1], heap[end], {},
                            QString("交换堆顶与堆尾，堆顶归位到位置 %1").arg(end),
                            QString("swap(heap[1], heap[%2])").arg(end)});
        std::swap(heap[1], heap[end]);
        --heapSize;

        int node = 1;
        int guard = 0;
        while (guard++ < 64) {
            int left = node * 2;
            int right = left + 1;
            int best = 0;
            if (left <= heapSize && right <= heapSize) {
                m_frames.push_back({ActionType::HS_COMPARE, node, left, heap[node], heap[left], {},
                                    QString("缩小堆区间后重新比较孩子"),
                                    QString("heap[%1]=%2  vs  heap[%3]=%4").arg(left).arg(heap[left]).arg(right).arg(heap[right])});
                best = (heap[left] >= heap[right]) ? left : right;
            } else if (left <= heapSize) {
                best = left;
            } else {
                break;
            }
            if (heap[best] > heap[node]) {
                m_frames.push_back({ActionType::HS_SWAP, node, best, heap[node], heap[best], {},
                                    QString("下沉：交换 heap[%1] 与 heap[%2]").arg(node).arg(best),
                                    QString("swap(heap[%1], heap[%2])").arg(node).arg(best)});
                std::swap(heap[node], heap[best]);
                node = best;
            } else {
                break;
            }
        }
        m_frames.push_back({ActionType::HS_DONE, 0, 0, end, 2, {},
                            QString("位置 %1 已归位为 %2").arg(end).arg(heap[end]),
                            QString("heap[%1] = %2 已进入有序区").arg(end).arg(heap[end])});
    }
    m_frames.push_back({ActionType::HS_DONE, 0, 0, 0, 3, {},
                        QString("堆排序完成，最终结果为升序"),
                        QString("堆排序完成")});
    return m_frames;
}

void HeapSortModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::HS_RESET) {
        for (auto& kv : m_arrayBlocks) { kv.second->color = kHsPale; kv.second->update(); }
        for (auto& kv : m_treeNodes) {
            kv.second->color = kHsPale;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_treeEdges) { kv.second->color = QColor(170, 180, 195); kv.second->update(); }
        clearDescentTrail();
    } else if (frame.type == ActionType::HS_COMPARE) {
        auto paint = [&](int index, QColor c) {
            if (index <= 0) return;
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
        paint(frame.u, kHsCompare);
        if (frame.v > 0) paint(frame.v, kHsHeap);
        if (frame.u > 0) {
            GraphNode* node = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
            if (node) playNodeWake(node, kHsCompare, 30.0, 460);
        }
        if (frame.v > 0 && frame.v != frame.u) {
            auto edgeIt = m_treeEdges.find({std::min(frame.u, frame.v), std::max(frame.u, frame.v)});
            if (edgeIt != m_treeEdges.end()) playEdgeTravel(edgeIt->second, kHsCompare, 420);
        }
        if (frame.u > 0) appendDescent(frame.u, kHsCompare);
        int current = frame.u;
        int partner = frame.v;
        QString caption;
        if (partner > 0 && (partner == current * 2 || partner == current * 2 + 1)) {
            caption = QString("孩子关系：%1 的孩子是 %1*2 = %2 与 %1*2+1 = %3")
                          .arg(current).arg(current * 2).arg(current * 2 + 1);
        } else if (partner == current / 2 && partner > 0) {
            caption = QString("父节点关系：%1 的父亲是 %1/2 = %2").arg(current).arg(partner);
        } else {
            caption = QString("下标 %1　父 %2　孩子 %3 与 %4")
                          .arg(current).arg(current / 2).arg(current * 2).arg(current * 2 + 1);
        }
        highlightRelation(current, partner, kHsCompare, caption);
    } else if (frame.type == ActionType::HS_SWAP) {
        ArrayBlock* ba = m_arrayBlocks.count(frame.u) ? m_arrayBlocks[frame.u] : nullptr;
        ArrayBlock* bb = m_arrayBlocks.count(frame.v) ? m_arrayBlocks[frame.v] : nullptr;
        GraphNode* na = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        GraphNode* nb = m_treeNodes.count(frame.v) ? m_treeNodes[frame.v] : nullptr;
        if (ba && bb) {
            playSwapAnim(ba, bb, frame.val1, frame.val2);
            ba->color = kHsSwap;
            bb->color = kHsSwap;
            ba->update();
            bb->update();
        }
        if (na && nb) {
            playNodeSwapAnim(na, nb, frame.val1, frame.val2);
            auto edgeIt = m_treeEdges.find({std::min(frame.u, frame.v), std::max(frame.u, frame.v)});
            if (edgeIt != m_treeEdges.end()) playEdgeTravel(edgeIt->second, kHsSwap, 460);
        }
        if (frame.u > 0) appendDescent(frame.u, kHsSwap);
        swapHeapValues(frame.u, frame.v, frame.val1, frame.val2);
        if (na) { na->color = kHsSwap; na->setHighlightColor(kHsSwap); na->update(); }
        if (nb) { nb->color = kHsSwap; nb->setHighlightColor(kHsSwap); nb->update(); }
        if (m_relationText) {
            setScriptText(m_relationText, QString("swap(heap[%1], heap[%2])：值真正互换")
                                             .arg(frame.u).arg(frame.v));
            m_relationText->setDefaultTextColor(kHsSwap.darker(120));
        }
    } else if (frame.type == ActionType::HS_DONE) {
        if (frame.val2 == 1) {
            for (auto& kv : m_arrayBlocks) { kv.second->color = kHsHeap; kv.second->update(); }
            for (auto& kv : m_treeNodes) { kv.second->color = kHsHeap; kv.second->update(); }
        } else if (frame.val2 == 2) {
            ArrayBlock* b = m_arrayBlocks.count(frame.val1) ? m_arrayBlocks[frame.val1] : nullptr;
            if (b) { b->color = kHsSorted; b->update(); }
        } else if (frame.val2 == 3) {
            for (auto& kv : m_arrayBlocks) { kv.second->color = kHsSorted; kv.second->update(); }
            for (auto& kv : m_treeNodes) {
                kv.second->setHighlightColor(Qt::transparent);
                kv.second->color = kHsSorted;
                kv.second->update();
                playNodePop(kv.second, 1.14, 320);
            }
            freezeDescentTrail(kHsSorted);
        }
    }
}

void HeapSortModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "heap" && index >= 1 && index < static_cast<int>(m_heap.size())) m_heap[index] = value;
}

void HeapSortModule::reset() {
    m_frames.clear();
    m_heap.clear();
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
    m_n = 0;
    m_valid = false;
}

void HeapSortModule::releaseSceneItems() {
    m_arrayBlocks.clear();
    m_treeNodes.clear();
    m_treeEdges.clear();
    m_descentTrail = nullptr;
    m_descentPoints.clear();
    m_mathText = nullptr;
}
