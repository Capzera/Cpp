#include "ExtremeWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QRandomGenerator>
#include <QSettings>
#include <QMessageBox>
#include <QtMath>
#include <QPropertyAnimation>
#include <QVariantList>
#include <QShowEvent>
#include <QApplication>
#include <QKeyEvent>
#include <QRegularExpressionValidator>

// ================= 1. 长方形 3D 拟真能量管实现 =================
VerticalHealthBar::VerticalHealthBar(QWidget *parent) : QWidget(parent), m_timeMs(0) {
    setMinimumWidth(80);
}

void VerticalHealthBar::setTimeMs(int timeMs) {
    m_timeMs = qMax(0, timeMs);
    update();
}

void VerticalHealthBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect(10, 10, width() - 20, height() - 20);
    painter.setPen(QPen(QColor(30, 30, 30), 4));
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect);

    QList<QColor> colors = {
        QColor(80, 80, 80), QColor(229, 57, 53), QColor(251, 140, 0),
        QColor(67, 160, 71), QColor(30, 136, 229), QColor(142, 36, 170), QColor(255, 215, 0)
    };

    int level = m_timeMs / 60000;
    float progress = (m_timeMs % 60000) / 60000.0f;
    QColor baseColor = colors[qMin(level, colors.size() - 1)];
    QColor topColor = colors[qMin(level + 1, colors.size() - 1)];

    if (level > 0) {
        QLinearGradient baseLiquid(rect.topLeft(), rect.topRight());
        baseLiquid.setColorAt(0.0, baseColor.darker(150));
        baseLiquid.setColorAt(0.5, baseColor.lighter(120));
        baseLiquid.setColorAt(1.0, baseColor.darker(200));
        painter.setBrush(baseLiquid);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);
    }

    QRectF liquidRect = rect;
    liquidRect.setTop(rect.bottom() - rect.height() * progress);

    QLinearGradient topLiquid(liquidRect.topLeft(), liquidRect.topRight());
    topLiquid.setColorAt(0.0, topColor.darker(150));
    topLiquid.setColorAt(0.5, topColor.lighter(120));
    topLiquid.setColorAt(1.0, topColor.darker(200));

    painter.setBrush(topLiquid);
    painter.drawRect(liquidRect);

    QLinearGradient highlight(rect.topLeft(), rect.topRight());
    highlight.setColorAt(0.0, QColor(255, 255, 255, 0));
    highlight.setColorAt(0.15, QColor(255, 255, 255, 90));
    highlight.setColorAt(0.3, QColor(255, 255, 255, 0));
    highlight.setColorAt(0.85, QColor(255, 255, 255, 0));
    highlight.setColorAt(0.95, QColor(255, 255, 255, 40));
    highlight.setColorAt(1.0, QColor(255, 255, 255, 0));

    painter.setBrush(highlight);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);

    painter.setPen(QPen(QColor(255, 255, 255, 120), 2));
    for (int i = 1; i < 4; ++i) {
        int y = rect.top() + rect.height() * (i / 4.0);
        painter.drawLine(rect.left(), y, rect.left() + 10, y);
        painter.drawLine(rect.right() - 10, y, rect.right(), y);
    }
}

