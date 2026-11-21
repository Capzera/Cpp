#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>
#include <QMouseEvent>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/CartesianInfoPanel.h"
#include "ui/widgets/ToolIconButton.h"
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

static void shot(QWidget* w, const QString& name) {
    QImage img(w->size(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    w->render(&p);
    p.end();
    const QString path = QString("D:/Program/Cpp/Project/QT/SmartGround/tests/build/%1.png").arg(name);
    img.save(path);
    printf("saved %s\n", qPrintable(path));
}

static void sendMouse(QWidget* widget, QEvent::Type type, Qt::MouseButton button,
                      const QPoint& pos, Qt::MouseButtons buttons) {
    QMouseEvent event(type, QPointF(pos), QPointF(widget->mapToGlobal(pos)),
                      button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
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
    CartesianInfoPanel* info = w.findChild<CartesianInfoPanel*>();

    if (engine) {
        engine->setStep(1.0);
        engine->addPoint(0.0, 0.0, "O");
        engine->addPoint(3.0, 4.0, "A");
        engine->addPoint(0.5, 0.8, "B");
        engine->linkPoints(engine->pointAt(1), engine->pointAt(2));
        engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
    }
    pump(250);
    if (view && engine) {
        const QPointF tl = view->mapToSceneF(QPointF(0, 0));
        const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                    view->viewport()->height()));
        engine->updateViewport(QRectF(tl, br).normalized());
    }
    pump(250);
    shot(&w, "cartesian_round2_overview");

    printf("--- double click a point opens the info box ---\n");
    if (engine) {
        CartesianPointItem* point = engine->pointAt(1);
        engine->pointDoubleClicked(point->index());
        pump(250);
    }
    shot(&w, "cartesian_round2_info");
    printf("info visible = %d\n", info && info->isVisible() ? 1 : 0);

    printf("--- right drag pans the canvas ---\n");
    if (view && engine) {
        const QPointF before = engine->originScene();
        QWidget* viewport = view->viewport();
        sendMouse(viewport, QEvent::MouseButtonPress, Qt::RightButton, QPoint(500, 350), Qt::RightButton);
        sendMouse(viewport, QEvent::MouseMove, Qt::NoButton, QPoint(600, 420), Qt::RightButton);
        sendMouse(viewport, QEvent::MouseButtonRelease, Qt::RightButton, QPoint(600, 420), Qt::NoButton);
        pump(200);
        printf("origin moved by %f,%f\n", engine->originScene().x() - before.x(),
               engine->originScene().y() - before.y());
    }
    shot(&w, "cartesian_round2_panned");

    printf("--- pan far away to prove the grid is unbounded ---\n");
    if (engine) {
        engine->setOriginScene(engine->originScene() + QPointF(4200.0, 2600.0));
        pump(200);
        if (view) {
            const QPointF tl = view->mapToSceneF(QPointF(0, 0));
            const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                        view->viewport()->height()));
            engine->updateViewport(QRectF(tl, br).normalized());
        }
        pump(250);
    }
    shot(&w, "cartesian_round2_far_pan");

    return 0;
}
