#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/MyTextInput.h"
#include "ui/widgets/MyButton.h"
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

    if (engine) {
        engine->addPoint(-4.0, 1.0, "A");
        engine->addPoint(1.0, 4.0, "B");
        engine->addPoint(3.0, -2.0, "C");
        engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
    }
    pump(200);

    if (view) {
        const QPointF tl = view->mapToSceneF(QPointF(0, 0));
        const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(), view->viewport()->height()));
        engine->updateViewport(QRectF(tl, br).normalized());
    }
    pump(200);

    QImage img(w.size(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    w.render(&p);
    p.end();
    img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_full_ui.png");
    printf("saved full ui screenshot\n");
    return 0;
}
