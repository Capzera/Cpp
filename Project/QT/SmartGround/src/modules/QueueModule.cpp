#include "QueueModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QFont>
QueueModule::QueueModule() : m_capacity(0), m_ptrHead(nullptr), m_ptrTail(nullptr) {}
QString QueueModule::getModuleName() const { return "队列"; }
QString QueueModule::getDefaultInput() const { return "8\nPUSH 10\nPUSH 20\nPUSH 30\nPOP\nPUSH 40"; }
QString QueueModule::getInputHint() const {
    return "第一行输入一个正整数 N，表示队列的容量上限\n"
           "接下来每行一条操作指令：\n"
           "  PUSH x  把 x 从队尾入队\n"
           "  POP     从队头出队";
}
void QueueModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_operations.clear();
    m_ptrHead = nullptr; m_ptrTail = nullptr;
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
    QGraphicsTextItem *label = scene->addText("q[N]");
    label->setFont(QFont("Microsoft YaHei", 12, QFont::Bold)); label->setPos(m_startX - 120, 300);
    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold)); m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 100);
    for (int i = 0; i <= m_capacity; ++i) {
        ArrayBlock* b = new ArrayBlock(i, 0); b->setPos(m_startX + i * 60, 300);
        b->showValue = false; m_scene->addItem(b); m_blocks[i] = b;
    }
    m_ptrHead = new PointerBlock("H", QColor(50, 150, 220));
    m_ptrHead->setPos(m_startX + 1 * 60, 340); m_scene->addItem(m_ptrHead);
    m_ptrTail = new PointerBlock("T", QColor(220, 150, 50));
    m_ptrTail->setPos(m_startX + 0 * 60, 340); m_scene->addItem(m_ptrTail);
}
std::vector<AnimFrame> QueueModule::runAlgorithm() {
    m_frames.clear();
    int head = 1, tail = 0;
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "初始化队列", "int q[N], head = 1, tail = 0;"});
    for (const QString& op : m_operations) {
        QStringList parts = op.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;
        if (parts[0].toUpper() == "PUSH" && parts.size() >= 2) {
            if (tail >= m_capacity) {
                m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "队列已满，忽略本次入队", "if (tail < N) q[++tail] = x;"});
                continue;
            }
            int val = parts[1].toInt();
            tail++;
            m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, tail, 0, 0, {}, QString("tail 指针后移"), QString("q[++tail] = %1;").arg(val)});
            m_frames.push_back({ActionType::SQ_UPDATE_VAL, tail, 0, val, 0, {}, QString("元素 %1 入队").arg(val), ""});
        } else if (parts[0].toUpper() == "POP") {
            if (head <= tail) {
                m_frames.push_back({ActionType::SQ_CLEAR_VAL, head, 0, 0, 0, {}, "队头元素出队", "head++;"});
                head++;
                m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, head, 0, 0, {}, QString("head 指针后移"), ""});
            }
        }
    }
    return m_frames;
}
ArrayBlock* QueueModule::getBlock(int id) { return m_blocks.count(id) ? m_blocks[id] : nullptr; }
void QueueModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) m_mathText->setPlainText(step.mathText);
    if (step.type == ActionType::SQ_POINTER_MOVE) {
        ArrayBlock* anchor = getBlock(step.v);
        if (!anchor) return;
        if (step.u == 0) playPointerArrange({m_ptrHead}, {anchor});
        else playPointerArrange({m_ptrTail}, {anchor});
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
void QueueModule::reset() {
	m_blocks.clear();
	m_frames.clear();
	m_operations.clear();
	m_ptrHead = nullptr;
	m_ptrTail = nullptr;
}
void QueueModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrHead = nullptr;
    m_ptrTail = nullptr;
    m_mathText = nullptr;
}
