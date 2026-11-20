#include "HuffmanTreeModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <functional>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kHfLeaf = QColor(206, 226, 250);
static const QColor kHfSorted = QColor(174, 208, 244);
static const QColor kHfPick = QColor(255, 205, 130);
static const QColor kHfMerge = QColor(196, 230, 200);
static const QColor kHfRoot = QColor(255, 214, 168);
static const QColor kHfEdge = QColor(152, 168, 192);
static const QColor kHfEdgeNew = QColor(224, 138, 40);
static const QColor kHfEmpty = QColor(243, 245, 249);
static const QColor kHfIdle = QColor(250, 250, 253);
static const QColor kHfCode = QColor(212, 234, 250);
static const QColor kHfCodeHot = QColor(255, 214, 150);
static const QColor kHfWpl = QColor(255, 226, 175);
static const QColor kHfText = QColor(20, 30, 45);
static const QColor kHfPath = QColor(214, 96, 32);
static const QColor kHfFrame = QColor(206, 214, 226);

HuffmanTreeModule::HuffmanTreeModule() {}

QString HuffmanTreeModule::getModuleName() const { return "哈夫曼树"; }
QString HuffmanTreeModule::getDefaultInput() const { return "7\n45 13 20 16 9 12 5"; }
QString HuffmanTreeModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示叶子个数（2 <= n <= 8）\n"
           "第二行输入 n 个非负整数，表示每个叶子的权值\n"
           "先对权值数组升序排序，再每次取两个最小权值合并成父节点，父节点权值 = 两者之和";
}

HuffmanNodeData* HuffmanTreeModule::nodeAt(int id) {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return nullptr;
    return &m_nodes[id];
}

const HuffmanNodeData* HuffmanTreeModule::nodeAt(int id) const {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return nullptr;
    return &m_nodes[id];
}

bool HuffmanTreeModule::subtreeHas(int nodeId, int leafId) const {
    const HuffmanNodeData* node = nodeAt(nodeId);
    if (!node) return false;
    if (node->original) return node->id == leafId;
    return subtreeHas(node->left, leafId) || subtreeHas(node->right, leafId);
}

void HuffmanTreeModule::collectSubtree(int id, std::vector<int>& out) const {
    const HuffmanNodeData* node = nodeAt(id);
    if (!node) return;
    out.push_back(id);
    collectSubtree(node->left, out);
    collectSubtree(node->right, out);
}

void HuffmanTreeModule::computeTree() {
    m_plan.clear();
    m_sortedLeaves.clear();
    m_leafCount = static_cast<int>(m_nodes.size());

    for (int i = 0; i < m_leafCount; ++i) m_sortedLeaves.push_back(i);
    std::sort(m_sortedLeaves.begin(), m_sortedLeaves.end(), [this](int a, int b) {
        if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
        if (m_nodes[a].original != m_nodes[b].original) return m_nodes[a].original;
        return a < b;
    });

    std::vector<int> forest;
    for (int i = 0; i < m_leafCount; ++i) forest.push_back(i);

    while (forest.size() > 1) {
        std::sort(forest.begin(), forest.end(), [this](int a, int b) {
            if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
            if (m_nodes[a].original != m_nodes[b].original) return m_nodes[a].original;
            return a < b;
        });
        const int a = forest[0];
        const int b = forest[1];
        const bool aLeaf = m_nodes[a].original;
        const bool bLeaf = m_nodes[b].original;

        HuffmanNodeData parent;
        parent.id = static_cast<int>(m_nodes.size());
        parent.weight = m_nodes[a].weight + m_nodes[b].weight;
        parent.left = a;
        parent.right = b;
        parent.original = false;
        m_nodes.push_back(parent);
        m_nodes[a].parent = parent.id;
        m_nodes[b].parent = parent.id;

        HuffmanMergeStep step;
        step.a = a;
        step.b = b;
        step.parent = parent.id;
        step.kind = (aLeaf && bLeaf) ? 0 : ((aLeaf != bLeaf) ? 1 : 2);
        step.aLeaf = aLeaf;
        step.bLeaf = bLeaf;
        m_plan.push_back(step);

        forest.erase(forest.begin(), forest.begin() + 2);
        forest.push_back(parent.id);
    }
}

void HuffmanTreeModule::simulateStates() {
    std::map<int, int> rows;
    for (int i = 0; i < m_leafCount; ++i) rows[i] = 0;

    std::vector<int> queue;
    for (int id : m_sortedLeaves) queue.push_back(id);

    for (HuffmanMergeStep& step : m_plan) {
        const int a = step.a;
        const int b = step.b;
        const int parent = step.parent;

        step.before.rows = rows;
        step.before.queue = queue;

        const int aRow = rows.count(a) ? rows.at(a) : 0;
        const int bRow = rows.count(b) ? rows.at(b) : 0;
        const int base = qMax(aRow, bRow) + 1;

        step.liftedNode = -1;
        step.liftDelta = 0;

        if (aRow != bRow) {
            const int lower = (aRow < bRow) ? a : b;
            const int lowerRow = (aRow < bRow) ? aRow : bRow;
            const int higherRow = (aRow < bRow) ? bRow : aRow;
            const int delta = higherRow - lowerRow;
            step.liftedNode = lower;
            step.liftDelta = delta;
            std::vector<int> members;
            collectSubtree(lower, members);
            for (int id : members) {
                if (rows.count(id)) rows[id] += delta;
            }
        }

        step.afterLift.rows = rows;
        step.afterLift.queue = queue;

        rows[parent] = base;

        queue.erase(std::remove(queue.begin(), queue.end(), a), queue.end());
        queue.erase(std::remove(queue.begin(), queue.end(), b), queue.end());
        queue.push_back(parent);
        std::sort(queue.begin(), queue.end(), [this](int x, int y) {
            if (m_nodes[x].weight != m_nodes[y].weight) return m_nodes[x].weight < m_nodes[y].weight;
            if (m_nodes[x].original != m_nodes[y].original) return m_nodes[x].original;
            return x < y;
        });

        step.after.rows = rows;
        step.after.queue = queue;
    }
}

