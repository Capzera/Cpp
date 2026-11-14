#include "FenwickTreeModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kFtOrange = QColor(250, 196, 110);
static const QColor kFtBlue = QColor(150, 185, 235);
static const QColor kFtGreen = QColor(140, 205, 150);
static const QColor kFtRed = QColor(238, 120, 108);
static const QColor kFtGray = QColor(232, 235, 240);
static const QColor kFtPale = QColor(250, 250, 252);
static const QColor kFtLink = QColor(150, 178, 222);

FenwickTreeModule::FenwickTreeModule() {}

QString FenwickTreeModule::getModuleName() const { return "树状数组 BIT"; }
QString FenwickTreeModule::getDefaultInput() const { return "5\n3 1 4 2 6\n2\n1 2 5\n2 5"; }

QString FenwickTreeModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即原数组 a[1..n]\n"
           "第三行输入一个非负整数 m，表示操作次数\n"
           "接下来 m 行，每行两个整数 op x：\n"
           "  op = 1 表示单点修改 add(x, delta)，使 a[x] += delta\n"
           "  op = 2 表示前缀和查询 sum(x)，求 a[1] + ... + a[x]\n"
           "约定树状数组 t[i] 维护区间 [i-lowbit(i)+1, i] 的和";
}

int FenwickTreeModule::lowbit(int x) { return x & (-x); }

int FenwickTreeModule::levelOf(int x) {
    int level = 0;
    while (x > 0 && (x & 1) == 0) {
        x >>= 1;
        ++level;
    }
    return level;
}

void FenwickTreeModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (!tokens.isEmpty()) {
        m_n = tokens[0].toInt(&ok);
        m_valid = ok && m_n >= 1 && m_n <= 16;
    }
    if (m_valid && tokens.size() < 1 + m_n) m_valid = false;
    if (m_valid) {
        m_a.assign(m_n + 1, 0);
        for (int i = 1; i <= m_n; ++i) {
            m_a[i] = tokens[i].toInt(&ok);
            if (!ok) { m_valid = false; break; }
        }
    }
    int opCount = 0;
    if (m_valid) {
        int idx = 1 + m_n;
        if (idx < tokens.size()) {
            opCount = tokens[idx].toInt(&ok);
            if (!ok || opCount < 0 || opCount > 8) opCount = 0;
        }
    }
    if (m_valid) {
        int idx = 2 + m_n;
        for (int k = 0; k < opCount; ++k) {
            if (idx + 1 >= tokens.size()) { m_valid = false; break; }
            Operation op;
            op.op = tokens[idx].toInt(&ok);
            op.x = tokens[idx + 1].toInt(&ok);
            idx += 2;
            if (!ok || (op.op != 1 && op.op != 2)) { m_valid = false; break; }
            if (op.x < 1 || op.x > m_n) { m_valid = false; break; }
            if (op.op == 1) {
                if (idx >= tokens.size()) { m_valid = false; break; }
                op.delta = tokens[idx].toInt(&ok);
                idx += 1;
                if (!ok) { m_valid = false; break; }
            }
            m_ops.push_back(op);
        }
    }
    if (!m_valid) m_ops.clear();

    m_t.assign(m_n + 1, 0);
    if (m_valid) {
        for (int i = 1; i <= m_n; ++i) {
            int j = i;
            while (j <= m_n) { m_t[j] += m_a[i]; j += lowbit(j); }
        }
    }

    m_mathText = scene->addText(m_valid ? "树状数组分层结构：t[i] 位于第 ctz(i) 层，维护区间 [i-lowbit(i)+1, i]"
                                         : "输入格式：n、n 个元素、m、m 行 op x；限制 n≤16，m≤8");
    m_mathText->setFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 20);
    if (!m_valid) return;

    buildLevels();
    layoutLevels();
    drawGuides();

    m_ptrIndex = new PointerBlock("i", kFtRed);
    m_ptrIndex->setOpacity(0);
    scene->addItem(m_ptrIndex);
}

