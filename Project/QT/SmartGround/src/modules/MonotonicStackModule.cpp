#include "MonotonicStackModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>

MonotonicStackModule::MonotonicStackModule() {}

QString MonotonicStackModule::getModuleName() const {
    return "单调栈";
}

QString MonotonicStackModule::getDefaultInput() const {
    return "7\n3 5 1 9 2 8 4";
}
QString MonotonicStackModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待求「下一个更大元素」的数组\n"
           "不存在更大元素的位置会被标记出来";
}

void MonotonicStackModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_data.clear();
    m_arrayBlocks.clear();
    m_frames.clear();
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) {
        m_data.push_back(items[i].toInt());
    }
    n = m_data.size();
    m_startX = 400 - (n * 60) / 2;
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        m_scene->addItem(block);
        block->setPos(m_startX + i * 60, 150);
        m_arrayBlocks[i] = block;
        registerArraySlot(block, "a", i);
    }
    m_mathText = m_scene->addText("单调栈：存储未找到下一个更大元素的索引");
    m_mathText->setPos(100, 50);
    m_mathText->setFont(QFont("Consolas", 11, QFont::Bold));
}

std::vector<AnimFrame> MonotonicStackModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> next(n, -1);
    static const int MAXN = 1005;
    static int stk[MAXN];
    int top = 0;
    for (int i = 0; i < n; ++i) {
        m_frames.push_back({ActionType::SQ_POINTER_MOVE, i, 0, m_data[i], 0, {},
                            QString("处理元素 arr[%1] = %2").arg(i).arg(m_data[i])});
        while (top > 0 && m_data[stk[top - 1]] < m_data[i]) {
            int idx = stk[--top];
            next[idx] = i;
            m_frames.push_back({ActionType::SQ_UPDATE_VAL, idx, i, 0, 0, {},
                                QString("找到 arr[%1] 的下一个更大元素：arr[%2] = %3").arg(idx).arg(i).arg(m_data[i])});
        }
        if (top < MAXN - 1) stk[top++] = i;
        m_frames.push_back({ActionType::SQ_UPDATE_VAL, i, 0, 0, 0, {},
                            QString("将索引 %1 入栈").arg(i)});
    }
    while (top > 0) {
        int idx = stk[--top];
        m_frames.push_back({ActionType::SQ_CLEAR_VAL, idx, 0, 0, 0, {},
                            QString("索引 %1 没有下一个更大元素").arg(idx)});
    }
    return m_frames;
}

void MonotonicStackModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.explanation);
    if (frame.type == ActionType::SQ_POINTER_MOVE) {
        if (m_arrayBlocks.count(frame.u)) {
            m_arrayBlocks[frame.u]->color = QColor(255, 200, 100);
            m_arrayBlocks[frame.u]->setZValue(2);
            m_arrayBlocks[frame.u]->update();
            if (m_animated) {
                playScalePulse(m_arrayBlocks[frame.u], 1.15, 200);
            }
        }
    } else if (frame.type == ActionType::SQ_UPDATE_VAL) {
        if (frame.u >= 0 && m_arrayBlocks.count(frame.u)) {
            m_arrayBlocks[frame.u]->color = QColor(150, 255, 150);
            m_arrayBlocks[frame.u]->update();
        }
        if (frame.v >= 0 && m_arrayBlocks.count(frame.v)) {
            m_arrayBlocks[frame.v]->color = QColor(100, 200, 255);
            m_arrayBlocks[frame.v]->update();
        }
    } else if (frame.type == ActionType::SQ_CLEAR_VAL) {
        if (m_arrayBlocks.count(frame.u)) {
            m_arrayBlocks[frame.u]->color = QColor(200, 200, 200);
            m_arrayBlocks[frame.u]->update();
        }
    }
}

void MonotonicStackModule::reset() {
    m_data.clear();
    m_arrayBlocks.clear();
    m_frames.clear();
}
void MonotonicStackModule::releaseSceneItems() {
    m_arrayBlocks.clear();
    m_mathText = nullptr;
}
