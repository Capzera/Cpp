#include <QApplication>
#include <QAbstractButton>
#include <QTimer>
#include <QEventLoop>
#include <QSettings>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/ToolIconButton.h"
#include "ui/widgets/CanvasBrushSlider.h"
#include "ui/widgets/MySegmentedControl.h"
#include "ui/widgets/MyToggleSwitch.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"
#include "canvas/AnnotationScene.h"
#include "canvas/ShapeItem.h"

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
    if (!w) return;
    const QPointF g = w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

static void clickButton(QAbstractButton* w) {
    if (!w) return;
    const QPointF g = w->mapToGlobal(QPoint(w->width() / 2, w->height() / 2));
    QMouseEvent press(QEvent::MouseButtonPress, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &press);
    QMouseEvent release(QEvent::MouseButtonRelease, g, g, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(w, &release);
    QApplication::processEvents();
}

static QWidget* findByName(QWidget* root, const char* name) {
    for (QWidget* w : root->findChildren<QWidget*>()) {
        if (w->objectName() == QLatin1String(name)) return w;
    }
    return nullptr;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget host;
    host.resize(1200, 800);
    host.show();

    AnnotationScene scene;
    CanvasView view(&scene);
    view.resize(1200, 800);
    view.show();

    ToolController controller(&scene, &view);
    CanvasToolBar bar(&controller, &host);
    bar.setHostWidget(&host);
    bar.resize(600, 60);
    bar.move(300, 40);
    CanvasBrushSlider slider(&controller, &view);
    slider.setHostWidget(&view);
    pump(250);

    printf("--- feature 1: floating brush slider ---\n");
    controller.setTool(ToolController::Select);
    pump(150);
    check("slider hidden for select tool", !slider.isVisible());
    controller.setTool(ToolController::Pen);
    pump(150);
    check("slider visible for pen tool", slider.isVisible());
    controller.setTool(ToolController::Highlighter);
    pump(150);
    check("slider visible for highlighter tool", slider.isVisible());
    controller.setTool(ToolController::Eraser);
    pump(150);
    check("slider visible for eraser tool", slider.isVisible());

    controller.setTool(ToolController::Pen);
    pump(150);
    const int before = qRound(controller.penWidth());
    slider.setFocus();
    QWheelEvent wheel(QPointF(50, 50), QPointF(50, 50), QPoint(), QPoint(0, 120), Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QApplication::sendEvent(&slider, &wheel);
    pump(80);
    const int after = qRound(controller.penWidth());
    printf("  pen width %d -> %d\n", before, after);
    check("slider wheel changes pen width", after != before);

    controller.setTool(ToolController::Eraser);
    pump(120);
    const int eraserBefore = qRound(controller.eraserRadius());
    QWheelEvent wheel2(QPointF(50, 50), QPointF(50, 50), QPoint(), QPoint(0, -120), Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QApplication::sendEvent(&slider, &wheel2);
    pump(80);
    printf("  eraser radius %d -> %d\n", eraserBefore, qRound(controller.eraserRadius()));
    check("slider wheel changes eraser radius", qRound(controller.eraserRadius()) != eraserBefore);

    printf("--- feature 2: right-drag erase preference ---\n");
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                           QStringLiteral("SmartGround"), QStringLiteral("SmartGround"));
        settings.setValue("canvas/rightClickErase", true);
        settings.sync();
    }
    ToolController fresh(&scene, &view);
    check("right erase defaults to on", fresh.rightClickErasePreference());
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                           QStringLiteral("SmartGround"), QStringLiteral("SmartGround"));
        settings.setValue("canvas/rightClickErase", false);
        settings.sync();
    }
    ToolController fresh2(&scene, &view);
    check("right erase preference is remembered", !fresh2.rightClickErasePreference());    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                           QStringLiteral("SmartGround"), QStringLiteral("SmartGround"));
        settings.setValue("canvas/rightClickErase", true);
        settings.sync();
    }

    controller.setTool(ToolController::Eraser);
    pump(150);
    ToolIconButton* eraserButton = bar.toolButtonFor(static_cast<int>(ToolController::Eraser));
    check("eraser button found", eraserButton != nullptr);
    if (eraserButton) {
        clickButton(eraserButton);
        pump(250);
    }    QWidget* panel = bar.toolPanel();
    check("eraser panel visible", panel && panel->isVisible());
    MyToggleSwitch* rightSwitch = qobject_cast<MyToggleSwitch*>(findByName(panel, "canvasToolRightClickErase"));
    check("right erase toggle exists in eraser panel", rightSwitch != nullptr);
    if (rightSwitch) {
        rightSwitch->setChecked(false);
        pump(120);
        check("toggling right erase updates controller", !controller.rightClickEraseEnabled());
        rightSwitch->setChecked(true);
        pump(120);
        check("toggling right erase back restores controller", controller.rightClickEraseEnabled());
    }

    printf("--- feature 3: hollow / solid shape switcher ---\n");
    controller.setTool(ToolController::Rect);
    pump(200);
    QWidget* rectPanel = bar.toolPanel();
    MySegmentedControl* fillPicker = qobject_cast<MySegmentedControl*>(findByName(rectPanel, "canvasToolShapeFill"));
    check("shape fill switcher exists for rect", fillPicker != nullptr);
    if (fillPicker) {
        check("shape fill switcher has hollow and solid entries", fillPicker->options().size() == 2);
        fillPicker->setCurrentIndex(1);
        pump(120);
        ToolController::ToolStyle style = controller.styleFor(ToolController::Rect);
        check("solid selection recorded in tool style", style.filled);
        fillPicker->setCurrentIndex(0);
        pump(120);
        style = controller.styleFor(ToolController::Rect);
        check("hollow selection recorded in tool style", !style.filled);
    }
    controller.setTool(ToolController::Circle);
    pump(200);
    MySegmentedControl* circlePicker = qobject_cast<MySegmentedControl*>(findByName(bar.toolPanel(), "canvasToolShapeFill"));
    check("shape fill switcher exists for circle", circlePicker != nullptr);
    controller.setTool(ToolController::Triangle);
    pump(200);
    MySegmentedControl* triPicker = qobject_cast<MySegmentedControl*>(findByName(bar.toolPanel(), "canvasToolShapeFill"));
    check("shape fill switcher exists for triangle", triPicker != nullptr);
    if (fillPicker) {
        check("rect fill switcher shows rect icons", fillPicker->shapeIcons().size() == 2);
    }
    if (circlePicker) {
        check("circle fill switcher shows circle icons", circlePicker->shapeIcons().size() == 2);
    }

    printf("--- feature 4: hollow / solid canvas rendering ---\n");
    {
        QImage hollowImg(40, 40, QImage::Format_ARGB32);
        hollowImg.fill(Qt::transparent);
        {
            QPainter p(&hollowImg);
            ShapeItem hollow(ShapeItem::Rectangle);
            hollow.setGeometry(QPointF(2, 2), QPointF(38, 38));
            hollow.setPenColor(QColor(20, 30, 45));
            hollow.setPenWidth(2.0);
            hollow.setFilled(false);
            hollow.paint(&p, nullptr, nullptr);
        }
        int opaqueInside = 0;
        for (int y = 8; y < 32; ++y) {
            for (int x = 8; x < 32; ++x) {
                if (qAlpha(hollowImg.pixel(x, y)) > 0) opaqueInside++;
            }
        }
        check("hollow shape interior is fully transparent", opaqueInside == 0);

        QImage solidImg(40, 40, QImage::Format_ARGB32);
        solidImg.fill(Qt::transparent);
        {
            QPainter p(&solidImg);
            ShapeItem solid(ShapeItem::Rectangle);
            solid.setGeometry(QPointF(2, 2), QPointF(38, 38));
            solid.setPenColor(QColor(20, 30, 45));
            solid.setPenWidth(2.0);
            solid.setFilled(true);
            solid.paint(&p, nullptr, nullptr);
        }
        int opaqueSolid = 0;
        for (int y = 8; y < 32; ++y) {
            for (int x = 8; x < 32; ++x) {
                if (qAlpha(solidImg.pixel(x, y)) > 200) opaqueSolid++;
            }
        }
        check("solid shape interior is fully filled", opaqueSolid == (24 * 24));
    }

    printf("--- feature 5: hollow / solid segmented icons ---\n");
    {
        const QRectF box(0.0, 0.0, 13.0, 13.0);
        const QColor ink(20, 30, 45);

        QImage hollowImg(14, 14, QImage::Format_ARGB32);
        hollowImg.fill(Qt::transparent);
        {
            QPainter p(&hollowImg);
            MySegmentedControl::paintShapeIcon(&p, MySegmentedControl::RectIcon, box, ink, false);
        }
        QImage solidImg(14, 14, QImage::Format_ARGB32);
        solidImg.fill(Qt::transparent);
        {
            QPainter p(&solidImg);
            MySegmentedControl::paintShapeIcon(&p, MySegmentedControl::RectIcon, box, ink, true);
        }

        auto countInk = [&](const QImage& img, bool interiorOnly) {
            int n = 0;
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    const bool border = (x <= 1 || x >= img.width() - 2 || y <= 1 || y >= img.height() - 2);
                    if (interiorOnly && border) continue;
                    if (qAlpha(img.pixel(x, y)) > 40) n++;
                }
            }
            return n;
        };

        const int hollowEdge = countInk(hollowImg, false);
        const int hollowInner = countInk(hollowImg, true);
        const int solidEdge = countInk(solidImg, false);
        const int solidInner = countInk(solidImg, true);

        check("hollow icon draws a border", hollowEdge > 0);
        check("hollow icon has transparent interior", hollowInner == 0);
        check("solid icon fills its interior", solidInner > 0);
        check("solid icon is denser than hollow icon", solidEdge > hollowEdge);
    }

    printf("\n%s  fail=%d\n", g_fail == 0 ? "ALL PASS" : "HAS FAILURE", g_fail);
    return g_fail == 0 ? 0 : 1;
}
