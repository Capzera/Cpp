#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QMap>
#include <algorithm>
#include <cstdio>
#include "modules/PrefixSumModule.h"
#include "modules/BubbleSortModule.h"
#include "modules/SegmentTreeBuildModule.h"
#include "render/GraphElements.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static QImage shot(QGraphicsScene* scene) {
    QRectF sr = scene->itemsBoundingRect();
    QImage img(sr.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p, QRectF(QPointF(0, 0), sr.size()), sr);
    return img;
}

struct Column {
    int x = 0;
    int width = 0;
    int height = 0;
};

static std::vector<Column> darkColumns(const QImage& img, int y0, int y1, int x0, int x1) {
    std::vector<Column> cols;
    int start = -1;
    for (int x = x0; x <= x1; ++x) {
        int height = 0;
        for (int y = y0; y <= y1; ++y) {
            if (x < img.width() && y < img.height() && img.pixelColor(x, y).lightness() < 160) height++;
        }
        if (height > 0 && start < 0) start = x;
        if (height == 0 && start >= 0) {
            Column c;
            c.x = start;
            c.width = x - start;
            for (int y = y0; y <= y1; ++y)
                if (img.pixelColor(start, y).lightness() < 160) c.height++;
            cols.push_back(c);
            start = -1;
        }
    }
    return cols;
}

static void reportScene(const char* label, QGraphicsScene* scene,
                        const QVector<ArrayBlock*>& row, double sceneTop, double sceneBottom) {
    QImage img = shot(scene);
    QRectF sr = scene->itemsBoundingRect();
    double minX = 1e9, maxX = -1e9;
    for (ArrayBlock* b : row) {
        minX = qMin(minX, b->pos().x());
        maxX = qMax(maxX, b->pos().x() + b->cellWidth());
    }
    int y0 = int(sceneTop - sr.y());
    int y1 = int(sceneBottom - sr.y());
    int x0 = int(minX - sr.x());
    int x1 = int(maxX - sr.x());
    std::vector<Column> cols = darkColumns(img, y0, y1, x0, x1);
    int wide = 0;
    int dividers = 0;
    std::vector<double> dividerXs;
    for (const Column& c : cols) {
        if (c.height < 25) continue;
        dividers++;
        dividerXs.push_back(c.x);
        printf("%s divider: x=%d w=%d h=%d\n", label, c.x, c.width, c.height);
        if (c.width > 3) wide++;
    }
    printf("%s: %d dividers, %d over-wide\n", label, dividers, wide);
    check(wide == 0, "no thick double border band between cells");
    check(dividers >= row.size(), "every cell boundary is drawn");
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        QGraphicsScene scene;
        PrefixSumModule mod;
        mod.buildLayout(&scene, "4 1\n1 2 3 4\n1 4");
        QApplication::processEvents();
        QVector<ArrayBlock*> row;
        for (QGraphicsItem* item : scene.items()) {
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (b && qAbs(b->pos().y() - 200.0) < 1.0) row.push_back(b);
        }
        reportScene("prefix sum row a", &scene, row, 205.0, 235.0);
    }

    {
        QGraphicsScene scene;
        BubbleSortModule mod;
        mod.buildLayout(&scene, "5 3 8 1 9 2");
        QApplication::processEvents();
        QVector<ArrayBlock*> row;
        for (QGraphicsItem* item : scene.items()) {
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (b) row.push_back(b);
        }
        reportScene("bubble sort row", &scene, row, 305.0, 335.0);
    }

    {
        QGraphicsScene scene;
        SegmentTreeBuildModule mod;
        mod.buildLayout(&scene, "6 100\n1 2 3 4 5 6");
        QApplication::processEvents();
        QVector<ArrayBlock*> row;
        for (QGraphicsItem* item : scene.items()) {
            ArrayBlock* b = qgraphicsitem_cast<ArrayBlock*>(item);
            if (b && qAbs(b->pos().y() - 120.0) < 1.0) row.push_back(b);
        }
        reportScene("segment tree row a", &scene, row, 125.0, 155.0);
    }

    {
        QGraphicsScene scene;
        BubbleSortModule mod;
        mod.buildLayout(&scene, "5 3 8 1 9 2");
        QApplication::processEvents();
        QMap<double, QVector<double>> rows;
        for (QGraphicsItem* item : scene.items()) {
            ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!blk) continue;
            rows[blk->pos().y()].append(blk->pos().x());
        }
        int badPitch = 0;
        int checkedPairs = 0;
        for (auto it = rows.begin(); it != rows.end(); ++it) {
            QVector<double> xs = it.value();
            std::sort(xs.begin(), xs.end());
            for (int i = 0; i + 1 < xs.size(); ++i) {
                double pitch = xs[i + 1] - xs[i];
                checkedPairs++;
                if (pitch < 59.5 || pitch > 60.5) badPitch++;
            }
        }
        printf("checked %d adjacent pairs, bad pitch: %d\n", checkedPairs, badPitch);
        check(checkedPairs >= 4, "pitch check actually inspected adjacent pairs");
        check(badPitch == 0, "adjacent cells are exactly pitch-wide (no gap, no overlap)");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
