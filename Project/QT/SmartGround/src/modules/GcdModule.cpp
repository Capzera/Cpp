#include "GcdModule.h"
#include <QFont>
#include <QRegularExpression>
#include <algorithm>
#include "ui/UiFonts.h"

static const QColor kGcA = QColor(130, 180, 235);
static const QColor kGcB = QColor(245, 195, 145);
static const QColor kGcR = QColor(140, 205, 160);
static const QColor kGcIdle = QColor(250, 250, 252);

GcdModule::GcdModule() {}

QString GcdModule::getModuleName() const { return "GCD 辗转相除"; }
QString GcdModule::getDefaultInput() const { return "48 18"; }
QString GcdModule::getInputHint() const {
    return "第一行输入两个正整数 a b（均不超过 100000）\n"
           "辗转相除：先算余数 r = a % b，再让 a = b、b = r（小的替换大的）\n"
           "直到 b = 0，此时的 a 就是最大公约数 gcd(a, b)";
}

void GcdModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    const QStringList tokens = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok = false;
    int a = 0;
    int b = 0;
    if (tokens.size() >= 2) {
        a = tokens[0].toInt(&ok);
        const bool okA = ok && a >= 1 && a <= 100000;
        b = tokens[1].toInt(&ok);
        m_valid = okA && ok && b >= 1 && b <= 100000;
    }

    m_mathText = createScriptText(scene, m_valid ? "辗转相除：r = a % b，小的替换大的，直到 b = 0"
                                        : "输入格式：两个正整数 a b");
    m_mathText->setFont(UiFonts::mono(15, true));
    m_mathText->setDefaultTextColor(QColor(40, 70, 110));
    m_mathText->setPos(150, 18);
    if (!m_valid) return;

    m_a = a;
    m_b = b;

    const double boxX = 330.0;
    const double boxW = 150.0;
    const double boxH = 84.0;

    auto makeLabel = [scene](const QString& text, double x, double y, int size, QColor color) {
        QGraphicsTextItem* item = scene->addText(text);
        item->setFont(UiFonts::cjk(size, true));
        item->setDefaultTextColor(color);
        item->setPos(x, y);
        return item;
    };

    m_aLabel = makeLabel("a（较大的数）", 150, 86, 11, QColor(60, 75, 95));
    m_bLabel = makeLabel("b（较小的数）", 150, 236, 11, QColor(60, 75, 95));
    m_rLabel = makeLabel("余数 r = a % b", 150, 386, 11, QColor(60, 75, 95));

    m_aBox = new ArrayBlock(0, 0, QString());
    m_aBox->setCellSize(boxW, boxH);
    m_aBox->setShowLabel(false);
    m_aBox->setValueFontSize(34);
    m_aBox->setCustomValue(QString::number(a));
    m_aBox->color = kGcA;
    m_aBox->setPos(boxX, 118);
    scene->addItem(m_aBox);

    m_bBox = new ArrayBlock(1, 0, QString());
    m_bBox->setCellSize(boxW, boxH);
    m_bBox->setShowLabel(false);
    m_bBox->setValueFontSize(34);
    m_bBox->setCustomValue(QString::number(b));
    m_bBox->color = kGcB;
    m_bBox->setPos(boxX, 268);
    scene->addItem(m_bBox);

    m_rBox = new ArrayBlock(2, 0, QString());
    m_rBox->setCellSize(boxW, boxH);
    m_rBox->setShowLabel(false);
    m_rBox->setValueFontSize(34);
    m_rBox->setCustomValue("—");
    m_rBox->color = kGcIdle;
    m_rBox->setPos(boxX, 418);
    scene->addItem(m_rBox);

    m_aValue = scene->addText("");
    m_aValue->setFont(UiFonts::mono(13, true));
    m_aValue->setDefaultTextColor(QColor(60, 75, 95));
    m_aValue->setPos(boxX + boxW + 26, 150);

    m_bValue = scene->addText("");
    m_bValue->setFont(UiFonts::mono(13, true));
    m_bValue->setDefaultTextColor(QColor(60, 75, 95));
    m_bValue->setPos(boxX + boxW + 26, 300);

    m_rValue = scene->addText("");
    m_rValue->setFont(UiFonts::mono(13, true));
    m_rValue->setDefaultTextColor(QColor(60, 75, 95));
    m_rValue->setPos(boxX + boxW + 26, 450);
}

