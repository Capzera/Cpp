#include "modules/HuffmanTreeModule.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <cstdio>
#include <map>

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    const char* inputs[] = {
        "7\n45 13 20 16 9 12 5",
        "8\n40 8 15 22 3 12 7 18",
        "5\n10 20 30 40 50",
        "6\n1 2 3 4 5 6",
        "4\n5 5 5 5",
        "2\n7 9"
    };

    bool allOk = true;
    for (const char* input : inputs) {
        QGraphicsScene scene;
        HuffmanTreeModule m;
        m.m_animated = false;
        m.buildLayout(&scene, input);
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);

        std::map<GraphNode*, QPointF> tree;
        std::map<GraphNode*, GraphNode*> parentOf;
        for (QGraphicsItem* item : scene.items()) {
            GraphNode* n = dynamic_cast<GraphNode*>(item);
            if (!n || n->id >= 5000) continue;
            if (n->opacity() < 0.5) continue;
            if (n->pos().y() > 600.0) continue;
            tree[n] = n->pos();
        }
        for (QGraphicsItem* item : scene.items()) {
            GraphEdge* e = dynamic_cast<GraphEdge*>(item);
            if (!e || !e->source || !e->dest) continue;
            if (!tree.count(e->source) || !tree.count(e->dest)) continue;
            parentOf[e->source] = e->dest;
        }

        printf("=== %s : frames=%d treeNodes=%d ===\n", input, (int)frames.size(), (int)tree.size());
        for (const auto& kv : tree) {
            printf("  node %d at (%.1f, %.1f)\n", kv.first->id, kv.second.x(), kv.second.y());
        }

        int midBad = 0;
        for (const auto& kv : tree) {
            GraphNode* child = kv.first;
            GraphNode* parent = nullptr;
            auto pit = parentOf.find(child);
            if (pit != parentOf.end()) parent = pit->second;
            if (!parent) continue;
            GraphNode* sibling = nullptr;
            for (const auto& kv2 : parentOf) {
                if (kv2.second == parent && kv2.first != child) sibling = kv2.first;
            }
            if (!sibling) continue;
            const double mid = (tree[child].x() + tree[sibling].x()) / 2.0;
            const double err = qAbs(mid - tree[parent].x());
            if (err > 0.6) {
                printf("  !! MIDPOINT VIOLATION parent %d x=%.1f mid=%.1f err=%.2f\n",
                       parent->id, tree[parent].x(), mid, err);
                midBad++;
            }
        }

        double minDist = 1e18;
        QList<QPointF> pts;
        for (const auto& kv : tree) pts.append(kv.second);
        for (int i = 0; i < pts.size(); ++i) {
            for (int j = i + 1; j < pts.size(); ++j) {
                const double dx = pts[i].x() - pts[j].x();
                const double dy = pts[i].y() - pts[j].y();
                const double d = dx * dx + dy * dy;
                if (d < minDist) minDist = d;
            }
        }
        const double minSep = minDist > 1e17 ? -1.0 : qSqrt(minDist);
        printf("  midpoint violations=%d  min node separation=%.2f\n", midBad, minSep);
        if (midBad > 0) allOk = false;
        if (minSep >= 0.0 && minSep < 44.0) { printf("  !! NODES TOO CLOSE\n"); allOk = false; }
    }
    printf("%s\n", allOk ? "ALL PASS" : "FAIL");
    return allOk ? 0 : 1;
}
