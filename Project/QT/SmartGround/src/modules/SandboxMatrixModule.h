#ifndef SANDBOXMATRIXMODULE_H
#define SANDBOXMATRIXMODULE_H
#include "FreeStructureBase.h"
#include "render/MatrixBlock.h"

class SandboxMatrixModule : public FreeStructureBase {
    Q_OBJECT
public:
    SandboxMatrixModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    QString getInputHint() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;

private:
    void rebuildMatrix(int rows, int cols);
    MatrixBlock* m_matrix = nullptr;
    bool m_determinant = false;
};

#endif
