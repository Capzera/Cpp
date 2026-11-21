#include "HungarianModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kHnLeft = QColor(150, 185, 235);
static const QColor kHnRight = QColor(250, 200, 150);
static const QColor kHnTry = QColor(255, 214, 140);
static const QColor kHnMatched = QColor(140, 205, 150);
static const QColor kHnFailed = QColor(238, 120, 108);
static const QColor kHnPale = QColor(250, 250, 252);

HungarianModule::HungarianModule() {}

QString HungarianModule::getModuleName() const { return "二分图匹配 - 匈牙利算法"; }
QString HungarianModule::getDefaultInput() const {
    return "3 3\n"
           "1 1\n"
           "1 2\n"
           "2 1\n"
           "3 1\n"
           "3 2\n"
           "3 3";
}

bool HungarianModule::supportsGraphOptions() const { return true; }
bool HungarianModule::supportsDirected() const { return false; }
bool HungarianModule::supportsBidirectional() const { return false; }
GraphModuleUtils::GraphOptions HungarianModule::getGraphOptions() const { return m_options; }
void HungarianModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) {
    m_options = options;
}

QString HungarianModule::getInputHint() const {
    return "第一行输入两个正整数：左部顶点数 nL、右部顶点数 nR\n"
           "接下来若干行，每行两个整数 u, v，表示左部 u 与右部 v 之间存在边\n"
           "算法按左部顶点依次尝试匹配，遇到已占用的右部顶点就沿增广路递归让位\n"
           "默认数据特意构造了「分手」场景：L1 先占住 R1，L2 到来时被迫与 L1 分手，\n"
           "L1 改配 R2、L2 接盘 R1，随后 L3 同时冲击 R1/R2/R3 触发多级让位";
}

std::map<int, QPointF> HungarianModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void HungarianModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void HungarianModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_nodes.clear();
    m_edges.clear();
    m_matchLeft.clear();
    m_matchRight.clear();
    m_frames.clear();

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    bool ok = false;
    if (lines.isEmpty()) {
        m_mathText = createScriptText(scene, "输入为空");
        return;
    }
    QStringList head = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (head.size() < 2) {
        m_mathText = createScriptText(scene, "第一行需要 nL 与 nR");
        return;
    }
    m_leftCount = head[0].toInt(&ok);
    if (!ok || m_leftCount < 1 || m_leftCount > 8) {
        m_mathText = createScriptText(scene, "nL 需为 1–8 的整数");
        return;
    }
    m_rightCount = head[1].toInt(&ok);
    if (!ok || m_rightCount < 1 || m_rightCount > 8) {
        m_mathText = createScriptText(scene, "nR 需为 1–8 的整数");
        return;
    }
    std::vector<std::pair<int, int>> edgeList;
    for (int i = 1; i < lines.size(); ++i) {
        QStringList t = lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (t.size() < 2) continue;
        int u = t[0].toInt(&ok);
        if (!ok || u < 1 || u > m_leftCount) continue;
        int v = t[1].toInt(&ok);
        if (!ok || v < 1 || v > m_rightCount) continue;
        edgeList.push_back({u, v});
    }
    m_valid = true;

    m_mathText = createScriptText(scene, "");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    setScriptText(m_mathText, "匈牙利算法：为每个左部顶点寻找增广路，匹配数逐个增加");

    GraphRenderHelper::addLabel(scene, "左部 L", QPointF(m_leftX - 26, m_topY - 52));
    GraphRenderHelper::addLabel(scene, "右部 R", QPointF(m_rightX - 26, m_topY - 52));

    const double bandW = m_rightX - m_leftX;
    const double bandH = m_rowGap * std::max(m_leftCount, m_rightCount) + 24.0;
    QGraphicsRectItem* leftBand = scene->addRect(QRectF(m_leftX - 78.0, m_topY - 34.0, 156.0, bandH),
                                                QPen(QColor(150, 185, 235, 90), 1.6, Qt::DashLine),
                                                QBrush(QColor(150, 185, 235, 16)));
    leftBand->setZValue(-2);
    leftBand->setAcceptedMouseButtons(Qt::NoButton);
    QGraphicsRectItem* rightBand = scene->addRect(QRectF(m_rightX - 78.0, m_topY - 34.0, 156.0, bandH),
                                                 QPen(QColor(250, 200, 150, 90), 1.6, Qt::DashLine),
                                                 QBrush(QColor(250, 200, 150, 16)));
    rightBand->setZValue(-2);
    rightBand->setAcceptedMouseButtons(Qt::NoButton);
    Q_UNUSED(bandW);

    m_augmentTrail = new PathTrail(kHnTry);
    m_augmentTrail->setTrailWidth(4.5);
    m_augmentTrail->setHeadRadius(7.0);
    scene->addItem(m_augmentTrail);
    m_augmentPoints.clear();

    std::vector<int> leftY, rightY;
    for (int i = 1; i <= m_leftCount; ++i) {
        GraphNode* node = new GraphNode(i);
        node->setPos(m_leftX, m_topY + (i - 1) * m_rowGap);
        node->color = kHnLeft;
        scene->addItem(node);
        m_nodes[i] = node;
        leftY.push_back(static_cast<int>(node->pos().y()));
    }
    for (int i = 1; i <= m_rightCount; ++i) {
        int id = 100 + i;
        GraphNode* node = new GraphNode(id);
        node->setPos(m_rightX, m_topY + (i - 1) * m_rowGap);
        node->color = kHnRight;
        scene->addItem(node);
        m_nodes[id] = node;
        rightY.push_back(static_cast<int>(node->pos().y()));
    }

    for (const auto& e : edgeList) {
        int rid = 100 + e.second;
        GraphEdge* edge = new GraphEdge(m_nodes[e.first], m_nodes[rid], false, false);
        edge->color = QColor(190, 198, 210);
        scene->addItem(edge);
        m_nodes[e.first]->addEdge(edge);
        m_nodes[rid]->addEdge(edge);
        m_edges[{e.first, e.second}] = edge;
    }

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(m_leftX - 40, m_topY + m_rowGap * m_leftCount + 20);
}

