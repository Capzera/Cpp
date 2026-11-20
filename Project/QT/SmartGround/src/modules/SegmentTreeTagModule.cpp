#include "SegmentTreeTagModule.h"
#include "ui/UiTheme.h"
#include <QFont>

static const QColor kTagRed = QColor(238, 120, 108);
static const QColor kTagOrange = QColor(250, 196, 110);
static const QColor kTagGreen = QColor(140, 205, 150);
static const QColor kTagBlue = QColor(150, 185, 235);
static const QColor kTagYellow = QColor(255, 214, 140);
static const QColor kTagPurple = QColor(206, 176, 232);
static const QColor kTagGray = QColor(228, 231, 236);

SegmentTreeTagModule::SegmentTreeTagModule() {}

QString SegmentTreeTagModule::getModuleName() const { return "线段树 - 区间乘 / 区间加（懒标记）"; }
QString SegmentTreeTagModule::getDefaultInput() const { return "4 2 100\n1 2 3 4\n1 2 3 2\n2 1 4 5"; }
QString SegmentTreeTagModule::getInputHint() const {
    return "第一行输入 n、m、p：数组长度、操作次数、模数\n"
           "接下来一行输入 n 个整数，即 a[1..n]\n"
           "再接下来 m 行，每行一个操作：\n"
           "  1 l r k  区间 [l,r] 乘以 k\n"
           "  2 l r k  区间 [l,r] 加上 k\n"
           "  3 l r    查询区间 [l,r] 的和（本模块展示其内部过程）";
}

std::vector<AnimFrame> SegmentTreeTagModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    for (int i = 1; i <= m_n; ++i) m_arr[i] = liveValue("a", i, m_arr[i]);
    m_tr.assign(m_slotCount + 1, 0);
    m_add.assign(m_slotCount + 1, 0);
    m_mul.assign(m_slotCount + 1, 1);
    buildTree(1, 1, m_n);
    for (const auto& op : m_ops) {
        if (op.op == 1) {
            m_frames.push_back({ActionType::SEG_OP_START, 1, op.l, op.r, static_cast<int>(op.k), {},
                                QString("执行区间乘：updateMul(%1, %2, %3)").arg(op.l).arg(op.r).arg(op.k),
                                QString("op = 1 → st.updateMul(%1, %2, %3)").arg(op.l).arg(op.r).arg(op.k)});
            changeMul(1, 1, m_n, op.l, op.r, op.k % m_mod);
        } else if (op.op == 2) {
            m_frames.push_back({ActionType::SEG_OP_START, 1, op.l, op.r, static_cast<int>(op.k), {},
                                QString("执行区间加：updateAdd(%1, %2, %3)").arg(op.l).arg(op.r).arg(op.k),
                                QString("op = 2 → st.updateAdd(%1, %2, %3)").arg(op.l).arg(op.r).arg(op.k)});
            changeAdd(1, 1, m_n, op.l, op.r, op.k % m_mod);
        } else {
            m_frames.push_back({ActionType::SEG_OP_START, 1, op.l, op.r, 0, {},
                                QString("查询区间 [%1,%2]（本模块展示其内部修改过程）").arg(op.l).arg(op.r),
                                QString("op = 3 → st.query(%1, %2)").arg(op.l).arg(op.r)});
        }
    }
    m_frames.push_back({ActionType::SEG_DONE, 1, 0, 0, 0, {}, "全部操作执行完毕", "所有懒标记已正确下放"});
    return m_frames;
}

