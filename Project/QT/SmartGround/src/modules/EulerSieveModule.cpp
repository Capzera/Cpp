#include "EulerSieveModule.h"
#include <QFont>
#include <QRegularExpression>
#include <QVariantAnimation>
#include <QPointer>
#include <cmath>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kEsPale = QColor(252, 252, 254);
static const QColor kEsPrime = QColor(150, 214, 170);
static const QColor kEsCross = QColor(206, 210, 218);
static const QColor kEsScan = QColor(255, 224, 165);
static const QColor kEsOuter = QColor(214, 226, 248);
static const QColor kEsInner = QColor(255, 205, 125);
static const QColor kEsBreak = QColor(240, 140, 130);

EulerSieveModule::EulerSieveModule() {}

QString EulerSieveModule::getModuleName() const { return "欧拉筛"; }
QString EulerSieveModule::getDefaultInput() const { return "120"; }

QString EulerSieveModule::getInputHint() const {
    return "只需输入一个正整数 n，表示要筛出 1~n 范围内的所有素数\n"
           "例如输入 120，右侧会显示 12 行 × 10 列的矩阵\n"
           "欧拉筛（线性筛）：外层枚举 i，内层用质数库里的 p 筛掉 i*p\n"
           "关键性质：每个合数只会被它的最小质因子筛掉一次\n"
           "当 i % p == 0 时立刻 break，保证不重复筛";
}

void EulerSieveModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    if (!tokens.isEmpty()) {
        m_n = tokens[0].toInt(&ok);
        m_valid = ok && m_n >= 2 && m_n <= 400;
    }
    if (!m_valid) m_n = 0;

    m_mathText = createScriptText(scene, m_valid
        ? QString("欧拉筛（线性筛）：i 递增，质数库里的 p 依次筛掉 i*p")
        : QString("输入格式：单个整数 n（2 ≤ n ≤ 400）"));
    m_mathText->setFont(UiFonts::mono(14, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);
    if (!m_valid) return;

    m_rows = (m_n + kColumns - 1) / kColumns;
    const double cell = 46.0;
    const double rowStep = 92.0;
    const double originX = 70.0;
    const double originY = 110.0;

    for (int value = 1; value <= m_n; ++value) {
        int row = (value - 1) / kColumns;
        int col = (value - 1) % kColumns;
        ArrayBlock* cellBlock = new ArrayBlock(value, value, "");
        cellBlock->setCellSize(cell, cell);
        cellBlock->setShowLabel(false);
        cellBlock->setPos(originX + col * cell, originY + row * rowStep);
        cellBlock->color = kEsPale;
        scene->addItem(cellBlock);
        m_cells[value] = cellBlock;
    }

    const double gridBottom = originY + (m_rows - 1) * rowStep + cell;

    m_scanText = createScriptText(scene, "");
    m_scanText->setFont(UiFonts::mono(13, true));
    m_scanText->setDefaultTextColor(QColor(190, 110, 40));
    m_scanText->setPos(originX, gridBottom + 24);

    m_ruleText = createScriptText(scene, "");
    m_ruleText->setFont(UiFonts::cjk(13, true));
    m_ruleText->setDefaultTextColor(QColor(60, 80, 110));
    m_ruleText->setPos(originX, gridBottom + 56);

    const double primeX = originX + kColumns * cell + 80.0;
    m_primeTitle = scene->addText("质数库 prime[]");
    m_primeTitle->setFont(UiFonts::cjk(11, true));
    m_primeTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_primeTitle->setPos(primeX, 58.0);

    const double primeCell = 52.0;
    const double primeRowStep = 112.0;
    int primeCount = 0;
    for (int value = 2; value <= m_n; ++value) {
        bool isPrime = true;
        for (int d = 2; d * d <= value; ++d) {
            if (value % d == 0) { isPrime = false; break; }
        }
        if (!isPrime) continue;
        ArrayBlock* block = new ArrayBlock(value, value, QString::number(primeCount));
        block->setCellSize(primeCell, 44);
        block->setPos(primeX + (primeCount % 5) * (primeCell + 10), 130.0 + (primeCount / 5) * primeRowStep);
        block->color = kEsPale;
        block->setOpacity(0);
        scene->addItem(block);
        m_primeBlocks.push_back(block);
        ++primeCount;
    }

    m_ptrI = new PointerBlock("i", QColor(210, 80, 55));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Down);
    m_ptrI->setAnchorOffset(PointerGeometry::aboveCell(cell, cell));
    m_ptrI->setLabelAtTail(10.0);
    scene->addItem(m_ptrI);

    m_ptrP = new PointerBlock("p", QColor(124, 107, 214));
    m_ptrP->setOpacity(0);
    m_ptrP->setDirection(PointerBlock::Up);
    m_ptrP->setAnchorOffset(PointerGeometry::belowCell(primeCell, 44));
    m_ptrP->setLabelAtTail(10.0);
    scene->addItem(m_ptrP);
}

