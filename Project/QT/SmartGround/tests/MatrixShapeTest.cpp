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

static QImage renderShape(int shapeIndex) {
    QGraphicsScene scene;
    SandboxMatrixModule mod;
    mod.buildLayout(&scene, "1 2 3\n4 5 6\n7 8 9");
    MatrixBlock* block = mod.matrixBlock();
    if (!block) return QImage();
    block->setShape(shapeIndex == 1 ? MatrixBlock::Determinant
                                   : (shapeIndex == 2 ? MatrixBlock::BracketedTable
                                                     : MatrixBlock::PlainMatrix));
    if (shapeIndex == 1) {
        block->cellColor = QColor(253, 250, 242);
        block->gridColor = QColor(206, 190, 158);
        block->barColor = QColor(150, 120, 70);
        block->setTitle("行列式");
    } else if (shapeIndex == 2) {
        block->cellColor = QColor(255, 255, 255);
        block->gridColor = QColor(158, 170, 188);
        block->barColor = QColor(50, 70, 105);
        block->setTitle("表格");
    }
    block->update();

    QGraphicsView view(&scene);
    view.resize(600, 500);
    view.show();
    pump(80);
    view.centerOn(scene.itemsBoundingRect().center());
    pump(30);
    return view.grab().toImage();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    const char* names[3] = {"matrix", "determinant", "table"};
    for (int i = 0; i < 3; ++i) {
        QImage img = renderShape(i);
        check(!img.isNull(), "shape renders");
        img.save(QString("matrix_shape_%1.png").arg(names[i]).toUtf8().constData());
    }

    QGraphicsScene scene;
    SandboxMatrixModule mod;
    mod.buildLayout(&scene, "1 2 3\n4 5 6");
    check(mod.matrixBlock() != nullptr, "block built");
    check(mod.matrixBlock()->rows == 3, "default rows = 3");
    check(mod.matrixBlock()->cols == 3, "default cols = 3");
    check(mod.matrixBlock()->cellAt(0, 0) == "1", "value 1");
    check(mod.matrixBlock()->cellAt(0, 2) == "3", "value 3");
    check(mod.matrixBlock()->cellAt(1, 2) == "6", "value 6");
    check(mod.matrixBlock()->cellAt(1, 0) == "4", "value 4");
    check(mod.matrixBlock()->cellAt(2, 2) == "0", "missing value padded to 0");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
