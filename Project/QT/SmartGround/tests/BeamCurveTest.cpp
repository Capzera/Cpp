#include "render/GraphElements.h"
#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include <cmath>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    GraphNode* a = new GraphNode(1);
    a->setPos(0, 0);
    scene.addItem(a);
    GraphNode* b = new GraphNode(2);
    b->setPos(200, 140);
    scene.addItem(b);
    GraphEdge* e = new GraphEdge(a, b, true);
    e->setBendOffset(QPointF(0, -60));
    scene.addItem(e);

    const qreal radius = 22.0;
    const QPointF p0 = e->trimmedStartPublic(radius);
    const QPointF p1 = e->trimmedEndPublic(radius);
    const QPointF c = e->controlPointPublic();

    double worst = 0.0;
    for (int step = 0; step <= 30; ++step) {
        const qreal from = (step / 30.0) * 0.66;
        const qreal to = from + 0.34;
        const QPointF got = GraphEdge::quadSubPathPointPublic(p0, c, p1, from, to);
        const QPointF expect = GraphEdge::quadPointPublic(p0, c, p1, to);
        const double err = std::hypot(got.x() - expect.x(), got.y() - expect.y());
        if (err > worst) worst = err;
    }

    printf("beam endpoint deviation from curve = %.8f px\n", worst);
    printf("PASS: %s\n", worst < 0.01 ? "yes" : "no");
    return worst < 0.01 ? 0 : 1;
}
