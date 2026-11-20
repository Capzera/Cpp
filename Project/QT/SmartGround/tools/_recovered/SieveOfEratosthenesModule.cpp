#include "SieveOfEratosthenesModule.h"
#include <QFont>
#include <QRegularExpression>
#include <QVariantAnimation>
#include <QPointer>
#include <cmath>

static const QColor kSvPale = QColor(252, 252, 254);
static const QColor kSvPrime = QColor(150, 214, 170);
static const QColor kSvCross = QColor(206, 210, 218);
static const QColor kSvScan = QColor(255, 224, 165);

SieveOfEratosthenesModule::SieveOfEratosthenesModule() {}

QString SieveOfEratosthenesModule::getModuleName() const { return "埃氏筛"; }
QString SieveOfEratosthenesModule::getDefaultInput() const { return "120"; }

QString SieveOfEratosthenesModule::getInputHint() const {
    return "只需输入一个正整数 n，表示要筛出 1~n 范围内的所有素数\n"
           "例如输入 120，右侧会显示 12 行 × 10 列的矩阵\n"
           "算法从 i*i 开始按步长 i 划掉 i 的倍数\n"
           "同一个素数的全部倍数在一帧内按顺序流动划掉；被划掉的数字保留显示，只是变灰并打叉";
}

void SieveOfEratosthenesModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
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

    m_mathText = scene->addText(m_valid ? "埃氏筛：找到素数 i 后，从 i*i 起划掉 i 的倍数" : "输入格式：单个整数 n（2 ≤ n ≤ 400）");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(60, 18);
    if (!m_valid) return;

    m_rows = (m_n + kColumns - 1) / kColumns;
    const double cell = 46.0;
    const double originX = 70.0;
    const double originY = 90.0;

    for (int value = 1; value <= m_n; ++value) {
        int row = (value - 1) / kColumns;
        int col = (value - 1) % kColumns;
        ArrayBlock* cellBlock = new ArrayBlock(value, value, "");
        cellBlock->setCellSize(cell, cell);
        cellBlock->setShowLabel(false);
        cellBlock->setPos(originX + col * cell, originY + row * cell);
        cellBlock->color = kSvPale;
        scene->addItem(cellBlock);
        m_cells[value] = cellBlock;
    }

    m_scanText = scene->addText("");
    m_scanText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_scanText->setDefaultTextColor(QColor(190, 110, 40));
    m_scanText->setPos(originX, originY + m_rows * cell + 16);

    const double primeX = originX + kColumns * cell + 70.0;
    m_primeTitle = scene->addText("素数表 prime[]");
    m_primeTitle->setFont(QFont("Microsoft YaHei", 11, QFont::Bold));
    m_primeTitle->setDefaultTextColor(QColor(60, 75, 95));
    m_primeTitle->setPos(primeX, 90.0);

    const double primeCell = 52.0;
    int primeCount = 0;
    for (int value = 2; value <= m_n; ++value) {
        bool isPrime = true;
        for (int d = 2; d * d <= value; ++d) {
            if (value % d == 0) { isPrime = false; break; }
        }
        if (!isPrime) continue;
        ArrayBlock* block = new ArrayBlock(value, value, QString::number(primeCount));
        block->setCellSize(primeCell, 44);
        block->setPos(primeX + (primeCount % 6) * (primeCell + 6), 120.0 + (primeCount / 6) * (primeCell + 8));
        block->color = kSvPale;
        block->setOpacity(0);
        scene->addItem(block);
        m_primeBlocks.push_back(block);
        ++primeCount;
    }
}

std::vector<AnimFrame> SieveOfEratosthenesModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    std::vector<bool> st(m_n + 1, false);
    st[1] = true;
    std::vector<int> primeList;

    m_frames.push_back({ActionType::SV_RESET, 0, 0, m_n, 0, {}, "初始化：标记 st[1] = 1，从 i = 2 开始扫描", "st[1] = 1"});

    for (long long i = 2; i <= m_n; ++i) {
        if (st[i]) continue;
        primeList.push_back(static_cast<int>(i));
        m_frames.push_back({ActionType::SV_PRIME, static_cast<int>(i), 0, static_cast<int>(i), 0, {},
                            QString("%1 未被划掉，是素数，加入 prime[]").arg(i),
                            QString("i = %1　是素数").arg(i)});
        std::vector<int> multiples;
        for (long long j = i * i; j <= m_n; j += i) {
            st[j] = true;
            multiples.push_back(static_cast<int>(j));
        }
        if (multiples.empty()) continue;
        m_frames.push_back({ActionType::SV_SCAN, static_cast<int>(i), 0, static_cast<int>(multiples.size()), 0, multiples,
                            QString("从 i*i = %1 起，划掉 %1 的 %2 个倍数").arg(i).arg(multiples.size()),
                            QString("j = i*i = %1　共 %2 个倍数").arg(i * i).arg(multiples.size())});
        m_frames.push_back({ActionType::SV_CROSS_BATCH, static_cast<int>(i), 0, static_cast<int>(multiples.size()), 0, multiples,
                            QString("一帧流动划掉 %1 的 %2 个倍数").arg(i).arg(multiples.size()),
                            QString("st[j] = 1　j = %1, %1+%2, ...").arg(i * i).arg(i)});
    }
    m_frames.push_back({ActionType::SV_PRIME_LIST, 0, 0, static_cast<int>(primeList.size()), 0, {primeList.begin(), primeList.end()},
                        QString("共筛出 %1 个素数").arg(primeList.size()),
                        QString("prime[] 共 %1 个素数").arg(primeList.size())});
    m_frames.push_back({ActionType::SV_DONE, 0, 0, 0, 0, {}, "埃氏筛完成", "时间复杂度 O(n log log n)"});
    return m_frames;
}

