#ifndef SANDBOXTREE_MODULE_H
#define SANDBOXTREE_MODULE_H
#include "FreeStructureBase.h"
#include <map>

class SandboxTreeModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxTreeModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;

private:
    void layoutTree(int root, const std::map<int, std::vector<int>>& children);
    void onNodeValueEdited(int id, const QString& text);

    std::map<int, GraphNode*> m_nodes;
    std::map<int, QString> m_values;
};

#endif
