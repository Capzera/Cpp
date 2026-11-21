#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <cstdio>
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"

static void countPixels(const QImage& img, const char* label) {
    int red = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++red;
        }
    }
    printf("%s redPixels=%d\n", label, red);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene scene;
    CartesianModule mod;
    mod.buildLayout(&scene, QString());
    CartesianEngine* engine = mod.engine();
    engine->setScale(60.0);
    engine->setOriginScene(QPointF(0.0, 0.0));
    engine->updateViewport(QRectF(-556.0, -409.0, 1112.0, 818.0));
    engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));

    CartesianCurveItem* curve = engine->curveAt(0);
    printf("runs=%d pts=%d\n", curve->visibleRunCount(), curve->visiblePointCount());
    const QRectF b = curve->visibleBounds();
    printf("scene bounds = %f,%f %fx%f\n", b.x(), b.y(), b.width(), b.height());
    printf("boundingRect = %f,%f %fx%f\n", curve->boundingRect().x(),
           curve->boundingRect().y(), curve->boundingRect().width(),
           curve->boundingRect().height());

    const QVector<QPointF> pts = curve->samplePoints(5);
    for (const QPointF& p : pts) printf("  pt = %f,%f\n", p.x(), p.y());

    QImage img(1112, 818, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, 1112, 818), QRectF(-556.0, -409.0, 1112.0, 818.0));
    p.end();
    countPixels(img, "scene render:");
    img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/curveisolate.png");

    QImage direct(1112, 818, QImage::Format_ARGB32);
    direct.fill(Qt::white);
    QPainter dp(&direct);
    dp.translate(556.0, 409.0);
    curve->paint(&dp, nullptr, nullptr);
    dp.end();
    countPixels(direct, "direct paint:");

    printf("\n--- what is actually blue ---\n");
    for (int y = 0; y < 818; y += 60) {
        for (int x = 0; x < 1112; x += 120) {
            const QColor c = img.pixelColor(x, y);
            if (c.blue() > 170 && c.red() < 150) {
                printf("  blue at (%d,%d) = %d,%d,%d\n", x, y, c.red(), c.green(), c.blue());
            }
        }
    }
    return 0;
}
