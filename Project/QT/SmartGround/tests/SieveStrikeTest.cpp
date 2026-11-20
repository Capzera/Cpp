#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <cstdio>
#include "modules/SieveOfEratosthenesModule.h"
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

static ArrayBlock* blockFor(QGraphicsScene& scene, int value) {
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
        if (block && block->value == value && block->customValue.isEmpty()) {
            if (block->isStruck()) return block;
        }
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;

    SieveOfEratosthenesModule mod;
    mod.buildLayout(&scene, "30");
    std::vector<AnimFrame> frames = mod.runAlgorithm();
    for (const AnimFrame& f : frames) mod.renderFrame(f);
    pump(50);

    ArrayBlock* struck = blockFor(scene, 4);
    check(struck != nullptr, "composite cell 4 found");
    if (!struck) { printf("\n%d failures\n", failures); return 1; }

    check(struck->isStruck(), "composite cell marked as struck");
    check(qFuzzyCompare(struck->scale(), 1.0), "struck cell keeps full scale (no shrink)");

    ArrayBlock* prime = nullptr;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* block = qgraphicsitem_cast<ArrayBlock*>(item);
        if (block && block->value == 7 && !block->isStruck()) prime = block;
    }
    check(prime != nullptr, "prime cell 7 found");
    if (prime) {
        check(qFuzzyCompare(prime->cellWidth(), struck->cellWidth()), "struck and prime cells share cell width");
        check(qFuzzyCompare(prime->cellHeight(), struck->cellHeight()), "struck and prime cells share cell height");
    }

    QImage img(60, 50, QImage::Format_ARGB32);
    img.fill(Qt::white);
    {
        QPainter p(&img);
        p.translate(2, 2);
        struck->paint(&p, nullptr, nullptr);
    }
    int darkRows = 0;
    bool inRow = false;
    for (int y = 0; y < img.height(); ++y) {
        bool has = false;
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 130 && c.green() < 130 && c.blue() < 130) { has = true; break; }
        }
        if (has && !inRow) { darkRows++; inRow = true; }
        else if (!has) inRow = false;
    }
    check(darkRows <= 1, "struck cell draws no strikethrough line (single text row only)");

    printf("\n%d failures\n", failures);
    return failures ? 1 : 0;
}
