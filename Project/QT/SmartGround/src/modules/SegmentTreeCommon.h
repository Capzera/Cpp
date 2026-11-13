#ifndef SEGMENTTREECOMMON_H
#define SEGMENTTREECOMMON_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <map>
#include <vector>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <algorithm>
#include "IModule.h"
#include "render/GraphElements.h"

class SegmentTreeCommon {
public:
    struct NodeInfo {
        int node = 0;
        int l = 0;
        int r = 0;
        QPointF pos;
        GraphNode* item = nullptr;
    };

    static int ls(int node) { return node << 1; }
    static int rs(int node) { return node << 1 | 1; }

    static bool parseInput(const QString& inputData, std::vector<int>& a, int& mod, QString& error) {
        QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
        if (lines.isEmpty()) { error = "输入为空"; return false; }
        QStringList head = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (head.size() < 2) { error = "第一行需要 n 和 p"; return false; }
        bool ok = false;
        int n = head[0].toInt(&ok);
        if (!ok || n < 1 || n > 8) { error = "n 需为 1–8 的整数"; return false; }
        int p = head[1].toInt(&ok);
        if (!ok || p < 1 || p > 1000000000) { error = "p 需为正整数"; return false; }
        mod = p;
        a.assign(n + 1, 0);
        QStringList rest;
        for (int i = 1; i < lines.size(); ++i) rest << lines[i];
        QStringList all = rest.join(' ').split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (all.size() < n) { error = "数组元素不足 n 个"; return false; }
        for (int i = 1; i <= n; ++i) {
            a[i] = all[i - 1].toInt(&ok);
            if (!ok) { error = "数组元素必须为整数"; return false; }
        }
        return true;
    }

    static std::vector<NodeInfo> buildTopology(int node, int l, int r, const QPointF& center, double dx, double dy) {
        std::vector<NodeInfo> result;
        NodeInfo info;
        info.node = node;
        info.l = l;
        info.r = r;
        info.pos = center;
        result.push_back(info);
        if (l == r) return result;
        int mid = (l + r) >> 1;
        std::vector<NodeInfo> left = buildTopology(ls(node), l, mid, center + QPointF(-dx, dy), dx / 2.0, dy);
        std::vector<NodeInfo> right = buildTopology(rs(node), mid + 1, r, center + QPointF(dx, dy), dx / 2.0, dy);
        result.insert(result.end(), left.begin(), left.end());
        result.insert(result.end(), right.begin(), right.end());
        return result;
    }

    static int treeDepth(int n) {
        if (n <= 0) return 1;
        int depth = 1;
        int span = n;
        while (span > 1) {
            span = (span + 1) / 2;
            ++depth;
        }
        return depth;
    }

    static std::vector<int> collectNodeIds(int node, int l, int r) {
        std::vector<int> ids;
        ids.push_back(node);
        if (l == r) return ids;
        int mid = (l + r) >> 1;
        std::vector<int> left = collectNodeIds(ls(node), l, mid);
        std::vector<int> right = collectNodeIds(rs(node), mid + 1, r);
        ids.insert(ids.end(), left.begin(), left.end());
        ids.insert(ids.end(), right.begin(), right.end());
        std::sort(ids.begin(), ids.end());
        return ids;
    }

    struct Operation {
        int op = 0;
        int l = 0;
        int r = 0;
        long long k = 0;
    };

    struct Layout {
        int depth = 1;
        double nodeY = 230.0;
        double treeBottom = 230.0;
        double rowGap = 92.0;
        double slotW = 54.0;
        double slotStartX = 110.0;
        double arrY = 120.0;
        double firstRowY = 560.0;
        double rowStep = 78.0;
        std::vector<int> slotIds;
    };

    static Layout computeLayout(int n, int rowCount) {
        Layout layout;
        if (n <= 0) return layout;
        layout.depth = treeDepth(n);
        layout.nodeY = 230.0;
        layout.treeBottom = layout.nodeY + (layout.depth - 1) * layout.rowGap;
        layout.firstRowY = layout.treeBottom + 130.0;
        layout.slotIds = collectNodeIds(1, 1, n);
        return layout;
    }

    static bool parseOps(const QString& inputData, std::vector<int>& a, int& mod,
                         std::vector<Operation>& ops, QString& error) {
        QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
        if (lines.isEmpty()) { error = "输入为空"; return false; }
        QStringList head = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (head.size() < 3) { error = "第一行需要 n、m、p 三个数"; return false; }
        bool ok = false;
        int n = head[0].toInt(&ok);
        if (!ok || n < 1 || n > 8) { error = "n 需为 1–8 的整数"; return false; }
        int m = head[1].toInt(&ok);
        if (!ok || m < 1 || m > 6) { error = "m 需为 1–6 的整数"; return false; }
        int p = head[2].toInt(&ok);
        if (!ok || p < 1 || p > 1000000000) { error = "p 需为正整数"; return false; }
        mod = p;
        a.assign(n + 1, 0);
        QStringList all;
        for (int i = 1; i < lines.size(); ++i) all << lines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (all.size() < n) { error = "数组元素不足 n 个"; return false; }
        for (int i = 1; i <= n; ++i) {
            a[i] = all[i - 1].toInt(&ok);
            if (!ok) { error = "数组元素必须为整数"; return false; }
        }
        ops.clear();
        for (int i = 0; i < m; ++i) {
            int base = n + i * 4;
            if (base + 2 >= all.size() + 1) { error = "操作行不足"; return false; }
            Operation op;
            op.op = all[base].toInt(&ok);
            if (!ok || (op.op != 1 && op.op != 2 && op.op != 3)) { error = "op 只能是 1 / 2 / 3"; return false; }
            op.l = all[base + 1].toInt(&ok);
            op.r = all[base + 2].toInt(&ok);
            if (!ok || op.l < 1 || op.r < 1 || op.l > n || op.r > n) { error = "操作区间越界"; return false; }
            if (op.l > op.r) std::swap(op.l, op.r);
            if (op.op == 1 || op.op == 2) {
                if (base + 3 >= all.size() + 1) { error = "缺少操作数 k"; return false; }
                op.k = all[base + 3].toLongLong(&ok);
                if (!ok) { error = "k 必须为整数"; return false; }
            }
            ops.push_back(op);
        }
        return true;
    }
};

#endif