void SegmentTreeTagModule::buildTree(int node, int l, int r) {
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

void SegmentTreeTagModule::pushUp(int node) {
    int lc = SegmentTreeCommon::ls(node);
    int rc = SegmentTreeCommon::rs(node);
    if (lc > m_slotCount || rc > m_slotCount) return;
    m_tr[node] = (m_tr[lc] + m_tr[rc]) % m_mod;
    m_frames.push_back({ActionType::SEG_PUSHUP, node, lc, static_cast<int>(m_tr[lc]), static_cast<int>(m_tr[rc]), {lc, rc},
                        QString("node=%1 汇总两个孩子的区间和").arg(node),
                        QString("tr[%1] = (tr[%2] + tr[%3]) %% mod = %4").arg(node).arg(lc).arg(rc).arg(m_tr[node])});
}

void SegmentTreeTagModule::pushDown(int node, int l, int r) {
    int lc = SegmentTreeCommon::ls(node);
    int rc = SegmentTreeCommon::rs(node);
    if (lc > m_slotCount || rc > m_slotCount) return;
    int mid = (l + r) >> 1;
    bool hasTag = (m_mul[node] != 1 || m_add[node] != 0);
    if (!hasTag) return;
    m_tr[lc] = (m_tr[lc] * m_mul[node] + m_add[node] * (mid - l + 1)) % m_mod;
    m_tr[rc] = (m_tr[rc] * m_mul[node] + m_add[node] * (r - mid)) % m_mod;
    m_add[lc] = (m_add[lc] * m_mul[node] + m_add[node]) % m_mod;
    m_add[rc] = (m_add[rc] * m_mul[node] + m_add[node]) % m_mod;
    m_mul[lc] = (m_mul[lc] * m_mul[node]) % m_mod;
    m_mul[rc] = (m_mul[rc] * m_mul[node]) % m_mod;
    m_frames.push_back({ActionType::SEG_PUSH_DOWN, node, lc, static_cast<int>(m_mul[node]), static_cast<int>(m_add[node]), {},
                        QString("node=%1 的懒标记下发给左右孩子").arg(node),
                        QString("tr[ls] = tr[ls]*mul[%1] + add[%1]*(mid-l+1)　add[ls] = add[ls]*mul[%1] + add[%1]　mul[ls] = mul[ls]*mul[%1]")
                            .arg(node)});
    m_mul[node] = 1;
    m_add[node] = 0;
    m_frames.push_back({ActionType::SEG_TAG_CLEAR, node, 0, 0, 0, {},
                        QString("node=%1 的标记已下发，本节点恢复干净").arg(node),
                        QString("mul[%1] = 1　add[%1] = 0").arg(node)});
}

void SegmentTreeTagModule::changeMul(int node, int l, int r, int lt, int rt, long long k) {
    if (l >= lt && r <= rt) {
        m_tr[node] = (m_tr[node] * k) % m_mod;
        m_mul[node] = (m_mul[node] * k) % m_mod;
        m_add[node] = (m_add[node] * k) % m_mod;
        m_frames.push_back({ActionType::SEG_COVER_MUL, node, l, static_cast<int>(m_tr[node]), static_cast<int>(m_mul[node]), {},
                            QString("node=%1 区间 [%2,%3] 被完全覆盖，直接打标记").arg(node).arg(l).arg(r),
                            QString("tr[%1] = tr[%1]*%2 %% mod　mul[%1] = mul[%1]*%2 %% mod　add[%1] = add[%1]*%2 %% mod")
                                .arg(node).arg(k)});
        return;
    }
    pushDown(node, l, r);
    int mid = (l + r) >> 1;
    if (lt <= mid) changeMul(SegmentTreeCommon::ls(node), l, mid, lt, rt, k);
    if (rt > mid) changeMul(SegmentTreeCommon::rs(node), mid + 1, r, lt, rt, k);
    m_frames.push_back({ActionType::SEG_PARTIAL, node, mid, (lt <= mid) ? 1 : 0, (rt > mid) ? 1 : 0, {},
                        QString("node=%1 区间 [%2,%3] 只被部分覆盖，递归处理").arg(node).arg(l).arg(r),
                        QString("if (lt <= mid) → ls　if (rt > mid) → rs")});
    pushUp(node);
}

void SegmentTreeTagModule::changeAdd(int node, int l, int r, int lt, int rt, long long k) {
    if (l >= lt && r <= rt) {
        m_tr[node] = (m_tr[node] + k * (r - l + 1)) % m_mod;
        m_add[node] = (m_add[node] + k) % m_mod;
        m_frames.push_back({ActionType::SEG_COVER_ADD, node, l, static_cast<int>(m_tr[node]), static_cast<int>(m_add[node]), {},
                            QString("node=%1 区间 [%2,%3] 被完全覆盖，直接打标记").arg(node).arg(l).arg(r),
                            QString("tr[%1] = (tr[%1] + %2*(r-l+1)) %% mod　add[%1] = (add[%1] + %2) %% mod").arg(node).arg(k)});
        return;
    }
    pushDown(node, l, r);
    int mid = (l + r) >> 1;
    if (lt <= mid) changeAdd(SegmentTreeCommon::ls(node), l, mid, lt, rt, k);
    if (rt > mid) changeAdd(SegmentTreeCommon::rs(node), mid + 1, r, lt, rt, k);
    m_frames.push_back({ActionType::SEG_PARTIAL, node, mid, (lt <= mid) ? 1 : 0, (rt > mid) ? 1 : 0, {},
                        QString("node=%1 区间 [%2,%3] 只被部分覆盖，递归处理").arg(node).arg(l).arg(r),
                        QString("if (lt <= mid) → ls　if (rt > mid) → rs")});
    pushUp(node);
}

void SegmentTreeTagModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    if (frame.type == ActionType::SEG_RESET) {
        resetTreeScene();
    } else if (frame.type == ActionType::SEG_INIT_MUL) {
        setArrayValue("mul", frame.u, 1, kTagBlue);
        highlightNode(frame.u, kTagRed, QColor(255, 240, 238));
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_DESCEND) {
        highlightNode(frame.u, kTagOrange, QColor(255, 248, 232));
        highlightNode(frame.val1, kTagOrange, QColor(255, 248, 232));
        GraphNode* child = m_treeNodes.count(frame.val1) ? m_treeNodes[frame.val1] : nullptr;
        if (child) playNodeRipple(child, kTagOrange, 34.0, 480);
        auto edgeIt = m_treeEdges.find({frame.u, frame.val1});
        if (edgeIt != m_treeEdges.end()) playEdgeTravel(edgeIt->second, kTagOrange, 420);
        appendDescent(frame.u, kTagOrange);
        appendDescent(frame.val1, kTagOrange);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_LEAF) {
        setArrayValue("tr", frame.u, frame.val1, kTagGreen);
        setNodeValue(frame.u, QString("tr=%1").arg(frame.val1));
        highlightNode(frame.u, kTagGreen, QColor(240, 252, 242));
        GraphNode* leaf = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (leaf) playNodeRipple(leaf, kTagGreen, 42.0, 560);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PUSHUP) {
        ArrayBlock* blk = arrayBlock("tr", frame.u);
        if (blk) {
            int rc = frame.group.size() > 1 ? frame.group[1] : 0;
            ArrayBlock* lb = arrayBlock("tr", frame.v);
            ArrayBlock* rb = arrayBlock("tr", rc);
            QPointF sa = lb ? lb->pos() + QPointF(30, 0) : blk->pos();
            QPointF sb = rb ? rb->pos() + QPointF(30, 0) : blk->pos();
            playDualValueFlight(sa, sb, blk, frame.val1, frame.val2, kTagGreen);
        }
        setNodeValue(frame.u, QString("tr=%1").arg((frame.val1 + frame.val2) % m_mod));
        highlightNode(frame.u, kTagGreen, QColor(240, 252, 242));
        GraphNode* parent = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (parent) playNodeRipple(parent, kTagGreen, 46.0, 600);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_COVER_MUL) {
        setArrayValue("tr", frame.u, frame.val1, kTagYellow);
        setArrayValue("mul", frame.u, frame.val2, kTagPurple);
        setNodeValue(frame.u, QString("tr=%1").arg(frame.val1));
        setNodeTag(frame.u, QString("×%1").arg(frame.val2), kTagPurple);
        highlightNode(frame.u, kTagPurple, QColor(246, 240, 252));
        GraphNode* cover = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (cover) playNodeRipple(cover, kTagPurple, 44.0, 560);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_COVER_ADD) {
        setArrayValue("tr", frame.u, frame.val1, kTagYellow);
        setArrayValue("add", frame.u, frame.val2, kTagBlue);
        setNodeValue(frame.u, QString("tr=%1").arg(frame.val1));
        setNodeTag(frame.u, QString("+%1").arg(frame.val2), kTagBlue);
        highlightNode(frame.u, kTagBlue, QColor(236, 244, 252));
        GraphNode* coverAdd = m_treeNodes.count(frame.u) ? m_treeNodes[frame.u] : nullptr;
        if (coverAdd) playNodeRipple(coverAdd, kTagBlue, 44.0, 560);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PARTIAL) {
        highlightNode(frame.u, kTagOrange, QColor(255, 248, 232));
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_PUSH_DOWN) {
        int lc = frame.v;
        int rc = SegmentTreeCommon::rs(frame.u);
        setArrayValue("mul", lc, frame.val1, kTagPurple);
        setArrayValue("add", lc, frame.val2, kTagBlue);
        setArrayValue("mul", rc, frame.val1, kTagPurple);
        setArrayValue("add", rc, frame.val2, kTagBlue);
        setNodeTag(lc, QString("×%1").arg(frame.val1), kTagPurple);
        setNodeTag(rc, QString("×%1").arg(frame.val1), kTagPurple);
        if (frame.val2 != 0) {
            setNodeTag(lc, QString("+"), kTagBlue);
            setNodeTag(rc, QString("+"), kTagBlue);
        }
        highlightNode(frame.u, kTagOrange, QColor(255, 248, 232));
        QPointF from = nodePos(frame.u) + QPointF(16, -22);
        QPointF toL = nodePos(lc) + QPointF(16, -22);
        QPointF toR = nodePos(rc) + QPointF(16, -22);
        playTagDrop(from, toL, QString("×%1").arg(frame.val1), kTagPurple);
        playTagDrop(from, toR, QString("×%1").arg(frame.val1), kTagPurple);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_TAG_CLEAR) {
        setArrayValue("mul", frame.u, 1, kTagBlue);
        setArrayValue("add", frame.u, 0, kTagBlue);
        clearNodeTag(frame.u);
        movePointerTo(frame.u);
    } else if (frame.type == ActionType::SEG_DONE) {
        clearHighlights();
        if (m_ptrNode) m_ptrNode->setOpacity(0);
    }
}
