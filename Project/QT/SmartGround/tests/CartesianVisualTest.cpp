#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QBuffer>
#include <QByteArray>
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

static void save(const QImage& img, const QString& path) {
    if (img.save(path, "PNG")) {
        printf("  wrote %s\n", qPrintable(path));
    } else {
        printf("  FAILED to write %s\n", qPrintable(path));
    }
}

static const QString kOutDir = "D:/Program/Cpp/Project/QT/SmartGround/tests/build";

static int countNonWhite(const QImage& img, int threshold = 245) {
    int count = 0;
    for (int y = 0; y < img.height(); y += 2) {
        for (int x = 0; x < img.width(); x += 2) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < threshold || c.green() < threshold || c.blue() < threshold) ++count;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    printf("--- cartesian grid rendering ---\n");
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
        save(img, kOutDir + "/cartesian_grid.png");
        check(countNonWhite(img) > 200, "grid paints visible lines and axis");
        check(!img.isNull(), "grid render produced an image");
    }

    printf("--- cartesian point and label rendering ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        engine->addPoint(2.0, 3.0, QString("A"));
        engine->addPoint(-3.0, 1.0, QString("B"));
        pump(60);

        const QImage img = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        save(img, kOutDir + "/cartesian_points.png");
        check(countNonWhite(img) > 200, "points render on the grid");

        const QPointF anchor = engine->toScene(2.0, 3.0);
        const QRectF labelRect = anchor.x() > 0
                                     ? QRectF(anchor + QPointF(2, -30), QSizeF(60, 24))
                                     : QRectF(anchor + QPointF(-62, -30), QSizeF(60, 24));
        const QImage labelImg = grab(scene, labelRect);
        save(labelImg, kOutDir + "/cartesian_point_label.png");
        check(countNonWhite(labelImg) > 3, "point name label is painted");
    }

    printf("--- cartesian segment rendering ---\n");
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
        save(img, kOutDir + "/cartesian_segment.png");
        check(countNonWhite(img) > 200, "segment renders on the grid");

        const QPointF mid = (engine->toScene(-4.0, -2.0) + engine->toScene(4.0, 3.0)) / 2.0;
        const QImage eqImg = grab(scene, QRectF(mid + QPointF(4, -30), QSizeF(150, 26)));
        save(eqImg, kOutDir + "/cartesian_segment_equation.png");
        check(countNonWhite(eqImg) > 3, "segment equation label is painted");
    }

    printf("--- cartesian function curve rendering ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
        engine->addFunction("g", "2x + 7", QColor(52, 120, 214));
        pump(60);

        const QImage img = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));
        save(img, kOutDir + "/cartesian_curves.png");
        check(countNonWhite(img) > 200, "function curves render on the grid");

        const QImage curveOnly = grab(scene, QRectF(0.0, 0.0, 400.0, 300.0));
        save(curveOnly, kOutDir + "/cartesian_curve_zoom.png");
    }

    printf("--- cartesian ruler ladder rendering ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        const QImage coarse = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));

        engine->setScale(600.0);
        engine->updateViewport(QRectF(-400.0, -300.0, 800.0, 600.0));
        pump(60);
        const QImage fine = grab(scene, QRectF(-400.0, -300.0, 800.0, 600.0));

        save(coarse, kOutDir + "/cartesian_ruler_1.png");
        save(fine, kOutDir + "/cartesian_ruler_2.png");
        check(countNonWhite(fine) > countNonWhite(coarse),
              "zooming in produces a denser grid");
    }

    printf("--- cartesian pan hides and restores curves ---\n");
    {
        QGraphicsScene scene;
        CartesianModule mod;
        mod.buildLayout(&scene, QString());
        CartesianEngine* engine = mod.engine();
        engine->setScale(60.0);
        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->addFunction("f", "2x + 7", QColor(214, 96, 77));

        engine->updateViewport(QRectF(-20.0, -15.0, 40.0, 30.0));
        pump(30);
        const QImage near = grab(scene, QRectF(-20.0, -15.0, 40.0, 30.0));

        engine->setOriginScene(QPointF(5000.0, 5000.0));
        engine->updateViewport(QRectF(4900.0, 4900.0, 40.0, 30.0));
        pump(30);
        const QImage far = grab(scene, QRectF(4900.0, 4900.0, 40.0, 30.0));

        engine->setOriginScene(QPointF(0.0, 0.0));
        engine->updateViewport(QRectF(-20.0, -15.0, 40.0, 30.0));
        pump(30);
        const QImage back = grab(scene, QRectF(-20.0, -15.0, 40.0, 30.0));

        check(countNonWhite(near) > 0, "curve visible when viewport covers it");
        check(countNonWhite(far) < countNonWhite(near), "curve disappears when panned away");
        check(countNonWhite(back) >= countNonWhite(near) - 5, "curve returns when panned back");
    }

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
