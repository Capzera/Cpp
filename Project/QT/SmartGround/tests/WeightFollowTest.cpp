#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <cstdio>
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/GraphRenderHelper.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    std::map<int, GraphNode*> nodes;
    std::map<std::pair<int,int>, GraphEdge*> edges;
    GraphModuleUtils::GraphOptions opt;
    opt.directed = true;
    GraphModuleUtils::EdgeInput e1{1, 2, 7};
    GraphModuleUtils::EdgeInput e2{2, 3, 11};
    GraphRenderHelper::buildNodes(&scene, nodes, 4, QPointF(470, 170), 115);
    GraphRenderHelper::buildEdges(&scene, nodes, edges, {e1, e2}, opt, true);
    GraphEdge* e = edges.begin()->second;
    printf("weight=%d has_weight=%d\n", e->weight, e->has_weight);
    if (!e->has_weight || e->weight != 7) { printf("FAIL: weight not attached\n"); return 1; }

    QPointF before = e->weightAnchor();
    nodes[2]->setPos(nodes[2]->pos() + QPointF(0, 180));
    QPointF after = e->weightAnchor();
    printf("node move: before=(%.1f,%.1f) after=(%.1f,%.1f) delta=(%.1f,%.1f)\n",
           before.x(), before.y(), after.x(), after.y(), after.x()-before.x(), after.y()-before.y());
    if (qAbs(after.y() - before.y()) < 1.0) { printf("FAIL: weight did not follow node\n"); return 1; }

    QPointF b2 = e->weightAnchor();
    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    press.setScenePos(e->shape().boundingRect().center());
    press.setButton(Qt::LeftButton);
    press.setButtons(Qt::LeftButton);
    e->mousePressEvent(&press);
    QGraphicsSceneMouseEvent move(QEvent::GraphicsSceneMouseMove);
    move.setScenePos(press.scenePos() + QPointF(0, -70));
    move.setButtons(Qt::LeftButton);
    e->mouseMoveEvent(&move);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);
    release.setScenePos(move.scenePos());
    release.setButton(Qt::LeftButton);
    e->mouseReleaseEvent(&release);
    QPointF a2 = e->weightAnchor();
    printf("edge bend: before=(%.1f,%.1f) after=(%.1f,%.1f) delta=(%.1f,%.1f)\n",
           b2.x(), b2.y(), a2.x(), a2.y(), a2.x()-b2.x(), a2.y()-b2.y());
    if (qAbs(a2.y() - b2.y()) < 1.0) { printf("FAIL: weight did not follow edge bend\n"); return 1; }

    std::map<int, GraphNode*> nodes2;
    std::map<std::pair<int,int>, GraphEdge*> edges2;
    GraphModuleUtils::GraphOptions und;
    und.directed = false;
    und.bidirectional = true;
    GraphRenderHelper::buildNodes(&scene, nodes2, 3, QPointF(470, 170), 115);
    GraphRenderHelper::buildEdges(&scene, nodes2, edges2, {e1}, und, false);
    GraphEdge* u = edges2.begin()->second;
    printf("undirected edge: directed=%d bidirectional=%d\n", u->is_directed, u->is_bidirectional);
    if (u->is_directed || !u->is_bidirectional) { printf("FAIL: bidirectional flag\n"); return 1; }

    std::map<int, GraphNode*> nodes3;
    std::map<std::pair<int,int>, GraphEdge*> edges3;
    GraphRenderHelper::buildNodes(&scene, nodes3, 3, QPointF(470, 170), 115);
    GraphRenderHelper::buildEdges(&scene, nodes3, edges3, {e1, GraphModuleUtils::EdgeInput{2, 1, 9}}, opt, true);
    printf("directed reciprocal edges count=%d\n", (int)edges3.size());
    if (edges3.size() != 2) { printf("FAIL: reciprocal directed edges merged\n"); return 1; }

    printf("WEIGHT FOLLOW OK\n");
    return 0;
}
