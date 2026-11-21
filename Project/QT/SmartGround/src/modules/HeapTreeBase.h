#ifndef HEAPTREEBASE_H
#define HEAPTREEBASE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <QFont>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QVector>

class HeapTreeBase : public IModule {
public:
    int heapDepth(int n) const {
        int d = 0;
        while ((1 << (d + 1)) <= n) ++d;
        return d;
    }

    void layoutHeap(int n, const std::vector<int>& heap) {
        m_heap.clear();
        m_heap = heap;
        m_arrayBlocks.clear();
        m_treeNodes.clear();
        m_treeEdges.clear();
        m_n = n;

        const double cellW = 62.0;
        const double arrY = 560.0;
        const double treeTop = 200.0;
        const double levelGap = 108.0;
        const double halfSpan = 210.0;
        const double centerX = 400.0;

        for (int i = 1; i <= n; ++i) {
            ArrayBlock* block = new ArrayBlock(i, heap[i], QString::number(i));
            block->setCellSize(cellW, 40.0);
            block->setPos(centerX - (n * cellW) / 2.0 + (i - 1) * cellW, arrY);
            block->color = QColor(250, 250, 252);
            m_scene->addItem(block);
            m_arrayBlocks[i] = block;
            registerArraySlot(block, "heap", i);
        }
        QGraphicsTextItem* arrTitle = m_scene->addText("heap[]（数组存储）");
        arrTitle->setFont(QFont("Consolas", 12, QFont::Bold));
        arrTitle->setDefaultTextColor(QColor(40, 70, 110));
        arrTitle->setPos(centerX - (n * cellW) / 2.0, arrY + 48);

        int depth = heapDepth(n);
        for (int i = 1; i <= n; ++i) {
            int d = 0;
            while ((1 << (d + 1)) <= i) ++d;
            int levelStart = 1 << d;
            int levelCount = std::min(1 << d, n - levelStart + 1);
            int posInLevel = i - levelStart;
            double span = halfSpan * std::pow(2.0, d) / std::pow(2.0, depth);
            double x = centerX + (posInLevel - (levelCount - 1) / 2.0) * (span * 2.0 / std::max(1, levelCount));
            double y = treeTop + d * levelGap;

            GraphNode* node = new GraphNode(i);
            node->setPos(x, y);
            node->color = QColor(250, 250, 252);
            node->setCustomValueText(QString::number(heap[i]));
            node->setSubLabel(QString::number(i));
            m_scene->addItem(node);
            m_treeNodes[i] = node;

            int parent = i / 2;
            if (parent >= 1) {
                GraphEdge* edge = new GraphEdge(m_treeNodes[parent], node, false, false);
                edge->color = QColor(170, 180, 195);
                m_scene->addItem(edge);
                m_treeNodes[parent]->addEdge(edge);
                node->addEdge(edge);
                m_treeEdges[{parent, i}] = edge;
            }
        }

        m_guideLines.clear();
        for (int i = 1; i <= n; ++i) {
            auto nodeIt = m_treeNodes.find(i);
            auto blockIt = m_arrayBlocks.find(i);
            if (nodeIt == m_treeNodes.end() || blockIt == m_arrayBlocks.end()) continue;
            QGraphicsLineItem* guide = m_scene->addLine(
                QLineF(QPointF(nodeIt->second->pos().x(), nodeIt->second->pos().y() + 26.0),
                       QPointF(blockIt->second->pos().x() + blockIt->second->cellWidth() / 2.0,
                               blockIt->second->pos().y() - 4.0)));
            guide->setPen(QPen(QColor(212, 218, 228), 1.0));
            guide->setZValue(-1);
            m_guideLines.push_back(guide);
        }

        QGraphicsTextItem* treeTitle = m_scene->addText("完全二叉树结构（heap[i] 的孩子是 heap[2i] 与 heap[2i+1]）");
        treeTitle->setFont(QFont("Microsoft YaHei UI", 11, QFont::Bold));
        treeTitle->setDefaultTextColor(QColor(120, 130, 145));
        treeTitle->setPos(centerX - 320, treeTop - 96);

        m_formulaText = m_scene->addText("左孩子 = 2k　右孩子 = 2k+1　父 = ⌊k/2⌋");
        m_formulaText->setFont(QFont("Consolas", 13, QFont::Bold));
        m_formulaText->setDefaultTextColor(QColor(40, 100, 150));
        m_formulaText->setPos(660.0, treeTop - 34);

        m_relationText = m_scene->addText("");
        m_relationText->setFont(QFont("Consolas", 12, QFont::Bold));
        m_relationText->setDefaultTextColor(QColor(190, 110, 40));
        m_relationText->setPos(centerX - 220, arrY + 92);
    }