void HuffmanTreeModule::computeXPositions() {
    const int leaves = qMax(1, m_leafCount);
    const double span = kLeafSpan;
    const double left = kCenterX - (leaves - 1) * span / 2.0;

    std::map<int, int> leafSlots;
    int counter = 0;
    std::function<void(int)> inorder;
    inorder = [&inorder, &counter, &leafSlots, this](int id) {
        const HuffmanNodeData* node = nodeAt(id);
        if (!node) return;
        if (node->original) {
            leafSlots[id] = counter++;
            return;
        }
        inorder(node->left);
        inorder(node->right);
    };
    if (!m_plan.empty()) inorder(m_plan.back().parent);

    for (int i = 0; i < m_leafCount; ++i) {
        m_nodes[i].slot = leafSlots.count(i) ? leafSlots.at(i) : i;
    }

    std::function<double(int)> place = [&](int id) -> double {
        HuffmanNodeData* node = nodeAt(id);
        if (!node) return left;
        if (node->original) {
            node->x = left + node->slot * span;
            return node->x;
        }
        const double lx = place(node->left);
        const double rx = place(node->right);
        node->x = (lx + rx) / 2.0;
        return node->x;
    };

    if (!m_plan.empty()) place(m_plan.back().parent);
    for (int i = 0; i < m_leafCount; ++i) place(i);
}

void HuffmanTreeModule::computeCodes() {
    m_codeMap.clear();
    if (m_nodes.empty()) return;
    const int root = m_plan.empty() ? 0 : m_plan.back().parent;
    std::function<void(int, QString)> walk = [&](int id, const QString& prefix) {
        const HuffmanNodeData* node = nodeAt(id);
        if (!node) return;
        if (node->original) {
            m_codeMap[id] = prefix.isEmpty() ? QString("0") : prefix;
            return;
        }
        walk(node->left, prefix + "0");
        walk(node->right, prefix + "1");
    };
    walk(root, QString());

    for (HuffmanCodeRow& row : m_codeRows) {
        auto it = m_codeMap.find(row.leafId);
        if (it == m_codeMap.end()) continue;
        row.code = it->second;
        row.codeLength = it->second.size();
    }
}

QPointF HuffmanTreeModule::treePosOf(int id, const HuffmanForest& forest) const {
    const HuffmanNodeData* node = nodeAt(id);
    if (!node) return QPointF();
    const int row = forest.rows.count(id) ? forest.rows.at(id) : 0;
    return QPointF(node->x, kTreeBottomY - row * kLevelH);
}

QPointF HuffmanTreeModule::queueSlotPos(int index) const {
    const int count = qMax(1, m_leafCount);
    const double left = kCenterX - (count - 1) * kQueueSpan / 2.0;
    return QPointF(left + index * kQueueSpan, kQueueY);
}

int HuffmanTreeModule::queueIndexOf(int id, const std::vector<int>& queue) const {
    for (size_t i = 0; i < queue.size(); ++i) {
        if (queue[i] == id) return static_cast<int>(i);
    }
    return -1;
}

void HuffmanTreeModule::applyTreeRows(const HuffmanForest& forest, int duration) {
    std::vector<QGraphicsItem*> movers;
    std::vector<QPointF> goals;
    for (const auto& kv : forest.rows) {
        auto it = m_treeNodes.find(kv.first);
        if (it == m_treeNodes.end() || !it->second) continue;
        if (it->second->opacity() < 0.02) continue;
        movers.push_back(it->second);
        goals.push_back(treePosOf(kv.first, forest));
    }
    if (movers.empty()) return;
    playMoveTo(movers, goals, duration);
}

void HuffmanTreeModule::applyQueueOrder(const std::vector<int>& order, int duration) {
    std::vector<QGraphicsItem*> movers;
    std::vector<QPointF> goals;
    for (size_t i = 0; i < order.size(); ++i) {
        auto it = m_queueNodes.find(order[i]);
        if (it == m_queueNodes.end() || !it->second) continue;
        if (it->second->opacity() < 0.02) continue;
        movers.push_back(it->second);
        goals.push_back(queueSlotPos(static_cast<int>(i)));
    }
    if (movers.empty()) return;
    playMoveTo(movers, goals, duration);
}

void HuffmanTreeModule::hideQueueChips(const std::vector<int>& order) {
    for (int id : order) {
        auto it = m_queueNodes.find(id);
        if (it == m_queueNodes.end() || !it->second) continue;
        it->second->setOpacity(0.0);
    }
}

void HuffmanTreeModule::completeEdgeGrowth() {
    for (auto& kv : m_childEdges) {
        if (!kv.second) continue;
        if (kv.second->opacity() < 0.02) continue;
        if (kv.second->drawProgress() < 1.0) kv.second->setDrawProgress(1.0);
    }
}

