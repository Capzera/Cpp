#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QPropertyAnimation>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QTextStream>
#include "CapzeraDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96.0;
    ui->setupUi(this);
    this->setFixedSize(500, 700);
    this->setWindowTitle(QString("信奥答题系统"));
    this->setWindowIcon(QIcon(":/logo.ico"));

    networkManager = new NetworkManager(this);

    connect(networkManager, &NetworkManager::connectedSuccessfully, this, [this]() {
        networkManager->checkUpdate(VersionNumber);
    });
    connect(networkManager, &NetworkManager::updateAvailable, this, &MainWindow::handleUpdateAvailable);
    connect(networkManager, &NetworkManager::loginResult, this, &MainWindow::handleLoginResult);

    connect(networkManager, &NetworkManager::updateDownloadProgress, this, &MainWindow::onUpdateDownloadProgress);
    connect(networkManager, &NetworkManager::updateDownloadFinished, this, &MainWindow::onUpdateDownloadFinished);

    networkManager->connectToServer("49.232.147.9", 8888);

    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    Init();

    QSettings settings("CapzeraEdu", "SaaSOlympic");
    if (settings.value("JustUpdated", false).toBool()) {
        settings.setValue("JustUpdated", false); // 清除标记

        this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        this->raise();
        this->activateWindow();

        QTimer::singleShot(500, this, [this](){
            CapzeraDialog::information(this, "更新成功", QString("当前版本已经更新到 %1").arg(VersionNumber));
        });
    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Init() {
    loginPage = new QWidget();
    loginPage->setStyleSheet("QWidget { background-color: #F8FAFC; }");

    QLabel *titleLabel = new QLabel("信奥答题系统", loginPage);
    titleLabel->setGeometry(0, 90, 500, 60);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #1E293B; font-family: 'Microsoft YaHei'; font-size: 36px; font-weight: bold; letter-spacing: 4px;");

    QLabel *authorLabel = new QLabel("By: Capzera", loginPage);
    authorLabel->setGeometry(0, 145, 500, 30);
    authorLabel->setAlignment(Qt::AlignCenter);
    authorLabel->setStyleSheet("color: #64748B; font-family: 'Microsoft YaHei'; font-size: 16px; font-style: bold;");

    QLabel *versionLabel = new QLabel(VersionNumber, loginPage);
    versionLabel->setGeometry(15, 665, 200, 20);
    versionLabel->setStyleSheet("color: #94A3B8; font-family: 'Consolas'; font-size: 12px; font-weight: bold;");

    QString inputQss = "QLineEdit { font-family: 'Microsoft YaHei'; font-size: 18px; color: #1E293B; border: 2px solid #CBD5E1; border-radius: 8px; padding: 0 15px; background: white; } QLineEdit:focus { border-color: #3B82F6; background: #F1F5F9; }";

    usernameLineEdit = new QLineEdit(loginPage);
    usernameLineEdit->setGeometry(100, 210, 300, 50);
    usernameLineEdit->setStyleSheet(inputQss);
    usernameLineEdit->setPlaceholderText("请输入账号");

    passwordLineEdit = new QLineEdit(loginPage);
    passwordLineEdit->setGeometry(100, 290, 300, 50);
    passwordLineEdit->setStyleSheet(inputQss);
    passwordLineEdit->setPlaceholderText("请输入密码");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    CapzeraButton *loginBtn = new CapzeraButton("登    录", CapzeraButton::Primary, loginPage);
    loginBtn->setGeometry(100, 390, 300, 50);
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::loginTriggle);

    CapzeraButton *exitBtn = new CapzeraButton("退     出", CapzeraButton::Secondary, loginPage);
    exitBtn->setGeometry(100, 460, 300, 50);
    exitBtn->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::close);

    QSettings settings("CapzeraEdu", "SaaSOlympic");
    QString lastUser = settings.value("LastSavedUser", "").toString();
    if (!lastUser.isEmpty()) { usernameLineEdit->setText(lastUser); passwordLineEdit->setFocus(); }
    else { usernameLineEdit->setFocus(); }

    connect(usernameLineEdit, &QLineEdit::returnPressed, this, &MainWindow::loginTriggle);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &MainWindow::loginTriggle);

    teacherPage = new TeacherPage(this, networkManager);
    studentPage = new StudentPage(this, networkManager);

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(studentPage);
    stackedWidget->addWidget(teacherPage);

    auto performLogout = [this]() {
        QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
        anim->setDuration(250); anim->setStartValue(1.0); anim->setEndValue(0.0); anim->setEasingCurve(QEasingCurve::OutCubic);
        connect(anim, &QPropertyAnimation::finished, this, [this]() {
            this->showNormal(); this->setFixedSize(500, 700);
            QRect screenRect = QGuiApplication::primaryScreen()->geometry();
            this->move((screenRect.width() - this->width()) / 2, (screenRect.height() - this->height()) / 2);
            stackedWidget->setCurrentIndex(0);
            passwordLineEdit->clear(); passwordLineEdit->setFocus();
            QPropertyAnimation *fadeIn = new QPropertyAnimation(this, "windowOpacity");
            fadeIn->setDuration(250); fadeIn->setStartValue(0.0); fadeIn->setEndValue(1.0); fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    };

    connect(studentPage, &StudentPage::logoutRequested, this, performLogout);
    connect(teacherPage, &TeacherPage::logoutRequested, this, performLogout);
}

// =========================================================
// 🚀 极简主义 OTA 更新逻辑
// =========================================================

void MainWindow::handleUpdateAvailable(bool isForced, const QString &latestVersion, const QString &releaseNotes) {
    Q_UNUSED(isForced);
    Q_UNUSED(latestVersion);
    Q_UNUSED(releaseNotes);

    // 🚀 极其简单粗暴：双击打开，弹出一个框
    CapzeraDialog::information(this, "发现新版本", "检测到有新版本发布，请等待更新自重启后使用！");

    // 关掉弹窗后，直接无脑启动下载
    startOtaDownload();
}

void MainWindow::startOtaDownload() {
    // 弹出一个小巧精致的进度条对话框，拦截用户操作
    m_updateProgressDlg = new QProgressDialog("正在拉取服务器资源...", QString(), 0, 100, this);
    m_updateProgressDlg->setWindowTitle("更新中");
    m_updateProgressDlg->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    m_updateProgressDlg->setCancelButton(nullptr); // 禁止取消，强迫症福音

    m_updateProgressDlg->setStyleSheet(
        "QProgressDialog { background-color: #FFFFFF; font-family: 'Microsoft YaHei'; font-size: 15px; font-weight: bold; color: #1E293B; }"
        "QProgressBar { border: 1px solid #E2E8F0; border-radius: 6px; text-align: center; color: #475569; font-weight: bold; }"
        "QProgressBar::chunk { background-color: #3B82F6; border-radius: 6px; }"
        );
    m_updateProgressDlg->setFixedSize(380, 120);
    m_updateProgressDlg->show();

    networkManager->requestUpdateFile();
}

void MainWindow::onUpdateDownloadProgress(int percent) {
    if (m_updateProgressDlg) {
        m_updateProgressDlg->setLabelText(QString("更新进度... %1%").arg(percent));
        m_updateProgressDlg->setValue(percent);
    }
}

void MainWindow::onUpdateDownloadFinished(bool success) {
    if (success) {
        if (m_updateProgressDlg) {
            m_updateProgressDlg->setLabelText("更新完成，正在重启并应用更新...");
            m_updateProgressDlg->setValue(100);
        }

        QSettings settings("CapzeraEdu", "SaaSOlympic");
        settings.setValue("JustUpdated", true);

        // 停留1秒，然后瞬间自杀脱壳
        QTimer::singleShot(200, this, &MainWindow::applyUpdateAndRestart);
    } else {
        if (m_updateProgressDlg) m_updateProgressDlg->close();
        CapzeraDialog::critical(this, "更新失败", "由于网络异常，新版本拉取失败，请重启软件重试。");
        QApplication::quit();
    }
}

// =========================================================
// 🚀 无敌版金蝉脱壳：毫秒级精准覆盖 & 强制 GBK 本地路径编码
// =========================================================
void MainWindow::applyUpdateAndRestart() {
    QString currentExePath = QCoreApplication::applicationFilePath();
    QString newExePath = QCoreApplication::applicationDirPath() + "/CapzeraQA_Update.exe";
    QString batPath = QCoreApplication::applicationDirPath() + "/update.bat";

    // 转换为 Windows 的本地反斜杠路径
    QString winCurrentExe = QDir::toNativeSeparators(currentExePath);
    QString winNewExe = QDir::toNativeSeparators(newExePath);
    QString winBatPath = QDir::toNativeSeparators(batPath);

    QFile batFile(batPath);
    // 使用纯二进制按字节写入，防止被 Qt 二次篡改换行符
    if (batFile.open(QIODevice::WriteOnly)) {
        QByteArray script;
        script.append("@echo off\r\n");
        script.append(QString("taskkill /f /im \"%1\" > NUL 2>&1\r\n").arg(QFileInfo(winCurrentExe).fileName()).toLocal8Bit());

        script.append(":retry\r\n");
        // 尝试覆盖，一旦文件锁解除，这个操作在毫秒级内完成！
        script.append(QString("move /y \"%1\" \"%2\" > NUL 2>&1\r\n").arg(winNewExe, winCurrentExe).toLocal8Bit());

        // 如果覆盖失败（证明 exe 还没完全退出），利用 ping 做极其短暂的微小延迟后继续重试
        script.append("if errorlevel 1 (\r\n");
        script.append("    ping 127.0.0.1 -n 1 -w 50 > NUL 2>&1\r\n");
        script.append("    goto retry\r\n");
        script.append(")\r\n");

        // 3. 覆盖成功后立刻启动！
        script.append(QString("start \"\" \"%1\"\r\n").arg(winCurrentExe).toLocal8Bit());
        script.append("del \"%~f0\"\r\n");

        batFile.write(script);
        batFile.close();
    }

    QProcess::startDetached("cmd.exe", QStringList() << "/c" << winBatPath);
    QApplication::quit();
}

void MainWindow::loginTriggle() {
    QString user = usernameLineEdit->text().trimmed();
    QString pass = passwordLineEdit->text().trimmed();
    if (user.isEmpty()) return;
    networkManager->sendLoginRequest(user, pass);
}

void MainWindow::handleLoginResult(bool success, const QString &message, const QString &realName) {
    if (!success) { CapzeraDialog::critical(this, "身份验证失败", message); passwordLineEdit->clear(); passwordLineEdit->setFocus(); return; }

    QSettings settings("CapzeraEdu", "SaaSOlympic");
    settings.setValue("LastSavedUser", usernameLineEdit->text().trimmed());

    this->setMinimumSize(0, 0); this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    this->setWindowOpacity(0.0); this->showFullScreen();

    if (message.contains("老师") || usernameLineEdit->text() == "superadmin") {
        teacherPage->setTeacherAccount(usernameLineEdit->text());
        QString displayName = realName.isEmpty() ? usernameLineEdit->text() : realName;
        teacherPage->setTeacherName(displayName);
        stackedWidget->setCurrentIndex(2);
    } else {
        studentPage->refreshDashboard();
        stackedWidget->setCurrentIndex(1);
    }

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(350); anim->setStartValue(0.0); anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InCubic); anim->start(QAbstractAnimation::DeleteWhenStopped);
}