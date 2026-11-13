#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <cstdio>
#include "ui/widgets/MyComboBox.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void clickWidget(QWidget* w) {
    QPointF g = w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MyComboBox combo;
    combo.addItem("请选择...");
    combo.addItems({"冒泡排序", "广度优先搜索", "Tarjan 强连通分量", "Kruskal 最小生成树"});
    combo.resize(220, 34);
    combo.move(40, 40);
    combo.show();
    QApplication::processEvents();

    combo.grab().save("combo_closed.png");

    clickWidget(&combo);
    check(combo.isPopupOpen(), "popup open for screenshot");

    QWidget* popup = nullptr;
    for (QWidget* w : QApplication::topLevelWidgets()) {
        if (w->isWindow() && w->windowType() == Qt::Popup && w != &combo) { popup = w; break; }
    }
    if (popup) {
        QPixmap pm = popup->grab();
        pm.save("combo_popup.png");
        QImage img = pm.toImage();
        int textDark = 0;
        int selectedBg = 0;
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                QColor c = img.pixelColor(x, y);
                if (c.lightness() < 120) textDark++;
                if (std::abs(c.red() - 233) < 8 && std::abs(c.green() - 241) < 8 && std::abs(c.blue() - 253) < 8) selectedBg++;
            }
        }
        printf("popup image %dx%d textDark=%d selectedBg=%d\n", img.width(), img.height(), textDark, selectedBg);
        check(img.width() > 100 && img.height() > 80, "popup image has real size");
        check(textDark > 100, "popup renders visible item text");
        check(selectedBg > 200, "popup renders accent-soft highlight for selected row");
    } else {
        check(false, "popup window found for screenshot");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
