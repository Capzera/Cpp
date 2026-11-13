#include "TrainerWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QRandomGenerator>
#include <QRegularExpressionValidator>
#include <QtMath>
#include <QFontMetrics>

// ================= MathCanvas 实现 =================
MathCanvas::MathCanvas(QWidget *parent) : QWidget(parent), isPracticeMode(false), isWrong(false), isCorrect(false) {
    answerEdit = new QLineEdit(this);
    answerEdit->setFont(QFont("Arial", 40, QFont::Bold));
    answerEdit->setAlignment(Qt::AlignCenter);
    answerEdit->setPlaceholderText("?");
    answerEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]+$"), this));
    answerEdit->setStyleSheet("background-color: white; border: 3px solid #000; border-radius: 5px; color: black;");

    statusLabel = new QLabel(this);
    statusLabel->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setText("");
}

void MathCanvas::setProblem(int a, int b) {
    strA = QString::number(a);
    strB = QString::number(b);
    isWrong = false;
    isCorrect = false;
    answerEdit->clear();
    answerEdit->setStyleSheet("background-color: white; border: 3px solid #000; border-radius: 5px; color: black;");
    statusLabel->setText("");
    updateLayout();
    update();
}

void MathCanvas::setPracticeMode(bool on) {
    isPracticeMode = on;
    update();
}

void MathCanvas::setWrongState(bool wrong) {
    isWrong = wrong;
    isCorrect = !wrong;
    if (isWrong) {
        statusLabel->setText("❌ 答案错误，请看下方复盘槽位");
        statusLabel->setStyleSheet("color: #d32f2f;");
        answerEdit->setStyleSheet("background-color: #ffcdd2; color: #b71c1c; border: 3px solid #d32f2f; border-radius: 5px;");
    }
    update();
}

void MathCanvas::setCorrectState(bool correct) {
    isCorrect = correct;
    isWrong = !correct;
    if (isCorrect) {
        statusLabel->setText("✅ 答案正确！");
        statusLabel->setStyleSheet("color: #2e7d32;");
        answerEdit->setStyleSheet("background-color: #c8e6c9; color: #1b5e20; border: 3px solid #2e7d32; border-radius: 5px;");
    }
    update();
}

void MathCanvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateLayout();
}

// 核心布局算法
void MathCanvas::updateLayout() {
    if (strA.isEmpty() || strB.isEmpty()) return;

    int N = strA.length();
    int M = strB.length();

    int digitW = 55;
    int symW = 70;
    int ansW = 250;
    int heightTop = 80;

    int totalW = N * digitW + symW + M * digitW + symW + ansW;
    int startX = (width() - totalW) / 2;
    int yEq = 100;

    aRects.clear();
    bRects.clear();
    boxRects.clear();

    // 1. 排布上方题目
    int curX = startX;
    for (int i = 0; i < N; ++i) {
        aRects.append(QRect(curX, yEq, digitW, heightTop));
        curX += digitW;
    }

    symMulRect = QRect(curX, yEq, symW, heightTop);
    curX += symW;

    for (int j = 0; j < M; ++j) {
        bRects.append(QRect(curX, yEq, digitW, heightTop));
        curX += digitW;
    }

    symEqRect = QRect(curX, yEq, symW, heightTop);
    curX += symW;

    answerEdit->setGeometry(curX, yEq + 5, ansW, heightTop - 10);
    statusLabel->setGeometry(0, yEq - 60, width(), 40);

    // 2. 排布下方复盘槽位
    int K = N + M - 1;
    int boxW = qMax(80, 110 - K * 5);
    int boxGap = 20;
    int boxH = 65;

    // 关键修改：抛弃居中，让第一个槽位的中心和第一个乘数的首位数字中心对齐
    int startBoxX = aRects[0].center().x() - boxW / 2;

    int yBox = yEq + 220;

    for (int k = 0; k < K; ++k) {
        boxRects.append(QRect(startBoxX + k * (boxW + boxGap), yBox, boxW, boxH));
    }
}

void MathCanvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (strA.isEmpty() || strB.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int N = strA.length();
    int M = strB.length();
    int K = N + M - 1;

    // --- 1. 绘制连线 ---
    if (isPracticeMode || isWrong) {
        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                int k = i + j;

                QPoint pA(aRects[i].center().x(), aRects[i].bottom() - 10);
                QPoint pB(bRects[j].center().x(), bRects[j].bottom() - 10);
                QPoint pBox(boxRects[k].center().x(), boxRects[k].top());

                painter.drawLine(pA, pBox);
                painter.drawLine(pB, pBox);
            }
        }

        // --- 2. 绘制槽位 ---
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        for (int k = 0; k < K; ++k) {
            int sum = 0;
            for (int i = 0; i < N; ++i) {
                int j = k - i;
                if (j >= 0 && j < M) {
                    sum += (strA[i].unicode() - '0') * (strB[j].unicode() - '0');
                }
            }

            if (isWrong) {
                painter.setBrush(QColor(255, 235, 238));
                painter.setPen(QPen(QColor(198, 40, 40), 3));
            } else {
                painter.setBrush(Qt::white);
                painter.setPen(QPen(Qt::black, 3));
            }

            painter.drawRect(boxRects[k]);

            painter.setPen(Qt::black);
            painter.drawText(boxRects[k], Qt::AlignCenter, QString::number(sum));
        }
    }

    // --- 3. 绘制上方题目 ---
    painter.setFont(QFont("Arial", 56, QFont::Bold));
    painter.setPen(Qt::black);

    for (int i = 0; i < N; ++i) {
        painter.drawText(aRects[i], Qt::AlignCenter, QString(strA[i]));
    }
    painter.drawText(symMulRect, Qt::AlignCenter, "×");

    for (int j = 0; j < M; ++j) {
        painter.drawText(bRects[j], Qt::AlignCenter, QString(strB[j]));
    }
    painter.drawText(symEqRect, Qt::AlignCenter, "=");
}

// ================= TrainerWidget 实现 =================
TrainerWidget::TrainerWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    generateNextQuestion();
}

void TrainerWidget::setupUI() {
    QFont normalFont("Microsoft YaHei", 12);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(10, 10, 10, 10);

    backButton = new QPushButton("⬅ 返回菜单");
    backButton->setFont(normalFont);
    backButton->setFixedSize(200, 50);
    connect(backButton, &QPushButton::clicked, this, &TrainerWidget::backToMenuRequested);

    practiceCheckBox = new QCheckBox("开启练习模式 (显示引导线与槽位)");
    practiceCheckBox->setFont(normalFont);
    connect(practiceCheckBox, &QCheckBox::stateChanged, this, &TrainerWidget::togglePracticeMode);

    levelComboBox = new QComboBox();
    levelComboBox->setFont(normalFont);
    levelComboBox->addItems({"2位 × 2位", "3位 × 2位", "3位 × 3位", "4位 × 2位", "4位 × 3位", "4位 × 4位"});
    connect(levelComboBox, &QComboBox::currentIndexChanged, this, &TrainerWidget::generateNextQuestion);

    checkButton = new QPushButton("提交答案 (Enter)");
    checkButton->setFont(normalFont);
    checkButton->setMinimumHeight(40);
    checkButton->setStyleSheet("background-color: #1976d2; color: white; font-weight: bold; border-radius: 5px;");
    connect(checkButton, &QPushButton::clicked, this, &TrainerWidget::verifyAnswer);

    nextButton = new QPushButton("下一题 (Alt+N)");
    nextButton->setFont(normalFont);
    nextButton->setMinimumHeight(40);
    nextButton->setShortcut(QKeySequence("Alt+N"));
    connect(nextButton, &QPushButton::clicked, this, &TrainerWidget::generateNextQuestion);

    topLayout->addWidget(backButton);
    topLayout->addSpacing(20);
    topLayout->addWidget(new QLabel("难度:", this));
    topLayout->addWidget(levelComboBox);
    topLayout->addSpacing(20);
    topLayout->addWidget(practiceCheckBox);
    topLayout->addStretch();
    topLayout->addWidget(checkButton);
    topLayout->addWidget(nextButton);

    canvas = new MathCanvas(this);
    connect(canvas->answerEdit, &QLineEdit::returnPressed, this, &TrainerWidget::verifyAnswer);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(canvas, 1);
}

void TrainerWidget::togglePracticeMode(int state) {
    canvas->setPracticeMode(state == Qt::Checked);
}

int TrainerWidget::generateValidNumber(int digits) {
    int min = qPow(10, digits - 1);
    int max = qPow(10, digits) - 1;
    int num = 0;
    do {
        num = QRandomGenerator::global()->bounded(min, max + 1);
    } while (num % 10 == 0);
    return num;
}

void TrainerWidget::generateNextQuestion() {
    QString mode = levelComboBox->currentText();
    int digitsA = mode.mid(0, 1).toInt();
    int digitsB = mode.mid(5, 1).toInt();

    currentNumA = generateValidNumber(digitsA);
    currentNumB = generateValidNumber(digitsB);

    canvas->setProblem(currentNumA, currentNumB);
    canvas->answerEdit->setReadOnly(false);
    canvas->answerEdit->setFocus();
}

void TrainerWidget::verifyAnswer() {
    QString userInput = canvas->answerEdit->text().trimmed();
    if (userInput.isEmpty()) return;

    long long userAnswer = userInput.toLongLong();
    long long correctAnswer = currentNumA * currentNumB;

    if (userAnswer == correctAnswer) {
        canvas->setCorrectState(true);
        canvas->answerEdit->setReadOnly(true);
        nextButton->setFocus();
    } else {
        canvas->setWrongState(true);
        canvas->answerEdit->setFocus();
        canvas->answerEdit->selectAll();
    }
}