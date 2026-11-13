#ifndef SELECTIONSORTMODULE_H
#define SELECTIONSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
class SelectionSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    SelectionSortModule();
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
	int m_startX;
	std::vector<int> m_data;
	std::map<int, ArrayBlock*> m_blocks;
	std::vector<AnimFrame> m_frames;
	PointerBlock* m_ptrI;
	ArrayBlock* m_minBlock;
	ArrayBlock* m_minIdxBlock;
};
#endif
