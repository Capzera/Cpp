#include <QApplication>
#include <QTimer>
#include <QEventLoop>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QWidget>
#include <QScreen>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"

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

static void clickWidget(QWidget* w) {
    const QPointF g(w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

static int dropAt(const QImage& img, int x, int y) {
    const QColor c = img.pixelColor(x, y);
    const QColor l = img.pixelColor(x - 4, y);
    return (l.red() + l.green() + l.blue()) - (c.red() + c.green() + c.blue());
}

static int dropAtV(const QImage& img, int x, int y) {
    const QColor c = img.pixelColor(x, y);
    const QColor l = img.pixelColor(x, y - 4);
    return (l.red() + l.green() + l.blue()) - (c.red() + c.green() + c.blue());
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);
    if (QGuiApplication::platformName() == QLatin1String("offscreen")) {
        printf("[SKIP] needs a real screen surface to capture the popup\n");
        return 0;
    }
    MainWindow w;
    w.resize(1500, 950);
    w.move(120, 60);
    w.show();
    w.raise();
    w.activateWindow();
    pump(900);

    MyComboBox* combo = w.findChild<MyComboBox*>();
    clickWidget(combo);
    pump(700);

    QWidget* popup = nullptr;
    for (QWidget* top : QApplication::topLevelWidgets()) {
        if (top->isWindow() && top->windowType() == Qt::Popup && top != combo) { popup = top; break; }
    }
    check("popup opened", popup != nullptr);
    if (!popup) { printf("fail=%d\n", g_fail); return 1; }

    check("popup has NoDropShadowWindowHint",
          (popup->windowFlags() & Qt::NoDropShadowWindowHint) != 0);
    check("popup is frameless",
          (popup->windowFlags() & Qt::FramelessWindowHint) != 0);
    check("popup is translucent",
          popup->testAttribute(Qt::WA_TranslucentBackground));

    const QPoint gp = popup->mapToGlobal(QPoint(0, 0));
    popup->raise();
    pump(400);

    QScreen* screen = popup->screen() ? popup->screen() : QGuiApplication::primaryScreen();
    QImage frame = screen->grabWindow(0).toImage();

    const qreal scale = frame.width() / qreal(screen->geometry().width());
    const int px = qRound(gp.x() * scale);
    const int py = qRound(gp.y() * scale);
    const int pw = qRound(popup->width() * scale);
    const int ph = qRound(popup->height() * scale);
    const int winRight = px + pw;
    const int winBottom = py + ph;

    ComboCard* card = popup->findChild<ComboCard*>();
    check("card found", card != nullptr);
    const int cardRight = card ? px + qRound((card->geometry().x() + card->geometry().width()) * scale) : winRight;
    const int cardBottom = card ? py + qRound((card->geometry().y() + card->geometry().height()) * scale) : winBottom;

    int hardV = 0;
    for (int x = cardRight + 8; x < winRight + 24; ++x) {
        int darker = 0;
        for (int y = py + 60; y < winBottom - 60; ++y) {
            if (dropAt(frame, x, y) > 30) ++darker;
        }
        if (darker > 120) {
            printf("  hard vertical line at x=%d (offset %+d from window right, darker=%d)\n",
                   x, x - winRight, darker);
            ++hardV;
        }
    }
    check("no hard vertical line in card right margin", hardV == 0);

    int hardH = 0;
    for (int y = cardBottom + 8; y < winBottom + 24; ++y) {
        int darker = 0;
        for (int x = px + 60; x < winRight - 60; ++x) {
            if (dropAtV(frame, x, y) > 30) ++darker;
        }
        if (darker > 120) {
            printf("  hard horizontal line at y=%d (offset %+d from window bottom, darker=%d)\n",
                   y, y - winBottom, darker);
            ++hardH;
        }
    }
    check("no hard horizontal line in card bottom margin", hardH == 0);

    popup->close();
    printf("\n%s fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
