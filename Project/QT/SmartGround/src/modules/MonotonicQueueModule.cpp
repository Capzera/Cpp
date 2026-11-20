#include "MonotonicQueueModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kMqIdle = QColor(252, 252, 254);
static const QColor kMqWindow = QColor(214, 234, 250);
static const QColor kMqCur = QColor(255, 214, 150);
static const QColor kMqQ = QColor(190, 220, 248);
static const QColor kMqOut = QColor(160, 215, 180);
static const QColor kMqDead = QColor(226, 230, 236);

MonotonicQueueModule::MonotonicQueueModule() {}

QString MonotonicQueueModule::getModuleName() const { return "单调队列"; }
QString MonotonicQueueModule::getDefaultInput() const { return "8\n5 1 9 8 7 6 1 2\n3"; }
QString MonotonicQueueModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n <= 10）\n"
           "第二行输入 n 个整数\n"
           "第三行输入窗口大小 k（k >= 1），求每个长度为 k 的窗口内的最大值\n"
           "单调队列 q[] 存下标并保持对应数值单调递减，队头即当前窗口最大值";
}

ArrayBlock* MonotonicQueueModule::aCell(int index) const {
    if (index < 0 || index >= static_cast<int>(m_aBlocks.size())) return nullptr;
    return m_aBlocks[index];
}

ArrayBlock* MonotonicQueueModule::qCell(int slot) const {
    if (slot < 0 || slot >= static_cast<int>(m_qBlocks.size())) return nullptr;
    return m_qBlocks[slot];
}

void MonotonicQueueModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int n = 0;
    if (!tokens.isEmpty()) {
        n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 10 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                const int value = tokens[i + 1].toInt(&ok);
                if (!ok) { m_valid = false; break; }
                m_data.push_back(value);
            }
        }
    }
    if (m_valid && tokens.size() >= n + 2) {
        const int k = tokens[n + 1].toInt(&ok);
        if (ok && k >= 1 && k <= n) m_k = k;
        else m_valid = false;
    }

    m_mathText = createScriptText(scene, m_valid ? QString("单调队列滑动窗口：窗口大小 k = %1，q[] 存下标且数值单调递减").arg(m_k)
                                        : QString("输入格式：n，随后 n 个整数，再输入窗口大小 k"));
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(120, 18);
    if (!m_valid) return;

    const double cell = 62.0;
    const double startX = std::max(250.0, 470.0 - static_cast<double>(m_data.size()) * cell / 2.0);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i], QString::number(i));
        block->setCellSize(cell, 42);
        block->color = kMqIdle;
        block->setPos(startX + i * cell, 120);
        scene->addItem(block);
        m_aBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }

    QGraphicsTextItem* aLabel = scene->addText("a[] 原数组");
    aLabel->setFont(UiFonts::cjk(11, true));
    aLabel->setDefaultTextColor(QColor(60, 75, 95));
    aLabel->setPos(30, 106);

    m_windowText = createScriptText(scene, "");
    m_windowText->setFont(UiFonts::cjk(12, true));
    m_windowText->setDefaultTextColor(QColor(70, 110, 170));
    m_windowText->setPos(startX, 186);

    QGraphicsTextItem* qLabel = scene->addText("q[] 单调队列（存下标）");
    qLabel->setFont(UiFonts::cjk(11, true));
    qLabel->setDefaultTextColor(QColor(60, 75, 95));
    qLabel->setPos(30, 246);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(1000 + i, 0, QString::number(i));
        block->setCellSize(cell, 42);
        block->showValue = false;
        block->color = kMqIdle;
        block->setPos(startX + i * cell, 296);
        scene->addItem(block);
        m_qBlocks.push_back(block);
    }

    QGraphicsTextItem* mLabel = scene->addText("max[] 窗口最大值");
    mLabel->setFont(UiFonts::cjk(11, true));
    mLabel->setDefaultTextColor(QColor(60, 75, 95));
    mLabel->setPos(30, 390);
    for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
        ArrayBlock* block = new ArrayBlock(2000 + i, 0, QString::number(i));
        block->setCellSize(cell, 42);
        block->showValue = false;
        block->color = kMqIdle;
        block->setPos(startX + i * cell, 440);
        scene->addItem(block);
        m_maxBlocks.push_back(block);
    }

    m_noteText = createScriptText(scene, "");
    m_noteText->setFont(UiFonts::mono(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(120, 500);

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Down);
    m_ptrI->setShaftLength(24);
    m_ptrI->setAnchorOffset(PointerGeometry::aboveCell(cell, 42.0));
    m_ptrI->setLabelAtTail(8.0);
    scene->addItem(m_ptrI);

    m_ptrHead = new PointerBlock("head", QColor(45, 130, 210));
    m_ptrHead->setOpacity(0);
    m_ptrHead->setDirection(PointerBlock::Down);
    m_ptrHead->setShaftLength(24);
    m_ptrHead->setAnchorOffset(PointerGeometry::aboveCell(cell, 42.0));
    m_ptrHead->setLabelAtTail(8.0);
    scene->addItem(m_ptrHead);

    m_ptrTail = new PointerBlock("tail", QColor(210, 95, 50));
    m_ptrTail->setOpacity(0);
    m_ptrTail->setDirection(PointerBlock::Down);
    m_ptrTail->setShaftLength(24);
    m_ptrTail->setAnchorOffset(PointerGeometry::aboveCell(cell, 42.0));
    m_ptrTail->setLabelAtTail(8.0);
    scene->addItem(m_ptrTail);
}

