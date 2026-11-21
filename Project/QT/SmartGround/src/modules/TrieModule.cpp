#include "TrieModule.h"
#include "GraphRenderHelper.h"
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
static const QColor kTrHit = QColor(124, 107, 214);
static const QColor kTrMiss = QColor(226, 87, 60);

TrieModule::TrieModule() {}

QString TrieModule::getModuleName() const { return "Trie 字典树"; }
QString TrieModule::getDefaultInput() const { return "3\ncat\ncar\ndog"; }
QString TrieModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示待插入的单词个数（n <= 6）\n"
           "接下来 n 行，每行一个小写字母组成的单词（长度 2~8）\n"
           "在「Trie 字典树选项」里切换演示模式：\n"
           "  插入建树：逐字符动态建树，已有分支直接复用，新分支才新建节点\n"
           "  查询单词：沿路径逐层下降，命中即绿色脉冲，中途无分支即判定失败\n"
           "  字典序遍历：按字符升序深度优先游走，逐词输出";
}
QString TrieModule::moduleSettingsTitle() const { return "Trie 字典树选项"; }

QVector<ModuleField> TrieModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeSegment("mode", "演示模式",
                                            {"插入建树", "查询单词", "字典序遍历"}, m_mode));
    fields.append(ModuleField::makeText("query", "查询单词", m_queryWord.isEmpty() ? QString("car") : m_queryWord));
    return fields;
}

void TrieModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "mode") m_mode = qBound(0, field.intValue, 2);
    if (key == "query") m_queryWord = field.textValue.trimmed().toLower();
}

double TrieModule::slotX(int index) const {
    return 230.0 + index * 54.0;
}

