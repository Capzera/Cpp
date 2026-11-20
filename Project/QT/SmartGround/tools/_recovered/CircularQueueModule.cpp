#include "CircularQueueModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kCqIdle = QColor(252, 252, 254);
static const QColor kCqFill = QColor(255, 205, 145);
static const QColor kCqNew = QColor(180, 220, 255);
static const QColor kCqDone = QColor(150, 214, 170);
static const QColor kCqEmpty = QColor(228, 231, 236);

static const double kCqCellW = 76.0;
static const double kCqCellH = 54.0;
static const double kCqRowY = 200.0;

CircularQueueModule::CircularQueueModule() {}

QString CircularQueueModule::getModuleName() const { return "循环队列"; }
QString CircularQueueModule::getDefaultInput() const {
    return "6\n"
           "PUSH 10\n"
           "PUSH 20\n"
           "PUSH 30\n"
           "POP\n"
           "POP\n"
           "PUSH 40\n"
           "PUSH 50\n"
           "PUSH 60\n"
           "PUSH 70";
}
QString CircularQueueModule::getInputHint() const {
    return "第一行输入队列容量 N（本模块固定展示 6 个槽位）\n"
           "接下来每行一条操作指令：\n"
           "  PUSH x  把 x 从队尾入队，tail = (tail + 1) % N\n"
           "  POP     从队头出队，head = (head + 1) % N\n"
           "当 tail 走到末尾再入队时，会折返回到数组开头";
}

ArrayBlock* CircularQueueModule::cellAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_cells.size())) return nullptr;
    return m_cells[index];
}

void CircularQueueModule::aimQueuePointers(int headIndex, int tailIndex) {
    if (!m_ptrHead || !m_ptrTail) return;
    const QPointF base = PointerGeometry::belowCell(kCqCellW, kCqCellH);
    double headDx = 0.0;
    double tailDx = 0.0;
    if (headIndex == tailIndex) {
        headDx = -15.0;
        tailDx = 15.0;
    }
    ArrayBlock* headCell = cellAt(headIndex);
    ArrayBlock* tailCell = cellAt(tailIndex);
    if (headCell) {
        m_ptrHead->setDirection(PointerBlock::Up);
        m_ptrHead->setOpacity(1);
        retargetPointerTo(m_ptrHead, headCell, base + QPointF(headDx, 0.0));
    }
    if (tailCell) {
        m_ptrTail->setDirection(PointerBlock::Up);
        m_ptrTail->setOpacity(1);
        retargetPointerTo(m_ptrTail, tailCell, base + QPointF(tailDx, 0.0));
    }
}

void CircularQueueModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    bool ok = false;
    int cap = 0;
    if (!lines.isEmpty()) {
        cap = lines[0].trimmed().toInt(&ok);
        m_valid = ok && cap >= 1 && cap <= 6;
    }
    if (m_valid) {
        for (int i = 1; i < lines.size(); ++i) {
            const QString op = lines[i].trimmed();
            if (op.isEmpty()) continue;
            const QStringList parts = op.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.isEmpty()) continue;
            if (parts[0].toUpper() == "PUSH" && parts.size() >= 2) {
                bool tok = false;
                const int value = parts[1].toInt(&tok);
                if (!tok) { m_valid = false; break; }
                m_operations.push_back(1000 + value);
            } else if (parts[0].toUpper() == "POP") {
                m_operations.push_back(-1);
            } else {
                m_valid = false;
                break;
            }
        }
    }
    if (!m_valid) m_operations.clear();

    m_mathText = scene->addText(m_valid ? "循环队列：head = (head + 1) % N，tail = (tail + 1) % N，队尾绕回数组开头"
                                        : "输入格式：N，随后每行 PUSH x 或 POP");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(120, 18);
    if (!m_valid) return;

    const double startX = std::max(120.0, 470.0 - kCapacity * kCqCellW / 2.0);
    for (int i = 0; i < kCapacity; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->setCellSize(kCqCellW, kCqCellH);
        block->showValue = false;
        block->color = kCqIdle;
        block->setPos(startX + i * kCqCellW, kCqRowY);
        scene->addItem(block);
        m_cells.push_back(block);
    }

    m_noteText = scene->addText("");
    m_noteText->setFont(UiFonts::mono(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(120, 400);

    m_ptrHead = new PointerBlock("head", QColor(45, 130, 210));
    m_ptrHead->setOpacity(0);
    m_ptrHead->setDirection(PointerBlock::Up);
    m_ptrHead->setShaftLength(26);
    m_ptrHead->setAnchorOffset(PointerGeometry::belowCell(kCqCellW, kCqCellH));
    m_ptrHead->setLabelAtTail(8.0);
    scene->addItem(m_ptrHead);

    m_ptrTail = new PointerBlock("tail", QColor(210, 95, 50));
    m_ptrTail->setOpacity(0);
    m_ptrTail->setDirection(PointerBlock::Up);
    m_ptrTail->setShaftLength(26);
    m_ptrTail->setAnchorOffset(PointerGeometry::belowCell(kCqCellW, kCqCellH));
    m_ptrTail->setLabelAtTail(8.0);
    scene->addItem(m_ptrTail);
}

std::vector<AnimFrame> CircularQueueModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = kCapacity;
    std::vector<int> q(n, -1);
    int head = 0;
    int tail = 0;
    int count = 0;

    m_frames.push_back({ActionType::CQ_RESET, 0, 0, 0, 0, {},
                        QString("初始化循环队列：head = tail = 0，队列为空"),
                        QString("head = 0, tail = 0, count = 0")});

    for (int op : m_operations) {
        if (op >= 1000) {
            const int value = op - 1000;
            if (count == n) {
                m_frames.push_back({ActionType::CQ_FULL, count, tail, 0, 0, {},
                                    QString("队列已满（%1 个元素），%2 入队失败").arg(n).arg(value),
                                    QString("count == N ⇒ 队满")});
                m_frames.push_back({ActionType::CQ_DONE, count, head, tail, 0, {},
                                    QString("循环队列演示结束，最终有 %1 个元素").arg(count),
                                    QString("tail = (tail + 1) %% N")});
                return m_frames;
            }
            const int oldTail = tail;
            tail = (tail + 1) % n;
            q[tail] = value;
            ++count;
            const bool wrapped = tail < oldTail;
            m_frames.push_back({ActionType::CQ_ENQUEUE, tail, value, count, head, {},
                                QString("入队 %1：tail = (%2 + 1) %% %3 = %4")
                                    .arg(value).arg(oldTail).arg(n).arg(tail),
                                QString("q[%1] = %2").arg(tail).arg(value)});
            if (wrapped) {
                m_frames.push_back({ActionType::CQ_WRAP, tail, oldTail, head, 0, {},
                                    QString("队尾到达数组末尾，绕回下标 %1").arg(tail),
                                    QString("tail = (tail + 1) %% N 实现折返")});
            }
            m_frames.push_back({ActionType::CQ_DONE, count, head, tail, 0, {},
                                QString("入队 %1 后队列有 %2 个元素").arg(value).arg(count),
                                QString("count = %1").arg(count)});
        } else {
            if (count == 0) {
                m_frames.push_back({ActionType::CQ_EMPTY, head, tail, 0, 0, {},
                                    QString("队列为空，无法出队"),
                                    QString("count == 0 ⇒ 队空")});
                m_frames.push_back({ActionType::CQ_DONE, count, head, tail, 0, {},
                                    QString("循环队列演示结束，最终有 %1 个元素").arg(count),
                                    QString("head = (head + 1) %% N")});
                return m_frames;
            }
            const int value = q[head];
            q[head] = -1;
            const int oldHead = head;
            head = (head + 1) % n;
            --count;
            const bool wrapped = head < oldHead;
            m_frames.push_back({ActionType::CQ_DEQUEUE, oldHead, value, count, head, {},
                                QString("出队 %1：head = (%2 + 1) %% %3 = %4")
                                    .arg(value).arg(oldHead).arg(n).arg(head),
                                QString("head = (head + 1) %% N")});
            if (wrapped) {
                m_frames.push_back({ActionType::CQ_WRAP, head, oldHead, tail, 1, {},
                                    QString("队头到达数组末尾，绕回下标 %1").arg(head),
                                    QString("head = (head + 1) %% N 实现折返")});
            }
            m_frames.push_back({ActionType::CQ_DONE, count, head, tail, 0, {},
                                QString("出队后队列有 %1 个元素").arg(count),
                                QString("count = %1").arg(count)});
        }
    }

    m_frames.push_back({ActionType::CQ_DONE, count, head, tail, 0, {},
                        QString("循环队列演示结束，最终有 %1 个元素").arg(count),
                        QString("循环队列靠取模实现下标折返")});
    return m_frames;
}

void CircularQueueModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::CQ_RESET) {
        for (auto* b : m_cells) { b->showValue = false; b->color = kCqIdle; b->update(); }
        if (m_ptrHead) m_ptrHead->setOpacity(0);
        if (m_ptrTail) m_ptrTail->setOpacity(0);
        if (m_noteText) {
            m_noteText->setPlainText("");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
    } else if (frame.type == ActionType::CQ_ENQUEUE) {
        const int slot = frame.u;
        const int value = frame.v;
        ArrayBlock* cell = cellAt(slot);
        if (cell) {
            cell->value = value;
            cell->showValue = true;
            cell->color = kCqNew;
            cell->update();
            if (m_animated) playScalePulse(cell, 1.16, 240);
        }
        aimQueuePointers(frame.val2, frame.val1);
        if (m_noteText) {
            m_noteText->setPlainText(QString("入队 %1 → q[%2]，队列元素个数 %3").arg(value).arg(slot).arg(frame.val1));
        }
    } else if (frame.type == ActionType::CQ_DEQUEUE) {
        const int slot = frame.u;
        const int value = frame.v;
        ArrayBlock* cell = cellAt(slot);
        if (cell) {
            cell->showValue = false;
            cell->color = kCqEmpty;
            cell->update();
            if (m_animated) playScalePulse(cell, 0.72, 260);
        }
        aimQueuePointers(frame.val2, slot);
        if (m_noteText) {
            m_noteText->setPlainText(QString("出队 %1（q[%2]），队列元素个数 %3").arg(value).arg(slot).arg(frame.val1));
        }
    } else if (frame.type == ActionType::CQ_WRAP) {
        const int slot = frame.u;
        const int from = frame.v;
        const bool isHead = frame.val2 == 1;
        ArrayBlock* cell = cellAt(slot);
        if (cell) {
            cell->color = kCqFill;
            cell->update();
            if (m_animated) playScalePulse(cell, 1.24, 300);
        }
        if (isHead) {
            aimQueuePointers(slot, frame.val1);
        } else {
            aimQueuePointers(frame.val1, slot);
        }
        if (m_noteText) {
            m_noteText->setPlainText(QString("下标从 %1 绕回到 %2（取模折返）").arg(from).arg(slot));
            m_noteText->setDefaultTextColor(QColor(180, 110, 30));
        }
    } else if (frame.type == ActionType::CQ_FULL || frame.type == ActionType::CQ_EMPTY) {
        if (m_noteText) {
            m_noteText->setPlainText(frame.explanation);
            m_noteText->setDefaultTextColor(QColor(200, 70, 60));
        }
    } else if (frame.type == ActionType::CQ_DONE) {
        aimQueuePointers(frame.val1, frame.val2);
        if (frame.val1 == 0 && m_noteText) {
            m_noteText->setPlainText(frame.explanation);
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void CircularQueueModule::reset() {
    m_frames.clear();
    m_operations.clear();
    m_cells.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrHead = nullptr;
    m_ptrTail = nullptr;
    m_valid = false;
}

void CircularQueueModule::releaseSceneItems() {
    m_cells.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_ptrHead = nullptr;
    m_ptrTail = nullptr;
}
