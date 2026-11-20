#include "DijkstraHeapModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <QVariantAnimation>
#include <algorithm>
#include <cmath>

static const QColor kDhPale = QColor(252, 252, 254);
static const QColor kDhTop = QColor(246, 176, 120);
static const QColor kDhHeap = QColor(158, 196, 240);
static const QColor kDhDone = QColor(198, 202, 210);
static const QColor kDhStale = QColor(214, 138, 128);

DijkstraHeapModule::DijkstraHeapModule() {}

QString DijkstraHeapModule::getModuleName() const { return "Dijkstra 堆优化"; }
QString DijkstraHeapModule::getDefaultInput() const { return "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9"; }

bool DijkstraHeapModule::supportsGraphOptions() const { return true; }
bool DijkstraHeapModule::supportsDirected() const { return true; }
bool DijkstraHeapModule::supportsBidirectional() const { return true; }
GraphModuleUtils::GraphOptions DijkstraHeapModule::getGraphOptions() const { return m_options; }
void DijkstraHeapModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }

QString DijkstraHeapModule::getInputHint() const {
    return "第一行输入三个正整数：顶点数 n、边数 m、源点 src\n"
           "接下来 m 行，每行三个整数 u, v, w，表示一条权值为 w 的边\n"
           "权值必须非负。本模板不使用 vis 数组，靠 if (d > dis[u]) continue 做懒删除\n"
           "优先队列 pq 用有序数组存储，元素是 pair(dis, u)，按 dis 从小到大排列\n"
           "新元素入队时插入到合适的位置 k，后面已有的元素整体右移一格";
}

std::map<int, QPointF> DijkstraHeapModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void DijkstraHeapModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void DijkstraHeapModule::layoutPqView(int capacity) {
    m_pqTitle = m_scene->addText("pq 堆（数组化优先队列，元素为 pair(dis, u)）");
    m_pqTitle->setFont(QFont("Microsoft YaHei", 11, QFont::Bold));
    m_pqTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_pqTitle->setPos(m_pqX - 10, m_pqY - 66);

    for (int i = 0; i < capacity; ++i) {
        ArrayBlock* block = new ArrayBlock(i + 1, 0, QString::number(i));
        block->setCellSize(m_pqCellW, m_pqCellH);
        block->setValueFontSize(11);
        block->setPos(m_pqX + i * (m_pqCellW + 10.0), m_pqY);
        block->color = kDhPale;
        block->setOpacity(0);
        m_scene->addItem(block);
        m_pqBlocks[i] = block;
        m_pqSpare.push_back(block);
    }
}

void DijkstraHeapModule::clearPqSlots() {
    for (auto& kv : m_pqBlocks) {
        if (!kv.second) continue;
        kv.second->setOpacity(0);
        kv.second->setScale(1.0);
        kv.second->customValue.clear();
        kv.second->showValue = false;
        kv.second->color = kDhPale;
        kv.second->update();
    }
    m_pqOrder.clear();
    m_pqSpare.clear();
    for (auto& kv : m_pqBlocks) {
        if (kv.second) m_pqSpare.push_back(kv.second);
    }
}

void DijkstraHeapModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    if (m_valid && input.weighted) {
        for (const auto& edge : input.edges) if (edge.w < 0) m_valid = false;
    }
    m_mathText = createScriptText(scene, m_valid ? "堆优化 Dijkstra：数组化优先队列 pq + 懒删除（无 vis 数组）" : "输入格式：n m src，随后输入 m 条非负权边 u v w");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    m_edgesInput = input.edges;
    m_heapCapacity = std::max(6, std::min(16, m_m + 2));

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(820, 220), m_n <= 8 ? 130 : 150, this);
    GraphRenderHelper::buildEdges(scene, m_nodes, m_edges, input.edges, m_options, input.weighted);
    GraphRenderHelper::registerEdgeSlots(this, m_edges, input.edges, m_options);

    layoutPqView(m_heapCapacity);

    m_heapText = createScriptText(scene, "");
    m_heapText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_heapText->setDefaultTextColor(QColor(40, 70, 110));
    m_heapText->setPos(m_pqX, m_pqY + m_pqCellH + 22);

    GraphRenderHelper::addLabel(scene, "dis[]", QPointF(24, m_pqY + 152));
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* dist = new ArrayBlock(i, 0, QString::number(i));
        dist->setCellSize(56, 40);
        dist->setPos(m_pqX + 20 + (i - 1) * 60, m_pqY + 148);
        dist->setCustomValue("∞");
        dist->color = kDhPale;
        scene->addItem(dist);
        m_distBlocks[i] = dist;
        registerArraySlot(dist, "dis", i);
    }

    m_ptrU = new PointerBlock("u", QColor(210, 80, 55));
    m_ptrU->setOpacity(0);
    scene->addItem(m_ptrU);
}