void EulerSieveModule::resetGrid() {
    for (auto& kv : m_cells) {
        kv.second->color = kEsPale;
        kv.second->showValue = true;
        kv.second->customValue.clear();
        kv.second->setStruck(false);
        kv.second->setScale(1.0);
        kv.second->update();
    }
    for (ArrayBlock* block : m_primeBlocks) {
        if (!block) continue;
        block->setOpacity(0);
        block->color = kEsPale;
        block->update();
    }
}

std::vector<AnimFrame> EulerSieveModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    std::vector<bool> isComposite(m_n + 1, false);
    m_primeList.clear();

    m_frames.push_back({ActionType::ES_RESET, 0, 0, m_n, 0, {},
                        QString("初始化：1 不是素数，从 i = 2 开始枚举"),
                        QString("1 先标记掉，prime[] 质数库初始为空")});

    const int fastFrom = m_n / 2;

    for (int i = 2; i <= fastFrom; ++i) {
        m_frames.push_back({ActionType::ES_OUTER, i, 0, 0, 0, {},
                            QString("外层：i = %1，用质数库里的 p 依次筛掉 i*p").arg(i),
                            QString("i = %1").arg(i)});
        if (!isComposite[i]) {
            m_primeList.push_back(i);
            m_frames.push_back({ActionType::ES_PRIME_ADD, i, 0, static_cast<int>(m_primeList.size()) - 1, 0, {},
                                QString("%1 没被任何 p 筛掉，是素数，加入质数库 prime[]").arg(i),
                                QString("i = %1 是素数，加入 prime[]").arg(i)});
        }
        bool brokeEarly = false;
        for (size_t pi = 0; pi < m_primeList.size(); ++pi) {
            const int p = m_primeList[pi];
            const long long product = static_cast<long long>(i) * p;
            if (product > m_n) break;
            isComposite[static_cast<int>(product)] = true;
            m_frames.push_back({ActionType::ES_INNER, i, p, static_cast<int>(product), static_cast<int>(pi), {},
                                QString("取质数 p = %1，筛掉 i*p = %2*%1 = %3").arg(p).arg(i).arg(product),
                                QString("i * p = %1 * %2 = %3").arg(i).arg(p).arg(product)});
            m_frames.push_back({ActionType::ES_CROSS, static_cast<int>(product), p, i, static_cast<int>(pi), {},
                                QString("%1 由最小质因子 %2 筛掉，只被筛这一次").arg(product).arg(p),
                                QString("st[%1] = 1　最小质因子 = %2").arg(product).arg(p)});
            if (i % p == 0) {
                brokeEarly = true;
                m_frames.push_back({ActionType::ES_BREAK, i, p, static_cast<int>(product), 0, {},
                                    QString("i % p == %1 % %2 == 0，立刻 break，交给 p 自己的那一轮处理").arg(i).arg(p),
                                    QString("i %% p == 0 → break，防止重复筛")});
                break;
            }
        }
        if (!brokeEarly) {
            m_frames.push_back({ActionType::ES_BREAK, i, 0, 0, 1, {},
                                QString("质数库已全部试过，或 i*p 已越界，本轮结束").arg(i),
                                QString("内层循环结束")});
        }
    }

    if (fastFrom >= 2 && fastFrom < m_n) {
        m_frames.push_back({ActionType::ES_FAST_START, fastFrom, 0, m_n, 0, {},
                            QString("i 已经到 %1：此时 i*2 > n，内层循环不可能再筛到任何数").arg(fastFrom),
                            QString("i > n/2 ⇒ 内层无操作，改为批量收录素数")});

        std::vector<int> batch;
        const int batchSize = 8;
        for (int i = fastFrom + 1; i <= m_n; ++i) {
            if (isComposite[i]) continue;
            m_primeList.push_back(i);
            batch.push_back(i);
            if (static_cast<int>(batch.size()) >= batchSize) {
                m_frames.push_back({ActionType::ES_FAST_BATCH, batch.front(), 0,
                                    static_cast<int>(batch.size()), static_cast<int>(m_primeList.size()) - 1,
                                    {batch.begin(), batch.end()},
                                    QString("快速筛：批量收录 %1 个素数（%2 ... %3）")
                                        .arg(batch.size()).arg(batch.front()).arg(batch.back()),
                                    QString("prime[] += {%1...}").arg(batch.front())});
                batch.clear();
            }
        }
        if (!batch.empty()) {
            m_frames.push_back({ActionType::ES_FAST_BATCH, batch.front(), 0,
                                static_cast<int>(batch.size()), static_cast<int>(m_primeList.size()) - 1,
                                {batch.begin(), batch.end()},
                                QString("快速筛：批量收录 %1 个素数（%2 ... %3）")
                                    .arg(batch.size()).arg(batch.front()).arg(batch.back()),
                                QString("prime[] += {%1...}").arg(batch.front())});
        }
        m_frames.push_back({ActionType::ES_FAST_DONE, fastFrom, 0, m_n, 0, {},
                            QString("快速筛完成，n/2 之后不再需要逐个演示"),
                            QString("后半段合数早已在前半段被筛掉")});
    }

    m_frames.push_back({ActionType::ES_DONE, 0, 0, static_cast<int>(m_primeList.size()), 0,
                        {m_primeList.begin(), m_primeList.end()},
                        QString("欧拉筛完成，共 %1 个素数").arg(m_primeList.size()),
                        QString("每个合数只被最小质因子筛掉一次，总复杂度 O(n)")});
    return m_frames;
}