std::vector<AnimFrame> GcdModule::runAlgorithm() {
    m_frames.clear();
    if (!m_valid) return m_frames;

    int a = m_a;
    int b = m_b;

    m_frames.push_back({ActionType::GC_RESET, a, b, 0, 0, {},
                        QString("初始状态：a = %1，b = %2").arg(a).arg(b),
                        QString("a = %1, b = %2").arg(a).arg(b)});

    int round = 0;
    int guard = 0;
    while (b != 0 && guard < 200) {
        ++guard;
        const int r = a % b;
        m_frames.push_back({ActionType::GC_DIVIDE, a, b, r, round, {},
                            QString("求余：%1 %% %2 = %3").arg(a).arg(b).arg(r),
                            QString("r = %1 %% %2 = %3").arg(a).arg(b).arg(r)});
        m_frames.push_back({ActionType::GC_SWAP, b, r, round, 0, {},
                            QString("小的替换大的：a = %1，b = %2").arg(b).arg(r),
                            QString("a = b = %1, b = r = %2").arg(b).arg(r)});
        a = b;
        b = r;
        ++round;
    }

    m_frames.push_back({ActionType::GC_DONE, a, b, round, 0, {},
                        QString("b = 0，最大公约数 gcd = %1").arg(a),
                        QString("gcd = %1").arg(a)});
    return m_frames;
}

void GcdModule::refreshBoxes() {
    if (m_aBox) { m_aBox->update(); }
    if (m_bBox) { m_bBox->update(); }
    if (m_rBox) { m_rBox->update(); }
}

void GcdModule::renderFrame(const AnimFrame& frame) {
    if (m_mathText) {
        setScriptText(m_mathText, frame.mathText.isEmpty() ? frame.explanation : frame.mathText);
    }
    if (!m_valid) return;

    if (frame.type == ActionType::GC_RESET) {
        m_aBox->setCustomValue(QString::number(frame.u));
        m_aBox->color = kGcA;
        m_bBox->setCustomValue(QString::number(frame.v));
        m_bBox->color = kGcB;
        m_rBox->setCustomValue("—");
        m_rBox->color = kGcIdle;
        m_aValue->setPlainText(QString("被除数"));
        m_bValue->setPlainText(QString("除数"));
        m_rValue->setPlainText(QString("待求"));
        refreshBoxes();
    } else if (frame.type == ActionType::GC_DIVIDE) {
        m_aBox->setCustomValue(QString::number(frame.u));
        m_aBox->color = kGcA;
        m_bBox->setCustomValue(QString::number(frame.v));
        m_bBox->color = kGcB;
        m_rBox->setCustomValue(QString::number(frame.val1));
        m_rBox->color = kGcR;
        m_aValue->setPlainText(QString("被除数 %1").arg(frame.u));
        m_bValue->setPlainText(QString("除数 %1").arg(frame.v));
        m_rValue->setPlainText(QString("%1 %% %2 = %3").arg(frame.u).arg(frame.v).arg(frame.val1));
        refreshBoxes();
        if (m_animated) {
            playScalePulse(m_rBox, 1.16, 280);
            playCellRipple(m_rBox, kGcR, 1.15, 600);
        }
    } else if (frame.type == ActionType::GC_SWAP) {
        m_aBox->setCustomValue(QString::number(frame.u));
        m_aBox->color = kGcA;
        m_bBox->setCustomValue(QString::number(frame.val1));
        m_bBox->color = kGcB;
        m_rBox->setCustomValue("—");
        m_rBox->color = kGcIdle;
        m_aValue->setPlainText(QString("接替原 b"));
        m_bValue->setPlainText(QString("接替原 r"));
        m_rValue->setPlainText(QString("余数已用"));
        refreshBoxes();
        if (m_animated) {
            playValueArcFlight(m_bBox->centerPos(), m_aBox->centerPos(), frame.u, kGcA, 480);
            playValueArcFlight(m_rBox->centerPos(), m_bBox->centerPos(), frame.val1, kGcB, 480);
            playScalePulse(m_aBox, 1.12, 240);
        }
    } else if (frame.type == ActionType::GC_DONE) {
        m_aBox->setCustomValue(QString::number(frame.u));
        m_aBox->color = kGcR;
        m_bBox->setCustomValue("0");
        m_bBox->color = kGcIdle;
        m_rBox->setCustomValue("—");
        m_rBox->color = kGcIdle;
        m_aValue->setPlainText(QString("gcd = %1").arg(frame.u));
        m_bValue->setPlainText(QString("余数为 0"));
        m_rValue->setPlainText(QString("算法结束"));
        refreshBoxes();
        if (m_animated) {
            playScalePulse(m_aBox, 1.24, 340);
            playCellRipple(m_aBox, kGcR, 1.5, 760);
        }
    }
}

void GcdModule::reset() {
    m_frames.clear();
    m_mathText = nullptr;
    m_aLabel = nullptr;
    m_bLabel = nullptr;
    m_rLabel = nullptr;
    m_aValue = nullptr;
    m_bValue = nullptr;
    m_rValue = nullptr;
    m_aBox = nullptr;
    m_bBox = nullptr;
    m_rBox = nullptr;
    m_a = 0;
    m_b = 0;
    m_valid = false;
}

void GcdModule::releaseSceneItems() {
    m_mathText = nullptr;
    m_aLabel = nullptr;
    m_bLabel = nullptr;
    m_rLabel = nullptr;
    m_aValue = nullptr;
    m_bValue = nullptr;
    m_rValue = nullptr;
    m_aBox = nullptr;
    m_bBox = nullptr;
    m_rBox = nullptr;
}
