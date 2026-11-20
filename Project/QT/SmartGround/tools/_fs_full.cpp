#include "ForwardStarModule.h"
#include "GraphRenderHelper.h"
#include <QFont>
#include <algorithm>

static const QColor kFsPale = QColor(252, 252, 254);
static const QColor kFsEdge = QColor(70, 165, 195);
static const QColor kFsNext = QColor(160, 110, 220);
static const QColor kFsCur = QColor(246, 176, 120);
static const QColor kFsHead = QColor(255, 214, 140);

ForwardStarModule::ForwardStarModule() {}

QString ForwardStarModule::getModuleName() const { return "é¾å¼ååæ"; }
QString ForwardStarModule::getDefaultInput() const { return "5 6 1\n1 2 3\n1 3 5\n2 4 2\n3 4 1\n4 5 4\n2 5 6"; }

QString ForwardStarModule::getInputHint() const {
    return "ç¬¬ä¸è¡è¾å¥ä¸ä¸ªæ­£æ´æ°ï¼é¡¶ç¹æ° nãè¾¹æ° mãæºç¹ src\n"
           "æ¥ä¸æ¥ m è¡ï¼æ¯è¡ä¸ä¸ªæ´æ° u, v, wï¼è¡¨ç¤ºä¸æ¡æå¼ä¸º w çè¾¹\n"
           "add(u, v, w) ä¼ææ°è¾¹æå° u çé¾è¡¨å¤´é¨ï¼to[++tot]=vï¼nxt[tot]=head[u]ï¼head[u]=tot";
}

std::map<int, QPointF> ForwardStarModule::captureNodePositions() const {
    return GraphRenderHelper::captureNodePositions(m_nodes);
}

void ForwardStarModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    GraphRenderHelper::restoreNodePositions(m_nodes, positions);
}

void ForwardStarModule::layoutArrays(int nodeCount, int edgeCapacity) {
    const double cellW = 52.0;
    const double cellH = 34.0;
    const double colX = 40.0;
    const double rowGap = cellH + 4.0;

    auto addColumn = [&](const QString& title, double x, double y, int count) {
        QGraphicsTextItem* label = m_scene->addText(title);
        label->setFont(QFont("Consolas", 12, QFont::Bold));
        label->setDefaultTextColor(QColor(40, 70, 110));
        label->setPos(x - 12, y - 26);
    };

    addColumn("head[u]", colX, 78, nodeCount);
    addColumn("to[k]", colX + 92, 78, edgeCapacity);
    addColumn("nxt[k]", colX + 184, 78, edgeCapacity);
    addColumn("weight[k]", colX + 276, 78, edgeCapacity);

    for (int u = 1; u <= nodeCount; ++u) {
        ArrayBlock* block = new ArrayBlock(u, 0, QString::number(u));
        block->setCellSize(cellW, cellH);
        block->setShowLabel(false);
        block->setPos(colX, 78 + (u - 1) * rowGap);
        block->color = kFsPale;
        m_scene->addItem(block);
        m_headBlocks[u] = block;
    }
    for (int k = 1; k <= edgeCapacity; ++k) {
        ArrayBlock* to = new ArrayBlock(k, 0, QString::number(k));
        to->setCellSize(cellW, cellH);
        to->setShowLabel(false);
        to->setPos(colX + 92, 78 + (k - 1) * rowGap);
        to->color = kFsPale;
        m_scene->addItem(to);
        m_toBlocks[k] = to;

        ArrayBlock* nxt = new ArrayBlock(k, 0, QString::number(k));
        nxt->setCellSize(cellW, cellH);
        nxt->setShowLabel(false);
        nxt->setPos(colX + 184, 78 + (k - 1) * rowGap);
        nxt->color = kFsPale;
        m_scene->addItem(nxt);
        m_nxtBlocks[k] = nxt;

        ArrayBlock* weight = new ArrayBlock(k, 0, QString::number(k));
        weight->setCellSize(cellW, cellH);
        weight->setShowLabel(false);
        weight->setPos(colX + 276, 78 + (k - 1) * rowGap);
        weight->color = kFsPale;
        m_scene->addItem(weight);
        m_weightBlocks[k] = weight;
    }
}

void ForwardStarModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    GraphModuleUtils::GraphInput input;
    m_valid = GraphModuleUtils::parse(inputData, true, m_options, input);
    m_mathText = scene->addText(m_valid ? "é¾å¼ååæï¼head / to / nxt / weight ååéææ°ç»è¿åæåå¾" : "è¾å¥æ ¼å¼ï¼n m srcï¼éåè¾å¥ m æ¡è¾¹ u v w");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(40, 18);
    if (!m_valid) return;

    m_n = input.n;
    m_m = input.m;
    m_src = input.src;
    m_capacity = std::max(1, m_m * (m_options.directed ? 1 : 2) + 1);

    layoutArrays(m_n, m_capacity);

    GraphRenderHelper::buildNodes(scene, m_nodes, m_n, QPointF(760, 300), m_n <= 8 ? 130 : 150, this);

    m_ptrK = new PointerBlock("k", QColor(210, 80, 55));
    m_ptrK->setOpacity(0);
    scene->addItem(m_ptrK);
    m_ptrHead = new PointerBlock("head[u]", QColor(45, 130, 210));
    m_ptrHead->setOpacity(0);
    scene->addItem(m_ptrHead);
}

std::vector<AnimFrame> ForwardStarModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<GraphModuleUtils::EdgeInput> edges;
    for (const auto& edge : GraphModuleUtils::GraphInput().edges) {
        Q_UNUSED(edge);
    }
    m_frames.push_back({ActionType::FS_RESET, 0, 0, 0, 0, {}, "åå§åï¼tot = 0ï¼head[] å¨ä¸º 0", "head[1..n] = 0"});

    int tot = 0;
    int index = 0;
    auto addEdge = [&](int u, int v, int w, int kind) {
        ++tot;
        m_frames.push_back({ActionType::FS_ADD, tot, u, v, w, {kind},
                            QString("add(%1, %2, %3)ï¼æå¥ç¬¬ %4 æ¡è¾¹").arg(u).arg(v).arg(w).arg(tot),
                            QString("to[%1] = %2").arg(tot).arg(v)});
        m_frames.push_back({ActionType::FS_LINK, tot, v, v, w, {kind},
                            QString("to[%1] = %2ï¼è¿åç»ç¹ %2").arg(tot).arg(v),
                            QString("to[%1] = %2").arg(tot).arg(v)});
        m_frames.push_back({ActionType::FS_HEAD, tot, u, 0, 0, {kind},
                            QString("nxt[%1] = head[%2]ï¼head[%2] = %1ï¼å¤´æï¼").arg(tot).arg(u),
                            QString("nxt[%1] = head[%2]ãhead[%2] = %1").arg(tot).arg(u)});
        m_frames.push_back({ActionType::FS_ADD, 0, 0, 0, 0, {kind},
                            QString("weight[%1] = %2").arg(tot).arg(w),
                            QString("weight[%1] = %2").arg(tot).arg(w)});
    };

    QStringList tokens = m_builtInputTokens;
    for (int i = 0; i < m_m; ++i) {
        int base = 1 + i * 3;
        if (base + 2 >= static_cast<int>(tokens.size())) break;
        bool ok = false;
        int u = tokens[base].toInt(&ok);
        if (!ok) continue;
        int v = tokens[base + 1].toInt(&ok);
        if (!ok) continue;
        int w = tokens[base + 2].toInt(&ok);
        if (!ok) continue;
        addEdge(u, v, w, 0);
        if (!m_options.directed) addEdge(v, u, w, 1);
        ++index;
    }

    m_frames.push_back({ActionType::FS_DONE, tot, 0, 0, 0, {}, QString("å±æå¥ %1 æ¡è¾¹").arg(tot), QString("tot = %1").arg(tot)});

    m_frames.push_back({ActionType::FS_TRAVERSE, m_src, 0, 0, 0, {}, QString("ä»ç»ç¹ %1 åºåéåé»æ¥è¡¨").arg(m_src), QString("for (i = head[%1]; i; i = nxt[i])").arg(m_src)});
    std::vector<int> head(m_n + 1, 0);
    std::vector<int> to(tot + 1, 0);
    std::vector<int> nxt(tot + 1, 0);
    for (int i = 0; i < m_m; ++i) {
        int base = 1 + i * 3;
        if (base + 2 >= static_cast<int>(tokens.size())) break;
        bool ok = false;
        int u = tokens[base].toInt(&ok);
        if (!ok) continue;
        int v = tokens[base + 1].toInt(&ok);
        if (!ok) continue;
        ++index;
        int slot = (tot >= 1) ? tot - index + 1 : 0;
        if (slot < 1 || slot > tot) continue;
        to[slot] = v;
        nxt[slot] = head[u];
        head[u] = slot;
        if (!m_options.directed) {
            int slot2 = slot - 1;
            if (slot2 < 1) continue;
            to[slot2] = u;
            nxt[slot2] = head[v];
            head[v] = slot2;
        }
    }
    for (int u = m_src; u <= m_n; ++u) {
        int i = head[u];
        if (i == 0) continue;
        m_frames.push_back({ActionType::FS_TRAVERSE, u, i, to[i], 0, {},
                            QString("head[%1] = %2ï¼è®¿é®å°ç»ç¹ %3").arg(u).arg(i).arg(to[i]),
                            QString("i = head[%1] = %2ãto[%2] = %3").arg(u).arg(i).arg(to[i])});
        int guard = 0;
        while (i != 0 && guard++ < 64) {
            i = nxt[i];
            if (i == 0) {
                m_frames.push_back({ActionType::FS_TRAVERSE, u, 0, 0, 0, {},
                                    QString("nxt é¾èµ°å° 0ï¼%1 çé»æ¥è¡¨ç»æ").arg(u),
                                    QString("i = 0ï¼éåç»æ")});
                break;
            }
            m_frames.push_back({ActionType::FS_TRAVERSE, u, i, to[i], 0, {},
                                QString("æ²¿ nxt è·³å° %1ï¼è®¿é®å°ç»ç¹ %2").arg(i).arg(to[i]),
                                QString("i = nxt[%1] = %2ãto[%2] = %3").arg(i).arg(i).arg(to[i])});
        }
    }
    return m_frames;
}

void ForwardStarModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    auto resetArrays = [&]() {
        for (auto& kv : m_headBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_toBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_nxtBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        for (auto& kv : m_weightBlocks) { kv.second->color = kFsPale; kv.second->update(); }
    };

    if (frame.type == ActionType::FS_RESET) {
        resetArrays();
        for (auto& kv : m_toLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
        m_toLines.clear();
        for (auto& kv : m_nxtLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
        m_nxtLines.clear();
        for (auto& kv : m_nodes) { kv.second->color = Qt::white; kv.second->update(); }
        if (m_ptrK) m_ptrK->setOpacity(0);
        if (m_ptrHead) m_ptrHead->setOpacity(0);
    } else if (frame.type == ActionType::FS_ADD) {
        int k = frame.u;
        if (k > 0) {
            if (m_toBlocks.count(k)) {
                m_toBlocks[k]->value = frame.val1;
                m_toBlocks[k]->customValue.clear();
                m_toBlocks[k]->showValue = true;
                m_toBlocks[k]->color = kFsCur;
                m_toBlocks[k]->update();
            }
            if (m_ptrK) {
                m_ptrK->setOpacity(1);
                m_ptrK->setPos(40 + 92 + 26, 78 + (k - 1) * 38 - 14);
            }
        }
    } else if (frame.type == ActionType::FS_LINK) {
        int k = frame.u;
        int v = frame.v;
        if (k > 0 && m_toBlocks.count(k) && m_nodes.count(v)) {
            ArrayBlock* toBlock = m_toBlocks[k];
            GraphNode* target = m_nodes[v];
            LinkLine* line = new LinkLine(toBlock->pos() + QPointF(toBlock->cellWidth(), toBlock->cellHeight() / 2.0),
                                          target->pos());
            line->setStyle(kFsEdge, 2.2, false, true);
            line->setBend(18.0);
            m_scene->addItem(line);
            m_toLines[k] = line;
            target->color = QColor(190, 232, 240);
            target->update();
        }
    } else if (frame.type == ActionType::FS_HEAD) {
        int k = frame.u;
        int u = frame.v;
        if (k > 0 && m_headBlocks.count(u) && m_nxtBlocks.count(k)) {
            m_headBlocks[u]->value = k;
            m_headBlocks[u]->customValue.clear();
            m_headBlocks[u]->showValue = true;
            m_headBlocks[u]->color = kFsHead;
            m_headBlocks[u]->update();
            m_nxtBlocks[k]->value = m_headBlocks[u]->value;
            m_nxtBlocks[k]->customValue.clear();
            m_nxtBlocks[k]->showValue = true;
            m_nxtBlocks[k]->color = kFsNext;
            m_nxtBlocks[k]->update();
            if (m_ptrHead) {
                m_ptrHead->setOpacity(1);
                m_ptrHead->setPos(40 + 26, 78 + (u - 1) * 38 - 14);
            }
            LinkLine* line = new LinkLine(m_nxtBlocks[k]->pos() + QPointF(0, m_nxtBlocks[k]->cellHeight() / 2.0),
                                          m_headBlocks[u]->pos() + QPointF(m_headBlocks[u]->cellWidth(), m_headBlocks[u]->cellHeight() / 2.0));
            line->setStyle(kFsNext, 1.8, true, true);
            line->setBend(-14.0);
            m_scene->addItem(line);
            m_nxtLines[k] = line;
        }
    } else if (frame.type == ActionType::FS_TRAVERSE) {
        int u = frame.u;
        int k = frame.v;
        for (auto& kv : m_headBlocks) {
            kv.second->color = kFsPale;
            kv.second->update();
        }
        for (auto& kv : m_toBlocks) { kv.second->color = kFsPale; kv.second->update(); }
        if (u > 0 && m_headBlocks.count(u)) {
            m_headBlocks[u]->color = kFsHead;
            m_headBlocks[u]->update();
        }
        if (k > 0) {
            if (m_toBlocks.count(k)) {
                m_toBlocks[k]->color = kFsCur;
                m_toBlocks[k]->update();
            }
            if (m_ptrK) {
                m_ptrK->setOpacity(1);
                m_ptrK->setPos(40 + 92 + 26, 78 + (k - 1) * 38 - 14);
            }
            auto it = m_toLines.find(k);
            if (it != m_toLines.end() && it->second) {
                it->second->setStyle(kFsCur, 3.0, false, true);
            }
        }
    } else if (frame.type == ActionType::FS_DONE) {
        resetArrays();
        if (m_ptrK) m_ptrK->setOpacity(0);
        if (m_ptrHead) m_ptrHead->setOpacity(0);
    }
}

void ForwardStarModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

void ForwardStarModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_headBlocks.clear();
    m_toBlocks.clear();
    m_nxtBlocks.clear();
    m_weightBlocks.clear();
    m_toLines.clear();
    m_nxtLines.clear();
    m_mathText = nullptr;
    m_ptrK = nullptr;
    m_ptrHead = nullptr;
    m_n = 0;
    m_m = 0;
    m_src = 1;
    m_capacity = 0;
    m_valid = false;
}

void ForwardStarModule::releaseSceneItems() {
    for (auto& kv : m_toLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
    m_toLines.clear();
    for (auto& kv : m_nxtLines) { if (kv.second) { m_scene->removeItem(kv.second); delete kv.second; } }
    m_nxtLines.clear();
    m_nodes.clear();
    m_headBlocks.clear();
    m_toBlocks.clear();
    m_nxtBlocks.clear();
    m_weightBlocks.clear();
    m_mathText = nullptr;
    m_ptrK = nullptr;
    m_ptrHead = nullptr;
}
