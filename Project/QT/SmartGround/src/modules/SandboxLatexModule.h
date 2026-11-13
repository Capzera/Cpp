#ifndef SANDBOXLATEXMODULE_H
#define SANDBOXLATEXMODULE_H
#include "FreeStructureBase.h"
#include "render/LatexBlock.h"

class SandboxLatexModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxLatexModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;

private:
    std::vector<LatexBlock*> m_blocks;
};

#endif
