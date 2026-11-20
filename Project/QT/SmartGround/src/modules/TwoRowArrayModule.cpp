#include "TwoRowArrayModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QFont>

void TwoRowArrayModule::parseTwoRowInput(const QString& inputData, int& n, int& m,
                                         std::vector<int>& first, QString& error) {
    error.clear();
    QStringList lines = inputData.split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 2) { error = "输入至少需要两行"; return; }
    QStringList head = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (head.size() < 2) { error = "第一行需要数组长度与操作次数"; return; }
    bool ok = false;
    n = head[0].toInt(&ok);
    if (!ok || n < 0 || n > 200) { error = "n 需为 0-200 的整数"; n = 0; return; }
    m = head[1].toInt(&ok);
    if (!ok || m < 0 || m > 200) { error = "m 需为 0-200 的整数"; m = 0; return; }
    QStringList arrStrs = lines[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    first.assign(n + 2, 0);
    for (int i = 1; i <= n && i - 1 < arrStrs.size(); ++i) first[i] = arrStrs[i - 1].toInt();
}

void TwoRowArrayModule::buildTwoRowLayout(QGraphicsScene* scene, const std::vector<int>& first,
                                          const std::vector<RowSpec>& rows) {
    const double cellW = 60.0;
    const int startX = 400 - ((m_n + 2) * static_cast<int>(cellW)) / 2;
    for (const RowSpec& row : rows) {
        QGraphicsTextItem* label = scene->addText(row.title);
        label->setFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
        label->setPos(startX - 100, row.y - 2);
        for (int i = 0; i < row.count; ++i) {
            const int dataIndex = row.startIndex + i;
            const int value = (row.offset == 0) ? first[dataIndex] : 0;
            ArrayBlock* block = new ArrayBlock(dataIndex, value, QString::number(dataIndex));
            block->setPos(startX + dataIndex * cellW, row.y);
            if (row.hideTail > 0 && i >= row.count - row.hideTail) block->showValue = false;
            scene->addItem(block);
            m_blocks[row.offset + dataIndex] = block;
            registerArraySlot(block, row.row, dataIndex);
        }
    }
}

ArrayBlock* TwoRowArrayModule::getBlock(int id) const {
    auto it = m_blocks.find(id);
    return it == m_blocks.end() ? nullptr : it->second;
}

void TwoRowArrayModule::renderTwoRowFrame(const AnimFrame& frame) {
    if (m_mathText && !frame.mathText.isEmpty()) setScriptText(m_mathText, frame.mathText);
    if (frame.type == ActionType::PD_HIGHLIGHT) {
        std::vector<ArrayBlock*> cells;
        for (int id : frame.group) {
            ArrayBlock* block = getBlock(id);
            if (block) {
                block->color = QColor(255, 200, 100);
                block->update();
                cells.push_back(block);
            }
        }
        if (frame.group.size() == 1 && !cells.empty()) {
            playNodePop(cells[0], 1.14, 260);
            playCellRipple(cells[0], QColor(255, 190, 90), 0.9, 480);
        } else if (cells.size() > 1) {
            playCellSweep(cells, QColor(255, 200, 100), 55, 300);
        }
    } else if (frame.type == ActionType::PD_UPDATE_VAL) {
        ArrayBlock* target = getBlock(frame.u);
        if (!target) return;
        const int finalVal = frame.val1;
        QPointer<ArrayBlock> safeTarget = target;
        if (m_animated && !frame.group.empty()) {
            target->showValue = false;
            target->update();
            QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
            std::vector<QPointer<FloatingText>> texts;
            for (int srcId : frame.group) {
                ArrayBlock* src = getBlock(srcId);
                if (!src) continue;
                QPointer<FloatingText> ft = new FloatingText(QString::number(src->value));
                ft->setFontSize(15);
                ft->setPos(src->centerPos() - QPointF(15, 15));
                m_scene->addItem(ft.data());
                texts.push_back(ft);
                QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
                anim->setEndValue(target->centerPos() - QPointF(15, 15));
                anim->setDuration(400);
                anim->setEasingCurve(QEasingCurve::InOutCubic);
                group->addAnimation(anim);
            }
            connect(group, &QParallelAnimationGroup::finished, [this, safeTarget, texts, finalVal]() {
                for (auto& t : texts) {
                    if (!t) continue;
                    if (t->scene()) t->scene()->removeItem(t.data());
                    delete t.data();
                }
                if (!safeTarget) return;
                safeTarget->value = finalVal;
                safeTarget->color = QColor(150, 255, 150);
                safeTarget->showValue = true;
                safeTarget->update();
                playScalePulse(safeTarget, 1.3, 300);
                playCellRipple(safeTarget, QColor(120, 220, 140), 1.1, 560);
            });
            group->start(QAbstractAnimation::DeleteWhenStopped);
        } else {
            target->value = finalVal;
            target->color = QColor(150, 255, 150);
            target->showValue = true;
            target->update();
            if (m_animated) playScalePulse(target, 1.3, 300);
        }
    } else if (frame.type == ActionType::PD_RESET) {
        for (int id : frame.group) {
            ArrayBlock* block = getBlock(id);
            if (block) { block->color = Qt::white; block->update(); }
        }
    }
}

void TwoRowArrayModule::reset() {
    m_blocks.clear();
    m_frames.clear();
    m_mathText = nullptr;
    m_n = 0;
    m_m = 0;
}

void TwoRowArrayModule::releaseSceneItems() {
    m_blocks.clear();
    m_mathText = nullptr;
}
