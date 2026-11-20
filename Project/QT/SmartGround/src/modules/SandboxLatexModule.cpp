#include "SandboxLatexModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>
#include <QFileInfo>

SandboxLatexModule::SandboxLatexModule() {}

QString SandboxLatexModule::getModuleName() const { return "自由结构 - LaTeX 公式"; }
QString SandboxLatexModule::getDefaultInput() const { return "1\n2\n3"; }
QString SandboxLatexModule::getInputHint() const {
    return "每行输入一个公式库编号（1 开始），多个公式会纵向排列\n"
           "公式库由构建期 KaTeX / MathJax 渲染为矢量图，缩放不失真\n"
           "提示：公式图元可自由拖动缩放";
}

void SandboxLatexModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QVector<LatexBlock::Formula>& lib = LatexBlock::library();
    if (lib.isEmpty()) return;

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    const double targetH = 56.0;
    const double gapX = 40.0;
    const double gapY = 30.0;
    double x = 200.0;
    double y = 160.0;
    double rowHeight = 0.0;
    for (const QString& line : lines) {
        bool ok = false;
        int index = line.trimmed().toInt(&ok);
        if (!ok) continue;
        int idx = index - 1;
        if (idx < 0 || idx >= lib.size()) continue;
        LatexBlock* block = new LatexBlock();
        block->setFormulaIndex(idx);
        block->setTint(UiTheme::textPrimary());
        block->setTargetHeight(targetH);
        block->setPos(x, y);
        scene->addItem(block);
        m_blocks.push_back(block);
        const double w = block->boundingRect().width();
        x += w / 2.0 + gapX / 2.0;
        rowHeight = qMax(rowHeight, targetH);
        if (x > 760.0) {
            x = 200.0;
            y += rowHeight + gapY;
            rowHeight = 0.0;
        }
    }
}

void SandboxLatexModule::reset() {
    FreeStructureBase::reset();
    m_blocks.clear();
}
