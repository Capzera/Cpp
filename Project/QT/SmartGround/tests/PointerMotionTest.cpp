#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include "modules/HashTableModule.h"
#include "modules/TrieModule.h"
#include "modules/PrimModule.h"
#include "render/GraphElements.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
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

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        QGraphicsScene scene;
        HashTableModule mod;
        mod.buildLayout(&scene, "4\n5\n3\n7\n11");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        PointerBlock* k = findPointer(scene, "k");
        check("hash k pointer exists", k != nullptr);
        if (k) {
            check("hash k pointer is placed after first frame", k->isPlaced());
            check("hash k pointer not stuck at origin", !k->pos().isNull());
        }
        pump(1200);
        if (k) {
            printf("  hash k final pos=(%.1f,%.1f)\n", k->pos().x(), k->pos().y());
            check("hash k pointer settles just above the slot row", std::abs(k->pos().y() - 116.0) < 1.0);
        }
    }

    {
        QGraphicsScene scene;
        TrieModule mod;
        mod.buildLayout(&scene, "3\ncat\ncar\ndog");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        PointerBlock* i = findPointer(scene, "i");
        PointerBlock* p = findPointer(scene, "p");
        check("trie i pointer exists", i != nullptr);
        check("trie p pointer exists", p != nullptr);
        if (i) check("trie i pointer is placed", i->isPlaced());
        if (p) check("trie p pointer is placed", p->isPlaced());
    }

    {
        QGraphicsScene scene;
        PrimModule mod;
        mod.buildLayout(&scene, "6 9 1\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8");
        auto frames = mod.runAlgorithm();
        for (const auto& f : frames) mod.renderFrame(f);
        PointerBlock* u = findPointer(scene, "u");
        check("prim u pointer exists", u != nullptr);
        if (u) {
            check("prim u pointer is placed", u->isPlaced());
            check("prim u pointer not stuck at origin", !u->pos().isNull());
        }
        pump(1200);
        if (u) {
            GraphNode* node = dynamic_cast<GraphNode*>(u->anchorItem());
            check("prim u anchored to a node", node != nullptr);
            if (node) {
                double dy = u->pos().y() - node->pos().y();
                printf("  prim u dy=%.2f\n", dy);
                check("prim u tip lands on the vertex circle", dy > 18.0 && dy < 24.0);
            }
        }
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
