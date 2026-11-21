#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QBuffer>
#include <QByteArray>
#include <cstdio>
#include "modules/EulerSieveModule.h"
#include "modules/MergeSortModule.h"
#include "modules/DSUPathCompressionModule.h"
#include "modules/Knapsack01Module.h"
#include "render/GraphElements.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static QImage grab(QGraphicsScene& scene, QRectF rect) {
    QImage img(rect.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, rect.width(), rect.height()), rect);
    p.end();
    return img;
}

static void save(const QImage& img, const QString& path) {
    if (img.save(path, "PNG")) {
        printf("  wrote %s\n", qPrintable(path));
    } else {
        printf("  FAILED to write %s\n", qPrintable(path));
    }
}

static const QString kOutDir = "D:/Program/Cpp/Project/QT/SmartGround/tests/build";

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    {
        printf("--- euler sieve visual ---\n");
        QGraphicsScene scene;
        EulerSieveModule mod;
        mod.buildLayout(&scene, "60");
        auto frames = mod.runAlgorithm();
        int target = static_cast<int>(frames.size()) * 2 / 3;
        for (int i = 0; i < target; ++i) mod.renderFrame(frames[i]);
        mod.finalizeFrame();
        pump(1200);
        QRectF bounds = scene.itemsBoundingRect();
        save(grab(scene, bounds), kOutDir + "/euler_sieve_mid.png");
        for (size_t i = 0; i < frames.size(); ++i) mod.renderFrame(frames[i]);
        mod.finalizeFrame();
        pump(600);
        save(grab(scene, bounds), kOutDir + "/euler_sieve_done.png");
    }

    {
        printf("--- merge sort visual ---\n");
        QGraphicsScene scene;
        MergeSortModule mod;
        mod.buildLayout(&scene, mod.getDefaultInput());
        auto frames = mod.runAlgorithm();
        for (size_t i = 0; i < frames.size(); ++i) {
            mod.renderFrame(frames[i]);
            mod.finalizeFrame();
            pump(30);
        }
        pump(800);
        QRectF bounds = scene.itemsBoundingRect();
        save(grab(scene, bounds), kOutDir + "/merge_sort_done.png");
    }

    {
        printf("--- dsu visual ---\n");
        QGraphicsScene scene;
        DSUPathCompressionModule mod;
        mod.buildLayout(&scene, mod.getDefaultInput());
        auto frames = mod.runAlgorithm();
        for (size_t i = 0; i < frames.size(); ++i) {
            mod.renderFrame(frames[i]);
            mod.finalizeFrame();
            pump(20);
        }
        pump(800);
        QRectF bounds = scene.itemsBoundingRect();
        save(grab(scene, bounds), kOutDir + "/dsu_compress.png");
    }

    {
        printf("--- knapsack visual ---\n");
        QGraphicsScene scene;
        Knapsack01Module mod;
        mod.buildLayout(&scene, mod.getDefaultInput());
        auto frames = mod.runAlgorithm();
        for (size_t i = 0; i < frames.size(); ++i) {
            mod.renderFrame(frames[i]);
            mod.finalizeFrame();
            pump(20);
        }
        pump(800);
        QRectF bounds = scene.itemsBoundingRect();
        save(grab(scene, bounds), kOutDir + "/knapsack_01.png");
    }

    printf("done\n");
    return 0;
}
