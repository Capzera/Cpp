#include "KMPMatchModule.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

static const QColor kKmpRed = QColor(238, 120, 108);
static const QColor kKmpBlue = QColor(80, 145, 220);
static const QColor kKmpOrange = QColor(250, 196, 110);
static const QColor kKmpGreen = QColor(140, 205, 150);

KMPMatchModule::KMPMatchModule() {}

QString KMPMatchModule::getModuleName() const { return "KMP 匹配过程"; }
QString KMPMatchModule::getDefaultInput() const { return "ABABCABAA\nABAB"; }
QString KMPMatchModule::getInputHint() const {
    return "第一行输入主串 s，第二行输入模式串 p\n"
           "本模块先构建 pi[] 前缀函数，再演示匹配过程中模式串的整体跳跃";
}

void KMPMatchModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    m_s = lines.size() > 0 ? lines[0].trimmed() : QString();
    m_p = lines.size() > 1 ? lines[1].trimmed() : QString();
    m_valid = !m_s.isEmpty() && !m_p.isEmpty() &&
              m_s.size() <= 14 && m_p.size() <= 10 &&
              !m_s.contains(QRegularExpression("\\s")) && !m_p.contains(QRegularExpression("\\s"));
    m_mathText = createScriptText(scene, m_valid ? "先求 pi[]，再匹配：主串指针 i 永不回退，模式串靠 pi[] 整体右跳"
                                        : "输入错误：需要两行非空字符串（主串 ≤ 14 字符，模式串 ≤ 10 字符）");
    m_mathText->setFont(QFont("Consolas", 14, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 22);
    if (!m_valid) return;

    m_n = static_cast<int>(m_s.size());
    m_m = static_cast<int>(m_p.size());
    m_pi.assign(m_m + 1, 0);

    QGraphicsTextItem* sTitle = scene->addText("s");
    sTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    sTitle->setDefaultTextColor(QColor(40, 70, 110));
    sTitle->setPos(m_sX - 42, m_sY - 2);
    for (int i = 0; i <= m_n; ++i) {
        QGraphicsRectItem* cell = scene->addRect(QRectF(0, 0, m_cellW, 42), QPen(QColor(70, 80, 95), 1.5), QBrush(Qt::white));
        cell->setPos(m_sX + i * m_cellW, m_sY);
        m_sCells.push_back(cell);
        QGraphicsTextItem* ch = scene->addText(i == 0 ? "#" : QString(m_s[i - 1]));
        ch->setFont(QFont("Consolas", 17, QFont::Bold));
        ch->setDefaultTextColor(QColor(35, 45, 60));
        ch->setPos(m_sX + i * m_cellW + m_cellW / 2 - ch->boundingRect().width() / 2, m_sY + 9);
        ch->setZValue(2);
        m_sChars.push_back(ch);
    }

    QGraphicsTextItem* pTitle = scene->addText("p");
    pTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    pTitle->setDefaultTextColor(QColor(40, 70, 110));
    pTitle->setPos(m_sX - 42, m_pY - 2);
    m_pTitle = pTitle;
    for (int i = 0; i <= m_m; ++i) {
        QGraphicsRectItem* cell = scene->addRect(QRectF(0, 0, m_cellW, 42), QPen(QColor(70, 80, 95), 1.5), QBrush(Qt::white));
        cell->setPos(m_sX + i * m_cellW, m_pY);
        m_pCells.push_back(cell);
        QGraphicsTextItem* ch = scene->addText(i == 0 ? "#" : QString(m_p[i - 1]));
        ch->setFont(QFont("Consolas", 17, QFont::Bold));
        ch->setDefaultTextColor(QColor(35, 45, 60));
        ch->setPos(m_sX + i * m_cellW + m_cellW / 2 - ch->boundingRect().width() / 2, m_pY + 9);
        ch->setZValue(2);
        m_pChars.push_back(ch);
    }

    QGraphicsTextItem* piTitle = scene->addText("pi");
    piTitle->setFont(QFont("Consolas", 13, QFont::Bold));
    piTitle->setDefaultTextColor(QColor(40, 70, 110));
    piTitle->setPos(m_sX - 42, m_piY);
    for (int i = 1; i <= m_m; ++i) {
        ArrayBlock* blk = new ArrayBlock(i, 0, QString::number(i));
        blk->showValue = false;
        blk->setCellSize(m_cellW, 40.0);
        blk->setPos(m_sX + i * m_cellW, m_piY);
        blk->color = QColor(232, 235, 240);
        scene->addItem(blk);
        m_piBlocks[i] = blk;
    }

    m_ptrI = new PointerBlock("i", kKmpRed);
    m_ptrI->setOpacity(0);
    scene->addItem(m_ptrI);
    m_ptrJ = new PointerBlock("j", kKmpBlue);
    m_ptrJ->setOpacity(0);
    scene->addItem(m_ptrJ);
}

