#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MySegmentedControl.h"
#include "modules/SandboxMatrixModule.h"
#include "render/MatrixBlock.h"

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

static QImage grabCanvas(QGraphicsView* view) {
    QImage img = view->grab().toImage();
    return img;
}

static int countDark(const QImage& img) {
    int count = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            if (img.pixelColor(x, y).lightness() < 110) ++count;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 900);
    w.show();
    pump(700);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    int matrixIndex = -1;
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("矩阵")) { matrixIndex = i; break; }
    }
    if (matrixIndex < 0) { printf("no matrix module\n"); return 1; }
    combo->setCurrentIndex(matrixIndex);
    pump(600);

    w.probeBuild();
    pump(500);

    SandboxMatrixModule* mod = dynamic_cast<SandboxMatrixModule*>(w.probeCurrentModule());
    check(mod != nullptr, "matrix module found");
    if (!mod) return 1;

    QGraphicsScene* scene = w.probeScene();
    QGraphicsView* view = scene->views().isEmpty() ? nullptr : scene->views().first();
    check(view != nullptr, "canvas view found");

    MySegmentedControl* segment = w.findChild<MySegmentedControl*>();
    check(segment != nullptr, "shape segmented control found");

    QImage before = grabCanvas(view);
    before.save("ghost_before.png");

    if (segment) {
        segment->setCurrentIndex(1);
        pump(500);
    }
    QImage after = grabCanvas(view);
    after.save("ghost_after.png");

    int blocks = 0;
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<MatrixBlock*>(item)) ++blocks;
    }
    check(blocks == 1, "exactly one matrix block after shape switch");
    printf("dark before=%d after=%d\n", countDark(before), countDark(after));

    QImage rightAfter = grabCanvas(view);
    segment->setCurrentIndex(2);
    QImage immediate = grabCanvas(view);
    pump(500);
    QImage settled = grabCanvas(view);
    rightAfter.save("ghost_right_after.png");
    settled.save("ghost_settled.png");
    printf("dark immediate=%d settled=%d\n", countDark(immediate), countDark(settled));
    check(countDark(immediate) == countDark(settled),
          "canvas refreshes immediately after shape switch (no ghost until viewport moves)");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
