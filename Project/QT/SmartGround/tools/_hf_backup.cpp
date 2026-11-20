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
static const QColor kHfMerge = QColor(190, 226, 196);
static const QColor kHfRoot = QColor(255, 226, 175);
static const QColor kHfEdge = QColor(152, 168, 192);
static const QColor kHfEdgeNew = QColor(224, 138, 40);
static const QColor kHfEmpty = QColor(242, 244, 248);
static const QColor kHfIdle = QColor(250, 250, 253);
static const QColor kHfCode = QColor(214, 236, 250);
static const QColor kHfCodeHot = QColor(255, 214, 150);
static const QColor kHfWpl = QColor(255, 226, 175);
static const QColor kHfText = QColor(20, 30, 45);
static const QColor kHfPath = QColor(214, 96, 32);

HuffmanTreeModule::HuffmanTreeModule() {}

QString HuffmanTreeModule::getModuleName() const { return "哈夫曼树"; }
QString HuffmanTreeModule::getDefaultInput() const { return "7\n5 9 12 13 16 45 20"; }
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

void HuffmanTreeModule::computeTree() {
    m_plan.clear();
    m_forest.clear();
    m_leafCount = static_cast<int>(m_nodes.size());

    for (int i = 0; i < static_cast<int>(m_nodes.size()); ++i) m_forest.push_back(i);

    while (m_forest.size() > 1) {
        std::sort(m_forest.begin(), m_forest.end(), [this](int a, int b) {
            if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
            return a < b;
        });
        int a = m_forest[0];
        int b = m_forest[1];
        const bool aOriginal = m_nodes[a].original;
        const bool bOriginal = m_nodes[b].original;
        const int kind = (aOriginal && bOriginal) ? 0 : ((aOriginal != bOriginal) ? 1 : 2);

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
        step.kind = kind;
        m_plan.push_back(step);

        m_forest.erase(m_forest.begin(), m_forest.begin() + 2);
        m_forest.push_back(parent.id);
    }
}

void HuffmanTreeModule::swapForLeftSmall() {
    for (HuffmanMergeStep& step : m_plan) {
        HuffmanNodeData* pa = nodeAt(step.a);
        HuffmanNodeData* pb = nodeAt(step.b);
        if (!pa || !pb) continue;
        if (pa->weight > pb->weight) {
            std::swap(step.a, step.b);
            std::swap(pa->left, pb->left);
        }
    }
}

int HuffmanTreeModule::countLeaves(int id) const {
    const HuffmanNodeData* node = nodeAt(id);
    if (!node) return 0;
    if (node->original) return 1;
    return countLeaves(node->left) + countLeaves(node->right);
}

void HuffmanTreeModule::shiftSubtree(int id, int delta, std::map<int, int>& levels) const {
    if (id < 0) return;
    auto it = levels.find(id);
    if (it != levels.end()) it->second += delta;
    const HuffmanNodeData* node = nodeAt(id);
    if (!node) return;
    shiftSubtree(node->left, delta, levels);
    shiftSubtree(node->right, delta, levels);
}

void HuffmanTreeModule::simulateLayout() {
    const int total = static_cast<int>(m_nodes.size());
    if (total == 0) return;

    m_sortedLeaves.clear();
    for (int i = 0; i < m_leafCount; ++i) m_sortedLeaves.push_back(i);
    std::sort(m_sortedLeaves.begin(), m_sortedLeaves.end(), [this](int a, int b) {
        if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
        return a < b;
    });

    std::map<int, int> levels;
    for (int i = 0; i < m_leafCount; ++i) levels[i] = 0;

    std::vector<int> forest;
    for (int i = 0; i < m_leafCount; ++i) forest.push_back(i);

    for (size_t s = 0; s < m_plan.size(); ++s) {
        HuffmanMergeStep& step = m_plan[s];
        const int a = step.a;
        const int b = step.b;
        const int parent = step.parent;

        const int aLevel = levels.count(a) ? levels.at(a) : 0;
        const int bLevel = levels.count(b) ? levels.at(b) : 0;
        const int base = qMax(aLevel, bLevel) + 1;

        step.liftChild = -1;
        step.liftDelta = 0;

        if (aLevel != bLevel) {
            const int lower = (aLevel < bLevel) ? a : b;
            const int higher = (aLevel < bLevel) ? b : a;
            step.liftChild = lower;
            step.liftDelta = higher - lower;
            shiftSubtree(lower, step.liftDelta, levels);
        }

        step.levelsAfterLift = levels;
        levels[parent] = base;
        step.levelsAfter = levels;

        forest.erase(std::remove(forest.begin(), forest.end(), a), forest.end());
        forest.erase(std::remove(forest.begin(), forest.end(), b), forest.end());
        forest.push_back(parent);
    }
}

