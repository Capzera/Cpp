#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"
#include "canvas/AnnotationItem.h"
#include "canvas/StrokeItem.h"
#include "canvas/ShapeItem.h"
#include "canvas/TextBoxItem.h"
#include "canvas/MarkdownItem.h"
#include "canvas/CanvasTableItem.h"
#include "canvas/LatexItem.h"

static int g_fail = 0;
static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void sendMouse(CanvasView* view, QEvent::Type type, const QPointF& scenePos,
                      Qt::MouseButton button, Qt::KeyboardModifiers mods) {
    const QPointF localF = view->mapFromScene(scenePos);
    const QPointF globalF = view->mapToGlobal(localF.toPoint());
    QMouseEvent ev(type, localF, globalF, button,
                   type == QEvent::MouseButtonRelease ? Qt::MouseButtons() : Qt::MouseButtons(button), mods);
    QApplication::sendEvent(view->viewport(), &ev);
}

static void drag(CanvasView* view, const QPointF& from, const QPointF& to,
                 Qt::KeyboardModifiers mods = Qt::NoModifier) {
    sendMouse(view, QEvent::MouseButtonPress, from, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseMove, (from + to) / 2.0, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseMove, to, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseButtonRelease, to, Qt::LeftButton, mods);
}

static int nonWhiteIn(const QImage& img, const QRect& region) {
    int n = 0;
    for (int y = region.top(); y < region.bottom(); ++y) {
        for (int x = region.left(); x < region.right(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 235 || c.green() < 235 || c.blue() < 235) ++n;
        }
    }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene base;
    base.setSceneRect(-500, -400, 1000, 800);
    base.setBackgroundBrush(Qt::white);

    AnnotationScene overlay;
    CanvasView view(&base);
    view.setAnnotationScene(&overlay);
    ToolController controller(&overlay, &view);
    view.setToolController(&controller);
    view.resize(1000, 800);
    view.show();
    pump(200);

    controller.setPenColor(QColor(200, 40, 40));
    controller.setPenWidth(4.0);

    controller.setTool(ToolController::Pen);
    drag(&view, QPointF(-420, -300), QPointF(-280, -240));
    check("pen: stroke drawn", overlay.annotationItems().size() == 1);

    controller.setTool(ToolController::Rect);
    drag(&view, QPointF(-420, -180), QPointF(-300, -100));

    controller.setTool(ToolController::Circle);
    drag(&view, QPointF(-250, -180), QPointF(-170, -100));

    controller.setTool(ToolController::Arrow);
    drag(&view, QPointF(-120, -300), QPointF(0, -240));

    controller.setTool(ToolController::Triangle);
    drag(&view, QPointF(40, -300), QPointF(140, -230));

    controller.setTool(ToolController::Mosaic);
    drag(&view, QPointF(160, -300), QPointF(240, -240));

    controller.setTool(ToolController::TextBox);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-420, -40), Qt::LeftButton, Qt::NoModifier);

    controller.setTool(ToolController::Markdown);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-140, -40), Qt::LeftButton, Qt::NoModifier);

    controller.setTool(ToolController::Table);
    controller.setTableSize(3, 3);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(120, -40), Qt::LeftButton, Qt::NoModifier);

    controller.setTool(ToolController::Latex);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-120, 220), Qt::LeftButton, Qt::NoModifier);

    check("visual: ten annotations created", overlay.annotationItems().size() == 10);

    pump(400);
    QImage img = view.grab().toImage();
    img.save("canvas_visual.png");

    int totalInk = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 235 || c.green() < 235 || c.blue() < 235) ++totalInk;
        }
    }
    check("visual: canvas renders annotation ink", totalInk > 6000);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