std::vector<AnimFrame> MonotonicQueueModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);

    m_frames.push_back({ActionType::MQ_RESET, 0, 0, 0, 0, {},
                        QString("初始化：窗口大小 k = %1，队列为空").arg(m_k),
                        QString("q[] 保持对应数值单调递减")});

    std::vector<int> q;
    for (int i = 0; i < n; ++i) {
        while (!q.empty() && q.front() <= i - m_k) {
            const int expired = q.front();
            q.erase(q.begin());
            m_frames.push_back({ActionType::MQ_EXPIRE, i, expired, static_cast<int>(q.size()), 0, {},
                                QString("下标 %1 滑出窗口 [i-k+1, i]，从队头淘汰").arg(expired),
                                QString("if (q.front() <= i - k) pop_front")});
        }
        while (!q.empty() && m_data[q.back()] <= m_data[i]) {
            const int kicked = q.back();
            q.pop_back();
            m_frames.push_back({ActionType::MQ_POP_BACK, i, kicked, static_cast<int>(q.size()), 0, {},
                                QString("a[%1] = %2 不比 a[%3] = %4 小，队尾 %1 被踢出")
                                    .arg(kicked).arg(m_data[kicked]).arg(i).arg(m_data[i]),
                                QString("保持 q 对应的数值单调递减")});
        }
        q.push_back(i);
        m_frames.push_back({ActionType::MQ_PUSH, i, static_cast<int>(q.size()) - 1, m_data[i], 0, {},
                            QString("下标 %1 入队，队尾位置 %2").arg(i).arg(q.size() - 1),
                            QString("q.push(%1)").arg(i)});
        if (i >= m_k - 1) {
            m_frames.push_back({ActionType::MQ_MAX, i, q.front(), m_data[q.front()], static_cast<int>(q.size()), {},
                                QString("窗口 [%1, %2] 的最大值是 a[%3] = %4")
                                    .arg(i - m_k + 1).arg(i).arg(q.front()).arg(m_data[q.front()]),
                                QString("队头即窗口最大值")});
        }
    }

    m_frames.push_back({ActionType::MQ_DONE, n, m_k, 0, 0, {},
                        QString("单调队列演示完成，共 %1 个窗口").arg(std::max(0, n - m_k + 1)),
                        QString("单调队列复杂度 O(n)")});
    return m_frames;
}

void MonotonicQueueModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::MQ_RESET) {
        for (auto* b : m_aBlocks) { b->color = kMqIdle; b->update(); }
        for (auto* b : m_qBlocks) { b->showValue = false; b->color = kMqIdle; b->update(); }
        for (auto* b : m_maxBlocks) { b->showValue = false; b->color = kMqIdle; b->update(); }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrHead) m_ptrHead->setOpacity(0);
        if (m_ptrTail) m_ptrTail->setOpacity(0);
        if (m_windowText) setScriptText(m_windowText, "");
        if (m_noteText) {
            setScriptText(m_noteText, "");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
    } else if (frame.type == ActionType::MQ_EXPIRE) {
        const int index = frame.u;
        const int expired = frame.v;
        ArrayBlock* a = aCell(expired);
        if (a) {
            a->color = kMqDead;
            a->update();
            playNodePop(a, 0.92, 260);
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("下标 %1 已滑出窗口，被淘汰").arg(expired));
        }
        if (m_ptrHead) {
            m_ptrHead->setOpacity(1);
            ArrayBlock* qb = qCell(frame.val1);
            if (qb) retargetPointerTo(m_ptrHead, qb, PointerGeometry::aboveCell(62.0, 42.0));
        }
    } else if (frame.type == ActionType::MQ_POP_BACK) {
        const int index = frame.u;
        const int kicked = frame.v;
        ArrayBlock* a = aCell(kicked);
        if (a) {
            a->color = kMqDead;
            a->update();
            playNodePop(a, 0.92, 260);
        }
        ArrayBlock* qb = qCell(frame.val1);
        if (qb) {
            qb->showValue = false;
            qb->color = kMqDead;
            qb->update();
            playNodePop(qb, 0.92, 260);
        }
        if (m_ptrTail) {
            m_ptrTail->setOpacity(1);
            if (qb) retargetPointerTo(m_ptrTail, qb, PointerGeometry::aboveCell(62.0, 42.0));
        }
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            ArrayBlock* cur = aCell(index);
            if (cur) retargetPointerTo(m_ptrI, cur, PointerGeometry::aboveCell(62.0, 42.0));
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("队尾下标 %1 被踢出（a[%1] = %2 ≤ a[%3] = %4）")
                                     .arg(kicked).arg(m_data[kicked]).arg(index).arg(m_data[index]));
        }
    } else if (frame.type == ActionType::MQ_PUSH) {
        const int index = frame.u;
        const int slot = frame.v;
        const int value = frame.val1;
        ArrayBlock* qb = qCell(slot);
        if (qb) {
            qb->value = index;
            qb->showValue = true;
            qb->color = kMqQ;
            qb->update();
            if (m_animated) {
                playScalePulse(qb, 1.16, 240);
                playCellRipple(qb, kMqQ, 0.95, 520);
            }
        }
        ArrayBlock* cur = aCell(index);
        if (cur) {
            cur->color = kMqCur;
            cur->update();
            if (m_ptrI) {
                m_ptrI->setOpacity(1);
                retargetPointerTo(m_ptrI, cur, PointerGeometry::aboveCell(62.0, 42.0));
            }
        }
        if (m_ptrTail && qb) {
            m_ptrTail->setOpacity(1);
            retargetPointerTo(m_ptrTail, qb, PointerGeometry::aboveCell(62.0, 42.0));
        }
        if (m_windowText) {
            const int lo = std::max(0, index - m_k + 1);
            setScriptText(m_windowText, QString("当前窗口 [%1, %2]").arg(lo).arg(index));
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("下标 %1（值 %2）入队到 q[%3]").arg(index).arg(value).arg(slot));
        }
    } else if (frame.type == ActionType::MQ_MAX) {
        const int index = frame.u;
        const int best = frame.v;
        const int value = frame.val1;
        const int outIndex = index - m_k + 1;
        if (outIndex >= 0 && outIndex < static_cast<int>(m_maxBlocks.size())) {
            ArrayBlock* target = m_maxBlocks[outIndex];
            target->value = value;
            target->showValue = true;
            target->color = kMqOut;
            target->update();
            if (m_animated) {
                playScalePulse(target, 1.18, 240);
                playCellLanding(target, kMqOut, 400);
            }
        }
        ArrayBlock* a = aCell(best);
        if (a) {
            a->color = kMqWindow;
            a->update();
            playNodePop(a, 1.18, 280);
            playCellRipple(a, kMqWindow, 1.1, 560);
        }
        if (m_ptrHead) {
            m_ptrHead->setOpacity(1);
            ArrayBlock* qb = qCell(0);
            if (qb) retargetPointerTo(m_ptrHead, qb, PointerGeometry::aboveCell(62.0, 42.0));
        }
        if (m_windowText) {
            setScriptText(m_windowText, QString("当前窗口 [%1, %2]").arg(outIndex).arg(index));
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("窗口 [%1, %2] 最大值 = a[%3] = %4")
                                     .arg(outIndex).arg(index).arg(best).arg(value));
        }
    } else if (frame.type == ActionType::MQ_DONE) {
        if (m_noteText) {
            setScriptText(m_noteText, QString("单调队列演示完成，共 %1 个窗口").arg(frame.u));
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
    }
}

void MonotonicQueueModule::reset() {
    m_frames.clear();
    m_data.clear();
    m_window.clear();
    m_aBlocks.clear();
    m_qBlocks.clear();
    m_maxBlocks.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_windowText = nullptr;
    m_ptrI = nullptr;
    m_ptrHead = nullptr;
    m_ptrTail = nullptr;
    m_k = 3;
    m_valid = false;
}

void MonotonicQueueModule::releaseSceneItems() {
    m_aBlocks.clear();
    m_qBlocks.clear();
    m_maxBlocks.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_windowText = nullptr;
    m_ptrI = nullptr;
    m_ptrHead = nullptr;
    m_ptrTail = nullptr;
}

void MonotonicQueueModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(row);
    Q_UNUSED(index);
    Q_UNUSED(value);
}
