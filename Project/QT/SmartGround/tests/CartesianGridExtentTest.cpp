#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CartesianPanel.h"
#include "render/CartesianElements.h"
#include "canvas/CanvasView.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static int findCartesianIndex(MyComboBox* combo) {
    for (int i = 0; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("笛卡尔坐标系")) return i;
    }
    return -1;
}

static int countNonBackground(const QImage& img, const QRect& area) {
    int count = 0;
    for (int y = area.top(); y < area.bottom(); ++y) {
        for (int x = area.left(); x < area.right(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 248 || c.green() < 248 || c.blue() < 248) count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 860);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    combo->setCurrentIndex(findCartesianIndex(combo));
    pump(400);

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    CanvasView* view = w.findChild<CanvasView*>();

    printf("--- grid coverage at the origin ---\n");
    auto grabGrid = [&](const char* tag) {
        if (!view || !engine) return;
        const QPointF tl = view->mapToSceneF(QPointF(0, 0));
        const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                    view->viewport()->height()));
        engine->updateViewport(QRectF(tl, br).normalized());
        pump(200);
        QImage img(view->viewport()->size(), QImage::Format_ARGB32);
        img.fill(Qt::white);
        QPainter p(&img);
        view->render(&p);
        p.end();
        const int painted = countNonBackground(img, QRect(0, 0, img.width(), img.height()));
        printf("%s: painted pixels = %d (viewport %dx%d)\n", tag, painted, img.width(), img.height());
    };
    grabGrid("origin");

    printf("--- pan far away, the grid must follow ---\n");
    if (engine) {
        engine->setOriginScene(engine->originScene() + QPointF(6000.0, 4000.0));
        pump(200);
        grabGrid("far +6000,+4000");
    }

    printf("--- pan extremely far ---\n");
    if (engine) {
        engine->setOriginScene(engine->originScene() + QPointF(120000.0, -90000.0));
        pump(200);
        grabGrid("far +126000,-86000");
    }

    return 0;
}
