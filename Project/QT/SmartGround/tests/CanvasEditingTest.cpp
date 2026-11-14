#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QKeyEvent>
#include <cstdio>
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"
#include "canvas/AnnotationItem.h"
#include "canvas/TextBoxItem.h"
#include "canvas/MarkdownItem.h"
#include "canvas/ShapeItem.h"

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

    controller.setTool(ToolController::TextBox);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(-200, -100));
    pump(150);

    auto* box = qgraphicsitem_cast<TextBoxItem*>(overlay.annotationItems().first());
    check("textbox: inserting a text box creates one item", box != nullptr);
    if (!box) {
        printf("\nHAS FAILURE  fail=%d\n", g_fail + 1);
        return 1;
    }
    check("textbox: after insert the tool falls back to select",
          controller.currentTool() == ToolController::AnnotationSelect);
    check("textbox: inserted box enters inline edit mode", box->isInlineEditing());

    box->setPlainText("课堂板书");
    check("textbox: inline edit accepts typed content", box->plainText() == QString("课堂板书"));

    controller.setTool(ToolController::Markdown);
    sendMouse(&view, QEvent::MouseButtonPress, QPointF(100, -100));
    pump(150);
    MarkdownItem* md = nullptr;
    for (AnnotationItem* it : overlay.annotationItems()) {
        if (auto* cand = qgraphicsitem_cast<MarkdownItem*>(it)) md = cand;
    }
    check("markdown: inserting a note creates one item", md != nullptr);
    if (md) {
        check("markdown: after insert the tool falls back to select",
              controller.currentTool() == ToolController::AnnotationSelect);
        check("markdown: inserted note enters inline edit mode", md->isInlineEditing());
        md->setMarkdown("# 标题\n正文");
        check("markdown: inline edit accepts source", md->markdown().contains("标题"));
    }

    check("archive: scene holds both items", overlay.annotationItems().size() == 2);
    overlay.stashTo("冒泡排序");
    check("archive: stashing clears the canvas", overlay.annotationItems().isEmpty());
    check("archive: restore brings items back", overlay.restoreFrom("冒泡排序"));
    check("archive: restored item count matches", overlay.annotationItems().size() == 2);
    check("archive: restoring an unknown bucket is a no-op", !overlay.restoreFrom("不存在的模块"));

    controller.setTool(ToolController::AnnotationSelect);
    sendMouse(&view, QEvent::MouseButtonDblClick, QPointF(-200, -100));
    pump(150);
    auto* box2 = qgraphicsitem_cast<TextBoxItem*>(overlay.annotationItems().first());
    check("double click: double clicking a text box does not create a new one",
          overlay.annotationItems().size() == 2);
    check("double click: double click toggles inline edit", box2 != nullptr && box2->isInlineEditing());

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
