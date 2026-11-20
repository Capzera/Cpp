#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "modules/SandboxMatrixModule.h"
#include "modules/GcdModule.h"
#include "modules/DfsModule.h"
#include "modules/PrimModule.h"
#include "modules/CountingSortModule.h"
#include "modules/BipartiteModule.h"
#include "modules/EulerModule.h"
#include "modules/IntervalSchedModule.h"
#include "modules/TrieModule.h"
#include "modules/HashTableModule.h"
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

static bool rendersNonBlank(IModule* mod, const QString& input, const QString& tag) {
    QGraphicsScene scene;
    mod->buildLayout(&scene, input);
    if (mod->runAlgorithm().empty()) {
        printf("  [warn] %s produced no frames\n", tag.toUtf8().constData());
        return false;
    }
    const QRectF bounds = scene.itemsBoundingRect().adjusted(-20, -20, 20, 20);
    if (bounds.isEmpty()) {
        printf("  [warn] %s produced empty bounds\n", tag.toUtf8().constData());
        return false;
    }
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    scene.render(&p, QRectF(QPointF(0, 0), img.size()), bounds);
    p.end();
    img.save(QString("newmod_%1.png").arg(tag).toUtf8().constData());
    return !img.isNull();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        SandboxMatrixModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, mod.getDefaultInput());
        check(mod.currentShapeIndex() == 2, "matrix default shape = table");
        check(mod.rows() == 3 && mod.cols() == 3, "matrix default 3x3");
        check(mod.matrixBlock() != nullptr, "matrix block built");
        if (mod.matrixBlock()) {
            check(mod.matrixBlock()->blockShape() == MatrixBlock::BracketedTable, "shape is BracketedTable");
            check(mod.matrixBlock()->cellAt(0, 0) == "1", "row-major value (0,0)=1");
            check(mod.matrixBlock()->cellAt(1, 0) == "4", "row-major value (1,0)=4");
            check(mod.matrixBlock()->cellAt(2, 2) == "9", "row-major value (2,2)=9");
        }
    }

    {
        SandboxMatrixModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "1 2 3 4\n5 6 7 8\n9 10 11 12");
        check(mod.rows() == 3 && mod.cols() == 3, "rows/cols default even with 12 values");
        if (mod.matrixBlock()) {
        check(mod.matrixBlock()->cellAt(0, 2) == "3", "row-major (0,2)=3");
        check(mod.matrixBlock()->cellAt(1, 0) == "4", "row-major (1,0)=4");
        check(mod.matrixBlock()->cellAt(2, 2) == "9", "extra values dropped");
    }

    {
        SandboxMatrixModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "1 2\n3");
        check(mod.rows() == 3 && mod.cols() == 3, "short input keeps 3x3");
        if (mod.matrixBlock()) {
            check(mod.matrixBlock()->cellAt(1, 1) == "0", "missing value padded to 0");
            check(mod.matrixBlock()->cellAt(2, 2) == "0", "missing value padded to 0");
        }
    }
    }

    {
        SandboxMatrixModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "1 2 3\n4 5 6\n7 8 9");
        ModuleField shapeField;
        shapeField.key = "shape";
        shapeField.intValue = 1;
        mod.applyModuleSetting("shape", shapeField);
        check(mod.currentShapeIndex() == 1, "shape switched to determinant");
        check(mod.matrixBlock()->blockShape() == MatrixBlock::Determinant, "determinant shape applied");

        ModuleField rowsField;
        rowsField.key = "rows";
        rowsField.intValue = 2;
        mod.applyModuleSetting("rows", rowsField);
        check(mod.rows() == 2 && mod.matrixBlock()->rows == 2, "rows applied to block");
    }

    {
        SandboxMatrixModule mod;
        const QVector<ModuleField> fields = mod.moduleSettingsFields();
        bool hasShape = false;
        bool hasRows = false;
        bool hasCols = false;
        for (const ModuleField& f : fields) {
            if (f.key == "shape") {
                hasShape = true;
                check(f.type == ModuleField::Segment, "shape field is segment");
                check(f.options.size() == 3, "shape has 3 options");
                check(f.options.contains("矩阵") && f.options.contains("行列式") && f.options.contains("表格"),
                      "shape options are matrix/determinant/table");
            }
            if (f.key == "rows") hasRows = true;
            if (f.key == "cols") hasCols = true;
        }
        check(hasShape && hasRows && hasCols, "settings expose shape/rows/cols");
        check(mod.moduleSettingsTitle() == "矩阵设置", "settings title = 矩阵设置");
    }

    {
        GcdModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "48 18");
        check(!mod.runAlgorithm().empty(), "gcd produces frames");
    }
    {
        DfsModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "5 5 1\n1 2\n2 3\n3 1\n3 4\n4 5");
        check(!mod.runAlgorithm().empty(), "dfs produces frames");
    }
    {
        PrimModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4 4 1\n1 2 1\n1 3 4\n2 3 2\n3 4 3");
        check(!mod.runAlgorithm().empty(), "prim produces frames");
    }
    {
        CountingSortModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "6\n5 3 8 1 9 4");
        check(!mod.runAlgorithm().empty(), "counting sort produces frames");
    }
    {
        BipartiteModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4 4\n1 2\n2 3\n3 4\n1 4");
        check(!mod.runAlgorithm().empty(), "bipartite produces frames");
    }
    {
        EulerModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4 4\n1 2\n2 3\n3 4\n4 1");
        check(!mod.runAlgorithm().empty(), "euler produces frames");
    }
    {
        IntervalSchedModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4\n1 3\n2 5\n3 7\n5 9");
        check(!mod.runAlgorithm().empty(), "interval sched produces frames");
    }
    {
        TrieModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "3\ncat\ncar\ncart");
        check(!mod.runAlgorithm().empty(), "trie produces frames");
    }
    {
        HashTableModule mod;
        QGraphicsScene scene;
        mod.buildLayout(&scene, "4\n3 10 17 5");
        check(!mod.runAlgorithm().empty(), "hash table produces frames");
    }

    {
        GcdModule mod;
        check(rendersNonBlank(&mod, "48 18", "gcd"), "gcd renders");
    }
    {
        DfsModule mod;
        check(rendersNonBlank(&mod, "5 5 1\n1 2\n2 3\n3 1\n3 4\n4 5", "dfs"), "dfs renders");
    }
    {
        PrimModule mod;
        check(rendersNonBlank(&mod, "4 4 1\n1 2 1\n1 3 4\n2 3 2\n3 4 3", "prim"), "prim renders");
    }
    {
        CountingSortModule mod;
        check(rendersNonBlank(&mod, "6\n5 3 8 1 9 4", "counting"), "counting renders");
    }
    {
        BipartiteModule mod;
        check(rendersNonBlank(&mod, "4 4\n1 2\n2 3\n3 4\n1 4", "bipartite"), "bipartite renders");
    }
    {
        EulerModule mod;
        check(rendersNonBlank(&mod, "4 4\n1 2\n2 3\n3 4\n4 1", "euler"), "euler renders");
    }
    {
        IntervalSchedModule mod;
        check(rendersNonBlank(&mod, "4\n1 3\n2 5\n3 7\n5 9", "interval"), "interval renders");
    }
    {
        TrieModule mod;
        check(rendersNonBlank(&mod, "3\ncat\ncar\ncart", "trie"), "trie renders");
    }
    {
        HashTableModule mod;
        check(rendersNonBlank(&mod, "4\n3 10 17 5", "hashtable"), "hashtable renders");
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
