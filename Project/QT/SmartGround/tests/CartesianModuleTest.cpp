#include <QApplication>
#include <QGraphicsScene>
#include <cstdio>
#include <cmath>
#include "core/CartesianExpression.h"
#include "render/CartesianElements.h"
#include "modules/CartesianModule.h"
#include "ui/widgets/CartesianPanel.h"

static int g_fail = 0;

static void check(bool ok, const QString& name) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", qPrintable(name));
    if (!ok) g_fail++;
}

static bool nearly(double a, double b, double eps = 1e-6) {
    return std::abs(a - b) < eps;
}

static void testExpressionParsing() {
    printf("--- expression parsing ---\n");
    {
        const CartesianExpression::Result r = CartesianExpression::parse("2x + 7");
        check(r.ok, "linear parse ok");
        check(nearly(r.c1, 2.0), "linear coefficient x = 2");
        check(nearly(r.c0, 7.0), "linear constant = 7");
        check(nearly(CartesianExpression::evaluate(r, 3.0), 13.0), "linear evaluate at x=3");
    }
    {
        const CartesianExpression::Result r = CartesianExpression::parse("3x^2 - 8x + 7");
        check(r.ok, "quadratic parse ok");
        check(nearly(r.c2, 3.0), "quadratic c2 = 3");
        check(nearly(r.c1, -8.0), "quadratic c1 = -8");
        check(nearly(r.c0, 7.0), "quadratic c0 = 7");
        check(nearly(CartesianExpression::evaluate(r, 0.0), 7.0), "quadratic f(0) = 7");
        check(nearly(CartesianExpression::evaluate(r, 1.0), 2.0), "quadratic f(1) = 2");
        check(nearly(CartesianExpression::evaluate(r, 2.0), 3.0), "quadratic f(2) = 3");
    }
    {
        const CartesianExpression::Result r = CartesianExpression::parse("x^3 - 2x^2 + x - 1");
        check(r.ok, "cubic parse ok");
        check(nearly(r.c3, 1.0), "cubic c3 = 1");
        check(nearly(r.c2, -2.0), "cubic c2 = -2");
        check(nearly(CartesianExpression::evaluate(r, 2.0), 1.0), "cubic f(2) = 1");
    }
    {
        const CartesianExpression::Result r = CartesianExpression::parse("(x + 1)(x - 1)");
        check(r.ok, "parenthesized product parse ok");
        check(nearly(r.c2, 1.0), "product expands to x^2");
        check(nearly(r.c0, -1.0), "product expands to -1");
    }
    {
        check(!CartesianExpression::parse("").ok, "empty expression rejected");
        check(!CartesianExpression::parse("3y + 1").ok, "unknown variable rejected");
        check(!CartesianExpression::parse("x^5").ok, "degree above 3 rejected");
        check(!CartesianExpression::parse("2x +").ok, "incomplete expression rejected");
    }
}

static void testExpressionDescription() {
    printf("--- expression description ---\n");
    {
        const CartesianExpression::Result r = CartesianExpression::parse("3x^2 - 8x + 7");
        const QString text = CartesianExpression::describe(r);
        check(text.startsWith("y = "), "describe starts with y =");
        check(text.contains("3x^2"), "describe contains 3x^2");
        check(text.contains("8x"), "describe contains 8x");
        check(text.contains("7"), "describe contains 7");
    }
    {
        const QString eq = CartesianExpression::describeSegment(0.0, 1.0, 1.0, 3.0);
        check(eq.startsWith("y = "), "segment equation starts with y =");
        check(eq.contains("2"), "segment equation contains slope 2");
    }
    {
        const QString horizontal = CartesianExpression::describeSegment(0.0, 5.0, 4.0, 5.0);
        check(horizontal.contains("5"), "horizontal segment equation constant 5");
    }
    {
        const QString vertical = CartesianExpression::describeSegment(2.0, 0.0, 2.0, 6.0);
        check(vertical.startsWith("x = "), "vertical segment equation is x = const");
    }
    {
        check(CartesianExpression::formatNumber(-0.0) == "0", "formatNumber normalizes negative zero");
        check(CartesianExpression::formatNumber(2.50) == "2.5", "formatNumber trims trailing zeros");
    }
}

