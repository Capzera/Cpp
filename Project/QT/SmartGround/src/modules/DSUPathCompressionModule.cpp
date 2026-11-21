#include "DSUPathCompressionModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kDpPale = QColor(252, 252, 254);
static const QColor kDpPath = QColor(246, 176, 120);
static const QColor kDpRoot = QColor(150, 214, 170);
static const QColor kDpFaint = QColor(214, 218, 226);
static const QColor kDpGold = QColor(255, 214, 140);
static const QColor kDpNoCompress = QColor(236, 176, 168);
static const QColor kDpSize = QColor(255, 205, 125);
static const QColor kDpHeight = QColor(124, 107, 214);
static const QColor kDpOther = QColor(155, 210, 245);

DSUPathCompressionModule::DSUPathCompressionModule() {}

QString DSUPathCompressionModule::getModuleName() const { return "并查集"; }
QString DSUPathCompressionModule::getDefaultInput() const { return "8\n1 2\n3 4\n1 3\n4 5\n5 6\n6 7\n7 8"; }

QString DSUPathCompressionModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示集合中元素的个数\n"
           "接下来 m 行，每行两个整数 u, v，表示一次 merge(u, v) 操作\n"
           "例如「3 4」表示把 3 所在集合与 4 所在集合合并\n"
           "模块按标准并查集模板演示：\n"
           "  初始化：pa[i] = i（每个点父亲是自己），sz[i] = 1\n"
           "  merge(u, v)：按大小合并，pa[v] = u，sz[u] += sz[v]\n"
           "  find(x)：沿父指针上行找根\n"
           "在「并查集选项」里切换路径压缩模式：\n"
           "  有路径压缩：find 回溯时把沿途结点一次性重定向到根，整棵树被压扁\n"
           "  无路径压缩：find 只找到根就返回，父指针原样保留，树高不会下降";
}

QString DSUPathCompressionModule::moduleSettingsTitle() const { return "并查集选项"; }

QVector<ModuleField> DSUPathCompressionModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeSegment("compress", "路径压缩",
                                            {"有路径压缩", "无路径压缩"}, m_compressMode));
    return fields;
}

void DSUPathCompressionModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "compress") m_compressMode = field.intValue;
}

int DSUPathCompressionModule::forestHeight(const std::vector<int> &pa) const {
    int maxHeight = 0;
    for (int i = 1; i <= static_cast<int>(pa.size()) - 1; ++i) {
        int depth = 0;
        int x = i;
        int guard = 0;
        while (pa[x] != x && guard++ < 64) {
            x = pa[x];
            ++depth;
        }
        if (depth > maxHeight) maxHeight = depth;
    }
    return maxHeight;
}

void DSUPathCompressionModule::resetForestColors() {
    for (auto &kv : m_nodes) {
        kv.second->color = kDpPale;
        kv.second->setHighlightColor(Qt::transparent);
        kv.second->update();
    }
    for (auto &kv : m_edges) {
        kv.second->color = QColor(180, 188, 202);
        kv.second->setPenWidth(2.0);
        kv.second->update();
    }
}

GraphEdge* DSUPathCompressionModule::edgeBetween(int a, int b) const {
    auto it = m_edges.find({a, b});
    if (it != m_edges.end()) return it->second;
    it = m_edges.find({b, a});
    if (it != m_edges.end()) return it->second;
    for (const auto &kv : m_edges) {
        GraphEdge* candidate = kv.second;
        if (!candidate) continue;
        const int u = candidate->source ? candidate->source->id : 0;
        const int v = candidate->dest ? candidate->dest->id : 0;
        if ((u == a && v == b) || (u == b && v == a)) return candidate;
    }
    return nullptr;
}

void DSUPathCompressionModule::paintEdgeTo(int node, int parent, const QColor &color, qreal width) {
    if (!m_nodes.count(node) || !m_nodes.count(parent)) return;
    GraphEdge* edge = edgeBetween(node, parent);
    if (!edge) return;
    edge->color = color;
    edge->setPenWidth(width);
    edge->update();
}

