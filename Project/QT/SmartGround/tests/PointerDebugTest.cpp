#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include "modules/HashTableModule.h"
#include "render/GraphElements.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    HashTableModule mod;
    mod.buildLayout(&scene, "4\n5\n3\n7\n11");
    auto frames = mod.runAlgorithm();
    printf("frames=%d valid=%d\n", (int)frames.size(), (int)mod.m_animated);
    for (const auto& f : frames) {
        mod.renderFrame(f);
        printf("frame type=%d pos=(%.1f,%.1f)\n", (int)f.type, 0.0, 0.0);
    }
    for (QGraphicsItem* item : scene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (p) printf("pointer %s pos=(%.1f,%.1f) opacity=%.2f anchor=%p\n",
                      p->label.toUtf8().constData(), p->pos().x(), p->pos().y(),
                      p->opacity(), (void*)p->anchorItem());
    }
    pump(1200);
    for (QGraphicsItem* item : scene.items()) {
        PointerBlock* p = dynamic_cast<PointerBlock*>(item);
        if (p) printf("after pump: pointer %s pos=(%.1f,%.1f)\n",
                      p->label.toUtf8().constData(), p->pos().x(), p->pos().y());
    }
    return 0;
}
