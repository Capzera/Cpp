#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/BFSModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/DijkstraNaiveModule.h"

static void renderScene(QGraphicsScene& scene, const QString& path) {
    QRectF bounds = scene.itemsBoundingRect().adjusted(-20, -20, 20, 20);
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(QColor(251, 252, 254));
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter, QRectF(QPointF(0, 0), bounds.size()), bounds);
    img.save(path);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    BFSModule bfs;
    QGraphicsScene s1;
    bfs.m_animated = false;
    bfs.buildLayout(&s1, "7 8 1\n1 2\n1 3\n2 4\n2 5\n3 5\n3 6\n5 7\n6 7");
    auto f1 = bfs.runAlgorithm();
    for (const auto& f : f1) bfs.renderFrame(f);
    QApplication::processEvents();
    renderScene(s1, "final_bfs.png");

    TopologicalSortModule topo;
    QGraphicsScene s2;
    topo.m_animated = false;
    topo.buildLayout(&s2, "8 9\n1 3\n1 4\n2 4\n2 5\n3 6\n4 6\n4 7\n5 7\n6 8");
    auto f2 = topo.runAlgorithm();
    for (const auto& f : f2) topo.renderFrame(f);
    QApplication::processEvents();
    renderScene(s2, "final_topo.png");

    DijkstraNaiveModule dijk;
    QGraphicsScene s3;
    dijk.m_animated = false;
    dijk.buildLayout(&s3, "6 9 1\n1 2 7\n1 3 9\n1 6 14\n2 3 10\n2 4 15\n3 4 11\n3 6 2\n4 5 6\n5 6 9");
    auto f3 = dijk.runAlgorithm();
    for (const auto& f : f3) dijk.renderFrame(f);
    QApplication::processEvents();
    for (QGraphicsItem* item : s3.items()) {
        GraphNode* n = dynamic_cast<GraphNode*>(item);
        if (n && n->id == 5) n->setPos(n->pos() + QPointF(60, -80));
    }
    QApplication::processEvents();
    renderScene(s3, "final_dijkstra.png");

    printf("saved\n");
    return 0;
}
