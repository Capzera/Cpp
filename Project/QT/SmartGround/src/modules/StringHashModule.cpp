#include "StringHashModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "core/PointerGeometry.h"
#include "ui/UiFonts.h"

static const QColor kShChar = QColor(252, 252, 254);
static const QColor kShCharScan = QColor(255, 214, 150);
static const QColor kShHash = QColor(238, 243, 250);
static const QColor kShHashScan = QColor(198, 226, 250);
static const QColor kShHashHit = QColor(186, 232, 200);
static const QColor kShPow = QColor(246, 240, 252);
static const QColor kShPowScan = QColor(226, 206, 246);

static const double kShCharW = 52.0;
static const double kShHashW = 104.0;
static const double kShRowH = 34.0;
static const double kShLeft = 150.0;

static const double kShYChar1 = 96.0;
static const double kShYHash1 = 210.0;
static const double kShYChar2 = 324.0;
static const double kShYHash2 = 438.0;
static const double kShYPow = 552.0;

StringHashModule::StringHashModule() {}

QString StringHashModule::getModuleName() const { return "字符串哈希"; }
QString StringHashModule::getDefaultInput() const { return "abababc\nababc"; }
QString StringHashModule::getInputHint() const {
    return "第一行输入字符串 s1\n"
           "第二行输入字符串 s2\n"
           "使用单模哈希：p = 131，mod = 998244353\n"
           "p[i] = p[i-1] * p % mod\n"
           "h[i] = (h[i-1] * p + s[i-1]) % mod\n"
           "从 len = min(n1, n2) 开始递减，比较 s1 的前缀与 s2 的后缀、s2 的前缀与 s1 的后缀是否相等";
}

long long StringHashModule::getHash(const std::vector<long long>& h, int l, int r, const std::vector<long long>& p) {
    return (h[r] - h[l - 1] * p[r - l + 1] % kMod + kMod) % kMod;
}

ArrayBlock* StringHashModule::powerCell(int i) const {
    if (i < 0 || i >= static_cast<int>(m_powCells.size())) return nullptr;
    return m_powCells[i];
}

ArrayBlock* StringHashModule::hashCell(int table, int i) const {
    const std::vector<ArrayBlock*>& row = (table == 1) ? m_h1Cells : m_h2Cells;
    if (i < 0 || i >= static_cast<int>(row.size())) return nullptr;
    return row[i];
}

ArrayBlock* StringHashModule::charCell(int table, int i) const {
    const std::vector<ArrayBlock*>& row = (table == 1) ? m_s1Cells : m_s2Cells;
    if (i < 0 || i >= static_cast<int>(row.size())) return nullptr;
    return row[i];
}

void StringHashModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    if (lines.size() >= 2) {
        m_s1 = lines[0].trimmed();
        m_s2 = lines[1].trimmed();
        const auto okStr = [](const QString& s) {
            return s.size() >= 1 && s.size() <= 12 && !s.contains(QRegularExpression("[^A-Za-z0-9]"));
        };
        m_valid = okStr(m_s1) && okStr(m_s2);
    }

    m_mathText = createScriptText(scene, m_valid ? "字符串哈希：p = 131，mod = 998244353，h[i] = (h[i-1] * p + s[i-1]) % mod"
                                        : QString("输入格式：两个字符串（各 1~12 个字母或数字）"));
    m_mathText->setFont(UiFonts::mixed(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(110, 18);
    if (!m_valid) return;

    m_n1 = static_cast<int>(m_s1.size());
    m_n2 = static_cast<int>(m_s2.size());
    const int nMax = std::max(m_n1, m_n2);

    auto makeRow = [&](std::vector<ArrayBlock*>& store, int count, double w, double y,
                       const QColor& color, int idBase) {
        for (int i = 0; i < count; ++i) {
            ArrayBlock* block = new ArrayBlock(idBase + i, 0, QString::number(i));
            block->setCellSize(w, kShRowH);
            block->setShowLabel(false);
            block->color = color;
            block->setPos(kShLeft + i * w, y);
            scene->addItem(block);
            store.push_back(block);
        }
    };

    makeRow(m_s1Cells, m_n1 + 1, kShCharW, kShYChar1, kShChar, 0);
    makeRow(m_s2Cells, m_n2 + 1, kShCharW, kShYChar2, kShChar, 100);
    makeRow(m_h1Cells, m_n1 + 1, kShHashW, kShYHash1, kShHash, 200);
    makeRow(m_h2Cells, m_n2 + 1, kShHashW, kShYHash2, kShHash, 300);
    makeRow(m_powCells, nMax + 1, kShHashW, kShYPow, kShPow, 400);
    for (int i = 1; i <= m_n1; ++i) {
        m_s1Cells[i]->setCustomValue(QString(m_s1.at(i - 1)));
        m_s1Cells[i]->setValueFontSize(15);
    }
    for (int i = 1; i <= m_n2; ++i) {
        m_s2Cells[i]->setCustomValue(QString(m_s2.at(i - 1)));
        m_s2Cells[i]->setValueFontSize(15);
    }
    m_powCells[0]->setCustomValue("1");
    m_h1Cells[0]->setCustomValue("0");
    m_h2Cells[0]->setCustomValue("0");
    for (auto* b : m_h1Cells) b->setValueFontSize(12);
    for (auto* b : m_h2Cells) b->setValueFontSize(12);
    for (auto* b : m_powCells) b->setValueFontSize(12);

    auto addLabel = [&](const QString& text, double y, QColor c) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(UiFonts::cjk(12, true));
        item->setDefaultTextColor(c);
        item->setPos(52, y + 6);
        return item;
    };
    m_s1Label = addLabel("s1[]", kShYChar1, QColor(40, 70, 110));
    m_h1Label = addLabel("h1[]", kShYHash1, QColor(70, 100, 150));
    m_s2Label = addLabel("s2[]", kShYChar2, QColor(40, 70, 110));
    m_h2Label = addLabel("h2[]", kShYHash2, QColor(70, 100, 150));
    m_pLabel = addLabel("p[]", kShYPow, QColor(120, 80, 160));

    m_noteText = createScriptText(scene, "");
    m_noteText->setFont(UiFonts::mixed(13, true));
    m_noteText->setDefaultTextColor(QColor(200, 110, 40));
    m_noteText->setPos(52, 640);

    m_resultText = createScriptText(scene, "");
    m_resultText->setFont(UiFonts::cjk(13, true));
    m_resultText->setDefaultTextColor(QColor(40, 140, 70));
    m_resultText->setPos(52, 676);

    m_ptrLen = new PointerBlock("len", QColor(210, 80, 55));
    m_ptrLen->setOpacity(0);
    m_ptrLen->setDirection(PointerBlock::Down);
    m_ptrLen->setShaftLength(24);
    m_ptrLen->setAnchorOffset(PointerGeometry::aboveCell(kShHashW, kShRowH));
    m_ptrLen->setLabelAtTail(8.0);
    scene->addItem(m_ptrLen);

    m_ptrP1 = new PointerBlock("h1", QColor(45, 130, 210));
    m_ptrP1->setOpacity(0);
    m_ptrP1->setDirection(PointerBlock::Down);
    m_ptrP1->setShaftLength(24);
    m_ptrP1->setAnchorOffset(PointerGeometry::aboveCell(kShHashW, kShRowH));
    m_ptrP1->setLabelAtTail(8.0);
    scene->addItem(m_ptrP1);

    m_ptrP2 = new PointerBlock("h2", QColor(200, 80, 160));
    m_ptrP2->setOpacity(0);
    m_ptrP2->setDirection(PointerBlock::Down);
    m_ptrP2->setShaftLength(24);
    m_ptrP2->setAnchorOffset(PointerGeometry::aboveCell(kShHashW, kShRowH));
    m_ptrP2->setLabelAtTail(8.0);
    scene->addItem(m_ptrP2);

    m_ptrI = new PointerBlock("i", QColor(60, 160, 90));
    m_ptrI->setOpacity(0);
    m_ptrI->setDirection(PointerBlock::Up);
    m_ptrI->setShaftLength(24);
    m_ptrI->setAnchorOffset(PointerGeometry::belowCell(kShCharW, kShRowH));
    m_ptrI->setLabelAtTail(8.0);
    scene->addItem(m_ptrI);

    m_ptrJ = new PointerBlock("j", QColor(220, 120, 60));
    m_ptrJ->setOpacity(0);
    m_ptrJ->setDirection(PointerBlock::Up);
    m_ptrJ->setShaftLength(24);
    m_ptrJ->setAnchorOffset(PointerGeometry::belowCell(kShCharW, kShRowH));
    m_ptrJ->setLabelAtTail(8.0);
    scene->addItem(m_ptrJ);
}
std::vector<AnimFrame> StringHashModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    const int nMax = std::max(m_n1, m_n2);
    std::vector<long long> p(nMax + 1, 0);
    std::vector<long long> h1(m_n1 + 1, 0);
    std::vector<long long> h2(m_n2 + 1, 0);

    m_frames.push_back({ActionType::STH_RESET, 0, 0, 0, 0, {},
                        QString("初始化：p = 131，mod = 998244353，h[0] = 0，p[0] = 1"),
                        QString("p = 131, mod = 998244353")});

    p[0] = 1;
    for (int i = 1; i <= nMax; ++i) {
        p[i] = p[i - 1] * kP % kMod;
        m_frames.push_back({ActionType::STH_POW, i, 0, static_cast<int>(p[i]), 0, {},
                            QString("p[%1] = p[%2] * 131 %% mod = %3")
                                .arg(i).arg(i - 1).arg(p[i]),
                            QString("p[%1] = %2").arg(i).arg(p[i])});
    }

    for (int i = 1; i <= m_n1; ++i) {
        h1[i] = (h1[i - 1] * kP + static_cast<long long>(m_s1.at(i - 1).unicode())) % kMod;
        m_frames.push_back({ActionType::STH_PREP, i, 1, static_cast<int>(h1[i]), 0, {},
                            QString("h1[%1] = (h1[%2] * 131 + '%3') %% mod = %4")
                                .arg(i).arg(i - 1).arg(m_s1.at(i - 1)).arg(h1[i]),
                            QString("h1[%1] = %2").arg(i).arg(h1[i])});
    }

    for (int i = 1; i <= m_n2; ++i) {
        h2[i] = (h2[i - 1] * kP + static_cast<long long>(m_s2.at(i - 1).unicode())) % kMod;
        m_frames.push_back({ActionType::STH_PREP, i, 2, static_cast<int>(h2[i]), 0, {},
                            QString("h2[%1] = (h2[%2] * 131 + '%3') %% mod = %4")
                                .arg(i).arg(i - 1).arg(m_s2.at(i - 1)).arg(h2[i]),
                            QString("h2[%1] = %2").arg(i).arg(h2[i])});
    }

    int ans = 0;
    for (int len = std::min(m_n1, m_n2); len >= 1; --len) {
        const long long h1Prefix = getHash(h1, 1, len, p);
        const long long h2Suffix = getHash(h2, m_n2 - len + 1, m_n2, p);
        m_frames.push_back({ActionType::STH_QUERY, len, 0, static_cast<int>(h1Prefix),
                            static_cast<int>(h2Suffix),
                            {1, m_n2 - len + 1, m_n2},
                            QString("len = %1：getHash(h1, 1, %1) = %2 ，getHash(h2, %3, %4) = %5")
                                .arg(len).arg(h1Prefix).arg(m_n2 - len + 1).arg(m_n2).arg(h2Suffix),
                            QString("h1[1..%1] vs h2[%2..%3]").arg(len).arg(m_n2 - len + 1).arg(m_n2)});

        if (h1Prefix == h2Suffix) {
            m_frames.push_back({ActionType::STH_MATCH, len, 0, static_cast<int>(h1Prefix), 0,
                                {1, m_n2 - len + 1, m_n2},
                                QString("匹配成功：s1 的前 %1 个字符与 s2 的后 %1 个字符相同，ans = %1").arg(len),
                                QString("getHash(h1,1,len) == getHash(h2,n2-len+1,n2)")});
            ans = len;
            break;
        }

        const long long h2Prefix = getHash(h2, 1, len, p);
        const long long h1Suffix = getHash(h1, m_n1 - len + 1, m_n1, p);
        m_frames.push_back({ActionType::STH_QUERY, len, 1, static_cast<int>(h2Prefix),
                            static_cast<int>(h1Suffix),
                            {2, m_n1 - len + 1, m_n1},
                            QString("len = %1：getHash(h2, 1, %1) = %2 ，getHash(h1, %3, %4) = %5")
                                .arg(len).arg(h2Prefix).arg(m_n1 - len + 1).arg(m_n1).arg(h1Suffix),
                            QString("h2[1..%1] vs h1[%2..%3]").arg(len).arg(m_n1 - len + 1).arg(m_n1)});

        if (h2Prefix == h1Suffix) {
            m_frames.push_back({ActionType::STH_MATCH, len, 1, static_cast<int>(h2Prefix), 0,
                                {2, m_n1 - len + 1, m_n1},
                                QString("匹配成功：s2 的前 %1 个字符与 s1 的后 %1 个字符相同，ans = %1").arg(len),
                                QString("getHash(h2,1,len) == getHash(h1,n1-len+1,n1)")});
            ans = len;
            break;
        }
    }

    m_frames.push_back({ActionType::STH_DONE, ans, 0, 0, 0, {},
                        ans > 0 ? QString("最长公共子串长度 ans = %1").arg(ans)
                                : QString("没有找到公共子串，ans = 0"),
                        QString("从大到小枚举 len，第一个匹配的即为答案")});
    return m_frames;
}

void StringHashModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::STH_RESET) {
        for (auto* b : m_s1Cells) { b->color = kShChar; b->update(); }
        for (auto* b : m_s2Cells) { b->color = kShChar; b->update(); }
        for (auto* b : m_h1Cells) { b->color = kShHash; b->update(); }
        for (auto* b : m_h2Cells) { b->color = kShHash; b->update(); }
        for (auto* b : m_powCells) { b->color = kShPow; b->update(); }
        if (m_ptrLen) m_ptrLen->setOpacity(0);
        if (m_ptrP1) m_ptrP1->setOpacity(0);
        if (m_ptrP2) m_ptrP2->setOpacity(0);
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_noteText) {
            setScriptText(m_noteText, "");
            m_noteText->setDefaultTextColor(QColor(200, 110, 40));
        }
        if (m_resultText) setScriptText(m_resultText, "");
        return;
    }

    if (frame.type == ActionType::STH_POW) {
        const int index = frame.u;
        ArrayBlock* cell = powerCell(index);
        if (cell) {
            cell->setCustomValue(QString::number(frame.val1));
            cell->color = kShPowScan;
            cell->update();
        }
        if (m_noteText) {
            setScriptText(m_noteText, QString("p[%1] = %2").arg(index).arg(frame.val1));
        }
        return;
    }

    if (frame.type == ActionType::STH_PREP) {
        const int index = frame.u;
        const int kind = frame.v;
        if (kind == 0) {
            ArrayBlock* cell = powerCell(index);
            if (cell) {
                cell->setCustomValue(QString::number(frame.val1));
                cell->color = kShPowScan;
                cell->update();
            }
            if (m_noteText) {
                setScriptText(m_noteText, QString("p[%1] = %2").arg(index).arg(frame.val1));
            }
        } else if (kind == 1) {
            ArrayBlock* cell = hashCell(1, index);
            if (cell) {
                cell->setCustomValue(QString::number(frame.val1));
                cell->color = kShHashScan;
                cell->update();
            }
            ArrayBlock* ch = charCell(1, index);
            if (ch) {
                ch->color = kShCharScan;
                ch->update();
                if (m_ptrI) {
                    m_ptrI->setOpacity(1);
                    retargetPointerTo(m_ptrI, ch, PointerGeometry::belowCell(kShCharW, kShRowH));
                }
            }
            if (m_noteText) {
                setScriptText(m_noteText, QString("h1[%1] = %2").arg(index).arg(frame.val1));
            }
        } else {
            ArrayBlock* cell = hashCell(2, index);
            if (cell) {
                cell->setCustomValue(QString::number(frame.val1));
                cell->color = kShHashScan;
                cell->update();
            }
            ArrayBlock* ch = charCell(2, index);
            if (ch) {
                ch->color = kShCharScan;
                ch->update();
                if (m_ptrJ) {
                    m_ptrJ->setOpacity(1);
                    retargetPointerTo(m_ptrJ, ch, PointerGeometry::belowCell(kShCharW, kShRowH));
                }
            }
            if (m_noteText) {
                setScriptText(m_noteText, QString("h2[%1] = %2").arg(index).arg(frame.val1));
            }
        }
        return;
    }

    if (frame.type == ActionType::STH_QUERY) {
        for (auto* b : m_h1Cells) { if (b->color != kShHash) { b->color = kShHash; b->update(); } }
        for (auto* b : m_h2Cells) { if (b->color != kShHash) { b->color = kShHash; b->update(); } }
        for (auto* b : m_s1Cells) { if (b->color == kShCharScan) { b->color = kShChar; b->update(); } }
        for (auto* b : m_s2Cells) { if (b->color == kShCharScan) { b->color = kShChar; b->update(); } }

        const int len = frame.u;
        const int dir = frame.v;
        const int table1 = dir == 0 ? 1 : 2;
        const int table2 = dir == 0 ? 2 : 1;
        const int l2 = frame.group.size() > 1 ? frame.group[1] : 1;
        const int r2 = frame.group.size() > 2 ? frame.group[2] : 1;

        for (int i = 1; i <= len; ++i) {
            ArrayBlock* b = hashCell(table1, i);
            if (b) { b->color = kShHashScan; b->update(); }
        }
        for (int i = l2; i <= r2; ++i) {
            ArrayBlock* b = hashCell(table2, i);
            if (b) { b->color = kShHashScan; b->update(); }
        }
        if (m_animated) {
            ArrayBlock* probe = hashCell(table1, 1);
            if (probe) playCellRipple(probe, kShHashScan, 1.15, 620);
        }

        ArrayBlock* powTarget = powerCell(len);
        if (powTarget) {
            powTarget->color = kShPowScan;
            powTarget->update();
        }
        if (m_ptrLen && powTarget) {
            m_ptrLen->setOpacity(1);
            retargetPointerTo(m_ptrLen, powTarget, PointerGeometry::aboveCell(kShHashW, kShRowH));
        }
        ArrayBlock* first1 = hashCell(table1, 1);
        if (m_ptrP1 && first1) {
            m_ptrP1->setOpacity(1);
            retargetPointerTo(m_ptrP1, first1, PointerGeometry::aboveCell(kShHashW, kShRowH));
        }
        ArrayBlock* first2 = hashCell(table2, l2);
        if (m_ptrP2 && first2) {
            m_ptrP2->setOpacity(1);
            retargetPointerTo(m_ptrP2, first2, PointerGeometry::aboveCell(kShHashW, kShRowH));
        }
        if (m_noteText) setScriptText(m_noteText, frame.explanation);
        return;
    }

    if (frame.type == ActionType::STH_MATCH) {
        const int len = frame.u;
        const int dir = frame.v;
        const int table1 = dir == 0 ? 1 : 2;
        const int table2 = dir == 0 ? 2 : 1;
        const int l2 = frame.group.size() > 1 ? frame.group[1] : 1;
        const int r2 = frame.group.size() > 2 ? frame.group[2] : 1;
        for (int i = 1; i <= len; ++i) {
            ArrayBlock* b = hashCell(table1, i);
            if (b) { b->color = kShHashHit; b->update(); }
        }
        for (int i = l2; i <= r2; ++i) {
            ArrayBlock* b = hashCell(table2, i);
            if (b) { b->color = kShHashHit; b->update(); }
        }
        for (int i = 1; i <= len; ++i) {
            ArrayBlock* b = charCell(table1, i);
            if (b) { b->color = kShHashHit; b->update(); playNodePop(b, 1.14, 240); }
        }
        for (int i = l2; i <= r2; ++i) {
            ArrayBlock* b = charCell(table2, i);
            if (b) { b->color = kShHashHit; b->update(); playNodePop(b, 1.14, 240); }
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.explanation);
            m_noteText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_ptrI) m_ptrI->setOpacity(0);
        return;
    }

    if (frame.type == ActionType::STH_DONE) {
        if (m_ptrLen) m_ptrLen->setOpacity(0);
        if (m_ptrP1) m_ptrP1->setOpacity(0);
        if (m_ptrP2) m_ptrP2->setOpacity(0);
        if (m_ptrI) m_ptrI->setOpacity(0);
        if (m_resultText) {
            setScriptText(m_resultText, frame.explanation);
            m_resultText->setDefaultTextColor(QColor(40, 140, 70));
        }
        if (m_noteText) {
            setScriptText(m_noteText, frame.mathText);
            m_noteText->setDefaultTextColor(QColor(70, 110, 170));
        }
    }
}

void StringHashModule::reset() {
    m_frames.clear();
    m_s1.clear();
    m_s2.clear();
    m_n1 = 0;
    m_n2 = 0;
    m_powCells.clear();
    m_s1Cells.clear();
    m_h1Cells.clear();
    m_s2Cells.clear();
    m_h2Cells.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_resultText = nullptr;
    m_s1Label = nullptr;
    m_s2Label = nullptr;
    m_pLabel = nullptr;
    m_h1Label = nullptr;
    m_h2Label = nullptr;
    m_ptrLen = nullptr;
    m_ptrP1 = nullptr;
    m_ptrP2 = nullptr;
    m_valid = false;
}

void StringHashModule::releaseSceneItems() {
    m_powCells.clear();
    m_s1Cells.clear();
    m_h1Cells.clear();
    m_s2Cells.clear();
    m_h2Cells.clear();
    m_mathText = nullptr;
    m_noteText = nullptr;
    m_resultText = nullptr;
    m_s1Label = nullptr;
    m_s2Label = nullptr;
    m_pLabel = nullptr;
    m_h1Label = nullptr;
    m_h2Label = nullptr;
    m_ptrLen = nullptr;
    m_ptrP1 = nullptr;
    m_ptrP2 = nullptr;
}
