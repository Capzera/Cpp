#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>
#include <QKeyEvent>
#include <cstdio>
#include <cmath>
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"
#include "canvas/AnnotationItem.h"
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

static void sendMouse(CanvasView* view, QEvent::Type type, const QPointF& scenePos,
                      Qt::MouseButton button, Qt::KeyboardModifiers mods = Qt::NoModifier) {
    const QPointF localF = view->mapFromScene(scenePos);
    const QPointF globalF = view->mapToGlobal(localF.toPoint());
    QMouseEvent ev(type, localF, globalF, button,
                   type == QEvent::MouseButtonRelease ? Qt::MouseButtons() : Qt::MouseButtons(button), mods);
    QApplication::sendEvent(view->viewport(), &ev);
}

static int redPixels(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 235 && c.green() < 150 && c.blue() < 150) ++n;
        }
    }
    return n;
}

static int yellowPixels(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() > 225 && c.green() > 200 && c.blue() > 140 && c.blue() < 215) ++n;
        }
    }
    return n;
}

static StrokeItem* findHighlighter(AnnotationScene& scene) {
    for (QGraphicsItem* item : scene.items()) {
        auto* s = qgraphicsitem_cast<StrokeItem*>(item);
        if (s && s->mode() == StrokeItem::Highlighter) return s;
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene base;
    base.setSceneRect(-800, -800, 1600, 1600);
    base.setBackgroundBrush(Qt::white);

    AnnotationScene overlay;
    CanvasView view(&base);
    view.setAnnotationScene(&overlay);
    ToolController controller(&overlay, &view);
    view.setToolController(&controller);
    view.resize(1000, 800);
    view.show();
    pump(300);

    check("realtime: annotation scene has a host view", overlay.hostView() == &view);

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(200, 40, 40));
    controller.setPenWidth(5.0);

    QVector<QPointF> pts;
    for (int i = 0; i <= 60; ++i) {
        pts.push_back(QPointF(-400.0 + i * 4.0, -300.0 + std::sin(i * 0.25) * 60.0));
    }

    sendMouse(&view, QEvent::MouseButtonPress, pts.first(), Qt::LeftButton);
    for (int i = 1; i <= 12; ++i) {
        sendMouse(&view, QEvent::MouseMove, pts.at(i), Qt::LeftButton);
    }
    pump(150);

    QImage mid = view.grab().toImage();
    const int midInk = redPixels(mid);
    printf("mid-stroke ink pixels = %d\n", midInk);
    check("realtime: partial stroke visible before mouse release", midInk > 80);

    for (int i = 13; i <= 35; ++i) {
        sendMouse(&view, QEvent::MouseMove, pts.at(i), Qt::LeftButton);
    }
    pump(150);
    QImage mid2 = view.grab().toImage();
    const int mid2Ink = redPixels(mid2);
    printf("after more movement ink pixels = %d (was %d)\n", mid2Ink, midInk);
    check("realtime: stroke grows live while dragging", mid2Ink > midInk);

    for (int i = 36; i < pts.size(); ++i) {
        sendMouse(&view, QEvent::MouseMove, pts.at(i), Qt::LeftButton);
    }
    pump(150);
    sendMouse(&view, QEvent::MouseButtonRelease, pts.last(), Qt::LeftButton);
    pump(150);
    QImage done = view.grab().toImage();
    const int doneInk = redPixels(done);
    printf("final ink pixels = %d\n", doneInk);
    check("realtime: completed stroke fully rendered", doneInk > mid2Ink);
    done.save("brush_realtime.png");

    auto* strokeItem = qgraphicsitem_cast<StrokeItem*>(overlay.items().first());
    check("brush: stroke item exists", strokeItem != nullptr);
    if (strokeItem) {
        check("brush: stroke is ink mode", strokeItem->mode() == StrokeItem::Ink);
        check("brush: stroke recorded the sampled path", strokeItem->points().size() >= 40);
    }

    const int before = overlay.items().size();
    controller.setTool(ToolController::Eraser);
    controller.setEraserRadius(20.0);
    pump(80);
    sendMouse(&view, QEvent::MouseButtonPress, pts.at(20), Qt::LeftButton);
    pump(150);
    QImage afterErase = view.grab().toImage();
    const int eraseInk = redPixels(afterErase);
    printf("ink after erase = %d (was %d)\n", eraseInk, doneInk);
    check("realtime: erase is visible without zoom/pan", eraseInk < doneInk);
    check("eraser: removed an item", overlay.items().size() < before);
    check("eraser: undo stack populated", controller.canUndo());

    controller.undo();
    pump(120);
    QImage afterUndo = view.grab().toImage();
    printf("ink after undo = %d\n", redPixels(afterUndo));
    check("undo: restores erased ink live", redPixels(afterUndo) >= doneInk - 20);
    controller.redo();
    pump(120);
    check("redo: removes ink again", redPixels(view.grab().toImage()) < doneInk);

    controller.setTool(ToolController::Highlighter);
    controller.setPenColor(QColor(250, 220, 60));
    controller.setPenWidth(12.0);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-400, 200), Qt::LeftButton);
    for (int i = 1; i <= 20; ++i) {
        sendMouse(&view, QEvent::MouseMove, QPointF(-400 + i * 10.0, 200 + std::sin(i * 0.4) * 30.0), Qt::LeftButton);
    }
    sendMouse(&view, QEvent::MouseButtonRelease, QPointF(-200, 200), Qt::LeftButton);
    pump(200);
    auto* hl = qgraphicsitem_cast<StrokeItem*>(findHighlighter(overlay));
    check("highlighter: creates a highlighter stroke", hl != nullptr);
    if (hl) check("highlighter: mode is Highlighter", hl->mode() == StrokeItem::Highlighter);

    QImage finalShot = view.grab().toImage();
    finalShot.save("brush_final.png");

    const int translucent = yellowPixels(finalShot);
    printf("highlighter tinted pixels = %d\n", translucent);
    check("highlighter: renders translucent yellow ink", translucent > 100);

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(30, 90, 200));
    controller.setPenWidth(5.0);
    QVector<QPointF> jag;
    for (int i = 0; i <= 12; ++i) {
        jag.push_back(QPointF(-400 + i * 16.0, 320));
    }
    sendMouse(&view, QEvent::MouseButtonPress, jag.first(), Qt::LeftButton);
    for (int i = 1; i < jag.size(); ++i) {
        sendMouse(&view, QEvent::MouseMove, jag.at(i), Qt::LeftButton);
    }
    sendMouse(&view, QEvent::MouseButtonRelease, jag.last(), Qt::LeftButton);
    pump(200);

    QImage smoothShot = view.grab().toImage();
    smoothShot.save("brush_smooth.png");

    int rowThickness = 0;
    int maxThickness = 0;
    for (int x = 0; x < smoothShot.width(); ++x) {
        int count = 0;
        for (int y = 0; y < smoothShot.height(); ++y) {
            const QColor c = smoothShot.pixelColor(x, y);
            if (c.red() < 120 && c.green() < 160 && c.blue() > 150) ++count;
        }
        if (count > 0) ++rowThickness;
        maxThickness = qMax(maxThickness, count);
    }
    printf("blue horizontal stroke columns = %d, max vertical thickness = %d\n", rowThickness, maxThickness);
    check("smooth: horizontal stroke spans its full length", rowThickness > 180);
    check("smooth: stroke keeps a constant width", maxThickness > 3 && maxThickness < 12);

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(40, 170, 90));
    const QVector<QPointF> barPts = {
        QPointF(-400, -100), QPointF(-350, -100), QPointF(-300, -100),
        QPointF(-250, -100), QPointF(-200, -100), QPointF(-150, -100)
    };
    sendMouse(&view, QEvent::MouseButtonPress, barPts.first(), Qt::LeftButton);
    for (int i = 1; i < barPts.size(); ++i) {
        sendMouse(&view, QEvent::MouseMove, barPts.at(i), Qt::LeftButton);
    }
    sendMouse(&view, QEvent::MouseButtonRelease, barPts.last(), Qt::LeftButton);
    pump(150);

    const int beforeDragErase = overlay.items().size();
    controller.setTool(ToolController::Eraser);
    controller.setEraserRadius(16.0);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-380, -100), Qt::LeftButton);
    for (int i = 1; i < barPts.size(); ++i) {
        sendMouse(&view, QEvent::MouseMove, barPts.at(i), Qt::LeftButton);
    }
    sendMouse(&view, QEvent::MouseButtonRelease, barPts.last(), Qt::LeftButton);
    pump(150);
    check("eraser: drag removes the whole stroke", overlay.items().size() < beforeDragErase);

    controller.setTool(ToolController::Pen);
    const qreal widthBefore = controller.penWidth();
    QWheelEvent wheel(QPointF(400, 300), QPointF(400, 300), QPoint(0, 0), QPoint(0, 120),
                      Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QApplication::sendEvent(view.viewport(), &wheel);
    pump(80);
    printf("pen width %f -> %f\n", widthBefore, controller.penWidth());
    check("zoomit: wheel up enlarges the pen", controller.penWidth() > widthBefore);

    QWheelEvent wheelDown(QPointF(400, 300), QPointF(400, 300), QPoint(0, 0), QPoint(0, -120),
                          Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QApplication::sendEvent(view.viewport(), &wheelDown);
    pump(80);
    check("zoomit: wheel down shrinks the pen", controller.penWidth() < controller.penWidth() + 100.0);

    controller.setTool(ToolController::Eraser);
    const qreal radiusBefore = controller.eraserRadius();
    QWheelEvent wheelErase(QPointF(400, 300), QPointF(400, 300), QPoint(0, 0), QPoint(0, 120),
                            Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QApplication::sendEvent(view.viewport(), &wheelErase);
    pump(80);
    printf("eraser radius %f -> %f\n", radiusBefore, controller.eraserRadius());
    check("zoomit: wheel resizes the eraser", controller.eraserRadius() > radiusBefore);

    controller.setTool(ToolController::Pen);
    QKeyEvent keyColor(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier);
    controller.applyColorShortcut(&keyColor);
    check("zoomit: number key switches pen color",
          controller.penColor() == QColor(59, 125, 221));

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