void HuffmanTreeModule::computeXPositions(const std::map<int, int>& levels) {
    const int leaves = qMax(1, static_cast<int>(m_sortedLeaves.size()));
    const double span = kLeafSpan;
    const double totalW = leaves * span;
    const double left = kCenterX - totalW / 2.0 + span / 2.0;

    std::map<int, double> leafX;
    for (int k = 0; k < static_cast<int>(m_sortedLeaves.size()); ++k) {
        leafX[m_sortedLeaves[k]] = left + k * span;
    }

    std::function<double(int)> place = [&](int id) -> double {
        if (!nodeAt(id)) return left;
        const HuffmanNodeData* node = nodeAt(id);
        if (node->original) {
            m_nodes[id].x = leafX.count(id) ? leafX.at(id) : left;
            return m_nodes[id].x;
        }
        const double lx = place(node->left);
        const double rx = place(node->right);
        m_nodes[id].x = (lx + rx) / 2.0;
        return m_nodes[id].x;
    };

    for (int id = m_leafCount; id < static_cast<int>(m_nodes.size()); ++id) place(id);
    for (int id = 0; id < m_leafCount; ++id) place(id);

    std::map<int, double> levelMin;
    std::map<int, double> levelMax;
    for (const auto& kv : levels) {
        const double x = m_nodes[kv.first].x;
        if (!levelMin.count(kv.second) || x < levelMin[kv.second]) levelMin[kv.second] = x;
        if (!levelMax.count(kv.second) || x > levelMax[kv.second]) levelMax[kv.second] = x;
    }

    for (const auto& kv : levelMin) {
        const double mid = (kv.first < 0 ? 0.0 : (levelMax[kv.first] + kv.second)) / 2.0;
        const double shift = kCenterX - mid;
        if (qAbs(shift) < 0.5) continue;
        for (const auto& lv : levels) {
            if (lv.second != kv.first) continue;
            m_nodes[lv.first].x += shift;
        }
    }
}

void HuffmanTreeModule::computeCodes() {
    m_codeMap.clear();
    if (m_nodes.empty()) return;
    const int root = static_cast<int>(m_nodes.size()) - 1;
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

    for (size_t r = 0; r < m_codeRows.size(); ++r) {
        const int leafId = m_codeRows[r].leafId;
        auto it = m_codeMap.find(leafId);
        if (it == m_codeMap.end()) continue;
        m_codeRows[r].code = it->second;
        m_codeRows[r].codeLength = it->second.size();
    }
}