void HuffmanTreeModule::buildCodeTable() {
    m_codeRows.clear();
    m_codeFreqCells.clear();
    m_codeCells.clear();
    m_codeLenCells.clear();

    for (int i = 0; i < m_leafCount; ++i) {
        HuffmanCodeRow row;
        row.leafId = i;
        row.originalIndex = i;
        row.weight = m_nodes[i].weight;
        m_codeRows.push_back(row);
    }

    for (size_t r = 0; r < m_codeRows.size(); ++r) {
        const double y = kTableTop + static_cast<double>(r) * kTableRowH;
        ArrayBlock* freq = new ArrayBlock(6000 + static_cast<int>(r), 0, QString::number(r + 1));
        freq->setCellSize(kTableCol1, kTableRowH - 2.0);
        freq->setShowLabel(false);
        freq->setValueFontSize(12);
        freq->setCustomValue(QString::number(m_codeRows[r].weight));
        freq->color = kHfIdle;
        freq->setPos(kTableX, y);
        m_scene->addItem(freq);
        m_codeFreqCells.push_back(freq);

        ArrayBlock* code = new ArrayBlock(6100 + static_cast<int>(r), 0, QString::number(r + 1));
        code->setCellSize(kTableCol2, kTableRowH - 2.0);
        code->setShowLabel(false);
        code->setValueFontSize(12);
        code->setCustomValue("");
        code->color = kHfIdle;
        code->setPos(kTableX + kTableCol1, y);
        m_scene->addItem(code);
        m_codeCells.push_back(code);

        ArrayBlock* len = new ArrayBlock(6200 + static_cast<int>(r), 0, QString::number(r + 1));
        len->setCellSize(kTableCol3, kTableRowH - 2.0);
        len->setShowLabel(false);
        len->setValueFontSize(12);
        len->setCustomValue("");
        len->color = kHfIdle;
        len->setPos(kTableX + kTableCol1 + kTableCol2, y);
        m_scene->addItem(len);
        m_codeLenCells.push_back(len);
    }
}

void HuffmanTreeModule::clearCodeTable() {
    for (auto* cell : m_codeCells) { if (cell) { cell->setCustomValue(""); cell->color = kHfIdle; cell->update(); } }
    for (auto* cell : m_codeLenCells) { if (cell) { cell->setCustomValue(""); cell->color = kHfIdle; cell->update(); } }
}

void HuffmanTreeModule::clearWplRow() {
    for (auto* cell : m_wplCells) {
        if (!cell) continue;
        cell->setCustomValue("");
        cell->color = kHfIdle;
        cell->update();
    }
    if (m_wplSumCell) {
        m_wplSumCell->setCustomValue("");
        m_wplSumCell->color = kHfIdle;
        m_wplSumCell->update();
    }
    m_wplTotal = 0;
}

void HuffmanTreeModule::setLeafCode(int leafId, const QString& code) {
    m_codeMap[leafId] = code;
    for (size_t r = 0; r < m_codeRows.size(); ++r) {
        if (m_codeRows[r].leafId != leafId) continue;
        m_codeRows[r].code = code;
        m_codeRows[r].codeLength = code.size();
        if (r < m_codeCells.size() && m_codeCells[r]) {
            m_codeCells[r]->setCustomValue(code);
            m_codeCells[r]->color = kHfCode;
            m_codeCells[r]->update();
        }
        if (r < m_codeLenCells.size() && m_codeLenCells[r]) {
            m_codeLenCells[r]->setCustomValue(QString::number(code.size()));
            m_codeLenCells[r]->color = kHfCode;
            m_codeLenCells[r]->update();
        }
        return;
    }
}

void HuffmanTreeModule::revealPathLabels(int leafId) {
    int cur = leafId;
    while (cur >= 0) {
        const HuffmanNodeData* node = nodeAt(cur);
        if (!node || node->original) break;
        const bool goesLeft = subtreeHas(node->left, leafId);
        const int key = node->id * 4 + (goesLeft ? 0 : 1);
        auto it = m_childEdges.find(key);
        if (it != m_childEdges.end() && it->second) {
            it->second->setPathLabel(goesLeft ? "0" : "1");
            it->second->setPathLabelColor(kHfPath);
            it->second->update();
        }
        cur = node->parent;
    }
}

void HuffmanTreeModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!tokens.isEmpty()) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 2 && n <= 8 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                const long long value = tokens[i + 1].toLongLong(&ok);
                if (!ok || value < 0 || value > 9999) {
                    m_valid = false;
                    break;
                }
                HuffmanNodeData node;
                node.id = i;
                node.weight = value;
                node.original = true;
                m_nodes.push_back(node);
            }
        }
    }

    m_mathText = createScriptText(scene, m_valid ? "哈夫曼树：每次取权值最小的两个节点合并，父节点权值 = 两者之和"
                                        : "输入格式：n，随后输入 n 个权值；2 <= n <= 8");
    m_mathText->setFont(UiFonts::mixed(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(40, 14);
    if (!m_valid) return;

    computeTree();
    simulateStates();
    computeXPositions();
    buildCodeTable();
    computeCodes();

    m_arrayTitle = scene->addText("频率数组 a[]");
    m_arrayTitle->setFont(UiFonts::cjk(12, true));
    m_arrayTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_arrayTitle->setPos(kArrayLeft, kArrayY - 32);

    m_phaseText = scene->addText("阶段一：把频率数组升序排序");
    m_phaseText->setFont(UiFonts::cjk(13, true));
    m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
    m_phaseText->setPos(40, 52);

    for (int i = 0; i < m_leafCount; ++i) {
        ArrayBlock* cell = new ArrayBlock(i, 0, QString::number(i));
        cell->setCellSize(kCellW, kCellH);
        cell->setShowLabel(false);
        cell->setValueFontSize(13);
        cell->color = kHfLeaf;
        cell->setCustomValue(QString::number(m_nodes[i].weight));
        cell->setPos(kArrayLeft + static_cast<double>(i) * kCellW, kArrayY);
        scene->addItem(cell);
        m_arrayCells.push_back(cell);
    }

    m_treeTitle = scene->addText("哈夫曼树");
    m_treeTitle->setFont(UiFonts::cjk(12, true));
    m_treeTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_treeTitle->setPos(40, 206);

    m_treeFrame = scene->addRect(QRectF(kCenterX - 262.0, 232.0, 524.0, 372.0), QPen(kHfFrame, 1.6, Qt::DashLine), Qt::NoBrush);
    m_treeFrame->setZValue(-1.0);

    m_queueTitle = scene->addText("候选队列");
    m_queueTitle->setFont(UiFonts::cjk(12, true));
    m_queueTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_queueTitle->setPos(40, kQueueY - 62);

    m_tableTitle = scene->addText("哈夫曼编码表");
    m_tableTitle->setFont(UiFonts::cjk(12, true));
    m_tableTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_tableTitle->setPos(kTableX, kTableTop - 62);

    m_tableHead1 = scene->addText("a[i]");
    m_tableHead1->setFont(UiFonts::mono(11, true));
    m_tableHead1->setDefaultTextColor(QColor(96, 110, 130));
    m_tableHead1->setPos(kTableX, kTableTop - 28);

    m_tableHead2 = scene->addText("编码");
    m_tableHead2->setFont(UiFonts::cjk(11, true));
    m_tableHead2->setDefaultTextColor(QColor(96, 110, 130));
    m_tableHead2->setPos(kTableX + kTableCol1, kTableTop - 28);

    m_tableHead3 = scene->addText("码长");
    m_tableHead3->setFont(UiFonts::cjk(11, true));
    m_tableHead3->setDefaultTextColor(QColor(96, 110, 130));
    m_tableHead3->setPos(kTableX + kTableCol1 + kTableCol2, kTableTop - 28);

    m_wplTitle = scene->addText("带权路径长度 WPL = Σ (a[i] × 码长)");
    m_wplTitle->setFont(UiFonts::cjk(12, true));
    m_wplTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_wplTitle->setPos(40, kWplY - 30);

    for (int i = 0; i < m_leafCount; ++i) {
        ArrayBlock* cell = new ArrayBlock(6400 + i, 0, QString::number(i + 1));
        cell->setCellSize(kWplCellW, kWplCellH);
        cell->setShowLabel(false);
        cell->setValueFontSize(13);
        cell->setCustomValue("");
        cell->color = kHfIdle;
        cell->setPos(40 + static_cast<double>(i) * kWplCellW, kWplY);
        scene->addItem(cell);
        m_wplCells.push_back(cell);
    }

    m_wplSumCell = new ArrayBlock(6500, 0, "WPL");
    m_wplSumCell->setCellSize(kWplCellW + 20, kWplCellH);
    m_wplSumCell->setShowLabel(false);
    m_wplSumCell->setValueFontSize(13);
    m_wplSumCell->setCustomValue("");
    m_wplSumCell->color = kHfIdle;
    m_wplSumCell->setPos(40 + static_cast<double>(m_leafCount) * kWplCellW + 18, kWplY);
    scene->addItem(m_wplSumCell);

    m_noteText = createScriptText(scene, "");
    m_noteText->setFont(UiFonts::mixed(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(40, 786);

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(40, 818);

    m_ptrA = new PointerBlock("x", QColor(210, 80, 55));
    m_ptrA->setOpacity(0);
    m_ptrA->setDirection(PointerBlock::Down);
    m_ptrA->setShaftLength(22);
    m_ptrA->setAnchorOffset(PointerGeometry::aboveCell(kCellW, kCellH));
    m_ptrA->setLabelAtTail(6.0);
    scene->addItem(m_ptrA);

    m_ptrB = new PointerBlock("y", QColor(45, 130, 210));
    m_ptrB->setOpacity(0);
    m_ptrB->setDirection(PointerBlock::Down);
    m_ptrB->setShaftLength(22);
    m_ptrB->setAnchorOffset(PointerGeometry::aboveCell(kCellW, kCellH));
    m_ptrB->setLabelAtTail(6.0);
    scene->addItem(m_ptrB);

    for (int id = 0; id < static_cast<int>(m_nodes.size()); ++id) {
        GraphNode* item = new GraphNode(id);
        item->color = m_nodes[id].original ? kHfLeaf : kHfMerge;
        item->setCustomValueText(QString::number(m_nodes[id].weight));
        item->setOpacity(0.0);
        scene->addItem(item);
        m_nodes[id].treeItem = item;
        m_treeNodes[id] = item;
    }

    for (const HuffmanMergeStep& step : m_plan) {
        HuffmanNodeData* pa = nodeAt(step.a);
        HuffmanNodeData* pb = nodeAt(step.b);
        HuffmanNodeData* pp = nodeAt(step.parent);
        if (!pa || !pb || !pp || !pa->treeItem || !pb->treeItem || !pp->treeItem) continue;
        GraphEdge* ea = new GraphEdge(pa->treeItem, pp->treeItem, false);
        ea->setColor(kHfEdge);
        ea->setPenWidth(2.0);
        ea->setOpacity(0.0);
        scene->addItem(ea);
        pa->treeItem->addEdge(ea);
        pp->treeItem->addEdge(ea);
        m_childEdges[step.parent * 4 + 0] = ea;
        GraphEdge* eb = new GraphEdge(pb->treeItem, pp->treeItem, false);
        eb->setColor(kHfEdge);
        eb->setPenWidth(2.0);
        eb->setOpacity(0.0);
        scene->addItem(eb);
        pb->treeItem->addEdge(eb);
        pp->treeItem->addEdge(eb);
        m_childEdges[step.parent * 4 + 1] = eb;
    }

    for (int id = 0; id < m_leafCount; ++id) {
        GraphNode* chip = new GraphNode(9000 + id);
        chip->color = kHfIdle;
        chip->setCustomValueText(QString::number(m_nodes[id].weight));
        chip->setOpacity(0.0);
        scene->addItem(chip);
        m_nodes[id].queueChip = chip;
        m_queueNodes[id] = chip;
    }
}

std::vector<AnimFrame> HuffmanTreeModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int leafCount = m_leafCount;

    m_frames.push_back({ActionType::HF_RESET, 0, 0, 0, 0, {},
                        QString("频率数组 a[]：%1 个权值待排序").arg(leafCount),
                        QString("哈夫曼树 = 最优前缀编码树")});

    std::vector<int> target;
    for (int i = 0; i < leafCount; ++i) target.push_back(i);
    std::sort(target.begin(), target.end(), [this](int a, int b) {
        if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
        if (m_nodes[a].original != m_nodes[b].original) return m_nodes[a].original;
        return a < b;
    });

    std::vector<int> state;
    for (int i = 0; i < leafCount; ++i) state.push_back(i);

    for (size_t k = 0; k < target.size(); ++k) {
        const int wanted = target[k];
        int from = k;
        for (size_t s = 0; s < state.size(); ++s) {
            if (state[s] == wanted) { from = static_cast<int>(s); break; }
        }
        if (from == static_cast<int>(k)) continue;

        std::swap(state[k], state[from]);

        m_frames.push_back({ActionType::HF_SORT_LIFT, from, wanted,
                            static_cast<int>(m_nodes[wanted].weight), 0, state,
                            QString("a[%1] = %2 不在排序后应在的位置，先飞出数组")
                                .arg(from).arg(m_nodes[wanted].weight),
                            QString("不在应有位置的元素先飞出")});

        m_frames.push_back({ActionType::HF_SORT_DROP, static_cast<int>(k), wanted,
                            static_cast<int>(m_nodes[wanted].weight), 0, state,
                            QString("a[%1] = %2 像流星一样飞回第 %3 个位置")
                                .arg(k).arg(m_nodes[wanted].weight).arg(k),
                            QString("飞星落位")});
    }

    QStringList sortedText;
    for (int id : target) sortedText << QString::number(m_nodes[id].weight);
    m_frames.push_back({ActionType::HF_SORT_DONE, 0, 0, 0, 0, {},
                        QString("频率数组排序完成：%1").arg(sortedText.join(" < ")),
                        QString("阶段一完成，进入建树阶段")});

    m_frames.push_back({ActionType::HF_QUEUE_DROP, 0, 0, 0, 0, {},
                        QString("全部数字飞入底部候选队列，按权值升序排队"),
                        QString("阶段二：数字进入候选队列")});

    for (size_t s = 0; s < m_plan.size(); ++s) {
        const HuffmanMergeStep& step = m_plan[s];
        const QString kindText = step.kind == 0
                                     ? "两个都是原始数据，直接作为叶子连到父节点"
                                     : (step.kind == 1 ? "其中一个已带子树，让另一个飞过去与它相连"
                                                        : "两个都带子树，先把树高调整到同一行再相连");

        m_frames.push_back({ActionType::HF_PICK, step.a, step.b,
                            static_cast<int>(m_nodes[step.a].weight),
                            static_cast<int>(m_nodes[step.b].weight), {step.kind},
                            QString("取出两个最小权值：%1（%2）与 %3（%4）；%5")
                                .arg(step.a).arg(m_nodes[step.a].weight)
                                .arg(step.b).arg(m_nodes[step.b].weight).arg(kindText),
                            QString("每次取两个最小")});

        m_frames.push_back({ActionType::HF_PROMOTE, step.a, step.b,
                            static_cast<int>(m_nodes[step.a].weight),
                            static_cast<int>(m_nodes[step.b].weight), {step.kind},
                            step.kind == 0
                                ? QString("%1 与 %2 一起向上飞入树区域，准备作为叶子连接")
                                    .arg(m_nodes[step.a].weight).arg(m_nodes[step.b].weight)
                                : QString("带子树的一方上移到与另一方同一行，准备相连"),
                            QString("向上飞入树区域")});

        if (step.liftedNode >= 0 && step.liftDelta != 0) {
            m_frames.push_back({ActionType::HF_LIFT, step.a, step.b, step.liftedNode, step.liftDelta,
                                {step.kind},
                                QString("子树整体上移 %1 行，与另一棵子树对齐").arg(step.liftDelta),
                                QString("对齐树高")});
        }

        m_frames.push_back({ActionType::HF_JELLY, step.a, step.b, step.parent,
                            static_cast<int>(m_nodes[step.parent].weight), {step.kind},
                            QString("在两子节点中点长出新节点 %1：%2 + %3 = %4")
                                .arg(step.parent).arg(m_nodes[step.a].weight)
                                .arg(m_nodes[step.b].weight).arg(m_nodes[step.parent].weight),
                            QString("新节点权值 = 左 + 右")});

        m_frames.push_back({ActionType::HF_MERGE, step.a, step.b, step.parent,
                            static_cast<int>(m_nodes[step.parent].weight), {step.kind},
                            QString("父节点 %1 加入候选队列，其余成员左右让位重排").arg(step.parent),
                            QString("父节点落回候选队列")});
    }

    const int rootId = m_plan.empty() ? 0 : m_plan.back().parent;
    m_frames.push_back({ActionType::HF_FINAL, rootId, static_cast<int>(m_nodes.size()), 0, 0, {},
                        QString("哈夫曼树构造完成：共 %1 个节点，根节点权值 = %2")
                            .arg(m_nodes.size()).arg(m_nodes[rootId].weight),
                        QString("带权路径长度最小 = 最优前缀编码")});

    for (size_t r = 0; r < m_codeRows.size(); ++r) {
        const HuffmanCodeRow& row = m_codeRows[r];
        m_frames.push_back({ActionType::HF_CODE, row.leafId, static_cast<int>(r),
                            static_cast<int>(row.weight), row.codeLength, {},
                            QString("叶子 a[%1] = %2 的哈夫曼编码为 %3，码长 %4")
                                .arg(row.originalIndex).arg(row.weight).arg(row.code).arg(row.codeLength),
                            QString("沿根到叶的路径，左 0 右 1")});
    }

    long long running = 0;
    for (size_t r = 0; r < m_codeRows.size(); ++r) {
        const HuffmanCodeRow& row = m_codeRows[r];
        const long long product = row.weight * row.codeLength;
        running += product;
        m_frames.push_back({ActionType::HF_WPL, row.leafId, static_cast<int>(r),
                            static_cast<int>(row.weight), row.codeLength, {running},
                            QString("a[%1] = %2，码长 %3，贡献 %4，累计 WPL = %5")
                                .arg(row.originalIndex).arg(row.weight)
                                .arg(row.codeLength).arg(product).arg(running),
                            QString("逐项累加 WPL")});
    }
    m_wplTotal = running;
    m_frames.push_back({ActionType::HF_WPL_DONE, static_cast<int>(m_wplTotal), 0, 0, 0, {},
                        QString("带权路径长度 WPL = %1，等于全部内部节点权值之和").arg(m_wplTotal),
                        QString("WPL 计算完成")});
    return m_frames;
}

void HuffmanTreeModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::HF_RESET) {
        for (auto& kv : m_treeNodes) {
            if (!kv.second) continue;
            const HuffmanNodeData* node = nodeAt(kv.first);
            const bool isLeaf = node && node->original;
            kv.second->color = isLeaf ? kHfLeaf : kHfMerge;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->setOpacity(0.0);
            kv.second->update();
        }
        for (auto& kv : m_queueNodes) {
            if (!kv.second) continue;
            kv.second->setOpacity(0.0);
            kv.second->update();
        }
        for (auto& kv : m_childEdges) {
            kv.second->setColor(kHfEdge);
            kv.second->setPenWidth(2.0);
            kv.second->setPathLabel("");
            kv.second->setDrawProgress(1.0);
            kv.second->setOpacity(0.0);
            kv.second->update();
        }
        for (size_t i = 0; i < m_arrayCells.size(); ++i) {
            ArrayBlock* cell = m_arrayCells[i];
            if (!cell) continue;
            cell->color = kHfLeaf;
            if (i < static_cast<size_t>(m_leafCount)) {
                cell->setCustomValue(QString::number(m_nodes[i].weight));
            } else {
                cell->setCustomValue("");
            }
            cell->update();
        }
        clearCodeTable();
        clearWplRow();
        if (m_ptrA) m_ptrA->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, "");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
        if (m_resultText) setScriptText(m_resultText, "");
        m_phaseText->setPlainText("阶段一：把频率数组升序排序");
        m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
        return;
    }

    if (frame.type == ActionType::HF_SORT_LIFT) {
        const int slot = frame.u;
        const int nodeId = frame.v;
        ArrayBlock* cell = (slot >= 0 && slot < static_cast<int>(m_arrayCells.size()))
                               ? m_arrayCells[slot] : nullptr;
        if (cell) {
            cell->color = kHfPick;
            cell->showValue = false;
            cell->setCustomValue("");
            cell->update();
            if (m_animated) {
                playScalePulse(cell, 1.18, 240);
                const long long weight = (nodeId >= 0 && nodeId < static_cast<int>(m_nodes.size()))
                                             ? m_nodes[nodeId].weight : 0;
                playValueFlyOut(cell->centerPos(), static_cast<int>(weight), kHfText, 460);
            }
        }
        if (m_ptrA && cell) {
            m_ptrA->setOpacity(1);
            retargetPointerTo(m_ptrA, cell, PointerGeometry::aboveCell(kCellW, kCellH));
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_SORT_DROP) {
        const int slot = frame.u;
        const int nodeId = frame.v;
        const std::vector<int>& perm = frame.group;
        if (perm.size() != m_arrayCells.size()) return;

        for (size_t i = 0; i < perm.size(); ++i) {
            ArrayBlock* cell = m_arrayCells[i];
            if (!cell) continue;
            const int id = perm[i];
            if (id >= 0 && id < static_cast<int>(m_nodes.size())) {
                cell->setCustomValue(QString::number(m_nodes[id].weight));
            }
            cell->color = (static_cast<int>(i) == slot) ? kHfSorted : kHfLeaf;
            cell->update();
        }

        ArrayBlock* dest = (slot >= 0 && slot < static_cast<int>(m_arrayCells.size()))
                               ? m_arrayCells[slot] : nullptr;
        if (dest && m_animated) {
            const QPointF start = QPointF(dest->centerPos().x(), kArrayY - 44.0);
            const long long weight = (nodeId >= 0 && nodeId < static_cast<int>(m_nodes.size()))
                                         ? m_nodes[nodeId].weight : 0;
            playValueDropInto(start, dest, static_cast<int>(weight), kHfText, 520);
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }
    if (frame.type == ActionType::HF_SORT_DONE) {
        for (size_t i = 0; i < m_arrayCells.size(); ++i) {
            if (m_arrayCells[i]) { m_arrayCells[i]->color = kHfSorted; m_arrayCells[i]->update(); }
        }
        if (m_ptrA) m_ptrA->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
        return;
    }

    if (frame.type == ActionType::HF_QUEUE_DROP) {
        m_phaseText->setPlainText("阶段二：数字进入底部候选队列，按权值升序排队");
        m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
        for (size_t i = 0; i < m_sortedLeaves.size(); ++i) {
            const int id = m_sortedLeaves[i];
            auto chipIt = m_queueNodes.find(id);
            if (chipIt == m_queueNodes.end() || !chipIt->second) continue;
            GraphNode* chip = chipIt->second;
            const QPointF target = queueSlotPos(static_cast<int>(i));
            const int src = (id >= 0 && id < static_cast<int>(m_arrayCells.size())) ? id : 0;
            const QPointF origin = (src >= 0 && src < static_cast<int>(m_arrayCells.size()))
                                       ? m_arrayCells[src]->centerPos()
                                       : QPointF(kCenterX, kArrayY);
            chip->setOpacity(1.0);
            chip->setPos(origin);
            chip->update();
            if (!m_animated) {
                chip->setPos(target);
                continue;
            }
            playMoveTo({chip}, {target}, 620);
            playScalePulse(chip, 1.18, 420);
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_PICK) {
        completeEdgeGrowth();
        const int a = frame.u;
        const int b = frame.v;
        for (int id : {a, b}) {
            auto chipIt = m_queueNodes.find(id);
            if (chipIt == m_queueNodes.end() || !chipIt->second) continue;
            GraphNode* chip = chipIt->second;
            chip->color = kHfPick;
            chip->setHighlightColor(kHfPick);
            chip->update();
            if (m_animated) playScalePulse(chip, 1.16, 240);
        }
        if (m_ptrA) {
            auto it = m_queueNodes.find(a);
            if (it != m_queueNodes.end() && it->second) {
                m_ptrA->setOpacity(1);
                retargetPointerTo(m_ptrA, it->second, QPointF(0.0, -(kNodeR + 26.0)));
            }
        }
        if (m_ptrB) {
            auto it = m_queueNodes.find(b);
            if (it != m_queueNodes.end() && it->second) {
                m_ptrB->setOpacity(1);
                retargetPointerTo(m_ptrB, it->second, QPointF(0.0, -(kNodeR + 26.0)));
            }
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_PROMOTE) {
        const int a = frame.u;
        const int b = frame.v;
        int stepIndex = -1;
        for (size_t s = 0; s < m_plan.size(); ++s) {
            if (m_plan[s].a == a && m_plan[s].b == b) { stepIndex = static_cast<int>(s); break; }
        }
        if (stepIndex < 0) return;
        const HuffmanMergeStep& step = m_plan[stepIndex];

        for (int id : {a, b}) {
            const HuffmanNodeData* data = nodeAt(id);
            if (!data) continue;
            if (!data->original) continue;
            auto chipIt = m_queueNodes.find(id);
            if (chipIt == m_queueNodes.end() || !chipIt->second) continue;
            GraphNode* chip = chipIt->second;
            const QPointF target = treePosOf(id, step.afterLift);
            if (m_animated) {
                playMoveTo({chip}, {target}, 640);
            } else {
                chip->setPos(target);
            }
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_LIFT) {
        const int a = frame.u;
        const int b = frame.v;
        int stepIndex = -1;
        for (size_t s = 0; s < m_plan.size(); ++s) {
            if (m_plan[s].a == a && m_plan[s].b == b) { stepIndex = static_cast<int>(s); break; }
        }
        if (stepIndex < 0) return;
        const HuffmanMergeStep& step = m_plan[stepIndex];
        applyTreeRows(step.afterLift, m_animated ? 380 : 0);
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_JELLY) {
        completeEdgeGrowth();
        const int parentId = frame.val1;
        const int a = frame.u;
        const int b = frame.v;
        int stepIndex = -1;
        for (size_t s = 0; s < m_plan.size(); ++s) {
            if (m_plan[s].parent == parentId) { stepIndex = static_cast<int>(s); break; }
        }
        if (stepIndex < 0) return;
        const HuffmanMergeStep& step = m_plan[stepIndex];

        for (int id : {a, b}) {
            auto it = m_treeNodes.find(id);
            if (it == m_treeNodes.end() || !it->second) continue;
            it->second->setPos(treePosOf(id, step.after));
            it->second->setOpacity(1.0);
            it->second->setHighlightColor(Qt::transparent);
            it->second->update();
            auto chipIt = m_queueNodes.find(id);
            if (chipIt != m_queueNodes.end() && chipIt->second) {
                chipIt->second->setOpacity(0.0);
                chipIt->second->update();
            }
        }

        HuffmanNodeData* parent = nodeAt(parentId);
        if (parent && parent->treeItem) {
            parent->treeItem->setPos(treePosOf(parentId, step.after));
            parent->treeItem->setOpacity(1.0);
            parent->treeItem->color = kHfMerge;
            parent->treeItem->setHighlightColor(kHfMerge);
            parent->treeItem->update();
            if (m_animated) {
                playNodeAppear(parent->treeItem, 460);
                playRipple(parent->treeItem->pos(), kHfEdgeNew, 42.0, 620);
            }
        }

        for (int key : {parentId * 4 + 0, parentId * 4 + 1}) {
            auto it = m_childEdges.find(key);
            if (it == m_childEdges.end() || !it->second) continue;
            GraphEdge* edge = it->second;
            edge->setOpacity(1.0);
            edge->setColor(kHfEdgeNew);
            edge->setPenWidth(2.6);
            edge->setDrawProgress(0.0);
            edge->update();
            if (m_animated) playEdgeGrow(edge, 200);
            else edge->setDrawProgress(1.0);
        }

        applyTreeRows(step.after, m_animated ? 460 : 0);
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_MERGE) {
        completeEdgeGrowth();
        const int a = frame.u;
        const int b = frame.v;
        const int parentId = frame.val1;
        int stepIndex = -1;
        for (size_t s = 0; s < m_plan.size(); ++s) {
            if (m_plan[s].parent == parentId) { stepIndex = static_cast<int>(s); break; }
        }
        if (stepIndex < 0) return;
        const HuffmanMergeStep& step = m_plan[stepIndex];
        const bool isRoot = (stepIndex == static_cast<int>(m_plan.size()) - 1);

        GraphNode* chip = nullptr;
        auto chipIt = m_queueNodes.find(parentId);
        if (chipIt != m_queueNodes.end()) chip = chipIt->second;
        if (!chip) {
            chip = new GraphNode(9000 + parentId);
            chip->color = kHfMerge;
            chip->setCustomValueText(QString::number(m_nodes[parentId].weight));
            chip->setOpacity(1.0);
            chip->setPos(treePosOf(parentId, step.after));
            m_scene->addItem(chip);
            m_queueNodes[parentId] = chip;
        }
        chip->setCustomValueText(QString::number(m_nodes[parentId].weight));
        chip->color = kHfMerge;
        chip->setHighlightColor(kHfMerge);
        chip->setOpacity(1.0);
        chip->update();

        if (isRoot) {
            chip->setOpacity(0.0);
            chip->update();
            hideQueueChips({a, b});
            applyQueueOrder(step.after.queue, m_animated ? 420 : 0);
            if (m_ptrA) m_ptrA->setOpacity(0);
            if (m_ptrB) m_ptrB->setOpacity(0);
            if (m_noteText) {
                setScriptText(m_noteText, frame.explanation);
                m_noteText->setDefaultTextColor(QColor(70, 110, 170));
            }
            return;
        }

        const QPointF target = queueSlotPos(queueIndexOf(parentId, step.after.queue));
        if (m_animated) {
            playMoveTo({chip}, {target}, 520);
            playScalePulse(chip, 1.16, 380);
        } else {
            chip->setPos(target);
        }

        hideQueueChips({a, b});
        applyQueueOrder(step.after.queue, m_animated ? 420 : 0);

        if (m_ptrA) m_ptrA->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(70, 110, 170));
        }
        return;
    }

    if (frame.type == ActionType::HF_FINAL) {
        const int rootId = frame.u;
        auto it = m_treeNodes.find(rootId);
        if (it != m_treeNodes.end() && it->second) {
            it->second->color = kHfRoot;
            it->second->setHighlightColor(kHfRoot);
            it->second->update();
            if (m_animated) playScalePulse(it->second, 1.26, 340);
        }
        auto chipIt = m_queueNodes.find(rootId);
        if (chipIt != m_queueNodes.end() && chipIt->second) {
            chipIt->second->setOpacity(0.0);
            chipIt->second->update();
        }
        if (m_ptrA) m_ptrA->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        m_phaseText->setPlainText("阶段三：沿根到叶的路径（左 0 右 1）生成哈夫曼编码");
        m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
        if (m_resultText) {
            setScriptText(m_resultText, frame.explanation);
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        return;
    }

    if (frame.type == ActionType::HF_CODE) {
        const int leafId = frame.u;
        const int rowIndex = frame.v;
        revealPathLabels(leafId);
        const QString code = m_codeMap.count(leafId) ? m_codeMap.at(leafId) : QString();
        setLeafCode(leafId, code);
        if (rowIndex >= 0 && rowIndex < static_cast<int>(m_codeRows.size())) {
            m_codeRows[rowIndex].code = code;
            m_codeRows[rowIndex].codeLength = code.size();
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(70, 110, 170));
        }
        return;
    }

    if (frame.type == ActionType::HF_WPL) {
        const int leafId = frame.u;
        const int rowIndex = frame.v;
        const long long running = frame.group.size() > 1 ? frame.group[0] : 0;
        m_wplTotal = running;

        int targetRow = rowIndex;
        for (size_t r = 0; r < m_codeRows.size(); ++r) {
            if (m_codeRows[r].leafId == leafId) { targetRow = static_cast<int>(r); break; }
        }

        if (targetRow >= 0 && targetRow < static_cast<int>(m_wplCells.size())) {
            ArrayBlock* cell = m_wplCells[targetRow];
            const long long weight = m_codeRows[targetRow].weight;
            const int len = m_codeRows[targetRow].codeLength;
            cell->setCustomValue(QString("%1×%2").arg(weight).arg(len));
            cell->color = kHfCodeHot;
            cell->update();
            if (m_animated) playScalePulse(cell, 1.16, 240);
        }
        if (m_wplSumCell) {
            m_wplSumCell->setCustomValue(QString::number(running));
            m_wplSumCell->color = kHfWpl;
            m_wplSumCell->update();
            if (m_animated) playScalePulse(m_wplSumCell, 1.14, 220);
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
        return;
    }

    if (frame.type == ActionType::HF_WPL_DONE) {
        m_wplTotal = frame.u;
        if (m_wplSumCell) {
            m_wplSumCell->setCustomValue(QString::number(m_wplTotal));
            m_wplSumCell->color = kHfWpl;
            m_wplSumCell->update();
            if (m_animated) playScalePulse(m_wplSumCell, 1.22, 320);
        }
        m_phaseText->setPlainText("全部演示完成");
        m_phaseText->setDefaultTextColor(QColor(40, 140, 70));
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void HuffmanTreeModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_plan.clear();
    m_sortedLeaves.clear();
    m_arrayCells.clear();
    m_codeRows.clear();
    m_codeFreqCells.clear();
    m_codeCells.clear();
    m_codeLenCells.clear();
    m_wplCells.clear();
    m_wplSumCell = nullptr;
    m_treeNodes.clear();
    m_queueNodes.clear();
    m_childEdges.clear();
    m_codeMap.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_phaseText = nullptr;
    m_resultText = nullptr;
    m_arrayTitle = nullptr;
    m_treeTitle = nullptr;
    m_queueTitle = nullptr;
    m_tableTitle = nullptr;
    m_tableHead1 = nullptr;
    m_tableHead2 = nullptr;
    m_tableHead3 = nullptr;
    m_wplTitle = nullptr;
    m_treeFrame = nullptr;
    m_ptrA = nullptr;
    m_ptrB = nullptr;
    m_valid = false;
    m_leafCount = 0;
    m_wplTotal = 0;
}

void HuffmanTreeModule::releaseSceneItems() {
    m_nodes.clear();
    m_plan.clear();
    m_sortedLeaves.clear();
    m_arrayCells.clear();
    m_codeRows.clear();
    m_codeFreqCells.clear();
    m_codeCells.clear();
    m_codeLenCells.clear();
    m_wplCells.clear();
    m_wplSumCell = nullptr;
    m_treeNodes.clear();
    m_queueNodes.clear();
    m_childEdges.clear();
    m_codeMap.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_phaseText = nullptr;
    m_resultText = nullptr;
    m_arrayTitle = nullptr;
    m_treeTitle = nullptr;
    m_queueTitle = nullptr;
    m_tableTitle = nullptr;
    m_tableHead1 = nullptr;
    m_tableHead2 = nullptr;
    m_tableHead3 = nullptr;
    m_wplTitle = nullptr;
    m_treeFrame = nullptr;
    m_ptrA = nullptr;
    m_ptrB = nullptr;
}
