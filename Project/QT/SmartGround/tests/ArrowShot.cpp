#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include "render/GraphElements.h"
#include "modules/GraphModuleUtils.h"
#include "modules/GraphRenderHelper.h"

static void shot(bool directed, bool bidirectional, const QString& path) {
    QGraphicsScene scene;
    std::map<int, GraphNode*> nodes;
    std::map<std::pair<int,int>, GraphEdge*> edges;
    GraphModuleUtils::GraphOptions opt;
    opt.directed = directed;
    opt.bidirectional = bidirectional;
    GraphRenderHelper::buildNodes(&scene, nodes, 3, QPointF(300, 300), 140);
    std::vector<GraphModuleUtils::EdgeInput> inputs = {{1,2,0}};
    GraphRenderHelper::buildEdges(&scene, nodes, edges, inputs, opt, false);
    QPointF target = nodes[2]->pos();
    QPointF source = nodes[1]->pos();
    QRectF crop(target.x()-70, target.y()-70, 140, 140);
    QImage img(560, 560, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    scene.render(&p, QRectF(0, 0, 560, 560), crop);
    p.end();
    img.save(path);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    shot(true, false, "crop_directed.png");
    shot(false, true, "crop_bidir.png");
    shot(false, false, "crop_plain.png");

    {
        QGraphicsScene scene;
        std::map<int, GraphNode*> nodes;
        std::map<std::pair<int,int>, GraphEdge*> edges;
        GraphModuleUtils::GraphOptions opt;
        opt.directed = true;
        GraphRenderHelper::buildNodes(&scene, nodes, 4, QPointF(300, 300), 130);
        std::vector<GraphModuleUtils::EdgeInput> inputs = {{1,2,7},{2,3,5},{3,4,11},{1,4,3}};
        GraphRenderHelper::buildEdges(&scene, nodes, edges, inputs, opt, true);
        scene.setSceneRect(100, 100, 400, 400);
        QImage img(800, 800, QImage::Format_ARGB32);
        img.fill(Qt::white);
        QPainter p(&img);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        scene.render(&p, QRectF(0, 0, 800, 800), QRectF(100, 100, 400, 400));
        p.end();
        img.save("crop_weight.png");

        QPointF n2 = nodes[2]->pos();
        QImage img2(400, 400, QImage::Format_ARGB32);
        img2.fill(Qt::white);
        QPainter p2(&img2);
        scene.render(&p2, QRectF(0, 0, 400, 400), QRectF(n2.x() - 55, n2.y() - 55, 110, 110));
        p2.end();
        img2.save("crop_weight_node.png");
    }
    return 0;
}