void FenwickTreeModule::buildLevels() {
    m_levels.clear();
    m_maxLevel = 0;
    for (int i = 1; i <= m_n; ++i) {
        LevelSlot slot;
        slot.index = i;
        slot.level = levelOf(i);
        slot.left = i - lowbit(i) + 1;
        slot.right = i;
        m_levels.push_back(slot);
        m_maxLevel = std::max(m_maxLevel, slot.level);
    }
}

void FenwickTreeModule::layoutLevels() {
    const double rowWidth = m_n * m_cellW;
    m_startX = 430.0 - rowWidth / 2.0;

    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_a[i], QString::number(i));
        block->setCellSize(m_cellW, m_cellH);
        block->setPos(m_startX + (i - 1) * m_cellW, m_arrY);
        block->color = kFtPale;
        m_scene->addItem(block);
        m_aBlocks.push_back(block);
        registerArraySlot(block, "a", i);
    }

    QGraphicsTextItem* baseTitle = m_scene->addText("a[]");
    baseTitle->setFont(QFont("Microsoft YaHei UI", 13, QFont::Bold));
    baseTitle->setDefaultTextColor(QColor(40, 70, 110));
    baseTitle->setPos(m_startX - 52, m_arrY + 12);

    for (LevelSlot& slot : m_levels) {
        const double y = m_arrY - (slot.level + 1) * m_levelStep;
        const double x = m_startX + (slot.left - 1) * m_cellW;
        const double width = (slot.right - slot.left + 1) * m_cellW;

        ArrayBlock* block = new ArrayBlock(1000 + slot.index, m_t[slot.index], QString::number(slot.index));
        block->setCellSize(width, m_cellH);
        block->setPos(x, y);
        block->color = kFtPale;
        m_scene->addItem(block);
        slot.block = block;
        registerArraySlot(block, "t", slot.index);
    }

    for (int lv = 0; lv <= m_maxLevel; ++lv) {
        const double levelY = m_arrY - (lv + 1) * m_levelStep;
        QGraphicsTextItem* tag = m_scene->addText(QString("L%1").arg(lv));
        tag->setFont(QFont("Microsoft YaHei UI", 11, QFont::Bold));
        tag->setDefaultTextColor(QColor(112, 128, 152));
        tag->setPos(m_startX - 76, levelY + 12);

        QGraphicsTextItem* note = m_scene->addText(
            QString("lowbit=%1  t[i] 覆盖 [%2, %3]")
                .arg(1 << lv)
                .arg(1 << lv)
                .arg(std::min(m_n, (1 << (lv + 1)) - 1)));
        note->setFont(QFont("Microsoft YaHei UI", 8));
        note->setDefaultTextColor(QColor(138, 150, 170));
        note->setPos(m_startX + rowWidth + 22, levelY + 14);
    }

    m_accText = m_scene->addText("");
    m_accText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_accText->setDefaultTextColor(QColor(180, 60, 40));
    m_accText->setPos(m_startX, m_arrY + 60);

    m_formulaText = m_scene->addText("t[i] = a[i-lowbit(i)+1] + ... + a[i]");
    m_formulaText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_formulaText->setDefaultTextColor(QColor(80, 100, 130));
    m_formulaText->setPos(m_startX, m_arrY + 88);
}

void FenwickTreeModule::drawGuides() {
    for (QGraphicsItem* item : m_guides) {
        if (!item) continue;
        if (item->scene()) item->scene()->removeItem(item);
        delete item;
    }
    m_guides.clear();
    if (!m_valid) return;

    for (const LevelSlot& slot : m_levels) {
        const double y = m_arrY - (slot.level + 1) * m_levelStep;
        const double x0 = m_startX + (slot.left - 1) * m_cellW;
        const double x1 = m_startX + slot.right * m_cellW;

        QGraphicsPolygonItem* poly = new QGraphicsPolygonItem();
        poly->setPolygon(QPolygonF({QPointF(x0, m_arrY), QPointF(x1, m_arrY), QPointF(x1, y + m_cellH)}));
        poly->setPen(Qt::NoPen);
        poly->setBrush(QColor(kFtLink.red(), kFtLink.green(), kFtLink.blue(), 18));
        poly->setZValue(-3);
        m_scene->addItem(poly);
        m_guides.push_back(poly);

        QGraphicsLineItem* line = new QGraphicsLineItem();
        line->setLine(x0, m_arrY, x1, m_arrY);
        line->setPen(QPen(QColor(kFtLink.red(), kFtLink.green(), kFtLink.blue(), 110), 1.2));
        line->setZValue(-2);
        m_scene->addItem(line);
        m_guides.push_back(line);
    }
}