// ================= 2. 霸体大字号并发题卡实现 =================
CleanQuestionWidget::CleanQuestionWidget(int a, int b, int rewardSec, int lifespanSec, QWidget *parent)
    : QWidget(parent), numA(a), numB(b), m_rewardSec(rewardSec), m_lifespanMs(lifespanSec * 1000), m_elapsedMs(0), m_isFrozen(false), m_isPaused(false) {

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setAlignment(Qt::AlignCenter);
    hLayout->setSpacing(30);

    equationLabel = new QLabel(QString("%1 × %2 =").arg(a).arg(b), this);
    equationLabel->setAlignment(Qt::AlignCenter);
    equationLabel->setStyleSheet("QLabel { font-size: 48pt; font-family: 'Arial'; font-weight: bold; color: black; background: transparent; border: none; }");

    answerEdit = new QLineEdit(this);
    answerEdit->setAlignment(Qt::AlignCenter);
    answerEdit->setFixedSize(250, 70);
    // 增加正则验证器，防止敲击后门秘籍字母时被录入到框里
    answerEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]+$"), this));
    answerEdit->setStyleSheet("QLineEdit { font-size: 36pt; font-family: 'Arial'; font-weight: bold; background-color: white; border: 3px solid #000; border-radius: 5px; color: black; }");
    connect(answerEdit, &QLineEdit::returnPressed, this, &CleanQuestionWidget::verifyAnswer);

    hLayout->addWidget(equationLabel);
    hLayout->addWidget(answerEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(hLayout);

    if (m_lifespanMs > 0) {
        progBar = new QProgressBar(this);
        progBar->setTextVisible(false);
        progBar->setFixedHeight(4);
        progBar->setStyleSheet("QProgressBar { border: none; background-color: transparent; }"
                               "QProgressBar::chunk { background-color: #ff1744; border-radius: 2px; }");

        mainLayout->addWidget(progBar);
        progBar->setRange(0, m_lifespanMs);
        progBar->setValue(m_lifespanMs);
    }

    lifeTimer = new QTimer(this);
    connect(lifeTimer, &QTimer::timeout, this, &CleanQuestionWidget::tick);
}

void CleanQuestionWidget::start() {
    lifeTimer->start(50);
    answerEdit->setFocus();
}

void CleanQuestionWidget::pause() { m_isPaused = true; }
void CleanQuestionWidget::resume() { m_isPaused = false; }

void CleanQuestionWidget::tick() {
    if (m_isFrozen || m_isPaused) return;

    m_elapsedMs += 50;
    if (m_lifespanMs > 0) {
        progBar->setValue(m_lifespanMs - m_elapsedMs);
        if (m_elapsedMs >= m_lifespanMs) {
            lifeTimer->stop();
            emit expired(this);
        }
    }
}

void CleanQuestionWidget::verifyAnswer() {
    if (m_isFrozen || m_isPaused) return;

    QString text = answerEdit->text().trimmed();
    if (text.isEmpty()) return;

    if (text.toLongLong() == (long long)numA * numB) {
        lifeTimer->stop();
        answerEdit->setStyleSheet("QLineEdit { font-size: 36pt; font-family: 'Arial'; font-weight: bold; background-color: #c8e6c9; border: 3px solid #388e3c; color: #1b5e20; border-radius: 5px; }");
        answerEdit->setEnabled(false);
        emit answeredCorrectly(m_rewardSec, this);
    } else {
        m_isFrozen = true;
        answerEdit->setStyleSheet("QLineEdit { font-size: 32pt; font-family: 'Arial'; font-weight: bold; background-color: #bbdefb; border: 3px solid #1976d2; color: #1976d2; border-radius: 5px; }");
        answerEdit->setText("❄️冰冻...");
        answerEdit->setEnabled(false);
        QTimer::singleShot(5000, this, &CleanQuestionWidget::unfreeze);
    }
}

void CleanQuestionWidget::unfreeze() {
    if (m_isPaused) {
        QTimer::singleShot(100, this, &CleanQuestionWidget::unfreeze);
        return;
    }
    m_isFrozen = false;
    answerEdit->setEnabled(true);
    answerEdit->clear();
    answerEdit->setStyleSheet("QLineEdit { font-size: 36pt; font-family: 'Arial'; font-weight: bold; background-color: white; border: 3px solid #000; border-radius: 5px; color: black; }");
    answerEdit->setFocus();
}

// ================= 3. 极限模式主控实现 =================
ExtremeWidget::ExtremeWidget(QWidget *parent)
    : QWidget(parent), timeLeftMs(60000), currentScore(0), highScore(0), m_currentPhase(1), isGameActive(false) {

    for (int i = 0; i < 3; ++i) m_qSlots[i] = nullptr;

    mainLoopTimer = new QTimer(this);
    connect(mainLoopTimer, &QTimer::timeout, this, &ExtremeWidget::gameTick);

    setupUI();
    loadHighScore();

    qApp->installEventFilter(this);
}

ExtremeWidget::~ExtremeWidget() {
    mainLoopTimer->stop();
    qApp->removeEventFilter(this);
}

