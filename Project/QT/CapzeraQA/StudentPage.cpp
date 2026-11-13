#include "StudentPage.h"
#include <QGuiApplication>
#include <QScreen>
StudentPage::StudentPage(QWidget *parent, NetworkManager *net)
    : QWidget(parent), networkManager(net) {
    dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96.0;
    connect(networkManager, &NetworkManager::examListReceived, this, &StudentPage::onExamListReceived);
    initUi();

    connect(networkManager, &NetworkManager::studentInfoReceived, this, [this](const QJsonObject &studentObj) {
        StudentInfo info;
        info.account = studentObj["account"].toString();
        if (info.account.isEmpty()) info.account = m_studentAccount;

        info.realName = studentObj["realName"].toString();
        info.password = studentObj["password"].toString();
        info.motto = studentObj["motto"].toString();
        info.phone = studentObj["phone"].toString();
        info.email = studentObj["email"].toString();
        info.gender = studentObj["gender"].toString();
        info.idCard = studentObj["idCard"].toString();
        info.className = studentObj["className"].toString();
        info.regTime = studentObj["registerTime"].toString();

        this->setStudentInfo(info);
    });
}

void StudentPage::initUi() {
    this->setStyleSheet("background-color: #F8FAFC;");
    QHBoxLayout *globalLayout = new QHBoxLayout(this);
    globalLayout->setContentsMargins(0, 0, 0, 0);
    globalLayout->setSpacing(0);

    // ================= 1. 纯 Web 化高定左侧导航栏 =================
    m_sidebarContainer = new QWidget(this);
    m_sidebarContainer->setFixedWidth(qRound(240 * dpi));
    QVBoxLayout *sideL = new QVBoxLayout(m_sidebarContainer);
    sideL->setContentsMargins(0, 0, 0, 0);

    m_sidebarWebView = new QWebEngineView(m_sidebarContainer);
    m_sidebarWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_sidebarWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_sidebarWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *channel = new QWebChannel(m_sidebarWebView);
    channel->registerObject("studentSidebarBridge", this);
    m_sidebarWebView->page()->setWebChannel(channel);
    m_sidebarWebView->setUrl(QUrl("qrc:///www/student_sidebar.html"));

    sideL->addWidget(m_sidebarWebView);
    globalLayout->addWidget(m_sidebarContainer);

    // ================= 2. 右侧核心多栈调度区 (QStackedWidget) =================
    mainStack = new QStackedWidget(this);

    m_dashboardWidget = new StudentDashboardWidget(dpi, this);
    m_historyWidget   = new StudentHistoryWidget(dpi, this);
    m_settingWidget   = new StudentSettingWidget(dpi, networkManager, this);
    m_examWidget      = new StudentExamWidget(dpi, networkManager, this);

    mainStack->addWidget(m_dashboardWidget); // Index 0
    mainStack->addWidget(m_historyWidget);   // Index 1
    mainStack->addWidget(m_settingWidget);   // Index 2
    mainStack->addWidget(m_examWidget);      // Index 3

    globalLayout->addWidget(mainStack, 1);

    // ================= 3. 编排各个子兵团的神经信号互通 =================
    connect(m_dashboardWidget, &StudentDashboardWidget::startExamRequested, this, &StudentPage::onStartExam);

    connect(m_examWidget, &StudentExamWidget::returnToDashboardRequested, this, &StudentPage::resetToDashboard);

    connect(m_historyWidget, &StudentHistoryWidget::backToDashboardRequested, this, &StudentPage::resetToDashboard);
}

void StudentPage::setStudentAccount(const QString &account) {
    m_studentAccount = account;
    m_studentInfo.account = account;
    if (m_settingWidget) {
        m_settingWidget->updateStudentInfo(m_studentInfo);
    }
}
void StudentPage::setStudentInfo(const StudentInfo &info) {
    m_studentInfo = info;
    if (m_settingWidget) {
        m_settingWidget->updateStudentInfo(info);
    }
}

void StudentPage::onExamListReceived(const QJsonArray &exams, const QJsonObject &myRecord) {
    m_cachedExams = exams;
    m_cachedMyRecord = myRecord;
    m_dashboardWidget->updateExams(exams, myRecord);
    m_historyWidget->updateData(exams, myRecord);
}

void StudentPage::webSignalSwitchPage(int targetPage) {
    if (targetPage == PAGE_DASHBOARD) {
        refreshDashboard();
        mainStack->setCurrentWidget(m_dashboardWidget);
    }
    else if (targetPage == PAGE_HISTORY) {
        m_historyWidget->showHistory();
        mainStack->setCurrentWidget(m_historyWidget);
    }
    else if (targetPage == PAGE_SETTING) {
        mainStack->setCurrentWidget(m_settingWidget);
    }
}

void StudentPage::onStartExam(const QString &examId) {
    m_sidebarContainer->hide();
    mainStack->setCurrentWidget(m_examWidget);
    QJsonObject targetExamObj;
    for (int i = 0; i < m_cachedExams.size(); ++i) {
        if (m_cachedExams[i].toObject()["examId"].toString() == examId) {
            targetExamObj = m_cachedExams[i].toObject();
            break;
        }
    }
    m_examWidget->startExam(targetExamObj, m_cachedMyRecord);
}

void StudentPage::resetToDashboard() {
    m_sidebarContainer->show();
    mainStack->setCurrentWidget(m_dashboardWidget);
    refreshDashboard();
}

void StudentPage::refreshDashboard() {
    networkManager->requestExamList();
}

void StudentPage::webSignalLogout() {
    emit logoutRequested();
}