ArrayBlock* FenwickTreeModule::aBlock(int index) const {
    if (index < 1 || index > static_cast<int>(m_aBlocks.size())) return nullptr;
    return m_aBlocks[index - 1];
}

ArrayBlock* FenwickTreeModule::tBlock(int index) const {
    for (const LevelSlot& slot : m_levels) {
        if (slot.index == index) return slot.block;
    }
    return nullptr;
}

int FenwickTreeModule::levelOfBlock(const ArrayBlock* block) const {
    for (const LevelSlot& slot : m_levels) {
        if (slot.block == block) return slot.level;
    }
    return -1;
}

void FenwickTreeModule::movePointerTo(int index) {
    if (!m_ptrIndex) return;
    ArrayBlock* t = tBlock(index);
    if (!t) return;
    m_ptrIndex->setOpacity(1);
    playPointerMove(m_ptrIndex, t->pos() + QPointF(t->cellWidth() / 2.0, m_cellH + 4.0));
}

void FenwickTreeModule::clearAllStates() {
    for (int i = 1; i <= m_n; ++i) {
        if (ArrayBlock* b = aBlock(i)) { b->color = kFtPale; b->update(); }
    }
    for (const LevelSlot& slot : m_levels) {
        if (slot.block) {
            slot.block->color = kFtPale;
            slot.block->update();
        }
    }
    if (m_accText) m_accText->setPlainText("");
    if (m_ptrIndex) m_ptrIndex->setOpacity(0);
}

