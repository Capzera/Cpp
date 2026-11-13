#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QTimer>
#include <QRegularExpression>
#include <cmath>
#include <vector>
#include <queue>
#include <map>
#include <iostream>

// ================= 底层数据结构与算法 (竞赛模板风格) =================
const int N = 1005;
const int M = 5005;

int head[N], to[M], nxt[M], tot;
int dfn[N], low[N], timestamp;
int stk[N], top;
bool ins[N];
int scc[N], scc_cnt;
bool is_cut[N];

void init_graph() {
    tot = 0; timestamp = 0; top = 0; scc_cnt = 0;
    memset(head, 0, sizeof(head));
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(ins, 0, sizeof(ins));
    memset(scc, 0, sizeof(scc));
    memset(is_cut, 0, sizeof(is_cut));
}

void add_edge(int u, int v) {
    to[++tot] = v;
    nxt[tot] = head[u];
    head[u] = tot;
}

// ================= 动画与可视化相关定义 =================
enum ActionType {
    HIGHLIGHT_NODE,   // 凸出节点并分配 dfn, low
    TRAVERSE_EDGE,    // 遍历边
    UPDATE_LOW,       // 更新 low 值
    PUSH_STACK,       // 压入栈，更新 ins
    POP_STACK,        // 弹出栈，形成 SCC
    MARK_CUT,         // 标记割点/割边
    CONDENSE          // 缩点动画 (移动并合并)
};

struct AnimStep {
    ActionType type;
    int u, v;
    int val1, val2;
    std::vector<int> group;
};
std::vector<AnimStep> anim_queue;

class GraphEdge;

// ================= 图形元素 =================
class GraphNode : public QGraphicsObject {
    Q_OBJECT
public:
    int id, d, l;
    bool in_stack;
    QColor color;
    std::vector<GraphEdge*> edgeList;

    GraphNode(int _id) : id(_id), d(0), l(0), in_stack(false), color(Qt::white) {
        setFlags(ItemIsMovable | ItemSendsGeometryChanges);
        setZValue(1); // 保证节点永远在边的上层
    }

    void addEdge(GraphEdge *edge) { edgeList.push_back(edge); }

    // 扩大检测与刷新范围，向下预留文字空间
    QRectF boundingRect() const override { return QRectF(-60, -30, 120, 80); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(color);
        QPen borderPen(in_stack ? Qt::red : Qt::black, in_stack ? 3 : 2);
        painter->setPen(borderPen);

        // 中心(0,0)，半径20的圆
        painter->drawEllipse(-20, -20, 40, 40);

        // 节点编号 ID
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 12, QFont::Bold));
        painter->drawText(QRectF(-20, -20, 40, 40), Qt::AlignCenter, QString::number(id));

        // 绘制完整的 dfn : x, low : x
        if (d > 0) {
            painter->setFont(QFont("Arial", 10, QFont::Bold));
            painter->setPen(QColor(20, 80, 150)); // 深蓝色凸显
            painter->drawText(QRectF(-60, 25, 120, 20), Qt::AlignCenter, QString("dfn : %1, low : %2").arg(d).arg(l));
        }
    }

    void updateData(int _d, int _l) { d = _d; l = _l; update(); }

    // 当节点移动时（哪怕是动画控制的），立即通知绑定的边重绘，不留残影
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

class GraphEdge : public QGraphicsItem {
public:
    GraphNode *source, *dest;
    bool is_directed;
    QColor color;

    GraphEdge(GraphNode *s, GraphNode *d, bool dir)
        : source(s), dest(d), is_directed(dir), color(Qt::black) { // 边和箭头默认为黑色
        setZValue(-1); // 确保在最底层
    }

    // 边界巨型用于告知底层图形引擎何处需要刷新
    QRectF boundingRect() const override {
        if (!source || !dest) return QRectF();
        QPointF p1 = source->pos();
        QPointF p2 = dest->pos();
        return QRectF(p1, p2).normalized().adjusted(-20, -20, 20, 20);
    }

