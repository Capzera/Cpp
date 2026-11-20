#include "FastPowModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include <climits>
#include "core/PointerGeometry.h"

static const QColor kFpPale = QColor(252, 252, 254);
static const QColor kFpBit = QColor(255, 220, 160);
static const QColor kFpBase = QColor(158, 196, 240);
static const QColor kFpRes = QColor(150, 214, 170);
static const QColor kFpHit = QColor(246, 176, 120);

static const double kFpLeftX = 70.0;
static const double kFpBitCell = 48.0;
static const double kFpBitGap = 10.0;
static const double kFpPanelW = 250.0;
static const double kFpPanelH = 56.0;

FastPowModule::FastPowModule() {}

bool FastPowModule::fitsExactPower(long long base, long long exp) {
    long long acc = 1;
    for (long long i = 0; i < exp; ++i) {
        if (acc > LLONG_MAX / base) return false;
        acc *= base;
    }
    return true;
}

QString FastPowModule::getModuleName() const { return "快速幂"; }
QString FastPowModule::getDefaultInput() const { return "2 20"; }

QString FastPowModule::getInputHint() const {
    return "输入两个整数：底数 a、指数 b，表示求精确值 a^b（不取模）\n"
           "输入三个整数：底数 a、指数 b、模数 p，表示求 a^b mod p\n"
           "例如 2 20 求 2^20 = 1048576；3 13 1000000007 求 3^13 mod 1000000007\n"
           "指数 b 限制在 1~40 之间；模数 p 限制在 2~2000000000\n"
           "精确值超出 64 位整数范围时会提示改用取模分支";
}

void FastPowModule::layoutPanels() {
    m_baseBlock = new ArrayBlock(1, 0, "a");
    m_baseBlock->setCellSize(kFpPanelW, kFpPanelH);
    m_baseBlock->setPos(kFpLeftX, 108);
    m_baseBlock->color = kFpBase;
    m_scene->addItem(m_baseBlock);

    m_expBlock = new ArrayBlock(3, 0, "b");
    m_expBlock->setCellSize(kFpPanelW, kFpPanelH);
    m_expBlock->setPos(kFpLeftX, 232);
    m_expBlock->color = kFpPale;
    m_scene->addItem(m_expBlock);

    m_resBlock = new ArrayBlock(2, 0, "res");
    m_resBlock->setCellSize(kFpPanelW, kFpPanelH);
    m_resBlock->setPos(kFpLeftX, 356);
    m_resBlock->color = kFpRes;
    m_scene->addItem(m_resBlock);

    registerArraySlot(m_baseBlock, "a", 0);
    registerArraySlot(m_expBlock, "b", 0);
    registerArraySlot(m_resBlock, "res", 0);

    m_stepText = createScriptText(m_scene, "");
    m_stepText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_stepText->setDefaultTextColor(QColor(40, 70, 110));
    m_stepText->setPos(kFpLeftX, 640);
}

void FastPowModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool okA = false;
    bool okB = false;
    bool okP = false;
    if (tokens.size() >= 2) {
        m_base = tokens[0].toLongLong(&okA);
        m_exp = tokens[1].toLongLong(&okB);
        m_hasMod = tokens.size() >= 3;
        if (m_hasMod) m_mod = tokens[2].toLongLong(&okP);
        bool modOk = !m_hasMod || (okP && m_mod >= 2 && m_mod <= 2000000000LL);
        m_valid = okA && okB && modOk && m_base >= 1 && m_base <= 1000000 && m_exp >= 1 && m_exp <= 40;
    if (m_valid && !m_hasMod && !fitsExactPower(m_base, m_exp)) {
        m_valid = false;
    }
    }
    if (!m_valid) {
        m_base = 0;
        m_exp = 0;
        m_mod = 0;
        m_hasMod = false;
    }

    QString title = m_valid
        ? (m_hasMod ? QString("快速幂 qpow：b 的二进制位从右往左取，a ← a×a，b ← b>>1，结果 mod %1").arg(m_mod)
                    : QString("快速幂 qpow（精确值，不取模）：b 的二进制位从右往左取，a ← a×a，b ← b>>1"))
        : (tokens.size() >= 2 && !m_hasMod
               ? QString("精确值超出 64 位范围，请输入模数 p 走取模分支")
               : QString("输入格式：a b（精确值）或 a b p（取模）"));
    m_mathText = createScriptText(scene, title);
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);
    if (!m_valid) return;

    layoutPanels();

    int bitCount = 0;
    long long tmp = m_exp;
    while (tmp > 0) {
        ++bitCount;
        tmp >>= 1;
    }
    m_bitCount = bitCount;

    m_bitTitle = m_scene->addText("指数 b 的二进制位（最右为第 0 位）");
    m_bitTitle->setFont(QFont("Microsoft YaHei", 11, QFont::Bold));
    m_bitTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_bitTitle->setPos(kFpLeftX, 440);

    for (int i = 0; i < bitCount; ++i) {
        ArrayBlock* block = new ArrayBlock(10 + i, 0, QString::number(i));
        block->setCellSize(kFpBitCell, 42);
        block->setPos(bitCellX(i), 510);
        block->color = kFpPale;
        block->showValue = false;
        m_scene->addItem(block);
        m_bitBlocks.push_back(block);
    }

    m_ptrK = new PointerBlock("k", QColor(45, 130, 210));
    m_ptrK->setOpacity(0);
    m_ptrK->setDirection(PointerBlock::Up);
    m_ptrK->setShaftLength(24);
    m_ptrK->setLabelAtTail(8.0);
    m_scene->addItem(m_ptrK);
}