void SieveOfEratosthenesModule::applyCross(ArrayBlock* block) {
    if (!block) return;
    block->showValue = true;
    block->customValue.clear();
    block->color = kSvCross;
    block->setStruck(true);
    block->update();
}

void SieveOfEratosthenesModule::stopCrossFlow() {
    if (m_crossAnim) {
        m_crossAnim->stop();
        m_crossAnim = nullptr;
    }
    m_pendingCross.clear();
    m_crossDone = 0;
}

void SieveOfEratosthenesModule::flushPendingCross() {
    if (m_pendingCross.empty()) return;
    for (size_t i = static_cast<size_t>(m_crossDone); i < m_pendingCross.size(); ++i) {
        applyCross(m_pendingCross[i]);
    }
    stopCrossFlow();
}

void SieveOfEratosthenesModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type != ActionType::SV_CROSS_BATCH) {
        flushPendingCross();
    }

    if (frame.type == ActionType::SV_RESET) {
        stopCrossFlow();
        for (auto& kv : m_cells) {
            kv.second->color = kSvPale;
            kv.second->showValue = true;
            kv.second->customValue.clear();
            kv.second->setStruck(false);
            kv.second->setScale(1.0);
            kv.second->update();
        }
        for (ArrayBlock* block : m_primeBlocks) {            if (!block) continue;
            block->setOpacity(0);
            block->color = kSvPale;
            block->update();
        }
        if (m_scanText) m_scanText->setPlainText("");
    } else if (frame.type == ActionType::SV_SCAN) {
        int prime = frame.u;
        if (m_scanText) {
            m_scanText->setPlainText(QString("扫描线停在 %1　准备划掉它的 %2 个倍数").arg(prime).arg(frame.val1));
        }
        for (int value : frame.group) {
            auto it = m_cells.find(value);
            if (it == m_cells.end()) continue;
            if (it->second->isStruck()) continue;
            it->second->color = kSvScan;
            it->second->update();
        }
    } else if (frame.type == ActionType::SV_PRIME) {
        int value = frame.u;
        auto it = m_cells.find(value);
        if (it != m_cells.end()) {
            it->second->color = kSvPrime;
            it->second->setStruck(false);
            it->second->setScale(1.0);
            it->second->update();
            if (m_animated) playScalePulse(it->second, 1.3, 300);
        }
        if (m_scanText) m_scanText->setPlainText(QString("当前素数 %1").arg(value));
    } else if (frame.type == ActionType::SV_CROSS_BATCH) {
        int prime = frame.u;
        std::vector<ArrayBlock*> targets;
        for (int value : frame.group) {
            auto it = m_cells.find(value);
            if (it != m_cells.end()) targets.push_back(it->second);
        }
        if (m_scanText) {
            m_scanText->setPlainText(QString("划掉 %1 的倍数：%2").arg(prime).arg(frame.val1));
        }
        if (targets.empty()) return;
        if (!m_animated) {
            for (ArrayBlock* block : targets) applyCross(block);
            return;
        }
        stopCrossFlow();
        m_pendingCross = targets;
        m_crossDone = 0;
        QVariantAnimation* anim = new QVariantAnimation(this);
        m_crossAnim = anim;
        anim->setDuration(std::min(1500, std::max(280, static_cast<int>(targets.size()) * 28)));
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::Linear);
        QPointer<QVariantAnimation> safeAnim(anim);
        QObject::connect(anim, &QVariantAnimation::valueChanged, [this, targets, safeAnim](const QVariant& value) {
            if (!safeAnim) return;
            qreal t = value.toReal();
            int total = static_cast<int>(targets.size());
            int upto = static_cast<int>(t * total);
            if (upto > total) upto = total;
            while (m_crossDone < upto) {
                ArrayBlock* block = targets[m_crossDone];
                applyCross(block);
                if (block) {
                    QPointF c = block->pos() + QPointF(block->cellWidth() / 2.0, block->cellHeight() / 2.0);
                    playRipple(c, QColor(214, 118, 108), 24.0, 420);
                }
                ++m_crossDone;
            }
        });
        QObject::connect(anim, &QVariantAnimation::finished, [this, targets]() {
            while (m_crossDone < static_cast<int>(targets.size())) {
                applyCross(targets[m_crossDone]);
                ++m_crossDone;
            }
            m_pendingCross.clear();
            m_crossDone = 0;
            if (m_crossAnim) {
                m_crossAnim->deleteLater();
                m_crossAnim = nullptr;
            }
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else if (frame.type == ActionType::SV_PRIME_LIST) {
        int shown = 0;
        for (int p : frame.group) {
            if (shown >= static_cast<int>(m_primeBlocks.size())) break;
            ArrayBlock* block = m_primeBlocks[shown];
            block->value = p;
            block->customValue.clear();
            block->showValue = true;
            block->color = kSvPrime;
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
            } else {
                block->setOpacity(1.0);
            }
            ++shown;
        }
    } else if (frame.type == ActionType::SV_DONE) {
        if (m_scanText) {
            m_scanText->setPlainText(QString("筛完 1~%1，共 %2 个素数").arg(m_n).arg(m_primeBlocks.size()));
        }
    }
}

void SieveOfEratosthenesModule::reset() {
    m_frames.clear();
    m_cells.clear();
    m_primeBlocks.clear();
    stopCrossFlow();
    m_mathText = nullptr;
    m_primeTitle = nullptr;
    m_scanText = nullptr;
    m_n = 0;
    m_rows = 0;
    m_valid = false;
}

void SieveOfEratosthenesModule::releaseSceneItems() {
    m_cells.clear();
    m_primeBlocks.clear();
    stopCrossFlow();
    m_mathText = nullptr;
    m_primeTitle = nullptr;
    m_scanText = nullptr;
}
