#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include <QMouseEvent>
#include "modules/FenwickTreeModule.h"
#include "modules/HeapAdjustModule.h"
#include "modules/HeapSortModule.h"
#include "modules/SegmentTreeBuildModule.h"
#include "modules/SandboxGraphModule.h"
#include "modules/SandboxArrayModule.h"
#include "modules/SandboxMatrixModule.h"
#include "modules/SandboxTreeModule.h"
#include "render/GraphElements.h"
#include "ui/widgets/MySlider.h"

static int g_fail = 0;
static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static int nonWhite(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            if (img.pixelColor(x, y).lightness() < 250) n++;
    return n;
}

static int textItems(QGraphicsScene* scene) {
    int n = 0;
    for (QGraphicsItem* item : scene->items()) {
        if (qgraphicsitem_cast<QGraphicsTextItem*>(item)) ++n;
    }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        QGraphicsScene scene;
        FenwickTreeModule m;
        m.buildLayout(&scene, "8\n1 2 3 4 5 6 7 8");
        int lines = 0, polys = 0, blocks = 0;
        for (QGraphicsItem* item : scene.items()) {
            if (qgraphicsitem_cast<QGraphicsLineItem*>(item)) ++lines;
            if (qgraphicsitem_cast<QGraphicsPolygonItem*>(item)) ++polys;
            if (qgraphicsitem_cast<ArrayBlock*>(item)) ++blocks;
        }
        check("fenwick: builds array + level lattice blocks", blocks >= 8 + 8);
        check("fenwick: has level guide shapes", polys >= 8);
        check("fenwick: has level guide lines", lines >= 8);
    }

    {
        QGraphicsScene scene;
        HeapAdjustModule m;
        m.buildLayout(&scene, "5\n10 5 8 2 9");
        int nodesWithValueInBody = 0;
        int nodesWithIndexBeside = 0;
        for (QGraphicsItem* item : scene.items()) {
            GraphNode* n = qgraphicsitem_cast<GraphNode*>(item);
            if (!n) continue;
            if (!n->customValueText.isEmpty()) ++nodesWithValueInBody;
            if (!n->subLabel.isEmpty()) ++nodesWithIndexBeside;
        }
        check("heap: values inside node circles", nodesWithValueInBody == 5);
        check("heap: indices beside nodes", nodesWithIndexBeside == 5);
        int arrayBlocks = 0;
        for (QGraphicsItem* item : scene.items())
            if (qgraphicsitem_cast<ArrayBlock*>(item)) ++arrayBlocks;
        check("heap: complete binary tree array below", arrayBlocks == 5);

        auto frames = m.runAlgorithm();
        bool foundSwap = false;
        for (const auto& f : frames) {
            if (f.type != ActionType::HG_SWAP) continue;
            foundSwap = true;
            m.renderFrame(f);
            int a = f.u;
            int b = f.v;
            QString va, vb;
            for (QGraphicsItem* item : scene.items()) {
                GraphNode* n = qgraphicsitem_cast<GraphNode*>(item);
                if (!n) continue;
                if (n->id == a) va = n->customValueText;
                if (n->id == b) vb = n->customValueText;
            }
            check(QString("heap swap: node %1 now holds old %2").arg(a).arg(f.val2).toUtf8().constData(),
                  va == QString::number(f.val2));
            check(QString("heap swap: node %1 now holds old %2").arg(b).arg(f.val1).toUtf8().constData(),
                  vb == QString::number(f.val1));
            QString ba, bb;
            for (QGraphicsItem* item : scene.items()) {
                ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
                if (!blk) continue;
                if (blk->id == a) ba = QString::number(blk->value);
                if (blk->id == b) bb = QString::number(blk->value);
            }
            check(QString("heap swap: array[%1] holds %2").arg(a).arg(f.val2).toUtf8().constData(),
                  ba == QString::number(f.val2));
            check(QString("heap swap: array[%1] holds %2").arg(b).arg(f.val1).toUtf8().constData(),
                  bb == QString::number(f.val1));
            break;
        }
        check("heap: swap frame exists", foundSwap);
    }

    {
        QGraphicsScene scene;
        SegmentTreeBuildModule m;
        m.buildLayout(&scene, "4\n1 2 3 4");
        int trCount = 0, lzCount = 0;
        for (QGraphicsItem* item : scene.items()) {
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!b) continue;
            if (b->pos().y() > 500.0 && b->pos().y() < 600.0) ++trCount;
            else if (b->pos().y() >= 600.0) ++lzCount;
        }
        check("segment: tr row rendered", trCount >= 7);
        check("segment: lz row rendered", lzCount >= 7);
        auto frames = m.runAlgorithm();
        check("segment: build produces frames", frames.size() > 5);
        bool hasPushup = false, hasLeaf = false;
        for (const auto& f : frames) {
            if (f.type == ActionType::SEG_PUSHUP) hasPushup = true;
            if (f.type == ActionType::SEG_LEAF) hasLeaf = true;
            if (f.mathText.contains("%")) hasPushup = false;
        }
        check("segment: push_up frame present", hasPushup);
        check("segment: leaf frame present", hasLeaf);
    }

    {
        QGraphicsScene scene;
        SandboxArrayModule m;
        m.buildLayout(&scene, "5 1 2 3 4 5");
        check("sandbox array: no canvas text", textItems(&scene) == 0);
    }
    {
        QGraphicsScene scene;
        SandboxMatrixModule m;
        m.buildLayout(&scene, "rows=3\ncols=3\ntype=matrix\n1 2 3\n4 5 6\n7 8 9");
        check("sandbox matrix: no canvas text", textItems(&scene) == 0);
    }
    {
        QGraphicsScene scene;
        SandboxTreeModule m;
        m.buildLayout(&scene, "1 2 3\n2 4 5\n3 6");
        check("sandbox tree: no canvas text", textItems(&scene) == 0);
    }
    {
        QGraphicsScene scene;
        SandboxGraphModule m;
        m.buildLayout(&scene, "5\n1 2 3\n1 3 2\n2 4 5\n3 4 1\n4 5 4");
        check("sandbox graph: no canvas text", textItems(&scene) == 0);
        int weighted = 0;
        for (QGraphicsItem* item : scene.items()) {
            GraphEdge* e = qgraphicsitem_cast<GraphEdge*>(item);
            if (e && e->has_weight) ++weighted;
        }
        check("sandbox graph: 5 weighted edges from u v w", weighted == 5);
    }

    {
        QGraphicsScene scene;
        SandboxGraphModule m;
        m.buildLayout(&scene, "4\n1 2\n2 3\n3 4");
        int weighted = 0;
        for (QGraphicsItem* item : scene.items()) {
            GraphEdge* e = qgraphicsitem_cast<GraphEdge*>(item);
            if (e && e->has_weight) ++weighted;
        }
        check("sandbox graph: no default weights for 2-token lines", weighted == 0);
    }

    {
        MySlider slider;
        slider.setRange(0, 100);
        slider.resize(240, 26);
        slider.setValue(0);
        slider.setGlow(1.0);
        slider.show();
        QApplication::processEvents();
        QImage img = slider.grab().toImage();
        int dark = 0;
        for (int y = 0; y < img.height(); ++y)
            for (int x = 0; x < img.width(); ++x)
                if (img.pixelColor(x, y).lightness() < 220) ++dark;
        check("slider: knob fully drawn at leftmost position", dark > 40);
        QPointF local(1, slider.height() / 2.0);
        QPointF g = slider.mapToGlobal(local.toPoint());
        QMouseEvent press(QEvent::MouseButtonPress, local, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(&slider, &press);
        check("slider: click at far left maps to minimum", slider.value() == 0);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
