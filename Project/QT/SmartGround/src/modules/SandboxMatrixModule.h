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
    QString moduleSettingsTitle() const override;
    QVector<ModuleField> moduleSettingsFields() const override;
    void applyModuleSetting(const QString& key, const ModuleField& field) override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    void reset() override;
    void releaseSceneItems() override;

    MatrixBlock* matrixBlock() const { return m_matrix; }
    int cellWidth() const { return m_cell; }
    int cellHeight() const { return m_cellHeight; }
    int rowCount() const { return m_rows; }
    int columnCount() const { return m_cols; }
    int shapeIndex() const { return m_shapeIndex; }
    int currentShapeIndex() const { return m_shapeIndex; }
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }

private:
    static int shapeIndexOf(const QString& name);
    void rebuildMatrix(int rows, int cols);
    void fillFromTokens(const QStringList& tokens);
    void applyShape();
    void applyDimensions();
    MatrixBlock* m_matrix = nullptr;
    bool m_determinant = false;
    bool m_shapeOverridden = false;
    bool m_sizeOverridden = false;
    int m_shapeIndex = 2;
    int m_rows = 3;
    int m_cols = 3;
    int m_cell = 52;
    int m_cellHeight = 52;
};

#endif
