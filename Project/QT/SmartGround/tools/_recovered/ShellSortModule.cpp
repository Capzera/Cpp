#include "ShellSortModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <cmath>

static const QColor kShPale = QColor(252, 252, 254);
static const QColor kShGroup = QColor(160, 190, 240);
static const QColor kShDone = QColor(168, 214, 168);
static const QColor kShShift = QColor(248, 196, 138);
static const QColor kShPick = QColor(255, 214, 140);

static const QColor kShPalette[6] = {
    QColor(150, 190, 240), QColor(190, 175, 240), QColor(240, 175, 195),
    QColor(150, 225, 200), QColor(235, 205, 150), QColor(180, 215, 165)
};

ShellSortModule::ShellSortModule() {}

QString ShellSortModule::getModuleName() const { return "希尔排序"; }
QString ShellSortModule::getDefaultInput() const { return "8\n9 8 7 6 5 4 3 2"; }

QString ShellSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度（n ≤ 12）\n"
           "第二行输入 n 个整数，即待排序的数组\n"
           "希尔排序先按 gap 分组做插入排序，gap 从 n/2 逐次折半直到 1";
}

void ShellSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (!tokens.isEmpty()) {
        int n = tokens[0].toInt(&ok);
        m_valid = ok && n >= 1 && n <= 12 && tokens.size() >= n + 1;
        if (m_valid) {
            for (int i = 0; i < n; ++i) {
                int value = tokens[i + 1].toInt(&ok);
                if (!ok) { m_valid = false; break; }
                m_data.push_back(value);
            }
        }
    }

    m_mathText = scene->addText(m_valid ? "希尔排序：gap 分组 → 组内插入排序 → gap 折半" : "输入格式：n，随后输入 n 个整数；限制 n≤12");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(70, 18);
    if (!m_valid) return;

    int n = static_cast<int>(m_data.size());
    m_startX = std::max(110, 430 - n * 30);
    QFont labelFont("Microsoft YaHei", 10, QFont::Bold);
    auto addLabel = [scene, labelFont](const QString& text, QPointF pos) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(labelFont);
        item->setDefaultTextColor(QColor(60, 75, 95));
        item->setPos(pos);
    };
    addLabel("a[]", QPointF(28, 155));
    addLabel("gap", QPointF(28, 385));

    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i], QString::number(i));
        block->setPos(m_startX + i * 60, 175);
        scene->addItem(block);
        m_blocks[i] = block;
        registerArraySlot(block, "a", i);
    }

    m_gapText = scene->addText("");
    m_gapText->setFont(QFont("Consolas", 16, QFont::Bold));
    m_gapText->setDefaultTextColor(QColor(190, 110, 40));
    m_gapText->setPos(80, 400);

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    scene->addItem(m_ptrI);
    m_ptrJ = new PointerBlock("j", QColor(45, 130, 210));
    m_ptrJ->setOpacity(0);
    scene->addItem(m_ptrJ);
}

std::vector<AnimFrame> ShellSortModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    int n = static_cast<int>(m_data.size());
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;

    m_frames.push_back({ActionType::SH_RESET, 0, 0, 0, 0, {}, "初始化序列", "gap 从 n/2 开始逐次折半"});

    for (int gap = n / 2; gap >= 1; gap /= 2) {
        m_frames.push_back({ActionType::SH_GAP, gap, 0, gap, 0, {}, QString("当前增量 gap = %1").arg(gap), QString("gap = %1").arg(gap)});
        for (int start = 0; start < gap && start < n; ++start) {
            std::vector<int> members;
            for (int i = start; i < n; i += gap) members.push_back(i);
            if (members.size() > 1) {
                m_frames.push_back({ActionType::SH_GROUP, start, gap, 0, 0, {members.begin(), members.end()},
                                    QString("下标同余 %1 的元素归为一组").arg(start),
                                    QString("组：%1").arg(members.size())});
            }
        }
        for (int i = gap; i < n; ++i) {
            int key = arr[i];
            m_frames.push_back({ActionType::SH_COMPARE, i, i - gap, arr[i], arr[i - gap], {},
                                QString("取出 a[%1] = %2，与同组前驱 a[%3] 比较").arg(i).arg(key).arg(i - gap),
                                QString("key = a[%1] = %2").arg(i).arg(key)});
            int j = i - gap;
            while (j >= 0 && arr[j] > key) {
                m_frames.push_back({ActionType::SH_SHIFT, j, j + gap, arr[j], key, {},
                                    QString("a[%1]=%2 > key=%3，后移 %1 位").arg(j).arg(arr[j]).arg(key),
                                    QString("a[%1] = a[%2]").arg(j + gap).arg(j)});
                arr[j + gap] = arr[j];
                j -= gap;
                if (j >= 0) {
                    m_frames.push_back({ActionType::SH_COMPARE, j, j, arr[j], key, {},
                                        QString("继续同组回退到 a[%1]").arg(j),
                                        QString("key = %1，a[%2] = %3").arg(key).arg(j).arg(arr[j])});
                }
            }
            m_frames.push_back({ActionType::SH_INSERT, j + gap, 0, key, 0, {},
                                QString("key=%1 落位到 a[%2]").arg(key).arg(j + gap),
                                QString("a[%1] = %2").arg(j + gap).arg(key)});
            arr[j + gap] = key;
        }
        for (int i = 0; i < n; ++i) {
            m_frames.push_back({ActionType::SH_DONE, i, gap, 0, 0, {}, QString("gap = %1 这一轮结束").arg(gap)});
        }
    }
    m_frames.push_back({ActionType::SH_DONE, -1, 1, 0, 0, {}, "希尔排序完成", "gap = 1 的最后一轮保证整体有序"});
    return m_frames;
}

void ShellSortModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    int n = static_cast<int>(m_data.size());
    if (n == 0) return;

    if (frame.type == ActionType::SH_RESET) {
        for (int i = 0; i < n; ++i) {
            ArrayBlock* block = m_blocks[i];
            block->value = m_data[i];
            block->customValue.clear();
            block->showValue = true;
            block->color = kShPale;
            block->update();
        }
        for (LinkLine* line : m_groupLinks) {
            if (!line) continue;
            line->scene()->removeItem(line);
            delete line;
        }
        m_groupLinks.clear();
        if (m_gapText) m_gapText->setPlainText("");
        if (m_ptrI) { m_ptrI->setOpacity(0); m_ptrI->clearAnchor(); }
        if (m_ptrJ) { m_ptrJ->setOpacity(0); m_ptrJ->clearAnchor(); }
    } else if (frame.type == ActionType::SH_GAP) {
        int gap = frame.val1;
        for (int i = 0; i < n; ++i) {
            ArrayBlock* block = m_blocks[i];
            block->color = kShPale;
            block->update();
        }
        if (m_gapText) m_gapText->setPlainText(QString("gap = %1").arg(gap));
        for (int i = 0; i + gap < n; ++i) {
            ArrayBlock* a = m_blocks[i];
            ArrayBlock* b = m_blocks[i + gap];
            if (!a || !b) continue;
            LinkLine* line = new LinkLine(a->pos() + QPointF(30, 40), b->pos() + QPointF(30, 40));
            line->setStyle(kShPalette[i % 6], 2.4, false, true);
            line->setBend(46.0);
            m_scene->addItem(line);
            m_groupLinks.push_back(line);
            a->color = kShPalette[i % 6];
            a->update();
        }
    } else if (frame.type == ActionType::SH_GROUP) {
        int residue = frame.u;
        QColor tint = kShPalette[residue % 6];
        for (int idx : frame.group) {
            if (idx < 0 || idx >= n) continue;
            ArrayBlock* block = m_blocks[idx];
            block->color = tint;
            block->update();
            if (m_animated) playScalePulse(block, 1.14, 220);
        }
    } else if (frame.type == ActionType::SH_COMPARE) {
        if (frame.u >= 0 && frame.u < n && m_blocks[frame.u]) {
            m_blocks[frame.u]->color = kShPick;
            m_blocks[frame.u]->update();
        }
        if (frame.v >= 0 && frame.v < n && m_blocks[frame.v] && frame.v != frame.u) {
            m_blocks[frame.v]->color = kShGroup;
            m_blocks[frame.v]->update();
        }
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            playPointerMove(m_ptrI, QPointF(m_startX + frame.u * 60 + 30, 215));
        }
        if (m_ptrJ && frame.v != frame.u) {
            m_ptrJ->setOpacity(1);
            playPointerMove(m_ptrJ, QPointF(m_startX + frame.v * 60 + 30, 240));
        }
    } else if (frame.type == ActionType::SH_SHIFT) {
        if (frame.u >= 0 && frame.u < n && m_blocks[frame.u]) {
            ArrayBlock* src = m_blocks[frame.u];
            ArrayBlock* dst = m_blocks[frame.v];
            if (dst) playShiftAnim(src, dst, frame.val1, kShShift, true);
        }
    } else if (frame.type == ActionType::SH_INSERT) {
        int index = frame.u;
        if (index >= 0 && index < n && m_blocks[index]) {
            ArrayBlock* block = m_blocks[index];
            block->value = frame.val1;
            block->customValue.clear();
            block->showValue = true;
            block->color = kShDone;
            block->update();
            if (m_animated) playScaleAnim(block);
        }
        if (m_ptrJ) m_ptrJ->setOpacity(0);
    } else if (frame.type == ActionType::SH_DONE) {
        if (frame.u >= 0 && frame.u < n && m_blocks[frame.u]) {
            m_blocks[frame.u]->color = kShDone;
            m_blocks[frame.u]->update();
        }
        if (frame.u < 0) {
            for (int i = 0; i < n; ++i) {
                if (!m_blocks[i]) continue;
                m_blocks[i]->color = kShDone;
                m_blocks[i]->update();
            }
            if (m_ptrI) m_ptrI->setOpacity(0);
        }
    }
}

void ShellSortModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a" && index >= 0 && index < static_cast<int>(m_data.size())) m_data[index] = value;
}

void ShellSortModule::reset() {
    m_frames.clear();
    m_blocks.clear();
    m_groupLinks.clear();
    m_data.clear();
    m_mathText = nullptr;
    m_gapText = nullptr;
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_startX = 0;
    m_valid = false;
}

void ShellSortModule::releaseSceneItems() {
    for (LinkLine* line : m_groupLinks) {
        if (!line) continue;
        if (line->scene()) line->scene()->removeItem(line);
        delete line;
    }
    m_groupLinks.clear();
    m_blocks.clear();
    m_mathText = nullptr;
    m_gapText = nullptr;
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
}