void KMPMatchModule::layoutPatternRow(double offset) {
    for (int i = 0; i <= m_m; ++i) {
        m_pCells[i]->setPos(m_sX + i * m_cellW + offset, m_pY);
        m_pChars[i]->setPos(m_sX + i * m_cellW + offset + m_cellW / 2 - m_pChars[i]->boundingRect().width() / 2, m_pY + 9);
    }
    if (m_pTitle) m_pTitle->setPos(m_sX - 42 + offset, m_pY - 2);
}

void KMPMatchModule::syncPatternRow(int i, int j) {
    double target = (i - (j + 1)) * m_cellW;
    double current = m_patternOffset;
    double delta = target - current;
    if (qFuzzyCompare(current, target)) return;
    std::vector<QGraphicsItem*> movers;
    for (int k = 0; k <= m_m; ++k) {
        movers.push_back(m_pCells[k]);
        movers.push_back(m_pChars[k]);
    }
    if (m_pTitle) movers.push_back(m_pTitle);
    playRowShift(movers, QPointF(delta, 0));
    m_patternOffset = target;
}

void KMPMatchModule::refreshPatternHighlight() {
    for (int i = 0; i <= m_m; ++i) {
        m_pCells[i]->setBrush(QBrush(i == 0 ? QColor(238, 240, 244) : Qt::white));
        m_pChars[i]->setDefaultTextColor(QColor(35, 45, 60));
    }
}

