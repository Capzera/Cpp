#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QEventLoop>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <cstdio>
#include <cmath>
#include "ui/MainWindow.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyTextEdit.h"
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyCheckBox.h"
#include "ui/widgets/MyCollapsible.h"
#include "render/GraphElements.h"

static int failures = 0;
static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

static void pump(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

static MyButton* findButton(QWidget* root, const QString& keyword) {
    for (MyButton* b : root->findChildren<MyButton*>()) {
        if (b->text().contains(keyword)) return b;
    }
    return nullptr;
}

static int crashCycles() {
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    MyComboBox* combo = w.findChild<MyComboBox*>();
    MyToggleSwitch* toggle = w.findChild<MyToggleSwitch*>();
    MyCheckBox* biCheck = nullptr;
    for (MyCheckBox* c : w.findChildren<MyCheckBox*>()) {
        if (c->text().contains("双向箭头")) { biCheck = c; break; }
    }
    int bfs = -1, tarjan = -1, bubble = -1, kruskal = -1;
    for (int i = 1; i < combo->count(); ++i) {
        QString n = combo->itemText(i);
        if (n.contains("广度优先")) bfs = i;
        if (n.contains("强连通")) tarjan = i;
        if (n.contains("冒泡")) bubble = i;
        if (n.contains("Kruskal") || n.contains("最小生成")) kruskal = i;
    }
    int cycle[] = {bfs, tarjan, bubble, kruskal, bfs, bubble, bfs};
    int executed = 0;
    for (int rep = 0; rep < 3; ++rep) {
        for (int idx : cycle) {
            if (idx < 0) continue;
            combo->setCurrentIndex(idx);
            pump(60);
            MyButton* build = findButton(&w, "绘制");
            if (build) build->click();
            pump(80);
            if (toggle) {
                toggle->setChecked(toggle->isChecked() ? false : true);
                pump(80);
            }
            if (biCheck && biCheck->isEnabled()) {
                biCheck->setChecked(!biCheck->isChecked());
                pump(80);
            }
            executed++;
        }
    }
    printf("crash regression: no crash over %d module/option cycles\n", executed);
    return executed;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    {
        MainWindow w;
        w.resize(1400, 860);
        w.show();
        pump(150);

        MyComboBox* combo = w.findChild<MyComboBox*>();
        MyTextEdit* input = w.findChild<MyTextEdit*>();
        MyToggleSwitch* toggle = w.findChild<MyToggleSwitch*>();
        MyCollapsible* panel = w.findChild<MyCollapsible*>();
        MyButton* btnAuto = findButton(&w, "自动播放");
        MyButton* btnBuild = findButton(&w, "绘制");
        MyButton* btnStep = findButton(&w, "下一步");
        MyCheckBox* biCheck = nullptr;
        for (MyCheckBox* c : w.findChildren<MyCheckBox*>()) {
            if (c->text().contains("双向箭头")) { biCheck = c; break; }
        }
        printf("combo=%p input=%p toggle=%p panel=%p auto=%p build=%p step=%p bicheck=%p\n",
               (void*)combo, (void*)input, (void*)toggle, (void*)panel, (void*)btnAuto,
               (void*)btnBuild, (void*)btnStep, (void*)biCheck);
        check(combo && input && toggle && panel && btnAuto && btnBuild && btnStep && biCheck,
              "all custom widgets found");

        check(!panel->isVisible(), "BUG-039: graph panel HIDDEN on startup");

        int idxGraph = -1, idxNonGraph = -1;
        for (int i = 1; i < combo->count(); ++i) {
            QString name = combo->itemText(i);
            if (name.contains("广度优先") && idxGraph < 0) idxGraph = i;
            if (name.contains("冒泡") && idxNonGraph < 0) idxNonGraph = i;
        }
        printf("graph module = %s | nonggraph module = %s\n",
               qPrintable(combo->itemText(idxGraph)), qPrintable(combo->itemText(idxNonGraph)));

        combo->setCurrentIndex(idxGraph);
        pump(200);
        check(panel->isVisible(), "graph panel VISIBLE for graph module");
        check(input->placeholderText().contains("顶点数"), "graph placeholder mentions 顶点数");

        combo->setCurrentIndex(idxNonGraph);
        pump(200);
        check(!panel->isVisible(), "graph panel HIDDEN for non-graph module");
        check(input->placeholderText().contains("数组长度"), "sort placeholder mentions 数组长度");

        combo->setCurrentIndex(idxGraph);
        pump(200);
        check(panel->isVisible(), "graph panel VISIBLE again after switching back");
        check(toggle->isChecked(), "undirected switch ON by default for BFS");

        btnAuto->setText("自动播放");
        check(btnAuto->text() == "自动播放", "auto button starts as 自动播放");

        btnBuild->click();
        pump(400);
        check(w.probeFrameCount() > 0, "frames recorded");
        printf("frame count = %d\n", w.probeFrameCount());

        w.probeStartAuto();
        pump(60);
        check(w.probeIsPlaying(), "auto play entered playing state");
        check(btnAuto->text() == "暂停", "button becomes 暂停 while playing");

        w.probeAdvance();
        pump(60);
        check(w.probeFrameIndex() >= 1, "auto advance moved frame index");

        w.probeRequestPause();
        pump(60);
        check(w.probePendingPause(), "pause request registered as pending");
        check(btnAuto->text() == "暂停中...", "button shows 暂停中 while settling");
        int beforeSettle = w.probeFrameIndex();
        w.probeAdvance();
        check(w.probeFrameIndex() == beforeSettle, "no further frame advance while pending pause");

        w.probeSettle();
        pump(60);
        check(!w.probeIsPlaying(), "playing stops after frame settles");
        check(btnAuto->text() == "自动播放", "button back to 自动播放 after pause");
        int pausedIndex = w.probeFrameIndex();

        btnStep->click();
        pump(60);
        check(w.probeFrameIndex() == pausedIndex + 1, "manual step advances index");

        w.probeStartAuto();
        pump(60);
        check(w.probeIsPlaying(), "resume enters playing state");
        check(btnAuto->text() == "暂停", "button becomes 暂停 again on resume");
        int resumeIndex = w.probeFrameIndex();
        w.probeAdvance();
        pump(60);
        check(w.probeFrameIndex() == resumeIndex + 1, "resume continues from CURRENT index");

        {
            QGraphicsScene* scene = w.findChild<QGraphicsScene*>();
            check(scene != nullptr, "scene found");
            auto nodePositions = [&]() {
                std::map<int, QPointF> result;
                for (QGraphicsItem* item : scene->items()) {
                    GraphNode* node = dynamic_cast<GraphNode*>(item);
                    if (!node) continue;
                    result[node->id] = node->pos();
                }
                return result;
            };
            std::map<int, QPointF> before = nodePositions();
            check(before.size() >= 7, "graph nodes present in scene");
            int movedId = -1;
            for (auto& pair : before) { movedId = pair.first; break; }
            for (QGraphicsItem* item : scene->items()) {
                GraphNode* node = dynamic_cast<GraphNode*>(item);
                if (!node || node->id != movedId) continue;
                node->setPos(node->pos() + QPointF(77, -55));
                break;
            }
            pump(120);
            std::map<int, QPointF> moved = nodePositions();
            check(std::abs(moved[movedId].x() - (before[movedId].x() + 77)) < 0.01, "node manually moved");

            toggle->setChecked(false);
            pump(400);
            std::map<int, QPointF> after = nodePositions();
            check(after.size() == before.size(), "same node count after toggle");
            bool preserved = true;
            for (const auto& pair : after) {
                auto it = moved.find(pair.first);
                if (it == moved.end()) continue;
                if (std::abs(pair.second.x() - it->second.x()) > 0.5 || std::abs(pair.second.y() - it->second.y()) > 0.5) preserved = false;
            }
            check(preserved, "node positions PRESERVED across directed/undirected toggle");
        }

        {
            int shortDelay = w.probeDelayFor("ok");
            int midDelay = w.probeDelayFor("把当前节点标记为已访问并加入队列等待扩展");
            int longDelay = w.probeDelayFor(QString("先把起点入队并置为已访问，然后不断取出队头元素，检查它的所有邻接点，若邻接点尚未访问则立即标记并入队，直到队列为空为止，这一步体现了广度优先搜索逐层推进的核心思想"));
            printf("delays: short=%d mid=%d long=%d\n", shortDelay, midDelay, longDelay);
            check(shortDelay < midDelay, "adaptive delay: longer text waits longer (short < mid)");
            check(midDelay < longDelay, "adaptive delay: longer text waits longer (mid < long)");
            check(shortDelay >= 800, "adaptive delay: floor keeps a readable minimum");
            check(longDelay <= 3000, "adaptive delay: ceiling caps the wait");
        }

        {
            toggle->setChecked(true);
            pump(320);
            check(biCheck->isEnabled(), "bidirectional option enabled when undirected");
            double ratioOn = biCheck->property("ratio").toDouble();
            toggle->setChecked(false);
            pump(420);
            double ratioOff = biCheck->property("ratio").toDouble();
            printf("sub-option ratio: on=%.2f off=%.2f\n", ratioOn, ratioOff);
            check(ratioOn > 0.9, "sub-option fully visible when undirected");
            check(ratioOff < 0.1, "sub-option collapses when directed");
        }

        {
            btnBuild->click();
            pump(300);
            QGraphicsView* view = w.findChild<QGraphicsView*>();
            check(view != nullptr, "view found");
            QGraphicsScene* scene = w.findChild<QGraphicsScene*>();
            QRectF items = scene->itemsBoundingRect();
            QPointF viewCenter = view->mapToScene(view->viewport()->rect().center());
            QPointF contentCenter = items.center();
            qreal offset = std::hypot(viewCenter.x() - contentCenter.x(), viewCenter.y() - contentCenter.y());
            qreal scale = view->transform().m11();
            printf("center offset=%.1f scale=%.3f\n", offset, scale);
            check(offset < 120.0, "BUG-040: content centered in viewport after build");
            check(scale > 0.24 && scale < 2.45, "BUG-040: zoom clamped into readable range");
        }
    }

    printf("\n%d failures\n", failures);
    crashCycles();
    return failures == 0 ? 0 : 1;
}
