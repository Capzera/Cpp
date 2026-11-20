#include "SandboxMatrixModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

namespace {
const char* kShapeNames[3] = {"矩阵", "行列式", "表格"};
}

SandboxMatrixModule::SandboxMatrixModule() {}

int SandboxMatrixModule::shapeIndexOf(const QString& name) {
    for (int i = 0; i < 3; ++i) {
        if (name == QLatin1String(kShapeNames[i])) return i;
    }
    return -1;
}

QString SandboxMatrixModule::getModuleName() const { return "自由结构 - 矩阵 / 行列式"; }

QString SandboxMatrixModule::getDefaultInput() const {
    return "1 2 3\n4 5 6\n7 8 9";
}

QString SandboxMatrixModule::getInputHint() const {
    return "数据输入区只需按行优先顺序填写矩阵元素：\n"
           "从左到右填完第一行，再填第二行，以此类推；\n"
           "每行填的个数应与「矩阵设置」中的列数一致，不够时自动补 0。\n"
           "行数与列数在左侧「矩阵设置」面板中调整（默认 3 行 3 列）。\n"
           "「矩阵设置」是本模块的特化面板，只有选中本模块时才会出现。\n"
           "画布上的单元格可双击就地修改数值。";
}

QString SandboxMatrixModule::moduleSettingsTitle() const {
    return "矩阵设置";
}

QVector<ModuleField> SandboxMatrixModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    fields.append(ModuleField::makeSegment("shape", "形态",
                                            {"矩阵", "行列式", "表格"}, m_shapeIndex));
    fields.append(ModuleField::makeInt("rows", "行数", m_rows, 1, 8));
    fields.append(ModuleField::makeInt("cols", "列数", m_cols, 1, 8));
    fields.append(ModuleField::makeSize("cell", "单元格大小",
                                        m_cell, 34, 120, m_cellHeight, 26, 120));
    return fields;
}

void SandboxMatrixModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    if (key == "shape") {
        m_shapeIndex = qBound(0, field.intValue, 2);
        m_shapeOverridden = true;
        applyShape();
    } else if (key == "rows") {
        m_rows = qBound(1, field.intValue, 8);
        m_sizeOverridden = true;
        applyDimensions();
    } else if (key == "cols") {
        m_cols = qBound(1, field.intValue, 8);
        m_sizeOverridden = true;
        applyDimensions();
    } else if (key == "cell") {
        m_cell = qBound(34, field.intValue, 120);
        m_cellHeight = qBound(26, field.secondaryIntValue, 120);
        if (m_matrix) {
            m_matrix->setCellSize(m_cell);
            m_matrix->setCellHeight(m_cellHeight);
            m_matrix->update();
        }
    }
}

void SandboxMatrixModule::applyShape() {
    if (!m_matrix) return;
    MatrixBlock::Shape shape = MatrixBlock::PlainMatrix;
    if (m_shapeIndex == 1) shape = MatrixBlock::Determinant;
    else if (m_shapeIndex == 2) shape = MatrixBlock::BracketedTable;
    m_matrix->setShape(shape);
    if (shape == MatrixBlock::Determinant) {
        m_matrix->cellColor = QColor(253, 250, 242);
        m_matrix->gridColor = QColor(206, 190, 158);
        m_matrix->barColor = QColor(150, 120, 70);
        m_matrix->setTitle("行列式");
    } else if (shape == MatrixBlock::BracketedTable) {
        m_matrix->cellColor = QColor(255, 255, 255);
        m_matrix->gridColor = QColor(158, 170, 188);
        m_matrix->barColor = QColor(50, 70, 105);
        m_matrix->setTitle("表格");
    } else {
        m_matrix->cellColor = QColor(255, 255, 255);
        m_matrix->gridColor = QColor(158, 170, 188);
        m_matrix->barColor = QColor(40, 70, 110);
        m_matrix->setTitle("矩阵");
    }
    m_matrix->update();
}

void SandboxMatrixModule::applyDimensions() {
    if (!m_matrix) return;
    m_matrix->resizeGrid(m_rows, m_cols);
    applyShape();
}

void SandboxMatrixModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    const bool shapeOverridden = m_shapeOverridden;
    const bool sizeOverridden = m_sizeOverridden;
    const int keptShape = m_shapeIndex;
    const int keptRows = m_rows;
    const int keptCols = m_cols;
    const int keptCell = m_cell;
    const int keptCellHeight = m_cellHeight;
    reset();
    m_scene = scene;
    m_shapeOverridden = shapeOverridden;
    m_sizeOverridden = sizeOverridden;
    m_shapeIndex = keptShape;
    m_rows = keptRows;
    m_cols = keptCols;
    m_cell = keptCell;
    m_cellHeight = keptCellHeight;

    int rows = m_rows;
    int cols = m_cols;
    int shapeIndex = m_shapeIndex;
    int cell = m_cell;
    int cellHeight = m_cellHeight;
    QStringList values;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        const QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for (const QString& token : tokens) values.append(token);
    }

    if (!m_sizeOverridden) {
        m_rows = qBound(1, rows, 8);
        m_cols = qBound(1, cols, 8);
    }
    if (!m_shapeOverridden) m_shapeIndex = shapeIndex;
    m_cell = qBound(34, cell, 120);
    m_cellHeight = qBound(26, cellHeight, 120);

    rebuildMatrix(m_rows, m_cols);
    fillFromTokens(values);
}

void SandboxMatrixModule::fillFromTokens(const QStringList& tokens) {
    if (!m_matrix) return;
    int index = 0;
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            const QString text = index < tokens.size() ? tokens[index] : QString("0");
            m_matrix->setCell(r, c, text);
            ++index;
        }
    }
}

void SandboxMatrixModule::rebuildMatrix(int rows, int cols) {
    if (!m_scene) return;
    if (m_matrix) {
        m_matrix->setParentItem(nullptr);
        if (m_matrix->scene()) m_matrix->scene()->removeItem(m_matrix);
        delete m_matrix;
        m_matrix = nullptr;
    }
    m_matrix = new MatrixBlock(rows, cols);
    m_matrix->setEditable(true);
    m_matrix->setCellSize(m_cell);
    m_matrix->setCellHeight(m_cellHeight);
    m_matrix->setPos(220, 220);
    m_scene->addItem(m_matrix);
    applyShape();
    connect(m_matrix, &MatrixBlock::cellEdited, this, [this](int row, int col, const QString& text) {
        Q_UNUSED(text);
        if (m_matrix) m_matrix->setCell(row, col, text);
    });
}

void SandboxMatrixModule::reset() {
    FreeStructureBase::reset();
    if (m_matrix) {
        m_matrix->setParentItem(nullptr);
        if (m_matrix->scene()) m_matrix->scene()->removeItem(m_matrix);
        delete m_matrix;
        m_matrix = nullptr;
    }
    m_shapeOverridden = false;
    m_sizeOverridden = false;
    m_shapeIndex = 2;
    m_rows = 3;
    m_cols = 3;
    m_cell = 52;
    m_cellHeight = 52;
}

void SandboxMatrixModule::releaseSceneItems() {
    FreeStructureBase::releaseSceneItems();
    if (m_matrix) {
        m_matrix->setParentItem(nullptr);
        if (m_matrix->scene()) m_matrix->scene()->removeItem(m_matrix);
        delete m_matrix;
        m_matrix = nullptr;
    }
}