static void testEngineCoordinates() {
    printf("--- engine coordinates ---\n");
    CartesianEngine engine;
    engine.setScale(60.0);
    engine.setOriginScene(QPointF(300.0, 200.0));

    const QPointF scene = engine.toScene(2.0, -3.0);
    check(nearly(scene.x(), 420.0), "toScene x = origin + x * scale");
    check(nearly(scene.y(), 380.0), "toScene y = origin - y * scale");

    const QPointF back = engine.toMath(scene);
    check(nearly(back.x(), 2.0), "toMath round trip x");
    check(nearly(back.y(), -3.0), "toMath round trip y");
}

static void testEngineSnapping() {
    printf("--- engine snapping ---\n");
    CartesianEngine engine;
    engine.setScale(60.0);
    engine.setOriginScene(QPointF(0.0, 0.0));

    const QPointF snapped = engine.snapMath(2.37, -1.82);
    check(nearly(snapped.x(), 2.0), "snapMath x rounds to 2");
    check(nearly(snapped.y(), -2.0), "snapMath y rounds to -2");

    const QPointF sceneSnapped = engine.snapScene(QPointF(2.37 * 60.0, 1.82 * 60.0));
    const QPointF math = engine.toMath(sceneSnapped);
    check(nearly(math.x(), 2.0), "snapScene x lands on grid intersection");
    check(nearly(math.y(), -2.0), "snapScene y lands on grid intersection");
}

static void testEngineLadder() {
    printf("--- engine step ladder ---\n");
    const QVector<qreal> ladder = CartesianEngine::stepLadder();
    check(ladder.size() >= 13, "ladder has at least 13 rungs");
    check(nearly(ladder.first(), 100.0), "ladder starts at 100");
    check(nearly(ladder.last(), 0.01), "ladder ends at 0.01");
    bool descending = true;
    for (int i = 1; i < ladder.size(); ++i) {
        if (!(ladder.at(i) < ladder.at(i - 1))) descending = false;
    }
    check(descending, "ladder is strictly descending");
}

static void testEngineZoom() {
    printf("--- engine zoom and ruler ---\n");
    CartesianEngine engine;
    engine.setScale(60.0);
    check(nearly(engine.step(), 1.0), "default ruler is 1.0");

    engine.setScale(600.0);
    check(engine.step() < 1.0, "zooming in refines the ruler");

    engine.setScale(6000.0);
    check(engine.step() <= 0.01, "maximum zoom reaches 0.01 ruler");

    engine.setScale(6.0);
    check(engine.step() >= 1.0, "zooming out coarsens the ruler");
}

static void testEnginePoints() {
    printf("--- engine point lifecycle ---\n");
    QGraphicsScene scene;
    CartesianEngine engine;
    engine.attachScene(&scene);
    engine.setEnabled(true);
    engine.setScale(60.0);

    const int index = engine.addPoint(1.0, 2.0, QString());
    check(index == 0, "first point index is 0");
    check(engine.pointCount() == 1, "point count is 1");

    CartesianPointItem* point = engine.pointAt(0);
    check(point != nullptr, "pointAt returns the point");
    if (point) {
        check(point->name() == "A", "auto name is A");
        check(nearly(point->x(), 1.0), "point x = 1");
        check(nearly(point->y(), 2.0), "point y = 2");
        point->setName("P1");
        check(engine.pointAt(0)->name() == "P1", "point name is editable");
        point->setNameHidden(true);
        check(engine.pointAt(0)->nameHidden(), "point name can be hidden");
        point->setSize(12.0);
        check(nearly(engine.pointAt(0)->size(), 12.0), "point size is editable");
        point->setShape(2);
        check(engine.pointAt(0)->pointShape() == 2, "point shape is editable");
        point->setColor(QColor(10, 200, 90));
        check(engine.pointAt(0)->color() == QColor(10, 200, 90), "point color is editable");
    }

    engine.addPoint(-3.0, 4.0, QString());
    check(engine.pointAt(1)->name() == "B", "second auto name is B");

    engine.removePoint(0);
    check(engine.pointCount() == 1, "removePoint shrinks the list");
    check(engine.pointAt(0)->name() == "B", "remaining point keeps its name");
}

