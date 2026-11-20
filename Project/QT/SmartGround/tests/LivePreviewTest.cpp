#include <QApplication>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QTimer>
#include <QKeyEvent>
#include <QTextCursor>
#include <QMouseEvent>
#include <cstdio>
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"
#include "canvas/AnnotationItem.h"
#include "canvas/TextBoxItem.h"
#include "canvas/MarkdownItem.h"
#include "render/GraphElements.h"
#include "core/TextEditGuard.h"

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

static void sendKey(QWidget* target, int key, const QString& text,
                    Qt::KeyboardModifiers mods = Qt::NoModifier) {
    QKeyEvent ev(QEvent::KeyPress, key, mods, text);
    QApplication::sendEvent(target, &ev);
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

    auto* box = new TextBoxItem();
    overlay.addItem(box);
    controller.setTool(ToolController::AnnotationSelect);
    const QPointF blPos = view.mapFromScene(QPointF(0, 0));
    QMouseEvent dbl(QEvent::MouseButtonDblClick, blPos, view.mapToGlobal(blPos.toPoint()),
                    Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view.viewport(), &dbl);
    pump(120);
    check("textbox: inline edit active", box->isInlineEditing());

    sendKey(view.viewport(), Qt::Key_A, "a");
    pump(80);
    check("textbox: keystroke reaches the buffer", box->plainText() == QString("a"));
    printf("    textbox buffer='%s'\n", box->plainText().toUtf8().constData());

    sendKey(view.viewport(), Qt::Key_B, "b");
    pump(80);
    check("textbox: second keystroke appends", box->plainText() == QString("ab"));

    QImage boxShot = view.grab().toImage();
    int boxDark = 0;
    for (int y = 0; y < boxShot.height(); ++y)
        for (int x = 0; x < boxShot.width(); ++x)
            if (boxShot.pixelColor(x, y).lightness() < 170) boxDark++;
    check("textbox: typed glyphs are painted live", boxDark > 8);
    printf("    textbox dark pixels=%d size=%dx%d\n", boxDark, boxShot.width(), boxShot.height());

    auto* md = new MarkdownItem();
    overlay.addItem(md);
    const QPointF mdPos = view.mapFromScene(QPointF(50, 50));
    QMouseEvent dbl2(QEvent::MouseButtonDblClick, mdPos, view.mapToGlobal(mdPos.toPoint()),
                     Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(view.viewport(), &dbl2);
    pump(120);
    check("markdown: inline edit active", md->isInlineEditing());

    sendKey(view.viewport(), Qt::Key_H, "#");
    pump(80);
    check("markdown: keystroke reaches the source", md->markdown().contains("#"));
    printf("    markdown source='%s'\n", md->markdown().toUtf8().constData());

    QImage mdShot = view.grab().toImage();
    int mdDark = 0;
    for (int y = 0; y < mdShot.height(); ++y)
        for (int x = 0; x < mdShot.width(); ++x)
            if (mdShot.pixelColor(x, y).lightness() < 170) mdDark++;
    check("markdown: rendered preview updates live", mdDark > 20);
    printf("    markdown dark pixels=%d size=%dx%d\n", mdDark, mdShot.width(), mdShot.height());

    ArrayBlock* block = new ArrayBlock(1, 5, "1");
    base.addItem(block);
    block->setEditable(true);
    block->beginEdit();
    pump(120);
    check("array: editor focused", TextEditGuard::active(&base));
    check("array: color shortcut blocked while editing", !controller.applyColorShortcut(nullptr));
    printf("    guard active=%d\n", TextEditGuard::active(&base) ? 1 : 0);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