// [开发者模式] 核心防抖拦截器
bool ExtremeWidget::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        // 【核心修复】：放弃内存地址防抖，改用系统级物理时间戳防抖！
        // 完美解决由于 QLineEdit 拒收字母导致的事件冒泡多次触发问题
        static quint32 lastTimestamp = 0;

        if (!keyEvent->isAutoRepeat() && keyEvent->timestamp() != lastTimestamp) {
            lastTimestamp = keyEvent->timestamp();
            int key = keyEvent->key();

            // 严格过滤，只有 A-Z 才会进缓冲区
            if (key >= Qt::Key_A && key <= Qt::Key_Z) {
                m_cheatBuffer += QChar(key).toLower();

                if (m_cheatBuffer.length() > 10) {
                    m_cheatBuffer = m_cheatBuffer.right(10);
                }

                // 成功触发秘籍
                if (m_cheatBuffer.endsWith("super")) {
                    m_cheatBuffer.clear();
                    if (isGameActive) {
                        timeLeftMs += 60000; // 瞬间猛涨 1 分钟
                        updateStatsUI();

                        // 动画和普通 +10s 表现完全一致，从能量管上方低调冒出
                        QPoint pos = healthBar->mapTo(this, QPoint(healthBar->width() / 2, 0));
                        showTimeBonusAnimation(pos, 60);
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ExtremeWidget::setupUI() {
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setContentsMargins(10, 10, 10, 0);

    backButton = new QPushButton("⬅ 返回菜单");
    backButton->setFixedSize(200, 50);
    connect(backButton, &QPushButton::clicked, this, &ExtremeWidget::onBackButtonClicked);

    levelComboBox = new QComboBox();
    levelComboBox->addItems({"2位 × 2位", "3位 × 2位", "3位 × 3位", "4位 × 2位", "4位 × 3位", "4位 × 4位"});
    connect(levelComboBox, &QComboBox::currentIndexChanged, this, &ExtremeWidget::loadHighScore);

    startButton = new QPushButton("▶ 极速开局");
    startButton->setFixedSize(160, 50);
    connect(startButton, &QPushButton::clicked, this, &ExtremeWidget::startGame);

    controlLayout->addWidget(backButton);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(new QLabel("题型:", this));
    controlLayout->addWidget(levelComboBox);
    controlLayout->addStretch();
    controlLayout->addWidget(startButton);

    QVBoxLayout *leftLayout = new QVBoxLayout();

    QHBoxLayout *statsLayout = new QHBoxLayout();
    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("color: #1976d2; font-weight: bold;");

    highScoreLabel = new QLabel();
    highScoreLabel->setStyleSheet("color: #388e3c; font-weight: bold;");

    statsLayout->addWidget(scoreLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(highScoreLabel);

    leftLayout->addLayout(statsLayout);
    leftLayout->addSpacing(20);

    questionsContainer = new QWidget(this);
    questionsLayout = new QVBoxLayout(questionsContainer);
    questionsLayout->setAlignment(Qt::AlignCenter);
    questionsLayout->setSpacing(40);
    leftLayout->addWidget(questionsContainer, 1);

    healthBar = new VerticalHealthBar(this);

    QHBoxLayout *battleLayout = new QHBoxLayout();
    battleLayout->setContentsMargins(20, 20, 20, 30);
    battleLayout->addLayout(leftLayout, 1);
    battleLayout->addSpacing(20);
    battleLayout->addWidget(healthBar);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(controlLayout);
    mainLayout->addSpacing(10);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    mainLayout->addLayout(battleLayout, 1);
}

void ExtremeWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (!isGameActive && hasGameSave()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "发现存档",
                                                                  "发现上次未完成的极限模式对局，是否继续？\n\n选择 [Yes] 恢复生命和分数\n选择 [No] 清除存档重新开始",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            loadGameProgress();
        } else {
            clearGameProgress();
        }
    }
}

void ExtremeWidget::onBackButtonClicked() {
    if (!isGameActive) {
        emit backToMenuRequested();
        return;
    }

    mainLoopTimer->stop();
    for (int i = 0; i < 3; ++i) {
        if (m_qSlots[i]) m_qSlots[i]->pause();
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "保存进度",
                                                              "游戏正在高能进行中！是否保存当前进度？\n\n选择 [Yes] 保存进度并返回主菜单\n选择 [No] 放弃进度并返回主菜单\n选择 [Cancel] 继续死战",
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        saveGameProgress();
        isGameActive = false;

        for (int i = 0; i < 3; ++i) {
            if (m_qSlots[i]) {
                m_qSlots[i]->deleteLater();
                m_qSlots[i] = nullptr;
            }
        }
        emit backToMenuRequested();
    }
    else if (reply == QMessageBox::No) {
        clearGameProgress();
        isGameActive = false;

        for (int i = 0; i < 3; ++i) {
            if (m_qSlots[i]) {
                m_qSlots[i]->deleteLater();
                m_qSlots[i] = nullptr;
            }
        }
        emit backToMenuRequested();
    }
    else {
        mainLoopTimer->start(50);
        for (int i = 0; i < 3; ++i) {
            if (m_qSlots[i]) m_qSlots[i]->resume();
        }
    }
}

bool ExtremeWidget::hasGameSave() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    return settings.value("Extreme_Save_Active", false).toBool();
}

void ExtremeWidget::saveGameProgress() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    settings.setValue("Extreme_Save_Active", true);
    settings.setValue("Extreme_Save_Score", currentScore);
    settings.setValue("Extreme_Save_Time", timeLeftMs);
    settings.setValue("Extreme_Save_Phase", m_currentPhase);
    settings.setValue("Extreme_Save_Level", levelComboBox->currentIndex());
}

void ExtremeWidget::loadGameProgress() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    currentScore = settings.value("Extreme_Save_Score", 0).toInt();
    timeLeftMs = settings.value("Extreme_Save_Time", 60000).toInt();
    m_currentPhase = settings.value("Extreme_Save_Phase", 1).toInt();
    levelComboBox->setCurrentIndex(settings.value("Extreme_Save_Level", 0).toInt());

    clearGameProgress();

    isGameActive = true;
    levelComboBox->setEnabled(false);
    startButton->setText("🔄 重新开始");

    updateStatsUI();
    mainLoopTimer->start(50);
}

