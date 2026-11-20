#include "SegmentTreeQueryModule.h"
#include "ui/UiTheme.h"
#include <QFont>

static const QColor kQRed = QColor(238, 120, 108);
static const QColor kQOrange = QColor(250, 196, 110);
static const QColor kQGreen = QColor(140, 205, 150);
static const QColor kQBlue = QColor(150, 185, 235);
static const QColor kQYellow = QColor(255, 214, 140);

SegmentTreeQueryModule::SegmentTreeQueryModule() {}

QString SegmentTreeQueryModule::getModuleName() const { return "线段树 - 区间查询"; }
QString SegmentTreeQueryModule::getDefaultInput() const { return "4 1 100\n1 2 3 4\n3 2 4"; }
QString SegmentTreeQueryModule::getInputHint() const {
    return "第一行输入 n、m、p：数组长度、操作次数、模数\n"
           "接下来一行输入 n 个整数，即 a[1..n]\n"
           "再接下来 m 行，每行一个操作：\n"
           "  1 l r k  区间 [l,r] 乘以 k\n"
           "  2 l r k  区间 [l,r] 加上 k\n"
           "  3 l r    查询区间 [l,r] 的和（本模块主演示该操作）";
}

void SegmentTreeQueryModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QString error;
    std::vector<SegmentTreeCommon::Operation> ops;
    m_valid = SegmentTreeCommon::parseOps(inputData, m_arr, m_mod, ops, error);
    m_lt = 0;
    m_rt = 0;
    for (const auto& op : ops) {
        if (op.op == 3) { m_lt = op.l; m_rt = op.r; break; }
    }
    buildTreeScene("querySum(node, l, r, lt, rt)：递归查询区间和，ans 逐段累加", error);
    if (!m_valid) return;
    m_ansBlock = new ArrayBlock(0, 0, "ans");
    m_ansBlock->setPos(760, 300);
    m_ansBlock->color = QColor(255, 248, 220);
    m_scene->addItem(m_ansBlock);
}

std::vector<AnimFrame> SegmentTreeQueryModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    for (int i = 1; i <= m_n; ++i) m_arr[i] = liveValue("a", i, m_arr[i]);
    m_tr.assign(m_slotCount + 1, 0);
    m_add.assign(m_slotCount + 1, 0);
    m_mul.assign(m_slotCount + 1, 1);
    m_answer = 0;
    buildTree(1, 1, m_n);
    m_frames.push_back({ActionType::SEG_OP_START, 1, m_lt, m_rt, 0, {},
                        QString("开始查询区间 [%1,%2]").arg(m_lt).arg(m_rt),
                        QString("querySum(1, 1, n, %1, %2)").arg(m_lt).arg(m_rt)});
    querySum(1, 1, m_n, m_lt, m_rt);
    m_frames.push_back({ActionType::SEG_QUERY_DONE, 1, m_lt, static_cast<int>(m_answer), 0, {},
                        QString("查询完成，区间和为 %1").arg(m_answer),
                        QString("ans = %1").arg(m_answer)});
    return m_frames;
}

void SegmentTreeQueryModule::buildTree(int node, int l, int r) {
    m_mul[node] = 1;
    m_frames.push_back({ActionType::SEG_INIT_MUL, node, 0, 0, 0, {},
                        QString("建树进入 node=%1，区间 [%2,%3]").arg(node).arg(l).arg(r),
                        QString("mul[%1] = 1").arg(node)});
    if (l == r) {
        m_tr[node] = m_arr[l] % m_mod;
        m_frames.push_back({ActionType::SEG_LEAF, node, l, static_cast<int>(m_tr[node]), 0, {},
                            QString("叶子 node=%1 取 a[%2]").arg(node).arg(l),
                            QString("tr[%1] = a[%2] %% p = %3").arg(node).arg(l).arg(m_tr[node])});
        return;
    }
    int mid = (l + r) >> 1;
    m_frames.push_back({ActionType::SEG_DESCEND, node, mid, SegmentTreeCommon::ls(node), SegmentTreeCommon::rs(node), {},
                        QString("node=%1 的区间 [%2,%3] 被中点 %4 一分为二").arg(node).arg(l).arg(r).arg(mid),
                        QString("mid = (%1+%2)>>1 = %3　ls = %1<<1 = %4　rs = %1<<1|1 = %5")
                            .arg(node).arg(r).arg(mid).arg(SegmentTreeCommon::ls(node)).arg(SegmentTreeCommon::rs(node))});
    buildTree(SegmentTreeCommon::ls(node), l, mid);
    buildTree(SegmentTreeCommon::rs(node), mid + 1, r);
    pushUp(node);
}

