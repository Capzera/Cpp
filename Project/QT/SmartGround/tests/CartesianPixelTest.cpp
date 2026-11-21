#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static QImage grab(QGraphicsScene& scene, const QRectF& rect) {
    QImage img(rect.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, rect.width(), rect.height()), rect);
    p.end();
    return img;
}

static bool isDark(const QImage& img, int x, int y) {
    if (x < 0 || y < 0 || x >= img.width() || y >= img.height()) return false;
    const QColor c = img.pixelColor(x, y);
    return c.red() < 200 && c.green() < 200 && c.blue() < 200;
}

static bool isBlueish(const QImage& img, int x, int y) {
    if (x < 0 || y < 0 || x >= img.width() || y >= img.height()) return false;
    const QColor c = img.pixelColor(x, y);
    return c.blue() > 150 && c.red() < 160;
}

static int countDark(const QImage& img, int threshold = 200) {
    int count = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < threshold || c.green() < threshold || c.blue() < threshold) ++count;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    printf("--- grid geometry probe ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);

        const QImage img = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        check(img.width() == 800 && img.height() == 600, "probe image is 800x600");

        const int originX = 400;
        const int originY = 300;
        check(isDark(img, originX, originY), "origin pixel belongs to the axis");
        check(isDark(img, originX + 60, originY), "x axis runs horizontally at y = 0");
        check(isDark(img, originX, originY - 60), "y axis runs vertically at x = 0");
        check(!isDark(img, originX + 30, originY + 30), "cell interior is not painted");
    }

    printf("--- ruler ladder density probe ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        check(engine->step() > 0.99 && engine->step() < 1.01, "scale 60 gives ruler 1.0");

        engine->setScale(600.0);
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        check(engine->step() < 0.11 && engine->step() > 0.09, "scale 600 gives ruler 0.1");

        engine->setScale(6000.0);
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        check(engine->step() <= 0.01 + 1e-9, "scale 6000 gives the finest ruler 0.01");

        engine->setScale(6.0);
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        check(engine->step() >= 1.0, "scale 6 coarsens the ruler back to 1.0 or wider");
    }

    printf("--- point and label probe ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        engine->addPoint(2.0, 3.0, QString("A"));
        pump(60);

        const QRectF grabRect(-400.0, -300.0, 800.0, 600.0);
        const QImage img = grab(scene, grabRect);
        const QPointF scenePos = engine->toScene(2.0, 3.0);
        const int px = static_cast<int>(scenePos.x() - grabRect.left());
        const int py = static_cast<int>(scenePos.y() - grabRect.top());
        check(isBlueish(img, px, py), "point marker is painted at its math coordinate");

        bool labelFound = false;
        for (int y = py - 28; y < py - 1 && !labelFound; ++y) {
            for (int x = px + 6; x < px + 60 && !labelFound; ++x) {
                if (isDark(img, x, y)) labelFound = true;
            }
        }
        check(labelFound, "point name label is drawn beside the marker");
    }

    printf("--- segment probe ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        engine->addPoint(-4.0, -2.0, QString("A"));
        engine->addPoint(4.0, 3.0, QString("B"));
        engine->linkPoints(engine->pointAt(0), engine->pointAt(1));
        pump(60);

        const QImage img = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        const QPointF mid = (engine->toScene(-4.0, -2.0) + engine->toScene(4.0, 3.0)) / 2.0;
        const int mx = static_cast<int>(mid.x() + 400.0);
        const int my = static_cast<int>(300.0 - mid.y());
        bool onSegment = false;
        for (int dy = -3; dy <= 3 && !onSegment; ++dy) {
            for (int dx = -3; dx <= 3 && !onSegment; ++dx) {
                if (isDark(img, mx + dx, my + dy)) onSegment = true;
            }
        }
        check(onSegment, "segment is painted through its midpoint");
    }

    printf("--- curve probe ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
        pump(60);

        const QImage img = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        int redPixels = 0;
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                const QColor c = img.pixelColor(x, y);
                if (c.red() > 150 && c.green() < 120 && c.blue() < 120) ++redPixels;
            }
        }
        printf("  red curve pixels = %d\n", redPixels);
        check(redPixels > 200, "quadratic curve is painted in its own color");

        engine->addFunction("g", "2x + 7", QColor(52, 120, 214));
        pump(60);
        const QImage both = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        int bluePixels = 0;
        for (int y = 0; y < both.height(); ++y) {
            for (int x = 0; x < both.width(); ++x) {
                const QColor c = both.pixelColor(x, y);
                if (c.blue() > 140 && c.red() < 120) ++bluePixels;
            }
        }
        printf("  blue curve pixels = %d\n", bluePixels);
        check(bluePixels > 100, "second curve renders simultaneously");
    }

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
