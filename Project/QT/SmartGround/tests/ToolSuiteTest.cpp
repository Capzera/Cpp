#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QMouseEvent>
#include <cstdio>
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/ToolIconButton.h"
#include "canvas/AnnotationScene.h"
#include "canvas/ToolController.h"
#include "canvas/CanvasView.h"

static int failures = 0;

static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void clickWidget(QWidget* w) {
    const QPointF g(w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2)));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
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

static int countAccentPixels(const QImage& img, int x0, int y0, int x1, int y1) {
    int count = 0;
    for (int y = y0; y < qMin(y1, img.height()); ++y) {
        for (int x = x0; x < qMin(x1, img.width()); ++x) {
            const QColor c = img.pixelColor(x, y);
            if (qAbs(c.red() - 59) < 24 && qAbs(c.green() - 125) < 24 && qAbs(c.blue() - 221) < 24) count++;
        }
    }
    return count;
}

static void testComboHeaderText() {
    MyComboBox combo;
    combo.addItem("请选择...");
    combo.addGroup("基础结构", {"栈", "队列", "并查集"});
    combo.addGroup("数学基础", {"前缀和", "差分"});
    combo.resize(240, 34);
    combo.move(40, 40);
    combo.show();
    QApplication::processEvents();

    clickWidget(&combo);

    QWidget* popup = nullptr;
    for (QWidget* w : QApplication::topLevelWidgets()) {
        if (w->isWindow() && w->windowType() == Qt::Popup && w != &combo) { popup = w; break; }
    }
    check(popup != nullptr, "combo popup opened");
    if (!popup) return;

    QImage shot = popup->grab().toImage();
    shot.save("tool_suite_combo.png");

    const int accent = countAccentPixels(shot, 0, 0, shot.width(), shot.height());
    const int dark = countDarkPixels(shot, 0, 0, shot.width(), shot.height());
    printf("combo popup %dx%d accent=%d dark=%d\n", shot.width(), shot.height(), accent, dark);
    check(accent > 40, "combo popup renders section accent bars");
    check(dark > 200, "combo popup renders visible section title text (dark pixels present)");

    QWidget* content = popup->findChild<QWidget*>("canvasNothing");
    Q_UNUSED(content);
}

static void testToolSuite() {
    AnnotationScene scene;
    CanvasView view(&scene);
    view.resize(1200, 800);
    view.show();
    QApplication::processEvents();

    ToolController controller(&scene, &view);
    CanvasToolBar bar(&controller, &view);
    bar.setHostWidget(&view);
    bar.show();
    bar.raise();
    QApplication::processEvents();

    const int barW = bar.mainBarWidth();
    printf("tool suite main bar width = %d\n", barW);
    check(barW > 0 && barW < 520, "floating tool suite is compact (< 520px)");

    const int barH = bar.height();
    check(barH > 0 && barH < 80, "floating tool suite is a single short row");

    QImage shot = view.grab().toImage();
    shot.save("tool_suite_canvas.png");

    ToolIconButton* styleBtn = view.findChild<ToolIconButton*>("canvasStyleToggle");
    check(styleBtn != nullptr, "style toggle button exists in suite");
    if (styleBtn) {
        clickWidget(styleBtn);
        QApplication::processEvents();
        check(bar.isStylePanelOpen(), "style flyout opens");
        QImage shot2 = view.grab().toImage();
        shot2.save("tool_suite_style.png");
        clickWidget(styleBtn);
        QApplication::processEvents();
        check(!bar.isStylePanelOpen(), "style flyout closes");
    }

    ToolIconButton* moreBtn = view.findChild<ToolIconButton*>("canvasMoreToggle");
    check(moreBtn != nullptr, "more toggle button exists in suite");
    if (moreBtn) {
        clickWidget(moreBtn);
        QApplication::processEvents();
        check(bar.isMorePanelOpen(), "more flyout opens");
        QImage shot3 = view.grab().toImage();
        shot3.save("tool_suite_more.png");
    }
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    testComboHeaderText();
    testToolSuite();
    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