std::vector<AnimFrame> KMPMatchModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    m_pi.assign(m_m + 1, 0);
    m_resultCount = 0;

    for (int i = 2, j = 0; i <= m_m; ++i) {
        while (j > 0 && m_p[i - 1] != m_p[j]) {
            m_frames.push_back({ActionType::KMPM_FALLBACK, i, j, m_pi[j], 0, {-1},
                                QString("p[%1] 与 p[%2] 不匹配，沿 pi 回退").arg(i).arg(j + 1),
                                QString("j = pi[%1] = %2").arg(j).arg(m_pi[j])});
            j = m_pi[j];
        }
        if (m_p[i - 1] == m_p[j]) {
            j++;
            m_frames.push_back({ActionType::KMPM_ADVANCE, i, j, 0, 0, {-1},
                                QString("p[%1] == p[%2]，前缀长度加一").arg(i).arg(j),
                                QString("j++ → j = %1").arg(j)});
        }
        m_pi[i] = j;
        m_frames.push_back({ActionType::KMPM_PI_SET, i, j, static_cast<int>(m_pi[i]), 0, {-1},
                            QString("确定 pi[%1]").arg(i),
                            QString("pi[%1] = %2").arg(i).arg(j)});
    }
    m_frames.push_back({ActionType::KMPM_RESET, 1, 0, 0, 0, {},
                        QString("pi[] 构建完成，开始在主串 s 上匹配"),
                        QString("for (i = 1, j = 0; i <= n; i++)")});

    for (int i = 1, j = 0; i <= m_n; ++i) {
        while (j > 0 && m_s[i - 1] != m_p[j]) {
            m_frames.push_back({ActionType::KMPM_COMPARE, i, j, 0, 0, {},
                                QString("比较 s[%1] 与 p[%2]").arg(i).arg(j + 1),
                                QString("s[%1] = '%2'　p[%3] = '%4'").arg(i).arg(m_s[i - 1]).arg(j + 1).arg(m_p[j])});
            int fallback = m_pi[j];
            m_frames.push_back({ActionType::KMPM_FALLBACK, i, j, fallback, 0, {},
                                QString("失配：j 回退到 pi[%1]，主串指针 i 不动").arg(j),
                                QString("j = pi[%1] = %2").arg(j).arg(fallback)});
            j = fallback;
        }
        if (j < m_m && m_s[i - 1] == m_p[j]) {
            j++;
            m_frames.push_back({ActionType::KMPM_ADVANCE, i, j, 0, 0, {},
                                QString("匹配成功，j 加一"),
                                QString("if (s[%1] == p[%2]) j++　→ j = %3").arg(i).arg(j).arg(j)});
        }
        if (j == m_m) {
            m_resultCount++;
            m_frames.push_back({ActionType::KMPM_HIT, i, m_m, i - m_m + 1, 0, {},
                                QString("完整匹配到模式串，起点为 %1").arg(i - m_m + 1),
                                QString("cout << i - j + 1 = %1").arg(i - m_m + 1)});
            j = m_pi[j];
            m_frames.push_back({ActionType::KMPM_ADVANCE, i, j, 0, 0, {},
                                QString("命中后按 pi 继续匹配，主串指针依旧不回退"),
                                QString("j = pi[%1] = %2").arg(m_m).arg(j)});
        }
    }

    m_frames.push_back({ActionType::KMPM_DONE, m_resultCount, 0, 0, 0, {},
                        QString("匹配结束，共命中 %1 处").arg(m_resultCount),
                        QString("主串指针 i 从未回退，总共 O(n+m)")});
    return m_frames;
}

void KMPMatchModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    const int n = m_n;
    const int m = m_m;
    if (frame.type == ActionType::KMPM_RESET) {
        refreshPatternHighlight();
        for (int i = 0; i <= n; ++i) m_sCells[i]->setBrush(QBrush(i == 0 ? QColor(238, 240, 244) : Qt::white));
        layoutPatternRow(0);
        m_patternOffset = 0.0;
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_ptrJ) m_ptrJ->setOpacity(0);
    } else if (frame.type == ActionType::KMPM_COMPARE) {
        int i = frame.u;
        int j = frame.v;
        for (int k = 1; k <= n; ++k) m_sCells[k]->setBrush(QBrush(Qt::white));
        for (int k = 1; k <= m; ++k) m_pCells[k]->setBrush(QBrush(Qt::white));
        if (i >= 1 && i <= n) m_sCells[i]->setBrush(QBrush(QColor(255, 224, 216)));
        if (j + 1 >= 1 && j + 1 <= m) m_pCells[j + 1]->setBrush(QBrush(QColor(255, 236, 200)));
        syncPatternRow(i, j);
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            playPointerMove(m_ptrI, QPointF(m_sX + i * m_cellW + m_cellW / 2 - 8, m_sY + 44));
        }
        if (m_ptrJ && j + 1 <= m) {
            m_ptrJ->setOpacity(1);
            playPointerMove(m_ptrJ, QPointF(m_pCells[j + 1]->pos().x() + m_cellW / 2 - 8, m_pY + 44), 460, QEasingCurve::InOutCubic);
        }
    } else if (frame.type == ActionType::KMPM_FALLBACK) {
        int i = frame.u;
        int j = frame.v;
        for (int k = 1; k <= m; ++k) m_pCells[k]->setBrush(QBrush(Qt::white));
        if (j >= 1 && j <= m) m_pCells[j]->setBrush(QBrush(QColor(205, 224, 248)));
        ArrayBlock* pi = m_piBlocks[j];
        if (pi) {
            pi->color = QColor(190, 216, 248);
            pi->update();
        }
        syncPatternRow(i, j);
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            playPointerMove(m_ptrI, QPointF(m_sX + i * m_cellW + m_cellW / 2 - 8, m_sY + 44));
        }
        if (m_ptrJ && j <= m) {
            m_ptrJ->setOpacity(1);
            playPointerMove(m_ptrJ, QPointF(m_pCells[j]->pos().x() + m_cellW / 2 - 8, m_pY + 44), 460, QEasingCurve::InOutCubic);
        }
    } else if (frame.type == ActionType::KMPM_ADVANCE) {
        int i = frame.u;
        int j = frame.val2;
        refreshPatternHighlight();
        if (j >= 1 && j <= m) m_pCells[j]->setBrush(QBrush(QColor(220, 240, 220)));
        syncPatternRow(i, j);
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            m_ptrI->setPos(m_sX + i * m_cellW + 23, m_sY + 44);
        }
        if (m_ptrJ && j <= m) {
            m_ptrJ->setOpacity(1);
            m_ptrJ->setPos(m_pCells[j]->pos() + QPointF(23, 44));
        }
        if (j >= 1 && j <= m && m_pCells[j]) {
            playCellRippleAt(m_pCells[j]->pos() + QPointF(m_cellW / 2.0, m_cellW / 2.0),
                             QSizeF(m_cellW, m_cellW), QColor(150, 215, 165), m_cellW * 0.3, 460);
        }
    } else if (frame.type == ActionType::KMPM_HIT) {
        int i = frame.u;
        int start = frame.val1;
        for (int k = start; k < start + m && k <= n; ++k) m_sCells[k]->setBrush(QBrush(kKmpGreen));
        ArrayBlock* blk = new ArrayBlock(m_resultCount, start, QString("pos%1").arg(m_resultCount));
        blk->setPos(700, m_piY + 96 + m_resultCount * 56);
        blk->color = QColor(200, 235, 205);
        m_scene->addItem(blk);
        m_resultBlocks.push_back(blk);
        playCellLanding(blk, QColor(200, 235, 205), 460);
        if (start >= 1 && start + m - 1 <= n) {
            playCellRippleAt(m_sCells[start]->pos() + QPointF(m_cellW / 2.0, m_cellW / 2.0),
                             QSizeF(m_cellW, m_cellW), kKmpGreen, m_cellW * 0.4, 700);
        }
        syncPatternRow(i, m);
        if (m_ptrI) {
            m_ptrI->setOpacity(1);
            playPointerMove(m_ptrI, QPointF(m_sX + i * m_cellW + 23, m_sY + 44));
        }
    } else if (frame.type == ActionType::KMPM_DONE) {
        for (auto& kv : m_piBlocks) {
            kv.second->color = kKmpGreen;
            kv.second->update();
            playNodePop(kv.second, 1.12, 240);
        }
    }
}

void KMPMatchModule::reset() {
    m_frames.clear();
    m_sCells.clear();
    m_pCells.clear();
    m_sChars.clear();
    m_pChars.clear();
    m_piBlocks.clear();
    m_resultBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
    m_pTitle = nullptr;
    m_s.clear();
    m_p.clear();
    m_pi.clear();
    m_n = 0;
    m_m = 0;
    m_resultCount = 0;
    m_valid = false;
    m_patternOffset = 0.0;
}

void KMPMatchModule::releaseSceneItems() {
    m_sCells.clear();
    m_pCells.clear();
    m_sChars.clear();
    m_pChars.clear();
    m_piBlocks.clear();
    m_resultBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
    m_pTitle = nullptr;
}
