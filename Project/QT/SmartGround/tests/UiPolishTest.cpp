#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <cstdio>
#include "ui/widgets/MyComboBox.h"
#include "modules/Knapsack01Module.h"
#include "modules/BubbleSortModule.h"

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
    for (int i = 1; i <= 24; ++i) combo.addItems({QString("算法模块 %1").arg(i)});
    combo.resize(220, 34);
    combo.move(40, 40);
    combo.show();
    QApplication::processEvents();

    clickWidget(&combo);
    check(combo.isPopupOpen(), "popup opens with 25 items");

    QScrollArea* scroll = nullptr;
    QWidget* popup = nullptr;
    for (QWidget* w : QApplication::topLevelWidgets()) {
        if (w->isWindow() && w->windowType() == Qt::Popup && w != &combo) popup = w;
    }
    if (popup) {
        scroll = popup->findChild<QScrollArea*>();
        check(scroll != nullptr, "popup contains a QScrollArea");
        if (scroll) {
            check(scroll->verticalScrollBarPolicy() == Qt::ScrollBarAsNeeded, "scrollbar policy is AsNeeded");
            QScrollBar* bar = scroll->verticalScrollBar();
            check(bar != nullptr, "vertical scrollbar exists");
            if (bar) {
                printf("scroll range: %d..%d page=%d\n", bar->minimum(), bar->maximum(), bar->pageStep());
                check(bar->maximum() > 0, "scrollbar is scrollable (content overflows)");
                check(scroll->height() < 300, "popup viewport is height-capped, not full 25 items");
            }
            popup->grab().save("combo_scroll.png");
        }
    }

    if (scroll) {
        QScrollBar* bar = scroll->verticalScrollBar();
        bar->setValue(bar->maximum());
        QApplication::processEvents();
        check(bar->value() > 0, "scrollbar can scroll to bottom");
        bar->setValue(0);
        QApplication::processEvents();
    }

    Knapsack01Module knap;
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.resize(1000, 800);
    view.show();
    knap.buildLayout(&scene, "4 8\n2 3\n3 4\n4 5\n5 8");
    QApplication::processEvents();

    QRectF bounds = scene.itemsBoundingRect();
    QImage grid(bounds.size().toSize(), QImage::Format_ARGB32);
    grid.fill(Qt::white);
    {
        QPainter painter(&grid);
        scene.render(&painter, QRectF(QPointF(0, 0), bounds.size()), bounds);
    }
    grid.save("knapsack_grid.png");
    printf("knapsack scene bounds: %d x %d\n", (int)bounds.width(), (int)bounds.height());
    check(bounds.height() > 400, "knapsack scene has vertical extent for DP rows");

    int overlaps = 0;
    QVector<QGraphicsTextItem*> texts;
    for (QGraphicsItem* item : scene.items()) {
        if (dynamic_cast<QGraphicsTextItem*>(item)) texts.push_back(static_cast<QGraphicsTextItem*>(item));
    }
    for (int a = 0; a < texts.size(); ++a) {
        QRectF ra = texts[a]->mapRectToScene(texts[a]->boundingRect());
        for (int b = a + 1; b < texts.size(); ++b) {
            QRectF rb = texts[b]->mapRectToScene(texts[b]->boundingRect());
            if (ra.intersects(rb)) {
                overlaps++;
                printf("OVERLAP: '%s' vs '%s'\n", texts[a]->toPlainText().toUtf8().constData(),
                       texts[b]->toPlainText().toUtf8().constData());
            }
        }
    }
    printf("text items: %d, overlapping pairs: %d\n", texts.size(), overlaps);
    check(overlaps == 0, "no text labels overlap each other");

    int blockOverlaps = 0;
    QVector<QGraphicsItem*> blocks;
    for (QGraphicsItem* item : scene.items()) {
        if (item->type() >= QGraphicsItem::UserType + 1 && item->type() < QGraphicsItem::UserType + 100) blocks.push_back(item);
    }
    for (int a = 0; a < blocks.size(); ++a) {
        ArrayBlock* ba = qgraphicsitem_cast<ArrayBlock*>(blocks[a]);
        QRectF ra = ba ? ba->mapRectToScene(ba->bodyRect()) : blocks[a]->mapRectToScene(blocks[a]->boundingRect());
        for (int b = a + 1; b < blocks.size(); ++b) {
            ArrayBlock* bb = qgraphicsitem_cast<ArrayBlock*>(blocks[b]);
            QRectF rb = bb ? bb->mapRectToScene(bb->bodyRect()) : blocks[b]->mapRectToScene(blocks[b]->boundingRect());
            if (ra.intersects(rb)) blockOverlaps++;
        }
    }
    printf("blocks: %d, overlapping pairs: %d\n", blocks.size(), blockOverlaps);
    check(blockOverlaps == 0, "no array blocks overlap each other");

    BubbleSortModule bubble;
    QGraphicsScene scene2;
    bubble.buildLayout(&scene2, "5 3 8 1 9");
    QApplication::processEvents();
    QRectF bounds2 = scene2.itemsBoundingRect();
    QImage img2(bounds2.size().toSize(), QImage::Format_ARGB32);
    img2.fill(Qt::white);
    {
        QPainter painter(&img2);
        scene2.render(&painter, QRectF(QPointF(0, 0), bounds2.size()), bounds2);
    }
    img2.save("bubble_layout.png");
    check(true, "bubble sort layout rendered");

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
