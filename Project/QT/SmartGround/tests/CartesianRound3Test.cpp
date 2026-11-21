#include <QApplication>
#include <QTimer>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <cstdio>
#include <cmath>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/CartesianInfoPanel.h"
#include "ui/widgets/ToolIconButton.h"
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"
#include "core/ViewportFitter.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

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

static int rowCount(CartesianPanel* panel) {
    if (!panel) return 0;
    int count = 0;
    for (QWidget* widget : panel->findChildren<QWidget*>()) {
        if (widget->objectName() == "cartesianObjectRow") count++;
    }
    return count;
}

static int visibleRowCount(CartesianPanel* panel) {
    if (!panel) return 0;
    int count = 0;
    for (QWidget* widget : panel->findChildren<QWidget*>()) {
        if (widget->objectName() != "cartesianObjectRow") continue;
        if (widget->isVisible() && !widget->isHidden()) count++;
    }
    return count;
}

static void sendMouse(QWidget* widget, QEvent::Type type, Qt::MouseButton button,
                      const QPoint& pos, Qt::MouseButtons buttons) {
    QMouseEvent event(type, QPointF(pos), QPointF(widget->mapToGlobal(pos)),
                      button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

static void clickAt(CanvasView* view, const QPoint& viewportPos) {
    QWidget* vp = view->viewport();
    sendMouse(vp, QEvent::MouseButtonPress, Qt::LeftButton, viewportPos, Qt::LeftButton);
    sendMouse(vp, QEvent::MouseButtonRelease, Qt::LeftButton, viewportPos, Qt::NoButton);
}

static void doubleClickAt(CanvasView* view, const QPoint& viewportPos) {
    QWidget* vp = view->viewport();
    sendMouse(vp, QEvent::MouseButtonPress, Qt::LeftButton, viewportPos, Qt::LeftButton);
    sendMouse(vp, QEvent::MouseButtonRelease, Qt::LeftButton, viewportPos, Qt::NoButton);
    sendMouse(vp, QEvent::MouseButtonDblClick, Qt::LeftButton, viewportPos, Qt::LeftButton);
}

static QPoint mathToViewport(CanvasView* view, CartesianEngine* engine,
                             double mx, double my) {
    const QPointF scene = engine->toScene(mx, my);
    return view->mapFromScene(scene);
}

static void clickButtonAt(ToolIconButton* button, const QPoint& localPos) {
    const QPoint global = button->mapToGlobal(localPos);
    QMouseEvent press(QEvent::MouseButtonPress, QPointF(localPos), QPointF(global),
                      Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(button, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, QPointF(localPos), QPointF(global),
                        Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(button, &release);
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    MainWindow w;
    w.resize(1400, 860);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    if (!combo) { printf("combo missing\n"); return 1; }
    const int index = findCartesianIndex(combo);
    if (index < 0) { printf("cartesian module missing\n"); return 1; }
    combo->setCurrentIndex(index);
    pump(400);

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    CanvasView* view = w.findChild<CanvasView*>();
    CanvasToolBar* toolbar = w.findChild<CanvasToolBar*>();
    ToolController* tc = w.findChild<ToolController*>();
    CartesianInfoPanel* info = w.findChild<CartesianInfoPanel*>();

    if (!engine || !view) { printf("engine or view missing\n"); return 1; }

    printf("=== issue 1 (REAL interaction): clicking the canvas adds a point AND a list row ===\n");
    {
        const int rowsBefore = rowCount(panel);
        const int pointsBefore = engine->pointCount();

        const QPoint target = mathToViewport(view, engine, 3.0, 2.0);
        clickAt(view, target);
        pump(200);

        printf("  points %d -> %d, rows %d -> %d\n", pointsBefore, engine->pointCount(),
               rowsBefore, rowCount(panel));
        check(engine->pointCount() == pointsBefore + 1,
              "a real canvas click created a point");
        check(rowCount(panel) == rowsBefore + 1,
              "a real canvas click added a row to the left object list");
        check(visibleRowCount(panel) >= 1,
              "at least one object row is actually visible in the panel");
    }

    printf("=== issue 3 (REAL interaction): a click away from an intersection keeps free coordinates ===\n");
    {
        const QPoint target = mathToViewport(view, engine, 0.5, 0.8);
        clickAt(view, target);
        pump(200);
        CartesianPointItem* point = engine->pointAt(engine->pointCount() - 1);
        check(point && qAbs(point->x() - 0.5) < 1e-6 && qAbs(point->y() - 0.8) < 1e-6,
              QString("the free click landed on (0.5, 0.8), got (%1, %2)")
                  .arg(point ? point->x() : 0.0).arg(point ? point->y() : 0.0));
        check(point && !engine->isSnapped(QPointF(point->x(), point->y())),
              "the free point is reported as not snapped");
    }

    printf("=== issue 3b (REAL interaction): a click right on an intersection snaps ===\n");
    {
        const QPoint target = mathToViewport(view, engine, 5.0, 4.0);
        clickAt(view, target);
        pump(200);
        CartesianPointItem* point = engine->pointAt(engine->pointCount() - 1);
        check(point && qAbs(point->x() - 5.0) < 1e-6 && qAbs(point->y() - 4.0) < 1e-6,
              "the click on the intersection landed exactly on (5, 4)");
        check(point && engine->isSnapped(QPointF(point->x(), point->y())),
              "the point on the intersection is reported as snapped");
    }

    printf("=== issue 4 (REAL interaction): clicking the caption text selects the tool ===\n");
    {
        ToolIconButton* lineButton =
            toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartLine));
        check(lineButton != nullptr, "the line button exists");
        if (lineButton) {
            const QPoint captionPos(lineButton->width() - 6, lineButton->height() / 2);
            printf("  button size = %dx%d, caption click at %d,%d\n",
                   lineButton->width(), lineButton->height(),
                   captionPos.x(), captionPos.y());
            clickButtonAt(lineButton, captionPos);
            pump(200);
            check(tc && tc->currentTool() == ToolController::CartLine,
                  "clicking the caption text switched to the line tool");
            check(lineButton->isActive(), "the line button shows the active state");
        }
    }

    printf("=== issue 5 (REAL interaction): right dragging pans the canvas ===\n");
    {
        const QPointF originBefore = engine->originScene();
        QWidget* vp = view->viewport();
        sendMouse(vp, QEvent::MouseButtonPress, Qt::RightButton, QPoint(400, 300),
                  Qt::RightButton);
        sendMouse(vp, QEvent::MouseMove, Qt::NoButton, QPoint(470, 350), Qt::RightButton);
        sendMouse(vp, QEvent::MouseButtonRelease, Qt::RightButton, QPoint(470, 350),
                  Qt::NoButton);
        pump(200);
        const QPointF originAfter = engine->originScene();
        printf("  origin (%f, %f) -> (%f, %f)\n", originBefore.x(), originBefore.y(),
               originAfter.x(), originAfter.y());
        check(qAbs(originBefore.x() - originAfter.x()) > 1.0 ||
                  qAbs(originBefore.y() - originAfter.y()) > 1.0,
              "right dragging moved the coordinate origin");
    }

    printf("=== issue 2 (REAL interaction): link two points, no equation on the canvas ===\n");
    {
        tc->setTool(ToolController::CartPoint);
        const QPoint a = mathToViewport(view, engine, -3.0, -2.0);
        const QPoint b = mathToViewport(view, engine, 2.0, 3.0);
        clickAt(view, a);
        pump(150);
        clickAt(view, b);
        pump(150);
        tc->setTool(ToolController::CartLine);
        pump(100);
        clickAt(view, a);
        pump(120);
        clickAt(view, b);
        pump(200);

        printf("  points=%d segments=%d\n", engine->pointCount(), engine->segments().size());
        check(engine->segments().size() >= 1, "a segment was created by clicking two points");
        if (!engine->segments().isEmpty()) {
            CartesianSegmentItem* segment = engine->segments().last();
            check(!segment->equationVisible(),
                  "the segment does not paint its equation on the canvas");
            check(!segment->equationText().isEmpty(),
                  "the segment still exposes the equation text");
        }
    }

    printf("=== issue 7 (REAL interaction): double clicking a point opens the right info box ===\n");
    {
        const QPoint target = mathToViewport(view, engine, 3.0, 2.0);
        doubleClickAt(view, target);
        pump(250);
        printf("  info visible = %d\n", info && info->isVisible() ? 1 : 0);
        check(info && info->isVisible(), "the info panel opened after a real double click");
        if (info && info->isVisible()) {
            printf("  title=%s body=%s\n", qPrintable(info->infoTitle()),
                   qPrintable(info->infoBody()));
            check(info->infoBody().contains("x =") && info->infoBody().contains("y ="),
                  "the info panel lists the coordinates");
            const QRect canvas = view->viewport()->rect();
            const QRect area = info->geometry();
            check(area.left() > canvas.left() + canvas.width() / 2,
                  "the info panel is anchored on the right half of the canvas");
            check(area.right() <= canvas.right() + 1,
                  "the info panel stays inside the canvas bounds");
        }
    }

    printf("=== issue 2b (REAL interaction): double clicking a segment shows its equation ===\n");
    {
        if (!engine->segments().isEmpty()) {
            CartesianSegmentItem* segment = engine->segments().last();
            const QString equation = segment->equationText();
            const QPoint mid = view->mapFromScene((segment->start()->pos() + segment->end()->pos())
                                                      / 2.0);
            doubleClickAt(view, mid);
            pump(250);
            check(info && info->isVisible(), "the info panel opened for the segment");
            check(info && info->infoBody().contains(equation),
                  QString("the info panel carries the segment equation '%1'").arg(equation));
        }
    }

    printf("=== issue 6: the coordinate system is not bounded ===\n");
    {
        QGraphicsScene* scene = view->scene();
        check(scene && ViewportFitter::infiniteCanvas(scene),
              "the infinite canvas flag is on inside the cartesian module");
        check(scene && scene->sceneRect().width() > 100000.0,
              "the scene rect covers a practically infinite area");

        auto countGridPixels = [&]() {
            const QPointF tl = view->mapToSceneF(QPointF(0, 0));
            const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                          view->viewport()->height()));
            engine->updateViewport(QRectF(tl, br).normalized());
            pump(80);
            QImage img(view->viewport()->size(), QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            view->render(&p);
            p.end();
            int dark = 0;
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    const QColor c = img.pixelColor(x, y);
                    if (c.red() < 250 && c.green() < 250 && c.blue() < 250) ++dark;
                }
            }
            return dark;
        };

        engine->setOriginScene(QPointF(0.0, 0.0));
        pump(120);
        const int atOrigin = countGridPixels();

        engine->setOriginScene(QPointF(126000.0, -86000.0));
        pump(120);
        const int farAway = countGridPixels();

        printf("  grid pixels at origin = %d, far away = %d\n", atOrigin, farAway);
        check(atOrigin > 1000, "the grid renders around the origin");
        check(farAway > atOrigin / 2,
              "the grid still renders just as densely far away from the origin");
    }

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
