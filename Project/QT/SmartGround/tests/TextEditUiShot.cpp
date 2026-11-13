#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QImage>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyTextEdit.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static void pickModule(MainWindow& w, const QString& keyword) {
    MyComboBox* combo = w.findChild<MyComboBox*>();
    for (int i = 1; i < combo->count(); ++i) {
        if (combo->itemText(i).contains(keyword)) { combo->setCurrentIndex(i); break; }
    }
    pump(250);
}

static int grayHintCount(const QImage& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (qAbs(c.red() - 154) <= 26 && qAbs(c.green() - 164) <= 26 && qAbs(c.blue() - 180) <= 26) n++;
        }
    return n;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 860);
    w.show();
    pump(400);

    MyTextEdit* input = w.findChild<MyTextEdit*>();
    printf("found textInput=%d\n", input != nullptr);

    QImage start = input->grab().toImage();
    printf("start   text='%s' hint=%d\n", input->toPlainText().toUtf8().constData(), grayHintCount(start));
    w.grab().save("ui_input_start.png");

    pickModule(w, "广度优先");
    QImage withData = input->grab().toImage();
    printf("bfs     text='%s' hint=%d\n", input->toPlainText().left(20).toUtf8().constData(), grayHintCount(withData));
    w.grab().save("ui_input_bfs.png");

    input->clear();
    pump(200);
    QImage cleared = input->grab().toImage();
    printf("cleared text='%s' hint=%d\n", input->toPlainText().toUtf8().constData(), grayHintCount(cleared));
    w.grab().save("ui_input_cleared.png");

    input->setFocus();
    pump(100);
    input->insertPlainText("7 8 1");
    pump(200);
    QImage typed = input->grab().toImage();
    printf("typed   text='%s' hint=%d\n", input->toPlainText().toUtf8().constData(), grayHintCount(typed));
    w.grab().save("ui_input_typed.png");

    pickModule(w, "冒泡");
    QImage sort = input->grab().toImage();
    printf("bubble  text='%s' hint=%d\n", input->toPlainText().left(20).toUtf8().constData(), grayHintCount(sort));
    w.grab().save("ui_input_sort.png");

    return 0;
}
