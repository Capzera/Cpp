#include "modules/HuffmanTreeModule.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <cstdio>

static QImage shot(QGraphicsScene* scene, const QString& name) {
    QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    scene->render(&p);
    img.save(name);
    return img;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;
    HuffmanTreeModule m;
    m.buildLayout(&scene, "7\n45 13 20 16 9 12 5");
    auto frames = m.runAlgorithm();
    printf("frames=%d\n", (int)frames.size());

    int step = 0;
    for (auto& f : frames) {
        m.renderFrame(f);
        if (f.type == ActionType::HF_SORT_DONE) shot(&scene, "hf_1_sorted.png");
        if (f.type == ActionType::HF_QUEUE_DROP) shot(&scene, "hf_2_queue.png");
        if (f.type == ActionType::HF_MERGE) {
            ++step;
            shot(&scene, QString("hf_3_merge%1.png").arg(step));
        }
        if (f.type == ActionType::HF_FINAL) shot(&scene, "hf_4_final.png");
        if (f.type == ActionType::HF_WPL_DONE) shot(&scene, "hf_5_code.png");
    }
    printf("done\n");
    return 0;
}
