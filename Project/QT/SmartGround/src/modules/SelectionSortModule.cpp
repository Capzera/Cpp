#include "SelectionSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QFont>
SelectionSortModule::SelectionSortModule() : m_mathText(nullptr), m_startX(0) {}
QString SelectionSortModule::getModuleName() const {
    return "选择排序";
}
QString SelectionSortModule::getDefaultInput() const {
    return "8\n3 5 1 9 2 7 4 6";
}
QString SelectionSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组";
}
void SelectionSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_minBlock = nullptr; m_minIdxBlock = nullptr;
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) m_data.push_back(items[i].toInt());
    n = m_data.size();
    m_startX = 400 - (n * 60) / 2;
    m_mathText = createScriptText(m_scene, "");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 30);
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        m_scene->addItem(block);
        block->setPos(m_startX + i * 60, 300);
        m_blocks[i] = block;
        registerArraySlot(block, "a", i);
    }
    m_minBlock = new ArrayBlock(1000, 0, "val");
    m_minBlock->setPos(320, 150);
    m_minBlock->showValue = false;
    m_scene->addItem(m_minBlock);
    m_blocks[1000] = m_minBlock;
    QGraphicsTextItem *minLabel = scene->addText("当前最小值");
    minLabel->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    minLabel->setPos(320 - 45, 80);
    m_minIdxBlock = new ArrayBlock(1001, 0, "k");
    m_minIdxBlock->setPos(480, 150);
    m_minIdxBlock->showValue = false;
    m_scene->addItem(m_minIdxBlock);
    m_blocks[1001] = m_minIdxBlock;
    QGraphicsTextItem *minIdxLabel = scene->addText("当前最小值下标");
    minIdxLabel->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    minIdxLabel->setPos(480 - 65, 80);
    m_ptrI = new PointerBlock("i", QColor(220, 50, 50));
    m_ptrI->setPos(m_startX + 30, 350);
    m_scene->addItem(m_ptrI);
}

std::vector<AnimFrame> SelectionSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;
    for (int i = 0; i < n - 1; ++i) {
        m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, i, 0, 0, {}, QString("第 %1 轮，i 指向位置 %1").arg(i)});
        int minIdx = i;
        m_frames.push_back({ActionType::AR_SCAN_MIN, i, 1000, arr[i], i, {}, QString("默认当前最小值为 arr[%1]=%2").arg(i).arg(arr[i])});
        for (int j = i + 1; j < n; ++j) {
            m_frames.push_back({ActionType::AR_HIGHLIGHT, j, 0, 0, 0, {}, QString("游标 j 扫描 arr[%1]=%2").arg(j).arg(arr[j])});
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
                m_frames.push_back({ActionType::AR_SCAN_MIN, j, 1000, arr[j], j, {}, QString("发现更小值 arr[%1]=%2，同步更新两个变量").arg(j).arg(arr[j])});
            }
            m_frames.push_back({ActionType::AR_RESET, j, 0, 0, 0, {}, ""});
        }
        if (minIdx != i) {
            m_frames.push_back({ActionType::AR_SWAP, i, minIdx, 0, 0, {}, QString("扫描结束，交换 arr[%1] 和 arr[%2]").arg(i).arg(minIdx)});
            std::swap(arr[i], arr[minIdx]);
        }
        m_frames.push_back({ActionType::AR_DONE, i, 0, 0, 0, {}, QString("位置 %1 排序完成").arg(i)});
    }
    m_frames.push_back({ActionType::AR_DONE, n - 1, 0, 0, 0, {}, "排序完成！"});
    return m_frames;
}

void SelectionSortModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) setScriptText(m_mathText, step.mathText);
    if (step.type == ActionType::SQ_POINTER_MOVE) {
        if (m_ptrI) {
            QPointF targetPos(m_startX + step.v * 60 + 30, 350);
            playPointerMove(m_ptrI, targetPos);
        }
    } else if (step.type == ActionType::AR_HIGHLIGHT) {
        if (m_blocks.count(step.u)) {
            m_blocks[step.u]->color = QColor(255, 200, 100);
            m_blocks[step.u]->update();
            playNodePop(m_blocks[step.u], 1.14, 240);
            playCellRipple(m_blocks[step.u], QColor(255, 190, 90), 0.9, 460);
        }
    } else if (step.type == ActionType::AR_SCAN_MIN) {
        ArrayBlock* src = m_blocks.count(step.u) ? m_blocks[step.u] : nullptr;
        ArrayBlock* targetVal = m_blocks.count(1000) ? m_blocks[1000] : nullptr;
        ArrayBlock* targetIdx = m_blocks.count(1001) ? m_blocks[1001] : nullptr;
        if (src && targetVal && targetIdx) {
            int finalVal = step.val1;
            int finalIdx = step.val2;
            QPointer<ArrayBlock> safeTargetVal = targetVal;
            QPointer<ArrayBlock> safeTargetIdx = targetIdx;
            if (m_animated) {
                QPointer<FloatingText> ftVal = new FloatingText(QString::number(finalVal));
                ftVal->setPos(src->pos());
                m_scene->addItem(ftVal.data());
                QPointer<FloatingText> ftIdx = new FloatingText(QString::number(finalIdx));
                ftIdx->setPos(src->pos());
                m_scene->addItem(ftIdx.data());
                QParallelAnimationGroup* par = new QParallelAnimationGroup(this);
                QPropertyAnimation* animVal = new QPropertyAnimation(ftVal.data(), "pos");
                animVal->setEndValue(targetVal->pos());
                animVal->setDuration(400);
                animVal->setEasingCurve(QEasingCurve::InOutQuad);
                par->addAnimation(animVal);
                QPropertyAnimation* animIdx = new QPropertyAnimation(ftIdx.data(), "pos");
                animIdx->setEndValue(targetIdx->pos());
                animIdx->setDuration(400);
                animIdx->setEasingCurve(QEasingCurve::InOutQuad);
                par->addAnimation(animIdx);
                connect(par, &QParallelAnimationGroup::finished, [this, safeTargetVal, safeTargetIdx, ftVal, ftIdx, finalVal, finalIdx]() {
                    if (ftVal) { if (ftVal->scene()) ftVal->scene()->removeItem(ftVal.data()); delete ftVal.data(); }
                    if (ftIdx) { if (ftIdx->scene()) ftIdx->scene()->removeItem(ftIdx.data()); delete ftIdx.data(); }
                    if (!safeTargetVal || !safeTargetIdx) return;
                    safeTargetVal->value = finalVal;
                    safeTargetVal->color = QColor(255, 150, 150);
                    safeTargetVal->showValue = true;
                    safeTargetVal->update();
                    safeTargetIdx->value = finalIdx;
                    safeTargetIdx->color = QColor(255, 150, 150);
                    safeTargetIdx->showValue = true;
                    safeTargetIdx->update();
                    playScalePulse(safeTargetVal, 1.3, 300);
                    playScalePulse(safeTargetIdx, 1.3, 300);
                });
                par->start(QAbstractAnimation::DeleteWhenStopped);
            } else {
                targetVal->value = finalVal;
                targetVal->color = QColor(255, 150, 150);
                targetVal->showValue = true;
                targetVal->update();
                targetIdx->value = finalIdx;
                targetIdx->color = QColor(255, 150, 150);
                targetIdx->showValue = true;
                targetIdx->update();
            }
        }
    } else if (step.type == ActionType::AR_SWAP) {
        if (!m_blocks.count(step.u) || !m_blocks.count(step.v)) return;
        ArrayBlock* b1 = m_blocks[step.u];
        ArrayBlock* b2 = m_blocks[step.v];
        int val1 = b1->value;
        int val2 = b2->value;
        playSwapAnim(b1, b2, val1, val2);
    } else if (step.type == ActionType::AR_RESET) {
        if (m_blocks.count(step.u)) {
            m_blocks[step.u]->color = Qt::white;
            m_blocks[step.u]->update();
        }
    } else if (step.type == ActionType::AR_DONE) {
        if (m_blocks.count(step.u)) {
            m_blocks[step.u]->color = QColor(150, 255, 150);
            m_blocks[step.u]->update();
            playScaleAnim(m_blocks[step.u]);
            playCellRipple(m_blocks[step.u], QColor(120, 220, 140), 1.15, 560);
        }
    }
}

void SelectionSortModule::reset() {
    m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_minBlock = nullptr; m_minIdxBlock = nullptr;
}
void SelectionSortModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrI = nullptr;
    m_minBlock = nullptr;
    m_minIdxBlock = nullptr;
    m_mathText = nullptr;
}
