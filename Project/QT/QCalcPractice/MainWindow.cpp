#include "MainWindow.h"
#include "TrainerWidget.h"
#include "ChallengeWidget.h"
#include "ExtremeWidget.h"
#include "LeaderboardWidget.h"
#include "SettingsWidget.h"
#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QSettings>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("速算练习器");
    this->resize(1000, 750);

    stackedWidget = new QStackedWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);

    setupMainMenu();
    setupChallengeMenu();

    // 软件启动时检查是否需要全屏
    QSettings settings("TrachtenbergApp", "SpeedMath");
    if (settings.value("Settings_Fullscreen", false).toBool()) {
        this->showFullScreen();
    }
}

// ================= 解决全屏缩放的终极绝招 =================
void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // 基于设计基准高度 750 计算缩放系数
    double scale = this->height() / 750.0;

    // 动态计算字体 pt 大小，基础字号全部调大！
    int basePt = qMax(14, int(16 * scale));
    int titlePt = qMax(40, int(52 * scale));
    int btnPt = qMax(16, int(20 * scale)); // 专门给按钮放大的字号

    // 动态注入 QSS，全局统一配色、字体和质感
    QString style = QString(
                        "QLabel { font-size: %1pt; color: #2c3e50; font-family: 'Microsoft YaHei'; }"
                        "QLabel#TitleLabel { font-size: %2pt; font-weight: bold; color: #1a252f; }"
                        "QWidget#Background { background-color: #f4f7f6; }"

                        /* 大厂级别的克制质感按钮：微渐变 + 细腻边框 + 微软雅黑 */
                        "QPushButton {"
                        "  font-family: 'Microsoft YaHei';"
                        "  font-size: %3pt;"
                        "  font-weight: bold;"
                        "  color: #333333;"
                        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #f0f0f0);"
                        "  border: 1px solid #dcdcdc;"
                        "  border-radius: 8px;"
                        "}"
                        "QPushButton:hover {"
                        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f8f9fa, stop:1 #e9ecef);"
                        "  border: 1px solid #c0c0c0;"
                        "}"
                        "QPushButton:pressed {"
                        "  background: #e0e4e8;"
                        "  padding-top: 2px;" /* 点击时文字轻微下沉，模拟物理按压感 */
                        "}"
                        ).arg(basePt).arg(titlePt).arg(btnPt);

    this->setStyleSheet(style);
}

void MainWindow::setupMainMenu() {
    menuWidget = new QWidget();
    menuWidget->setObjectName("Background");

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(25);

    QLabel *titleLabel = new QLabel("速算练习器", menuWidget);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(40);

    // 换回原生的 QPushButton，依靠上方的 QSS 统一提供颜值和字号
    QStringList btnNames = {
        "无尽模式", "挑战模式", "极限模式",
        "排行榜", "练习设置", "退出练习"
    };
    QVector<QPushButton*> btns;

    for (const QString &name : btnNames) {
        QPushButton *btn = new QPushButton(name, menuWidget);
        btn->setFixedSize(320, 65);
        btn->setCursor(Qt::PointingHandCursor);
        btns.append(btn);
        layout->addWidget(btn, 0, Qt::AlignHCenter);
    }

    connect(btns[0], &QPushButton::clicked, this, &MainWindow::startEndlessMode);
    connect(btns[1], &QPushButton::clicked, this, &MainWindow::showChallengeMenu); // 跳入二级菜单
    connect(btns[2], &QPushButton::clicked, this, &MainWindow::startExtremeMode);
    connect(btns[3], &QPushButton::clicked, this, &MainWindow::showLeaderboard);
    connect(btns[4], &QPushButton::clicked, this, &MainWindow::showSettings);
    connect(btns[5], &QPushButton::clicked, qApp, &QApplication::quit);

    stackedWidget->addWidget(menuWidget);
}

