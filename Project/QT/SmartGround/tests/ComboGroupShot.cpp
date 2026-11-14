#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <cstdio>
#include "ui/widgets/MyComboBox.h"

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
    combo.addGroup("基础结构", {"栈", "队列", "并查集", "树状数组 BIT"});
    combo.addGroup("数学", {"前缀和", "差分", "二分查找(闭区间)", "二分查找(半开)"});
    combo.addGroup("排序", {"冒泡排序", "选择排序", "插入排序", "快速排序", "归并排序", "堆排序"});
    combo.addGroup("图论", {"BFS 广度优先搜索", "拓扑排序", "Dijkstra 最短路", "Kruskal 最小生成树"});
    combo.addGroup("线段树", {"线段树 (建树)", "线段树 (区间查询)", "线段树 (lz标记)"});
    combo.resize(240, 34);
    combo.move(40, 40);
    combo.show();
    QApplication::processEvents();

    clickWidget(&combo);
    QWidget* popup = nullptr;
    for (QWidget* w : QApplication::topLevelWidgets()) {
        if (w->isWindow() && w->windowType() == Qt::Popup && w != &combo) { popup = w; break; }
    }
    if (popup) {
        QPixmap pm = popup->grab();
        pm.save("combo_groups.png");
        printf("saved %dx%d\n", pm.width(), pm.height());
    }
    return 0;
}
