#include "DSUPathCompressionModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kDpPale = QColor(252, 252, 254);
static const QColor kDpPath = QColor(246, 176, 120);
static const QColor kDpRoot = QColor(150, 214, 170);
static const QColor kDpFaint = QColor(214, 218, 226);
static const QColor kDpGold = QColor(255, 214, 140);

DSUPathCompressionModule::DSUPathCompressionModule() {}

QString DSUPathCompressionModule::getModuleName() const { return "并查集 路径压缩"; }
QString DSUPathCompressionModule::getDefaultInput() const { return "8\n0 1 2 0 4 0 5 6"; }

QString DSUPathCompressionModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示集合中元素的个数\n"
           "接下来 n 个整数 p[i]，表示元素 i 的父节点编号（从 0 开始）\n"
           "p[i] = i 表示该元素自成一个集合";
}

std::map<int, QPointF> DSUPathCompressionModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void DSUPathCompressionModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void DSUPathCompressionModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    if (n < 2) n = 0;
    if (n > 12) n = 12;
    if (n > static_cast<int>(items.size()) - 1) n = items.size() - 1;
    if (n <= 0) return;
    m_valid = true;
    m_n = n;

    std::vector<int> parent(n);
    for (int i = 1; i <= n; ++i) {
        int p = items[i].toInt();
        if (p < 0 || p >= n) p = i - 1;
        parent[i - 1] = p;
    }
    m_parents = parent;

    GraphRenderHelper::buildNodes(scene, m_nodes, n, QPointF(400, 250), n <= 6 ? 150 : 175, this);
    for (int i = 0; i < n; ++i) {
        GraphNode* node = m_nodes[i + 1];
        if (!node) continue;
        node->color = kDpPale;
        node->setCustomValueText(QString::number(i + 1));
        node->setSubLabel("pa=" + QString::number(parent[i] + 1));
        node->update();
    }
    for (int i = 0; i < n; ++i) {
        if (parent[i] == i) continue;
        GraphEdge* edge = new GraphEdge(m_nodes[i + 1], m_nodes[parent[i] + 1], true);
        edge->color = QColor(180, 188, 202);
        scene->addItem(edge);
        m_nodes[i + 1]->addEdge(edge);
        m_nodes[parent[i] + 1]->addEdge(edge);
        m_edges[{i + 1, parent[i] + 1}] = edge;
    }

    m_mathText = scene->addText("并查集：find 沿父指针上行，回溯时一次性把路径压扁到根");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);

    GraphRenderHelper::addLabel(scene, "pa[]", QPointF(660, 430));
    GraphRenderHelper::addLabel(scene, "sz[]", QPointF(660, 530));
    for (int i = 1; i <= n; ++i) {
        ArrayBlock* pa = new ArrayBlock(i, 0, QString::number(i));
        pa->setCellSize(56, 40);
        pa->setPos(720 + (i - 1) * 58, 445);
        pa->color = kDpPale;
        scene->addItem(pa);
        m_paBlocks[i] = pa;
        registerArraySlot(pa, "pa", i);

        ArrayBlock* sz = new ArrayBlock(i, 1, QString::number(i));
        sz->setCellSize(56, 40);
        sz->setPos(720 + (i - 1) * 58, 545);
        sz->color = kDpPale;
        scene->addItem(sz);
        m_szBlocks[i] = sz;
        registerArraySlot(sz, "sz", i);
    }

    m_ptrX = new PointerBlock("x", QColor(210, 80, 55));
    m_ptrX->setOpacity(0);
    scene->addItem(m_ptrX);
}

std::vector<AnimFrame> DSUPathCompressionModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<int> pa(m_n + 1, 0);
    std::vector<int> sz(m_n + 1, 1);
    for (int i = 1; i <= m_n; ++i) {
        pa[i] = i;
        if (i - 1 < static_cast<int>(m_parents.size())) {
            int p = m_parents[i - 1];
            if (p >= 0 && p < m_n) pa[i] = p + 1;
        }
        int override = liveValue("pa", i, -1);
        if (override >= 0 && override < m_n) pa[i] = override + 1;
    }

    m_frames.push_back({ActionType::DP_RESET, 0, 0, 0, 0, {}, "初始化并查集：每个元素自成一个集合", "pa[i] = i　sz[i] = 1"});

    for (int i = 0; i < m_n; ++i) {
        m_frames.push_back({ActionType::DP_FIND, i + 1, 0, pa[i], 0, {},
                            QString("find(%1) 开始向上寻找根").arg(i + 1),
                            QString("find(%1)").arg(i + 1)});
        std::vector<int> path;
        int x = i + 1;
        int guard = 0;
        while (pa[x] != x && guard++ < 64) {
            path.push_back(x);
            x = pa[x];
            m_frames.push_back({ActionType::DP_FIND, x, 0, pa[x], 0, {path.begin(), path.end()},
                                QString("沿父指针走到 %1").arg(x),
                                QString("pa[%1] = %2").arg(x).arg(pa[x])});
        }
        path.push_back(x);
        m_frames.push_back({ActionType::DP_FIND, x, 1, x, 0, {path.begin(), path.end()},
                            QString("找到根结点 %1").arg(x),
                            QString("根 = %1").arg(x)});
        m_frames.push_back({ActionType::DP_COMPRESS, x, 0, x, 0, {path.begin(), path.end()},
                            QString("回溯：把路径上 %1 个结点一次性重定向到根 %2").arg(path.size()).arg(x),
                            QString("路径压扁：%1").arg(path.size())});
        for (size_t p = 0; p + 1 < path.size(); ++p) {
            pa[path[p]] = x;
        }
        for (int node : path) {
            m_frames.push_back({ActionType::DP_COMPRESS, node, 1, x, 0, {path.begin(), path.end()},
                                QString("pa[%1] = %2").arg(node).arg(x),
                                QString("pa[%1] = %2").arg(node).arg(x)});
        }
    }

    m_frames.push_back({ActionType::DP_DONE, 0, 0, 0, 0, {}, "路径压缩完成", "整棵树被压扁成一颗星"});
    return m_frames;
}

void DSUPathCompressionModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    auto refresh = [&]() {
        for (int i = 1; i <= m_n; ++i) {
            if (!m_paBlocks.count(i)) continue;
            ArrayBlock* block = m_paBlocks[i];
            block->update();
        }
    };

    if (frame.type == ActionType::DP_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kDpPale;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->color = QColor(180, 188, 202);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (int i = 1; i <= m_n; ++i) {
            if (m_paBlocks.count(i)) {
                m_paBlocks[i]->setCustomValue("—");
                m_paBlocks[i]->color = kDpPale;
                m_paBlocks[i]->update();
            }
            if (m_szBlocks.count(i)) {
                m_szBlocks[i]->value = 1;
                m_szBlocks[i]->customValue.clear();
                m_szBlocks[i]->showValue = true;
                m_szBlocks[i]->color = kDpPale;
                m_szBlocks[i]->update();
            }
        }
        if (m_ptrX) m_ptrX->setOpacity(0);
        refresh();
    } else if (frame.type == ActionType::DP_FIND) {
        for (auto& kv : m_nodes) {
            kv.second->color = kDpFaint;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->color = kDpFaint;
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (int node : frame.group) {
            auto it = m_nodes.find(node);
            if (it == m_nodes.end()) continue;
            it->second->color = kDpPath;
            it->second->setHighlightColor(kDpPath);
            it->second->update();
        }
        if (frame.v == 1) {
            auto it = m_nodes.find(frame.u);
            if (it != m_nodes.end()) {
                it->second->color = kDpRoot;
                it->second->setHighlightColor(kDpRoot);
                it->second->update();
                if (m_animated) playScalePulse(it->second, 1.25, 280);
            }
        }
        if (m_ptrX) {
            m_ptrX->setOpacity(1);
            auto it = m_nodes.find(frame.u);
            if (it != m_nodes.end()) {
                retargetPointerToNode(m_ptrX, it->second);
            }
        }
    } else if (frame.type == ActionType::DP_COMPRESS) {
        int root = frame.val1;
        if (frame.v == 0) {
            for (auto& kv : m_nodes) {
                kv.second->color = kDpPath;
                kv.second->setHighlightColor(kDpPath);
                kv.second->update();
            }
            for (auto& kv : m_edges) {
                kv.second->color = kDpPath;
                kv.second->setPenWidth(3.0);
                kv.second->update();
            }
            return;
        }
        int node = frame.u;
        auto it = m_nodes.find(node);
        if (it == m_nodes.end()) return;
        it->second->color = kDpPath;
        it->second->setHighlightColor(kDpPath);
        it->second->update();
        auto rootIt = m_nodes.find(root);
        if (rootIt != m_nodes.end()) {
            rootIt->second->color = kDpRoot;
            rootIt->second->setHighlightColor(kDpRoot);
            rootIt->second->update();
        }
        for (auto& kv : m_edges) {
            GraphEdge* edge = kv.second;
            if (!edge) continue;
            int u = edge->source ? edge->source->id : 0;
            int v = edge->dest ? edge->dest->id : 0;
            if ((u == node && v == root) || (u == root && v == node)) {
                edge->color = kDpRoot;
                edge->setPenWidth(3.4);
                edge->update();
            } else if (u == node || v == node) {
                edge->color = kDpFaint;
                edge->setPenWidth(2.0);
                edge->update();
            }
        }
        if (m_paBlocks.count(node)) {
            m_paBlocks[node]->setCustomValue(QString::number(root));
            m_paBlocks[node]->color = kDpPath;
            m_paBlocks[node]->update();
        }
        auto nodeIt = m_nodes.find(node);
        if (nodeIt != m_nodes.end()) {
            nodeIt->second->setSubLabel("pa=" + QString::number(root));
            nodeIt->second->update();
        }
    } else if (frame.type == ActionType::DP_DONE) {
        for (auto& kv : m_nodes) {
            kv.second->color = kDpGold;
            kv.second->setHighlightColor(kDpGold);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->color = kDpGold;
            kv.second->setPenWidth(2.4);
            kv.second->update();
        }
        if (m_ptrX) m_ptrX->setOpacity(0);
    }
}

void DSUPathCompressionModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

void DSUPathCompressionModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_paBlocks.clear();
    m_szBlocks.clear();
    m_parents.clear();
    m_mathText = nullptr;
    m_ptrX = nullptr;
    m_n = 0;
    m_valid = false;
}

void DSUPathCompressionModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_paBlocks.clear();
    m_szBlocks.clear();
    m_mathText = nullptr;
    m_ptrX = nullptr;
}
