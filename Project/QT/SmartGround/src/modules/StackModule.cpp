#include "StackModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QFont>
StackModule::StackModule() : m_capacity(0), m_ptrTop(nullptr) {}
QString StackModule::getModuleName() const { return "栈"; }
QString StackModule::getDefaultInput() const { return "8\nPUSH 10\nPUSH 20\nPUSH 30\nPOP\nPUSH 40"; }
QString StackModule::getInputHint() const {
    return "第一行输入一个正整数 N，表示栈的容量上限\n"
           "接下来每行一条操作指令：\n"
           "  PUSH x  把 x 压入栈顶\n"
           "  POP     弹出栈顶元素";
}
void StackModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_operations.clear(); m_ptrTop = nullptr;
    QStringList lines = inputData.split('\n', Qt::SkipEmptyParts);
    if (lines.isEmpty()) return;
    m_capacity = lines[0].trimmed().toInt();
    if (m_capacity < 1) m_capacity = 1;
    if (m_capacity > 60) m_capacity = 60;
    for (int i = 1; i < lines.size(); ++i) {
        QString op = lines[i].trimmed();
        if (!op.isEmpty()) m_operations.push_back(op);
    }
    m_startX = 400 - ((m_capacity + 1) * 60) / 2;
    QGraphicsTextItem *label = scene->addText("栈 stk[N]");
    label->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); label->setPos(m_startX - 120, 300);
    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold)); m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 100);
    for (int i = 0; i <= m_capacity; ++i) {
        ArrayBlock* b = new ArrayBlock(i, 0); b->setPos(m_startX + i * 60, 300);
        b->showValue = false; m_scene->addItem(b); m_blocks[i] = b;
    }
    m_ptrTop = new PointerBlock("top", QColor(220, 50, 50));    m_ptrTop->setPos(m_startX + 0 * 60, 340); m_scene->addItem(m_ptrTop);
}
std::vector<AnimFrame> StackModule::runAlgorithm() {
    m_frames.clear();
    int top = 0;
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "初始化栈", "int stk[N], top = 0;"});
    for (const QString& op : m_operations) {
        QStringList parts = op.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;
        if (parts[0].toUpper() == "PUSH" && parts.size() >= 2) {
            if (top >= m_capacity) {
                m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "栈已满，忽略本次入栈", "if (top < N) stk[++top] = x;"});
                continue;
            }
            int val = parts[1].toInt();
            top++;
            m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, top, 0, 0, {}, QString("top 指针上移"), QString("stk[++top] = %1;").arg(val)});
            m_frames.push_back({ActionType::SQ_UPDATE_VAL, top, 0, val, 0, {}, QString("元素 %1 入栈").arg(val), ""});
        } else if (parts[0].toUpper() == "POP") {
            if (top > 0) {
                m_frames.push_back({ActionType::SQ_CLEAR_VAL, top, 0, 0, 0, {}, "元素出栈", "top--;"});
                top--;
                m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, top, 0, 0, {}, QString("top 指针回退"), ""});
            }
        }
    }
    return m_frames;
}
ArrayBlock* StackModule::getBlock(int id) { return m_blocks.count(id) ? m_blocks[id] : nullptr; }
void StackModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) m_mathText->setPlainText(step.mathText);
    if (step.type == ActionType::SQ_POINTER_MOVE) {
        if (m_ptrTop) {
            QPointF targetPos(m_startX + step.v * 60, 340);
            if (m_animated) {
                QPropertyAnimation* anim = acquireAnim(m_ptrTop, "pos", 400, QEasingCurve::InOutQuad);
                if (anim) {
                    anim->setStartValue(m_ptrTop->pos());
                    anim->setEndValue(targetPos);
                    anim->start();
                }
            } else { m_ptrTop->setPos(targetPos); }
        }
    } else if (step.type == ActionType::SQ_UPDATE_VAL) {
        ArrayBlock* b = getBlock(step.u);
        if (b) {
            b->value = step.val1; b->showValue = true; b->update();
            playScalePulse(b, 1.3, 300);
        }
    } else if (step.type == ActionType::SQ_CLEAR_VAL) {
        ArrayBlock* b = getBlock(step.u);
        if (b) {
            b->showValue = false; b->update();
            playScalePulse(b, 0.7, 300);
        }
    }
}
void StackModule::reset() {
	m_blocks.clear();
	m_frames.clear();
	m_operations.clear();
	m_ptrTop = nullptr;
}
void StackModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrTop = nullptr;
    m_mathText = nullptr;
}
