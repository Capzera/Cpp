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
    m_mathText = scene->addText(getInputHint());
    m_mathText->setFont(QFont("Consolas", 12, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 20);

    const QVector<LatexBlock::Formula>& lib = LatexBlock::library();
    if (lib.isEmpty()) {
        m_mathText->setPlainText("公式库为空：请先运行 npm install 并执行 node tools/latex/render.js 生成 src/render/latex_formulas.json");
        return;
    }

    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    double y = 140.0;
    for (const QString& line : lines) {
        bool ok = false;
        int index = line.trimmed().toInt(&ok);
        if (!ok) continue;
        int idx = index - 1;
        if (idx < 0 || idx >= lib.size()) continue;
        LatexBlock* block = new LatexBlock();
        block->setFormulaIndex(idx);
        block->setTint(UiTheme::textPrimary());
        block->setScaleFactor(0.62);
        block->setPos(360, y);
        scene->addItem(block);
        m_blocks.push_back(block);
        y += 96.0;

        QGraphicsTextItem* caption = scene->addText(lib[idx].tex);
        caption->setFont(QFont("Consolas", 9));
        caption->setDefaultTextColor(UiTheme::textMuted());
        caption->setPos(360 - 260, y - 96.0 + 30.0);
        caption->setZValue(1);
    }
    if (m_blocks.empty()) {
        m_mathText->setPlainText(QString("公式库共 %1 条，输入编号即可插入（例如输入 1）").arg(lib.size()));
        return;
    }
    m_mathText->setPlainText(QString("已插入 %1 个公式　（公式库共 %2 条，可自由拖动）").arg(m_blocks.size()).arg(lib.size()));
}

void SandboxLatexModule::reset() {
    FreeStructureBase::reset();
    m_blocks.clear();
}
