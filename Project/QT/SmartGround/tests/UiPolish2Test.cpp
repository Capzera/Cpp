#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <cstdio>
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyTextEdit.h"
#include "modules/BFSModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/DijkstraNaiveModule.h"
#include "modules/BinarySearchClosedModule.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void renderScene(QGraphicsScene& scene, const QString& path) {
    QRectF bounds = scene.itemsBoundingRect();
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter painter(&img);
    scene.render(&painter, QRectF(QPointF(0, 0), bounds.size()), bounds);
    img.save(path);
}

static ArrayBlock* findBlock(QGraphicsScene& scene, int index) {
    int count = 0;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* block = dynamic_cast<ArrayBlock*>(item);
        if (!block) continue;
        if (count == index) return block;
        count++;
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MyToggleSwitch toggle;
    toggle.setOnText("无向图");
    toggle.setOffText("有向图");
    toggle.setChecked(false);
    QSize hint = toggle.sizeHint();
    printf("toggle sizeHint: %dx%d\n", hint.width(), hint.height());
    check(hint.width() < 190, "toggle switch is compact (not overly long)");
    check(hint.height() <= 30, "toggle switch is short in height");

    MyTextEdit edit;
    edit.setPlaceholderText("提示文本");
    edit.setText("");
    edit.show();
    QApplication::processEvents();
    check(edit.placeholderText() == "提示文本", "empty edit keeps placeholder");
    QImage shot(edit.size(), QImage::Format_ARGB32);
    shot.fill(Qt::white);
    edit.render(&shot);
    shot.save("input_border.png");
    bool hasBlueTint = false;
    for (int y = 0; y < shot.height(); ++y) {
        QColor c = shot.pixelColor(shot.width() / 2, y);
        printf("row %d: %d,%d,%d\n", y, c.red(), c.green(), c.blue());
        if (c.blue() > c.red() + 8 && c.blue() > 200) hasBlueTint = true;
    }
    check(hasBlueTint, "input box has a soft blue border");

    BFSModule bfs;
    QGraphicsScene scene;
    bfs.m_animated = false;
    bfs.buildLayout(&scene, "5 4 1\n1 2\n1 3\n2 4\n3 5");
    QApplication::processEvents();

    QVector<ArrayBlock*> allBlocks;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* b = dynamic_cast<ArrayBlock*>(item);
        if (b) allBlocks.push_back(b);
    }
    printf("BFS blocks: %d\n", allBlocks.size());
    int infinityCount = 0, dashCount = 0;
    for (ArrayBlock* b : allBlocks) {
        if (b->customValue == "∞") infinityCount++;
        if (b->customValue == "—") dashCount++;
    }
    printf("infinity=%d dash=%d\n", infinityCount, dashCount);
    check(infinityCount == 5, "BFS distance array shows a value for every node");
    check(dashCount == 5, "BFS parent array shows a value for every node");

    auto frames = bfs.runAlgorithm();
    for (const auto& f : frames) bfs.renderFrame(f);
    QApplication::processEvents();
    renderScene(scene, "bfs_after.png");

    int headCount = 0, tailCount = 0;
    for (QGraphicsItem* item : scene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (!p) continue;
        if (p->label == "H") headCount++;
        if (p->label == "T") tailCount++;
    }
    check(headCount == 1 && tailCount == 1, "BFS uses short H/T pointer labels");

    TopologicalSortModule topo;
    QGraphicsScene topoScene;
    topo.m_animated = false;
    topo.buildLayout(&topoScene, "5 5\n1 2\n1 3\n2 4\n3 4\n4 5");
    QApplication::processEvents();
    auto topoFrames = topo.runAlgorithm();
    for (const auto& f : topoFrames) topo.renderFrame(f);
    QApplication::processEvents();
    renderScene(topoScene, "topo_after.png");

    int topoH = 0, topoT = 0;
    QVector<PointerBlock*> topoPtrs;
    for (QGraphicsItem* item : topoScene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (!p) continue;
        topoPtrs.push_back(p);
        if (p->label == "H") topoH++;
        if (p->label == "T") topoT++;
    }
    check(topoH == 1 && topoT == 1, "topo uses short H/T pointer labels");

    int crowded = 0;
    for (int i = 0; i < topoPtrs.size(); ++i) {
        for (int j = i + 1; j < topoPtrs.size(); ++j) {
            QRectF ra = topoPtrs[i]->mapRectToScene(topoPtrs[i]->boundingRect());
            QRectF rb = topoPtrs[j]->mapRectToScene(topoPtrs[j]->boundingRect());
            if (ra.intersects(rb)) crowded++;
        }
    }
    check(crowded == 0, "topo pointers do not overlap each other");

    DijkstraNaiveModule dijk;
    QGraphicsScene dijkScene;
    dijk.m_animated = false;
    dijk.buildLayout(&dijkScene, "4 4 1\n1 2 5\n1 3 2\n3 2 1\n2 4 3");
    QApplication::processEvents();
    auto dijkFrames = dijk.runAlgorithm();
    for (const auto& f : dijkFrames) dijk.renderFrame(f);
    QApplication::processEvents();
    renderScene(dijkScene, "dijkstra_after.png");

    PointerBlock* anchored = nullptr;
    GraphNode* anchoredNode = nullptr;
    for (QGraphicsItem* item : dijkScene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (p && p->anchorNode()) { anchored = p; anchoredNode = p->anchorNode(); }
    }
    check(anchored != nullptr, "Dijkstra pointers are anchored to graph nodes");
    if (anchored && anchoredNode) {
        QPointF before = anchored->pos();
        anchoredNode->setPos(anchoredNode->pos() + QPointF(120, 90));
        QApplication::processEvents();
        QPointF after = anchored->pos();
        printf("pointer moved %f,%f after node drag\n", after.x() - before.x(), after.y() - before.y());
        check((after - before).x() > 100 && (after - before).y() > 70, "anchored pointer follows node drag");
    }

    BinarySearchClosedModule bs;
    QGraphicsScene bsScene;
    bs.m_animated = false;
    bs.buildLayout(&bsScene, "5 8\n1 3 5 7 9");
    QApplication::processEvents();
    auto bsFrames = bs.runAlgorithm();
    for (const auto& f : bsFrames) bs.renderFrame(f);
    QApplication::processEvents();
    renderScene(bsScene, "binarysearch_after.png");

    QVector<PointerBlock*> bsPtrs;
    for (QGraphicsItem* item : bsScene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (p) bsPtrs.push_back(p);
    }
    printf("binary search pointers: %d\n", bsPtrs.size());
    check(bsPtrs.size() == 3, "binary search has L/R/mid pointers");
    int bsCrowded = 0;
    for (int i = 0; i < bsPtrs.size(); ++i) {
        for (int j = i + 1; j < bsPtrs.size(); ++j) {
            QRectF ra = bsPtrs[i]->mapRectToScene(bsPtrs[i]->boundingRect());
            QRectF rb = bsPtrs[j]->mapRectToScene(bsPtrs[j]->boundingRect());
            if (ra.intersects(rb)) bsCrowded++;
        }
    }
    check(bsCrowded == 0, "binary search pointers crowd instead of overlapping");

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