std::vector<AnimFrame> DijkstraHeapModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    constexpr int inf = 1000000000;

    std::vector<int> dist(m_n + 1, inf);
    dist[m_src] = 0;

    std::vector<std::pair<int, int>> pq;
    std::vector<std::vector<std::pair<int, int>>> adj(m_n + 1);
    for (size_t i = 0; i < m_edgesInput.size(); ++i) {
        const auto& edge = m_edgesInput[i];
        int w = liveValue("w", static_cast<int>(i), edge.w);
        adj[edge.u].push_back({edge.v, w});
        if (!m_options.directed) adj[edge.v].push_back({edge.u, w});
    }

    auto pushInto = [&pq](int d, int node) {
        int k = 0;
        while (k < static_cast<int>(pq.size()) && pq[k].first <= d) ++k;
        pq.insert(pq.begin() + k, std::make_pair(d, node));
        return k;
    };

    m_frames.push_back({ActionType::DH_RESET, m_src, 0, 0, 0, {},
                        QString("初始化 dis[] = ∞，源点 %1 入堆").arg(m_src),
                        QString("dis[%1] = 0　pq.push({0, %1})").arg(m_src)});
    int firstSlot = pushInto(0, m_src);
    m_frames.push_back({ActionType::DH_PUSH, firstSlot, 0, 0, m_src, {1, 0, 0},
                        QString("把 {0, %1} 压入 pq 的第 %2 个位置").arg(m_src).arg(firstSlot + 1),
                        QString("pq.push({0, %1})　→ 位置 %2").arg(m_src).arg(firstSlot + 1)});

    int guard = 0;
    while (!pq.empty() && guard++ < 4000) {
        std::pair<int, int> top = pq.front();
        pq.erase(pq.begin());
        int d = top.first;
        int u = top.second;
        m_frames.push_back({ActionType::DH_POP, 0, u, d, static_cast<int>(pq.size()), {1, 0, 0},
                            QString("弹出 pq 队首 {d=%1, u=%2}").arg(d).arg(u),
                            QString("pq.pop() → {%1, %2}　剩余 %3 个").arg(d).arg(u).arg(pq.size())});
        if (d > dist[u]) {
            m_frames.push_back({ActionType::DH_SKIP, u, 0, d, dist[u], {},
                                QString("d=%1 > dis[%2]=%3，是过期条目，直接跳过").arg(d).arg(u).arg(dist[u]),
                                QString("if (d > dis[u]) continue")});
            continue;
        }
        m_frames.push_back({ActionType::DH_RELAX, u, 0, dist[u], 0, {2, 0, 0},
                            QString("确定结点 %1 的最短距离 dis[%1] = %2").arg(u).arg(dist[u]),
                            QString("dis[%1] = %2").arg(u).arg(dist[u])});
        for (const auto& pair : adj[u]) {
            int v = pair.first;
            int w = pair.second;
            if (dist[u] + w < dist[v]) {
                int old = dist[v];
                dist[v] = dist[u] + w;
                int slot = pushInto(dist[v], v);
                m_frames.push_back({ActionType::DH_RELAX, u, v, dist[v], old, {2, 1, v},
                                    QString("松弛成功：dis[%1] = %2，{ %2, %1 } 入堆").arg(v).arg(dist[v]),
                                    QString("dis[%1] = dis[%2] + %3 = %4").arg(v).arg(u).arg(w).arg(dist[v])});
                m_frames.push_back({ActionType::DH_PUSH, slot, 0, dist[v], v, {1, 0, v},
                                    QString("把 { %1, %2 } 插入 pq 的第 %3 个位置").arg(dist[v]).arg(v).arg(slot + 1),
                                    QString("pq.insert(%1, {%2, %3})").arg(slot).arg(dist[v]).arg(v)});
            }
        }
    }
    m_frames.push_back({ActionType::DH_FINISH, m_n, 0, 0, 0, {},
                        QString("Dijkstra 堆优化完成"),
                        QString("有序数组 pq：O(MN)；改用二叉堆可达 O(M log N)")});
    return m_frames;
}

