#ifndef QUICKSORTMODULE_H
#define QUICKSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>

class QuickSortModule : public QObject, public IModule {
	Q_OBJECT
public:
	QuickSortModule();
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
	void quickSort(std::vector<int>& arr, int l, int r, int depth);
	
	std::vector<AnimFrame> m_frames;
	std::map<int, ArrayBlock*> m_blocks;
	QGraphicsTextItem* m_mathText;
	int m_startX;
	std::vector<int> m_data;
	
	PointerBlock* m_ptrI;
	PointerBlock* m_ptrJ;
	ArrayBlock* m_pivotBlock;
};
#endif