void HuffmanTreeModule::applyLevels(const std::map<int, int>& levels, int duration) {
    computeXPositions(levels);

    int maxLevel = 0;
    for (const auto& kv : levels) maxLevel = qMax(maxLevel, kv.second);
    const double levelH = (maxLevel > 0) ? kLevelH : kLeafRowY;

    std::vector<QGraphicsItem*> movers;
    std::vector<QPointF> goals;
    for (const auto& kv : levels) {
        auto it = m_treeNodes.find(kv.first);
        if (it == m_treeNodes.end() || !it->second) continue;
        movers.push_back(it->second);
        goals.push_back(QPointF(m_nodes[kv.first].x, kLeafRowY - levelH * kv.second));
    }
    if (movers.empty()) return;
    playMoveTo(movers, goals, duration);
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

    m_arrayTitle = scene->addText("频率数组 a[]");
    m_arrayTitle->setFont(UiFonts::cjk(12, true));
    m_arrayTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_arrayTitle->setPos(kArrayLeft, kArrayY - 32);

    m_phaseText = scene->addText("阶段一：把频率数组升序排序");
    m_phaseText->setFont(UiFonts::cjk(13, true));
    m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
    m_phaseText->setPos(40, 52);

    for (int i = 0; i < static_cast<int>(m_nodes.size()); ++i) {
        ArrayBlock* cell = new ArrayBlock(i, 0, QString::number(i));
        cell->setCellSize(kCellW, kCellH);
        cell->setShowLabel(false);
        cell->color = kHfEmpty;
        cell->showValue = false;
        cell->setPos(kArrayLeft + static_cast<double>(i) * kCellW, kArrayY);
        scene->addItem(cell);
        m_arrayCells.push_back(cell);
    }

    computeTree();
    simulateLayout();
    buildCodeTable();
    computeCodes();

    m_tableTitle = scene->addText("哈夫曼编码表");
    m_tableTitle->setFont(UiFonts::cjk(12, true));
    m_tableTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_tableTitle->setPos(kTableX, kTableTop - 54);

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
    m_noteText->setPos(40, 766);

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(40, 800);

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

    computeTree();
    swapForLeftSmall();
    simulateLayout();
    computeCodes();

    for (int id = 0; id < static_cast<int>(m_nodes.size()); ++id) {
        GraphNode* item = new GraphNode(id);
        item->color = m_nodes[id].original ? kHfLeaf : kHfMerge;
        item->setCustomValueText(QString::number(m_nodes[id].weight));
        item->setOpacity(0.0);
        scene->addItem(item);
        m_nodes[id].item = item;
        m_treeNodes[id] = item;
    }

    for (const HuffmanMergeStep& step : m_plan) {
        HuffmanNodeData* pa = nodeAt(step.a);
        HuffmanNodeData* pb = nodeAt(step.b);
        HuffmanNodeData* pp = nodeAt(step.parent);
        if (!pa || !pb || !pp || !pa->item || !pb->item || !pp->item) continue;
        GraphEdge* ea = new GraphEdge(pa->item, pp->item, true);
        ea->setColor(kHfEdge);
        ea->setPenWidth(2.0);
        ea->setOpacity(0.0);
        scene->addItem(ea);
        pa->item->addEdge(ea);
        pp->item->addEdge(ea);
        m_childEdges[step.parent * 4 + 0] = ea;
        GraphEdge* eb = new GraphEdge(pb->item, pp->item, true);
        eb->setColor(kHfEdge);
        eb->setPenWidth(2.0);
        eb->setOpacity(0.0);
        scene->addItem(eb);
        pb->item->addEdge(eb);
        pp->item->addEdge(eb);
        m_childEdges[step.parent * 4 + 1] = eb;
    }

    if (!m_plan.empty()) applyLevels(m_plan.back().levelsAfter, 0);

    for (int i = 0; i < m_leafCount; ++i) {
        auto it = m_treeNodes.find(i);
        if (it == m_treeNodes.end() || !it->second) continue;
        it->second->setPos(kArrayLeft + static_cast<double>(i) * kCellW + kCellW / 2.0,
                           kArrayY + kCellH / 2.0);
    }
}

std::vector<AnimFrame> HuffmanTreeModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int leafCount = m_leafCount;

    m_frames.push_back({ActionType::HF_RESET, 0, 0, 0, 0, {},
                        QString("频率数组 a[]：%1 个权值待排序").arg(leafCount),
                        QString("哈夫曼树 = 最优前缀编码树")});

    std::vector<long long> weights;
    for (int i = 0; i < leafCount; ++i) weights.push_back(m_nodes[i].weight);

    std::vector<int> target;
    for (int i = 0; i < leafCount; ++i) target.push_back(i);
    std::sort(target.begin(), target.end(), [this](int a, int b) {
        if (m_nodes[a].weight != m_nodes[b].weight) return m_nodes[a].weight < m_nodes[b].weight;
        return a < b;
    });

    for (size_t k = 0; k < target.size(); ++k) {
        if (weights[k] == weights[target[k]]) continue;
        m_frames.push_back({ActionType::HF_SORT_LIFT, static_cast<int>(k), target[k],
                            static_cast<int>(weights[target[k]]), 0, {target[k]},
                            QString("a[%1] = %2 不在排序后应在的位置，先飞出数组")
                                .arg(k).arg(weights[target[k]]),
                            QString("不在应有位置的元素先飞出")});
    }

    std::vector<int> state;
    for (int i = 0; i < leafCount; ++i) state.push_back(i);
    for (size_t k = 0; k < target.size(); ++k) {
        state[k] = target[k];
        m_frames.push_back({ActionType::HF_SORT_DROP, static_cast<int>(k), target[k],
                            static_cast<int>(weights[target[k]]), 0, state,
                            QString("a[%1] = %2 像流星一样飞回第 %3 个位置")
                                .arg(k).arg(weights[target[k]]).arg(k),
                            QString("飞星落位")});
    }

    QStringList sortedText;
    for (int id : target) sortedText << QString::number(m_nodes[id].weight);
    m_frames.push_back({ActionType::HF_SORT_DONE, 0, 0, 0, 0, {},
                        QString("频率数组排序完成：%1").arg(sortedText.join(" < ")),
                        QString("阶段一完成，进入建树阶段")});

    m_frames.push_back({ActionType::HF_RESET, 1, 0, 0, 0, {},
                        QString("阶段二：开始建树，每次从数组中取两个最小权值"),
                        QString("阶段二：建树")});

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

        m_frames.push_back({ActionType::HF_LIFT, step.a, step.b,
                            static_cast<int>(m_nodes[step.a].weight),
                            static_cast<int>(m_nodes[step.b].weight), {step.kind},
                            step.kind == 0
                                ? QString("%1 与 %2 一起飞向汇合点，准备作为叶子连接")
                                    .arg(m_nodes[step.a].weight).arg(m_nodes[step.b].weight)
                                : QString("带子树的一方上移到与另一方同一行，准备相连"),
                            QString("飞到一起")});

        m_frames.push_back({ActionType::HF_MERGE, step.a, step.b, step.parent,
                            static_cast<int>(m_nodes[step.parent].weight), {step.kind},
                            QString("合并成新节点 %1：%2 + %3 = %4")
                                .arg(step.parent).arg(m_nodes[step.a].weight)
                                .arg(m_nodes[step.b].weight).arg(m_nodes[step.parent].weight),
                            QString("新节点权值 = 左 + 右")});
    }

    const int rootId = m_plan.empty() ? 0 : m_plan.back().parent;
    m_frames.push_back({ActionType::HF_DONE, rootId, static_cast<int>(m_nodes.size()), 0, 0, {},
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
                            static_cast<int>(row.weight), row.codeLength,
                            {static_cast<int>(product), static_cast<int>(running)},
                            QString("WPL 第 %1 项：a[%2] × 码长 = %3 × %4 = %5")
                                .arg(r + 1).arg(row.originalIndex).arg(row.weight)
                                .arg(row.codeLength).arg(product),
                            QString("带权路径长度逐项累加")});
    }

    m_frames.push_back({ActionType::HF_WPL_DONE, static_cast<int>(m_wplTotal), 0, 0, 0, {},
                        QString("WPL = %1").arg(m_wplTotal),
                        QString("WPL = Σ (a[i] × 码长)")});
    return m_frames;
}

void HuffmanTreeModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::HF_RESET) {
        const bool phaseTwo = frame.u == 1;
        for (auto& kv : m_treeNodes) {
            if (!kv.second) continue;
            const HuffmanNodeData* node = nodeAt(kv.first);
            const bool isLeaf = node && node->original;
            kv.second->color = isLeaf ? kHfLeaf : kHfMerge;
            kv.second->setHighlightColor(Qt::transparent);
            kv.second->setOpacity((phaseTwo && isLeaf) ? 1.0 : 0.0);
            kv.second->update();
        }
        for (auto& kv : m_childEdges) {
            kv.second->setColor(kHfEdge);
            kv.second->setPenWidth(2.0);
            kv.second->setPathLabel("");
            kv.second->setOpacity(0.0);
            kv.second->update();
        }
        for (size_t i = 0; i < m_arrayCells.size(); ++i) {
            ArrayBlock* cell = m_arrayCells[i];
            if (!cell) continue;
            if (phaseTwo) {
                cell->color = kHfSorted;
            } else {
                cell->color = kHfEmpty;
                cell->showValue = false;
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
        if (phaseTwo) {
            m_phaseText->setPlainText("阶段二：每次从数组中取两个最小权值合并建树");
            m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
            if (!m_plan.empty()) applyLevels(m_plan.back().levelsAfter, m_animated ? 460 : 0);
        } else {
            m_phaseText->setPlainText("阶段一：把频率数组升序排序");
            m_phaseText->setDefaultTextColor(QColor(70, 110, 170));
        }
        return;
    }

    if (frame.type == ActionType::HF_SORT_LIFT) {
        const int slot = frame.u;
        ArrayBlock* cell = (slot >= 0 && slot < static_cast<int>(m_arrayCells.size()))
                               ? m_arrayCells[slot] : nullptr;
        if (cell) {
            cell->color = kHfPick;
            cell->update();
            if (m_animated) playScalePulse(cell, 1.18, 240);
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
            const QPointF start = QPointF(dest->centerPos().x(), kHoldY);
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

    if (frame.type == ActionType::HF_PICK) {
        const int a = frame.u;
        const int b = frame.v;
        HuffmanNodeData* na = nodeAt(a);
        HuffmanNodeData* nb = nodeAt(b);
        if (na && na->item) {
            na->item->color = kHfPick;
            na->item->setHighlightColor(kHfPick);
            na->item->update();
            if (m_animated) playScalePulse(na->item, 1.16, 240);
        }
        if (nb && nb->item) {
            nb->item->color = kHfPick;
            nb->item->setHighlightColor(kHfPick);
            nb->item->update();
            if (m_animated) playScalePulse(nb->item, 1.16, 240);
        }
        if (m_ptrA && na && na->item) {
            m_ptrA->setOpacity(1);
            retargetPointerTo(m_ptrA, na->item, QPointF(0.0, -(kNodeR + 30.0)));
        }
        if (m_ptrB && nb && nb->item) {
            m_ptrB->setOpacity(1);
            retargetPointerTo(m_ptrB, nb->item, QPointF(0.0, -(kNodeR + 30.0)));
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_LIFT) {
        const int a = frame.u;
        const int b = frame.v;
        HuffmanNodeData* na = nodeAt(a);
        HuffmanNodeData* nb = nodeAt(b);
        if (na && na->item) {
            na->item->setHighlightColor(kHfPick);
            na->item->update();
            if (m_animated) playScalePulse(na->item, 1.24, 320);
        }
        if (nb && nb->item) {
            nb->item->setHighlightColor(kHfPick);
            nb->item->update();
            if (m_animated) playScalePulse(nb->item, 1.24, 320);
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::HF_MERGE) {
        const int parentId = frame.val1;

        int stepIndex = -1;
        for (size_t s = 0; s < m_plan.size(); ++s) {
            if (m_plan[s].parent == parentId) { stepIndex = static_cast<int>(s); break; }
        }
        if (stepIndex < 0) return;
        const HuffmanMergeStep& step = m_plan[stepIndex];

        auto revealEdge = [&](int key) {
            auto it = m_childEdges.find(key);
            if (it == m_childEdges.end()) return;
            it->second->setOpacity(1.0);
            it->second->setColor(kHfEdgeNew);
            it->second->setPenWidth(2.6);
            it->second->update();
        };
        revealEdge(parentId * 4 + 0);
        revealEdge(parentId * 4 + 1);

        HuffmanNodeData* parent = nodeAt(parentId);
        if (parent && parent->item) {
            parent->item->setOpacity(1.0);
            parent->item->color = kHfMerge;
            parent->item->setHighlightColor(kHfMerge);
            parent->item->update();
            if (m_animated) playNodeAppear(parent->item, 460);
        }

        if (step.liftChild >= 0 && step.liftDelta != 0) {
            applyLevels(step.levelsAfterLift, m_animated ? 340 : 0);
            if (m_animated) applyLevels(step.levelsAfter, 480);
            else applyLevels(step.levelsAfter, 0);
        } else {
            applyLevels(step.levelsAfter, m_animated ? 520 : 0);
        }

        if (m_ptrA) m_ptrA->setOpacity(0);
        if (m_ptrB) m_ptrB->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(70, 110, 170));
        }
        return;
    }

    if (frame.type == ActionType::HF_DONE) {
        const int rootId = frame.u;
        auto it = m_treeNodes.find(rootId);
        if (it != m_treeNodes.end() && it->second) {
            it->second->color = kHfRoot;
            it->second->setHighlightColor(kHfRoot);
            it->second->update();
            if (m_animated) playScalePulse(it->second, 1.26, 340);
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
        const long long running = frame.group.size() > 1 ? frame.group[1] : 0;
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
    m_forest.clear();
    m_arrayCells.clear();
    m_codeRows.clear();
    m_codeFreqCells.clear();
    m_codeCells.clear();
    m_codeLenCells.clear();
    m_wplCells.clear();
    m_wplSumCell = nullptr;
    m_treeNodes.clear();
    m_childEdges.clear();
    m_codeMap.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_phaseText = nullptr;
    m_resultText = nullptr;
    m_arrayTitle = nullptr;
    m_tableTitle = nullptr;
    m_tableHead1 = nullptr;
    m_tableHead2 = nullptr;
    m_tableHead3 = nullptr;
    m_wplTitle = nullptr;
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
    m_forest.clear();
    m_arrayCells.clear();
    m_codeRows.clear();
    m_codeFreqCells.clear();
    m_codeCells.clear();
    m_codeLenCells.clear();
    m_wplCells.clear();
    m_wplSumCell = nullptr;
    m_treeNodes.clear();
    m_childEdges.clear();
    m_codeMap.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_phaseText = nullptr;
    m_resultText = nullptr;
    m_arrayTitle = nullptr;
    m_tableTitle = nullptr;
    m_tableHead1 = nullptr;
    m_tableHead2 = nullptr;
    m_tableHead3 = nullptr;
    m_wplTitle = nullptr;
    m_ptrA = nullptr;
    m_ptrB = nullptr;
}