void DSUPathCompressionModule::layoutForest() {
    if (m_n <= 0) return;
    const int columns = std::min(m_n, 5);
    const int rows = (m_n + columns - 1) / columns;
    for (int i = 1; i <= m_n; ++i) {
        auto it = m_nodes.find(i);
        if (it == m_nodes.end()) continue;
        const int col = (i - 1) % columns;
        const int row = (i - 1) / columns;
        it->second->setPos(190 + col * 145, 175 + row * 118);
    }
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
    if (n <= 0) return;

    m_n = n;
    m_parents.assign(n + 1, 0);
    m_sizes.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) m_parents[i] = i;

    for (int i = 1; i < items.size(); i += 2) {
        int u = items[i].toInt();
        int v = (i + 1 < items.size()) ? items[i + 1].toInt() : u;
        if (u < 1 || u > n || v < 1 || v > n) continue;
        m_mergeOps.push_back({u, v});
    }

    for (int i = 1; i <= n; ++i) {
        GraphNode* node = new GraphNode(i);
        node->setCustomValueText(QString::number(i));
        m_scene->addItem(node);
        m_nodes[i] = node;
        registerNodeSlot(i, "v", i);
    }
    layoutForest();

    m_mathText = createScriptText(scene, m_compressMode == 0
        ? QString("并查集 · 有路径压缩：find 沿父指针上行，回溯时一次性把路径压扁到根")
        : QString("并查集 · 无路径压缩：find 只找到根就返回，父指针原样保留，树高不会下降"));
    m_mathText->setFont(UiFonts::mono(14, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 24);

    m_codeText = createScriptText(scene, "");
    m_codeText->setFont(UiFonts::mono(13, true));
    m_codeText->setDefaultTextColor(QColor(60, 80, 110));
    m_codeText->setPos(60, 480);

    m_heightText = createScriptText(scene, "");
    m_heightText->setFont(UiFonts::cjk(12, true));
    m_heightText->setDefaultTextColor(kDpHeight.darker(115));
    m_heightText->setPos(60, 640);

    QGraphicsTextItem* paTitle = scene->addText("pa[] 父指针");
    paTitle->setFont(UiFonts::cjk(11, true));
    paTitle->setDefaultTextColor(QColor(120, 130, 145));
    paTitle->setPos(540, 440);

    QGraphicsTextItem* szTitle = scene->addText("sz[] 集合大小");
    szTitle->setFont(UiFonts::cjk(11, true));
    szTitle->setDefaultTextColor(QColor(120, 130, 145));
    szTitle->setPos(540, 540);
    for (int i = 1; i <= n; ++i) {
        ArrayBlock* pa = new ArrayBlock(i, 0, QString::number(i));
        pa->setCellSize(56, 40);
        pa->setPos(720 + (i - 1) * 58, 470);
        pa->color = kDpPale;
        scene->addItem(pa);
        m_paBlocks[i] = pa;
        registerArraySlot(pa, "pa", i);

        ArrayBlock* sz = new ArrayBlock(i, 0, QString::number(i));
        sz->setCellSize(56, 40);
        sz->setPos(720 + (i - 1) * 58, 570);
        sz->color = kDpPale;
        scene->addItem(sz);
        m_szBlocks[i] = sz;
        registerArraySlot(sz, "sz", i);
    }

    m_ptrX = new PointerBlock("x", QColor(210, 80, 55));
    m_ptrX->setOpacity(0);
    scene->addItem(m_ptrX);

    m_valid = true;
}