void SegmentTreeQueryModule::pushUp(int node) {
    int lc = SegmentTreeCommon::ls(node);
    int rc = SegmentTreeCommon::rs(node);
    if (lc > m_slotCount || rc > m_slotCount) return;
    m_tr[node] = (m_tr[lc] + m_tr[rc]) % m_mod;
    m_frames.push_back({ActionType::SEG_PUSHUP, node, lc, static_cast<int>(m_tr[lc]), static_cast<int>(m_tr[rc]), {lc, rc},
                        QString("node=%1 汇总两个孩子的区间和").arg(node),
                        QString("tr[%1] = (tr[%2] + tr[%3]) %% mod = %4").arg(node).arg(lc).arg(rc).arg(m_tr[node])});
}

void SegmentTreeQueryModule::pushDown(int node, int l, int r) {
    int lc = SegmentTreeCommon::ls(node);
    int rc = SegmentTreeCommon::rs(node);
    if (lc > m_slotCount || rc > m_slotCount) return;
    if (m_mul[node] == 1 && m_add[node] == 0) return;
    int mid = (l + r) >> 1;
    m_tr[lc] = (m_tr[lc] * m_mul[node] + m_add[node] * (mid - l + 1)) % m_mod;
    m_tr[rc] = (m_tr[rc] * m_mul[node] + m_add[node] * (r - mid)) % m_mod;
    m_add[lc] = (m_add[lc] * m_mul[node] + m_add[node]) % m_mod;
    m_add[rc] = (m_add[rc] * m_mul[node] + m_add[node]) % m_mod;
    m_mul[lc] = (m_mul[lc] * m_mul[node]) % m_mod;
    m_mul[rc] = (m_mul[rc] * m_mul[node]) % m_mod;
    m_frames.push_back({ActionType::SEG_PUSH_DOWN, node, lc, static_cast<int>(m_mul[node]), static_cast<int>(m_add[node]), {},
                        QString("node=%1 的懒标记下发给左右孩子").arg(node),
                        QString("查询前先 push_down，把欠着的标记结算掉")});
    m_mul[node] = 1;
    m_add[node] = 0;
    m_frames.push_back({ActionType::SEG_TAG_CLEAR, node, 0, 0, 0, {},
                        QString("node=%1 的标记已下发").arg(node),
                        QString("mul[%1] = 1　add[%1] = 0").arg(node)});
}

long long SegmentTreeQueryModule::querySum(int node, int l, int r, int lt, int rt) {
    if (l >= lt && r <= rt) {
        m_frames.push_back({ActionType::SEG_QUERY_COVER, node, l, static_cast<int>(m_tr[node]), 0, {},
                            QString("node=%1 区间 [%2,%3] 被完全覆盖，直接取 tr").arg(node).arg(l).arg(r),
                            QString("return tr[%1] = %2").arg(node).arg(m_tr[node])});
        m_answer = (m_answer + m_tr[node]) % m_mod;
        return m_tr[node];
    }
    pushDown(node, l, r);
    int mid = (l + r) >> 1;
    long long res = 0;
    m_frames.push_back({ActionType::SEG_PARTIAL, node, mid, (lt <= mid) ? 1 : 0, (rt > mid) ? 1 : 0, {},
                        QString("node=%1 区间 [%2,%3] 只被部分覆盖，递归处理").arg(node).arg(l).arg(r),
                        QString("if (lt <= mid) → ls　if (rt > mid) → rs")});
    if (lt <= mid) res = (res + querySum(SegmentTreeCommon::ls(node), l, mid, lt, rt)) % m_mod;
    if (rt > mid) res = (res + querySum(SegmentTreeCommon::rs(node), mid + 1, r, lt, rt)) % m_mod;
    m_frames.push_back({ActionType::SEG_QUERY_ACC, node, l, static_cast<int>(res), 0, {},
                        QString("node=%1 的左右两侧结果汇总").arg(node),
                        QString("ans = (ans + querySum(ls,…)) %% mod　当前 ans = %1").arg(m_answer)});
    return res;
}

void SegmentTreeQueryModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::SEG_RESET) {
        resetTreeScene();
    } else if (frame.type == ActionType::SEG_INIT_MUL) {
        setArrayValue("mul", frame.u, 1, kQBlue);
        highlightNode(frame.u, kQRed, QColor(255, 240, 238));
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_DESCEND) {
        highlightNode(frame.u, kQOrange, QColor(255, 248, 232));
        highlightNode(frame.val1, kQOrange, QColor(255, 248, 232));
        GraphNode* child = m_treeNodes.count(frame.val1) ? m_treeNodes[frame.val1] : nullptr;
        if (child) playNodeRipple(child, kQOrange, 34.0, 480);
        auto edgeIt = m_treeEdges.find({frame.u, frame.val1});
        if (edgeIt != m_treeEdges.end()) playEdgeTravel(edgeIt->second, kQOrange, 420);
        appendDescent(frame.u, kQOrange);
        appendDescent(frame.val1, kQOrange);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_LEAF) {
        setArrayValue("tr", frame.u, frame.val1, kQGreen);
        setNodeValue(frame.u, QString("tr=%1").arg(frame.val1));
        highlightNode(frame.u, kQGreen, QColor(240, 252, 242));
        GraphNode* leaf = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (leaf) playNodeRipple(leaf, kQGreen, 42.0, 560);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PUSHUP) {
        ArrayBlock* blk = arrayBlock("tr", frame.u);
        if (blk) {
            int rc = frame.group.size() > 1 ? frame.group[1] : 0;
            ArrayBlock* lb = arrayBlock("tr", frame.v);
            ArrayBlock* rb = arrayBlock("tr", rc);
            QPointF sa = lb ? lb->pos() + QPointF(30, 0) : blk->pos();
            QPointF sb = rb ? rb->pos() + QPointF(30, 0) : blk->pos();
            playDualValueFlight(sa, sb, blk, frame.val1, frame.val2, kQGreen);
        }
        setNodeValue(frame.u, QString("tr=%1").arg((frame.val1 + frame.val2) % m_mod));
        highlightNode(frame.u, kQGreen, QColor(240, 252, 242));
        GraphNode* parent = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (parent) playNodeRipple(parent, kQGreen, 46.0, 600);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_OP_START) {
        clearHighlights();
        if (m_ansBlock) {
            m_ansBlock->value = 0;
            m_ansBlock->showValue = true;
            m_ansBlock->color = QColor(255, 248, 220);
            m_ansBlock->update();
        }
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_QUERY_COVER) {
        ArrayBlock* blk = arrayBlock("tr", frame.u);
        if (blk && m_ansBlock) {
            QPointF from = blk->pos() + QPointF(30, 0);
            playValueFlight(from, m_ansBlock, frame.val1, QColor(255, 230, 150), true);
        }
        setNodeValue(frame.u, QString("tr=%1").arg(frame.val1));
        highlightNode(frame.u, kQYellow, QColor(255, 250, 225));
        GraphNode* cover = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (cover) playNodeRipple(cover, kQYellow, 40.0, 520);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PARTIAL) {
        highlightNode(frame.u, kQOrange, QColor(255, 248, 232));
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PUSH_DOWN) {
        int lc = frame.v;
        int rc = SegmentTreeCommon::rs(frame.u);
        setNodeTag(lc, QString("×%1").arg(frame.val1), kQBlue);
        setNodeTag(rc, QString("×%1").arg(frame.val1), kQBlue);
        highlightNode(frame.u, kQOrange, QColor(255, 248, 232));
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_TAG_CLEAR) {
        setArrayValue("mul", frame.u, 1, kQBlue);
        setArrayValue("add", frame.u, 0, kQBlue);
        clearNodeTag(frame.u);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_QUERY_ACC) {
        if (m_ansBlock) {
            m_ansBlock->value = frame.val1;
            m_ansBlock->showValue = true;
            m_ansBlock->update();
        }
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_QUERY_DONE) {
        if (m_ansBlock) {
            m_ansBlock->value = frame.val1;
            m_ansBlock->showValue = true;
            m_ansBlock->color = QColor(200, 235, 205);
            m_ansBlock->update();
        }
        clearHighlights();
        if (m_ptrNode) m_ptrNode->setOpacity(0);
    }
}
