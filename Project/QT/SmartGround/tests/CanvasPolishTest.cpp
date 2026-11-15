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
#include "canvas/ShapeItem.h"
#include "canvas/TextBoxItem.h"
#include "canvas/CanvasTableItem.h"

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
                      Qt::MouseButton button = Qt::LeftButton,
                      Qt::KeyboardModifiers mods = Qt::NoModifier) {
    const QPointF localF = view->mapFromScene(scenePos);
    const QPointF globalF = view->mapToGlobal(localF.toPoint());
    QMouseEvent ev(type, localF, globalF, button,
                   type == QEvent::MouseButtonRelease ? Qt::MouseButtons() : Qt::MouseButtons(button), mods);
    QApplication::sendEvent(view->viewport(), &ev);
}

static void drag(CanvasView* view, const QPointF& from, const QPointF& to) {
    sendMouse(view, QEvent::MouseButtonPress, from);
    sendMouse(view, QEvent::MouseMove, (from + to) / 2.0);
    sendMouse(view, QEvent::MouseMove, to);
    sendMouse(view, QEvent::MouseButtonRelease, to);
}

static int inkPixels(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() < 235 && c.green() < 235 && c.blue() < 235) ++n;
        }
    }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene base;
    base.setSceneRect(-900, -900, 1800, 1800);
    base.setBackgroundBrush(Qt::white);

    AnnotationScene overlay;
    CanvasView view(&base);
    view.setAnnotationScene(&overlay);
    ToolController controller(&overlay, &view);
    view.setToolController(&controller);
    view.resize(1000, 800);
    view.show();
    pump(300);

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(20, 20, 20));
    controller.setPenWidth(6.0);

    QVector<QPointF> fast;
    for (int i = 0; i <= 8; ++i) {
        fast.push_back(QPointF(-400.0 + i * 80.0, -300.0 + (i % 2 ? 40.0 : -40.0)));
    }
    sendMouse(&view, QEvent::MouseButtonPress, fast.first());
    for (int i = 1; i < fast.size(); ++i) sendMouse(&view, QEvent::MouseMove, fast.at(i));
    sendMouse(&view, QEvent::MouseButtonRelease, fast.last());
    pump(200);

    QImage fastShot = view.grab().toImage();
    fastShot.save("fast_stroke.png");

    int gaps = 0;
    for (int x = 0; x < fastShot.width(); ++x) {
        bool inkOnColumn = false;
        for (int y = 0; y < fastShot.height(); ++y) {
            const QColor c = fastShot.pixelColor(x, y);
            if (c.red() < 200 && c.green() < 200 && c.blue() < 200) { inkOnColumn = true; break; }
        }
        if (!inkOnColumn) ++gaps;
    }
    printf("columns without ink = %d\n", gaps);
    check("speed: fast stroke has no vertical gaps", gaps < 120);

    controller.setTool(ToolController::Rect);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-400, -150));
    sendMouse(&view, QEvent::MouseMove, QPointF(-250, -60));
    pump(200);
    QImage previewShot = view.grab().toImage();
    const int previewInk = inkPixels(previewShot);
    printf("rect preview ink before release = %d\n", previewInk);
    check("preview: rectangle shows before mouse release", previewInk > 200);
    sendMouse(&view, QEvent::MouseButtonRelease, QPointF(-250, -60));
    pump(150);

    controller.setTool(ToolController::Circle);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-180, -150));
    sendMouse(&view, QEvent::MouseMove, QPointF(-60, -60));
    pump(200);
    QImage circleShot = view.grab().toImage();
    const int circleInk = inkPixels(circleShot);
    printf("circle preview ink before release = %d\n", circleInk);
    check("preview: circle shows before mouse release", circleInk > previewInk);
    sendMouse(&view, QEvent::MouseButtonRelease, QPointF(-60, -60));
    pump(150);

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(30, 60, 200));
    controller.setPenWidth(10.0);
    drag(&view, QPointF(-400, 100), QPointF(100, 100));
    drag(&view, QPointF(-150, 20), QPointF(-150, 180));
    pump(200);

    QImage overlapShot = view.grab().toImage();
    overlapShot.save("overlap_stroke.png");
    const QPoint crossScene(-150, 100);
    const QPoint crossPixel = view.mapFromScene(crossScene);
    printf("cross scene=%f,%f pixel=%d,%d image=%dx%d\n",
           crossScene.x(), crossScene.y(), crossPixel.x(), crossPixel.y(),
           overlapShot.width(), overlapShot.height());
    for (int dy = -10; dy <= 10; dy += 5) {
        for (int dx = -10; dx <= 10; dx += 5) {
            const QPoint p = crossPixel + QPoint(dx, dy);
            if (p.x() < 0 || p.y() < 0 || p.x() >= overlapShot.width() || p.y() >= overlapShot.height()) continue;
            const QColor c = overlapShot.pixelColor(p);
            printf("  (%+d,%+d) rgba(%d,%d,%d,%d)\n", dx, dy, c.red(), c.green(), c.blue(), c.alpha());
        }
    }
    int whiteCross = 0;
    for (int dy = -3; dy <= 3; ++dy) {
        for (int dx = -3; dx <= 3; ++dx) {
            const QPoint p = crossPixel + QPoint(dx, dy);
            if (p.x() < 0 || p.y() < 0 || p.x() >= overlapShot.width() || p.y() >= overlapShot.height()) continue;
            const QColor c = overlapShot.pixelColor(p);
            if (c.red() > 235 && c.green() > 235 && c.blue() > 235) ++whiteCross;
        }
    }
    printf("white pixels at crossing core = %d\n", whiteCross);
    check("overlap: crossing does not punch a white hole", whiteCross == 0);

    controller.setTool(ToolController::Mosaic);
    controller.setMosaicCellSize(18);
    drag(&view, QPointF(150, 20), QPointF(330, 160));
    pump(250);
    QImage mosaicShot = view.grab().toImage();
    mosaicShot.save("mosaic_result.png");
    int mosaicItems = 0;
    for (QGraphicsItem* item : overlay.items()) {
        auto* shape = qgraphicsitem_cast<ShapeItem*>(item);
        if (shape && shape->kind() == ShapeItem::Mosaic) ++mosaicItems;
    }
    check("mosaic: creates a mosaic shape", mosaicItems == 1);
    ShapeItem* mosaic = nullptr;
    for (QGraphicsItem* item : overlay.items()) {
        auto* shape = qgraphicsitem_cast<ShapeItem*>(item);
        if (shape && shape->kind() == ShapeItem::Mosaic) mosaic = shape;
    }
    if (mosaic) {
        check("mosaic: captured a real pixelated bitmap", !mosaic->mosaicImage().isNull());
        check("mosaic: bitmap is blocky not source resolution",
              mosaic->mosaicImage().width() > 0 && mosaic->mosaicImage().width() % 18 == 0);
    }

    {
        QGraphicsRectItem* rect = base.addRect(160, 40, 140, 50);
        rect->setBrush(QBrush(QColor(230, 120, 60)));
        rect->setPen(Qt::NoPen);
        base.addRect(180, 55, 40, 20);
        pump(150);
        controller.setTool(ToolController::Mosaic);
        controller.setMosaicCellSize(12);
        drag(&view, QPointF(150, 20), QPointF(330, 100));
        pump(250);
        ShapeItem* mosaic2 = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* shape = qgraphicsitem_cast<ShapeItem*>(item);
            if (shape && shape->kind() == ShapeItem::Mosaic && shape != mosaic) mosaic2 = shape;
        }
        check("mosaic: second mosaic created", mosaic2 != nullptr);
        if (mosaic2) {
            const QImage& src = mosaic2->mosaicImage();
            int opaque = 0;
            int orange = 0;
            for (int y = 0; y < src.height(); ++y) {
                for (int x = 0; x < src.width(); ++x) {
                    const QColor c = src.pixelColor(x, y);
                    if (c.alpha() > 0) ++opaque;
                    if (c.red() > 180 && c.green() < 160 && c.blue() < 120) ++orange;
                }
            }
            printf("mosaic bitmap opaque=%d orange=%d of %d\n", opaque, orange, src.width() * src.height());
            check("mosaic: captured underlying content (not blank)", opaque > 0);
            check("mosaic: underlying color preserved in blocks", orange > 0);
        }
        QImage m2 = view.grab().toImage();
        m2.save("mosaic_over_content.png");
    }

    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(30, 160, 90));
    drag(&view, QPointF(150, 250), QPointF(400, 300));
    pump(150);

    const int beforeCopy = overlay.annotationItems().size();
    controller.setTool(ToolController::AnnotationSelect);
    controller.selectAll();
    check("clipboard: select all picks items", controller.selectedItems().size() == beforeCopy);
    check("clipboard: copy succeeds", controller.copySelection());
    check("clipboard: clipboard is populated", controller.hasClipboard());
    check("clipboard: cut succeeds", controller.cutSelection());
    check("clipboard: cut removes items", overlay.annotationItems().size() < beforeCopy);
    const int afterCut = overlay.annotationItems().size();
    check("clipboard: paste succeeds", controller.pasteAt(QPointF(0, 400)));
    check("clipboard: paste restores item count", overlay.annotationItems().size() == afterCut + beforeCopy);
    controller.undo();
    check("clipboard: undo after paste removes pasted copy", overlay.annotationItems().size() == afterCut);
    controller.redo();
    check("clipboard: redo restores pasted copy", overlay.annotationItems().size() == afterCut + beforeCopy);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