    // 节点调用此方法刷新边
    void adjust() {
        if (!source || !dest) return;
        prepareGeometryChange();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        if (!source || !dest) return;
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        QPointF p1 = source->pos();
        QPointF p2 = dest->pos();
        QLineF line(p1, p2);
        if (line.length() <= 40.0) return; // 距离太近不绘

        // 计算圆周边缘的完美接合点 (半径为20)
        qreal dx = p2.x() - p1.x();
        qreal dy = p2.y() - p1.y();
        qreal rad = std::atan2(dy, dx);

        QPointF startPoint = p1 + QPointF(20 * std::cos(rad), 20 * std::sin(rad));
        QPointF endPoint = p2 - QPointF(20 * std::cos(rad), 20 * std::sin(rad));

        painter->drawLine(startPoint, endPoint);

        // 绘制黑色箭头
        if (is_directed) {
            qreal arrowSize = 12.0;
            QPointF arrowP1 = endPoint - QPointF(arrowSize * std::cos(rad - M_PI / 6.0), arrowSize * std::sin(rad - M_PI / 6.0));
            QPointF arrowP2 = endPoint - QPointF(arrowSize * std::cos(rad + M_PI / 6.0), arrowSize * std::sin(rad + M_PI / 6.0));
            QPolygonF arrowHead;
            arrowHead << endPoint << arrowP1 << arrowP2;
            painter->setBrush(color);
            painter->setPen(QPen(color, 1));
            painter->drawPolygon(arrowHead);
        }
    }
};

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (GraphEdge *edge : edgeList) edge->adjust();
    }
    return QGraphicsObject::itemChange(change, value);
}

class StackBlock : public QGraphicsObject {
    Q_OBJECT
public:
    int id;
    StackBlock(int _id) : id(_id) {
        setOpacity(0);
        setZValue(2);
    }
    QRectF boundingRect() const override { return QRectF(-30, -15, 60, 30); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setBrush(QColor(135, 206, 235)); // 天蓝色
        painter->setPen(QPen(Qt::black, 2));
        painter->drawRect(-30, -15, 60, 30);
        painter->setFont(QFont("Arial", 11, QFont::Bold));
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(id));
    }
};

// ================= 主窗口与控制面板 =================
class TarjanVisualizer : public QMainWindow {
    Q_OBJECT
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTextEdit *inputText;
    QComboBox *modeSelect;

    std::map<int, GraphNode*> nodes;
    std::vector<GraphEdge*> edges;
    std::vector<StackBlock*> stack_ui;

    int n, m;
    int current_anim_index = 0;
    QPointF stack_base_pos = QPointF(720, 500); // 侧边栈的位置

public:
    TarjanVisualizer() {
        QWidget *central = new QWidget;
        QHBoxLayout *mainLayout = new QHBoxLayout(central);

        QVBoxLayout *controlLayout = new QVBoxLayout;
        inputText = new QTextEdit;
        inputText->setFont(QFont("Consolas", 10));
        inputText->setPlaceholderText("输入格式:\nn m\nu1 v1\nu2 v2...");
        inputText->setText("6 8\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6\n6 4\n2 6"); // 经典强连通分量测例

        modeSelect = new QComboBox;
        modeSelect->addItems({"有向图 - 强连通分量 (SCC/缩点)", "无向图 - 割点与割边 (Bridge/Cut)"});

        QPushButton *btnGen = new QPushButton("1. 生成并分层展示图");
        QPushButton *btnRun = new QPushButton("2. 计算 Tarjan 步骤");
        QPushButton *btnStep = new QPushButton("3. 播放单步动画");
        QPushButton *btnAuto = new QPushButton("4. 自动演示全过程");

        QFont btnFont("Microsoft YaHei", 10, QFont::Bold);
        btnGen->setFont(btnFont); btnRun->setFont(btnFont);
        btnStep->setFont(btnFont); btnAuto->setFont(btnFont);

        controlLayout->addWidget(inputText);
        controlLayout->addWidget(modeSelect);
        controlLayout->addWidget(btnGen);
        controlLayout->addWidget(btnRun);
        controlLayout->addWidget(btnStep);
        controlLayout->addWidget(btnAuto);
        mainLayout->addLayout(controlLayout, 1);

        scene = new QGraphicsScene(0, 0, 800, 600);
        view = new QGraphicsView(scene);
        view->setRenderHint(QPainter::Antialiasing);
        mainLayout->addWidget(view, 4);
        setCentralWidget(central);
        resize(1100, 750);

        connect(btnGen, &QPushButton::clicked, this, &TarjanVisualizer::buildGraph);
        connect(btnRun, &QPushButton::clicked, this, &TarjanVisualizer::runAlgorithm);
        connect(btnStep, &QPushButton::clicked, this, &TarjanVisualizer::playNextAnim);
        connect(btnAuto, &QPushButton::clicked, [this](){
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, [this, timer](){
                if(current_anim_index < anim_queue.size()) playNextAnim();
                else timer->stop();
            });
            timer->start(800);
        });

