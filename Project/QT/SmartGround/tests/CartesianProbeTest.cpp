#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"
#include "core/CartesianExpression.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    const CartesianExpression::Result r = CartesianExpression::parse("3x^2 - 8x + 7");
    printf("parse ok=%d c3=%f c2=%f c1=%f c0=%f err=%s\n",
           r.ok, r.c3, r.c2, r.c1, r.c0, r.error.toUtf8().constData());
    for (double x : {-9.27, -3.68, -2.0, 0.0, 1.3333, 4.667, 6.34, 9.27}) {
        printf("  f(%8.4f) = %10.4f\n", x, CartesianExpression::evaluate(r, x));
    }

    QGraphicsScene scene;
    CartesianModule mod;
    mod.buildLayout(&scene, QString());
    CartesianEngine* engine = mod.engine();
    engine->setScale(60.0);
    engine->setOriginScene(QPointF(0.0, 0.0));
    engine->updateViewport(QRectF(-556.0, -409.0, 1112.0, 818.0));
    engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));

    CartesianCurveItem* curve = engine->curveAt(0);
    printf("\nengine origin = %f,%f scale=%f step=%f\n",
           engine->originScene().x(), engine->originScene().y(),
           engine->scale(), engine->step());
    printf("runs=%d points=%d\n", curve->visibleRunCount(), curve->visiblePointCount());
    const QRectF b = curve->visibleBounds();
    printf("scene bounds = %f,%f  ->  %f,%f\n", b.x(), b.y(), b.width(), b.height());
    printf("  math x range = %f .. %f\n", b.x() / 60.0, (b.x() + b.width()) / 60.0);
    printf("  math y range = %f .. %f\n", -b.y() / 60.0, -(b.y() + b.height()) / 60.0);

    QImage img(1112, 818, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, 1112, 818),
                 QRectF(-556.0, -409.0, 1112.0, 818.0));
    p.end();
    int red = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() > 180 && c.green() < 130 && c.blue() < 110) ++red;
        }
    }
    printf("curve pixels in viewport = %d\n", red);
    img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_probe.png");
    return 0;
}
