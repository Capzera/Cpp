#include <QApplication>
#include <QImage>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <cstdio>
#include "ui/widgets/MyColorPicker.h"

static int g_fail = 0;
static void check(const char* name, bool ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) g_fail++;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MyColorPicker picker;
    picker.resize(244, 430);
    picker.setColor(QColor(229, 72, 77));
    picker.show();
    app.processEvents();

    ColorField* field = picker.findChild<ColorField*>("colorField");
    check("color field widget exists", field != nullptr);

    ColorPreview* preview = picker.findChild<ColorPreview*>("colorPreview");
    check("preview widget exists", preview != nullptr);

    if (field) {
        const QImage fs = field->grab().toImage();
        const int y = fs.height() - 7;
        const QColor left = fs.pixel(3, y);
        const QColor mid = fs.pixel(fs.width() / 2, y);
        const QColor magenta = fs.pixel(static_cast<int>(fs.width() * 0.83), y);
        const QColor right = fs.pixel(fs.width() - 4, y);
        printf("hue bar left=%s mid=%s magenta=%s right=%s\n", left.name().toLatin1().constData(),
               mid.name().toLatin1().constData(), magenta.name().toLatin1().constData(),
               right.name().toLatin1().constData());
        check("hue bar left is red-ish", left.red() > left.green() + 40);
        check("hue bar mid is green-ish", mid.green() > mid.red() + 20);
        check("hue bar passes through magenta", magenta.red() > 100 && magenta.blue() > 100);
        check("hue bar wraps back to red at the right end", right.red() > right.green() + 40);
        check("hue bar is a real gradient, not one solid color", left != mid && mid != magenta);
    }

    if (preview) {
        const QImage ps = preview->grab().toImage();
        const QColor c = ps.pixel(ps.width() / 2, ps.height() / 2);
        printf("preview color=%s\n", c.name().toLatin1().constData());
        check("preview shows the current color, not black", c != QColor(0, 0, 0));
    }

    bool hasPreset = false;
    bool hasOld = false;
    for (QLabel* l : picker.findChildren<QLabel*>()) {
        if (l->text() == QString::fromUtf8("预设")) hasPreset = true;
        if (l->text() == QString::fromUtf8("渐变预设")) hasOld = true;
    }
    check("label renamed to 预设", hasPreset);
    check("old 渐变预设 label is gone", !hasOld);

    QColor picked;
    QObject::connect(&picker, &MyColorPicker::colorPicked, [&](const QColor& c) { picked = c; });

    if (field) {
        const QPointF huePoint(field->width() * 0.35, field->height() - 7.0);
        QMouseEvent press(QEvent::MouseButtonPress, huePoint, huePoint, Qt::LeftButton,
                          Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(field, &press);
        QMouseEvent release(QEvent::MouseButtonRelease, huePoint, huePoint, Qt::LeftButton,
                            Qt::MouseButtons(), Qt::NoModifier);
        QApplication::sendEvent(field, &release);
        app.processEvents();
        printf("after hue drag picked=%s\n", picked.name().toLatin1().constData());
        check("dragging the hue bar picks a new color", picked.isValid() && picked != QColor(229, 72, 77));
    }

    printf("\nfailures=%d\n", g_fail);
    return g_fail;
}
