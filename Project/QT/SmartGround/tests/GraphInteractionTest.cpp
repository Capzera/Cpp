#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QEventLoop>
#include <QTimer>
#include <QMouseEvent>
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

static void sendMouse(QWidget* target, QEvent::Type type, const QPointF& global,
                      Qt::MouseButton button, Qt::MouseButtons buttons) {
    QMouseEvent ev(type, global, global, button, buttons, Qt::NoModifier);
    QApplication::sendEvent(target, &ev);
}

static void doubleClick(QWidget* target, const QPointF& global) {
    sendMouse(target, QEvent::MouseButtonPress, global, Qt::LeftButton, Qt::LeftButton);
    sendMouse(target, QEvent::MouseButtonRelease, global, Qt::LeftButton, Qt::NoButton);
    pump(30);
    sendMouse(target, QEvent::MouseButtonDblClick, global, Qt::LeftButton, Qt::LeftButton);
    sendMouse(target, QEvent::MouseButtonRelease, global, Qt::LeftButton, Qt::NoButton);
    pump(30);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.resize(1200, 900);
    view.show();
    pump(60);

    SandboxGraphModule mod;
    mod.buildLayout(&scene, "3\n1 2 5\n2 3 8");
    mod.enableLiveEditing();
    pump(50);

    GraphNode* node = nullptr;
    GraphEdge* edge = nullptr;
    for (QGraphicsItem* item : scene.items()) {
        GraphNode* n = qgraphicsitem_cast<GraphNode*>(item);
        if (n && n->id == 1) node = n;
        GraphEdge* e = qgraphicsitem_cast<GraphEdge*>(item);
        if (e && e->weight == 5) edge = e;
    }
    if (!node || !edge) { printf("FAIL: setup\n"); return 1; }

    view.centerOn(scene.itemsBoundingRect().center());
    pump(30);

    const QPointF nodeGlobal = view.mapToGlobal(view.mapFromScene(node->pos()));
    printf("node pos=%f,%f global=%f,%f\n", node->pos().x(), node->pos().y(), nodeGlobal.x(), nodeGlobal.y());
    fflush(stdout);
    doubleClick(view.viewport(), nodeGlobal);
    printf("after dbl click node editing=%d\n", (int)node->isEditing());
    fflush(stdout);
    check(node->isEditing(), "node opens editor on real double click");
    if (node->isEditing()) node->cancelEdit();
    pump(20);
    printf("after cancel\n");
    fflush(stdout);

    const QPointF weightGlobal = view.mapToGlobal(view.mapFromScene(edge->weightAnchor()));
    printf("weight global=%f,%f\n", weightGlobal.x(), weightGlobal.y());
    fflush(stdout);
    doubleClick(view.viewport(), weightGlobal);
    printf("after dbl click edge editing=%d\n", (int)edge->isEditing());
    fflush(stdout);
    check(edge->isEditing(), "edge opens weight editor on real double click");
    if (edge->isEditing()) edge->cancelEdit();
    pump(20);

    const QPointF midGlobal = view.mapToGlobal(view.mapFromScene(edge->pos() + edge->weightAnchor()));
    doubleClick(view.viewport(), midGlobal);
    check(edge->isEditing(), "edge opens weight editor via local+scene sum");

    printf("\n%d failures\n", failures);
    return failures ? 1 : 0;
}