void ExtremeWidget::clearGameProgress() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    settings.setValue("Extreme_Save_Active", false);
}

void ExtremeWidget::loadHighScore() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    QString key = QString("Extreme_HighScore_Level%1").arg(levelComboBox->currentIndex());
    highScore = settings.value(key, 0).toInt();
    updateStatsUI();
}

void ExtremeWidget::saveHighScore() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    QString key = QString("Extreme_HighScore_Level%1").arg(levelComboBox->currentIndex());
    settings.setValue(key, highScore);
}

void ExtremeWidget::updateStatsUI() {
    scoreLabel->setText(QString("当前得分: %1").arg(currentScore));
    highScoreLabel->setText(QString("当前最高记录: %1").arg(highScore));
    healthBar->setTimeMs(timeLeftMs);
}

void ExtremeWidget::startGame() {
    for (int i = 0; i < 3; ++i) {
        if (m_qSlots[i]) {
            m_qSlots[i]->deleteLater();
            m_qSlots[i] = nullptr;
        }
    }
    clearGameProgress();

    timeLeftMs = 60000;
    currentScore = 0;
    m_currentPhase = 1;
    isGameActive = true;

    loadHighScore();
    updateStatsUI();

    levelComboBox->setEnabled(false);
    startButton->setText("🔄 重新开始");

    mainLoopTimer->start(50);
}

void ExtremeWidget::gameTick() {
    if (!isGameActive) return;

    timeLeftMs -= 50;
    healthBar->setTimeMs(timeLeftMs);

    if (timeLeftMs <= 0) {
        gameOver();
        return;
    }

    int calculatedPhase = 1;
    if (timeLeftMs > 180000) {
        calculatedPhase = 3;
    } else if (timeLeftMs > 90000 || currentScore >= 20) {
        calculatedPhase = 2;
    }

    if (calculatedPhase > m_currentPhase) {
        m_currentPhase = calculatedPhase;
    }

    int targetNodes = 1, rewardSec = 10, lifespanSec = 0;
    if (m_currentPhase == 3) {
        targetNodes = 3; rewardSec = 5; lifespanSec = 20;
    } else if (m_currentPhase == 2) {
        targetNodes = 2; rewardSec = 7; lifespanSec = 15;
    } else {
        targetNodes = 1; rewardSec = 10; lifespanSec = 0;
    }

    for (int i = 0; i < targetNodes; ++i) {
        if (m_qSlots[i] == nullptr) {
            spawnQuestionAtSlot(i, rewardSec, lifespanSec);
        }
    }
}