void DijkstraHeapModule::writePqSlot(int slot, int dist, int node) {
    if (slot < 0 || slot >= static_cast<int>(m_pqOrder.size())) return;
    ArrayBlock* block = m_pqOrder[slot];
    if (!block) return;
    block->setCustomValue(QString("(%1,%2)").arg(dist).arg(node));
    block->showValue = true;
    block->color = kDhHeap;
    block->setOpacity(1.0);
    block->update();
}

void DijkstraHeapModule::shiftPqFrom(int slot) {
    if (slot < 0 || slot >= static_cast<int>(m_pqOrder.size())) return;
    std::vector<QGraphicsItem*> items;
    for (int i = slot; i < static_cast<int>(m_pqOrder.size()); ++i) {
        if (m_pqOrder[i]) items.push_back(m_pqOrder[i]);
    }
    playRowShift(items, QPointF(m_pqCellW + 10.0, 0.0), 420);
}

void DijkstraHeapModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::DH_RESET) {
        for (auto& kv : m_nodes) { kv.second->color = Qt::white; kv.second->update(); }
        for (auto& kv : m_edges) { kv.second->setColor(Qt::black); kv.second->update(); }
        for (int i = 1; i <= m_n; ++i) {
            if (!m_distBlocks.count(i)) continue;
            m_distBlocks[i]->setCustomValue("∞");
            m_distBlocks[i]->color = kDhPale;
            m_distBlocks[i]->update();
        }
        clearPqSlots();
        if (m_distBlocks.count(m_src)) {
            m_distBlocks[m_src]->setCustomValue("0");
            m_distBlocks[m_src]->color = QColor(170, 220, 245);
            m_distBlocks[m_src]->update();
        }
        if (m_ptrU) m_ptrU->setOpacity(0);
        if (m_heapText) setScriptText(m_heapText, "");
    } else if (frame.type == ActionType::DH_PUSH) {
        int slot = frame.u;
        int dist = frame.val1;
        int node = frame.val2;
        shiftPqFrom(slot);
        ArrayBlock* block = nullptr;
        if (!m_pqSpare.empty()) {
            block = m_pqSpare.back();
            m_pqSpare.pop_back();
        }
        if (!block) return;
        block->setCustomValue(QString("(%1,%2)").arg(dist).arg(node));
        block->showValue = true;
        block->color = kDhHeap;
        block->setOpacity(1.0);
        block->setScale(1.0);
        block->setPos(m_pqX + slot * (m_pqCellW + 10.0), m_pqY);
        block->update();
        m_pqOrder.insert(m_pqOrder.begin() + slot, block);
        if (m_animated) playScalePulse(block, 1.22, 280);
        if (m_heapText) {
            setScriptText(m_heapText, QString("入队位置 %1　pq 大小 %2　新元素 { %3, %4 }")
                .arg(slot + 1).arg(m_pqOrder.size()).arg(dist).arg(node));
        }
    } else if (frame.type == ActionType::DH_POP) {
        int node = frame.v;
        int dist = frame.val1;
        ArrayBlock* removed = nullptr;
        if (!m_pqOrder.empty()) {
            removed = m_pqOrder.front();
            m_pqOrder.erase(m_pqOrder.begin());
        }
        std::vector<QGraphicsItem*> rest;
        for (ArrayBlock* b : m_pqOrder) {
            if (b) rest.push_back(b);
        }
        playRowShift(rest, QPointF(-(m_pqCellW + 10.0), 0.0), 420);
        if (removed) {
            QPointer<ArrayBlock> safe = removed;
            if (m_animated) {
                QVariantAnimation* anim = new QVariantAnimation(safe);
                anim->setDuration(360);
                anim->setStartValue(1.0);
                anim->setEndValue(0.0);
                anim->setEasingCurve(QEasingCurve::InCubic);
                QObject::connect(anim, &QVariantAnimation::valueChanged, [safe](const QVariant& value) {
                    if (!safe) return;
                    safe->setOpacity(value.toReal());
                    safe->setScale(0.7 + 0.3 * value.toReal());
                });
                QObject::connect(anim, &QVariantAnimation::finished, [this, safe]() {
                    if (!safe) return;
                    safe->setOpacity(0);
                    safe->setScale(1.0);
                    safe->showValue = false;
                    safe->customValue.clear();
                    safe->color = kDhPale;
                    safe->update();
                    m_pqSpare.push_back(safe.data());
                });
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            } else {
                removed->setOpacity(0);
                removed->showValue = false;
                removed->customValue.clear();
                removed->color = kDhPale;
                removed->update();
                m_pqSpare.push_back(removed);
            }
        }
        if (m_ptrU) {
            m_ptrU->setOpacity(1);
            if (m_nodes.count(node)) {
                retargetPointerToNode(m_ptrU, m_nodes[node]);
            }
        }
        if (m_heapText) {
            setScriptText(m_heapText, QString("弹出队首 { %1, %2 }，剩余 %3 个").arg(dist).arg(node).arg(m_pqOrder.size()));
        }
    } else if (frame.type == ActionType::DH_SKIP) {
        int node = frame.u;
        if (node > 0 && m_nodes.count(node)) {
            m_nodes[node]->color = kDhStale;
            m_nodes[node]->update();
            playNodePop(m_nodes[node], 0.93, 260);
        }
        if (m_heapText) {
            setScriptText(m_heapText, QString("过期条目：%1 > dis[%2]，跳过").arg(frame.val1).arg(node));
        }
    } else if (frame.type == ActionType::DH_RELAX) {
        int u = frame.u;
        int v = frame.v;
        bool isUpdate = frame.group.size() > 1 && frame.group[1] == 1;
        if (frame.group.size() > 0 && frame.group[0] == 2) {
            if (u > 0 && m_nodes.count(u)) {
                m_nodes[u]->color = QColor(150, 220, 190);
                m_nodes[u]->update();
            }
            if (v > 0 && m_distBlocks.count(v)) {
                m_distBlocks[v]->setCustomValue(QString::number(frame.val1));
                m_distBlocks[v]->color = QColor(170, 220, 245);
                m_distBlocks[v]->update();
            }
            if (isUpdate) {
                auto it = m_edges.find({u, v});
                if (it == m_edges.end()) it = m_edges.find({v, u});
                if (it != m_edges.end()) {
                    it->second->setColor(QColor(70, 175, 125));
                    it->second->update();
                    if (v > 0 && m_nodes.count(v)) {
                        playEdgeDiscover(it->second, m_nodes[v], QColor(70, 175, 125), 520);
                    } else {
                        playEdgeTravel(it->second, QColor(70, 175, 125), 520);
                    }
                }
                if (v > 0 && m_nodes.count(v)) {
                    m_nodes[v]->color = QColor(155, 220, 245);
                    m_nodes[v]->update();
                }
                if (m_animated && m_nodes.count(v)) {
                    playRipple(m_nodes[v]->pos(), QColor(110, 200, 150), 50.0, 560);
                }
                if (v > 0 && m_distBlocks.count(v)) {
                    playValueFlight(m_nodes[u] ? m_nodes[u]->pos() : QPointF(470, 170),
                                    m_distBlocks[v], frame.val1, QColor(170, 220, 245), true);
                }
            } else if (v > 0 && m_nodes.count(v)) {
                playNodePop(m_nodes[v], 1.16, 300);
            }
            if (m_heapText) {
                setScriptText(m_heapText, isUpdate
                    ? QString("松弛 %1 → %2：dis[%2] = %3").arg(u).arg(v).arg(frame.val1)
                    : QString("确定结点 %1：dis[%1] = %2").arg(u).arg(frame.val1));
            }
        }
    } else if (frame.type == ActionType::DH_FINISH) {
        if (m_ptrU) m_ptrU->setOpacity(0);
        if (m_heapText) setScriptText(m_heapText, "堆优化 Dijkstra 完成");
    }
}

void DijkstraHeapModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

void DijkstraHeapModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_pqBlocks.clear();
    m_pqOrder.clear();
    m_pqSpare.clear();
    m_edgesInput.clear();
    m_mathText = nullptr;
    m_heapText = nullptr;
    m_pqTitle = nullptr;
    m_ptrU = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_heapCapacity = 0;
    m_valid = false;
}

void DijkstraHeapModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_distBlocks.clear();
    m_pqBlocks.clear();
    m_pqOrder.clear();
    m_pqSpare.clear();
    m_mathText = nullptr;
    m_heapText = nullptr;
    m_pqTitle = nullptr;
    m_ptrU = nullptr;
}
