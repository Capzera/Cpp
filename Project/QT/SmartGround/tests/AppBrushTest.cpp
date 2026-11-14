#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include <cmath>
#include "ui/MainWindow.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"
#include "canvas/AnnotationScene.h"
#include "canvas/StrokeItem.h"

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

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 900);
    w.show();
    pump(600);

    CanvasView* view = w.findChild<CanvasView*>();
    ToolController* controller = w.findChild<ToolController*>();
    AnnotationScene* overlay = w.findChild<AnnotationScene*>();
    check("app: canvas view found", view != nullptr);
    check("app: tool controller found", controller != nullptr);
    check("app: annotation scene found", overlay != nullptr);
    if (!view || !controller || !overlay) return 1;

    check("app: annotation scene is hosted by the canvas view",
          overlay->hostView() == view);

    controller->setTool(ToolController::Pen);
    controller->setPenColor(QColor(229, 72, 77));
    controller->setPenWidth(6.0);

    auto send = [&](QEvent::Type type, const QPointF& scenePos) {
        const QPointF localF = view->mapFromScene(scenePos);
        const QPointF globalF = view->mapToGlobal(localF.toPoint());
        QMouseEvent ev(type, localF, globalF, Qt::LeftButton,
                       type == QEvent::MouseButtonRelease ? Qt::MouseButtons() : Qt::LeftButton,
                       Qt::NoModifier);
        QApplication::sendEvent(view->viewport(), &ev);
    };

    const QPointF origin = view->mapToScene(QPoint(120, 220));
    send(QEvent::MouseButtonPress, origin);
    for (int i = 1; i <= 60; ++i) {
        const qreal t = i / 60.0;
        send(QEvent::MouseMove, origin + QPointF(t * 520.0, std::sin(t * 6.0) * 55.0));
    }
    send(QEvent::MouseButtonRelease, origin + QPointF(520.0, 0));
    pump(300);
    w.grab().save("app_brush_stroke.png");
    check("app: stroke created in the real window", overlay->annotationItems().size() >= 1);

    controller->setTool(ToolController::Highlighter);
    controller->setPenColor(QColor(240, 190, 40));
    const QPointF hlOrigin = view->mapToScene(QPoint(120, 330));
    send(QEvent::MouseButtonPress, hlOrigin);
    for (int i = 1; i <= 40; ++i) {
        const qreal t = i / 40.0;
        send(QEvent::MouseMove, hlOrigin + QPointF(t * 520.0, 0));
    }
    send(QEvent::MouseButtonRelease, hlOrigin + QPointF(520.0, 0));
    pump(300);
    w.grab().save("app_brush_highlighter.png");

    controller->setTool(ToolController::Pen);
    controller->setPenColor(QColor(59, 125, 221));
    const QPointF lineOrigin = view->mapToScene(QPoint(120, 430));
    send(QEvent::MouseButtonPress, lineOrigin);
    for (int i = 1; i <= 30; ++i) {
        send(QEvent::MouseMove, lineOrigin + QPointF(i * 8.0, std::sin(i * 0.5) * 20.0));
    }
    send(QEvent::MouseButtonRelease, lineOrigin + QPointF(240.0, 0));
    pump(300);

    controller->setTool(ToolController::Eraser);
    controller->setEraserRadius(18.0);
    const QPointF eraseAt = lineOrigin + QPointF(120.0, 0);
    send(QEvent::MouseButtonPress, eraseAt);
    pump(200);
    w.grab().save("app_brush_eraser.png");

    const int afterErase = overlay->annotationItems().size();
    printf("items after erase = %d\n", afterErase);
    check("app: eraser removed the stroke", afterErase < 3);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
