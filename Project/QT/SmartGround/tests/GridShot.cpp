#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/UiTheme.h"
#include "render/SceneGrid.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 900);
    w.show();
    app.processEvents();

    QGraphicsScene* scene = w.findChild<QGraphicsScene*>();
    if (!scene) return 1;
    QGraphicsView* view = scene->views().isEmpty() ? nullptr : scene->views().first();
    if (!view) return 1;

    view->viewport()->grab().toImage().save("grid_canvas.png");
    printf("saved grid_canvas.png\n");
    return 0;
}
