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

static int countMatrixBlocks(QGraphicsScene* scene) {
    int count = 0;
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<MatrixBlock*>(item)) ++count;
    }
    return count;
}

static QImage renderScene(QGraphicsScene* scene) {
    QGraphicsView view(scene);
    view.resize(600, 500);
    view.show();
    pump(80);
    view.centerOn(scene->itemsBoundingRect().center());
    pump(30);
    return view.grab().toImage();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene scene;
    SandboxMatrixModule mod;
    mod.buildLayout(&scene, "1 2 3\n4 5 6\n7 8 9");
    check(countMatrixBlocks(&scene) == 1, "single matrix block after build");

    mod.applyModuleSetting("shape", ModuleField::makeSegment("shape", "形态", {"矩阵", "行列式", "表格"}, 1));
    check(countMatrixBlocks(&scene) == 1, "shape switch keeps exactly one block");

    mod.applyModuleSetting("shape", ModuleField::makeSegment("shape", "形态", {"矩阵", "行列式", "表格"}, 2));
    check(countMatrixBlocks(&scene) == 1, "second shape switch keeps exactly one block");

    mod.applyModuleSetting("shape", ModuleField::makeSegment("shape", "形态", {"矩阵", "行列式", "表格"}, 0));
    check(countMatrixBlocks(&scene) == 1, "third shape switch keeps exactly one block");
    check(mod.matrixBlock() != nullptr, "matrix pointer still valid");
    check(mod.matrixBlock()->blockShape() == MatrixBlock::PlainMatrix, "matrix uses plain matrix shape");

    mod.applyModuleSetting("rows", ModuleField::makeInt("rows", "行数", 4, 1, 8));
    mod.applyModuleSetting("cols", ModuleField::makeInt("cols", "列数", 5, 1, 8));
    check(countMatrixBlocks(&scene) == 1, "dimension change keeps exactly one block");
    check(mod.matrixBlock()->rows == 4, "rows applied");
    check(mod.matrixBlock()->cols == 5, "cols applied");

    ModuleField sizeField = ModuleField::makeSize("cell", "单元格大小", 70, 34, 120, 40, 26, 120);
    mod.applyModuleSetting("cell", sizeField);
    check(mod.cellWidth() == 70, "cell width applied");
    check(mod.cellHeight() == 40, "cell height applied");
    check(qFuzzyCompare(mod.matrixBlock()->cellWidth(), 70.0), "block cell width applied");
    check(qFuzzyCompare(mod.matrixBlock()->cellHeightValue(), 40.0), "block cell height applied");

    QImage img = renderScene(&scene);
    check(!img.isNull(), "scene renders");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
