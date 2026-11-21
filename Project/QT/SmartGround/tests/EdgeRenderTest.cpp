#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <cstdio>
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/GraphRenderHelper.h"
#include "modules/DSUPathCompressionModule.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        QGraphicsScene scene;
        std::map<int, GraphNode*> nodes;
        std::map<std::pair<int,int>, GraphEdge*> edges;
        GraphModuleUtils::GraphOptions dir;
        dir.directed = true;
        GraphRenderHelper::buildNodes(&scene, nodes, 4, QPointF(470, 170), 115);
        GraphRenderHelper::buildEdges(&scene, nodes, edges, {GraphModuleUtils::EdgeInput{1,2,7}}, dir, true);
        GraphEdge* e = edges.begin()->second;
        check(e->is_directed, "directed edge has is_directed");
        check(e->is_bidirectional == false, "directed edge not bidirectional");
    }
    {
        QGraphicsScene scene;
        std::map<int, GraphNode*> nodes;
        std::map<std::pair<int,int>, GraphEdge*> edges;
        GraphModuleUtils::GraphOptions und;
        und.directed = false;
        und.bidirectional = true;
        GraphRenderHelper::buildNodes(&scene, nodes, 4, QPointF(470, 170), 115);
        GraphRenderHelper::buildEdges(&scene, nodes, edges, {GraphModuleUtils::EdgeInput{1,2,0}}, und, false);
        GraphEdge* e = edges.begin()->second;
        check(!e->is_directed, "undirected edge not directed");
        check(e->is_bidirectional, "undirected+bidirectional flag set");
    }
    {
        QGraphicsScene scene;
        DSUPathCompressionModule m;
        m.m_animated = false;
        m.buildLayout(&scene, "6\n1 2\n2 3\n3 4\n5 6");
        std::map<int, QPointF> before = m.captureNodePositions();
        check(before.size() == 6, "DSU built 6 nodes");
        std::map<int, QPointF> moved = before;
        moved[1] = before[1] + QPointF(120, 90);
        m.restoreNodePositions(moved);
        std::map<int, QPointF> after = m.captureNodePositions();
        check(std::abs(after[1].x() - moved[1].x()) < 0.01, "DSU node 1 moved to target");
        check(std::abs(after[2].x() - before[2].x()) < 0.01, "DSU node 2 untouched");
    }
    {
        QGraphicsScene scene;
        DSUPathCompressionModule m;
        m.m_animated = false;
        m.buildLayout(&scene, "6\n1 2\n2 3\n3 4\n5 6");
        bool idsOk = true;
        for (auto& p : m.captureNodePositions()) {
            if (p.first < 1 || p.first > 6) idsOk = false;
        }
        check(idsOk, "DSU capture returns 6 keyed positions");
    }
    printf(failures == 0 ? "ALL EDGE TESTS PASSED\n" : "EDGE TESTS HAD FAILURES\n");
    return failures == 0 ? 0 : 1;
}
