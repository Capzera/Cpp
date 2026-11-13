#include "SandboxArrayModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

SandboxArrayModule::SandboxArrayModule() {}

QString SandboxArrayModule::getModuleName() const { return "自由结构 - 数组"; }
QString SandboxArrayModule::getDefaultInput() const { return "5 1 2 3 4 5"; }
QString SandboxArrayModule::getInputHint() const {
    return "第一行输入 n，表示数组长度\n"
           "接下来一行输入 n 个整数，作为数组初始值\n"
           "提示：直接双击画布中的任意格子即可修改数值";
}

void SandboxArrayModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    m_mathText = scene->addText(getInputHint());
    m_mathText->setFont(QFont("Consolas", 12, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 20);

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = tokens.isEmpty() ? 0 : tokens[0].toInt(&ok);
    if (!ok || n < 1 || n > 24) {
        m_mathText->setPlainText("输入错误：第一行需要数组长度 n（1 - 24）");
        return;
    }
    std::vector<int> values(n, 0);
    for (int i = 0; i < n; ++i) {
        int v = i + 1 < tokens.size() ? tokens[i + 1].toInt(&ok) : 0;
        values[i] = ok ? v : 0;
    }

    const double cellW = 62.0;
    const double startX = 160.0;
    const double y = 260.0;
    QGraphicsTextItem* title = scene->addText("a");
    title->setFont(QFont("Consolas", 13, QFont::Bold));
    title->setDefaultTextColor(QColor(40, 70, 110));
    title->setPos(startX - 42, y - 2);

    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i + 1, values[i], QString::number(i + 1));
        block->setCellSize(cellW, 40.0);
        block->setPos(startX + i * cellW, y);
        block->color = QColor(255, 250, 230);
        block->setEditable(true);
        scene->addItem(block);
        connect(block, &ArrayBlock::valueEdited, this, &SandboxArrayModule::onCellEdited);
        m_blocks[i + 1] = block;
    }
    m_mathText->setPlainText(QString("自由数组：长度 %1，下标 1..%2　（双击格子可改值）").arg(n).arg(n));
}

void SandboxArrayModule::onCellEdited(int id, const QString& text) {
    auto it = m_blocks.find(id);
    if (it == m_blocks.end()) return;
    ArrayBlock* block = it->second;
    bool ok = false;
    int v = text.toInt(&ok);
    if (ok) {
        block->value = v;
        block->customValue.clear();
        block->showValue = true;
    } else {
        block->setCustomValue(text);
        block->showValue = true;
    }
    block->color = QColor(255, 244, 200);
    block->update();
}

void SandboxArrayModule::reset() {
    FreeStructureBase::reset();
    m_blocks.clear();
}
