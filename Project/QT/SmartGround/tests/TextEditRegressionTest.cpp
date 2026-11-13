#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QVBoxLayout>
#include <cstdio>
#include "ui/widgets/MyTextEdit.h"
#include "ui/UiTheme.h"

static int g_fail = 0;

static int darkCount(const QImage& img) {
    int dark = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            if (img.pixelColor(x, y).lightness() < 190) dark++;
    return dark;
}

static QImage grabWidget(QWidget* w) {
    QImage img(w->size(), QImage::Format_ARGB32);
    img.fill(Qt::white);
    w->render(&img);
    return img;
}

static int grayHintCount(const QImage& img) {
    const QColor muted = UiTheme::textMuted();
    int n = 0;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (qAbs(c.red() - muted.red()) <= 26 && qAbs(c.green() - muted.green()) <= 26 &&
                qAbs(c.blue() - muted.blue()) <= 26) n++;
        }
    return n;
}

static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget host;
    host.resize(420, 460);
    QVBoxLayout* lay = new QVBoxLayout(&host);

    MyTextEdit* empty = new MyTextEdit(&host);
    empty->setPlaceholderText("请输入数据");
    lay->addWidget(empty);

    MyTextEdit* multi = new MyTextEdit(&host);
    multi->setPlaceholderText("第一行输入两个正整数\n接下来 m 行，每行两个整数");
    lay->addWidget(multi);

    MyTextEdit* filled = new MyTextEdit(&host);
    filled->setPlaceholderText("请输入数据");
    filled->setText("7 8 1\n1 2\n1 3");
    lay->addWidget(filled);

    MyTextEdit* cleared = new MyTextEdit(&host);
    cleared->setPlaceholderText("请输入数据");
    cleared->setText("临时内容");
    lay->addWidget(cleared);

    host.show();
    QApplication::processEvents();

    const int emptyDark = darkCount(grabWidget(empty));
    check("empty shows placeholder", emptyDark > 60);
    printf("    empty dark=%d hint=%d\n", emptyDark, grayHintCount(grabWidget(empty)));

    const int multiDark = darkCount(grabWidget(multi));
    check("multi-line placeholder visible", grayHintCount(grabWidget(multi)) > grayHintCount(grabWidget(empty)));
    printf("    multi dark=%d hint=%d\n", multiDark, grayHintCount(grabWidget(multi)));

    const int filledDark = darkCount(grabWidget(filled));
    check("typed text visible", filledDark > 100);
    check("no placeholder when text present", grayHintCount(grabWidget(filled)) < 40);
    printf("    filled dark=%d hint=%d\n", filledDark, grayHintCount(grabWidget(filled)));

    cleared->clear();
    QApplication::processEvents();
    const int clearedDark = darkCount(grabWidget(cleared));
    check("placeholder returns after clear", grayHintCount(grabWidget(cleared)) > 60);
    printf("    cleared dark=%d hint=%d\n", clearedDark, grayHintCount(grabWidget(cleared)));

    filled->setFocus();
    QApplication::processEvents();
    filled->insertPlainText("9");
    QApplication::processEvents();
    check("focus + typing works", filled->toPlainText().startsWith("9"));
    printf("    filled text=%s\n", filled->toPlainText().toUtf8().constData());

    const QImage hostImg = host.grab().toImage();
    check("host render non-blank", darkCount(hostImg) > 200);
    hostImg.save("textedit_regression.png");

    empty->setPlaceholderText("新的提示文字");
    QApplication::processEvents();
    check("placeholder updates live", grayHintCount(grabWidget(empty)) > 60);

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