    void syncTreeValues(const std::vector<int>& heap) {
        for (size_t i = 1; i < heap.size(); ++i) {
            auto it = m_treeNodes.find(static_cast<int>(i));
            if (it == m_treeNodes.end()) continue;
            it->second->setCustomValueText(QString::number(heap[i]));
            it->second->update();
        }
    }

    void applyHeapValues(const std::vector<int>& heap) {
        for (size_t i = 1; i < heap.size(); ++i) {
            auto ab = m_arrayBlocks.find(static_cast<int>(i));
            if (ab != m_arrayBlocks.end()) {
                ab->second->value = heap[i];
                ab->second->customValue.clear();
                ab->second->showValue = true;
                ab->second->update();
            }
            auto tn = m_treeNodes.find(static_cast<int>(i));
            if (tn != m_treeNodes.end()) {
                tn->second->setCustomValueText(QString::number(heap[i]));
                tn->second->update();
            }
        }
    }

    void swapHeapValues(int a, int b, int valueA, int valueB) {
        auto abA = m_arrayBlocks.find(a);
        auto abB = m_arrayBlocks.find(b);
        if (abA != m_arrayBlocks.end()) {
            abA->second->value = valueB;
            abA->second->customValue.clear();
            abA->second->showValue = true;
            abA->second->update();
        }
        if (abB != m_arrayBlocks.end()) {
            abB->second->value = valueA;
            abB->second->customValue.clear();
            abB->second->showValue = true;
            abB->second->update();
        }
        auto tnA = m_treeNodes.find(a);
        auto tnB = m_treeNodes.find(b);
        if (tnA != m_treeNodes.end()) {
            tnA->second->setCustomValueText(QString::number(valueB));
            tnA->second->update();
        }
        if (tnB != m_treeNodes.end()) {
            tnB->second->setCustomValueText(QString::number(valueA));
            tnB->second->update();
        }
    }

    void highlightRelation(int current, int partner, const QColor& color, const QString& caption) {
        for (auto& kv : m_arrayBlocks) {
            kv.second->color = QColor(250, 250, 252);
            kv.second->update();
        }
        for (auto& kv : m_treeNodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        auto mark = [&](int index, const QColor& c) {
            if (index <= 0) return;
            auto ab = m_arrayBlocks.find(index);
            if (ab != m_arrayBlocks.end()) {
                ab->second->color = c;
                ab->second->update();
            }
            auto tn = m_treeNodes.find(index);
            if (tn != m_treeNodes.end()) {
                tn->second->color = c;
                tn->second->setHighlightColor(c.darker(112));
                tn->second->update();
            }
        };
        mark(current, color);
        mark(partner, color);
        int parent = current / 2;
        mark(parent, QColor(215, 228, 245));
        mark(current * 2, QColor(215, 228, 245));
        mark(current * 2 + 1, QColor(215, 228, 245));
        if (m_relationText) {
            m_relationText->setPlainText(caption);
            m_relationText->setDefaultTextColor(color.darker(120));
        }
    }

    void ensureDescentTrail(QColor color) {
        if (m_descentTrail) return;
        m_descentTrail = new PathTrail(color);
        m_descentTrail->setTrailWidth(4.5);
        m_descentTrail->setHeadRadius(7.0);
        m_scene->addItem(m_descentTrail);
        m_descentPoints.clear();
    }

    void clearDescentTrail() {
        if (!m_descentTrail) return;
        m_descentTrail->clearPoints();
        m_descentTrail->setProgress(0.0);
        m_descentPoints.clear();
    }

    void appendDescent(int nodeIndex, QColor color) {
        if (nodeIndex <= 0) return;
        auto it = m_treeNodes.find(nodeIndex);
        if (it == m_treeNodes.end()) return;
        ensureDescentTrail(color);
        m_descentPoints.append(it->second->pos());
        if (m_descentPoints.size() >= 2) {
            playPathTrail(m_descentTrail, m_descentPoints, color, 420);
        } else {
            m_descentTrail->setPoints(m_descentPoints);
            m_descentTrail->setColor(color);
            m_descentTrail->setProgress(1.0);
        }
    }

    void freezeDescentTrail(QColor color) {
        if (!m_descentTrail) return;
        m_descentTrail->setColor(color);
        m_descentTrail->setProgress(1.0);
    }

protected:
    QPointer<PathTrail> m_descentTrail;
    QVector<QPointF> m_descentPoints;
    std::vector<int> m_heap;
    std::map<int, ArrayBlock*> m_arrayBlocks;
    std::map<int, GraphNode*> m_treeNodes;
    std::map<std::pair<int, int>, GraphEdge*> m_treeEdges;
    std::vector<QGraphicsLineItem*> m_guideLines;
    QGraphicsTextItem* m_relationText = nullptr;
    QGraphicsTextItem* m_formulaText = nullptr;
    int m_n = 0;
};

#endif
