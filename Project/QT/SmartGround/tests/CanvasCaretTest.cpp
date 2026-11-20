#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <cstdio>
#include "canvas/TextBoxItem.h"
#include "canvas/MarkdownItem.h"
#include "canvas/InlineTextEdit.h"

static int failures = 0;

static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static int caretX(QPainter* p, const QRectF& r) {
    return 0;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QGraphicsScene scene;

    TextBoxItem box;
    box.setPlainText("Hello World");
    scene.addItem(&box);
    box.beginEdit();
    pump(50);

    const QRectF bounds = box.contentBounds();
    const QPointF target = box.mapToScene(QPointF(bounds.left() + 30.0, bounds.top() + 10.0));
    const bool placed = box.placeCaretAt(target);
    check(placed, "text box caret placed by click");

    const int cursorPos = box.cursorPositionForTest();
    check(cursorPos > 0 && cursorPos < 11, "caret landed inside the clicked character range");

    box.setPlainText("Hello World");
    box.beginEdit();
    pump(30);
    box.placeCaretAt(box.mapToScene(QPointF(bounds.left() + 80.0, bounds.top() + 10.0)));
    const int posAfter = box.cursorPositionForTest();
    check(posAfter > cursorPos + 2, "caret moves to a later character on second click");

    MarkdownItem md;
    md.setMarkdown("# Title\n\nbody text here");
    scene.addItem(&md);
    md.beginEdit();
    pump(50);
    const QRectF mdBounds = md.contentBounds();
    const bool mdPlaced = md.placeCaretAt(md.mapToScene(QPointF(mdBounds.left() + 20.0, mdBounds.top() + 8.0)));
    check(mdPlaced, "markdown caret placed by click");
    check(md.cursorPositionForTest() >= 0, "markdown caret position valid");

    printf("\n%d failures\n", failures);
    return failures ? 1 : 0;
}
