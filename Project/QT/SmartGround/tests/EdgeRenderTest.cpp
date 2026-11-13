#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <cstdio>
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/GraphRenderHelper.h"
#include "modules/DSUBasicModule.h"

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
        DSUBasicModule m;
        m.m_animated = false;
        m.buildLayout(&scene, "6\n0 1 1 2 3 4 4 5 6 7");
        std::map<int, QPointF> before = m.captureNodePositions();
        check(before.size() == 6, "DSU built 6 nodes");
        std::map<int, QPointF> moved = before;
        moved[0] = before[0] + QPointF(120, 90);
        m.restoreNodePositions(moved);
        std::map<int, QPointF> after = m.captureNodePositions();
        check(std::abs(after[0].x() - moved[0].x()) < 0.01, "DSU node 0 moved to target");
        check(std::abs(after[1].x() - before[1].x()) < 0.01, "DSU node 1 untouched");
    }
    {
        QGraphicsScene scene;
        DSUBasicModule m;
        m.m_animated = false;
        m.buildLayout(&scene, "6\n0 1 1 2 3 4 4 5 6 7");
        bool idsOk = true;
        for (auto& p : m.captureNodePositions()) {
            if (p.first < 0 || p.first > 5) idsOk = false;
        }
        check(idsOk, "DSU capture returns 6 keyed positions");
    }
    printf(failures == 0 ? "ALL EDGE TESTS PASSED\n" : "EDGE TESTS HAD FAILURES\n");
    return failures == 0 ? 0 : 1;
}
