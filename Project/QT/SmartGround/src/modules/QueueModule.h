#ifndef QUEUEMODULE_H
#define QUEUEMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>
class QueueModule : public QObject, public IModule {
	Q_OBJECT
public:
	QueueModule();
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
	int m_capacity;
	std::vector<QString> m_operations;
	std::map<int, ArrayBlock*> m_blocks;
	PointerBlock* m_ptrHead;
	PointerBlock* m_ptrTail;
	int m_startX;
	std::vector<AnimFrame> m_frames;
};
#endif
