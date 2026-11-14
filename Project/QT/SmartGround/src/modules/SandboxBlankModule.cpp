#include "SandboxBlankModule.h"

SandboxBlankModule::SandboxBlankModule() {}

QString SandboxBlankModule::getModuleName() const { return "自由结构 - 空白画布"; }
QString SandboxBlankModule::getDefaultInput() const { return ""; }
QString SandboxBlankModule::getInputHint() const {
    return "空白画布：下层不放任何算法结构，供老师用画笔批注\n"
           "在右上角切换到「画笔 / 橡皮 / 图形 / 文本 / 笔记 / 公式 / 表格」即可自由绘制\n"
           "提示：标注层与算法层完全分离，不会影响下层数据";
}

void SandboxBlankModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    Q_UNUSED(inputData);
    m_scene = scene;
    reset();
    m_scene = scene;
}

void SandboxBlankModule::reset() {
    FreeStructureBase::reset();
}
