#include "ChallengeWidget.h"
#include "TrainerWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QSettings>
#include <QMessageBox>
#include <QtMath>
#include <QVariantList>

ChallengeWidget::ChallengeWidget(QWidget *parent)
    : QWidget(parent), isGameActive(false), currentScore(0), highScore(0), m_difficulty(0) {

    maxLives = 5;
    currentLives = maxLives;

    setupUI();
    loadHighScore();
    updateStatsUI();
}

// 核心接口：接收来自 MainWindow 二级菜单的难度指令
void ChallengeWidget::setDifficulty(int diff) {
    m_difficulty = diff;
    if (diff == 0) maxLives = 5;
    else if (diff == 1) maxLives = 3;
    else maxLives = 1;

    currentLives = maxLives;
    currentScore = 0;
    isGameActive = false;

    // 切换难度时，刷新对应的最高分并重置界面状态
    loadHighScore();
    updateStatsUI();

    startButton->setText("▶ 开始挑战");
    levelComboBox->setEnabled(true);
    checkButton->setEnabled(false);
    nextButton->setEnabled(false);

    if (canvas) {
        canvas->answerEdit->clear();
        canvas->answerEdit->setReadOnly(true);
        canvas->setProblem(0, 0); // 清空画板上的旧题目
    }
}

void ChallengeWidget::setupUI() {
    // 删除了 inline 的 QSS 颜色，因为现在全局统一接管了高级感渐变按钮
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setContentsMargins(10, 10, 10, 0);

    backButton = new QPushButton("⬅ 返回菜单");
    backButton->setFixedSize(200, 50);
    connect(backButton, &QPushButton::clicked, this, &ChallengeWidget::backToMenuRequested);

    levelComboBox = new QComboBox();
    levelComboBox->addItems({"2位 × 2位", "3位 × 2位", "3位 × 3位", "4位 × 2位", "4位 × 3位", "4位 × 4位"});
    connect(levelComboBox, &QComboBox::currentIndexChanged, this, &ChallengeWidget::loadHighScore);

    startButton = new QPushButton("▶ 开始挑战");
    startButton->setFixedSize(140, 40);
    connect(startButton, &QPushButton::clicked, this, &ChallengeWidget::startGame);

    controlLayout->addWidget(backButton);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(new QLabel("题型:", this));
    controlLayout->addWidget(levelComboBox);
    controlLayout->addStretch();
    controlLayout->addWidget(startButton);

    // --- 第二行：游戏状态栏 ---
    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setContentsMargins(20, 10, 20, 10);

    livesLabel = new QLabel();
    scoreLabel = new QLabel();
    scoreLabel->setStyleSheet("color: #1976d2; font-weight: bold;");

    highScoreLabel = new QLabel();
    highScoreLabel->setStyleSheet("color: #388e3c; font-weight: bold;");

    statsLayout->addWidget(livesLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(scoreLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(highScoreLabel);

    // --- 画板区 ---
    canvas = new MathCanvas(this);
    canvas->setPracticeMode(false);
    connect(canvas->answerEdit, &QLineEdit::returnPressed, this, &ChallengeWidget::verifyAnswer);

    // --- 底部：操作按钮 ---
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 20);

    checkButton = new QPushButton("提交答案 (Enter)");
    checkButton->setFixedSize(220, 50);
    checkButton->setEnabled(false);
    connect(checkButton, &QPushButton::clicked, this, &ChallengeWidget::verifyAnswer);

    nextButton = new QPushButton("下一题 (Alt+N)");
    nextButton->setFixedSize(220, 50);
    nextButton->setShortcut(QKeySequence("Alt+N"));
    nextButton->setEnabled(false);
    connect(nextButton, &QPushButton::clicked, this, &ChallengeWidget::generateNextQuestion);

    bottomLayout->addStretch();
    bottomLayout->addWidget(checkButton);
    bottomLayout->addSpacing(30);
    bottomLayout->addWidget(nextButton);
    bottomLayout->addStretch();

    // --- 主布局整合 ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(statsLayout);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    mainLayout->addWidget(canvas, 1);
    mainLayout->addLayout(bottomLayout);
}

void ChallengeWidget::updateStatsUI() {
    QString hearts = "";
    for (int i = 0; i < maxLives; ++i) {
        if (i < currentLives) hearts += "❤️";
        else hearts += "🤍";
    }
    // 根据当前难度显示对应的前缀
    QString diffStr = (m_difficulty == 0) ? "简单" : ((m_difficulty == 1) ? "中等" : "困难");

    livesLabel->setText(QString("生命值 [%1]: %2").arg(diffStr).arg(hearts));
    scoreLabel->setText(QString("当前连续答对: %1 题").arg(currentScore));
    highScoreLabel->setText(QString("当前最高记录: %1 题").arg(highScore));
}

void ChallengeWidget::loadHighScore() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    QString key = QString("Challenge_HighScore_Diff%1_Level%2")
                      .arg(m_difficulty)
                      .arg(levelComboBox->currentIndex());
    highScore = settings.value(key, 0).toInt();
    updateStatsUI();
}