void MainWindow::setupChallengeMenu() {
    challengeMenuWidget = new QWidget();
    challengeMenuWidget->setObjectName("Background");

    QVBoxLayout *layout = new QVBoxLayout(challengeMenuWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(25);

    QLabel *titleLabel = new QLabel("选择挑战难度", challengeMenuWidget);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(40);

    QStringList btnNames = {"简单 (5条命)", "中等 (3条命)", "困难 (1条命)", "返回主界面"};
    QVector<QPushButton*> btns;

    for (const QString &name : btnNames) {
        QPushButton *btn = new QPushButton(name, challengeMenuWidget);
        btn->setFixedSize(320, 65);
        btn->setCursor(Qt::PointingHandCursor);
        btns.append(btn);
        layout->addWidget(btn, 0, Qt::AlignHCenter);
    }

    // 传入对应的难度参数
    connect(btns[0], &QPushButton::clicked, this, [this]() { startChallengeMode(0); });
    connect(btns[1], &QPushButton::clicked, this, [this]() { startChallengeMode(1); });
    connect(btns[2], &QPushButton::clicked, this, [this]() { startChallengeMode(2); });
    connect(btns[3], &QPushButton::clicked, this, &MainWindow::showMainMenu);

    stackedWidget->addWidget(challengeMenuWidget);
}

void MainWindow::showMainMenu() {
    stackedWidget->setCurrentWidget(menuWidget);
}

void MainWindow::showChallengeMenu() {
    stackedWidget->setCurrentWidget(challengeMenuWidget);
}

// ================= 路由控制 =================
void MainWindow::startEndlessMode() {
    static int endlessIndex = -1;
    if (endlessIndex == -1) {
        TrainerWidget *endlessWidget = new TrainerWidget(this);
        endlessIndex = stackedWidget->addWidget(endlessWidget);
        connect(endlessWidget, &TrainerWidget::backToMenuRequested, this, &MainWindow::showMainMenu);
    }
    stackedWidget->setCurrentIndex(endlessIndex);
}

void MainWindow::startChallengeMode(int difficulty) {
    static int challengeIndex = -1;
    if (challengeIndex == -1) {
        ChallengeWidget *challengeWidget = new ChallengeWidget(this);
        challengeIndex = stackedWidget->addWidget(challengeWidget);
        connect(challengeWidget, &ChallengeWidget::backToMenuRequested, this, &MainWindow::showMainMenu);
    }
    auto *w = qobject_cast<ChallengeWidget*>(stackedWidget->widget(challengeIndex));
    if (w) w->setDifficulty(difficulty);
    stackedWidget->setCurrentIndex(challengeIndex);
}

void MainWindow::startExtremeMode() {
    static int extremeIndex = -1;
    if (extremeIndex == -1) {
        ExtremeWidget *extremeWidget = new ExtremeWidget(this);
        extremeIndex = stackedWidget->addWidget(extremeWidget);
        connect(extremeWidget, &ExtremeWidget::backToMenuRequested, this, &MainWindow::showMainMenu);
    }
    stackedWidget->setCurrentIndex(extremeIndex);
}

void MainWindow::showLeaderboard() {
    static int leaderboardIndex = -1;
    if (leaderboardIndex == -1) {
        LeaderboardWidget *lbWidget = new LeaderboardWidget(this);
        leaderboardIndex = stackedWidget->addWidget(lbWidget);
        connect(lbWidget, &LeaderboardWidget::backToMenuRequested, this, &MainWindow::showMainMenu);
    } else {
        auto *lbWidget = qobject_cast<LeaderboardWidget*>(stackedWidget->widget(leaderboardIndex));
        if (lbWidget) lbWidget->refreshData();
    }
    stackedWidget->setCurrentIndex(leaderboardIndex);
}

void MainWindow::showSettings() {
    static int settingsIndex = -1;
    if (settingsIndex == -1) {
        SettingsWidget *settingsWidget = new SettingsWidget(this);
        settingsIndex = stackedWidget->addWidget(settingsWidget);
        connect(settingsWidget, &SettingsWidget::backToMenuRequested, this, &MainWindow::showMainMenu);

        connect(settingsWidget, &SettingsWidget::fullscreenToggled, this, [this](bool isFull) {
            if (isFull) this->showFullScreen();
            else this->showNormal();
        });
    }
    stackedWidget->setCurrentIndex(settingsIndex);
}