std::vector<AnimFrame> FenwickTreeModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    for (int i = 1; i <= m_n; ++i) m_a[i] = liveValue("a", i, m_a[i]);

    m_frames.push_back({ActionType::FT_RESET, 0, 0, 0, 0, {},
                        "构建树状数组：每个 a[i] 沿 lowbit 链向上累加",
                        "t[i] = a[i-lowbit(i)+1] + ... + a[i]"});

    m_t.assign(m_n + 1, 0);
    m_frames.push_back({ActionType::FT_RESET, 0, 0, 0, 0, {},
                        "构建树状数组：自底向上把每个节点的子块合并到父节点",
                        "t[i] = t[i-2^(k-1)] + a[i-2^(k-1)+1] + ... + a[i]　（lowbit(i)=2^k）"});
    for (int lv = 0; lv <= m_maxLevel; ++lv) {
        for (const LevelSlot& slot : m_levels) {
            if (slot.level != lv) continue;
            const int i = slot.index;
            if (lv == 0) {
                m_t[i] = m_a[i];
                m_frames.push_back({ActionType::FT_MERGE, i, 0, m_t[i], 0, {i},
                                    QString("t[%1] = a[%1]").arg(i),
                                    QString("t[%1] = a[%1] = %2　　lowbit(%1) = 1").arg(i).arg(m_t[i])});
                continue;
            }
            const int half = 1 << (lv - 1);
            const int sibling = i - half;
            m_t[i] = m_t[sibling];
            std::vector<int> cells;
            for (int j = i - half + 1; j <= i; ++j) {
                m_t[i] += m_a[j];
                cells.push_back(j);
            }
            m_frames.push_back({ActionType::FT_MERGE, i, sibling, m_t[i], 0, cells,
                                QString("t[%1] = t[%2] + a[%3..%1]").arg(i).arg(sibling).arg(i - half + 1),
                                QString("t[%1] = t[%2] + a[%3..%1] = %4 + %5 = %6　　lowbit(%1) = %7")
                                    .arg(i).arg(sibling).arg(i - half + 1)
                                    .arg(m_t[sibling]).arg(m_t[i] - m_t[sibling]).arg(m_t[i]).arg(lowbit(i))});
        }
    }    m_frames.push_back({ActionType::FT_DONE, 0, 0, 0, 0, {},
                        "树状数组构建完成，接下来演示单点修改与前缀和查询",
                        "build 完成"});

    for (size_t k = 0; k < m_ops.size(); ++k) {
        const Operation& op = m_ops[k];
        if (op.op == 1) {
            m_a[op.x] += op.delta;
            m_frames.push_back({ActionType::FT_RESET, 0, 0, 0, 0, {},
                                QString("操作 %1：单点修改 add(%2, %3)").arg(k + 1).arg(op.x).arg(op.delta),
                                QString("add(%1, %2)：a[%1] += %2").arg(op.x).arg(op.delta)});
            int i = op.x;
            int guard = 0;
            while (i <= m_n && guard++ < 64) {
                const int prev = m_t[i];
                m_t[i] += op.delta;
                const int acc = m_t[i];
                const int next = i + lowbit(i);
                m_frames.push_back({ActionType::FT_JUMP_UP, i, next, acc, prev, {},
                                    QString("t[%1] 累加 %2，沿链跳到 t[%3]").arg(i).arg(op.delta).arg(next),
                                    QString("t[%1] += %2 = %3　　i = i + lowbit(i) = %1 + %4 = %5")
                                        .arg(i).arg(op.delta).arg(acc).arg(lowbit(i)).arg(next)});
                i = next;
            }
        } else {
            m_frames.push_back({ActionType::FT_RESET, 0, 0, 0, 0, {},
                                QString("操作 %1：前缀和查询 sum(%2)").arg(k + 1).arg(op.x),
                                QString("sum(%1)：沿 lowbit 链向左上跳并累加").arg(op.x)});
            int i = op.x;
            int acc = 0;
            int guard = 0;
            while (i > 0 && guard++ < 64) {
                acc += m_t[i];
                const int next = i - lowbit(i);
                m_frames.push_back({ActionType::FT_ACC, i, next, acc, m_t[i], {},
                                    QString("收集 t[%1]，当前累计 %2").arg(i).arg(acc),
                                    QString("sum += t[%1] = %2　　i = i - lowbit(i) = %1 - %3 = %4")
                                        .arg(i).arg(m_t[i]).arg(lowbit(i)).arg(next)});
                i = next;
            }
            m_frames.push_back({ActionType::FT_RESULT, op.x, 0, acc, 0, {},
                                QString("sum(%1) = %2").arg(op.x).arg(acc),
                                QString("sum(%1) = %2").arg(op.x).arg(acc)});
        }
    }
    return m_frames;
}

void FenwickTreeModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);

    if (frame.type == ActionType::FT_RESET) {
        clearAllStates();
    } else if (frame.type == ActionType::FT_MERGE) {
        const int target = frame.u;
        ArrayBlock* t = tBlock(target);
        if (!t) return;
        const int sibling = frame.v;

        std::vector<QPointF> sources;
        std::vector<int> values;
        if (sibling > 0) {
            ArrayBlock* src = tBlock(sibling);
            if (src) {
                sources.push_back(src->pos() + QPointF(src->cellWidth() / 2.0, m_cellH / 2.0));
                values.push_back(src->value);
            }
        }
        for (int idx : frame.group) {
            ArrayBlock* a = aBlock(idx);
            if (!a) continue;
            sources.push_back(a->pos() + QPointF(m_cellW / 2.0, m_cellH / 2.0));
            values.push_back(a->value);
            a->color = kFtOrange;
            a->update();
        }
        t->color = kFtGreen;
        t->showValue = false;
        t->update();
        if (sources.empty()) {
            t->value = frame.val1;
            t->showValue = true;
            t->update();
        } else {
            playMultiValueFlight(sources, values, t, frame.val1, kFtGreen);
        }
        movePointerTo(target);
    } else if (frame.type == ActionType::FT_INSERT) {
        ArrayBlock* a = aBlock(frame.u);
        ArrayBlock* t = tBlock(frame.v);
        const int prev = frame.val2;
        if (a) { a->color = kFtOrange; a->update(); }
        if (t) {
            t->value = frame.val1;
            t->showValue = true;
            t->color = kFtGreen;
            t->update();

            std::vector<QPointF> sources;
            std::vector<int> values;
            ArrayBlock* src = tBlock(frame.u);
            if (src && src != t && prev != 0) {
                sources.push_back(src->pos() + QPointF(src->cellWidth() / 2.0, m_cellH / 2.0));
                values.push_back(prev);
            }
            if (a) {
                sources.push_back(a->pos() + QPointF(m_cellW / 2.0, m_cellH / 2.0));
                values.push_back(a->value);
            }
            if (!sources.empty()) {
                playMultiValueFlight(sources, values, t, frame.val1, kFtGreen);
            }
        }
        movePointerTo(frame.v);
    } else if (frame.type == ActionType::FT_JUMP_UP) {
        ArrayBlock* t = tBlock(frame.u);
        ArrayBlock* a = aBlock(frame.u);
        if (t) {
            t->value = frame.val1;
            t->showValue = true;
            t->color = kFtOrange;
            t->update();
            playScalePulse(t, 1.2, 240);
        }
        if (a && frame.val2 != 0) {
            a->color = kFtOrange;
            a->update();
        }
        movePointerTo(frame.u);
        if (frame.v > frame.u && frame.v <= m_n) {
            ArrayBlock* nextT = tBlock(frame.v);
            if (nextT) {
                nextT->color = kFtBlue;
                nextT->update();
            }
        }
    } else if (frame.type == ActionType::FT_ACC) {
        ArrayBlock* t = tBlock(frame.u);
        if (t) {
            t->color = kFtOrange;
            t->update();
            playScalePulse(t, 1.25, 240);
        }
        if (m_accText) {
            m_accText->setPlainText(QString("累计 sum = %1").arg(frame.val1));
            m_accText->setDefaultTextColor(QColor(180, 60, 40));
        }
        movePointerTo(frame.u);
        if (frame.v > 0 && frame.v <= m_n) {
            ArrayBlock* nextT = tBlock(frame.v);
            if (nextT) {
                nextT->color = kFtBlue;
                nextT->update();
            }
        }
    } else if (frame.type == ActionType::FT_RESULT) {
        if (m_accText) {
            m_accText->setPlainText(QString("sum(%1) = %2").arg(frame.u).arg(frame.val1));
            m_accText->setDefaultTextColor(QColor(40, 140, 70));
        }
        for (const LevelSlot& slot : m_levels) {
            if (slot.block) { slot.block->color = kFtGray; slot.block->update(); }
        }
    } else if (frame.type == ActionType::FT_DONE) {
        clearAllStates();
        for (const LevelSlot& slot : m_levels) {
            if (!slot.block) continue;
            const int shade = 250 - slot.level * 7;
            slot.block->color = QColor(shade, shade + 2, 253);
            slot.block->update();
        }
    }
}

void FenwickTreeModule::onLiveDataChanged(const QString& row, int index, int value) {
    if (row == "a" && index >= 1 && index <= m_n) m_a[index] = value;
}

void FenwickTreeModule::reset() {
    m_frames.clear();
    m_a.clear();
    m_t.clear();
    m_ops.clear();
    m_aBlocks.clear();
    m_levels.clear();
    m_guides.clear();
    m_mathText = nullptr;
    m_accText = nullptr;
    m_formulaText = nullptr;
    m_ptrIndex = nullptr;
    m_n = 0;
    m_maxLevel = 0;
    m_valid = false;
}

void FenwickTreeModule::releaseSceneItems() {
    m_aBlocks.clear();
    m_levels.clear();
    m_guides.clear();
    m_mathText = nullptr;
    m_accText = nullptr;
    m_formulaText = nullptr;
    m_ptrIndex = nullptr;
}
