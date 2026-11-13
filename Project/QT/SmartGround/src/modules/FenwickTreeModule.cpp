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
           "约定树状数组 t[i] 维护区间 [i - lowbit(i) + 1, i] 的和";
}

int FenwickTreeModule::lowbit(int x) { return x & (-x); }

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
        if (idx >= tokens.size()) {
            m_valid = false;
        } else {
            opCount = tokens[idx].toInt(&ok);
            if (!ok || opCount < 0 || opCount > 8) m_valid = false;
        }
    }
    if (m_valid) {
        int idx = 2 + m_n;
        for (int k = 0; k < opCount; ++k) {
            if (idx + 1 >= tokens.size() + 1) { m_valid = false; break; }
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

    m_mathText = scene->addText(m_valid ? "树状数组：t[i] 维护区间 [i-lowbit(i)+1, i] 的和"
                                         : "输入格式：n、n 个元素、m、m 行 op x；限制 n≤16，m≤8");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    if (!m_valid) return;

    const double cellW = m_cellW;
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_a[i], QString::number(i));
        block->setCellSize(cellW, 40.0);
        block->setPos(m_startX + (i - 1) * cellW, m_arrY);
        block->color = kFtPale;
        scene->addItem(block);
        m_aBlocks.push_back(block);
        registerArraySlot(block, "a", i);

        ArrayBlock* t = new ArrayBlock(1000 + i, m_t[i], QString::number(i));
        t->setCellSize(cellW, 40.0);
        t->setPos(m_startX + (i - 1) * cellW, m_treeY);
        t->color = kFtPale;
        scene->addItem(t);
        m_tBlocks.push_back(t);
        registerArraySlot(t, "t", i);
    }

    QGraphicsTextItem* aTitle = scene->addText("a");
    aTitle->setFont(QFont("Consolas", 14, QFont::Bold));
    aTitle->setDefaultTextColor(QColor(40, 70, 110));
    aTitle->setPos(m_startX - 40, m_arrY - 2);

    QGraphicsTextItem* tTitle = scene->addText("t");
    tTitle->setFont(QFont("Consolas", 14, QFont::Bold));
    tTitle->setDefaultTextColor(QColor(40, 70, 110));
    tTitle->setPos(m_startX - 40, m_treeY - 2);

    QGraphicsTextItem* bandTitle = scene->addText("t[i] 的覆盖区间");
    bandTitle->setFont(QFont("Microsoft YaHei UI", 10, QFont::Bold));
    bandTitle->setDefaultTextColor(QColor(120, 130, 145));
    bandTitle->setPos(m_startX - 40, m_bandY - 22);

    m_accText = scene->addText("");
    m_accText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_accText->setDefaultTextColor(QColor(180, 60, 40));
    m_accText->setPos(m_startX, m_treeY + 66);

    m_ptrIndex = new PointerBlock("i", kFtRed);
    m_ptrIndex->setOpacity(0);
    scene->addItem(m_ptrIndex);

    refreshCoverBands();
}

ArrayBlock* FenwickTreeModule::aBlock(int index) const {
    if (index < 1 || index > static_cast<int>(m_aBlocks.size())) return nullptr;
    return m_aBlocks[index - 1];
}

ArrayBlock* FenwickTreeModule::tBlock(int index) const {
    if (index < 1 || index > static_cast<int>(m_tBlocks.size())) return nullptr;
    return m_tBlocks[index - 1];
}

void FenwickTreeModule::movePointerTo(int index) {
    if (!m_ptrIndex) return;
    ArrayBlock* t = tBlock(index);
    if (!t) return;
    m_ptrIndex->setOpacity(1);
    playPointerMove(m_ptrIndex, t->pos() + QPointF(m_cellW / 2.0, 44.0));
}

void FenwickTreeModule::refreshCoverBands() {
    if (!m_scene) return;
    for (QGraphicsRectItem* band : m_coverBands) {
        if (!band) continue;
        if (band->scene()) band->scene()->removeItem(band);
        delete band;
    }
    m_coverBands.clear();
    for (int i = 1; i <= m_n; ++i) {
        int len = lowbit(i);
        double x = m_startX + (i - len) * m_cellW;
        double w = len * m_cellW;
        QGraphicsRectItem* band = new QGraphicsRectItem(x, m_bandY, w, 26.0);
        band->setBrush(QColor(150, 185, 235, 85));
        band->setPen(QPen(QColor(90, 130, 190), 1.2));
        band->setZValue(-1);
        m_scene->addItem(band);
        m_coverBands.push_back(band);
    }
}

void FenwickTreeModule::clearAllStates() {
    for (int i = 1; i <= m_n; ++i) {
        if (ArrayBlock* b = aBlock(i)) { b->color = kFtPale; b->update(); }
        if (ArrayBlock* b = tBlock(i)) { b->color = kFtPale; b->update(); }
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
    for (int i = 1; i <= m_n; ++i) {
        int j = i;
        while (j <= m_n) {
            m_t[j] += m_a[i];
            int acc = m_t[j];
            m_frames.push_back({ActionType::FT_INSERT, i, j, acc, lowbit(j), {},
                                QString("a[%1] 累加进 t[%2]").arg(i).arg(j),
                                QString("t[%1] += a[%2] = %3　　lowbit(%1) = %4")
                                    .arg(j).arg(i).arg(acc).arg(lowbit(j))});
            j += lowbit(j);
        }
    }
    m_frames.push_back({ActionType::FT_DONE, 0, 0, 0, 0, {},
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
                m_t[i] += op.delta;
                int acc = m_t[i];
                int next = i + lowbit(i);
                m_frames.push_back({ActionType::FT_JUMP_UP, i, next, acc, lowbit(i), {},
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
                int next = i - lowbit(i);
                m_frames.push_back({ActionType::FT_ACC, i, next, acc, lowbit(i), {},
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
    } else if (frame.type == ActionType::FT_INSERT) {
        ArrayBlock* a = aBlock(frame.u);
        ArrayBlock* t = tBlock(frame.v);
        if (a) { a->color = kFtOrange; a->update(); }
        if (t) {
            t->value = frame.val1;
            t->showValue = true;
            t->color = kFtGreen;
            t->update();
            if (a) playValueFlight(a->pos(), t, frame.val1, kFtGreen, false);
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
        for (int i = 1; i <= m_n; ++i) {
            if (ArrayBlock* b = tBlock(i)) { b->color = kFtGray; b->update(); }
        }
    } else if (frame.type == ActionType::FT_DONE) {
        clearAllStates();
        for (int i = 1; i <= m_n; ++i) {
            if (ArrayBlock* b = tBlock(i)) { b->color = kFtGreen; b->update(); }
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
    m_tBlocks.clear();
    m_coverBands.clear();
    m_mathText = nullptr;
    m_accText = nullptr;
    m_ptrIndex = nullptr;
    m_n = 0;
    m_valid = false;
}

void FenwickTreeModule::releaseSceneItems() {
    m_aBlocks.clear();
    m_tBlocks.clear();
    m_coverBands.clear();
    m_mathText = nullptr;
    m_accText = nullptr;
    m_ptrIndex = nullptr;
}
