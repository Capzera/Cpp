#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QMouseEvent>
#include <QPixmap>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static int countDarkPixels(const QImage& img, int x0, int y0, int x1, int y1) {
    int count = 0;
    for (int y = y0; y < qMin(y1, img.height()); ++y) {
        for (int x = x0; x < qMin(x1, img.width()); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (c.red() + c.green() + c.blue() < 330) count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    pump(400);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    if (!combo) { printf("FAIL: no combo\n"); return 1; }

    const QPointF g(combo->mapToGlobal(QPoint(combo->width() / 2, combo->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(combo, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(combo, &release);
    pump(400);

    QWidget* popup = nullptr;
    for (QWidget* top : QApplication::topLevelWidgets()) {
        if (top->isWindow() && top->windowType() == Qt::Popup && top != combo) { popup = top; break; }
    }

    int failures = 0;
    if (!popup) {
        printf("FAIL: real app dropdown popup did not open\n");
        return 1;
    }
    printf("popup = %dx%d at (%d,%d)\n", popup->width(), popup->height(), popup->x(), popup->y());

    QImage shot = popup->grab().toImage();
    shot.save("app_combo_popup.png");

    const int dark = countDarkPixels(shot, 0, 0, shot.width(), 120);
    printf("dark pixels in header band = %d\n", dark);
    if (dark < 300) {
        printf("FAIL: category titles still not visible in real app dropdown\n");
        failures++;
    } else {
        printf("PASS: category titles visible in real app dropdown\n");
    }

    popup->close();
    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
