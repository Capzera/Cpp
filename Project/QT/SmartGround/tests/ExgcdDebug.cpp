#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <cstdio>
#include "modules/ExgcdModule.h"
#include "render/GraphElements.h"

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    QGraphicsScene scene;
    ExgcdModule m;
    m.buildLayout(&scene, "11914 146415");
    auto frames = m.runAlgorithm();
    for (auto& f : frames) m.renderFrame(f);
    for (QGraphicsItem* item : scene.items()) {
        QGraphicsTextItem* t = dynamic_cast<QGraphicsTextItem*>(item);
        if (!t) continue;
        const QString s = t->toPlainText();
        if (!s.startsWith("146415")) continue;
        printf("text='%s' pos=(%.1f,%.1f) bbox=(%.1f,%.1f %.1fx%.1f) centerY=%.1f\n",
               s.toUtf8().constData(), t->pos().x(), t->pos().y(),
               t->boundingRect().x(), t->boundingRect().y(),
               t->boundingRect().width(), t->boundingRect().height(),
               t->boundingRect().center().y());
    }
    return 0;
}