void EulerSieveModule::applyCross(ArrayBlock* block) {
    if (!block) return;
    block->showValue = true;
    block->customValue.clear();
    block->color = kEsCross;
    block->setStruck(true);
    block->update();
}

void EulerSieveModule::stopCrossFlow() {
    if (m_crossAnim) {
        m_crossAnim->stop();
        m_crossAnim = nullptr;
    }
    m_pendingCross.clear();
    m_crossDone = 0;
}

void EulerSieveModule::flushPendingCross() {
    if (m_pendingCross.empty()) return;
    for (size_t i = static_cast<size_t>(m_crossDone); i < m_pendingCross.size(); ++i) {
        applyCross(m_pendingCross[i]);
    }
    stopCrossFlow();
}

void EulerSieveModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type != ActionType::ES_CROSS) {
        flushPendingCross();
    }

    if (frame.type == ActionType::ES_RESET) {
        stopCrossFlow();
        resetGrid();
        if (m_scanText) setScriptText(m_scanText, "");
        if (m_ruleText) {
            setScriptText(m_ruleText, "线性筛的关键：每个合数只会被它的最小质因子筛掉一次");
            m_ruleText->setDefaultTextColor(QColor(60, 80, 110));
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrP) m_ptrP->setOpacity(0);
    } else if (frame.type == ActionType::ES_OUTER) {
        const int i = frame.u;
        auto it = m_cells.find(i);
        if (it != m_cells.end()) {
            it->second->color = kEsOuter;
            it->second->update();
            if (m_animated) {
                playScalePulse(it->second, 1.22, 280);
                playCellRipple(it->second, kEsOuter, 1.05, 560);
            }
        }
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            if (it != m_cells.end()) {
                retargetPointerTo(m_ptrI, it->second, PointerGeometry::aboveCell(46, 46));
            }
        }
        if (m_scanText) setScriptText(m_scanText, QString("外层 i = %1").arg(i));
    } else if (frame.type == ActionType::ES_INNER) {
        const int i = frame.u;
        const int p = frame.v;
        const int product = frame.val1;
        const int primeIndex = frame.val2;
        if (m_ptrP) {
            m_ptrP->setOpacity(1);
            if (primeIndex >= 0 && primeIndex < static_cast<int>(m_primeBlocks.size())) {
                retargetPointerTo(m_ptrP, m_primeBlocks[primeIndex], PointerGeometry::belowCell(52, 44));
            }
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("i = %1，取质数 p = %2，目标是 i*p = %3").arg(i).arg(p).arg(product));
        }
        auto prod = m_cells.find(product);
        if (prod != m_cells.end()) {
            prod->second->color = kEsInner;
            prod->second->update();
            if (m_animated) playCellRipple(prod->second, kEsInner, 1.0, 520);
        }
        if (primeIndex >= 0 && primeIndex < static_cast<int>(m_primeBlocks.size())) {
            ArrayBlock* pb = m_primeBlocks[primeIndex];
            pb->color = kEsInner;
            pb->update();
            if (m_animated) playScalePulse(pb, 1.20, 240);
        }
    } else if (frame.type == ActionType::ES_CROSS) {
        const int product = frame.u;
        const int p = frame.v;
        const int i = frame.val1;
        auto it = m_cells.find(product);
        if (it != m_cells.end()) {
            applyCross(it->second);
            if (m_animated) playCellRipple(it->second, QColor(214, 118, 108), 0.62, 420);
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("%1 = %2 × %3，被最小质因子 %3 筛掉").arg(product).arg(i).arg(p));
        }
        if (m_ruleText) {
            setScriptText(m_ruleText, QString("每个合数只被它的最小质因子筛掉一次：%1 的最小质因子是 %2")
                               .arg(product).arg(p));
            m_ruleText->setDefaultTextColor(QColor(60, 80, 110));
        }
    } else if (frame.type == ActionType::ES_BREAK) {
        if (frame.val2 == 1) {
            if (m_scanText) setScriptText(m_scanText, QString("内层结束"));
            if (m_ptrP) m_ptrP->setOpacity(0);
        } else {
            const int i = frame.u;
            const int p = frame.v;
            if (m_scanText) {
                setScriptText(m_scanText, QString("i %% p == 0（%1 %% %2 == 0）→ break").arg(i).arg(p));
            }
            if (m_ruleText) {
                setScriptText(m_ruleText, QString("因为 i = %1 含质因子 %2，后面的 p 不必再试：%1*p 的最小质因子仍是 %2")
                                   .arg(i).arg(p));
                m_ruleText->setDefaultTextColor(kEsBreak.darker(115));
            }
            auto it = m_cells.find(i);
            if (it != m_cells.end()) {
                it->second->color = kEsBreak;
                it->second->update();
                if (m_animated) playCellRipple(it->second, kEsBreak, 0.9, 520);
            }
            if (m_ptrP) m_ptrP->setOpacity(0);
        }
    } else if (frame.type == ActionType::ES_PRIME_ADD) {
        const int value = frame.u;
        const int primeIndex = frame.val1;
        auto it = m_cells.find(value);
        if (it != m_cells.end()) {
            it->second->color = kEsPrime;
            it->second->setStruck(false);
            it->second->setScale(1.0);
            it->second->update();
            if (m_animated) {
                playScalePulse(it->second, 1.3, 300);
                playCellRipple(it->second, kEsPrime, 1.2, 620);
            }
        }
        if (primeIndex >= 0 && primeIndex < static_cast<int>(m_primeBlocks.size())) {
            ArrayBlock* block = m_primeBlocks[primeIndex];
            block->value = value;
            block->customValue.clear();
            block->showValue = true;
            block->color = kEsPrime;
            block->update();
            if (m_animated) {
                QVariantAnimation* anim = new QVariantAnimation(block);
                anim->setDuration(300);
                anim->setStartValue(0.0);
                anim->setEndValue(1.0);
                anim->setEasingCurve(QEasingCurve::OutCubic);
                QObject::connect(anim, &QVariantAnimation::valueChanged, [block](const QVariant& value) {
                    if (block) block->setOpacity(value.toReal());
                });
                anim->start(QAbstractAnimation::DeleteWhenStopped);
                playCellRipple(block, kEsPrime, 1.15, 560);
            } else {
                block->setOpacity(1.0);
            }
        }
        if (m_scanText) setScriptText(m_scanText, QString("%1 是素数，加入质数库").arg(value));
    } else if (frame.type == ActionType::ES_FAST_START) {
        if (m_scanText) {
            setScriptText(m_scanText, QString("i ≥ %1：此时 i*2 > n，内层循环筛不到任何数，切换到快速模式")
                               .arg(frame.u));
        }
        if (m_ruleText) {
            setScriptText(m_ruleText, QString("i > n/2 时，最小的质数 2 已经让 i*p 越界，后面的合数全部筛过了"));
            m_ruleText->setDefaultTextColor(QColor(60, 80, 110));
        }
        if (m_ptrP) m_ptrP->setOpacity(0);
    } else if (frame.type == ActionType::ES_FAST_BATCH) {
        const std::vector<int>& batch = frame.group;
        for (int value : batch) {
            auto it = m_cells.find(value);
            if (it == m_cells.end()) continue;
            it->second->color = kEsPrime;
            it->second->setStruck(false);
            it->second->setScale(1.0);
            it->second->update();
            if (m_animated) {
                playScalePulse(it->second, 1.24, 240);
                playCellRipple(it->second, kEsPrime, 1.1, 520);
            }
        }
        int shown = 0;
        for (int value : batch) {
            const int primeIndex = static_cast<int>(m_primeList.size()) - static_cast<int>(batch.size()) + shown;
            if (primeIndex < 0 || primeIndex >= static_cast<int>(m_primeBlocks.size())) { ++shown; continue; }
            ArrayBlock* block = m_primeBlocks[primeIndex];
            block->value = value;
            block->customValue.clear();
            block->showValue = true;
            block->color = kEsPrime;
            block->update();
            if (m_animated) {
                QVariantAnimation* anim = new QVariantAnimation(block);
                anim->setDuration(280);
                anim->setStartValue(block->opacity());
                anim->setEndValue(1.0);
                anim->setEasingCurve(QEasingCurve::OutCubic);
                QObject::connect(anim, &QVariantAnimation::valueChanged, [block](const QVariant& value) {
                    if (block) block->setOpacity(value.toReal());
                });
                anim->start(QAbstractAnimation::DeleteWhenStopped);
                playCellRipple(block, kEsPrime, 1.1, 500);
            } else {
                block->setOpacity(1.0);
            }
            ++shown;
        }
        if (m_scanText) {
            setScriptText(m_scanText, QString("快速筛：收录 %1 个素数（%2 ... %3）")
                               .arg(frame.val1).arg(batch.front()).arg(batch.back()));
        }
    } else if (frame.type == ActionType::ES_FAST_DONE) {
        if (m_scanText) {
            setScriptText(m_scanText, QString("快速筛完成，%1 ~ %2 一共 %3 个数已处理")
                               .arg(frame.u).arg(frame.val1).arg(frame.val1 - frame.u + 1));
        }
        if (m_ruleText) {
            setScriptText(m_ruleText, QString("后半段的合数全部在前半段被筛掉了，这一段只需收录素数"));
            m_ruleText->setDefaultTextColor(QColor(60, 80, 110));
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrP) m_ptrP->setOpacity(0);
    } else if (frame.type == ActionType::ES_DONE) {
        for (ArrayBlock* block : m_primeBlocks) {
            if (!block) continue;
            block->setOpacity(1.0);
            block->color = kEsPrime;
            block->update();
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrP) m_ptrP->setOpacity(0);
        if (m_scanText) {
            setScriptText(m_scanText, QString("筛完 1~%1，共 %2 个素数").arg(m_n).arg(m_primeList.size()));
        }
        if (m_ruleText) {
            setScriptText(m_ruleText, QString("结论：每个合数恰好被它的最小质因子筛掉一次，线性筛总复杂度 O(n)"));
            m_ruleText->setDefaultTextColor(QColor(40, 140, 70));
        }
    }
}

void EulerSieveModule::reset() {
    m_frames.clear();
    m_cells.clear();
    m_primeBlocks.clear();
    m_primeList.clear();
    m_pendingCross.clear();
    stopCrossFlow();
    m_mathText = nullptr;
    m_primeTitle = nullptr;
    m_scanText = nullptr;
    m_ruleText = nullptr;
    m_ptrI = nullptr;
    m_ptrP = nullptr;
    m_n = 0;
    m_rows = 0;
    m_valid = false;
}

void EulerSieveModule::releaseSceneItems() {
    m_cells.clear();
    m_primeBlocks.clear();
    m_primeList.clear();
    m_pendingCross.clear();
    stopCrossFlow();
    m_mathText = nullptr;
    m_primeTitle = nullptr;
    m_scanText = nullptr;
    m_ruleText = nullptr;
    m_ptrI = nullptr;
    m_ptrP = nullptr;
}
