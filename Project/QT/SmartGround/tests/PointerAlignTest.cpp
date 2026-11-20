#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
#include "modules/PrimModule.h"
#include "render/GraphElements.h"

static int g_fail = 0;

static void check(bool ok, const char* name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static PointerBlock* findPointer(QGraphicsScene& scene, const QString& label) {
    for (QGraphicsItem* item : scene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (p && p->label == label) return p;
    }
    return nullptr;
}

static void checkAligned(QGraphicsScene& scene, const QString& label,
                         double offX, double offY, const char* name) {
    PointerBlock* p = findPointer(scene, label);
    check(p != nullptr, "pointer exists");
    if (!p) return;
    QGraphicsItem* anchor = p->anchorItem();
    check(anchor != nullptr, "pointer has an anchor");
    if (!anchor) return;
    double dx = std::abs(p->pos().x() - (anchor->pos().x() + offX));
    double dy = std::abs(p->pos().y() - (anchor->pos().y() + offY));
    printf("  %s pos=(%.1f,%.1f) anchor=(%.1f,%.1f) dx=%.2f dy=%.2f\n",
           label.toUtf8().constData(), p->pos().x(), p->pos().y(),
           anchor->pos().x(), anchor->pos().y(), dx, dy);
    check(dx < 1.0 && dy < 1.0, name);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        QGraphicsScene scene;
        HashTableModule mod;
        mod.buildLayout(&scene, "4\n5\n3\n7\n11");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        pump(900);
        checkAligned(scene, "k", 33.0, -2.0, "k arrow tip stays just above the slot row, centered");
        PointerBlock* k = findPointer(scene, "k");
        if (k) check(k->labelOffset().y() < 0, "k label sits above the shaft tail");
    }

    {
        QGraphicsScene scene;
        TrieModule mod;
        mod.buildLayout(&scene, "3\ncat\ncar\ndog");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        pump(900);
        checkAligned(scene, "i", 27.0, -2.0, "i arrow tip stays just above the cell row, centered");
        PointerBlock* i = findPointer(scene, "i");
        if (i) check(i->labelOffset().y() < 0, "i label sits at the arrow tail");
    }

    {
        QGraphicsScene scene;
        PrimModule mod;
        mod.buildLayout(&scene, "6 9 1\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        pump(900);
        checkAligned(scene, "u", 0.0, 20.0, "u points straight up at the vertex center line");
        PointerBlock* u = findPointer(scene, "u");
        if (u) {
            GraphNode* node = dynamic_cast<GraphNode*>(u->anchorItem());
            check(node != nullptr, "u anchored to a graph node");
            if (node) {
                double dy = u->pos().y() - node->pos().y();
                printf("  u dy from node center=%.1f (circle radius 20)\n", dy);
                check(dy > 18.0 && dy < 24.0, "u arrow tip lands on the vertex circle edge");
            }
        }
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
