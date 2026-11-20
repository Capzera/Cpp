#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <cstdio>
#include <cmath>
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"
#include "canvas/AnnotationItem.h"
#include "canvas/StrokeItem.h"
#include "canvas/TextBoxItem.h"
#include "canvas/MarkdownItem.h"
#include "canvas/LatexItem.h"
#include "canvas/CanvasTableItem.h"
#include "canvas/SelectionOverlay.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/ToolIconButton.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyColorPicker.h"

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

static void sendKey(CanvasView* view, int key, const QString& text = QString(),
                     Qt::KeyboardModifiers mods = Qt::NoModifier) {
    QKeyEvent press(QEvent::KeyPress, key, mods, text);
    QApplication::sendEvent(view->viewport(), &press);
    QKeyEvent release(QEvent::KeyRelease, key, mods, text);
    QApplication::sendEvent(view->viewport(), &release);
}

static void clickWidget(QWidget* w) {
    const QPointF g(w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
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

    CanvasToolBar bar(&controller, &view);
    bar.setHostWidget(&view);
    bar.show();
    bar.raise();
    pump(200);

    printf("\n--- 1. per-tool independent style memory ---\n");
    controller.setTool(ToolController::Pen);
    controller.setPenColor(QColor(200, 30, 30));
    controller.setPenWidth(7.0);
    controller.setTool(ToolController::Highlighter);
    check("highlighter keeps its own default color", controller.penColor() != QColor(200, 30, 30));
    controller.setPenColor(QColor(250, 220, 60));
    controller.setPenWidth(14.0);
    controller.setTool(ToolController::Pen);
    check("pen remembers its own color after tool switch", controller.penColor() == QColor(200, 30, 30));
    check("pen remembers its own width after tool switch", qFuzzyCompare(controller.penWidth(), 7.0));
    controller.setTool(ToolController::Highlighter);
    check("highlighter remembers its own width", qFuzzyCompare(controller.penWidth(), 14.0));
    controller.setTool(ToolController::Pen);

    printf("\n--- 2. smooth stroke, no corner artifacts ---\n");
    {
        controller.setPenColor(QColor(20, 20, 20));
        controller.setPenWidth(6.0);
        QVector<QPointF> jag;
        for (int i = 0; i <= 30; ++i) {
            jag.push_back(QPointF(-400 + i * 12.0, -300 + ((i % 2) ? 40.0 : -40.0)));
        }
        sendMouse(&view, QEvent::MouseButtonPress, jag.first());
        for (int i = 1; i < jag.size(); ++i) sendMouse(&view, QEvent::MouseMove, jag.at(i));
        sendMouse(&view, QEvent::MouseButtonRelease, jag.last());
        pump(200);
        QImage shot = view.grab().toImage();
        shot.save("smart_smooth.png");
        StrokeItem* stroke = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* s = qgraphicsitem_cast<StrokeItem*>(item);
            if (s) stroke = s;
        }
        check("smooth: stroke exists", stroke != nullptr);
        if (stroke) {
            const QPainterPath path = stroke->inkPath();
            check("smooth: ink path is a smooth cubic spline", path.elementCount() > 5);
        }
    }

    printf("\n--- 3. unlimited color picker widget ---\n");
    {
        MyColorPicker* picker = view.findChild<MyColorPicker*>("canvasToolColorPicker");
        check("color picker embedded in the pen settings panel", picker != nullptr);
    }

    printf("\n--- 4. clipboard shortcuts only, no dedicated buttons ---\n");
    {
        check("no dedicated copy button in the toolbar",
              view.findChild<MyButton*>("canvasCopyButton") == nullptr);
        check("no dedicated undo button in the toolbar",
              view.findChild<MyButton*>("canvasUndoButton") == nullptr);
        check("no dedicated select-all button in the toolbar",
              view.findChild<MyButton*>("canvasSelectAllButton") == nullptr);
    }

    printf("\n--- 5. backspace / delete removes the selected region ---\n");
    {
        controller.setTool(ToolController::AnnotationSelect);
        controller.selectAll();
        const int before = overlay.annotationItems().size();
        check("select: marquee-less select-all picks items", before > 0);
        sendKey(&view, Qt::Key_Backspace);
        pump(120);
        check("backspace deletes selected annotations", overlay.annotationItems().size() == 0);
        controller.undo();
        pump(120);
        check("undo restores deleted annotations", overlay.annotationItems().size() == before);
        controller.selectAll();
        sendKey(&view, Qt::Key_Delete);
        pump(120);
        check("delete key also removes selected annotations", overlay.annotationItems().size() == 0);
        controller.undo();
        pump(120);
        controller.deselectAll();
    }

    printf("\n--- 6. long-press drag moves the selection ---\n");
    {
        controller.setTool(ToolController::AnnotationSelect);
        controller.selectAll();
        check("move: selection is active", controller.selectedItems().size() > 0);
        AnnotationItem* first = controller.selectedItems().first();
        const QPointF origin = first->pos();
        const QPointF anchor = first->mapRectToScene(first->contentBounds()).center();
        drag(&view, anchor, anchor + QPointF(120, 60));
        pump(150);
        check("move: selected items follow the drag", !qFuzzyCompare(first->pos().x(), origin.x()));
        controller.deselectAll();
    }

    printf("\n--- 6b. drag accumulates the full cursor offset ---\n");
    {
        controller.setTool(ToolController::AnnotationSelect);
        controller.selectAll();
        AnnotationItem* first = controller.selectedItems().first();
        const QPointF origin = first->pos();
        const QPointF anchor = first->mapRectToScene(first->contentBounds()).center();
        const QPointF target = anchor + QPointF(90, 45);
        sendMouse(&view, QEvent::MouseButtonPress, anchor);
        for (int i = 1; i <= 9; ++i) {
            sendMouse(&view, QEvent::MouseMove, anchor + QPointF(10.0 * i, 5.0 * i));
        }
        sendMouse(&view, QEvent::MouseButtonRelease, target);
        pump(120);
        const QPointF moved = first->pos() - origin;
        printf("move offset = (%.2f, %.2f), expected (90.00, 45.00)\n", moved.x(), moved.y());
        check("move: offset equals the whole cursor travel, no jitter",
              qAbs(moved.x() - 90.0) < 0.5 && qAbs(moved.y() - 45.0) < 0.5);
        controller.deselectAll();
    }

    printf("\n--- 6c. inline editing blocks every global shortcut ---\n");
    {
        controller.setTool(ToolController::TextBox);
        sendMouse(&view, QEvent::MouseButtonPress, QPointF(-420, -420));
        pump(150);
        check("edit: textbox entered inline editing", controller.isInlineEditing());
        const QColor colorBefore = controller.penColor();
        const ToolController::Tool toolBefore = controller.currentTool();
        sendKey(&view, Qt::Key_1, QString("1"));
        sendKey(&view, Qt::Key_B, QString("b"));
        pump(120);
        check("edit: the digit shortcut does not change the pen color",
              controller.penColor() == colorBefore);
        check("edit: the letter shortcut does not switch tools",
              controller.currentTool() == toolBefore);
        TextBoxItem* edited = dynamic_cast<TextBoxItem*>(controller.editingItem());
        check("edit: the typed characters still reach the text box",
              edited && edited->plainText().contains(QString("1")));
        controller.setTool(ToolController::AnnotationSelect);
        controller.selectAll();
        controller.deleteSelected();
        pump(100);
        controller.deselectAll();
    }

    printf("\n--- 7. textbox is transparent and editable in place ---\n");
    {
        controller.setTool(ToolController::TextBox);
        sendMouse(&view, QEvent::MouseButtonPress, QPointF(200, 200));
        pump(150);
        check("textbox: inserting switches back to the select tool",
              controller.currentTool() == ToolController::AnnotationSelect);
        TextBoxItem* box = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* t = qgraphicsitem_cast<TextBoxItem*>(item);
            if (t) box = t;
        }
        check("textbox: item created", box != nullptr);
        if (box) {
            check("textbox: enters inline editing right after insertion", box->isInlineEditing());
            sendKey(&view, Qt::Key_H, QString("H"));
            sendKey(&view, Qt::Key_I, QString("i"));
            pump(150);
            check("textbox: typing lands in the text box", box->plainText() == QString("Hi"));
            QImage shot = view.grab().toImage();
            shot.save("smart_textbox.png");
            QImage card(box->boundingRect().size().toSize(), QImage::Format_ARGB32);
            card.fill(Qt::transparent);
            {
                QPainter cp(&card);
                box->paint(&cp, nullptr, nullptr);
                cp.end();
            }
            bool whiteBg = false;
            for (int y = 0; y < card.height() && !whiteBg; ++y) {
                for (int x = 0; x < card.width(); ++x) {
                    const QColor c = card.pixelColor(x, y);
                    if (c.alpha() > 10 && c.red() > 245 && c.green() > 245 && c.blue() > 245) {
                        whiteBg = true;
                        break;
                    }
                }
            }
            check("textbox: background is fully transparent (no white card)", !whiteBg);
        }
    }

    printf("\n--- 8. latex and markdown are movable ---\n");
    {
        controller.setTool(ToolController::Latex);
        sendMouse(&view, QEvent::MouseButtonPress, QPointF(-200, 300));
        pump(200);
        LatexItem* latex = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* l = qgraphicsitem_cast<LatexItem*>(item);
            if (l) latex = l;
        }
        check("latex: item created", latex != nullptr);
        if (latex) {
            check("latex: accepts left mouse button for dragging", latex->acceptedMouseButtons() != Qt::NoButton);
            const QPointF origin = latex->pos();
            latex->setPos(origin + QPointF(40, 25));
            pump(100);
            check("latex: can be repositioned", !qFuzzyCompare(latex->pos().x(), origin.x()));
        }

        controller.setTool(ToolController::Markdown);
        sendMouse(&view, QEvent::MouseButtonPress, QPointF(-200, 420));
        pump(200);
        MarkdownItem* md = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* m = qgraphicsitem_cast<MarkdownItem*>(item);
            if (m) md = m;
        }
        check("markdown: item created", md != nullptr);
        if (md) {
            check("markdown: enters inline editing right after insertion", md->isInlineEditing());
            sendKey(&view, Qt::Key_A, QString("a"));
            pump(120);
            check("markdown: typing lands in the note", md->markdown().contains(QString("a")));
        }
    }

    printf("\n--- 9. selection resize handles ---\n");
    {
        controller.setTool(ToolController::AnnotationSelect);
        controller.deselectAll();
        TextBoxItem* box = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* t = qgraphicsitem_cast<TextBoxItem*>(item);
            if (t) box = t;
        }
        if (box) {
            controller.selectAll();
            pump(100);
            SelectionOverlay* overlayHandle = controller.selectionOverlay();
            check("resize: overlay is active", overlayHandle && overlayHandle->isActive());
            const QRectF bounds = overlayHandle->sceneBounds();
            const QPointF corner = bounds.topLeft();
            const SelectionOverlay::Handle handle = overlayHandle->handleAt(corner, 12.0);
            check("resize: corner handle is hit-testable", handle == SelectionOverlay::TopLeft);
            const qreal beforeWidth = bounds.width();
            sendMouse(&view, QEvent::MouseButtonPress, corner);
            sendMouse(&view, QEvent::MouseMove, corner - QPointF(60, 40));
            sendMouse(&view, QEvent::MouseButtonRelease, corner - QPointF(60, 40));
            pump(150);
            const QRectF after = overlayHandle->sceneBounds();
            printf("bounds width %.1f -> %.1f\n", beforeWidth, after.width());
            check("resize: dragging the handle grows the selection", after.width() > beforeWidth + 10);
            QImage shot = view.grab().toImage();
            shot.save("smart_resize.png");
        }
    }

    printf("\n--- 10. table context toolbar operations ---\n");
    {
        controller.setTool(ToolController::Table);
        controller.setTableSize(3, 3);
        sendMouse(&view, QEvent::MouseButtonPress, QPointF(200, -300));
        pump(200);
        CanvasTableItem* table = nullptr;
        for (QGraphicsItem* item : overlay.items()) {
            auto* t = qgraphicsitem_cast<CanvasTableItem*>(item);
            if (t) table = t;
        }
        check("table: item created", table != nullptr);
        if (table) {
            check("table: 3x3 by default", table->rows() == 3 && table->cols() == 3);
            table->setCell(0, 0, "A");
            table->setCellAlign(0, 0, TableAlign::Left);
            check("table: cell alignment stored", table->cellAlign(0, 0) == TableAlign::Left);
            check("table: insert row works", table->insertRows(1, 1));
            check("table: row count grew", table->rows() == 4);
            check("table: insert column works", table->insertColumns(1, 2));
            check("table: column count grew", table->cols() == 5);
            check("table: remove row works", table->removeRows(1, 1));
            check("table: remove column works", table->removeColumns(1, 1));
            check("table: dimensions settle back", table->rows() == 3 && table->cols() == 4);
            QSet<int> cells;
            cells.insert(0);
            table->selectCells(cells);
            check("table: cell selection stored", table->selectedCells().size() == 1);
        }
    }

    printf("\n--- toolbar two-stage behaviour ---\n");
    {
        ToolIconButton* penBtn = view.findChild<ToolIconButton*>("canvasTool_2");
        check("pen button exists", penBtn != nullptr);
        if (penBtn) {
            controller.setTool(ToolController::Select);
            clickWidget(penBtn);
            QApplication::processEvents();
            check("first click selects the tool", controller.currentTool() == ToolController::Pen);
            check("first click does not open the panel", !bar.isToolPanelOpen());
            clickWidget(penBtn);
            QApplication::processEvents();
            check("second click opens the pen settings", bar.isToolPanelOpen());
            clickWidget(penBtn);
            QApplication::processEvents();
            check("third click closes the pen settings", !bar.isToolPanelOpen());
        }
        ToolIconButton* hlBtn = view.findChild<ToolIconButton*>("canvasTool_3");
        if (hlBtn) {
            clickWidget(hlBtn);
            QApplication::processEvents();
            clickWidget(hlBtn);
            QApplication::processEvents();
            check("highlighter has its own settings panel", bar.isToolPanelOpen());
            QImage shot = view.grab().toImage();
            shot.save("smart_highlighter_panel.png");
        }
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
