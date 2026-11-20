#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <cstdio>
#include "modules/SandboxGraphModule.h"
#include "render/GraphElements.h"

static int failures = 0;

static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static GraphNode* findNode(QGraphicsScene& scene, int id) {
    for (QGraphicsItem* item : scene.items()) {
        GraphNode* node = qgraphicsitem_cast<GraphNode*>(item);
        if (node && node->id == id) return node;
    }
    return nullptr;
}

static GraphEdge* findEdge(QGraphicsScene& scene, int weight) {
    for (QGraphicsItem* item : scene.items()) {
        GraphEdge* edge = qgraphicsitem_cast<GraphEdge*>(item);
        if (edge && edge->weight == weight) return edge;
    }
    return nullptr;
}

static int countTextRows(const QImage& img, QRect region) {
    int rows = 0;
    bool inRow = false;
    for (int y = region.top(); y <= region.bottom(); ++y) {
        bool has = false;
        for (int x = region.left(); x <= region.right(); ++x) {
            if (x < 0 || y < 0 || x >= img.width() || y >= img.height()) continue;
            QColor c = img.pixelColor(x, y);
            if (c.red() < 120 && c.green() < 120 && c.blue() < 120) { has = true; break; }
        }
        if (has && !inRow) { rows++; inRow = true; }
        else if (!has) inRow = false;
    }
    return rows;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;

    SandboxGraphModule mod;
    mod.buildLayout(&scene, "3\n1 2 5\n2 3 8");
    mod.enableLiveEditing();
    pump(50);

    GraphNode* node = findNode(scene, 1);
    check(node != nullptr, "graph node 1 exists");
    GraphEdge* edge = findEdge(scene, 5);
    check(edge != nullptr, "graph edge exists");
    if (!node || !edge) { printf("\n%d failures\n", failures); return 1; }

    check(node->isEditable(), "node is editable");
    check(edge->isEditable(), "edge is editable");

    node->beginEdit();
    check(node->isEditing(), "node editor opens");
    node->commitEdit();

    node->setValueText(QString());
    node->setCenterText(QString("7"));
    node->update();
    pump(50);    QImage img(120, 110, QImage::Format_ARGB32);
    img.fill(Qt::white);
    {
        QPainter p(&img);
        p.translate(60, 45);
        node->paint(&p, nullptr, nullptr);
    }
    QRect center(20, 32, 80, 26);
    QRect below(20, 70, 80, 25);
    check(countTextRows(img, center) >= 1, "edited value painted at node center");
    check(countTextRows(img, below) == 0, "no text painted below the node");

    node->setCenterText(QString());
    node->setCustomValueText(QString());
    node->setValueText(QString());
    node->beginEdit();
    if (node->isEditing()) {
        QGraphicsTextItem* editor = node->editorItem();
        if (editor) {
            editor->setPlainText("9");
            node->commitEdit();
        }
    }
    pump(50);
    check(node->centerText() == "9", "committed edit fills the circle center text");
    check(node->belowText().isEmpty(), "committed edit leaves no text below the node");

    node->update();
    QImage img2(120, 110, QImage::Format_ARGB32);
    img2.fill(Qt::white);
    {
        QPainter p(&img2);
        p.translate(60, 45);
        node->paint(&p, nullptr, nullptr);
    }
    check(countTextRows(img2, QRect(20, 32, 80, 26)) >= 1, "committed value visible inside circle");
    check(countTextRows(img2, QRect(20, 70, 80, 25)) == 0, "committed value absent below circle");

    check(edge->has_weight, "edge carries a weight");
    check(edge->weight == 5, "edge weight is 5");
    edge->beginEdit();
    check(edge->isEditing(), "edge editor opens on double click");
    edge->commitEdit();

    edge->setWeight(42);
    edge->update();
    pump(30);
    check(edge->weight == 42, "edge weight updated to 42");

    const QPointF anchor = edge->weightAnchor() - edge->pos();
    QRectF badge(anchor.x() - 30, anchor.y() - 12, 60, 24);
    check(edge->shape().contains(badge.center()), "weight badge area is hit-testable for double click");

    printf("\n%d failures\n", failures);
    return failures ? 1 : 0;
}