static void testEngineSegments() {
    printf("--- engine segment lifecycle ---\n");
    QGraphicsScene scene;
    CartesianEngine engine;
    engine.attachScene(&scene);
    engine.setEnabled(true);

    engine.addPoint(0.0, 0.0, QString("A"));
    engine.addPoint(2.0, 4.0, QString("B"));

    CartesianPointItem* a = engine.pointAt(0);
    CartesianPointItem* b = engine.pointAt(1);
    check(a && b, "both endpoints exist");

    const int segmentIndex = engine.linkPoints(a, b);
    check(segmentIndex == 0, "linkPoints returns a segment index");

    check(engine.pointAtScene(engine.toScene(0.0, 0.0)) == a, "hit test finds point A");
    check(engine.pointAtScene(engine.toScene(2.0, 4.0)) == b, "hit test finds point B");
    check(engine.pointAtScene(QPointF(9999.0, 9999.0)) == nullptr, "hit test misses empty space");
}

static void testEngineFunctions() {
    printf("--- engine function curves ---\n");
    QGraphicsScene scene;
    CartesianEngine engine;
    engine.attachScene(&scene);
    engine.setEnabled(true);

    const int index = engine.addFunction("f", "3x^2 - 8x + 7", QColor(200, 60, 60));
    check(index == 0, "addFunction returns curve index");
    check(engine.curveCount() == 1, "curve count is 1");

    CartesianCurveItem* curve = engine.curveAt(0);
    check(curve != nullptr, "curveAt returns the curve");
    if (curve) {
        check(curve->name() == "f", "curve name is f");
        check(curve->expression() == "3x^2 - 8x + 7", "curve keeps the expression");
    }

    engine.addFunction("g", "2x + 7", QColor(60, 140, 220));
    check(engine.curveCount() == 2, "two curves can coexist");

    engine.updateViewport(QRectF(-600.0, -400.0, 1200.0, 800.0));
    check(engine.curveCount() == 2, "viewport update keeps both curves");

    engine.removeCurve("g");
    check(engine.curveCount() == 1, "removeCurve removes one curve");
}

static void testModule() {
    printf("--- cartesian module ---\n");
    CartesianModule module;
    check(module.isCartesian(), "module reports cartesian mode");
    check(module.getModuleName() == QString("自由结构 - 笛卡尔坐标系"), "module name matches grouping");
    check(module.engine() != nullptr, "module exposes an engine");
    check(!module.getInputHint().isEmpty(), "module provides an input hint");

    QGraphicsScene scene;
    module.buildLayout(&scene, QString());
    check(module.engine()->isEnabled(), "engine is enabled after buildLayout");

    module.releaseSceneItems();
}

static void testPanel() {
    printf("--- cartesian panel ---\n");
    CartesianPanel panel;
    check(panel.engine() == nullptr, "panel starts without an engine");
    panel.refresh();
    check(true, "panel refresh with no engine does not crash");
}

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    QApplication app(argc, argv);

    testExpressionParsing();
    testExpressionDescription();
    testEngineCoordinates();
    testEngineSnapping();
    testEngineLadder();
    testEngineZoom();
    testEnginePoints();
    testEngineSegments();
    testEngineFunctions();
    testModule();
    testPanel();

    printf("\n%s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