double FastPowModule::bitCellX(int bitIndex) const {
    int count = std::max(1, m_bitCount);
    int column = count - 1 - bitIndex;
    return kFpLeftX + column * (kFpBitCell + kFpBitGap);
}

void FastPowModule::applyBase(long long value) {
    if (!m_baseBlock) return;
    m_baseBlock->customValue = QString::number(value);
    m_baseBlock->showValue = true;
    m_baseBlock->update();
}

void FastPowModule::applyExp(long long value) {
    if (!m_expBlock) return;
    m_expBlock->customValue = QString::number(value);
    m_expBlock->showValue = true;
    m_expBlock->update();
}

void FastPowModule::applyRes(long long value) {
    if (!m_resBlock) return;
    m_resBlock->customValue = QString::number(value);
    m_resBlock->showValue = true;
    m_resBlock->update();
}

std::vector<AnimFrame> FastPowModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    m_base = liveValue("a", 0, static_cast<int>(m_base));
    m_exp = liveValue("b", 0, static_cast<int>(m_exp));
    if (m_hasMod) m_mod = liveValue("p", 0, static_cast<int>(m_mod));

    const QString modSuffix = m_hasMod ? QString(" mod %1").arg(m_mod) : QString();

    AnimFrame reset;
    reset.type = ActionType::FP_RESET;
    reset.u = 0;
    reset.v = 0;
    reset.val1 = 0;
    reset.val2 = 0;
    reset.big1 = m_base;
    reset.big2 = m_exp;
    reset.explanation = QString("初始化：a = %1，b = %2，res = 1").arg(m_base).arg(m_exp);
    reset.mathText = QString("a = %1　b = %2　res = 1%3").arg(m_base).arg(m_exp).arg(modSuffix);
    m_frames.push_back(reset);

    long long res = 1;
    long long a = m_base;
    long long b = m_exp;
    int bitIndex = 0;

    while (b > 0) {
        long long bit = b & 1LL;
        long long oldA = a;
        if (bit == 1) {
            res = res * oldA;
            if (m_hasMod) res %= m_mod;
        }
        long long newA = oldA * oldA;
        if (m_hasMod) newA %= m_mod;
        long long newB = b >> 1;

        AnimFrame frame;
        frame.type = ActionType::FP_BIT;
        frame.u = bitIndex;
        frame.v = static_cast<int>(bit);
        frame.val1 = static_cast<int>(bit);
        frame.val2 = static_cast<int>(b);
        frame.big1 = oldA;
        frame.big2 = newA;
        frame.big3 = newB;
        frame.big4 = res;
        frame.explanation = bit == 1
            ? QString("b % 2 = 1，写入第 %1 位；res = res × a = %2；a = a×a = %3；b = b>>1 = %4")
                .arg(bitIndex).arg(res).arg(newA).arg(newB)
            : QString("b % 2 = 0，写入第 %1 位；a = a×a = %2；b = b>>1 = %3")
                .arg(bitIndex).arg(newA).arg(newB);
        frame.mathText = bit == 1
            ? QString("b % 2 = 1　→　第 %1 位　　res = %2　a = %3　b = %4")
                .arg(bitIndex).arg(res).arg(newA).arg(newB)
            : QString("b % 2 = 0　→　第 %1 位　　a = %2　b = %3")
                .arg(bitIndex).arg(newA).arg(newB);
        m_frames.push_back(frame);

        a = newA;
        b = newB;
        ++bitIndex;
    }

    AnimFrame result;
    result.type = ActionType::FP_RESULT;
    result.u = 0;
    result.v = 1;
    result.val1 = 0;
    result.val2 = 0;
    result.big1 = res;
    result.explanation = QString("%1 ^ %2%3 = %4").arg(m_base).arg(m_exp).arg(modSuffix).arg(res);
    result.mathText = QString("最终结果 = %1").arg(res);
    m_frames.push_back(result);
    return m_frames;
}

void FastPowModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::FP_RESET) {
        applyBase(frame.big1);
        applyExp(frame.big2);
        applyRes(1);
        for (size_t i = 0; i < m_bitBlocks.size(); ++i) {
            m_bitBlocks[i]->color = kFpPale;
            m_bitBlocks[i]->setCustomValue("");
            m_bitBlocks[i]->showValue = false;
            m_bitBlocks[i]->update();
        }
        if (m_stepText) setScriptText(m_stepText, "等待取位");
        if (m_ptrK) m_ptrK->setOpacity(0);
    } else if (frame.type == ActionType::FP_BIT) {
        int index = frame.u;
        ArrayBlock* bitCell = nullptr;
        if (index >= 0 && index < static_cast<int>(m_bitBlocks.size())) {
            ArrayBlock* block = m_bitBlocks[index];
            bitCell = block;
            block->setCustomValue(QString::number(frame.val1));
            block->showValue = true;
            block->color = frame.val1 == 1 ? kFpHit : kFpBit;
            block->update();
            if (m_animated) {
                playScalePulse(block, 1.24, 260);
                playCellRipple(block, frame.val1 == 1 ? kFpHit : kFpBit,
                               frame.val1 == 1 ? 1.25 : 0.9, 520);
            }
        }
        applyBase(frame.big2);
        applyExp(frame.big3);
        applyRes(frame.big4);
        if (m_animated) {
            playScalePulse(m_baseBlock, 1.16, 240);
            playScalePulse(m_expBlock, 1.16, 240);
            if (frame.val1 == 1) {
                playScalePulse(m_resBlock, 1.22, 280);
                playValueArcFlight(bitCell ? bitCell->centerPos() : QPointF(), m_resBlock->centerPos(),
                                   frame.big4, kFpHit, 520);
            }
        }
        if (m_ptrK) {
            m_ptrK->setOpacity(1);
            playPointerMove(m_ptrK, QPointF(bitCellX(index) + kFpBitCell / 2.0, 554));
        }
        if (m_stepText) {
            setScriptText(m_stepText, QString("b % 2 = %1　→　第 %2 位　　a = %3　b = %4")
                .arg(frame.val1).arg(index).arg(frame.big2).arg(frame.big3));
        }
    } else if (frame.type == ActionType::FP_RESULT) {
        applyRes(frame.big1);
        if (m_animated) {
            playScalePulse(m_resBlock, 1.3, 340);
            playCellRipple(m_resBlock, kFpHit, 1.5, 760);
        }
        if (m_stepText) {
            setScriptText(m_stepText, QString("最终结果 = %1").arg(frame.big1));
        }
    }
}

void FastPowModule::onLiveDataChanged(const QString& row, int index, int value) {
    Q_UNUSED(index);
    if (row == "a") m_base = value;
    if (row == "b") m_exp = value;
    if (row == "p" && m_hasMod) m_mod = value;
}

void FastPowModule::reset() {
    m_frames.clear();
    m_bitBlocks.clear();
    m_baseBlock = nullptr;
    m_resBlock = nullptr;
    m_expBlock = nullptr;
    m_mathText = nullptr;
    m_bitTitle = nullptr;
    m_stepText = nullptr;
    m_ptrK = nullptr;
    m_data.clear();
    m_base = 0;
    m_exp = 0;
    m_mod = 0;
    m_hasMod = false;
    m_bitCount = 0;
    m_valid = false;
}

void FastPowModule::releaseSceneItems() {
    m_bitBlocks.clear();
    m_baseBlock = nullptr;
    m_resBlock = nullptr;
    m_expBlock = nullptr;
    m_mathText = nullptr;
    m_bitTitle = nullptr;
    m_stepText = nullptr;
    m_ptrK = nullptr;
}
