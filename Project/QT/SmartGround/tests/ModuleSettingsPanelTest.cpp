#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
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

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 900);
    w.show();
    pump(600);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    check(combo != nullptr, "combo exists");
    if (!combo) return 1;

    int matrixIndex = -1;
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("矩阵")) { matrixIndex = i; break; }
    }
    check(matrixIndex > 0, "matrix module listed");
    combo->setCurrentIndex(matrixIndex);
    pump(500);

    SandboxMatrixModule* mod = dynamic_cast<SandboxMatrixModule*>(w.probeCurrentModule());
    check(mod != nullptr, "matrix module instance found");
    if (!mod) { printf("\n%s  fail=%d\n", "HAS FAILURE", g_fail); return 1; }

    w.probeBuild();
    pump(400);
    check(mod->matrixBlock() != nullptr, "matrix block built");
    check(mod->matrixBlock() && mod->matrixBlock()->isEditable(), "cells editable");
    check(mod->matrixBlock() && mod->matrixBlock()->rows == 3, "default 3 rows");
    check(mod->matrixBlock() && mod->matrixBlock()->cols == 3, "default 3 cols");
    check(mod->matrixBlock() && mod->matrixBlock()->cellAt(0, 0) == "1", "cell(0,0) filled from input");

    QVector<ModuleField> fields = mod->moduleSettingsFields();
    check(fields.size() >= 4, "module exposes settings fields");
    bool hasShape = false;
    bool hasRows = false;
    bool hasCols = false;
    for (const ModuleField& f : fields) {
        if (f.key == "shape") hasShape = true;
        if (f.key == "rows") hasRows = true;
        if (f.key == "cols") hasCols = true;
    }
    check(hasShape, "shape field present");
    check(hasRows, "rows field present");
    check(hasCols, "cols field present");
    check(!mod->moduleSettingsTitle().isEmpty(), "title non-empty");

    ModuleField shapeField;
    ModuleField rowsField;
    ModuleField colsField;
    for (const ModuleField& f : fields) {
        if (f.key == "shape") shapeField = f;
        if (f.key == "rows") rowsField = f;
        if (f.key == "cols") colsField = f;
    }
    check(shapeField.options.size() == 3, "shape has three options");
    check(shapeField.type == ModuleField::Segment, "shape is a segmented control");
    check(rowsField.type == ModuleField::IntInput, "rows is a text int input");
    check(colsField.type == ModuleField::IntInput, "cols is a text int input");
    check(w.probeModulePanelVisible(), "matrix settings panel visible for matrix module");
    check(w.probeModulePanelTitle() == "矩阵设置", "panel title is 矩阵设置");

    shapeField.intValue = 1;
    mod->applyModuleSetting("shape", shapeField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->blockShape() == MatrixBlock::Determinant,
          "determinant shape applied");

    shapeField.intValue = 2;
    mod->applyModuleSetting("shape", shapeField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->blockShape() == MatrixBlock::BracketedTable,
          "table shape applied");

    shapeField.intValue = 0;
    mod->applyModuleSetting("shape", shapeField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->blockShape() == MatrixBlock::PlainMatrix,
          "plain matrix shape applied");

    rowsField.intValue = 4;
    mod->applyModuleSetting("rows", rowsField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->rows == 4, "rows resized to 4");

    colsField.intValue = 5;
    mod->applyModuleSetting("cols", colsField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->cols == 5, "cols resized to 5");
    check(mod->matrixBlock() && mod->matrixBlock()->cellAt(0, 0) == "1", "data preserved after resize");

    rowsField.intValue = 99;
    mod->applyModuleSetting("rows", rowsField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->rows <= 8, "rows clamped to max 8");

    mod->applyModuleSetting("shape", shapeField);
    mod->applyModuleSetting("rows", rowsField);
    mod->applyModuleSetting("cols", colsField);
    pump(200);
    check(mod->matrixBlock() && mod->matrixBlock()->cols == 5, "cols setting survives rebuild");

    QGraphicsScene* scene = w.probeScene();
    check(scene != nullptr, "scene accessible");
    if (scene) {
        int blockCount = 0;
        for (QGraphicsItem* item : scene->items()) {
            if (dynamic_cast<MatrixBlock*>(item)) ++blockCount;
        }
        check(blockCount == 1, "exactly one matrix block in scene after rebuilds");
    }

    w.grab().save("app_matrix_panel.png");

    w.probeSelectModule("BFS 广度优先搜索");
    pump(500);
    check(w.probeModulePanelVisible(), "graph options panel visible for BFS");
    check(w.probeModulePanelTitle() != "矩阵设置", "matrix title not shown for other module");

    w.probeSelectModule("01 背包");
    pump(500);
    check(!w.probeModulePanelVisible(), "matrix settings auto-hidden for 01 knapsack");

    w.probeSelectModule("自由结构 - 矩阵 / 行列式");
    pump(500);
    check(w.probeModulePanelVisible(), "matrix settings shown again for matrix module");
    check(w.probeModulePanelTitle() == "矩阵设置", "matrix title restored");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
