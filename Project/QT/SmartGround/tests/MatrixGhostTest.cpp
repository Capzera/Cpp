#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <cstdio>
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

static QImage render(QGraphicsScene* scene) {
    QGraphicsView view(scene);
    view.resize(700, 560);
    view.show();
    pump(90);
    view.centerOn(scene->itemsBoundingRect().center());
    pump(40);
    QImage img = view.grab().toImage();
    img.save("matrix_ghost_check.png");
    return img;
}

static int countDarkPixels(const QImage& img, const QRect& rect) {
    int count = 0;
    for (int y = rect.top(); y < rect.bottom(); ++y) {
        for (int x = rect.left(); x < rect.right(); ++x) {
            if (img.isNull()) continue;
            QColor c = img.pixelColor(x, y);
            if (c.lightness() < 120) ++count;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene scene;
    SandboxMatrixModule mod;
    mod.buildLayout(&scene, "1 2 3\n4 5 6\n7 8 9");

    QImage tableShot = render(&scene);
    check(!tableShot.isNull(), "table shot captured");

    ModuleField shapeField = ModuleField::makeSegment("shape", "形态", {"矩阵", "行列式", "表格"}, 0);
    mod.applyModuleSetting("shape", shapeField);
    pump(120);

    QImage matrixShot = render(&scene);
    check(!matrixShot.isNull(), "matrix shot captured");

    QRect full = matrixShot.rect().adjusted(40, 40, -40, -40);
    int tableDark = countDarkPixels(tableShot, full);
    int matrixDark = countDarkPixels(matrixShot, full);
    printf("dark pixels table=%d matrix=%d\n", tableDark, matrixDark);
    check(tableDark < matrixDark, "table has no outer brackets (fewer ink pixels than matrix)");
    check(matrixDark > tableDark * 2, "matrix draws square brackets around the grid");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
