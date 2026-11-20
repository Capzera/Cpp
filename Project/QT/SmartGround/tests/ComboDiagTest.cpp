#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QFontMetrics>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/UiFonts.h"

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    pump(400);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    if (!combo) { printf("FAIL: no combo\n"); return 1; }

    printf("combo width=%d\n", combo->width());
    for (int i = 0; i < combo->count(); ++i) {
        const QString t = combo->itemText(i);
        QFontMetrics fm(UiFonts::forText(t, 10));
        const int need = fm.horizontalAdvance(t);
        const int avail = combo->width() - 12 - 34;
        if (need > avail) printf("  BUTTON-TRUNCATED '%s' need=%d avail=%d\n",
                                t.toUtf8().constData(), need, avail);
    }

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
    if (!popup) { printf("FAIL: popup did not open\n"); return 1; }
    printf("popup = %dx%d at (%d,%d)\n", popup->width(), popup->height(), popup->x(), popup->y());

    QImage shot = popup->grab().toImage();
    shot.save("combo_diag.png");

    QImage full = w.grab().toImage();
    full.save("combo_full.png");

    QImage crop = shot.copy(10, 30, 230, 55).scaled(1150, 275, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    crop.save("combo_tiles.png");

    QImage btn = full.copy(10, 55, 280, 40).scaled(1120, 160, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    btn.save("combo_button.png");

    for (QObject* child : popup->findChildren<QObject*>()) {
        ComboOption* opt = qobject_cast<ComboOption*>(child);
        if (!opt) continue;
        QFontMetrics fm(opt->font());
        const int need = fm.horizontalAdvance(opt->text());
        printf("option '%s' w=%d need=%d %s\n", opt->text().toUtf8().constData(),
               opt->width(), need, opt->width() < need ? "TRUNCATED" : "ok");
    }

    int dark = 0;
    for (int y = 0; y < shot.height(); ++y) {
        for (int x = shot.width() - 3; x < shot.width(); ++x) {
            const QColor c = shot.pixelColor(x, y);
            if (c.red() + c.green() + c.blue() < 720) dark++;
        }
    }
    printf("right-edge nonwhite pixels = %d\n", dark);
    int darkBottom = 0;
    for (int x = 0; x < shot.width(); ++x) {
        for (int y = shot.height() - 3; y < shot.height(); ++y) {
            const QColor c = shot.pixelColor(x, y);
            if (c.red() + c.green() + c.blue() < 720) darkBottom++;
        }
    }
    printf("bottom-edge nonwhite pixels = %d\n", darkBottom);

    printf("--- vertical strip near card right edge ---\n");
    for (int x = shot.width() - 26; x < shot.width(); ++x) {
        const QColor c = shot.pixelColor(x, 150);
        printf("  x=%d rgba=%d,%d,%d,%d\n", x, c.red(), c.green(), c.blue(), c.alpha());
    }
    printf("--- horizontal strip near card bottom edge ---\n");
    for (int y = shot.height() - 26; y < shot.height(); ++y) {
        const QColor c = shot.pixelColor(300, y);
        printf("  y=%d rgba=%d,%d,%d,%d\n", y, c.red(), c.green(), c.blue(), c.alpha());
    }
    printf("--- horizontal strip near card top edge ---\n");
    for (int y = 14; y < 28; ++y) {
        const QColor c = shot.pixelColor(300, y);
        printf("  y=%d rgba=%d,%d,%d,%d\n", y, c.red(), c.green(), c.blue(), c.alpha());
    }

    popup->close();
    return 0;
}
