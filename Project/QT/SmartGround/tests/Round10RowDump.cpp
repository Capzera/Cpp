#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"

static void dumpRow(QGraphicsScene* scene, double y, const char* tag) {
    std::vector<ArrayBlock*> row;
    for (QGraphicsItem* item : scene->items()) {
        ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
        if (!b) continue;
        if (qAbs(b->pos().y() - y) < 2.0) row.push_back(b);
    }
    std::sort(row.begin(), row.end(), [](ArrayBlock* a, ArrayBlock* b) {
        return a->pos().x() < b->pos().x();
    });
    printf("%s: %zu cells\n", tag, row.size());
    for (size_t i = 0; i < row.size(); ++i) {
        double gap = 0.0;
        if (i > 0) gap = row[i]->pos().x() - row[i - 1]->pos().x() - row[i - 1]->cellWidth();
        printf("  cell %d id=%d x=%.1f w=%.1f gap=%.1f\n", (int)i, row[i]->id,
               row[i]->pos().x(), row[i]->cellWidth(), gap);
    }
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    {
        TrieModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "3\ncat\ncar\ndog");
        mod.m_animated = false;
        auto frames = mod.runAlgorithm();
        for (auto& f : frames) mod.renderFrame(f);
        dumpRow(&scene, 470.0, "trie string row");
        dumpRow(&scene, 560.0, "trie slot row");
    }
    {
        HashTableModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        mod.m_animated = false;
        auto frames = mod.runAlgorithm();
        for (auto& f : frames) mod.renderFrame(f);
        dumpRow(&scene, 118.0, "hash table row");
        dumpRow(&scene, 220.0, "hash key row");
    }
    return 0;
}
