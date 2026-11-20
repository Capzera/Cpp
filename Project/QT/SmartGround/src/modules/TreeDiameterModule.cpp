#include "TreeDiameterModule.h"
#include <QFont>
#include <algorithm>

static const QColor kTdPale = QColor(252, 252, 254);
static const QColor kTdWarm = QColor(248, 198, 150);
static const QColor kTdCool = QColor(158, 196, 240);
static const QColor kTdGold = QColor(255, 210, 110);
static const QColor kTdPath = QColor(228, 78, 72);

TreeDiameterModule::TreeDiameterModule() {}

QString TreeDiameterModule::getModuleName() const { return "树的直径"; }
QString TreeDiameterModule::getDefaultInput() const { return "1 2 3\n2 4 5\n3 6\n4 7"; }

QString TreeDiameterModule::getInputHint() const {
    return "每行输入一个节点的子节点编号，空格分隔\n"
           "第一行的编号为根节点，编号从 1 开始\n"
           "算法做两次 DFS：第一轮找到最远点 A，第二轮从 A 出发找到 B，A→B 即为直径";
}

void TreeDiameterModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    if (!parseTreeInput(inputData, m_children, m_maxId)) return;
    if (m_maxId > 14) return;
    m_valid = true;

    m_mathText = createScriptText(scene, "树的直径：两轮 DFS，先找最远点 A，再从 A 找最远点 B");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);

    layoutTreeScene(m_children, 1, QPointF(340, 140), 104.0, 178.0);

    m_depthText = createScriptText(scene, "");
    m_depthText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_depthText->setDefaultTextColor(QColor(40, 70, 110));
    m_depthText->setPos(70, 560);

    m_ptrU = new PointerBlock("u", QColor(210, 80, 55));
    m_ptrU->setOpacity(0);
    scene->addItem(m_ptrU);
}

std::vector<AnimFrame> TreeDiameterModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    m_frames.push_back({ActionType::TD_RESET, 0, 0, 0, 0, {}, "第一轮 DFS：从根出发维护 dep[] 与最深点 c", "dep[v] = dep[u] + 1"});

    std::map<int, std::vector<int>> undirected;
    for (const auto& kv : m_children) {
        for (int v : kv.second) {
            undirected[kv.first].push_back(v);
            undirected[v].push_back(kv.first);
        }
    }

    std::map<int, int> dep;
    int c = 1;
    std::function<void(int, int)> dfs1 = [&](int u, int fa) {
        auto it = undirected.find(u);
        if (it == undirected.end()) return;
        for (int v : it->second) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            m_frames.push_back({ActionType::TD_WALK, v, 0, dep[v], u, {},
                                QString("走到结点 %1，dep[%1] = %2").arg(v).arg(dep[v]),
                                QString("dep[%1] = dep[%2] + 1 = %3").arg(v).arg(u).arg(dep[v])});
            if (dep[v] > dep[c]) {
                c = v;
                m_frames.push_back({ActionType::TD_FAR, c, 0, dep[c], 0, {},
                                    QString("%1 更深，最深点更新为 %1").arg(c),
                                    QString("c = %1　dep = %2").arg(c).arg(dep[c])});
            }
            dfs1(v, u);
        }
    };
    dfs1(1, 0);

    int a = c;
    m_frames.push_back({ActionType::TD_PIN, a, 0, dep[a], 0, {},
                        QString("第一轮结束，最远点 A = %1，深度 %2").arg(a).arg(dep[a]),
                        QString("钉住起点 A = %1").arg(a)});

    m_frames.push_back({ActionType::TD_RESET, 1, 0, 0, 0, {}, "第二轮 DFS：从 A 出发再次寻找最远点", "dep 清零，重新计算"});

    std::map<int, int> dep2;
    std::map<int, int> parent2;
    int b = a;
    std::function<void(int, int)> dfs2 = [&](int u, int fa) {
        auto it = undirected.find(u);
        if (it == undirected.end()) return;
        for (int v : it->second) {
            if (v == fa) continue;
            dep2[v] = dep2[u] + 1;
            parent2[v] = u;
            m_frames.push_back({ActionType::TD_WALK, v, 1, dep2[v], u, {},
                                QString("从 A 走到 %1，dep = %2").arg(v).arg(dep2[v]),
                                QString("dep[%1] = %2").arg(v).arg(dep2[v])});
            if (dep2[v] > dep2[b]) {
                b = v;
                m_frames.push_back({ActionType::TD_FAR, b, 1, dep2[b], 0, {},
                                    QString("最深点更新为 %1").arg(b),
                                    QString("c = %1　dep = %2").arg(b).arg(dep2[b])});
            }
            dfs2(v, u);
        }
    };
    dfs2(a, 0);

    m_frames.push_back({ActionType::TD_PATH, a, b, dep2[b], 0, {1},
                        QString("直径端点 B = %1，直径长度 %2").arg(b).arg(dep2[b]),
                        QString("直径 A=%1 → B=%2　长度 %3").arg(a).arg(b).arg(dep2[b])});

    std::vector<int> path;
    int cur = b;
    path.push_back(cur);
    while (cur != a && parent2.count(cur) && path.size() < 40) {
        cur = parent2[cur];
        path.push_back(cur);
    }
    for (size_t i = 0; i < path.size(); ++i) {
        int prev = (i + 1 < path.size()) ? path[i + 1] : 0;
        m_frames.push_back({ActionType::TD_PATH, static_cast<int>(i), static_cast<int>(path.size()), path[i], prev, {0},
                            QString("点亮直径路径第 %1 段").arg(i + 1),
                            QString("直径节点 %1").arg(path[i])});
    }
    m_frames.push_back({ActionType::TD_DONE, a, b, dep2[b], 0, {},
                        QString("直径长度 = %1").arg(dep2[b]),
                        QString("A = %1　B = %2　长度 = %3").arg(a).arg(b).arg(dep2[b])});
    return m_frames;
}

void TreeDiameterModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    bool secondRound = false;
    if (frame.type == ActionType::TD_RESET) {
        secondRound = frame.u == 1;
        for (auto& kv : m_nodes) {
            kv.second->color = kTdPale;
            kv.second->setTag("", QColor());
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_treeEdges) {
            kv.second->color = QColor(178, 188, 204);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        if (m_depthText) setScriptText(m_depthText, secondRound ? "第二轮 DFS：从 A 出发" : "第一轮 DFS：从根出发");
        if (m_ptrU) m_ptrU->setOpacity(0);
        return;
    }

    if (frame.type == ActionType::TD_WALK) {
        int u = frame.u;
        int parent = frame.v;
        auto it = m_nodes.find(u);
        if (it == m_nodes.end()) return;
        QColor tint = frame.group.size() > 0 && frame.group[0] == 1 ? kTdCool : kTdWarm;
        it->second->color = tint;
        it->second->setHighlightColor(tint);
        it->second->setTag(QString("dep=%1").arg(frame.val1), tint);
        it->second->update();
        auto edgeIt = m_treeEdges.find({parent, u});
        if (edgeIt == m_treeEdges.end()) edgeIt = m_treeEdges.find({u, parent});
        if (edgeIt != m_treeEdges.end()) {
            edgeIt->second->color = tint;
            edgeIt->second->setPenWidth(3.2);
            edgeIt->second->update();
            playEdgeTravel(edgeIt->second, tint, 460);
        }
        if (m_animated && it->second->scene()) {
            playRipple(it->second->pos(), tint, 46.0, 620);
        }
        if (m_ptrU) {
            m_ptrU->setOpacity(1);
            retargetPointerToNode(m_ptrU, it->second);
        }
        if (m_depthText) {
            setScriptText(m_depthText, QString("当前结点 %1　dep = %2").arg(u).arg(frame.val1));
        }
    } else if (frame.type == ActionType::TD_FAR) {
        int u = frame.u;
        auto it = m_nodes.find(u);
        if (it == m_nodes.end()) return;
        it->second->color = kTdGold;
        it->second->setHighlightColor(kTdGold);
        it->second->setTag(QString("最深 dep=%1").arg(frame.val1), kTdGold);
        it->second->update();
        if (m_animated) {
            playScalePulse(it->second, 1.26, 300);
            playRipple(it->second->pos(), kTdGold, 52.0, 700);
        }
    } else if (frame.type == ActionType::TD_PIN) {
        int a = frame.u;
        auto it = m_nodes.find(a);
        if (it != m_nodes.end()) {
            it->second->color = kTdGold;
            it->second->setHighlightColor(kTdGold);
            it->second->setTag("A", kTdGold);
            it->second->update();
            if (m_animated) {
                playScalePulse(it->second, 1.35, 340);
                playRipple(it->second->pos(), kTdGold, 58.0, 780);
            }
        }
        if (m_depthText) setScriptText(m_depthText, QString("钉住 A = %1").arg(a));
    } else if (frame.type == ActionType::TD_PATH) {
        int node = frame.val1;
        auto it = m_nodes.find(node);
        if (it != m_nodes.end()) {
            it->second->color = kTdPath;
            it->second->setHighlightColor(kTdPath);
            it->second->update();
        }
        if (!frame.group.empty() && frame.group[0] == 1) {
            if (m_depthText) {
                setScriptText(m_depthText, QString("直径：%1 → %2　长度 %3").arg(frame.u).arg(frame.v).arg(frame.val1));
            }
            return;
        }
        int prev = frame.val2;
        if (prev > 0 && node > 0) {
            auto edgeIt = m_treeEdges.find({prev, node});
            if (edgeIt == m_treeEdges.end()) edgeIt = m_treeEdges.find({node, prev});
            if (edgeIt != m_treeEdges.end()) {
                edgeIt->second->color = kTdPath;
                edgeIt->second->setPenWidth(4.6);
                edgeIt->second->update();
                playEdgeTravel(edgeIt->second, kTdPath, 520);
            }
        }
    } else if (frame.type == ActionType::TD_DONE) {
        if (m_ptrU) m_ptrU->setOpacity(0);
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        if (m_depthText) {
            setScriptText(m_depthText, QString("直径 A=%1 → B=%2　长度 = %3").arg(frame.u).arg(frame.v).arg(frame.val1));
        }
    }
}

void TreeDiameterModule::reset() {
    m_frames.clear();
    m_children.clear();
    m_nodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
    m_depthText = nullptr;
    m_ptrU = nullptr;
    m_maxId = 0;
    m_valid = false;
}

void TreeDiameterModule::releaseSceneItems() {
    m_nodes.clear();
    m_treeEdges.clear();
    m_mathText = nullptr;
    m_depthText = nullptr;
    m_ptrU = nullptr;
}
