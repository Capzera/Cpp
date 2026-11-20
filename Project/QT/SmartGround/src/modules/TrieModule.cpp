#include "TrieModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kTrIdle = QColor(252, 252, 254);
static const QColor kTrNew = QColor(255, 210, 145);
static const QColor kTrEnd = QColor(140, 205, 165);
static const QColor kTrCursor = QColor(255, 235, 180);
static const QColor kTrEdge = QColor(196, 204, 216);

TrieModule::TrieModule() {}

QString TrieModule::getModuleName() const { return "Trie 字典树"; }
QString TrieModule::getDefaultInput() const { return "3\ncat\ncar\ndog"; }
QString TrieModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示待插入的单词个数（n <= 6）\n"
           "接下来 n 行，每行一个小写字母组成的单词（长度 2~8）\n"
           "算法沿字符逐层向下走：已有分支直接复用，新分支才新建节点";
}

double TrieModule::slotX(int index) const {
    return 230.0 + index * 54.0;
}

int TrieModule::findChild(int parent, char ch) const {
    if (parent < 0 || parent >= static_cast<int>(m_nodes.size())) return -1;
    for (int child : m_nodes[parent].children) {
        if (m_nodes[child].ch == ch) return child;
    }
    return -1;
}

void TrieModule::ensureEndBlock(int index) {
    if (index < 0) return;
    if (m_endBlocks.count(index)) return;
    ArrayBlock* block = new ArrayBlock(index, 0, QString::number(index));
    block->setCellSize(54, 38);
    block->setShowLabel(false);
    block->setCustomValue("0");
    block->setPos(slotX(index), 596);
    m_scene->addItem(block);
    m_endBlocks[index] = block;
}

void TrieModule::createNodeItem(int id) {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return;
    if (m_nodes[id].node) return;
    GraphNode* node = new GraphNode(id);
    node->color = kTrIdle;
    node->setCustomValueText(QString(QChar(m_nodes[id].ch)));
    m_scene->addItem(node);
    m_nodes[id].node = node;
}

void TrieModule::createEdgeItem(int childId) {
    if (childId <= 0 || childId >= static_cast<int>(m_nodes.size())) return;
    if (m_edges.count(childId)) return;
    const int parentId = m_nodes[childId].parent;
    if (parentId < 0) return;
    if (!m_nodes[parentId].node) createNodeItem(parentId);
    if (!m_nodes[childId].node) createNodeItem(childId);
    GraphEdge* edge = new GraphEdge(m_nodes[parentId].node, m_nodes[childId].node, true);
    edge->color = kTrEdge;
    m_scene->addItem(edge);
    m_nodes[parentId].node->addEdge(edge);
    m_nodes[childId].node->addEdge(edge);
    m_edges[childId] = edge;
}

void TrieModule::layoutNode(int id, double x, double y) {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) return;
    if (!m_nodes[id].node) createNodeItem(id);
    if (m_nodes[id].node) m_nodes[id].node->setPos(x, y);
}

void TrieModule::rebuildLayout() {
    const int count = static_cast<int>(m_nodes.size());
    std::vector<int> depth(count, 0);
    int maxDepth = 0;
    for (int i = 1; i < count; ++i) {
        depth[i] = depth[m_nodes[i].parent] + 1;
        maxDepth = qMax(maxDepth, depth[i]);
    }
    std::vector<int> levelCount(maxDepth + 1, 0);
    for (int i = 0; i < count; ++i) {
        if (i == 0) { levelCount[0] = 1; continue; }
        ++levelCount[depth[i]];
    }
    std::vector<int> levelIndex(maxDepth + 1, 0);
    const double levelGap = 76.0;
    const double leafGap = 74.0;
    const double centerX = 520.0;
    for (int i = 0; i < count; ++i) {
        const int d = depth[i];
        const double span = levelCount[d] * leafGap;
        const double x = centerX - span / 2.0 + levelIndex[d] * leafGap + leafGap / 2.0;
        const double y = 140.0 + d * levelGap;
        ++levelIndex[d];
        layoutNode(i, x, y);
    }
}

void TrieModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!lines.isEmpty()) {
        n = lines[0].trimmed().toInt(&ok);
        m_valid = ok && n >= 1 && n <= 6 && lines.size() >= n + 1;
    }

    m_mathText = createScriptText(scene, m_valid ? "Trie 字典树：逐字符动态建树，已有分支直接复用"
                                        : "输入格式：n，随后输入 n 个小写单词；n≤6");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(150, 18);
    if (!m_valid) return;

    for (int i = 1; i <= n; ++i) {
        const QString w = lines[i].trimmed();
        if (w.size() < 2 || w.size() > 8) {
            m_valid = false;
            break;
        }
        m_words.push_back(w);
    }
    if (!m_valid) return;

    m_wordText = createScriptText(scene, "");
    m_wordText->setFont(UiFonts::cjk(13, true));
    m_wordText->setDefaultTextColor(QColor(60, 75, 95));
    m_wordText->setPos(150, 62);

    m_stateText = createScriptText(scene, "");
    m_stateText->setFont(UiFonts::mono(14, true));
    m_stateText->setDefaultTextColor(QColor(40, 70, 110));
    m_stateText->setPos(150, 92);

    const int maxLen = 8;
    const double cellW = 54.0;
    const double rowY = 500.0;
    const double startX = 230.0;

    for (int i = 0; i < maxLen; ++i) {
        ArrayBlock* cell = new ArrayBlock(i, 0, QString::number(i));
        cell->setCellSize(cellW, 38);
        cell->setShowLabel(false);
        cell->setCustomValue("·");
        cell->color = QColor(250, 250, 252);
        cell->setPos(startX + i * cellW, rowY);
        scene->addItem(cell);
        m_charCells.push_back(cell);
    }

    QGraphicsTextItem* rowLabel = scene->addText("当前单词");
    rowLabel->setFont(UiFonts::cjk(11, true));
    rowLabel->setDefaultTextColor(QColor(60, 75, 95));
    rowLabel->setPos(40, rowY + 8);

    QGraphicsTextItem* slotLabel = scene->addText("节点槽位（树内存）");
    slotLabel->setFont(UiFonts::cjk(11, true));
    slotLabel->setDefaultTextColor(QColor(60, 75, 95));
    slotLabel->setPos(40, 608);

    m_indexPointer = new PointerBlock("i", QColor(210, 80, 55));
    m_indexPointer->setOpacity(0);
    m_indexPointer->setDirection(PointerBlock::Down);
    m_indexPointer->setShaftLength(26);
    m_indexPointer->setAnchorOffset(PointerGeometry::aboveCell(54.0, 38.0));
    m_indexPointer->setLabelAtTail(10.0);
    scene->addItem(m_indexPointer);

    m_cursorPointer = new PointerBlock("p", QColor(60, 140, 220));
    m_cursorPointer->setOpacity(0);
    m_cursorPointer->setLabelAbove(true);
    scene->addItem(m_cursorPointer);

    TrieNode root;
    root.id = 0;
    root.ch = 0;
    root.parent = -1;
    m_nodes.push_back(root);
    createNodeItem(0);
    m_nodes[0].node->setCustomValueText(QString("根"));
    rebuildLayout();
    ensureEndBlock(0);
}

std::vector<AnimFrame> TrieModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::map<int, std::vector<int>> children;
    children[0] = {};

    m_frames.push_back({ActionType::TR_RESET, 0, 0, 0, 0, {},
                        QString("初始化：画布上只有根节点，游标 p 指向根节点"),
                        QString("p = 根节点（编号 0）")});

    int nodeCount = 1;

    for (int w = 0; w < static_cast<int>(m_words.size()); ++w) {
        const QString word = m_words[w];
        m_frames.push_back({ActionType::TR_WORD, w, 0, 0, 0, {},
                            QString("取第 %1 个单词 \"%2\"").arg(w + 1).arg(word),
                            QString("插入 %1").arg(word)});

        int cursor = 0;
        for (int pos = 0; pos < word.size(); ++pos) {
            const char ch = word.at(pos).toLatin1();
            int existing = -1;
            for (int child : children[cursor]) {
                if (child < nodeCount && m_nodeChars.count(child) && m_nodeChars[child] == ch) {
                    existing = child;
                    break;
                }
            }
            if (existing >= 0) {
                m_frames.push_back({ActionType::TR_HIT, cursor, existing, ch, pos, {},
                                    QString("节点 %1 已有子节点 '%2'（编号 %3），直接复用")
                                        .arg(cursor).arg(QString(QChar(ch))).arg(existing),
                                    QString("p = %1（复用）").arg(existing)});
                cursor = existing;
            } else {
                const int newId = nodeCount++;
                m_nodeChars[newId] = ch;
                children[cursor].push_back(newId);
                children[newId] = {};
                m_frames.push_back({ActionType::TR_CREATE, cursor, newId, ch, pos, {},
                                    QString("节点 %1 没有 '%2' 子节点，新建节点 %3")
                                        .arg(cursor).arg(QString(QChar(ch))).arg(newId),
                                    QString("新建节点 %1").arg(newId)});
                cursor = newId;
            }
        }
        m_frames.push_back({ActionType::TR_END, cursor, word.size(), 0, 0, {},
                            QString("单词 \"%1\" 插入完毕，末节点 %2 标记 end = 1").arg(word).arg(cursor),
                            QString("end[%1] = 1").arg(cursor)});
    }

    m_frames.push_back({ActionType::TR_DONE, nodeCount, 0, 0, 0, {},
                        QString("字典树共 %1 个节点，公共前缀只建一份").arg(nodeCount),
                        QString("节点总数 = %1").arg(nodeCount)});
    return m_frames;
}

void TrieModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::TR_RESET) {
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->color = kTrIdle;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(kTrEdge);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& kv : m_endBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (size_t i = 0; i < m_charCells.size(); ++i) {
            m_charCells[i]->setCustomValue("·");
            m_charCells[i]->color = QColor(250, 250, 252);
            m_charCells[i]->update();
        }
        if (m_wordText) setScriptText(m_wordText, "");
        if (m_stateText) {
            setScriptText(m_stateText, "p → 根节点");
            m_stateText->setDefaultTextColor(QColor(40, 70, 110));
        }
        m_cursorPointer->setOpacity(1);
        if (m_nodes[0].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[0].node);
        }
        m_indexPointer->setOpacity(0);
    } else if (frame.type == ActionType::TR_WORD) {
        const int wordIndex = frame.u;
        const QString word = wordIndex >= 0 && wordIndex < static_cast<int>(m_words.size())
            ? m_words[wordIndex] : QString();
        if (m_wordText) {
            setScriptText(m_wordText, QString("当前单词：%1").arg(word));
        }
        for (size_t i = 0; i < m_charCells.size(); ++i) {
            m_charCells[i]->setCustomValue(i < static_cast<size_t>(word.size())
                ? QString(word.at(static_cast<int>(i))) : QString("·"));
            m_charCells[i]->color = i < static_cast<size_t>(word.size())
                ? QColor(226, 238, 252) : QColor(250, 250, 252);
            m_charCells[i]->update();
        }
        m_indexPointer->setOpacity(1);
        if (!m_charCells.empty()) {
            retargetPointerTo(m_indexPointer, m_charCells[0], PointerGeometry::aboveCell(54.0, 38.0));
        }
        m_cursorPointer->setOpacity(1);
        if (m_nodes[0].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[0].node);
        }
    } else if (frame.type == ActionType::TR_HIT) {
        const int from = frame.u;
        const int target = frame.v;
        const char ch = static_cast<char>(frame.val1);
        const int pos = frame.val2;

        if (pos >= 0 && pos < static_cast<int>(m_charCells.size())) {
            m_charCells[pos]->color = kTrCursor;
            m_charCells[pos]->update();
            retargetPointerTo(m_indexPointer, m_charCells[pos], PointerGeometry::aboveCell(54.0, 38.0));
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("p = %1：找 '%2' → 已有，p 移到 %3")
                                      .arg(from).arg(QString(QChar(ch))).arg(target));
        }
        auto edgeIt = m_edges.find(target);
        if (edgeIt != m_edges.end()) {
            edgeIt->second->setColor(QColor(80, 175, 120));
            edgeIt->second->setPenWidth(3.0);
            edgeIt->second->update();
            playEdgeTravel(edgeIt->second, QColor(80, 175, 120), 440);
        }
        m_cursorPointer->setOpacity(1);
        if (target >= 0 && target < static_cast<int>(m_nodes.size()) && m_nodes[target].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[target].node);
            if (m_animated) playScalePulse(m_nodes[target].node, 1.12, 200);
            playNodeRipple(m_nodes[target].node, QColor(110, 210, 150), 40.0, 520);
        }
    } else if (frame.type == ActionType::TR_CREATE) {
        const int parentId = frame.u;
        const int newId = frame.v;
        const char ch = static_cast<char>(frame.val1);
        const int pos = frame.val2;

        while (static_cast<int>(m_nodes.size()) <= newId) {
            TrieNode node;
            node.id = static_cast<int>(m_nodes.size());
            node.parent = -1;
            m_nodes.push_back(node);
        }
        m_nodes[newId].id = newId;
        m_nodes[newId].ch = ch;
        m_nodes[newId].parent = parentId;
        m_nodeChars[newId] = ch;
        bool alreadyLinked = false;
        for (int child : m_nodes[parentId].children) {
            if (child == newId) { alreadyLinked = true; break; }
        }
        if (!alreadyLinked) m_nodes[parentId].children.push_back(newId);

        createNodeItem(newId);
        createEdgeItem(newId);
        ensureEndBlock(newId);
        rebuildLayout();

        if (pos >= 0 && pos < static_cast<int>(m_charCells.size())) {
            m_charCells[pos]->color = kTrCursor;
            m_charCells[pos]->update();
            m_indexPointer->setAnchorItem(m_charCells[pos]);
            retargetPointerTo(m_indexPointer, m_charCells[pos], PointerGeometry::aboveCell(54.0, 38.0));
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("p = %1：找 '%2' → 没有，新建节点 %3，p 移到 %3")
                                      .arg(parentId).arg(QString(QChar(ch))).arg(newId));
        }
        auto edgeIt = m_edges.find(newId);
        if (edgeIt != m_edges.end()) {
            edgeIt->second->setColor(QColor(235, 155, 55));
            edgeIt->second->setPenWidth(3.2);
            edgeIt->second->update();
            playEdgeGrow(edgeIt->second, 260);
        }
        if (m_nodes[newId].node) {
            m_nodes[newId].node->color = kTrNew;
            m_nodes[newId].node->setHighlightColor(kTrNew);
            m_nodes[newId].node->update();
            if (m_animated) {
                playScalePulse(m_nodes[newId].node, 1.3, 300);
                playNodeRipple(m_nodes[newId].node, kTrNew, 50.0, 660);
            }
        }
        m_cursorPointer->setOpacity(1);
        if (m_nodes[newId].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[newId].node);
        }
    } else if (frame.type == ActionType::TR_END) {
        const int endNode = frame.u;
        ensureEndBlock(endNode);
        if (m_endBlocks.count(endNode)) {
            m_endBlocks[endNode]->setCustomValue("1");
            m_endBlocks[endNode]->color = kTrEnd;
            m_endBlocks[endNode]->update();
            if (m_animated) {
                playScalePulse(m_endBlocks[endNode], 1.24, 300);
                playCellRipple(m_endBlocks[endNode], kTrEnd, 1.2, 600);
            }
        }
        if (endNode >= 0 && endNode < static_cast<int>(m_nodes.size()) && m_nodes[endNode].node) {
            m_nodes[endNode].node->color = kTrEnd;
            m_nodes[endNode].node->update();
            if (m_animated) playScalePulse(m_nodes[endNode].node, 1.18, 240);
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("p = %1：单词结束，标记 end = 1").arg(endNode));
        }
    } else if (frame.type == ActionType::TR_DONE) {
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        for (auto& kv : m_edges) {
            if (kv.second->color == QColor(235, 155, 55)) {
                kv.second->setColor(kTrEdge);
                kv.second->setPenWidth(2.0);
                kv.second->update();
            }
        }
        m_cursorPointer->setOpacity(0);
        m_indexPointer->setOpacity(0);
        if (m_stateText) {
            setScriptText(m_stateText, QString("建树完成，共 %1 个节点").arg(frame.u));
        }
    }
}

void TrieModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_endBlocks.clear();
    m_charCells.clear();
    m_words.clear();
    m_nodeChars.clear();
    m_mathText = nullptr;
    m_wordText = nullptr;
    m_stateText = nullptr;
    m_charBlocks = nullptr;
    m_cursorPointer = nullptr;
    m_indexPointer = nullptr;
    m_valid = false;
}

void TrieModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_endBlocks.clear();
    m_charCells.clear();
    m_mathText = nullptr;
    m_wordText = nullptr;
    m_stateText = nullptr;
    m_charBlocks = nullptr;
    m_cursorPointer = nullptr;
    m_indexPointer = nullptr;
}
