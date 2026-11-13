#ifndef BINARYSEARCHHALFOPENMODULE_H
#define BINARYSEARCHHALFOPENMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>
class BinarySearchHalfOpenModule : public QObject, public IModule {
	Q_OBJECT
public:
	BinarySearchHalfOpenModule();
	QString getModuleName() const override;
	QString getDefaultInput() const override;
	void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
	std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;

    void releaseSceneItems() override;
    QString getInputHint() const override;
private:
	ArrayBlock* getBlock(int id);
	QGraphicsTextItem* m_mathText;
	int m_target;
	int m_n;
	std::vector<int> m_data;
	std::map<int, ArrayBlock*> m_blocks;
	PointerBlock* m_ptrL;
	PointerBlock* m_ptrR;
	PointerBlock* m_ptrMid;
	int m_startX;
	std::vector<AnimFrame> m_frames;
};
#endif
