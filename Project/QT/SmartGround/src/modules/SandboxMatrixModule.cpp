#include "SandboxMatrixModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

SandboxMatrixModule::SandboxMatrixModule() {}

QString SandboxMatrixModule::getModuleName() const { return "自由结构 - 矩阵 / 行列式"; }
QString SandboxMatrixModule::getDefaultInput() const { return "rows=3\ncols=3\ntype=matrix\n1 2 3\n4 5 6\n7 8 9"; }
QString SandboxMatrixModule::getInputHint() const {
    return "第一行输入 rows = 行数\n"
           "第二行输入 cols = 列数\n"
           "第三行输入 type = matrix（普通矩阵）或 determinant（行列式，加竖线边框）\n"
           "接下来每行输入 cols 个整数，作为矩阵元素\n"
           "提示：行列式会用方括号包围整体，便于区分";
}

void SandboxMatrixModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    m_mathText = scene->addText(getInputHint());
    m_mathText->setFont(QFont("Consolas", 12, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 20);

    std::vector<QPair<QString, QString>> kv;
    parseKeyValues(inputData, kv);
    int rows = 3;
    int cols = 3;
    bool determinant = false;
    for (const auto& pair : kv) {
        if (pair.first == "rows") rows = pair.second.toInt();
        else if (pair.first == "cols") cols = pair.second.toInt();
        else if (pair.first == "type") determinant = (pair.second.trimmed().toLower() == "determinant");
    }
    if (rows < 1 || rows > 8 || cols < 1 || cols > 8) {
        m_mathText->setPlainText("输入错误：rows / cols 需为 1 - 8 的整数");
        return;
    }
    m_determinant = determinant;
    rebuildMatrix(rows, cols);

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    int row = 0;
    for (const QString& line : lines) {
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) continue;
        bool allNumeric = true;
        for (const QString& t : tokens) {
            bool ok = false;
            t.toInt(&ok);
            if (!ok) { allNumeric = false; break; }
        }
        if (!allNumeric) continue;
        for (int j = 0; j < tokens.size() && j < cols; ++j) {
            if (m_matrix) m_matrix->setCell(row, j, tokens[j]);
        }
        ++row;
        if (row >= rows) break;
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
    m_matrix->setTitle(m_determinant ? "det(A)" : "A");
    if (m_determinant) {
        m_matrix->cellColor = QColor(252, 248, 238);
        m_matrix->gridColor = QColor(196, 168, 120);
    } else {
        m_matrix->cellColor = QColor(255, 255, 255);
        m_matrix->gridColor = QColor(150, 162, 180);
    }
    m_matrix->setPos(220, 220);
    m_scene->addItem(m_matrix);
}

void SandboxMatrixModule::reset() {
    FreeStructureBase::reset();
    m_matrix = nullptr;
}