std::vector<AnimFrame> HungarianModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<std::vector<int>> adj(m_leftCount + 1);
    for (const auto& kv : m_edges) adj[kv.first.first].push_back(kv.first.second);
    for (int i = 1; i <= m_leftCount; ++i) std::sort(adj[i].begin(), adj[i].end());

    std::map<int, int> matchRight;
    std::map<int, int> matchLeft;
    std::vector<int> visited;

    m_frames.push_back({ActionType::HN_RESET, 0, 0, 0, 0, {},
                        "开始匈牙利算法：逐个处理左部顶点，寻找增广路",
                        "matchR[v] 记录右部顶点 v 当前匹配的左部顶点"});

    std::function<bool(int)> augment = [&](int u) -> bool {
        for (int v : adj[u]) {
            if (std::find(visited.begin(), visited.end(), v) != visited.end()) continue;
            visited.push_back(v);
            m_frames.push_back({ActionType::HN_TRY, u, v, 0, 0, {},
                                QString("左 %1 尝试匹配右 %2").arg(u).arg(v),
                                QString("尝试匹配 L%1 - R%2").arg(u).arg(v)});
            if (matchRight.find(v) == matchRight.end()) {
                m_frames.push_back({ActionType::HN_ASSIGN, u, v, 0, 0, {},
                                    QString("右 %1 空闲，直接匹配 L%2 - R%1").arg(v).arg(u),
                                    QString("R%1 未被占用 → 匹配 L%2 - R%1").arg(v).arg(u)});
                matchRight[v] = u;
                matchLeft[u] = v;
                return true;
            }
            int holder = matchRight[v];
            m_frames.push_back({ActionType::HN_OCCUPIED, u, v, holder, 0, {},
                                QString("右 %1 已被左 %2 占用，尝试让左 %2 另找对象").arg(v).arg(holder),
                                QString("R%1 已被 L%2 占用 → 递归 augment(L%2)").arg(v).arg(holder)});
            if (augment(holder)) {
                m_frames.push_back({ActionType::HN_BACKTRACK, u, v, holder, 0, {},
                                    QString("左 %1 成功另找对象，右 %2 让给左 %1").arg(holder).arg(v),
                                    QString("L%1 腾出位置 → R%2 改配 L%3").arg(holder).arg(v).arg(u)});
                matchRight[v] = u;
                matchLeft[u] = v;
                matchLeft.erase(holder);
                return true;
            }
            m_frames.push_back({ActionType::HN_FAIL, u, v, 0, 0, {},
                                QString("左 %1 无法为右 %2 腾出位置").arg(u).arg(v),
                                QString("L%1 无法通过 R%2 增广").arg(u).arg(v)});
        }
        return false;
    };

    int matched = 0;
    for (int u = 1; u <= m_leftCount; ++u) {
        visited.clear();
        m_frames.push_back({ActionType::HN_START, u, 0, matched, 0, {},
                            QString("处理左部顶点 %1").arg(u),
                            QString("从 L%1 出发寻找增广路").arg(u)});
        if (augment(u)) {
            ++matched;
            m_frames.push_back({ActionType::HN_ASSIGN, u, 0, matched, 0, {},
                                QString("左 %1 匹配成功，当前匹配数 %2").arg(u).arg(matched),
                                QString("匹配数 = %1").arg(matched)});
        } else {
            m_frames.push_back({ActionType::HN_FAIL, u, 0, 0, 0, {},
                                QString("左 %1 匹配失败（无增广路）").arg(u),
                                QString("L%1 匹配失败").arg(u)});
        }
    }
    m_frames.push_back({ActionType::HN_DONE, 0, 0, matched, 0, {},
                        QString("最终最大匹配数为 %1").arg(matched),
                        QString("最大匹配数 = %1").arg(matched)});
    return m_frames;
}

void HungarianModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (frame.type == ActionType::HN_RESET) {
        for (auto& kv : m_nodes) {
            kv.second->color = kv.first <= m_leftCount ? kHnLeft : kHnRight;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) { kv.second->color = QColor(190, 198, 210); kv.second->update(); }
        if (m_augmentTrail) {
            m_augmentTrail->clearPoints();
            m_augmentTrail->setProgress(0.0);
        }
        m_augmentPoints.clear();
        m_matchedEdges.clear();
        if (m_resultText) setScriptText(m_resultText, "");
        if (m_relationText) setScriptText(m_relationText, "");
    } else if (frame.type == ActionType::HN_START) {
        GraphNode* node = m_nodes[frame.u];
        if (node) {
            node->setHighlightColor(kHnTry);
            node->color = kHnTry;
            node->update();
            playScalePulse(node, 1.25, 260);
            playNodeRipple(node, kHnTry, 46.0, 620);
        }
        if (m_augmentTrail) {
            m_augmentTrail->clearPoints();
            m_augmentTrail->setProgress(0.0);
        }
        m_augmentPoints.clear();
        if (node) {
            m_augmentPoints.append(node->pos());
            if (m_augmentTrail) {
                m_augmentTrail->setPoints(m_augmentPoints);
                m_augmentTrail->setColor(kHnTry);
                m_augmentTrail->setProgress(1.0);
            }
        }
    } else if (frame.type == ActionType::HN_TRY) {
        GraphNode* left = m_nodes[frame.u];
        GraphNode* right = m_nodes[100 + frame.v];
        if (left) { left->setHighlightColor(kHnTry); left->update(); }
        if (right) { right->setHighlightColor(kHnTry); right->update(); }
        auto it = m_edges.find({frame.u, frame.v});
        if (it != m_edges.end()) {
            it->second->color = kHnTry;
            it->second->update();
            playEdgeTravelFrom(it->second, kHnTry, frame.u, 480);
        }
        if (m_augmentTrail && right) {
            if (m_augmentPoints.isEmpty() && left) m_augmentPoints.append(left->pos());
            m_augmentPoints.append(right->pos());
            playPathTrail(m_augmentTrail, m_augmentPoints, kHnTry, 460);
        }
    } else if (frame.type == ActionType::HN_OCCUPIED) {
        GraphNode* right = m_nodes[100 + frame.v];
        GraphNode* holder = m_nodes[frame.val1];
        if (right) { right->setHighlightColor(kHnFailed); right->update(); }
        if (holder) { holder->setHighlightColor(kHnFailed); holder->update(); }
        auto it = m_edges.find({frame.val1, frame.v});
        if (it != m_edges.end()) {
            it->second->color = kHnFailed;
            it->second->setPenWidth(4.0);
            it->second->update();
            playEdgeTravelFrom(it->second, kHnFailed, frame.val1, 460);
        }
        if (m_augmentTrail && holder) {
            if (m_augmentPoints.isEmpty() && right) m_augmentPoints.append(right->pos());
            m_augmentPoints.append(holder->pos());
            playPathTrail(m_augmentTrail, m_augmentPoints, kHnFailed, 520);
        }
        if (right) playNodeRipple(right, kHnFailed, 44.0, 600);
        if (holder) playNodeRipple(holder, kHnFailed, 44.0, 600);
        if (m_relationText) {
            setScriptText(m_relationText, 
                QString("R%1 已与 L%2 配对 → L%2 必须分手另找对象")
                    .arg(frame.v).arg(frame.val1));
            m_relationText->setDefaultTextColor(kHnFailed.darker(115));
        }
    } else if (frame.type == ActionType::HN_BACKTRACK) {
        auto it = m_edges.find({frame.u, frame.v});
        if (it != m_edges.end()) {
            it->second->color = kHnMatched;
            it->second->setPenWidth(4.0);
            it->second->update();
            playEdgeDiscover(it->second, m_nodes.count(100 + frame.v) ? m_nodes[100 + frame.v] : nullptr,
                             kHnMatched, 540);
        }
        GraphNode* left = m_nodes[frame.u];
        GraphNode* right = m_nodes[100 + frame.v];
        if (left) { left->color = kHnMatched; left->setHighlightColor(kHnMatched); left->update(); }
        if (right) { right->color = kHnMatched; right->setHighlightColor(kHnMatched); right->update(); }
        if (left) playNodeRipple(left, kHnMatched, 44.0, 580);
        if (right) playNodeRipple(right, kHnMatched, 44.0, 580);
        if (m_relationText) {
            setScriptText(m_relationText, 
                QString("分手完成：L%1 让出 R%2，改配 R%3").arg(frame.val1).arg(frame.v).arg(frame.u));
            m_relationText->setDefaultTextColor(kHnMatched.darker(115));
        }
    } else if (frame.type == ActionType::HN_ASSIGN) {
        if (frame.v > 0) {
            auto it = m_edges.find({frame.u, frame.v});
            if (it != m_edges.end()) {
                it->second->color = kHnMatched;
                it->second->setPenWidth(4.0);
                it->second->update();
                playEdgeDiscover(it->second, m_nodes.count(100 + frame.v) ? m_nodes[100 + frame.v] : nullptr,
                                 kHnMatched, 560);
            }
            GraphNode* left = m_nodes[frame.u];
            GraphNode* right = m_nodes[100 + frame.v];
            if (left) { left->color = kHnMatched; left->setHighlightColor(kHnMatched); left->update(); }
            if (right) { right->color = kHnMatched; right->setHighlightColor(kHnMatched); right->update(); }
            if (right) playNodeRipple(right, kHnMatched, 48.0, 640);
            const std::pair<int, int> pairKey(frame.u, frame.v);
            if (std::find(m_matchedEdges.begin(), m_matchedEdges.end(), pairKey) == m_matchedEdges.end()) {
                m_matchedEdges.push_back(pairKey);
            }
            if (m_augmentTrail && left && right) {
                m_augmentPoints.append(right->pos());
                playPathTrail(m_augmentTrail, m_augmentPoints, kHnMatched, 520);
            }
        }
        if (m_resultText) {
            setScriptText(m_resultText, QString("当前匹配数 = %1").arg(frame.val1));
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::HN_FAIL) {
        GraphNode* left = m_nodes[frame.u];
        if (left) { left->color = kHnFailed; left->setHighlightColor(kHnFailed); left->update(); }
        if (frame.v > 0) {
            auto it = m_edges.find({frame.u, frame.v});
            if (it != m_edges.end()) {
                it->second->color = kHnFailed;
                it->second->update();
            }
        }
    } else if (frame.type == ActionType::HN_DONE) {
        for (auto& kv : m_nodes) {
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            if (kv.second->color != kHnMatched) { kv.second->color = QColor(190, 198, 210); kv.second->update(); }
        }
        if (m_augmentTrail) {
            m_augmentTrail->setColor(kHnMatched);
            m_augmentTrail->setProgress(1.0);
        }
        for (const auto& pairKey : m_matchedEdges) {
            GraphNode* left = m_nodes.count(pairKey.first) ? m_nodes[pairKey.first] : nullptr;
            GraphNode* right = m_nodes.count(100 + pairKey.second) ? m_nodes[100 + pairKey.second] : nullptr;
            if (left) playNodePop(left, 1.16, 300);
            if (right) playNodePop(right, 1.16, 300);
        }
        if (m_resultText) {
            setScriptText(m_resultText, QString("最大匹配数 = %1").arg(frame.val1));
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void HungarianModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_matchLeft.clear();
    m_matchRight.clear();
    m_matchedEdges.clear();
    m_augmentTrail = nullptr;
    m_augmentPoints.clear();
    m_mathText = nullptr;
    m_resultText = nullptr;
    m_relationText = nullptr;
    m_leftCount = 0;
    m_rightCount = 0;
    m_valid = false;
}

void HungarianModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_matchedEdges.clear();
    m_augmentTrail = nullptr;
    m_augmentPoints.clear();
    m_mathText = nullptr;
    m_resultText = nullptr;
    m_relationText = nullptr;
}
