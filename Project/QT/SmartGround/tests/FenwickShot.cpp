#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <cstdio>
#include "modules/FenwickTreeModule.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    FenwickTreeModule m;
    m.buildLayout(&scene, "8\n1 2 3 4 5 6 7 8");
    QRectF bounds = scene.itemsBoundingRect().adjusted(-30, -30, 30, 30);
    QImage img(bounds.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    scene.render(&p, QRectF(img.rect()), bounds);
    img.save("fenwick_lattice.png");
    printf("saved %dx%d\n", img.width(), img.height());
    return 0;
}
