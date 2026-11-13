#ifndef TARJANMODULE_H
#define TARJANMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <map>
#include <QObject>
class TarjanModule : public QObject, public IModule {
	Q_OBJECT
public:
	TarjanModule();
	QString getModuleName() const override;
	QString getDefaultInput() const override;
	void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
	std::vector<AnimFrame> runAlgorithm() override;
	void renderFrame(const AnimFrame& frame) override;
	void reset() override;

	void releaseSceneItems() override;
	bool supportsGraphOptions() const override;
	bool supportsDirected() const override;
	bool supportsBidirectional() const override;
	GraphModuleUtils::GraphOptions getGraphOptions() const override;
	void setGraphOptions(const GraphModuleUtils::GraphOptions& options) override;
	QString getInputHint() const override;
	std::map<int, QPointF> captureNodePositions() const override;
	void restoreNodePositions(const std::map<int, QPointF>& positions) override;
private:
	void initGraph();
	void resetTraversalState();
    void addEdge(int u, int v);
	void tarjan(int u);
	std::map<int, GraphNode*> nodes;
	std::vector<GraphEdge*> edges;
	std::vector<StackBlock*> stack_ui;
	std::vector<AnimFrame> m_frames;
	GraphModuleUtils::GraphOptions m_options;
	int n, m;
	int head[1005], to[5005], nxt[5005], tot;
	int dfn[1005], low[1005], timestamp;
	int stk[1005], top;
    bool ins[1005];
	int scc[1005], scc_cnt;
	QPointF stack_base_pos;
};
#endif
