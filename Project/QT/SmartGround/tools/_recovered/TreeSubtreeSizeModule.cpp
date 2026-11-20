#include "TreeSubtreeSizeModule.h"
#include <QFont>
#include <algorithm>

static const QColor kTszPale = QColor(252, 252, 254);
static const QColor kTszDone = QColor(150, 214, 170);
static const QColor kTszActive = QColor(255, 214, 140);
static const QColor kTszFlight = QColor(246, 176, 120);

TreeSubtreeSizeModule::TreeSubtreeSizeModule() {}

QString TreeSubtreeSizeModule::getModuleName() const { return "子树大小统计"; }
QString TreeSubtreeSizeModule::getDefaultInput() const { return "1 2 3\n2 4 5\n3 6 7\n6 8"; }

QString TreeSubtreeSizeModule::getInputHint() const {
    return "每行输入一个节点的子节点编号，空格分隔\n"
           "第一行的编号为根节点，编号从 1 开始\n"
           "例如 1 2 3 表示根 1 有孩子 2 和 3\n"
           "算法按后序遍历自底向上计算 sz[u] = 1 + Σ sz[v]";
}

void TreeSubtreeSizeModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    if (!parseTreeInput(inputData, m_children, m_maxId)) return;
    if (m_maxId > 14) return;
    m_valid = true;

    m_mathText = scene->addText("子树大小：后序遍历自底向上，sz[u] = 1 + Σ sz[v]");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);

    layoutTreeScene(m_children, 1, QPointF(300, 130), 100.0, 110.0);

    m_orderTitle = scene->addText("后序遍历序列");
    m_orderTitle->setFont(QFont("Microsoft YaHei", 11, QFont::Bold));
    m_orderTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_orderTitle->setPos(70, 560);

    for (int i = 0; i < m_maxId; ++i) {
        ArrayBlock* block = new ArrayBlock(i + 1, 0, QString::number(i + 1));
        block->setCellSize(52, 40);
        block->setPos(70 + i * 58, 590);
        block->color = kTszPale;
        scene->addItem(block);
        m_orderBlocks.push_back(block);
    }
}

std::vector<AnimFrame> TreeSubtreeSizeModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<int> sz(m_maxId + 1, 0);
    std::vector<int> order;

    m_frames.push_back({ActionType::TSZ_RESET, 0, 0, 0, 0, {}, "初始化：sz[u] 全部待计算", "sz[u] = 1 + Σ sz[v]"});

    std::function<void(int, int)> dfs = [&](int u, int fa) {
        sz[u] = 1;
        m_frames.push_back({ActionType::TSZ_ENTER, u, 0, 1, 0, {},
                            QString("进入结点 %1，sz[%1] = 1").arg(u),
                            QString("sz[%1] = 1").arg(u)});
        auto it = m_children.find(u);
        if (it != m_children.end()) {
            for (int v : it->second) {
                if (v == fa) continue;
                dfs(v, u);
                m_frames.push_back({ActionType::TSZ_ADD, v, u, sz[v], sz[u], {},
                                    QString("子结点 %1 的 sz=%2 汇入父结点 %3").arg(v).arg(sz[v]).arg(u),
                                    QString("sz[%1] += sz[%2] = %3").arg(u).arg(v).arg(sz[u] + sz[v])});
                sz[u] += sz[v];
            }
        }
        m_frames.push_back({ActionType::TSZ_COMPUTE, u, 0, sz[u], 0, {},
                            QString("结点 %1 的所有孩子都已汇入，sz[%1] = %2").arg(u).arg(sz[u]),
                            QString("sz[%1] = %2").arg(u).arg(sz[u])});
        order.push_back(u);
        m_frames.push_back({ActionType::TSZ_ORDER, static_cast<int>(order.size()) - 1, u, sz[u], 0, {},
                            QString("后序序列第 %1 位：结点 %2").arg(order.size()).arg(u),
                            QString("post[%1] = %2").arg(order.size() - 1).arg(u)});
    };
    dfs(1, 0);

    m_frames.push_back({ActionType::TSZ_DONE, 0, 0, static_cast<int>(order.size()), 0, {},
                        QString("全部结点计算完成，共 %1 个结点").arg(order.size()),
                        QString("树共有 %1 个结点").arg(order.size())});
    return m_frames;
}

void TreeSubtreeSizeModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::TSZ_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kTszPale;
            kv.second->setTag("", QColor());
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_treeEdges) {
            kv.second->color = QColor(178, 188, 204);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (ArrayBlock* block : m_orderBlocks) {
            if (!block) continue;
            block->setCustomValue("—");
            block->color = kTszPale;
            block->update();
        }
    } else if (frame.type == ActionType::TSZ_ENTER) {
        int u = frame.u;
        auto it = m_nodes.find(u);
        if (it == m_nodes.end()) return;
        it->second->color = kTszActive;
        it->second->setHighlightColor(kTszActive);
        it->second->update();
        it->second->setTag("sz=1", kTszActive);
        if (m_animated) playScalePulse(it->second, 1.15, 240);
    } else if (frame.type == ActionType::TSZ_ADD) {
        int child = frame.u;
        int parent = frame.v;
        auto parentIt = m_nodes.find(parent);
        if (parentIt == m_nodes.end()) return;
        parentIt->second->color = kTszFlight;
        parentIt->second->setHighlightColor(kTszFlight);
        parentIt->second->setTag(QString("sz=%1").arg(frame.val2 + frame.val1), kTszFlight);
        parentIt->second->update();
        auto edgeIt = m_treeEdges.find({parent, child});
        if (edgeIt != m_treeEdges.end()) {
            edgeIt->second->color = kTszFlight;
            edgeIt->second->setPenWidth(3.4);
            edgeIt->second->update();
        }
        if (m_animated) {
            QPointF from = m_nodes[child] ? m_nodes[child]->pos() : parentIt->second->pos();
            QPointF to = parentIt->second->pos();
            playBubbleFlight(from + QPointF(26, 0), to + QPointF(26, 0), frame.val1, 560);
            playScalePulse(parentIt->second, 1.18, 320);
        }
    } else if (frame.type == ActionType::TSZ_COMPUTE) {
        int u = frame.u;
        auto it = m_nodes.find(u);
        if (it == m_nodes.end()) return;
        it->second->color = kTszDone;
        it->second->setHighlightColor(kTszDone);
        it->second->setTag(QString("sz=%1").arg(frame.val1), kTszDone);
        it->second->update();
        if (m_animated) playScalePulse(it->second, 1.22, 300);
    } else if (frame.type == ActionType::TSZ_ORDER) {
        int index = frame.u;
        if (index < 0 || index >= static_cast<int>(m_orderBlocks.size())) return;
        ArrayBlock* block = m_orderBlocks[index];
        block->value = frame.v;
        block->customValue.clear();
        block->showValue = true;
        block->color = kTszDone;
        block->update();
        if (m_animated) playScalePulse(block, 1.2, 240);
    } else if (frame.type == ActionType::TSZ_DONE) {
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
    }
}

void TreeSubtreeSizeModule::reset() {
    m_frames.clear();
    m_children.clear();
    m_nodes.clear();
    m_treeEdges.clear();
    m_orderBlocks.clear();
    m_mathText = nullptr;
    m_orderTitle = nullptr;
    m_maxId = 0;
    m_valid = false;
}

void TreeSubtreeSizeModule::releaseSceneItems() {
    m_nodes.clear();
    m_treeEdges.clear();
    m_orderBlocks.clear();
    m_mathText = nullptr;
    m_orderTitle = nullptr;
}
