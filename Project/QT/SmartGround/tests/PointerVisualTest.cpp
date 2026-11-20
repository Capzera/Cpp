#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <cstdio>
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
#include "modules/PrimModule.h"
#include "modules/BipartiteModule.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void cropScene(QGraphicsView* view, const QImage& shot, const char* file,
                      QPointF center, int w, int h) {
    if (!view) return;
    QPointF tl = view->mapFromScene(center - QPointF(w / 2.0, h / 2.0));
    int ox = qMax(0, static_cast<int>(tl.x()));
    int oy = qMax(0, static_cast<int>(tl.y()));
    QImage sub = shot.copy(ox, oy, w, h);
    sub.scaled(w * 2, h * 2).save(file);
    printf("saved %s\n", file);
}

template <typename Mod>
static void shoot(Mod& mod, const char* file, const QString& input, int stopFrame,
                  const char* cropFile, QPointF cropCenter, int cw, int ch) {
    QGraphicsScene scene;
    mod.buildLayout(&scene, input);
    auto frames = mod.runAlgorithm();
    int limit = stopFrame < 0 ? static_cast<int>(frames.size()) : std::min(stopFrame + 1, static_cast<int>(frames.size()));
    for (int i = 0; i < limit; ++i) mod.renderFrame(frames[i]);
    QGraphicsView view(&scene);
    view.resize(1100, 820);
    view.show();
    pump(120);
    view.fitInView(scene.itemsBoundingRect().adjusted(-40, -40, 40, 40), Qt::KeepAspectRatio);
    pump(60);
    QImage shot = view.grab().toImage();
    shot.save(file);
    printf("saved %s\n", file);
    if (cropFile) cropScene(&view, shot, cropFile, cropCenter, cw, ch);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    TrieModule trie;
    shoot(trie, "trie_shot.png", "3\ncat\ncar\ndog", 1, "trie_crop.png", QPointF(300, 520), 460, 200);

    HashTableModule hash;
    shoot(hash, "hash_shot.png", "4\n5\n3\n7\n11", 1, "hash_crop.png", QPointF(430, 160), 560, 260);

    PrimModule prim;
    shoot(prim, "prim_shot.png", "6 9 1\n1 2 4\n1 3 3\n2 3 2\n2 4 5\n3 4 3\n3 5 6\n4 5 1\n4 6 7\n5 6 8",
          1, "prim_crop.png", QPointF(470, 200), 420, 340);

    BipartiteModule bip;
    shoot(bip, "bip_shot.png", "5 6\n1 2\n1 3\n2 4\n2 5\n3 4\n4 5", 1, "bip_crop.png", QPointF(470, 350), 700, 500);

    return 0;
}
