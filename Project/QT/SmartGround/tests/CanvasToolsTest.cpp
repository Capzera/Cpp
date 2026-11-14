#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QEventLoop>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
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
    QMouseEvent ev(type, localF, globalF,
                   button, type == QEvent::MouseButtonRelease ? Qt::MouseButtons() : Qt::MouseButtons(button),
                   mods);
    QApplication::sendEvent(view->viewport(), &ev);
}

static void drag(CanvasView* view, const QPointF& from, const QPointF& to,
                 Qt::KeyboardModifiers mods = Qt::NoModifier) {
    sendMouse(view, QEvent::MouseButtonPress, from, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseMove, (from + to) / 2.0, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseMove, to, Qt::LeftButton, mods);
    sendMouse(view, QEvent::MouseButtonRelease, to, Qt::LeftButton, mods);
}

static int countType(AnnotationScene& scene, int type) {
    int n = 0;
    for (QGraphicsItem* item : scene.items()) {
        if (item->type() == type) ++n;
    }
    return n;
}

static AnnotationItem* newestOfType(AnnotationScene& scene, int type) {
    for (QGraphicsItem* item : scene.items()) {
        if (item->type() == type) return dynamic_cast<AnnotationItem*>(item);
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene base;
    base.setSceneRect(-500, -500, 2000, 2000);
    base.setBackgroundBrush(Qt::white);

    AnnotationScene overlay;
    CanvasView view(&base);
    view.setAnnotationScene(&overlay);
    ToolController controller(&overlay, &view);
    view.setToolController(&controller);
    view.resize(900, 700);
    view.show();
    pump(200);

    check("canvas: annotation scene starts empty", overlay.items().isEmpty());
    check("canvas: tool controller defaults to Select", controller.currentTool() == ToolController::Select);

    controller.setTool(ToolController::Pen);
    drag(&view, QPointF(100, 100), QPointF(220, 160));
    check("pen: creates a stroke", overlay.items().size() == 1);
    auto* stroke = qgraphicsitem_cast<StrokeItem*>(overlay.items().first());
    check("pen: item is a StrokeItem", stroke != nullptr);
    if (stroke) check("pen: stroke has multiple points", stroke->points().size() >= 2);

    controller.setTool(ToolController::Rect);
    drag(&view, QPointF(300, 100), QPointF(420, 200));
    check("rect: creates a second item", overlay.items().size() == 2);
    auto* shape = qgraphicsitem_cast<ShapeItem*>(newestOfType(overlay, ShapeItem::Type));
    check("rect: item is a ShapeItem", shape != nullptr);
    if (shape) check("rect: kind is Rectangle", shape->kind() == ShapeItem::Rectangle);

    controller.setTool(ToolController::Circle);
    drag(&view, QPointF(500, 100), QPointF(600, 200));
    controller.setTool(ToolController::Line);
    drag(&view, QPointF(100, 300), QPointF(300, 300));
    controller.setTool(ToolController::Arrow);
    drag(&view, QPointF(100, 380), QPointF(300, 380));
    controller.setTool(ToolController::Triangle);
    drag(&view, QPointF(400, 300), QPointF(520, 420));
    controller.setTool(ToolController::Mosaic);
    drag(&view, QPointF(600, 300), QPointF(700, 400));
    check("shapes: six shape tools produce items", overlay.items().size() == 7);

    const int before = overlay.items().size();
    controller.setTool(ToolController::Eraser);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(160, 130), Qt::LeftButton, Qt::NoModifier);
    check("eraser: removes an annotation", overlay.items().size() == before - 1);
    check("eraser: undo stack populated", controller.canUndo());

    controller.undo();
    check("undo: restores the erased item", overlay.items().size() == before);
    controller.redo();
    check("redo: removes it again", overlay.items().size() == before - 1);
    controller.undo();
    check("undo: restores again", overlay.items().size() == before);

    controller.setTool(ToolController::TextBox);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(120, 500), Qt::LeftButton, Qt::NoModifier);
    auto* textBox = qgraphicsitem_cast<TextBoxItem*>(newestOfType(overlay, TextBoxItem::Type));
    check("textbox: creates a text item", textBox != nullptr);

    controller.setTool(ToolController::Markdown);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(350, 500), Qt::LeftButton, Qt::NoModifier);
    auto* md = qgraphicsitem_cast<MarkdownItem*>(newestOfType(overlay, MarkdownItem::Type));
    check("markdown: creates a markdown item", md != nullptr);
    if (md) check("markdown: renders non-empty content", !md->contentBounds().isEmpty());

    controller.setTool(ToolController::Table);
    controller.setTableSize(3, 4);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(600, 500), Qt::LeftButton, Qt::NoModifier);
    auto* table = qgraphicsitem_cast<CanvasTableItem*>(newestOfType(overlay, CanvasTableItem::Type));
    check("table: creates a table item", table != nullptr);
    if (table) {
        check("table: dimensions respected", table->rows() == 3 && table->cols() == 4);
        check("table: cells default to empty", table->cellAt(0, 0).isEmpty());
        table->setCell(1, 2, "42");
        check("table: cell edit persists", table->cellAt(1, 2) == QString("42"));
    }

    controller.setTool(ToolController::Latex);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(820, 500), Qt::LeftButton, Qt::NoModifier);
    auto* latex = qgraphicsitem_cast<LatexItem*>(newestOfType(overlay, LatexItem::Type));
    check("latex: creates a latex item", latex != nullptr);
    if (latex) check("latex: source tex recorded", latex->tex() == QString("E = mc^2"));

    QJsonObject saved = overlay.toJson();
    check("serialize: has items array", saved.value("items").toArray().size() == overlay.annotationItems().size());

    AnnotationScene restored;
    restored.fromJson(saved);
    check("serialize: round trip restores items", restored.annotationItems().size() == overlay.annotationItems().size());

    overlay.setAnnotationsVisible(false);
    bool allHidden = true;
    for (QGraphicsItem* item : overlay.items()) {
        if (item->isVisible()) allHidden = false;
    }
    check("visibility: hiding hides every annotation", allHidden);
    overlay.setAnnotationsVisible(true);

    overlay.setAnnotationsLocked(true);
    bool allLocked = true;
    for (QGraphicsItem* item : overlay.items()) {
        auto* ann = qgraphicsitem_cast<AnnotationItem*>(item);
        if (ann && !ann->isLocked()) allLocked = false;
    }
    check("lock: locking locks every annotation", allLocked);
    overlay.setAnnotationsLocked(false);

    controller.setTool(ToolController::AnnotationSelect);
    drag(&view, QPointF(60, 60), QPointF(460, 440));
    check("marquee: selects multiple annotations", controller.selectedItems().size() >= 3);

    controller.selectAll();
    check("select all: picks every annotation",
          controller.selectedItems().size() == overlay.annotationItems().size());

    const qreal beforeScale = controller.selectedItems().isEmpty()
                                  ? 1.0
                                  : controller.selectedItems().first()->scale();
    controller.scaleSelected(1.2);
    const qreal afterScale = controller.selectedItems().isEmpty()
                                 ? beforeScale
                                 : controller.selectedItems().first()->scale();
    check("scale: enlarges selected annotations", afterScale > beforeScale);

    const int totalBefore = overlay.annotationItems().size();
    const int selCount = controller.selectedItems().size();
    controller.deleteSelected();
    check("delete: removes selected annotations",
          controller.selectedItems().isEmpty() &&
              overlay.annotationItems().size() == totalBefore - selCount);
    controller.undo();
    check("delete: undo restores deleted annotations",
          overlay.annotationItems().size() == totalBefore);

    view.grab().save("canvas_tools.png");

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}

