#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include "modules/PrefixSumModule.h"
#include "modules/BubbleSortModule.h"
#include "render/GraphElements.h"
#include "render/EditableModel.h"

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

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    EditableModel::instance().clear();

    PrefixSumModule mod;
    mod.buildLayout(&scene, "4 1\n1 2 3 4\n1 4");
    mod.enableLiveEditing();

    ArrayBlock* s2 = nullptr;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
        if (!blk) continue;
        if (blk->id == 2 && blk->pos().y() > 300) s2 = blk;
    }
    check(s2 != nullptr, "prefix sum s[2] block exists");
    if (!s2) { printf("\n%d failures\n", failures); return 1; }

    check(s2->isEditable(), "s[2] block is editable");
    std::vector<AnimFrame> frames = mod.runAlgorithm();
    check(!frames.empty(), "frames recorded");

    s2->value = 100;
    s2->showValue = true;
    s2->customValue.clear();
    s2->update();
    emit s2->valueEdited(102, "100");

    int stored = 0;
    check(EditableModel::instance().getValue(mod.liveScope(), "s", 2, stored) && stored == 100,
          "live override stored for s[2]");

    std::vector<AnimFrame> frames2 = mod.runAlgorithm();
    bool foundHundred = false;
    for (const AnimFrame& f : frames2) {
        if (f.mathText.contains("100") || f.explanation.contains("100")) { foundHundred = true; break; }
    }
    check(foundHundred, "recomputed frames reflect teacher edit of s[2]");

    bool hasS3 = false, hasS4 = false;
    for (const AnimFrame& f : frames2) {
        if (f.mathText.contains("S[3] = 100 + 3 = 103")) hasS3 = true;
        if (f.mathText.contains("S[4] = 103 + 4 = 107")) hasS4 = true;
    }
    check(hasS3 && hasS4, "downstream s[3]/s[4] recomputed from edited s[2]");

    scene.clear();
    EditableModel::instance().clear();

    QGraphicsScene scene2;
    BubbleSortModule bmod;
    bmod.buildLayout(&scene2, "4\n5 1 4 2");
    bmod.enableLiveEditing();
    ArrayBlock* b0 = nullptr;
    for (QGraphicsItem* item : scene2.items()) {
        ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
        if (blk && blk->id == 0) b0 = blk;
    }
    check(b0 != nullptr && b0->isEditable(), "bubble a[0] block editable");
    if (b0) {
        b0->value = 99;
        b0->showValue = true;
        b0->customValue.clear();
        b0->update();
        emit b0->valueEdited(0, "99");
        std::vector<AnimFrame> bf = bmod.runAlgorithm();
        bool has99 = false;
        for (const AnimFrame& f : bf) {
            if (f.explanation.contains("99")) { has99 = true; break; }
        }
        check(has99, "bubble sort frames reflect edited a[0]=99");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
