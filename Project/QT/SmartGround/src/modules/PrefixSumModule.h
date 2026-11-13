#ifndef PREFIXSUMMODULE_H
#define PREFIXSUMMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>

class PrefixSumModule : public QObject, public IModule {
	Q_OBJECT
public:
	PrefixSumModule();
	QString getModuleName() const override;
	QString getDefaultInput() const override;
	void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
	std::vector<AnimFrame> runAlgorithm() override;
	void renderFrame(const AnimFrame& frame) override;
	void reset() override;

	void releaseSceneItems() override;
	QString getInputHint() const override;
	bool supportsLiveEditing() const override { return true; }
	QString editableRowFor(ArrayBlock* block) const override;
	void onLiveDataChanged(const QString& row, int index, int value) override;
private:
	ArrayBlock* getBlock(int id);
	QGraphicsTextItem* m_mathText;
	int m_n, m_m;
	std::vector<int> m_dataA, m_dataS;
	struct Query { int l, r; };
	std::vector<Query> m_queries;
	std::map<int, ArrayBlock*> m_blocks; 
	std::vector<AnimFrame> m_frames;
};
#endif
