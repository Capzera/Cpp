#include "TarjanModule.h"
#include "GraphModuleUtils.h"
#include "GraphRenderHelper.h"
#include <QRegularExpression>
#include <queue>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPointer>
TarjanModule::TarjanModule() : stack_base_pos(750, 500) { initGraph(); }
QString TarjanModule::getModuleName() const { return "Tarjan 强连通分量"; }
QString TarjanModule::getDefaultInput() const {
    return "6 8\n1 2\n2 3\n3 1\n3 4\n4 5\n5 6\n6 4\n2 6";
}
bool TarjanModule::supportsGraphOptions() const { return true; }
bool TarjanModule::supportsDirected() const { return false; }
bool TarjanModule::supportsBidirectional() const { return false; }
GraphModuleUtils::GraphOptions TarjanModule::getGraphOptions() const { return m_options; }
void TarjanModule::setGraphOptions(const GraphModuleUtils::GraphOptions& options) { m_options = options; }
QString TarjanModule::getInputHint() const {
    return "第一行输入两个正整数：顶点数 n、边数 m\n"
           "接下来 m 行，每行两个整数 u, v，表示一条有向边 u -> v\n"
           "本模块固定按有向图处理，用于求强连通分量";
}
std::map<int, QPointF> TarjanModule::captureNodePositions() const {
    std::map<int, QPointF> result;
    for (const auto& pair : nodes) result[pair.first] = pair.second->pos();
    return result;
}
void TarjanModule::restoreNodePositions(const std::map<int, QPointF>& positions) {
    for (const auto& pair : positions) {
        auto it = nodes.find(pair.first);
        if (it != nodes.end()) it->second->setPos(pair.second);
    }
}
void TarjanModule::initGraph() {
	tot = 0;
	memset(head, 0, sizeof(head));
	memset(to, 0, sizeof(to));
	memset(nxt, 0, sizeof(nxt));
	resetTraversalState();
}
void TarjanModule::resetTraversalState() {
    timestamp = 0; top = 0; scc_cnt = 0;
	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
	memset(stk, 0, sizeof(stk));
	memset(ins, 0, sizeof(ins));
	memset(scc, 0, sizeof(scc));
}
void TarjanModule::addEdge(int u, int v) {
	to[++tot] = v; nxt[tot] = head[u]; head[u] = tot;
}
void TarjanModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
	m_scene = scene; nodes.clear(); edges.clear(); stack_ui.clear(); m_frames.clear(); initGraph();
	QGraphicsTextItem *stkText = scene->addText("ins / stk");
	stkText->setPos(700, 50); stkText->setFont(QFont("Arial", 14, QFont::Bold));
	GraphModuleUtils::GraphInput input;
	GraphModuleUtils::GraphOptions options = m_options;
	options.directed = true;
	bool valid = GraphModuleUtils::parse(inputData, false, options, input);
	if (!valid) return;
	n = input.n; m = input.m;
	for (const auto& edge : input.edges) addEdge(edge.u, edge.v);
	std::vector<int> inD(n + 1, 0), layer(n + 1, -1);
	for (int u = 1; u <= n; ++u) for (int e = head[u]; e; e = nxt[e]) inD[to[e]]++;
	std::queue<int> q; int max_l = 0;
	for (int i = 1; i <= n; i++) if (inD[i] == 0) { layer[i] = 0; q.push(i); }
	if (q.empty() && n > 0) { layer[1] = 0; q.push(1); }
	auto bfs = [&]() {
		while (!q.empty()) {
			int u = q.front(); q.pop(); max_l = std::max(max_l, layer[u]);
			for (int e = head[u]; e; e = nxt[e]) if (layer[to[e]] == -1) { layer[to[e]] = layer[u] + 1; q.push(to[e]); }
		}
	};
	bfs();
	for (int i = 1; i <= n; i++) if (layer[i] == -1) { layer[i] = max_l + 1; q.push(i); bfs(); }
	std::vector<std::vector<int>> layers(max_l + 1);
	for (int i = 1; i <= n; i++) if (layer[i] != -1) layers[layer[i]].push_back(i);
	QPointF center(350, 300);
	for (int i = 0; i < layers.size(); ++i) {
		int sz = layers[i].size(); int sx = 350 - (sz - 1) * 70;
		for (int j = 0; j < sz; ++j) {
			int u = layers[i][j]; nodes[u] = new GraphNode(u); scene->addItem(nodes[u]);
			QPointF targetPos(sx + j * 140, 80 + i * 120);
			if (m_animated) {
				nodes[u]->setPos(center); nodes[u]->setOpacity(0);
				QParallelAnimationGroup *grp = new QParallelAnimationGroup(this);
				QPropertyAnimation *mAnim = new QPropertyAnimation(nodes[u], "pos");
				mAnim->setDuration(800); mAnim->setEndValue(targetPos);
				mAnim->setEasingCurve(QEasingCurve::OutBack);
				QPropertyAnimation *fAnim = new QPropertyAnimation(nodes[u], "opacity");
				fAnim->setDuration(800); fAnim->setEndValue(1.0);
				grp->addAnimation(mAnim); grp->addAnimation(fAnim);
				grp->start(QAbstractAnimation::DeleteWhenStopped);
			} else {
				nodes[u]->setPos(targetPos); nodes[u]->setOpacity(1.0);
			}
		}
    }
	for (const auto& edge : input.edges) {
		GraphEdge *item = new GraphEdge(nodes[edge.u], nodes[edge.v], true);
		if (input.weighted) item->setWeight(edge.w);
		edges.push_back(item); scene->addItem(item);
		nodes[edge.u]->addEdge(item); nodes[edge.v]->addEdge(item);
	}
}
void TarjanModule::tarjan(int u) {
	dfn[u] = low[u] = ++timestamp; stk[++top] = u; ins[u] = true;
	m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, u, 0, dfn[u], low[u], {}, QString("访问节点%1").arg(u)});
	m_frames.push_back({ActionType::GN_PUSH_STACK, u, 0, 0, 0, {}, QString("节点%1入栈").arg(u)});
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		m_frames.push_back({ActionType::GN_TRAVERSE_EDGE, u, v, 0, 0, {}, QString("检查边 %1->%2").arg(u).arg(v)});
		if (!dfn[v]) {
			tarjan(v); low[u] = std::min(low[u], low[v]);
			m_frames.push_back({ActionType::GN_UPDATE_LOW, u, 0, dfn[u], low[u], {}, QString("回溯更新low[%1]=%2").arg(u).arg(low[u])});
		} else if (ins[v]) {
			low[u] = std::min(low[u], dfn[v]);
			m_frames.push_back({ActionType::GN_UPDATE_LOW, u, 0, dfn[u], low[u], {}, QString("返祖更新low[%1]=%2").arg(u).arg(low[u])});
		}
	}
	if (dfn[u] == low[u]) {
		++scc_cnt; std::vector<int> cur_scc; int y;
		do {
			y = stk[top--]; ins[y] = false; scc[y] = scc_cnt; cur_scc.push_back(y);
			m_frames.push_back({ActionType::GN_POP_STACK, y, 0, 0, 0, {}, QString("节点%1出栈").arg(y)});
		} while (y != u);
		m_frames.push_back({ActionType::GN_CONDENSE, u, 0, scc_cnt, 0, cur_scc, QString("找到强连通分量%1并缩点").arg(scc_cnt)});
	}
}
std::vector<AnimFrame> TarjanModule::runAlgorithm() {
	m_frames.clear(); resetTraversalState();
	for (int i = 1; i <= n; i++) if (!dfn[i]) tarjan(i);
	return m_frames;
}
void TarjanModule::renderFrame(const AnimFrame& step) {
    GraphNode *nu = nodes[step.u];
    if (step.type == ActionType::GN_HIGHLIGHT_NODE) {
        nu->color = QColor(255, 230, 180); nu->updateData(step.val1, step.val2);
        playScalePulse(nu, 1.3, 300);
    } else if (step.type == ActionType::GN_TRAVERSE_EDGE) {
        for (auto e : edges) if (e->source->id == step.u && e->dest->id == step.v) {
                if (m_animated) {
                    e->color = Qt::red; e->update();
                    QPointer<GraphEdge> safeEdge = e;
                    QTimer::singleShot(400, [safeEdge](){ if (safeEdge) { safeEdge->color = Qt::black; safeEdge->update(); } });
                } else { e->color = Qt::black; e->update(); }
            }
    } else if (step.type == ActionType::GN_UPDATE_LOW) {
        nu->updateData(step.val1, step.val2);
        playScalePulse(nu, 1.2, 200);
    } else if (step.type == ActionType::GN_PUSH_STACK) {
        nu->in_stack = true; nu->update();
        StackBlock *blk = new StackBlock(step.u); blk->setPos(nu->pos());
        m_scene->addItem(blk); stack_ui.push_back(blk);
        if (m_animated) {
            QParallelAnimationGroup *grp = new QParallelAnimationGroup(this);
            QPropertyAnimation *mv = new QPropertyAnimation(blk, "pos");
            mv->setEndValue(stack_base_pos - QPointF(0, stack_ui.size() * 35)); mv->setDuration(400);
            QPropertyAnimation *fd = new QPropertyAnimation(blk, "opacity"); fd->setEndValue(1.0); fd->setDuration(400);
            grp->addAnimation(mv); grp->addAnimation(fd); grp->start(QAbstractAnimation::DeleteWhenStopped);
        } else {
            blk->setPos(stack_base_pos - QPointF(0, stack_ui.size() * 35)); blk->setOpacity(1.0);
        }
    } else if (step.type == ActionType::GN_POP_STACK) {
        nu->in_stack = false; nu->update();
        if (!stack_ui.empty()) {
            StackBlock *blk = stack_ui.back(); stack_ui.pop_back();
            if (m_animated) {
                QPointer<StackBlock> safeBlk = blk;
                QParallelAnimationGroup *grp = new QParallelAnimationGroup(this);
                QPropertyAnimation *mv = new QPropertyAnimation(blk, "pos"); mv->setEndValue(nu->pos()); mv->setDuration(400);
                QPropertyAnimation *fd = new QPropertyAnimation(blk, "opacity"); fd->setEndValue(0.0); fd->setDuration(400);
                connect(grp, &QParallelAnimationGroup::finished, [this, safeBlk](){
                    if (!safeBlk) return;
                    if (safeBlk->scene()) safeBlk->scene()->removeItem(safeBlk.data());
                    delete safeBlk.data();
                });
                grp->addAnimation(mv); grp->addAnimation(fd); grp->start(QAbstractAnimation::DeleteWhenStopped);
            } else {
                if (blk->scene()) blk->scene()->removeItem(blk);
                delete blk;
            }
        }
    } else if (step.type == ActionType::GN_CONDENSE) {
        QColor sccColor = QColor::fromHsv((step.val1 * 60) % 360, 150, 255);
        for (int vid : step.group) {
            GraphNode *nv = nodes[vid]; nv->color = sccColor; nv->update();
            if (vid != step.u) {
                if (m_animated) {
                    QPropertyAnimation *mv = acquireAnim(nv, "pos", 600, QEasingCurve::InCubic);
                    if (mv) {
                        mv->setStartValue(nv->pos());
                        mv->setEndValue(nu->pos());
                        mv->start();
                    }
                } else { nv->setPos(nu->pos()); }
            }
        }
    }
}void TarjanModule::reset() {
	nodes.clear();
	edges.clear();
	stack_ui.clear();
	m_frames.clear();
	initGraph();
}
void TarjanModule::releaseSceneItems() {
    nodes.clear();
    edges.clear();
    stack_ui.clear();
}
