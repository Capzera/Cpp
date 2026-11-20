#include "modules/HuffmanTreeModule.h"
#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    HuffmanTreeModule m;
    m.buildLayout(&scene, "7\n45 13 20 16 9 12 5");
    auto frames = m.runAlgorithm();
    for (auto& f : frames) m.renderFrame(f);
    printf("sceneRect=%g,%g %gx%g\n", scene.sceneRect().x(), scene.sceneRect().y(),
           scene.sceneRect().width(), scene.sceneRect().height());
    for (const auto& f : frames) {
        if (f.type != ActionType::HF_MERGE) continue;
        printf("merge %d + %d = %d\n", f.u, f.v, f.val1);
    }
    int n = 0;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* ab = qgraphicsitem_cast<ArrayBlock*>(item);
        if (!ab) continue;
        if (ab->id < 6000) continue;
        printf("array id=%d pos=(%g,%g) val='%s'\n", ab->id, ab->pos().x(), ab->pos().y(),
               ab->customValue.toStdString().c_str());
        if (++n > 30) break;
    }
    return 0;
}
