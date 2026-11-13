#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include <cmath>
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/GraphRenderHelper.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

struct EdgeShot {
    QImage img;
    QPointF pa, pb;
};

static EdgeShot renderCase(bool directed, bool bidirectional) {
    QGraphicsScene scene;
    std::map<int, GraphNode*> nodes;
    std::map<std::pair<int,int>, GraphEdge*> edges;
    GraphModuleUtils::GraphOptions opt;
    opt.directed = directed;
    opt.bidirectional = bidirectional;
    GraphRenderHelper::buildNodes(&scene, nodes, 3, QPointF(300, 300), 140);
    GraphRenderHelper::buildEdges(&scene, nodes, edges, {GraphModuleUtils::EdgeInput{1,2,0}}, opt, false);
    scene.setSceneRect(-200, -200, 1000, 1000);
    QImage img(1000, 1000, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, 1000, 1000), QRectF(-200, -200, 1000, 1000));
    p.end();
    EdgeShot shot;
    shot.img = img;
    shot.pa = nodes[1]->pos();
    shot.pb = nodes[2]->pos();
    return shot;
}

static int countDarkRing(const QImage& img, QPointF c, double r0, double r1) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            double dx = x - 200 - c.x();
            double dy = y - 200 - c.y();
            double r = std::hypot(dx, dy);
            if (r < r0 || r > r1) continue;
            QColor col = img.pixelColor(x, y);
            if (col.red() < 150 && col.green() < 150 && col.blue() < 150) n++;
        }
    }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    EdgeShot plain  = renderCase(false, false);
    EdgeShot direct = renderCase(true,  false);
    EdgeShot bidir  = renderCase(false, true);

    double r0 = 23.0, r1 = 48.0;
    int plainA = countDarkRing(plain.img,  plain.pa, r0, r1);
    int plainB = countDarkRing(plain.img,  plain.pb, r0, r1);
    int directA = countDarkRing(direct.img, direct.pa, r0, r1);
    int directB = countDarkRing(direct.img, direct.pb, r0, r1);
    int bidirA = countDarkRing(bidir.img,  bidir.pa, r0, r1);
    int bidirB = countDarkRing(bidir.img,  bidir.pb, r0, r1);

    printf("plain  A=%d B=%d\ndirect A=%d B=%d\nbidir  A=%d B=%d\n", plainA, plainB, directA, directB, bidirA, bidirB);

    check(directB - plainB > 8, "directed: arrowhead added at TARGET end");
    check(directA - plainA <= 1, "directed: no arrowhead at SOURCE end");
    check(bidirA - plainA > 8, "bidirectional: arrowhead added at SOURCE end");
    check(bidirB - plainB > 8, "bidirectional: arrowhead added at TARGET end");

    printf(failures == 0 ? "ALL ARROW TESTS PASSED\n" : "ARROW TESTS HAD FAILURES\n");
    return failures == 0 ? 0 : 1;
}
