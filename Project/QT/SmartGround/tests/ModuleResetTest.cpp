#include <QApplication>
#include <QTimer>
#include <QComboBox>
#include <QPushButton>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyTextEdit.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    check(combo != nullptr, "combo box found");
    check(combo->count() >= 2, "combo has placeholder + modules");

    combo->setCurrentIndex(1);
    QApplication::processEvents();
    QGraphicsScene* scene = w.findChild<QGraphicsScene*>();
    check(scene != nullptr, "scene found");

    MyButton* buildBtn = nullptr;
    for (MyButton* b : w.findChildren<MyButton*>()) {
        if (b->text().contains("绘制")) buildBtn = b;
    }
    check(buildBtn != nullptr, "build button found");
    if (buildBtn) {
        buildBtn->click();
        QApplication::processEvents();
    }
    check(scene && !scene->items().isEmpty(), "selecting a module + drawing renders content");

    combo->setCurrentIndex(0);
    QApplication::processEvents();
    check(scene->items().isEmpty(), "switching to 请选择... clears the canvas");

    MyTextEdit* edit = w.findChild<MyTextEdit*>();
    check(edit != nullptr, "input edit found");
    check(edit->toPlainText().isEmpty(), "input box cleared on reset");
    check(!edit->placeholderText().isEmpty(), "placeholder restored on reset");

    combo->setCurrentIndex(2);
    QApplication::processEvents();
    MyTextEdit* edit2 = w.findChild<MyTextEdit*>();
    if (edit2) {
        edit2->setText("");
        QApplication::processEvents();
        check(edit2->toPlainText().isEmpty(), "input can be emptied");
        check(!edit2->placeholderText().isEmpty(), "placeholder present when emptied");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
