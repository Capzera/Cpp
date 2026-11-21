#include <QApplication>
#include <QTimer>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMetaObject>
#include <typeinfo>
#include <QLabel>
#include <QPushButton>
#include <QPointF>
#include <cstdio>
#include "ui/MainWindow.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/CanvasToolBar.h"
#include "ui/widgets/CartesianPanel.h"
#include "ui/widgets/ToolIconButton.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyTextInput.h"
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"
#include "canvas/CanvasView.h"
#include "canvas/ToolController.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static int findCartesianIndex(MyComboBox* combo) {
    for (int i = 0; i < combo->count(); ++i) {
        if (combo->itemText(i).contains("笛卡尔坐标系")) return i;
    }
    return -1;
}

static QGraphicsScene* graphicsSceneOf(MainWindow& w) {
    for (CanvasView* view : w.findChildren<CanvasView*>()) {
        if (view->scene()) return view->scene();
    }
    return nullptr;
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    MainWindow w;
    w.resize(1400, 860);
    w.show();
    QApplication::processEvents();

    MyComboBox* combo = w.findChild<MyComboBox*>();
    const int index = findCartesianIndex(combo);
    printf("cartesian index = %d\n", index);
    combo->setCurrentIndex(index);
    pump(300);

    printf("\n--- issue 1: where is the point tool ---\n");
    CanvasToolBar* toolbar = w.findChild<CanvasToolBar*>();
    ToolIconButton* pointButton = toolbar->cartesianButtonFor(static_cast<int>(ToolIconButton::CartPoint));
    printf("pointButton = %p\n", (void*)pointButton);
    if (pointButton) {
        printf("  button visible = %d\n", pointButton->isVisible());
        printf("  button geometry in toolbar = %d,%d %dx%d\n",
               pointButton->geometry().x(), pointButton->geometry().y(),
               pointButton->geometry().width(), pointButton->geometry().height());
        QWidget* row = pointButton->parentWidget();
        printf("  row = %p name = %s visible = %d\n", (void*)row,
               row->objectName().toUtf8().constData(), row->isVisible());
        if (row) {
            printf("  row geometry in toolbar = %d,%d %dx%d\n",
                   row->geometry().x(), row->geometry().y(),
                   row->geometry().width(), row->geometry().height());
        }
        QWidget* mainRow = toolbar->findChild<QWidget*>("canvasToolMainRow");
        if (mainRow) {
            printf("  mainRow geometry = %d,%d %dx%d\n",
                   mainRow->geometry().x(), mainRow->geometry().y(),
                   mainRow->geometry().width(), mainRow->geometry().height());
        }
        printf("  toolbar geometry = %d,%d %dx%d\n",
               toolbar->geometry().x(), toolbar->geometry().y(),
               toolbar->geometry().width(), toolbar->geometry().height());
        bool overlaps = mainRow && mainRow->geometry().intersects(pointButton->geometry());
        printf("  OVERLAPS MAIN ROW = %d\n", overlaps ? 1 : 0);
    }

    printf("\n--- issue 2: left panel data input ---\n");
    CartesianPanel* panel = w.findChild<CartesianPanel*>();
    printf("  cartesian panel visible = %d\n", panel ? panel->isVisible() : -1);
    for (QWidget* widget : w.findChildren<QWidget*>()) {
        const QString name = widget->objectName();
        if (name == "gridToggle" || name == "scriptTextToggle") {
            printf("  %s visible = %d\n", name.toUtf8().constData(), widget->isVisible());
        }
    }
    {
        QWidget* editor = nullptr;
        for (QWidget* widget : w.findChildren<QWidget*>()) {
            if (widget->objectName() == "myTextEdit" || widget->objectName() == "myTextEditRoot") {
                editor = widget;
                break;
            }
        }
        if (!editor) {
            for (QTextEdit* te : w.findChildren<QTextEdit*>()) { editor = te; break; }
        }
        printf("  text editor = %p visible = %d\n", (void*)editor,
               editor ? editor->isVisible() : -1);
        for (MyButton* b : w.findChildren<MyButton*>()) {
            const QString t = b->text();
            if (t.contains("绘制") || t.contains("播放") || t.contains("下一步")) {
                printf("    button '%s' visible = %d\n", t.toUtf8().constData(), b->isVisible());
            }
        }
    }

    printf("\n--- issue 3: alignment grid ---\n");
    for (QWidget* widget : w.findChildren<QWidget*>()) {
        if (widget->objectName() == "gridToggle") {
            printf("  gridToggle = %p\n", (void*)widget);
        }
    }

    printf("\n--- issue 4: function drawing ---\n");
    CartesianEngine* engine = panel ? panel->engine() : nullptr;
    printf("  engine = %p enabled = %d\n", (void*)engine, engine ? engine->isEnabled() : -1);
    if (engine) {
        engine->addFunction("f", "3x^2 - 8x + 7", QColor(214, 96, 77));
        pump(100);
        printf("  curveCount = %d\n", engine->curveCount());
        CartesianCurveItem* curve = engine->curveAt(0);
        if (curve) {
            printf("  curve scenePos = %f,%f visible=%d\n",
                   curve->pos().x(), curve->pos().y(), curve->isVisible());
        }
    }
    MyButton* addButton = panel ? panel->findChild<MyButton*>("cartesianFunctionAdd") : nullptr;
    printf("  function add button = %p visible=%d\n", (void*)addButton,
           addButton ? addButton->isVisible() : -1);
    MyTextInput* fnInput = panel ? panel->findChild<MyTextInput*>("cartesianFunctionInput") : nullptr;
    printf("  function input = %p visible=%d geometry=%d,%d %dx%d\n", (void*)fnInput,
           fnInput ? fnInput->isVisible() : -1,
           fnInput ? fnInput->geometry().x() : -1, fnInput ? fnInput->geometry().y() : -1,
           fnInput ? fnInput->geometry().width() : -1, fnInput ? fnInput->geometry().height() : -1);

    printf("\n--- issue 4b: real click path ---\n");
    if (fnInput && addButton) {
        fnInput->setText("3x^2 - 8x + 7");
        const int before = engine->curveCount();
        addButton->click();
        pump(200);
        printf("  curveCount before=%d after=%d\n", before, engine->curveCount());
    }

    printf("\n--- issue 4c: does the curve actually paint ---\n");
    {
        CanvasView* view = w.findChild<CanvasView*>();
        printf("  view = %p\n", (void*)view);
        if (view && engine) {
            const QPointF tl = view->mapToSceneF(QPointF(0, 0));
            const QPointF br = view->mapToSceneF(QPointF(view->viewport()->width(),
                                                          view->viewport()->height()));
            printf("  viewport scene rect = %f,%f %fx%f\n", tl.x(), tl.y(),
                   br.x() - tl.x(), br.y() - tl.y());
            engine->updateViewport(QRectF(tl, br).normalized());
            pump(80);
            QImage img(view->viewport()->size(), QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            view->render(&p);
            p.end();
            int curvePixels = 0;
            for (int y = 0; y < img.height(); y += 2) {
                for (int x = 0; x < img.width(); x += 2) {
                    const QColor c = img.pixelColor(x, y);
                    if (c.red() > 180 && c.green() < 130 && c.blue() < 110) ++curvePixels;
                }
            }
            printf("  curve-colored pixels in viewport = %d\n", curvePixels);
            img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_diag_view.png");
        }
    }

    printf("\n--- issue 5: keyboard shortcuts ---\n");
    {
        ToolController* tc = w.findChild<ToolController*>();
        printf("  toolController = %p currentTool = %d\n", (void*)tc,
               tc ? static_cast<int>(tc->currentTool()) : -1);
    }

    printf("\n--- issue 4d: scene-level render (isolate engine vs view) ---\n");
    {
        CartesianModule* mod = nullptr;
        for (CartesianModule* c : w.findChildren<CartesianModule*>()) mod = c;
        QGraphicsScene* sc = graphicsSceneOf(w);
        printf("  scene = %p\n", (void*)sc);
        printf("  engine origin = %f,%f scale=%f step=%f\n",
               engine->originScene().x(), engine->originScene().y(),
               engine->scale(), engine->step());
        CartesianCurveItem* curve = engine ? engine->curveAt(0) : nullptr;
        if (curve) {
            const QRectF b = curve->visibleBounds();
            printf("  curve runs=%d points=%d sceneBounds=%f,%f %fx%f\n",
                   curve->visibleRunCount(), curve->visiblePointCount(),
                   b.x(), b.y(), b.width(), b.height());
            printf("  math x = %f..%f  math y = %f..%f\n",
                   b.x() / 60.0, (b.x() + b.width()) / 60.0,
                   -b.y() / 60.0, -(b.y() + b.height()) / 60.0);
        }
        if (sc && engine) {
            const double mnx = -9.27, mxx = 9.27, mny = -6.82, mxy = 6.82;
            engine->updateViewport(QRectF(mnx * 60.0, -mxy * 60.0,
                                           (mxx - mnx) * 60.0, (mxy - mny) * 60.0));
            pump(100);
            QImage img(1112, 818, QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            sc->render(&p, QRectF(0, 0, 1112, 818),
                       QRectF(mnx * 60.0, -mxy * 60.0, (mxx - mnx) * 60.0, (mxy - mny) * 60.0));
            p.end();
            int curvePixels = 0;
            for (int y = 0; y < img.height(); y++) {
                for (int x = 0; x < img.width(); x++) {
                    const QColor c = img.pixelColor(x, y);
                    if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++curvePixels;
                }
            }
            printf("  scene-level curve pixels = %d\n", curvePixels);
            img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_diag_scene.png");
        }
    }

    printf("\n--- issue 4e: definitive color audit of the app view ---\n");
    {
        CanvasView* view = w.findChild<CanvasView*>();
        QGraphicsScene* sc = graphicsSceneOf(w);
        printf("  scene items = %d, sceneRect = %f,%f %fx%f\n",
               sc->items().size(), sc->sceneRect().x(), sc->sceneRect().y(),
               sc->sceneRect().width(), sc->sceneRect().height());
        for (QGraphicsItem* item : sc->items()) {
            const QString name = QString::fromLatin1(typeid(*item).name());
            if (name.contains("Cartesian")) {
                printf("    %s visible=%d pos=%f,%f br=%f,%f %fx%f\n",
                       name.toUtf8().constData(), item->isVisible(),
                       item->pos().x(), item->pos().y(),
                       item->boundingRect().x(), item->boundingRect().y(),
                       item->boundingRect().width(), item->boundingRect().height());
            }
        }
        if (view) {
            QImage img(view->viewport()->size(), QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            view->render(&p);
            p.end();
            int axis = 0, curve = 0, minor = 0;
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    const QColor c = img.pixelColor(x, y);
                    if (c.red() < 160 && c.green() < 170 && c.blue() < 190) ++axis;
                    if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++curve;
                    if (c.red() > 225 && c.red() < 250 && c.green() > 235) ++minor;
                }
            }
            printf("  view pixels: axis=%d curve=%d gridMinor=%d\n", axis, curve, minor);
        }
    }

    printf("\n--- issue 4e: decisive curve paint check ---\n");
    {
        CartesianCurveItem* curve = engine ? engine->curveAt(0) : nullptr;
        if (!curve) {
            printf("  no curve!\n");
        } else {
            printf("  before: runs=%d points=%d visible=%d\n",
                   curve->visibleRunCount(), curve->visiblePointCount(), curve->isVisible());
            QImage direct(1112, 818, QImage::Format_ARGB32);
            direct.fill(Qt::white);
            QPainter dp(&direct);
            dp.translate(-curve->pos());
            curve->paint(&dp, nullptr, nullptr);
            dp.end();
            int directPixels = 0;
            for (int y = 0; y < direct.height(); ++y) {
                for (int x = 0; x < direct.width(); ++x) {
                    const QColor c = direct.pixelColor(x, y);
                    if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++directPixels;
                }
            }
            printf("  direct paint() pixels = %d\n", directPixels);

            QGraphicsScene* sc2 = graphicsSceneOf(w);
            if (sc2) {
                QImage probe(1112, 818, QImage::Format_ARGB32);
                probe.fill(Qt::white);
                QPainter pp(&probe);
                sc2->render(&pp, QRectF(0, 0, 1112, 818),
                            QRectF(-556.0, -409.0, 1112.0, 818.0));
                pp.end();
                printf("  probe pixel at curve vertex (636,309) = %d,%d,%d\n",
                       probe.pixelColor(636, 309).red(), probe.pixelColor(636, 309).green(),
                       probe.pixelColor(636, 309).blue());
                printf("  probe pixel at (636,300) = %d,%d,%d\n",
                       probe.pixelColor(636, 300).red(), probe.pixelColor(636, 300).green(),
                       probe.pixelColor(636, 300).blue());
                probe.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_probe2.png");
            }

            const QRectF b = curve->visibleBounds();
            printf("  sceneBounds = %f,%f %fx%f\n", b.x(), b.y(), b.width(), b.height());
            printf("  viewport scene = -556,-409 1112x818\n");
            printf("  curve overlaps viewport vertically? %d\n",
                   b.y() < 409.0 && (b.y() + b.height()) > -409.0);
            const QVector<QPointF> samples = curve->samplePoints(6);
            for (const QPointF& p : samples) {
                printf("    sample point = %f,%f\n", p.x(), p.y());
            }
        }
    }

    printf("\n--- render toolbar and panel ---\n");
    {
        if (toolbar) {
            toolbar->reposition();
            pump(120);
            QImage img(toolbar->size(), QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            toolbar->render(&p);
            p.end();
            img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_toolbar.png");
            printf("  toolbar size = %dx%d\n", toolbar->width(), toolbar->height());
        }
        if (panel) {
            QImage img(panel->size(), QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            panel->render(&p);
            p.end();
            img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_panel.png");
            printf("  panel size = %dx%d\n", panel->width(), panel->height());
        }
    }

    printf("\n--- issue 4f: WHICH curve renders ---\n");
    {
        CartesianEngine* e2 = panel ? panel->engine() : nullptr;
        QGraphicsScene* sc2 = graphicsSceneOf(w);
        if (e2 && sc2) {
            for (int i = 0; i < e2->curveCount(); ++i) {
                CartesianCurveItem* cv = e2->curveAt(i);
                if (!cv) continue;
                const QRectF vp(-556.0, -409.0, 1112.0, 818.0);
                int inside = 0;
                const QVector<QPointF> all = cv->samplePoints(100000);
                for (const QPointF& p : all) {
                    if (vp.contains(p)) ++inside;
                }
                printf("  curve[%d] '%s' expr='%s' runs=%d pts=%d insideViewport=%d bounds=%f,%f %fx%f\n",
                       i, cv->name().toUtf8().constData(), cv->expression().toUtf8().constData(),
                       cv->visibleRunCount(), cv->visiblePointCount(), inside,
                       cv->visibleBounds().x(), cv->visibleBounds().y(),
                       cv->visibleBounds().width(), cv->visibleBounds().height());
            }
            QImage img(1112, 818, QImage::Format_ARGB32);
            img.fill(Qt::white);
            QPainter p(&img);
            sc2->render(&p, QRectF(0, 0, 1112, 818), QRectF(-556.0, -409.0, 1112.0, 818.0));
            p.end();
            int red = 0, blue = 0;
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x) {
                    const QColor c = img.pixelColor(x, y);
                    if (c.red() > 170 && c.green() < 150 && c.blue() < 130) ++red;
                    if (c.blue() > 170 && c.red() < 150) ++blue;
                }
            }
            printf("  rendered: redPixels=%d bluePixels=%d\n", red, blue);
            img.save("D:/Program/Cpp/Project/QT/SmartGround/tests/build/cartesian_probe3.png");
        }
    }

    printf("\n--- panel geometry ---\n");
    if (panel) {
        printf("  panel geometry = %d,%d %dx%d\n",
               panel->geometry().x(), panel->geometry().y(),
               panel->geometry().width(), panel->geometry().height());
    }

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return 0;
}
