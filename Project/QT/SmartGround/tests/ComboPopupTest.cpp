#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScreen>
#include <cstdio>
#include "ui/widgets/MyComboBox.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void clickWidget(QWidget* w) {
    QPointF center(w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, center, w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)),
                      Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, center, w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)),
                        Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

static void pressKey(QWidget* w, int key) {
    QKeyEvent press(QEvent::KeyPress, key, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QApplication::processEvents();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MyComboBox combo;
    combo.addItem("请选择...");
    combo.addItems({"冒泡排序", "广度优先搜索", "Kruskal 最小生成树"});
    combo.resize(200, 34);
    combo.show();
    combo.raise();
    QApplication::processEvents();

    check(combo.count() == 4, "combo has 4 items");
    check(combo.currentIndex() == 0, "combo starts at index 0");
    check(combo.currentText() == "请选择...", "combo current text correct");

    clickWidget(&combo);
    check(combo.isPopupOpen(), "popup OPENS on click");

    QWidget* popup = nullptr;
    for (QWidget* w : QApplication::topLevelWidgets()) {
        if (w->isWindow() && w->windowType() == Qt::Popup && w != &combo) { popup = w; break; }
    }
    check(popup != nullptr, "popup window exists as Qt::Popup");
    if (popup) {
        check(popup->isVisible(), "popup window is visible");
        check(popup->geometry().height() > 50, "popup has non-trivial height");
        printf("popup geom: %dx%d at (%d,%d)\n", popup->geometry().width(), popup->geometry().height(),
               popup->geometry().x(), popup->geometry().y());
    }

    pressKey(&combo, Qt::Key_Down);
    check(combo.currentIndex() == 1, "keyboard Down selects index 1");
    check(combo.currentText() == "冒泡排序", "keyboard Down updates text");

    clickWidget(&combo);
    check(!combo.isPopupOpen(), "click on combo toggles popup closed");
    clickWidget(&combo);
    check(combo.isPopupOpen(), "popup re-opens after toggle");
    pressKey(&combo, Qt::Key_Escape);
    check(!combo.isPopupOpen(), "Escape closes popup");

    combo.setCurrentIndex(3);
    QApplication::processEvents();
    check(combo.currentText() == "Kruskal 最小生成树", "setCurrentIndex by code works");

    clickWidget(&combo);
    check(combo.isPopupOpen(), "popup opens after selection");
    clickWidget(&combo);
    check(!combo.isPopupOpen(), "second click on combo closes popup");

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
