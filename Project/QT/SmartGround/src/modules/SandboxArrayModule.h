#ifndef SANDBOXARRAYMODULE_H
#define SANDBOXARRAYMODULE_H
#include "FreeStructureBase.h"
#include <map>

class SandboxArrayModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxArrayModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;

private:
    void onCellEdited(int id, const QString& text);
    std::map<int, ArrayBlock*> m_blocks;
};

#endif
