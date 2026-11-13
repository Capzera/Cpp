#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QTimer>
#include <QEventLoop>
#include <cstdio>
#include "modules/PrefixSumModule.h"
#include "render/GraphElements.h"

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

static QGraphicsTextItem* editorOf(QGraphicsItem* parent) {
    for (QGraphicsItem* child : parent->childItems()) {
        QGraphicsTextItem* t = dynamic_cast<QGraphicsTextItem*>(child);
        if (t) return t;
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setAttribute(Qt::WA_DontShowOnScreen, true);
    view.show();
    QApplication::processEvents();

    PrefixSumModule mod;
    mod.buildLayout(&scene, "4 1\n1 2 3 4\n1 4");
    mod.enableLiveEditing();

    ArrayBlock* a1 = nullptr;
    for (QGraphicsItem* item : scene.items()) {
        ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
        if (blk && blk->id == 1 && blk->pos().y() < 300) a1 = blk;
    }
    check(a1 != nullptr, "a[1] block exists");
    if (!a1) { printf("\n%d failures\n", failures); return 1; }
    check(a1->isEditable(), "a[1] is editable");

    a1->beginEdit();
    pump(30);
    check(a1->isEditing(), "double click starts editing");

    QGraphicsTextItem* ed = editorOf(a1);
    check(ed != nullptr, "editor item created");
    if (ed) {
        QFocusEvent fo(QEvent::FocusOut, Qt::ActiveWindowFocusReason);
        QApplication::sendEvent(ed, &fo);
        pump(30);
        printf("survived focus-out commit\n");
        check(!a1->isEditing(), "focus out ends editing");
    }

    a1->beginEdit();
    pump(30);
    ed = editorOf(a1);
    check(ed != nullptr, "second editor created");
    if (ed) {
        ed->setPlainText("7");
        QTextCursor c = ed->textCursor();
        c.select(QTextCursor::Document);
        ed->setTextCursor(c);
        QKeyEvent ke(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QApplication::sendEvent(ed, &ke);
    }
    pump(30);
    printf("survived enter commit\n");
    check(!a1->isEditing(), "enter commit ends editing");
    check(a1->value == 7, "committed value applied");

    a1->beginEdit();
    pump(30);
    ed = editorOf(a1);
    if (ed) {
        ed->setPlainText("9");
        QKeyEvent ke(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QApplication::sendEvent(ed, &ke);
    }
    pump(30);
    printf("survived escape cancel\n");
    check(!a1->isEditing(), "escape ends editing");
    check(a1->value == 7, "escape keeps old value");

    a1->beginEdit();
    pump(30);
    scene.clear();
    pump(30);
    printf("survived scene teardown while editing\n");
    check(true, "scene teardown during edit does not crash");

    {
        QGraphicsScene scene2;
        QGraphicsView view2(&scene2);
        view2.setAttribute(Qt::WA_DontShowOnScreen, true);
        view2.show();
        QApplication::processEvents();
        PrefixSumModule mod2;
        mod2.buildLayout(&scene2, "4 1\n1 2 3 4\n1 4");
        mod2.enableLiveEditing();
        ArrayBlock* c1 = nullptr;
        ArrayBlock* c2 = nullptr;
        for (QGraphicsItem* item : scene2.items()) {
            ArrayBlock* blk = qgraphicsitem_cast<ArrayBlock*>(item);
            if (!blk || blk->pos().y() >= 300) continue;
            if (blk->id == 1) c1 = blk;
            if (blk->id == 2) c2 = blk;
        }
        c1->beginEdit();
        pump(20);
        QGraphicsTextItem* e1 = editorOf(c1);
        c2->beginEdit();
        pump(20);
        check(c1->isEditing(), "first cell still editing");
        check(c2->isEditing(), "second cell editing");
        if (e1) {
            QFocusEvent fo(QEvent::FocusOut, Qt::ActiveWindowFocusReason);
            QApplication::sendEvent(e1, &fo);
        }
        pump(20);
        printf("survived stale editor focus-out\n");
        check(c2->isEditing(), "stale editor focus-out does not close the active editor");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
