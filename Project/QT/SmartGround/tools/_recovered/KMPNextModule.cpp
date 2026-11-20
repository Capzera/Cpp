#include "KMPNextModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>

static const QColor kPiRed = QColor(238, 120, 108);
static const QColor kPiBlue = QColor(80, 145, 220);
static const QColor kPiOrange = QColor(250, 196, 110);
static const QColor kPiGreen = QColor(140, 205, 150);

KMPNextModule::KMPNextModule() : m_ptrI(nullptr), m_ptrJ(nullptr), m_mathText(nullptr), m_startX(0), m_cellW(56), m_valid(false) {}

QString KMPNextModule::getModuleName() const { return "KMP pi 数组（前缀函数）"; }
QString KMPNextModule::getDefaultInput() const { return "ABABCABAA"; }
QString KMPNextModule::getInputHint() const {
    return "输入一行模式串（长度 1 - 12，不含空白字符）\n"
           "本模块按 p = \"#\" + p 的 1-indexed 写法构建前缀函数 pi[]";
}

void KMPNextModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    m_pattern = inputData.trimmed();
    m_valid = !m_pattern.isEmpty() && m_pattern.size() <= 12 && !m_pattern.contains(QRegularExpression("\\s"));
    m_mathText = scene->addText(m_valid ? "pi[]：p 的前缀函数，p 已加上哨兵 #，下标从 1 开始"
                                        : "输入格式：一行模式串；限制长度 1–12，暂不接受空白字符");
    m_mathText->setFont(QFont("Consolas", 13, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(50, 18);
    if (!m_valid) return;
    int m = static_cast<int>(m_pattern.size());
    m_startX = std::max(140, 400 - m * m_cellW);
    QGraphicsTextItem* patternTitle = scene->addText("p");
    patternTitle->setFont(QFont("Consolas", 12, QFont::Bold));
    patternTitle->setDefaultTextColor(QColor(40, 70, 110));
    patternTitle->setPos(m_startX - 36, 178);
    QGraphicsTextItem* piTitle = scene->addText("pi");
    piTitle->setFont(QFont("Consolas", 12, QFont::Bold));
    piTitle->setDefaultTextColor(QColor(40, 70, 110));
    piTitle->setPos(m_startX - 36, 288);
    for (int i = 0; i <= m; ++i) {
        QGraphicsRectItem* cell = scene->addRect(QRectF(0, 0, m_cellW, 42), QPen(QColor(70, 80, 95), 1.5), QBrush(Qt::white));
        cell->setPos(m_startX + i * m_cellW, 195);
        m_charCells[i] = cell;
        QGraphicsTextItem* character = scene->addText(i == 0 ? "#" : QString(m_pattern[i - 1]));
        character->setFont(QFont("Consolas", 17, QFont::Bold));
        character->setDefaultTextColor(QColor(35, 45, 60));
        character->setPos(m_startX + i * m_cellW + m_cellW / 2 - character->boundingRect().width() / 2, 201);
        character->setZValue(2);
        m_charLabels[i] = character;
    }
    for (int i = 1; i <= m; ++i) {
        ArrayBlock* block = new ArrayBlock(i, 0, QString::number(i));
        block->showValue = false;
        block->setCellSize(m_cellW, 40.0);
        block->setPos(m_startX + i * m_cellW, 305);
        scene->addItem(block);
        m_piBlocks[i] = block;
    }
    m_ptrI = new PointerBlock("i", kPiRed);
    m_ptrJ = new PointerBlock("j", kPiBlue);
    m_ptrI->setOpacity(0);
    m_ptrJ->setOpacity(0);
    scene->addItem(m_ptrI);
    scene->addItem(m_ptrJ);
}

std::vector<AnimFrame> KMPNextModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;
    int m = static_cast<int>(m_pattern.size());
    m_pi.assign(m + 1, 0);
    m_frames.push_back({ActionType::KMP_RESET, 0, 0, 0, 0, {}, "从第二个字符开始，pi[1] = 0", "for (i = 2, j = 0; i <= m; i++)"});
    m_frames.push_back({ActionType::KMP_SET, 1, 0, 0, 0, {}, "哨兵 # 与任何字符都不相等，pi[1] 必为 0", "pi[1] = 0"});
    for (int i = 2; i <= m; ++i) {
        int j = m_pi[i - 1];
        while (j > 0 && m_pattern[i - 1] != m_pattern[j]) {
            m_frames.push_back({ActionType::KMP_COMPARE, i, j + 1, static_cast<ushort>(m_pattern[i - 1].unicode()), static_cast<ushort>(m_pattern[j].unicode()), {0},
                                QString("比较 p[%1] 与 p[%2]").arg(i).arg(j + 1),
                                QString("p[%1] = '%2'　p[%3] = '%4'").arg(i).arg(m_pattern[i - 1]).arg(j + 1).arg(m_pattern[j])});
            int fallback = m_pi[j];
            m_frames.push_back({ActionType::KMP_FALLBACK, i, j, fallback, 0, {},
                                QString("失配：沿已知前缀回退"),
                                QString("j = pi[%1] = %2").arg(j).arg(fallback)});
            j = fallback;
        }
        bool matched = m_pattern[i - 1] == m_pattern[j];
        m_frames.push_back({ActionType::KMP_COMPARE, i, j + 1, static_cast<ushort>(m_pattern[i - 1].unicode()), static_cast<ushort>(m_pattern[j].unicode()), {matched ? 1 : 0},
                            QString("比较 p[%1] 与 p[%2]").arg(i).arg(j + 1),
                            QString("p[%1] = '%2'　p[%3] = '%4'").arg(i).arg(m_pattern[i - 1]).arg(j + 1).arg(m_pattern[j])});
        if (matched) ++j;
        m_pi[i] = j;
        m_frames.push_back({ActionType::KMP_SET, i, j, j, matched ? 1 : 0, {},
                            QString("确定 pi[%1]").arg(i),
                            QString("pi[%1] = %2").arg(i).arg(j)});
    }
    m_frames.push_back({ActionType::KMP_DONE, m, 0, 0, 0, {}, "pi 数组构建完成", "pi[i] = p[1..i] 的最长真前后缀长度"});
    return m_frames;
}

void KMPNextModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    int m = static_cast<int>(m_pattern.size());
    if (frame.type == ActionType::KMP_RESET) {
        for (int i = 0; i <= m; ++i) {
            m_charCells[i]->setBrush(QBrush(i == 0 ? QColor(238, 240, 244) : Qt::white));
        }
        for (int i = 1; i <= m; ++i) {
            m_piBlocks[i]->showValue = false;
            m_piBlocks[i]->value = 0;
            m_piBlocks[i]->color = QColor(232, 235, 240);
            m_piBlocks[i]->update();
        }
        m_ptrI->setOpacity(0);
        m_ptrJ->setOpacity(0);
    } else if (frame.type == ActionType::KMP_COMPARE) {
        for (int i = 0; i <= m; ++i) m_charCells[i]->setBrush(QBrush(i == 0 ? QColor(238, 240, 244) : Qt::white));
        m_charCells[frame.u]->setBrush(QBrush(QColor(255, 224, 216)));
        m_charCells[frame.v]->setBrush(QBrush(frame.group.size() && frame.group[0] ? QColor(200, 235, 205) : QColor(255, 236, 200)));
        m_ptrI->setOpacity(1);
        m_ptrJ->setOpacity(1);
        playPointerArrange({m_ptrI, m_ptrJ}, {m_charCells[frame.u], m_charCells[frame.v]}, 46.0);
    } else if (frame.type == ActionType::KMP_FALLBACK) {
        m_charCells[frame.v]->setBrush(QBrush(QColor(205, 224, 248)));
        m_ptrJ->setOpacity(1);
        playPointerArrange({m_ptrJ}, {m_charCells[frame.val1]}, 46.0);
    } else if (frame.type == ActionType::KMP_SET) {
        m_piBlocks[frame.u]->value = frame.val1;
        m_piBlocks[frame.u]->showValue = true;
        m_piBlocks[frame.u]->color = QColor(200, 235, 205);
        m_piBlocks[frame.u]->update();
        if (frame.u > 1) {
            m_ptrI->setOpacity(1);
            m_ptrJ->setOpacity(1);
            playPointerArrange({m_ptrI, m_ptrJ}, {m_charCells[frame.u], m_charCells[frame.val1]}, 46.0);
        }
    } else if (frame.type == ActionType::KMP_DONE) {
        for (int i = 1; i <= m; ++i) {
            if (m_piBlocks[i]->showValue) m_piBlocks[i]->color = QColor(200, 235, 205);
            m_piBlocks[i]->update();
        }
        m_ptrI->setOpacity(0);
        m_ptrJ->setOpacity(0);
    }
}

void KMPNextModule::reset() {
    m_frames.clear();
    m_charLabels.clear();
    m_charCells.clear();
    m_piBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
    m_pattern.clear();
    m_pi.clear();
    m_startX = 0;
    m_valid = false;
}

void KMPNextModule::releaseSceneItems() {
    m_charLabels.clear();
    m_charCells.clear();
    m_piBlocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_mathText = nullptr;
}