int TrieModule::findChild(int parent, char ch) const {
    if (parent < 0 || parent >= static_cast<int>(m_nodes.size())) return -1;
    for (int child : m_nodes[parent].children) {
        auto it = m_nodeChars.find(child);
        if (it != m_nodeChars.end() && it->second == ch) return child;
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
    edge->setPathLabel(QString(QChar(m_nodes[childId].ch)));
    edge->setPathLabelColor(QColor(80, 92, 112));
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
    const double treeTop = 132.0;
    const double treeBottom = 410.0;
    const double levelGap = maxDepth > 0 ? (treeBottom - treeTop) / maxDepth : 76.0;
    int maxLevelCount = 1;
    for (int c : levelCount) maxLevelCount = qMax(maxLevelCount, c);
    const double leafGap = qMin(74.0, 660.0 / maxLevelCount);
    const double centerX = 520.0;
    for (int i = 0; i < count; ++i) {
        const int d = depth[i];
        const double span = levelCount[d] * leafGap;
        const double x = centerX - span / 2.0 + levelIndex[d] * leafGap + leafGap / 2.0;
        const double y = treeTop + d * levelGap;
        ++levelIndex[d];
        layoutNode(i, x, y);
    }
}

void TrieModule::buildOrderRow() {
    GraphRenderHelper::addLabel(m_scene, "字典序输出", QPointF(0, 668));
    m_orderItems.clear();
    for (int i = 0; i < 8; ++i) {
        QGraphicsTextItem* item = m_scene->addText("");
        item->setFont(UiFonts::mono(13, true));
        item->setDefaultTextColor(QColor(40, 140, 70));
        item->setPos(96 + i * 92.0, 664);
        item->setZValue(2);
        m_orderItems.push_back(item);
    }
}

void TrieModule::clearOrderRow() {
    for (QGraphicsTextItem* item : m_orderItems) {
        item->setPlainText("");
    }
}

void TrieModule::setCharRow(const QString& word, int focusIndex) {
    for (size_t i = 0; i < m_charCells.size(); ++i) {
        const bool has = i < static_cast<size_t>(word.size());
        m_charCells[i]->setCustomValue(has ? QString(word.at(static_cast<int>(i))) : QString("·"));
        m_charCells[i]->color = (has && static_cast<int>(i) == focusIndex)
            ? kTrCursor : (has ? QColor(226, 238, 252) : QColor(250, 250, 252));
        m_charCells[i]->update();
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

    m_queryWord = m_queryWord.trimmed().toLower();
    if (m_queryWord.isEmpty()) m_queryWord = m_words.empty() ? QString() : m_words.front();

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

    m_charRowLabel = scene->addText(m_mode == 1 ? "查询单词" : "当前单词");
    m_charRowLabel->setFont(UiFonts::cjk(11, true));
    m_charRowLabel->setDefaultTextColor(QColor(60, 75, 95));
    m_charRowLabel->setPos(40, rowY + 8);

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

    buildOrderRow();

    TrieNode root;
    root.id = 0;
    root.ch = 0;
    root.parent = -1;
    m_nodes.push_back(root);
    createNodeItem(0);
    m_nodes[0].node->setCustomValueText(QString("根"));
    ensureEndBlock(0);

    if (m_mode != 0) buildStaticTrie();
    rebuildLayout();
}

void TrieModule::buildStaticTrie() {
    int nodeCount = 1;
    for (const QString& word : m_words) {
        int cursor = 0;
        for (int pos = 0; pos < word.size(); ++pos) {
            const char ch = word.at(pos).toLatin1();
            int existing = -1;
            for (int child : m_nodes[cursor].children) {
                auto it = m_nodeChars.find(child);
                if (it != m_nodeChars.end() && it->second == ch) {
                    existing = child;
                    break;
                }
            }
            if (existing < 0) {
                const int newId = nodeCount++;
                while (static_cast<int>(m_nodes.size()) <= newId) {
                    TrieNode node;
                    node.id = static_cast<int>(m_nodes.size());
                    node.parent = -1;
                    m_nodes.push_back(node);
                }
                m_nodes[newId].id = newId;
                m_nodes[newId].ch = ch;
                m_nodes[newId].parent = cursor;
                m_nodeChars[newId] = ch;
                m_nodes[cursor].children.push_back(newId);
                createNodeItem(newId);
                createEdgeItem(newId);
                ensureEndBlock(newId);
                existing = newId;
            }
            cursor = existing;
        }
        m_nodes[cursor].isEnd = true;
        m_endNodeIds.push_back(cursor);
    }
    m_staticNodeCount = nodeCount;
}

std::vector<AnimFrame> TrieModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    if (m_mode == 0) return runInsertMode();
    if (m_mode == 1) return runQueryMode();
    return runOrderMode();
}

std::vector<AnimFrame> TrieModule::runInsertMode() {
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

std::vector<AnimFrame> TrieModule::runQueryMode() {
    const int nodeCount = m_staticNodeCount;
    const QString query = m_queryWord;

    m_frames.push_back({ActionType::TR_QUERY_RESET, nodeCount, 0, 0, 0, m_endNodeIds,
                        QString("建树完成，共 %1 个节点，现在查询 \"%2\"").arg(nodeCount).arg(query),
                        QString("查询 %1").arg(query)});

    int cursor = 0;
    for (int pos = 0; pos < query.size(); ++pos) {
        const char ch = query.at(pos).toLatin1();
        m_frames.push_back({ActionType::TR_QUERY_CHAR, cursor, 0, ch, pos, {},
                            QString("取第 %1 个字符 '%2'，从节点 %3 往下找").arg(pos + 1).arg(QString(QChar(ch))).arg(cursor),
                            QString("查 '%1'").arg(QString(QChar(ch)))});
        int existing = -1;
        for (int child : m_nodes[cursor].children) {
            auto it = m_nodeChars.find(child);
            if (it != m_nodeChars.end() && it->second == ch) {
                existing = child;
                break;
            }
        }
        if (existing >= 0) {
            m_frames.push_back({ActionType::TR_QUERY_HIT, cursor, existing, ch, pos, {},
                                QString("找到 '%1' 分支，节点 %2 存在，游标下移")
                                    .arg(QString(QChar(ch))).arg(existing),
                                QString("p = %1（命中）").arg(existing)});
            cursor = existing;
        } else {
        m_frames.push_back({ActionType::TR_QUERY_MISS, cursor, 1, ch, pos, {},
                            QString("节点 %1 没有 '%2' 子分支 ⇒ 查询失败").arg(cursor).arg(QString(QChar(ch))),
                            QString("查询失败：无此分支")});
            m_frames.push_back({ActionType::TR_DONE, nodeCount, 1, 0, 0, {},
                                QString("结论：字典树中不存在 \"%1\"").arg(query),
                                QString("查询失败")});
            return m_frames;
        }
    }

    const bool found = m_nodes[cursor].isEnd;
    if (found) {
        m_frames.push_back({ActionType::TR_QUERY_FOUND, cursor, 0, 0, 0, {},
                            QString("到达末节点 %1，end = 1 ⇒ 查询成功").arg(cursor),
                            QString("end[%1] = 1 ⇒ 命中").arg(cursor)});
    } else {
        m_frames.push_back({ActionType::TR_QUERY_MISS, cursor, 0, 0, static_cast<int>(query.size()), {},
                            QString("走到节点 %1，但它的 end = 0 ⇒ 只是某个单词的前缀，查询失败").arg(cursor),
                            QString("end[%1] = 0 ⇒ 未命中").arg(cursor)});    }

    m_frames.push_back({ActionType::TR_DONE, nodeCount, found ? 1 : 0, 0, 0, {},
                        found ? QString("结论：字典树中存在 \"%1\"").arg(query)
                              : QString("结论：字典树中不存在 \"%1\"").arg(query),
                        found ? QString("查询成功") : QString("查询失败")});
    return m_frames;
}

std::vector<AnimFrame> TrieModule::runOrderMode() {
    m_frames.push_back({ActionType::TR_ORDER_START, m_staticNodeCount, 0, 0, 0, m_endNodeIds,
                        QString("建树完成，共 %1 个节点，现在按字典序遍历").arg(m_staticNodeCount),
                        QString("DFS：子节点按字符升序访问")});

    struct Frame { int node; QString prefix; };
    std::vector<Frame> pending;
    pending.push_back({0, QString()});

    std::vector<int> outputNodes;
    int guard = 0;
    while (!pending.empty() && guard < 400) {
        ++guard;
        Frame f = pending.back();
        pending.pop_back();
        std::vector<int> kids = m_nodes[f.node].children;
        std::sort(kids.begin(), kids.end(), [&](int a, int b) {
            auto ia = m_nodeChars.find(a);
            auto ib = m_nodeChars.find(b);
            const char ca = (ia == m_nodeChars.end()) ? 0 : ia->second;
            const char cb = (ib == m_nodeChars.end()) ? 0 : ib->second;
            return ca < cb;
        });
        for (int i = static_cast<int>(kids.size()) - 1; i >= 0; --i) {
            const int kid = kids[i];
            auto it = m_nodeChars.find(kid);
            const QString childPrefix = f.prefix + QString(QChar(it == m_nodeChars.end() ? 0 : it->second));
            pending.push_back({kid, childPrefix});
        }
        const QString here = f.prefix;
        m_frames.push_back({ActionType::TR_ORDER_NODE, f.node, 0, 0, 0, {},
                            here.isEmpty() ? QString("从根节点开始遍历")
                                           : QString("进入前缀 \"%1\" 对应的子树").arg(here),
                            here.isEmpty() ? QString("根") : QString("前缀 %1").arg(here)});
        if (f.node != 0 && m_nodes[f.node].isEnd) {
            outputNodes.push_back(f.node);
            m_frames.push_back({ActionType::TR_ORDER_NODE, f.node, 1, static_cast<int>(outputNodes.size()) - 1, 0, {},
                                QString("节点 %1 是词尾 ⇒ 输出 \"%2\"").arg(f.node).arg(here),
                                QString("end[%1] = 1 ⇒ 输出 %2").arg(f.node).arg(here)});
        }    }

    m_frames.push_back({ActionType::TR_ORDER_DONE, m_staticNodeCount, 0, static_cast<int>(outputNodes.size()), 0, outputNodes,
                        QString("字典序遍历完成，共输出 %1 个单词").arg(outputNodes.size()),
                        QString("输出 %1 个单词").arg(outputNodes.size())});
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
        clearOrderRow();
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
        setCharRow(word, -1);
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

        setCharRow(m_words[frame.u >= 0 && frame.u < static_cast<int>(m_words.size()) ? frame.u : 0], pos);
        if (pos >= 0 && pos < static_cast<int>(m_charCells.size())) {
            retargetPointerTo(m_indexPointer, m_charCells[pos], PointerGeometry::aboveCell(54.0, 38.0));
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("p = %1：找 '%2' → 已有，p 移到 %3")
                                      .arg(from).arg(QString(QChar(ch))).arg(target));
        }
        auto edgeIt = m_edges.find(target);
        if (edgeIt != m_edges.end()) {
            edgeIt->second->setColor(kTrHit);
            edgeIt->second->setPenWidth(3.4);
            edgeIt->second->update();
            playEdgeTravel(edgeIt->second, kTrHit, 440);
        }
        m_cursorPointer->setOpacity(1);
        if (target >= 0 && target < static_cast<int>(m_nodes.size()) && m_nodes[target].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[target].node);
            if (m_animated) playScalePulse(m_nodes[target].node, 1.12, 200);
            playNodeRipple(m_nodes[target].node, kTrHit, 40.0, 520);
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
    } else if (frame.type == ActionType::TR_QUERY_RESET) {
        for (auto& kv : m_endBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(kTrEdge);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->color = kTrIdle;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        clearOrderRow();
        setCharRow(m_queryWord, -1);
        if (m_wordText) {
            setScriptText(m_wordText, QString("查询单词：%1").arg(m_queryWord));
        }
        if (m_stateText) {
            setScriptText(m_stateText, "p → 根节点");
            m_stateText->setDefaultTextColor(QColor(40, 70, 110));
        }
        m_cursorPointer->setOpacity(1);
        if (!m_nodes.empty() && m_nodes[0].node) retargetPointerToNode(m_cursorPointer, m_nodes[0].node);
        m_indexPointer->setOpacity(1);
        if (!m_charCells.empty()) {
            retargetPointerTo(m_indexPointer, m_charCells[0], PointerGeometry::aboveCell(54.0, 38.0));
        }
    } else if (frame.type == ActionType::TR_QUERY_CHAR) {
        const int node = frame.u;
        const char ch = static_cast<char>(frame.val1);
        const int pos = frame.val2;
        setCharRow(m_queryWord, pos);
        if (pos >= 0 && pos < static_cast<int>(m_charCells.size())) {
            retargetPointerTo(m_indexPointer, m_charCells[pos], PointerGeometry::aboveCell(54.0, 38.0));
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("p = %1：查找子分支 '%2'").arg(node).arg(QString(QChar(ch))));
        }
        if (node >= 0 && node < static_cast<int>(m_nodes.size()) && m_nodes[node].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[node].node);
        }
    } else if (frame.type == ActionType::TR_QUERY_HIT) {
        const int from = frame.u;
        const int target = frame.v;
        const char ch = static_cast<char>(frame.val1);
        const int pos = frame.val2;
        setCharRow(m_queryWord, pos);
        if (pos >= 0 && pos < static_cast<int>(m_charCells.size())) {
            retargetPointerTo(m_indexPointer, m_charCells[pos], PointerGeometry::aboveCell(54.0, 38.0));
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("命中 '%1' 分支 → 节点 %2，游标下移").arg(QString(QChar(ch))).arg(target));
        }
        auto edgeIt = m_edges.find(target);
        if (edgeIt != m_edges.end()) {
            edgeIt->second->setColor(kTrHit);
            edgeIt->second->setPenWidth(3.4);
            edgeIt->second->update();
            playEdgeTravel(edgeIt->second, kTrHit, 420);
        }
        if (target >= 0 && target < static_cast<int>(m_nodes.size()) && m_nodes[target].node) {
            retargetPointerToNode(m_cursorPointer, m_nodes[target].node);
            if (m_animated) playScalePulse(m_nodes[target].node, 1.14, 200);
            playNodeRipple(m_nodes[target].node, kTrHit, 40.0, 500);
        }
        if (from >= 0 && from < static_cast<int>(m_nodes.size()) && m_nodes[from].node) {
            m_nodes[from].node->setHighlightColor(Qt::transparent);
            m_nodes[from].node->update();
        }
    } else if (frame.type == ActionType::TR_QUERY_MISS) {
        const int node = frame.u;
        const char ch = static_cast<char>(frame.val1);
        if (frame.v == 1) {
            setCharRow(m_queryWord, frame.val2);
            if (m_stateText) {
                setScriptText(m_stateText, QString("节点 %1 没有 '%2' 子分支 ⇒ 查询失败").arg(node).arg(QString(QChar(ch))));
            }
        } else {
            if (m_stateText) {
                setScriptText(m_stateText, QString("节点 %1 的 end = 0，只是前缀 ⇒ 查询失败").arg(node));
            }
        }
        if (node >= 0 && node < static_cast<int>(m_nodes.size()) && m_nodes[node].node) {
            GraphNode* g = m_nodes[node].node;
            g->color = kTrMiss;
            g->setHighlightColor(kTrMiss);
            g->update();
            if (m_animated) {
                playScalePulse(g, 1.26, 280);
                playNodeRipple(g, kTrMiss, 48.0, 620);
            }
        }
        m_cursorPointer->setOpacity(0);
    } else if (frame.type == ActionType::TR_QUERY_FOUND) {
        const int node = frame.u;
        ensureEndBlock(node);
        if (m_endBlocks.count(node)) {
            m_endBlocks[node]->setCustomValue("1");
            m_endBlocks[node]->color = kTrEnd;
            m_endBlocks[node]->update();
            if (m_animated) {
                playScalePulse(m_endBlocks[node], 1.24, 300);
                playCellRipple(m_endBlocks[node], kTrEnd, 1.2, 600);
            }
        }
        if (node >= 0 && node < static_cast<int>(m_nodes.size()) && m_nodes[node].node) {
            GraphNode* g = m_nodes[node].node;
            g->color = kTrEnd;
            g->update();
            if (m_animated) {
                playScalePulse(g, 1.24, 260);
                playNodeRipple(g, kTrEnd, 48.0, 620);
            }
        }
        if (m_stateText) {
            setScriptText(m_stateText, QString("末节点 %1 的 end = 1 ⇒ 查询成功").arg(node));
        }
    } else if (frame.type == ActionType::TR_ORDER_START) {
        for (auto& kv : m_endBlocks) {
            kv.second->setCustomValue("0");
            kv.second->color = Qt::white;
            kv.second->update();
        }
        for (auto& kv : m_edges) {
            kv.second->setColor(kTrEdge);
            kv.second->setPenWidth(2.0);
            kv.second->update();
        }
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->color = kTrIdle;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        clearOrderRow();
        m_indexPointer->setOpacity(0);
        m_cursorPointer->setOpacity(0);
        if (m_stateText) {
            setScriptText(m_stateText, "按字符升序做深度优先遍历");
            m_stateText->setDefaultTextColor(QColor(40, 70, 110));
        }
    } else if (frame.type == ActionType::TR_ORDER_NODE) {
        const int node = frame.u;
        const bool isOutput = frame.v == 1;
        const int outputIndex = frame.val1;
        if (node >= 0 && node < static_cast<int>(m_nodes.size()) && m_nodes[node].node) {
            GraphNode* g = m_nodes[node].node;
            g->color = isOutput ? kTrEnd : kTrCursor;
            g->setHighlightColor(isOutput ? kTrEnd : kTrCursor);
            g->update();
            if (m_animated) {
                playScalePulse(g, isOutput ? 1.24 : 1.10, 240);
                if (isOutput) playNodeRipple(g, kTrEnd, 44.0, 560);
            }
        }
        if (isOutput) {
            const QString sentence = frame.explanation;
            QString word;
            const int marker = sentence.indexOf(QString("\""));
            if (marker >= 0) {
                const int end = sentence.indexOf(QString("\""), marker + 1);
                if (end > marker) word = sentence.mid(marker + 1, end - marker - 1);
            }
            if (outputIndex >= 0 && outputIndex < static_cast<int>(m_orderItems.size())) {
                m_orderItems[outputIndex]->setPlainText(word);
                if (m_animated) playFadeItem(m_orderItems[outputIndex], 0.0, 1.0, 320);
            }
            ensureEndBlock(node);
            if (m_endBlocks.count(node)) {
                m_endBlocks[node]->setCustomValue("1");
                m_endBlocks[node]->color = kTrEnd;
                m_endBlocks[node]->update();
                if (m_animated) playScalePulse(m_endBlocks[node], 1.24, 300);
            }
            if (m_stateText) {
                setScriptText(m_stateText, sentence);
            }
        }
    } else if (frame.type == ActionType::TR_ORDER_DONE) {
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        for (auto& kv : m_edges) {
            if (kv.second->color == kTrHit || kv.second->color == QColor(235, 155, 55)) {
                kv.second->setColor(kTrEdge);
                kv.second->setPenWidth(2.0);
                kv.second->update();
            }
        }
        if (m_stateText) {
            setScriptText(m_stateText, frame.explanation);
        }
    } else if (frame.type == ActionType::TR_DONE) {
        for (auto& n : m_nodes) {
            if (!n.node) continue;
            n.node->setHighlightColor(Qt::transparent);
            n.node->update();
        }
        for (auto& kv : m_edges) {
            if (kv.second->color == QColor(235, 155, 55) || kv.second->color == kTrHit) {
                kv.second->setColor(kTrEdge);
                kv.second->setPenWidth(2.0);
                kv.second->update();
            }
        }
        m_cursorPointer->setOpacity(0);
        m_indexPointer->setOpacity(0);
        if (m_stateText) {
            setScriptText(m_stateText, m_mode == 0
                ? QString("建树完成，共 %1 个节点").arg(frame.u)
                : frame.explanation);
        }
    }
}

void TrieModule::reset() {
    m_frames.clear();
    m_nodes.clear();
    m_edges.clear();
    m_endBlocks.clear();
    m_charCells.clear();
    m_orderItems.clear();
    m_words.clear();
    m_nodeChars.clear();
    m_endNodeIds.clear();
    m_mathText = nullptr;
    m_wordText = nullptr;
    m_stateText = nullptr;
    m_orderText = nullptr;
    m_charRowLabel = nullptr;
    m_charBlocks = nullptr;
    m_cursorPointer = nullptr;
    m_indexPointer = nullptr;
    m_valid = false;
    m_staticNodeCount = 0;
}

void TrieModule::releaseSceneItems() {
    m_nodes.clear();
    m_edges.clear();
    m_endBlocks.clear();
    m_charCells.clear();
    m_orderItems.clear();
    m_endNodeIds.clear();
    m_mathText = nullptr;
    m_wordText = nullptr;
    m_stateText = nullptr;
    m_orderText = nullptr;
    m_charRowLabel = nullptr;
    m_charBlocks = nullptr;
    m_cursorPointer = nullptr;
    m_indexPointer = nullptr;
}
