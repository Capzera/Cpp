#ifndef SANDBOXBLANKMODULE_H
#define SANDBOXBLANKMODULE_H
#include "FreeStructureBase.h"

class SandboxBlankModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxBlankModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;
};

#endif