int ExtremeWidget::generateValidNumber(int digits) {
    int min = qPow(10, digits - 1);
    int max = qPow(10, digits) - 1;
    int num;
    do { num = QRandomGenerator::global()->bounded(min, max + 1); } while (num % 10 == 0);
    return num;
}

void ExtremeWidget::spawnQuestionAtSlot(int index, int rewardSec, int lifespanSec) {
    int dA = levelComboBox->currentText().mid(0, 1).toInt();
    int dB = levelComboBox->currentText().mid(5, 1).toInt();

    CleanQuestionWidget *q = new CleanQuestionWidget(generateValidNumber(dA), generateValidNumber(dB), rewardSec, lifespanSec, this);

    questionsLayout->insertWidget(index, q);
    m_qSlots[index] = q;

    connect(q, &CleanQuestionWidget::answeredCorrectly, this, &ExtremeWidget::handleQuestionCorrect);
    connect(q, &CleanQuestionWidget::expired, this, &ExtremeWidget::handleQuestionExpired);

    q->start();
}

void ExtremeWidget::handleQuestionCorrect(int rewardSec, CleanQuestionWidget *q) {
    currentScore++;
    timeLeftMs += rewardSec * 1000;
    if (currentScore > highScore) {
        highScore = currentScore;
        saveHighScore();
    }

    showTimeBonusAnimation(q->mapTo(this, QPoint(q->width()/2, 0)), rewardSec);
    updateStatsUI();

    for (int i = 0; i < 3; ++i) {
        if (m_qSlots[i] == q) {
            m_qSlots[i] = nullptr;
            break;
        }
    }
    q->deleteLater();
}

void ExtremeWidget::handleQuestionExpired(CleanQuestionWidget *q) {
    for (int i = 0; i < 3; ++i) {
        if (m_qSlots[i] == q) {
            m_qSlots[i] = nullptr;
            break;
        }
    }
    q->deleteLater();
}

void ExtremeWidget::showTimeBonusAnimation(const QPoint &pos, int reward) {
    QLabel *bonusLabel = new QLabel(QString("+%1s").arg(reward), this);
    // 与 +10s 动画保持一模一样，不再夸大
    bonusLabel->setFont(QFont("Arial", 36, QFont::Bold));
    bonusLabel->setStyleSheet("color: #00e676; background: transparent; font-weight: 900;");
    bonusLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    bonusLabel->move(pos - QPoint(40, 20));
    bonusLabel->show();

    QPropertyAnimation *anim = new QPropertyAnimation(bonusLabel, "pos");
    anim->setDuration(1200);
    anim->setStartValue(bonusLabel->pos());
    anim->setEndValue(bonusLabel->pos() - QPoint(0, 100));
    anim->setEasingCurve(QEasingCurve::OutCubic);

    connect(anim, &QPropertyAnimation::finished, bonusLabel, &QLabel::deleteLater);
    connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
    anim->start();
}

void ExtremeWidget::gameOver() {
    isGameActive = false;
    mainLoopTimer->stop();
    clearGameProgress();

    for (int i = 0; i < 3; ++i) {
        if (m_qSlots[i]) m_qSlots[i]->answerEdit->setEnabled(false);
    }

    levelComboBox->setEnabled(true);
    startButton->setText("▶ 重新挑战");

    QSettings settings("TrachtenbergApp", "SpeedMath");
    QVariantList recentList = settings.value("Extreme_Recent5").toList();
    recentList.append(currentScore);
    if (recentList.size() > 5) recentList.removeFirst();
    settings.setValue("Extreme_Recent5", recentList);

    QMessageBox::information(this, "时间耗尽",
                             QString("你的 CPU 已烧毁！\n\n最终得分：%1 题\n极限最高：%2 题")
                                 .arg(currentScore).arg(highScore));
}