std::vector<AnimFrame> DSUPathCompressionModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<int> pa(m_n + 1, 0);
    std::vector<int> sz(m_n + 1, 1);
    for (int i = 1; i <= m_n; ++i) {
        pa[i] = i;
        sz[i] = 1;
        int override = liveValue("pa", i, -1);
        if (override >= 1 && override <= m_n) pa[i] = override;
    }

    m_frames.push_back({ActionType::DSU_INIT, 0, 0, m_n, 0, {},
                        QString("初始化：每个点自成一个集合，父亲指向自己"),
                        QString("pa[i] = i　sz[i] = 1")});

    const bool compress = (m_compressMode == 0);

    auto walkFind = [&](int start, const char* who) {
        std::vector<int> path;
        int x = start;
        int guard = 0;
        while (pa[x] != x && guard++ < 64) {
            path.push_back(x);
            x = pa[x];
            m_frames.push_back({ActionType::DSU_FIND_STEP, x, 0, pa[x], 0, {path.begin(), path.end()},
                                QString("find(%1)：沿父指针走到 %2").arg(start).arg(x),
                                QString("pa[%1] = %2").arg(x).arg(pa[x])});
        }
        path.push_back(x);
        m_frames.push_back({ActionType::DSU_FIND_ROOT, x, start, 1, 0, {path.begin(), path.end()},
                            QString("find(%1) 的根是 %2").arg(start).arg(x),
                            QString("root(%1) = %2").arg(start).arg(x)});
        if (compress) {
            for (size_t p = 0; p + 1 < path.size(); ++p) {
                pa[path[p]] = x;
                m_frames.push_back({ActionType::DSU_COMPRESS_STEP, path[p], x, 0, 0, {path.begin(), path.end()},
                                    QString("路径压缩：pa[%1] = %2（直接指向根）").arg(path[p]).arg(x),
                                    QString("pa[%1] = %2").arg(path[p]).arg(x)});
            }
        } else {
            m_frames.push_back({ActionType::DP_NO_COMPRESS, x, 0, 0, 0, {path.begin(), path.end()},
                                QString("无路径压缩：find(%1) 直接返回根 %2，父指针原样保留").arg(start).arg(x),
                                QString("不做回溯重定向，路径长度不变")});
        }
        const int height = forestHeight(pa);
        m_frames.push_back({ActionType::DP_HEIGHT, height, 0, 0, 0, {},
                            compress ? QString("路径压缩后，当前森林最大深度 = %1").arg(height)
                                     : QString("无路径压缩，当前森林最大深度仍为 %1").arg(height),
                            QString("当前森林的最大深度 = %1").arg(height)});
        Q_UNUSED(who);
        return x;
    };

    for (size_t op = 0; op < m_mergeOps.size(); ++op) {
        int u = m_mergeOps[op].first;
        int v = m_mergeOps[op].second;
        const int opIndex = static_cast<int>(op) + 1;

        m_frames.push_back({ActionType::DSU_MERGE_START, u, v, opIndex, 0, {},
                            QString("第 %1 次 merge(%2, %3)：先 find 两个点的根").arg(opIndex).arg(u).arg(v),
                            QString("merge(%1, %2)").arg(u).arg(v)});

        const int rootU = walkFind(u, "u");
        const int rootV = walkFind(v, "v");

        if (rootU == rootV) {
            m_frames.push_back({ActionType::DSU_PREDICATE, u, v, rootU, 0, {},
                                QString("两个点的根相同，说明已经在同一集合里，什么都不做"),
                                QString("find(%1) == find(%2) == %3").arg(u).arg(v).arg(rootU)});
            continue;
        }

        int child = rootV;
        int parent = rootU;
        if (sz[rootU] < sz[rootV]) {
            std::swap(parent, child);
        }
        m_frames.push_back({ActionType::DSU_MERGE_LINK, child, parent, sz[parent], sz[child], {},
                            QString("按大小合并：把 %1 挂到 %2 下（sz[%3] = %4 ≥ sz[%5] = %6）")
                                .arg(child).arg(parent).arg(parent).arg(sz[parent]).arg(child).arg(sz[child]),
                            QString("pa[%1] = %2").arg(child).arg(parent)});

        pa[child] = parent;
        sz[parent] += sz[child];

        m_frames.push_back({ActionType::DSU_MERGE_SIZE, parent, child, sz[parent], sz[child], {},
                            QString("集合大小累加：sz[%1] = %2 + %3 = %4")
                                .arg(parent).arg(sz[parent] - sz[child]).arg(sz[child]).arg(sz[parent]),
                            QString("sz[%1] += sz[%2] ⇒ %3").arg(parent).arg(child).arg(sz[parent])});

        const int height = forestHeight(pa);
        m_frames.push_back({ActionType::DP_HEIGHT, height, 0, 0, 0, {},
                            QString("当前森林最大深度 = %1").arg(height),
                            QString("树高 = %1").arg(height)});
    }

    m_frames.push_back({ActionType::DSU_DONE, 0, 0, 0, 0, {},
                        QString("所有 merge 结束，并查集维护完毕"),
                        QString("pa[] 与 sz[] 即为最终状态")});
    return m_frames;
}

void DSUPathCompressionModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::DSU_INIT) {
        if (frame.val2 == 1) {
            if (m_heightText) {
                setScriptText(m_heightText, QString("森林最大深度 = %1").arg(frame.val1));
                m_heightText->setDefaultTextColor(kDpHeight.darker(115));
            }
            if (m_animated && frame.val1 > 0) playScalePulse(m_heightText, 1.15, 260);
            return;
        }
        resetForestColors();        for (int i = 1; i <= m_n; ++i) {
            if (m_paBlocks.count(i)) {
                m_paBlocks[i]->setCustomValue(QString::number(i));
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
            auto nodeIt = m_nodes.find(i);
            if (nodeIt != m_nodes.end()) {
                nodeIt->second->setSubLabel("pa=" + QString::number(i));
                nodeIt->second->update();
            }
        }
        if (m_ptrX) m_ptrX->setOpacity(0);
        if (m_codeText) {
            setScriptText(m_codeText, "初始化：pa[i] = i（父亲是自己），sz[i] = 1（集合只有自己）");
            m_codeText->setDefaultTextColor(QColor(60, 80, 110));
        }
    } else if (frame.type == ActionType::DSU_MERGE_START) {
        resetForestColors();
        const int u = frame.u, v = frame.v;
        if (m_nodes.count(u)) {
            m_nodes[u]->color = kDpPath;
            m_nodes[u]->setHighlightColor(kDpPath);
            m_nodes[u]->update();
        }
        if (m_nodes.count(v)) {
            m_nodes[v]->color = kDpOther;
            m_nodes[v]->setHighlightColor(kDpOther);
            m_nodes[v]->update();
        }
        if (m_ptrX) {
            m_ptrX->setOpacity(1);
            if (m_nodes.count(u)) retargetPointerToNode(m_ptrX, m_nodes[u]);
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("merge(%1, %2)：u = find(%1)，v = find(%2)").arg(u).arg(v));
            m_codeText->setDefaultTextColor(QColor(60, 80, 110));
        }
    } else if (frame.type == ActionType::DSU_FIND_STEP) {
        const int x = frame.u;
        for (int node : frame.group) {
            auto it = m_nodes.find(node);
            if (it == m_nodes.end()) continue;
            it->second->color = kDpPath;
            it->second->setHighlightColor(kDpPath);
            it->second->update();
            if (m_animated) playNodeRipple(it->second, kDpPath, 30.0, 420);
        }
        for (size_t k = 1; k < frame.group.size(); ++k) {
            paintEdgeTo(frame.group[k - 1], frame.group[k], kDpPath, 3.0);
        }
        if (m_ptrX) {
            m_ptrX->setOpacity(1);
            auto it = m_nodes.find(x);
            if (it != m_nodes.end()) retargetPointerToNode(m_ptrX, it->second);
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("find：x = %1，沿 pa 向上走（pa[%1] = %2）").arg(x).arg(frame.val1));
            m_codeText->setDefaultTextColor(QColor(60, 80, 110));
        }
    } else if (frame.type == ActionType::DSU_FIND_ROOT) {
        const int root = frame.u;
        auto it = m_nodes.find(root);
        if (it != m_nodes.end()) {
            it->second->color = kDpRoot;
            it->second->setHighlightColor(kDpRoot);
            it->second->update();
            if (m_animated) {
                playScalePulse(it->second, 1.25, 260);
                playNodeRipple(it->second, kDpRoot, 46.0, 600);
            }
        }
        for (size_t k = 1; k < frame.group.size(); ++k) {
            paintEdgeTo(frame.group[k - 1], frame.group[k], kDpRoot, 3.0);
        }
        if (m_ptrX) {
            m_ptrX->setOpacity(1);
            if (it != m_nodes.end()) retargetPointerToNode(m_ptrX, it->second);
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("找到根：root(%1) = %2").arg(frame.v).arg(root));
            m_codeText->setDefaultTextColor(QColor(40, 140, 70));
        }
    } else if (frame.type == ActionType::DSU_COMPRESS_STEP) {
        const int node = frame.u;
        const int root = frame.v;
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
        GraphEdge* redirected = nullptr;
        for (auto &kv : m_edges) {
            GraphEdge* edge = kv.second;
            if (!edge) continue;
            const int u = edge->source ? edge->source->id : 0;
            const int v = edge->dest ? edge->dest->id : 0;
            if ((u == node && v != root) || (v == node && u != root)) {
                edge->repointDest(m_nodes.count(root) ? m_nodes[root] : nullptr);
                redirected = edge;
            }
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
        if (redirected) {
            redirected->color = kDpRoot;
            redirected->setPenWidth(3.4);
            redirected->update();
            playEdgeTravel(redirected, kDpRoot, 480);
        }
        if (m_paBlocks.count(node)) {
            m_paBlocks[node]->setCustomValue(QString::number(root));
            m_paBlocks[node]->color = kDpPath;
            m_paBlocks[node]->update();
            playCellLanding(m_paBlocks[node], kDpPath, 380);
        }
        auto nodeIt = m_nodes.find(node);
        if (nodeIt != m_nodes.end()) {
            nodeIt->second->setSubLabel("pa=" + QString::number(root));
            nodeIt->second->update();
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("路径压缩：pa[%1] = %2，直接指向根").arg(node).arg(root));
            m_codeText->setDefaultTextColor(kDpPath.darker(115));
        }
    } else if (frame.type == ActionType::DSU_MERGE_LINK) {
        const int child = frame.u;
        const int parent = frame.v;
        auto childIt = m_nodes.find(child);
        auto parentIt = m_nodes.find(parent);
        if (childIt != m_nodes.end()) {
            childIt->second->color = kDpSize;
            childIt->second->setHighlightColor(kDpSize);
            childIt->second->update();
            if (m_animated) playNodePop(childIt->second, 1.16, 300);
        }
        if (parentIt != m_nodes.end()) {
            parentIt->second->color = kDpRoot;
            parentIt->second->setHighlightColor(kDpRoot);
            parentIt->second->update();
            if (m_animated) {
                playScalePulse(parentIt->second, 1.25, 280);
                playNodeRipple(parentIt->second, kDpRoot, 46.0, 600);
            }
        }
        GraphEdge* edge = edgeBetween(child, parent);
        if (!edge) {
            edge = new GraphEdge(m_nodes[child], m_nodes[parent], true);
            edge->color = kDpRoot;
            m_scene->addItem(edge);
            m_nodes[child]->addEdge(edge);
            m_nodes[parent]->addEdge(edge);
            m_edges[{child, parent}] = edge;
        } else {
            edge->repointDest(m_nodes[parent]);
            edge->color = kDpRoot;
        }
        edge->setPenWidth(3.4);
        edge->update();
        playEdgeTravel(edge, kDpRoot, 520);
        if (m_paBlocks.count(child)) {
            m_paBlocks[child]->setCustomValue(QString::number(parent));
            m_paBlocks[child]->color = kDpSize;
            m_paBlocks[child]->update();
            playCellLanding(m_paBlocks[child], kDpSize, 400);
        }
        auto childNode = m_nodes.find(child);
        if (childNode != m_nodes.end()) {
            childNode->second->setSubLabel("pa=" + QString::number(parent));
            childNode->second->update();
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("按大小合并：pa[%1] = %2").arg(child).arg(parent));
            m_codeText->setDefaultTextColor(kDpSize.darker(115));
        }
    } else if (frame.type == ActionType::DSU_MERGE_SIZE) {
        const int parent = frame.u;
        const int child = frame.v;
        if (m_szBlocks.count(parent)) {
            m_szBlocks[parent]->value = frame.val1;
            m_szBlocks[parent]->customValue.clear();
            m_szBlocks[parent]->showValue = true;
            m_szBlocks[parent]->color = kDpSize;
            m_szBlocks[parent]->update();
            playCellLanding(m_szBlocks[parent], kDpSize, 400);
        }
        if (m_szBlocks.count(child)) {
            m_szBlocks[child]->color = kDpFaint;
            m_szBlocks[child]->update();
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("集合大小累加：sz[%1] += sz[%2] ⇒ %3")
                               .arg(parent).arg(child).arg(frame.val1));
            m_codeText->setDefaultTextColor(kDpSize.darker(115));
        }
    } else if (frame.type == ActionType::DSU_PREDICATE) {
        const int root = frame.val1;
        auto it = m_nodes.find(root);
        if (it != m_nodes.end()) {
            it->second->color = kDpFaint;
            it->second->setHighlightColor(kDpFaint);
            it->second->update();
            playNodePop(it->second, 0.94, 260);
        }
        if (m_codeText) {
            setScriptText(m_codeText, QString("find(%1) == find(%2) == %3：已在同一集合，merge 直接返回")
                               .arg(frame.u).arg(frame.v).arg(root));
            m_codeText->setDefaultTextColor(QColor(150, 120, 60));
        }
    } else if (frame.type == ActionType::DP_NO_COMPRESS) {
        for (int node : frame.group) {
            auto it = m_nodes.find(node);
            if (it == m_nodes.end()) continue;
            it->second->color = kDpNoCompress;
            it->second->setHighlightColor(kDpNoCompress);
            it->second->update();
            if (m_animated) playNodeRipple(it->second, kDpNoCompress, 30.0, 420);
        }
        for (size_t k = 1; k < frame.group.size(); ++k) {
            paintEdgeTo(frame.group[k - 1], frame.group[k], kDpNoCompress, 3.0);
        }
        if (m_codeText) {
            setScriptText(m_codeText, "无路径压缩：find 找到根就返回，父指针保持原样，路径长度不变");
            m_codeText->setDefaultTextColor(kDpNoCompress.darker(115));
        }
    } else if (frame.type == ActionType::DP_HEIGHT) {
        if (m_heightText) {
            setScriptText(m_heightText, QString("树高标尺：当前森林最大深度 = %1").arg(frame.u));
            m_heightText->setDefaultTextColor(kDpHeight.darker(115));
        }
        if (m_animated && frame.u > 0) playScalePulse(m_heightText, 1.15, 260);
    } else if (frame.type == ActionType::DSU_DONE) {
        for (auto &kv : m_nodes) {
            kv.second->color = kDpGold;
            kv.second->setHighlightColor(kDpGold);
            kv.second->update();
            playNodePop(kv.second, 1.10, 250);
        }
        for (auto &kv : m_edges) {
            kv.second->color = kDpGold;
            kv.second->setPenWidth(2.4);
            kv.second->update();
        }
        if (m_ptrX) m_ptrX->setOpacity(0);
        if (m_codeText) {
            setScriptText(m_codeText, "所有 merge 完成：pa[] 是最终父指针，sz[] 是各集合大小");
            m_codeText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_heightText) {
            if (m_compressMode == 0) {
                setScriptText(m_heightText, "结论：路径压缩把每条查找路径一次性压扁，树高大幅下降，find 代价接近 O(1)");
                m_heightText->setDefaultTextColor(QColor(40, 140, 70));
            } else {
                setScriptText(m_heightText, "结论：没有路径压缩时，同一条链会被反复遍历，查找代价随树高增长");
                m_heightText->setDefaultTextColor(kDpNoCompress.darker(115));
            }
        }
    }
}

void DSUPathCompressionModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}

std::map<int, QPointF> DSUPathCompressionModule::captureNodePositions() const {
    std::map<int, QPointF> result;
    for (const auto& kv : m_nodes) result[kv.first] = kv.second->pos();
    return result;
}

void DSUPathCompressionModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    for (const auto& kv : positions) {
        auto it = m_nodes.find(kv.first);
        if (it != m_nodes.end()) it->second->setPos(kv.second);
    }
}

void DSUPathCompressionModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_paBlocks.clear();
    m_szBlocks.clear();
    m_parents.clear();
    m_sizes.clear();
    m_mergeOps.clear();
    m_mathText = nullptr;
    m_codeText = nullptr;
    m_heightText = nullptr;
    m_ptrX = nullptr;
    m_n = 0;
    m_valid = false;
    m_round = 0;
}

void DSUPathCompressionModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_paBlocks.clear();
    m_szBlocks.clear();
    m_mathText = nullptr;
    m_codeText = nullptr;
    m_heightText = nullptr;
    m_ptrX = nullptr;
}
