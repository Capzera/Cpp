#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScrollArea>
#include <QPainter>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/CartesianInfoPanel.h"
#include "ui/widgets/ToolIconButton.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/MyTextInput.h"
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"
#include <QScrollBar>
#include <QLabel>
#include <QLineEdit>

static int g_fail = 0;
static int g_pass = 0;

static void check(bool ok, const QString& label) {
    if (ok) {
        ++g_pass;
        printf("  [PASS] %s\n", qPrintable(label));
    } else {
        ++g_fail;
        printf("  [FAIL] %s\n", qPrintable(label));
    }
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void sendMouse(QWidget* widget, QEvent::Type type, Qt::MouseButton button,
                      const QPoint& pos, Qt::MouseButtons buttons) {
    QMouseEvent event(type, QPointF(pos), QPointF(widget->mapToGlobal(pos)),
                      button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

static void sendKey(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers mods) {
    QKeyEvent press(QEvent::KeyPress, key, mods);
    QApplication::sendEvent(widget, &press);
    QKeyEvent release(QEvent::KeyRelease, key, mods);
    QApplication::sendEvent(widget, &release);
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    MainWindow w;
    w.resize(1500, 900);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    int index = -1;
    for (int i = 0; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("笛卡尔坐标系")) { index = i; break; }
    }
    combo->setCurrentIndex(index);
    pump(500);

    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    CanvasView* view = w.findChild<CanvasView*>();
    ToolController* tc = w.findChild<ToolController*>();
    CanvasToolBar* bar = w.findChild<CanvasToolBar*>();
    CartesianInfoPanel* info = w.findChild<CartesianInfoPanel*>();
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    if (!engine || !view || !tc || !bar || !info) { printf("missing widgets\n"); return 1; }

    printf("=== 1. drag a point: free placement allowed, near-intersection snaps ===\n");
    {
        engine->clearObjects();
        engine->setStep(1.0);
        engine->addPoint(0.0, 0.0, QString());
        pump(150);
        CartesianPointItem* p = engine->pointAt(0);
        const QPoint from = view->mapFromScene(p->pos());
        const QPoint to = view->mapFromScene(engine->toScene(0.5, 0.8));
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::LeftButton, from, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseMove, Qt::NoButton, to, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::LeftButton, to, Qt::NoButton);
        pump(150);
        check(qAbs(p->x() - 0.5) < 1e-6 && qAbs(p->y() - 0.8) < 1e-6,
              QString("drag lands on the free cell position (0.5, 0.8), got (%1, %2)")
                  .arg(p->x()).arg(p->y()));

        const QPoint from2 = view->mapFromScene(p->pos());
        const QPoint near = view->mapFromScene(engine->toScene(0.05, 0.05));
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::LeftButton, from2, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseMove, Qt::NoButton, near, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::LeftButton, near, Qt::NoButton);
        pump(150);
        check(qAbs(p->x()) < 1e-6 && qAbs(p->y()) < 1e-6,
              QString("drag near the origin snaps onto the intersection, got (%1, %2)")
                  .arg(p->x()).arg(p->y()));
    }

    printf("\n=== 2. right-drag pan direction ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0.0, 0.0, QString("marker"));
        pump(150);
        CartesianPointItem* m = engine->pointAt(0);
        const QPoint before = view->mapFromScene(m->pos());
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::RightButton, QPoint(600, 300), Qt::RightButton);
        sendMouse(view->viewport(), QEvent::MouseMove, Qt::NoButton, QPoint(600, 360), Qt::RightButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::RightButton, QPoint(600, 360), Qt::NoButton);
        pump(150);
        const QPoint after = view->mapFromScene(m->pos());
        check(after.y() < before.y(),
              QString("mouse down 60px moves the picture UP, %1 -> %2").arg(before.y()).arg(after.y()));

        const QPoint leftBefore = view->mapFromScene(m->pos());
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::RightButton, QPoint(600, 300), Qt::RightButton);
        sendMouse(view->viewport(), QEvent::MouseMove, Qt::NoButton, QPoint(540, 300), Qt::RightButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::RightButton, QPoint(540, 300), Qt::NoButton);
        pump(150);
        const QPoint leftAfter = view->mapFromScene(m->pos());
        check(leftAfter.x() > leftBefore.x(),
              QString("mouse left 60px moves the picture RIGHT, %1 -> %2").arg(leftBefore.x()).arg(leftAfter.x()));
    }

    printf("\n=== 3. segment follows a dragged endpoint (no ghost) ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0.0, 0.0, QString("A"));
        engine->addPoint(4.0, 0.0, QString("B"));
        pump(150);
        CartesianPointItem* a = engine->pointAt(0);
        CartesianPointItem* b = engine->pointAt(1);
        engine->linkPoints(a, b);
        pump(150);
        CartesianSegmentItem* seg = engine->segments().last();
        const QRectF before = seg->boundingRect();

        const QPoint from = view->mapFromScene(a->pos());
        const QPoint to = view->mapFromScene(engine->toScene(0.0, 3.0));
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::LeftButton, from, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseMove, Qt::NoButton, to, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::LeftButton, to, Qt::NoButton);
        pump(200);
        const QRectF after = seg->boundingRect();
        check(qAbs(after.height() - before.height()) > 1.0,
              QString("segment geometry grew with the dragged endpoint, %1 -> %2")
                  .arg(before.height()).arg(after.height()));
        check(qAbs(a->y() - 3.0) < 1e-6, QString("endpoint reached y=3.0, got %1").arg(a->y()));
    }

    printf("\n=== 4. object list font size + double-click coordinate edit ===\n");
    {
        engine->clearObjects();
        engine->addPoint(2.0, 3.0, QString("P"));
        pump(200);
        QLabel* coordLabel = panel->findChild<QLabel*>();
        Q_UNUSED(coordLabel);
        bool bigEnough = false;
        bool found = false;
        for (QLabel* label : panel->findChildren<QLabel*>()) {
            if (!label->text().startsWith("(2, 3)")) continue;
            found = true;
            if (label->height() >= 16) bigEnough = true;
            printf("         coordinate label height=%d\n", label->height());
        }
        check(found, "the object list shows the point coordinate row");
        check(bigEnough, "the object list coordinate row is rendered large enough to read");
    }

    printf("\n=== 4b. editing a coordinate moves the point and its segment ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0.0, 0.0, QString("A"));
        engine->addPoint(4.0, 0.0, QString("B"));
        pump(150);
        engine->linkPoints(engine->pointAt(0), engine->pointAt(1));
        pump(150);
        CartesianSegmentItem* seg = engine->segments().last();
        const QRectF before = seg->boundingRect();

        CartesianPointItem* a = engine->pointAt(0);
        a->setMathPos(0.0, 5.0);
        pump(200);
        const QRectF after = seg->boundingRect();
        check(qAbs(after.height() - before.height()) > 1.0,
              QString("the attached segment follows a coordinate change, %1 -> %2")
                  .arg(before.height()).arg(after.height()));
    }

    printf("\n=== 5. Ctrl+A / Ctrl+C / Backspace on Cartesian points ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0.0, 0.0, QString("A"));
        engine->addPoint(1.0, 1.0, QString("B"));
        engine->addPoint(2.0, 2.0, QString("C"));
        pump(200);
        check(engine->pointCount() == 3, "three points exist");

        sendKey(&w, Qt::Key_A, Qt::ControlModifier);
        pump(150);
        check(engine->selectedPoints().size() == 3,
              QString("Ctrl+A selects all 3 points, got %1").arg(engine->selectedPoints().size()));

        sendKey(&w, Qt::Key_C, Qt::ControlModifier);
        pump(150);
        check(engine->clipboardPoints().size() == 3,
              QString("Ctrl+C copies 3 coordinates, got %1").arg(engine->clipboardPoints().size()));

        sendKey(&w, Qt::Key_A, Qt::ControlModifier);
        pump(120);
        sendKey(&w, Qt::Key_Backspace, Qt::NoModifier);
        pump(200);
        check(engine->pointCount() == 0,
              QString("Backspace deletes the selected points, %1 left").arg(engine->pointCount()));
    }

    printf("\n=== 6. freed point names are reused in lexicographic order ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0, 0, QString("A"));
        engine->addPoint(1, 0, QString("B"));
        engine->addPoint(2, 0, QString("C"));
        engine->addPoint(3, 0, QString("D"));
        engine->addPoint(4, 0, QString("E"));
        engine->addPoint(5, 0, QString("F"));
        engine->addPoint(6, 0, QString("G"));
        pump(150);
        for (const QString& victim : {QString("C"), QString("D"), QString("F")}) {
            for (int i = 0; i < engine->pointCount(); ++i) {
                CartesianPointItem* pt = engine->pointAt(i);
                if (pt && pt->name() == victim) { engine->removePoint(i); break; }
            }
            pump(80);
        }
        pump(150);
        QStringList remaining;
        for (CartesianPointItem* pt : engine->points()) remaining << pt->name();
        check(remaining == QStringList({"A", "B", "E", "G"}),
              QString("remaining after deleting C, D, F: %1").arg(remaining.join(",")));

        engine->addPoint(7, 0, QString());
        engine->addPoint(8, 0, QString());
        engine->addPoint(9, 0, QString());
        pump(150);
        QStringList created;
        for (CartesianPointItem* pt : engine->points()) created << pt->name();
        check(created == QStringList({"A", "B", "E", "G", "C", "D", "F"}),
              QString("the three new points take the freed names C, D, F -> %1").arg(created.join(",")));
    }

    printf("\n=== 7. info panel closes on deletion and on an outside click ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0, 0, QString("A"));
        engine->addPoint(3, 3, QString("B"));
        pump(150);
        engine->linkPoints(engine->pointAt(0), engine->pointAt(1));
        pump(150);
        emit engine->segmentDoubleClicked(0);
        pump(150);
        check(info->isVisible(), "the segment info panel opens after inspection");

        engine->removeSegment(0);
        pump(250);
        check(!info->isVisible(), "the info panel disappears once the segment is deleted");

        engine->addPoint(0, 0, QString("A"));
        engine->addPoint(3, 3, QString("B"));
        pump(150);
        engine->linkPoints(engine->pointAt(0), engine->pointAt(1));
        pump(150);
        emit engine->segmentDoubleClicked(0);
        pump(150);
        check(info->isVisible(), "the segment info panel opens again");

        const QPoint emptySpot = view->mapFromScene(engine->toScene(-9.0, -9.0));
        sendMouse(view->viewport(), QEvent::MouseButtonPress, Qt::LeftButton, emptySpot, Qt::LeftButton);
        sendMouse(view->viewport(), QEvent::MouseButtonRelease, Qt::LeftButton, emptySpot, Qt::NoButton);
        pump(200);
        check(!info->isVisible(), "clicking empty canvas dismisses the info panel");
    }

    printf("\n=== 7b. inspecting one segment survives a sibling deletion ===\n");
    {
        engine->clearObjects();
        engine->addPoint(0, 0, QString("A"));
        engine->addPoint(3, 0, QString("B"));
        engine->addPoint(0, 4, QString("C"));
        engine->addPoint(3, 4, QString("D"));
        pump(150);
        engine->linkPoints(engine->pointAt(0), engine->pointAt(1));
        engine->linkPoints(engine->pointAt(2), engine->pointAt(3));
        pump(150);
        check(engine->segments().size() == 2, "two segments exist");

        emit engine->segmentDoubleClicked(1);
        pump(150);
        check(info->isVisible(), "the second segment info panel opens");

        engine->removeSegment(0);
        pump(250);
        check(info->isVisible(),
              "deleting a sibling segment keeps the inspected segment's panel");

        engine->removeSegment(0);
        pump(250);
        check(!info->isVisible(),
              "deleting the inspected segment itself dismisses the panel");
    }

    printf("\n=== 8. function input lives in the top toolbar dropdown ===\n");
    {
        MyTextInput* input = w.findChild<MyTextInput*>("canvasCartFunctionInput");
        check(input != nullptr, "the toolbar dropdown owns the function input");
        check(panel->findChild<MyTextInput*>("cartesianFunctionInput") == nullptr,
              "the left panel no longer hosts a function input");
        if (input) {
            input->setText("3x^2 - 8x + 7");
            pump(120);
            QLineEdit* line = input->findChild<QLineEdit*>();
            if (line) {
                line->setFocus();
                sendKey(line, Qt::Key_Return, Qt::NoModifier);
            } else {
                sendKey(input, Qt::Key_Return, Qt::NoModifier);
            }
            pump(250);
            check(engine->curveCount() == 1,
                  QString("Enter in the dropdown draws the curve, curves=%1").arg(engine->curveCount()));
        }
    }

    printf("\n=== 9. the object list has a scroll bar ===\n");
    {
        QScrollArea* scroll = panel->findChild<QScrollArea*>("cartesianListScroll");
        check(scroll != nullptr, "the object list is wrapped in a scroll area");
        if (scroll) {
            engine->clearObjects();
            for (int i = 0; i < 40; ++i) engine->addPoint(i, i, QString());
            pump(400);
            scroll->setWidgetResizable(true);
            QApplication::processEvents();
            const int range = scroll->verticalScrollBar()->maximum();
            check(range > 0,
                  QString("the scroll bar becomes active with many objects, range=%1").arg(range));
        }
    }

    printf("\n========================================\n");
    printf("PASS %d   FAIL %d\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
