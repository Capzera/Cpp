#ifndef GRAPHMODULEUTILS_H
#define GRAPHMODULEUTILS_H
#include <QRegularExpression>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <cmath>
#include <vector>

namespace GraphModuleUtils {
constexpr int MAX_NODES = 20;
constexpr int MAX_EDGES = 300;

struct GraphOptions {
    bool directed = false;
    bool bidirectional = false;
    bool hasSource = false;
    bool weighted = false;
};

struct EdgeInput {
    int u;
    int v;
    int w;
};

struct GraphInput {
    int n = 0;
    int m = 0;
    int src = 1;
    bool weighted = false;
    std::vector<EdgeInput> edges;
};

inline bool parseHeader(const QString& headerLine, bool hasSource, GraphInput& graph) {
    QStringList tokens = headerLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int headerSize = hasSource ? 3 : 2;
    if (tokens.size() < headerSize) return false;
    bool ok = false;
    graph.n = tokens[0].toInt(&ok);
    if (!ok || graph.n < 1 || graph.n > MAX_NODES) return false;
    graph.m = tokens[1].toInt(&ok);
    if (!ok || graph.m < 0 || graph.m > MAX_EDGES) return false;
    graph.src = 1;
    if (hasSource) {
        graph.src = tokens[2].toInt(&ok);
        if (!ok || graph.src < 1 || graph.src > graph.n) return false;
    }
    return true;
}

inline bool parse(const QString& data, bool hasSource, const GraphOptions& options, GraphInput& graph) {
    QStringList lines = data.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    if (lines.isEmpty()) return false;
    graph = GraphInput();
    if (!parseHeader(lines[0], hasSource, graph)) return false;
    QStringList edgeLines = lines.mid(1);
    while (edgeLines.size() > graph.m) edgeLines.removeLast();
    bool weighted = false;
    for (const QString& line : edgeLines) {
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.size() >= 3) { weighted = true; break; }
    }
    graph.weighted = weighted;
    graph.edges.clear();
    for (int i = 0; i < graph.m; ++i) {
        if (i >= edgeLines.size()) return false;
        QStringList tokens = edgeLines[i].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        bool ok = false;
        EdgeInput edge;
        edge.u = tokens.value(0).toInt(&ok);
        if (!ok || edge.u < 1 || edge.u > graph.n) return false;
        edge.v = tokens.value(1).toInt(&ok);
        if (!ok || edge.v < 1 || edge.v > graph.n) return false;
        edge.w = 1;
        if (weighted) {
            if (tokens.size() < 3) return false;
            edge.w = tokens[2].toInt(&ok);
            if (!ok || edge.w < -1000000 || edge.w > 1000000) return false;
        }
        graph.edges.push_back(edge);
    }
    return true;
}

inline QPointF circlePoint(int index, int count, QPointF center, qreal radius) {
    qreal angle = -1.5707963267948966 + 6.283185307179586 * index / count;
    return QPointF(center.x() + radius * std::cos(angle), center.y() + radius * std::sin(angle));
}
}
#endif
