#ifndef DIFFERENCEMODULE_H
#define DIFFERENCEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>

class DifferenceModule : public QObject, public IModule {
	Q_OBJECT
public:
	DifferenceModule();
	QString getModuleName() const override;
	QString getDefaultInput() const override;
	void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
	std::vector<AnimFrame> runAlgorithm() override;
	void renderFrame(const AnimFrame& frame) override;
	void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
    bool supportsLiveEditing() const override { return true; }
private:
	ArrayBlock* getBlock(int id);
	QGraphicsTextItem* m_mathText;
	int m_n, m_m;
	std::vector<int> m_dataA, m_dataD;
	struct Query { int l, r, x; };
	std::vector<Query> m_queries;
	std::map<int, ArrayBlock*> m_blocks; 
	std::vector<AnimFrame> m_frames;
};
#endif
