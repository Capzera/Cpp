#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <cstdio>
#include "modules/DijkstraNaiveModule.h"
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

static int countDarkPixelsUnder(const QImage& img, QRect region) {
    int n = 0;
    for (int y = region.top(); y <= region.bottom(); ++y) {
        for (int x = region.left(); x <= region.right(); ++x) {
            if (x < 0 || y < 0 || x >= img.width() || y >= img.height()) continue;
            QColor c = img.pixelColor(x, y);
            if (c.red() < 120 && c.green() < 120 && c.blue() < 120) ++n;
        }
    }
    return n;
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

    DijkstraNaiveModule mod;
    mod.buildLayout(&scene, "4 3 1\n1 2 7\n2 3 5\n3 4 2");
    mod.enableLiveEditing();
    pump(50);

    GraphNode* node = findNode(scene, 4);
    check(node != nullptr, "dijkstra node 4 exists");
    if (!node) { printf("\n%d failures\n", failures); return 1; }

    check(node->isEditable(), "node 4 is editable");
    check(node->valueText.isEmpty(), "node 4 starts with no custom value text");

    node->beginEdit();
    check(node->isEditing(), "inline editor opens on node 4");
    node->setValueText(QString("3"));
    node->value = 3;
    node->update();
    pump(50);

    check(node->value == 3, "node value updated to 3");
    check(node->valueText == QString("3"), "node value text is exactly '3'");

    QImage before = QImage(120, 110, QImage::Format_ARGB32);
    before.fill(Qt::white);
    {
        QPainter p(&before);
        p.translate(60, 45);
        node->paint(&p, nullptr, nullptr);
    }
    QRect below(10, 70, 100, 32);
    int rows = countTextRows(before, below);
    check(rows <= 1, "only one text row rendered below the node (no stacked duplicates)");

    QImage after = QImage(120, 110, QImage::Format_ARGB32);
    after.fill(Qt::white);
    {
        QPainter p(&after);
        p.translate(60, 45);
        node->paint(&p, nullptr, nullptr);
    }
    check(countDarkPixelsUnder(before, below) > 0, "edited value is actually painted below node");

    node->setValueText(QString());
    node->value = 0;
    node->update();
    pump(30);

    printf("\n%d failures\n", failures);
    return failures ? 1 : 0;
}
