#ifndef SHELLSORTMODULE_H
#define SHELLSORTMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <QObject>
#include <QGraphicsTextItem>
#include <map>
#include <vector>

class ShellSortModule : public QObject, public IModule {
    Q_OBJECT
public:
    ShellSortModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }
    void onLiveDataChanged(const QString& row, int index, int value) override;

private:
    std::vector<AnimFrame> m_frames;
    std::map<int, ArrayBlock*> m_blocks;
    std::vector<LinkLine*> m_groupLinks;
    std::vector<int> m_data;
    QGraphicsTextItem* m_mathText = nullptr;
    QGraphicsTextItem* m_gapText = nullptr;
    PointerBlock* m_ptrI = nullptr;
    PointerBlock* m_ptrJ = nullptr;
    int m_startX = 0;
    bool m_valid = false;
};

#endif
