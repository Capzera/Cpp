#include <QApplication>
#include <QTimer>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointF>
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

static void sendMouse(QWidget* widget, QEvent::Type type, Qt::MouseButton button,
                      const QPoint& pos, Qt::MouseButtons buttons) {
    QMouseEvent event(type, QPointF(pos), QPointF(widget->mapToGlobal(pos)),
                      button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    MainWindow w;
    w.resize(1280, 820);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    if (!combo) { printf("combo missing\n"); return 1; }
    const int index = findCartesianIndex(combo);
    if (index < 0) { printf("cartesian module missing\n"); return 1; }
    combo->setCurrentIndex(index);
    pump(300);

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    CanvasView* view = w.findChild<CanvasView*>();
    CanvasToolBar* toolbar = w.findChild<CanvasToolBar*>();

    printf("=== issue 1: left object list updates after drawing ===\n");
    check(engine != nullptr, "cartesian engine reachable");
    check(panel && panel->engine() == engine, "panel tracks the engine");
    const int rowsBefore = rowCount(panel);
    if (engine) {
        engine->addPoint(1.0, 2.0, QString());
        pump(120);
    }
    check(rowCount(panel) == rowsBefore + 1,
          QString("list gains one row after addPoint (%1 -> %2)")
              .arg(rowsBefore).arg(rowCount(panel)));

    printf("=== issue 3: proximity snap, free coordinates stay free ===\n");
    if (engine) {
        engine->setStep(1.0);
        const QPointF freeMath(0.5, 0.8);
        check(!engine->isSnapped(freeMath), "0.5, 0.8 is not considered snapped");
        const QPointF snapped = engine->snappedMath(freeMath);
        check(qAbs(snapped.x() - 0.5) < 1e-9 && qAbs(snapped.y() - 0.8) < 1e-9,
              "0.5, 0.8 keeps its free position");
        const QPointF nearIntersection(0.02, 0.03);
        check(engine->isSnapped(nearIntersection), "a point next to the origin counts as snapped");
        const QPointF snappedNear = engine->snappedMath(nearIntersection);
        check(qAbs(snappedNear.x()) < 1e-9 && qAbs(snappedNear.y()) < 1e-9,
              "a point next to the origin snaps to (0, 0)");

        engine->addPoint(0.5, 0.8, QString());
        pump(80);
        CartesianPointItem* freePoint = engine->pointAt(engine->pointCount() - 1);
        check(freePoint && qAbs(freePoint->x() - 0.5) < 1e-6 && qAbs(freePoint->y() - 0.8) < 1e-6,
              "a free point is stored at exactly (0.5, 0.8)");
    }

    printf("=== issue 2: segment equation hidden until inspected ===\n");
    if (engine && engine->pointCount() >= 2) {
        CartesianPointItem* a = engine->pointAt(0);
        CartesianPointItem* b = engine->pointAt(1);
        const int segIndex = engine->linkPoints(a, b);
        pump(80);
        check(segIndex >= 0, "a segment was created");
        CartesianSegmentItem* segment = engine->segments().isEmpty() ? nullptr
                                                                    : engine->segments().last();
        check(segment && !segment->equationVisible(),
              "the segment does not paint its equation by default");
        check(segment && !segment->equationText().isEmpty(),
              "the segment still exposes the equation text for the inspector");
    }

    printf("=== issue 4: the icon and the caption share one hit area ===\n");
    if (toolbar) {
        ToolIconButton* pointButton =
            toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartPoint));
        ToolIconButton* lineButton =
            toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartLine));
        check(pointButton && pointButton->hasCaption(), "the point button carries a caption");
        check(lineButton && lineButton->hasCaption(), "the line button carries a caption");
        check(pointButton && pointButton->width() > 40,
              "the point button is wider than the bare icon");
        check(pointButton && pointButton->caption().contains("画点"),
              "the point button caption reads 画点");
    }

    printf("=== issue 5: right drag pans the canvas ===\n");
    if (engine && view) {
        const QPointF originBefore = engine->originScene();
        QWidget* viewport = view->viewport();
        const QPoint start(400, 300);
        sendMouse(viewport, QEvent::MouseButtonPress, Qt::RightButton, start, Qt::RightButton);
        sendMouse(viewport, QEvent::MouseMove, Qt::NoButton, QPoint(460, 340),
                  Qt::RightButton);
        sendMouse(viewport, QEvent::MouseButtonRelease, Qt::RightButton, QPoint(460, 340),
                  Qt::NoButton);
        pump(80);
        const QPointF originAfter = engine->originScene();
        check(qAbs(originBefore.x() - originAfter.x()) > 1.0 ||
                  qAbs(originBefore.y() - originAfter.y()) > 1.0,
              "right dragging moved the coordinate origin");
    }

    printf("=== issue 6: the coordinate system is not bounded ===\n");
    {
        QGraphicsScene* scene = view ? view->scene() : nullptr;
        check(scene && ViewportFitter::infiniteCanvas(scene),
              "the infinite canvas flag is on inside the cartesian module");
        check(scene && scene->sceneRect().width() > 100000.0,
              "the scene rect covers a practically infinite area");
    }

    printf("=== issue 7: double click opens the right side info box ===\n");
    CartesianInfoPanel* info = w.findChild<CartesianInfoPanel*>();
    check(info != nullptr, "the info panel exists");
    if (info) {
        check(!info->isVisible(), "the info panel starts hidden");
        engine->pointDoubleClicked(0);
        pump(120);
        check(info->isVisible(), "the info panel becomes visible after an inspection");
        check(info->width() > 100 && info->height() > 60, "the info panel has a real size");
        check(info->parentWidget() == view, "the info panel is anchored over the canvas");
        check(info->infoTitle().contains("A"), "the info panel names the inspected point");
        check(info->infoBody().contains("x =") && info->infoBody().contains("y ="),
              "the info panel lists the coordinates");
        if (view) {
            const QRect area = info->geometry();
            const QRect canvas = view->viewport()->rect();
            check(area.right() <= canvas.right() + 1 && area.left() >= canvas.left() - 1,
                  "the info panel sits inside the canvas bounds");
            check(area.left() > canvas.left() + canvas.width() / 2,
                  "the info panel is anchored on the right half of the canvas");
        }
    }

    printf("=== issue 2b: double clicking a segment shows its equation ===\n");
    if (engine && !engine->segments().isEmpty()) {
        CartesianSegmentItem* segment = engine->segments().first();
        const QString equation = segment->equationText();
        check(!equation.isEmpty(), "the segment has an equation");
        engine->segmentDoubleClicked(segment->index());
        pump(120);
        check(info && info->isVisible(), "the info panel shows for a segment");
        check(info && info->infoBody().contains(equation),
              "the info panel body carries the segment equation");
    }

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
