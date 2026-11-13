#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/SegmentTreeBuildModule.h"
#include "modules/SegmentTreeTagModule.h"
#include "modules/KMPMatchModule.h"
#include "modules/SandboxArrayModule.h"
#include "modules/SandboxMatrixModule.h"
#include "modules/SandboxTreeModule.h"
#include "modules/SandboxGraphModule.h"
#include "modules/SandboxLatexModule.h"
#include "render/LatexBlock.h"

static int g_fail = 0;

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static QImage shot(QGraphicsScene* scene, const QString& name) {
    QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p);
    img.save(name);
    return img;
}

static int nonWhite(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            if (img.pixelColor(x, y).lightness() < 250) n++;
    return n;
}

static int countGaps(const QImage& img, int y, int x0, int x1) {
    int gaps = 0;
    for (int x = x0; x < x1; ++x) {
        QColor c = img.pixelColor(x, y);
        if (c.lightness() > 248) gaps++;
    }
    return gaps;
}

static bool hasDarkBorderAt(const QImage& img, int y, int x) {
    for (int dy = -3; dy <= 3; ++dy) {
        if (y + dy < 0 || y + dy >= img.height()) continue;
        if (x < 0 || x >= img.width()) continue;
        if (img.pixelColor(x, y + dy).lightness() < 200) return true;
    }
    return false;
}

static std::vector<int> findBorderColumns(const QImage& img, int y, int x0, int x1) {
    std::vector<int> cols;
    for (int x = x0; x < x1; ++x) {
        if (img.pixelColor(x, y).lightness() < 200) cols.push_back(x);
    }
    return cols;
}

static std::vector<int> findBorderRuns(const QImage& img, int y, int x0, int x1) {
    std::vector<int> cols = findBorderColumns(img, y, x0, x1);
    std::vector<int> runs;
    for (size_t i = 0; i < cols.size(); ++i) {
        bool paired = (i + 1 < cols.size() && cols[i + 1] - cols[i] <= 1);
        if (paired) runs.push_back(cols[i]);
    }
    return runs;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    printf("=== start ===\n");

    {
        QGraphicsScene scene;
        SegmentTreeBuildModule m;
        m.buildLayout(&scene, "6 100\n1 2 3 4 5 6");
        auto frames = m.runAlgorithm();
        check("build: frames generated", frames.size() > 10);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "seg_build6.png");
        check("build: scene rendered", nonWhite(img) > 500);
        check("build: no 4n trailing blanks", img.width() < 1600);

        int deepestLeaf = 0;
        int trRow = 0;
        QList<QGraphicsItem*> items = scene.items();
        for (QGraphicsItem* it : items) {
            ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(it);
            if (!blk) continue;
            if (blk->labelText() == "1") trRow = qMax(trRow, static_cast<int>(blk->pos().y()));
        }
        for (QGraphicsItem* it : items) {
            GraphNode* nd = qgraphicsitem_cast<GraphNode*>(it);
            if (!nd) continue;
            if (nd->subLabelText().startsWith("[6,6]") || nd->subLabelText().startsWith("[5,6]")) {
                deepestLeaf = qMax(deepestLeaf, static_cast<int>(nd->pos().y()));
            }
        }
        check("build: tr row exists", trRow > 0);
        check("build: tr row below deepest leaf", trRow > deepestLeaf + 40);
    }

    {
        QGraphicsScene scene;
        SegmentTreeTagModule m;
        m.buildLayout(&scene, "6 2 100\n1 2 3 4 5 6\n1 1 6 3\n2 2 5 7");
        auto frames = m.runAlgorithm();
        check("tag: frames generated", frames.size() > 15);
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "seg_tag6.png");
        check("tag: scene rendered", nonWhite(img) > 500);

        QFontMetrics fm(QFont("Consolas", 10, QFont::Bold));
        int outside = 0;
        QList<QRectF> labelRects;
        for (QGraphicsItem* it : scene.items()) {
            GraphNode* nd = qgraphicsitem_cast<GraphNode*>(it);
            if (!nd) continue;
            QString txt = nd->subLabelText();
            if (txt.isEmpty()) continue;
            int w = fm.horizontalAdvance(txt) + 16;
            QRectF label(nd->pos().x() - w / 2.0, nd->pos().y() - 44, w, 18);
            if (!nd->boundingRect().contains(label.translated(-nd->pos()))) outside++;
            for (const QRectF& other : labelRects) {
                if (label.intersects(other)) outside++;
            }
            labelRects.push_back(label);
        }
        check("tag: node interval labels fit and do not overlap", outside == 0);
    }

    {
        QGraphicsScene scene;
        KMPMatchModule m;
        m.buildLayout(&scene, "ABABCABAA\nABAB");
        auto frames = m.runAlgorithm();
        for (auto& f : frames) m.renderFrame(f);
        QImage img = shot(&scene, "kmp_match2.png");
        check("kmp: scene rendered", nonWhite(img) > 300);
        std::vector<int> cols = findBorderRuns(img, 124, 140, 700);
        bool contiguous = cols.size() >= 10;
        for (size_t i = 1; i < cols.size() && contiguous; ++i) {
            int delta = cols[i] - cols[i - 1];
            if (delta < 52 || delta > 54) contiguous = false;
        }
        check("kmp: cell borders contiguous (no gaps)", contiguous);
        check("kmp: enough cell borders drawn", cols.size() >= 10);
    }

    {
        QGraphicsScene scene;
        SandboxArrayModule m;
        m.buildLayout(&scene, "6 10 20 30 40 50 60");
        QImage img = shot(&scene, "sb_array.png");
        check("sandbox array: rendered", nonWhite(img) > 200);
    }

    {
        QGraphicsScene scene;
        SandboxMatrixModule m;
        m.buildLayout(&scene, "rows=3\ncols=3\ntype=determinant\n1 2 3\n4 5 6\n7 8 9");
        QImage img = shot(&scene, "sb_matrix.png");
        check("sandbox matrix: rendered", nonWhite(img) > 200);
    }

    {
        QGraphicsScene scene;
        SandboxTreeModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6");
        QImage img = shot(&scene, "sb_tree.png");
        check("sandbox tree: rendered", nonWhite(img) > 200);
    }

    {
        QGraphicsScene scene;
        SandboxGraphModule m;
        m.buildLayout(&scene, "5\n1 2\n1 3\n2 4\n3 4\n4 5");
        QImage img = shot(&scene, "sb_graph.png");
        check("sandbox graph: rendered", nonWhite(img) > 200);
    }

    {
        bool loaded = LatexBlock::loadLibrary(QApplication::applicationDirPath() + "/latex_formulas.json");
        if (!loaded) loaded = LatexBlock::loadLibrary("../src/render/latex_formulas.json");
        if (!loaded) loaded = LatexBlock::loadLibrary("../../src/render/latex_formulas.json");
        check("latex: library loaded", loaded || !LatexBlock::library().isEmpty());
        QGraphicsScene scene;
        SandboxLatexModule m;
        m.buildLayout(&scene, "1\n3\n5");
        QImage img = shot(&scene, "sb_latex.png");
        check("sandbox latex: rendered", nonWhite(img) > 100);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}