void ChallengeWidget::saveHighScore() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    QString key = QString("Challenge_HighScore_Diff%1_Level%2")
                      .arg(m_difficulty)
                      .arg(levelComboBox->currentIndex());
    settings.setValue(key, highScore);
}

void ChallengeWidget::startGame() {
    currentLives = maxLives;
    currentScore = 0;
    isGameActive = true;

    loadHighScore();
    updateStatsUI();

    startButton->setText("🔄 重新开始");
    levelComboBox->setEnabled(false); // 游戏中禁止切换题型

    generateNextQuestion();
}

int ChallengeWidget::generateValidNumber(int digits) {
    int min = qPow(10, digits - 1);
    int max = qPow(10, digits) - 1;
    int num = 0;
    do {
        num = QRandomGenerator::global()->bounded(min, max + 1);
    } while (num % 10 == 0);
    return num;
}

void ChallengeWidget::generateNextQuestion() {
    if (!isGameActive) return;

    currentNumA = generateValidNumber(levelComboBox->currentText().mid(0, 1).toInt());
    currentNumB = generateValidNumber(levelComboBox->currentText().mid(5, 1).toInt());

    canvas->setProblem(currentNumA, currentNumB);
    canvas->answerEdit->setReadOnly(false);
    canvas->answerEdit->setFocus();

    checkButton->setEnabled(true);
    nextButton->setEnabled(false);
}

void ChallengeWidget::verifyAnswer() {
    if (!isGameActive) return;
    QString userInput = canvas->answerEdit->text().trimmed();
    if (userInput.isEmpty()) return;

    long long userAnswer = userInput.toLongLong();
    long long correctAnswer = currentNumA * currentNumB;

    canvas->answerEdit->setReadOnly(true);
    checkButton->setEnabled(false);

    if (userAnswer == correctAnswer) {
        currentScore++;
        if (currentScore > highScore) {
            highScore = currentScore;
            saveHighScore();
        }
        updateStatsUI();
        canvas->setCorrectState(true);
        nextButton->setEnabled(true);
        nextButton->setFocus();
    } else {
        currentLives--;
        updateStatsUI();
        canvas->setWrongState(true);

        if (currentLives <= 0) {
            gameOver();
        } else {
            nextButton->setEnabled(true);
            nextButton->setFocus();
        }
    }
}

void ChallengeWidget::gameOver() {
    isGameActive = false;
    nextButton->setEnabled(false);
    checkButton->setEnabled(false);

    levelComboBox->setEnabled(true);
    startButton->setText("▶ 再来一局");

    // ==========================================
    // 填坑：向排行榜推送最近 5 次的记录（全局模式）
    // ==========================================
    QSettings settings("TrachtenbergApp", "SpeedMath");
    QVariantList recentList = settings.value("Challenge_Recent5").toList();

    recentList.append(currentScore);
    if (recentList.size() > 5) {
        recentList.removeFirst(); // 队列满了就挤掉最老的一条记录
    }
    settings.setValue("Challenge_Recent5", recentList);

    QMessageBox::warning(this, "游戏结束",
                         QString("您的生命值已耗尽！\n\n本次连续答对：%1 题\n历史最高记录：%2 题")
                             .arg(currentScore).arg(highScore));
}