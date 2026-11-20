#include <QApplication>
#include <QImage>
#include <QTimer>
#include <QEventLoop>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MySegmentedControl.h"

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

static void analyze(const QPixmap& pix, bool expectPanel, int y0, int y1) {
    QImage img = pix.toImage();
    if (img.isNull()) { printf("FAIL: screenshot is null\n"); failures++; return; }
    int accent = 0;
    int lightBlue = 0;
    int panelBox = 0;
    for (int y = qMax(0, y0); y < qMin(y1, img.height()); ++y) {
        for (int x = 10; x < 270 && x < img.width(); ++x) {
            QColor c = img.pixelColor(x, y);
            if (c.blue() > 170 && c.red() < 130 && c.green() < 160) accent++;
            if (c.blue() > 235 && c.red() > 225 && c.green() > 230) lightBlue++;
            if (std::abs(c.red() - 246) <= 1 && std::abs(c.green() - 248) <= 1 && std::abs(c.blue() - 251) <= 1) panelBox++;
        }
    }
    printf("accent=%d lightBlue=%d panelSurface=%d (band %d..%d)\n", accent, lightBlue, panelBox, y0, y1);
    if (!expectPanel) {
        check(panelBox < 300, "no graph panel surface box in left column");
        check(accent < 200, "no accent-tinted block in left column at startup");
    } else {
        check(panelBox > 800, "graph panel surface present in left column");
    }
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1400, 860);
    w.show();
    pump(500);

    MySegmentedControl* picker = w.findChild<MySegmentedControl*>("comboStylePicker");
    check(picker == nullptr, "style picker removed from left column");

    int bandTop = 87;
    int bandBottom = 300;

    analyze(w.grab(), false, bandTop, bandBottom);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    if (combo) {
        for (int i = 1; i < combo->count(); ++i) {
            if (combo->itemText(i).contains("广度优先")) { combo->setCurrentIndex(i); break; }
        }
        pump(400);
    }
    analyze(w.grab(), true, bandTop, bandBottom);

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
