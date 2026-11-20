#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "modules/DfsModule.h"
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
#include "modules/PrimModule.h"
#include "modules/GcdModule.h"
#include "modules/BipartiteModule.h"

static void renderModule(IModule* mod, const QString& input, const QString& tag, int frameIndex) {
    QGraphicsScene scene;
    mod->buildLayout(&scene, input);
    auto frames = mod->runAlgorithm();
    mod->m_animated = false;
    if (frameIndex < 0) frameIndex = static_cast<int>(frames.size()) - 1;
    for (int i = 0; i <= frameIndex && i < static_cast<int>(frames.size()); ++i) {
        mod->renderFrame(frames[i]);
    }
    const QRectF bounds = scene.itemsBoundingRect().adjusted(-30, -30, 30, 30);
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    scene.render(&p, QRectF(QPointF(0, 0), img.size()), bounds);
    p.end();
    img.save(QString("vis_%1.png").arg(tag).toUtf8().constData());
    printf("saved vis_%1.png  frames=%2 bounds=%3\n", tag.toUtf8().constData(),
           frames.size(), qPrintable(QString("%1,%2 %3x%4").arg(bounds.x()).arg(bounds.y())
                                     .arg(bounds.width()).arg(bounds.height())));
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    { DfsModule m; renderModule(&m, "5 5 1\n1 2\n2 3\n3 1\n3 4\n4 5", "dfs", 4); }
    { TrieModule m; renderModule(&m, "3\ncat\ncar\ndog", "trie", -1); }
    { HashTableModule m; renderModule(&m, m.getDefaultInput(), "hash", -1); }
    { PrimModule m; renderModule(&m, m.getDefaultInput(), "prim", -1); }
    { GcdModule m; renderModule(&m, "48 18", "gcd", -1); }
    { BipartiteModule m; renderModule(&m, m.getDefaultInput(), "bipartite", -1); }

    return 0;
}
