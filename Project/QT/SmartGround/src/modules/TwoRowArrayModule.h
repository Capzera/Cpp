#ifndef TWOROWARRAYMODULE_H
#define TWOROWARRAYMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <map>
#include <QObject>
#include <QGraphicsTextItem>

class TwoRowArrayModule : public QObject, public IModule {
    Q_OBJECT
public:
    void reset() override;
    void releaseSceneItems() override;
    bool supportsLiveEditing() const override { return true; }

protected:
    struct RowSpec {
        QString row = "a";
        QString title = "a";
        int startIndex = 1;
        int count = 0;
        int offset = 0;
        int keyBase = 0;
        double y = 200.0;
        int hideTail = 0;
    };

    void parseTwoRowInput(const QString& inputData, int& n, int& m, std::vector<int>& first, QString& error);
    void buildTwoRowLayout(QGraphicsScene* scene, const std::vector<int>& first,
                           const std::vector<RowSpec>& rows);
    void renderTwoRowFrame(const AnimFrame& frame);
    ArrayBlock* getBlock(int id) const;

    std::map<int, ArrayBlock*> m_blocks;
    std::vector<AnimFrame> m_frames;
    QGraphicsTextItem* m_mathText = nullptr;
    int m_n = 0;
    int m_m = 0;
};

#endif