        QGraphicsTextItem *stkText = scene->addText("ins / stk");
        stkText->setPos(680, 50);
        stkText->setFont(QFont("Arial", 14, QFont::Bold));
    }

private:
    void buildGraph() {
        scene->clear(); nodes.clear(); edges.clear();
        stack_ui.clear(); anim_queue.clear();
        init_graph(); current_anim_index = 0;

        QGraphicsTextItem *stkText = scene->addText("ins / stk");
        stkText->setPos(680, 50); stkText->setFont(QFont("Arial", 14, QFont::Bold));

        QStringList lines = inputText->toPlainText().split('\n');
        if (lines.isEmpty()) return;
        QStringList nm = lines[0].split(QRegularExpression("\\s+"));
        if(nm.size() < 2) return;
        n = nm[0].toInt(); m = nm[1].toInt();
        bool is_dir = modeSelect->currentIndex() == 0;

        // 1. 读取并构建底层逻辑边 (为拓扑排序做准备)
        int line_idx = 1;
        for (int i = 0; i < m && line_idx < lines.size(); ) {
            QStringList uv = lines[line_idx++].split(QRegularExpression("\\s+"));
            if(uv.size() < 2) continue;
            int u = uv[0].toInt(), v = uv[1].toInt();
            add_edge(u, v);
            if(!is_dir) add_edge(v, u);
            i++;
        }

        // 2. BFS 拓扑分层算法 (计算从上到下的坐标结构)
        std::vector<int> inDegree(n + 1, 0);
        for (int u = 1; u <= n; ++u)
            for (int e = head[u]; e; e = nxt[e]) inDegree[to[e]]++;

        std::vector<int> layer(n + 1, -1);
        int max_layer = 0;
        std::queue<int> q;

        for (int i = 1; i <= n; i++)
            if (inDegree[i] == 0) { layer[i] = 0; q.push(i); }

        if (q.empty() && n > 0) { layer[1] = 0; q.push(1); } // 强连通纯环图备用方案

        auto bfs_layer = [&]() {
            while (!q.empty()) {
                int u = q.front(); q.pop();
                max_layer = std::max(max_layer, layer[u]);
                for (int e = head[u]; e; e = nxt[e]) {
                    int v = to[e];
                    if (layer[v] == -1) { layer[v] = layer[u] + 1; q.push(v); }
                }
            }
        };
        bfs_layer();

        for (int i = 1; i <= n; i++) { // 兜底处理非连通块
            if (layer[i] == -1) {
                layer[i] = max_layer + 1;
                q.push(i); bfs_layer();
            }
        }

        std::vector<std::vector<int>> layers(max_layer + 1);
        for (int i = 1; i <= n; i++) if (layer[i] != -1) layers[layer[i]].push_back(i);

        // 3. 构建视图节点并分配树状坐标
        QPointF center(350, 300);
        int startY = 80;
        for (int i = 0; i < layers.size(); ++i) {
            int numNodes = layers[i].size();
            int startX = 350 - (numNodes - 1) * 70; // 居中排布，水平间距140
            for (int j = 0; j < numNodes; ++j) {
                int u = layers[i][j];
                GraphNode *node = new GraphNode(u);
                nodes[u] = node;
                scene->addItem(node);

                QPointF targetPos(startX + j * 140, startY + i * 120);
                node->setPos(center);
                node->setOpacity(0);

                QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
                QPropertyAnimation *moveAnim = new QPropertyAnimation(node, "pos");
                moveAnim->setDuration(1000);
                moveAnim->setEndValue(targetPos);
                moveAnim->setEasingCurve(QEasingCurve::OutBack);

                QPropertyAnimation *fadeAnim = new QPropertyAnimation(node, "opacity");
                fadeAnim->setDuration(1000);
                fadeAnim->setEndValue(1.0);

                group->addAnimation(moveAnim); group->addAnimation(fadeAnim);
                group->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }

        // 4. 构建可视化的边 (绑定给源点和目标点)
        line_idx = 1;
        for (int i = 0; i < m && line_idx < lines.size(); ) {
            QStringList uv = lines[line_idx++].split(QRegularExpression("\\s+"));
            if(uv.size() < 2) continue;
            int u = uv[0].toInt(), v = uv[1].toInt();

            GraphEdge *edge = new GraphEdge(nodes[u], nodes[v], is_dir);
            edges.push_back(edge);
            scene->addItem(edge);
            nodes[u]->addEdge(edge); // 将边注册给节点，移动节点时会自动拉扯该边
            nodes[v]->addEdge(edge);
            i++;
        }
    }

    // 后台瞬间执行 Tarjan 并记录步骤
    void tarjan_scc(int u) {
        dfn[u] = low[u] = ++timestamp;
        stk[++top] = u; ins[u] = true;

        anim_queue.push_back({HIGHLIGHT_NODE, u, 0, dfn[u], low[u], {}});
        anim_queue.push_back({PUSH_STACK, u, 0, 0, 0, {}});

        for (int i = head[u]; i; i = nxt[i]) {
            int v = to[i];
            anim_queue.push_back({TRAVERSE_EDGE, u, v, 0, 0, {}});

            if (!dfn[v]) {
                tarjan_scc(v);
                low[u] = std::min(low[u], low[v]);
                anim_queue.push_back({UPDATE_LOW, u, 0, dfn[u], low[u], {}});
            } else if (ins[v]) {
                low[u] = std::min(low[u], dfn[v]);
                anim_queue.push_back({UPDATE_LOW, u, 0, dfn[u], low[u], {}});
            }
        }

        if (dfn[u] == low[u]) {
            ++scc_cnt;
            std::vector<int> current_scc;
            int y;
            do {
                y = stk[top--];
                ins[y] = false;
                scc[y] = scc_cnt;
                current_scc.push_back(y);
                anim_queue.push_back({POP_STACK, y, 0, 0, 0, {}});
            } while (y != u);
            anim_queue.push_back({CONDENSE, u, 0, scc_cnt, 0, current_scc}); // 缩点
        }
    }

    void tarjan_cut(int u, int in_edge) {
        dfn[u] = low[u] = ++timestamp;
        anim_queue.push_back({HIGHLIGHT_NODE, u, 0, dfn[u], low[u], {}});
        int child = 0;

        for (int i = head[u]; i; i = nxt[i]) {
            int v = to[i];
            anim_queue.push_back({TRAVERSE_EDGE, u, v, 0, 0, {}});
            if (!dfn[v]) {
                child++;
                tarjan_cut(v, i);
                low[u] = std::min(low[u], low[v]);
                anim_queue.push_back({UPDATE_LOW, u, 0, dfn[u], low[u], {}});

                if (low[v] >= dfn[u] && u != 1) anim_queue.push_back({MARK_CUT, u, 0, 1, 0, {}}); // 割点
                if (low[v] > dfn[u]) anim_queue.push_back({MARK_CUT, u, v, 2, 0, {}}); // 割边
            } else if (i != (in_edge ^ 1)) {
                low[u] = std::min(low[u], dfn[v]);
                anim_queue.push_back({UPDATE_LOW, u, 0, dfn[u], low[u], {}});
            }
        }
        if (u == 1 && child >= 2) anim_queue.push_back({MARK_CUT, u, 0, 1, 0, {}});
    }

    void runAlgorithm() {
        anim_queue.clear();
        current_anim_index = 0;
        memset(dfn, 0, sizeof(dfn)); memset(low, 0, sizeof(low)); memset(ins, 0, sizeof(ins));
        timestamp = 0; top = 0; scc_cnt = 0;

        bool is_dir = modeSelect->currentIndex() == 0;
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) {
                if (is_dir) tarjan_scc(i);
                else {
                    tot = 1;
                    tarjan_cut(i, 0);
                }
            }
        }
    }

    void playNextAnim() {
        if (current_anim_index >= anim_queue.size()) return;
        AnimStep step = anim_queue[current_anim_index++];
        GraphNode *nu = nodes[step.u];

        if (step.type == HIGHLIGHT_NODE) {
            nu->color = QColor(255, 230, 180); // 浅黄色代表访问过
            nu->updateData(step.val1, step.val2);

            QPropertyAnimation *scaleAnim = new QPropertyAnimation(nu, "scale");
            scaleAnim->setDuration(300);
            scaleAnim->setKeyValueAt(0, 1.0);
            scaleAnim->setKeyValueAt(0.5, 1.3);
            scaleAnim->setKeyValueAt(1.0, 1.0);
            scaleAnim->start(QAbstractAnimation::DeleteWhenStopped);

        } else if (step.type == TRAVERSE_EDGE) {
            for (auto e : edges) {
                if (e->source->id == step.u && e->dest->id == step.v) {
                    e->color = Qt::red;
                    e->update();
                    QTimer::singleShot(500, [e](){ e->color = Qt::black; e->update(); });
                }
            }
        } else if (step.type == UPDATE_LOW) {
            nu->updateData(step.val1, step.val2);
            QPropertyAnimation *scaleAnim = new QPropertyAnimation(nu, "scale");
            scaleAnim->setDuration(200);
            scaleAnim->setKeyValueAt(0.5, 1.2);
            scaleAnim->setKeyValueAt(1.0, 1.0);
            scaleAnim->start(QAbstractAnimation::DeleteWhenStopped);

        } else if (step.type == PUSH_STACK) {
            nu->in_stack = true;
            nu->update();

            StackBlock *block = new StackBlock(step.u);
            block->setPos(nu->pos()); // 从该节点的绝对坐标起飞
            scene->addItem(block);
            stack_ui.push_back(block);

            QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
            QPropertyAnimation *move = new QPropertyAnimation(block, "pos");
            move->setEndValue(stack_base_pos - QPointF(0, stack_ui.size() * 35));
            move->setDuration(400);
            move->setEasingCurve(QEasingCurve::InOutQuad);
            QPropertyAnimation *fade = new QPropertyAnimation(block, "opacity");
            fade->setEndValue(1.0); fade->setDuration(400);

            group->addAnimation(move); group->addAnimation(fade);
            group->start(QAbstractAnimation::DeleteWhenStopped);

        } else if (step.type == POP_STACK) {
            nu->in_stack = false;
            nu->update();
            if (!stack_ui.empty()) {
                StackBlock *block = stack_ui.back();
                stack_ui.pop_back();

                QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
                QPropertyAnimation *move = new QPropertyAnimation(block, "pos");
                move->setEndValue(nu->pos()); // 飞回至节点并消失
                move->setDuration(400);
                QPropertyAnimation *fade = new QPropertyAnimation(block, "opacity");
                fade->setEndValue(0.0); fade->setDuration(400);

                connect(group, &QParallelAnimationGroup::finished, [block](){ delete block; });
                group->addAnimation(move); group->addAnimation(fade);
                group->start(QAbstractAnimation::DeleteWhenStopped);
            }
        } else if (step.type == CONDENSE) {
            QColor sccColor = QColor::fromHsv((step.val1 * 60) % 360, 150, 255);
            for (int vid : step.group) {
                GraphNode *nv = nodes[vid];
                nv->color = sccColor;
                if (vid != step.u) {
                    QPropertyAnimation *move = new QPropertyAnimation(nv, "pos");
                    move->setEndValue(nu->pos()); // 吸附合并到SCC根节点
                    move->setDuration(600);
                    move->setEasingCurve(QEasingCurve::InCubic);
                    move->start(QAbstractAnimation::DeleteWhenStopped);
                }
            }
        } else if (step.type == MARK_CUT) {
            if (step.val1 == 1) { nu->color = Qt::magenta; nu->update(); } // 割点
            else if (step.val1 == 2) {
                for (auto e : edges) {
                    if ((e->source->id == step.u && e->dest->id == step.v) ||
                        (e->source->id == step.v && e->dest->id == step.u)) {
                        e->color = Qt::magenta; e->update();
                    }
                }
            }
        }
        scene->update();
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TarjanVisualizer w;
    w.setWindowTitle("Tarjan Visualizer Pro - SCC / 缩点 / 割边");
    w.show();
    return a.exec();
}

#include "main.moc"