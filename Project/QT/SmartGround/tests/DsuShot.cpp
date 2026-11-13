#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include "modules/DSUBasicModule.h"
#include "modules/GraphRenderHelper.h"
#include "render/GraphElements.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    DSUBasicModule m;
    m.m_animated = false;
    m.buildLayout(&scene, "10\n0 1 1 2 3 4 4 5 6 7");
    scene.setSceneRect(50, 50, 700, 450);
    QImage img(700, 450, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene.render(&p, QRectF(0, 0, 700, 450), QRectF(50, 50, 700, 450));
    p.end();
    img.save("dsu_scene.png");
    return 0;
}
