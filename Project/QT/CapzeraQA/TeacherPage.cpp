#include "TeacherPage.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QVBoxLayout>

TeacherPage::TeacherPage(QWidget *parent, NetworkManager *net)
    : QWidget(parent), networkManager(net), m_studentExpanded(false), m_classExpanded(false), m_analysisExpanded(false), m_lastActiveParentText("") {
    qDebug() << "【TeacherPage】启动全盘 Web 化侧边栏中枢构造...";
    dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96.0;
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setStyleSheet("background-color: #F8FAFC;");

    initMockData();

    m_settingPage = new TeacherSettingPage(dpi, this);
    m_classPage = new TeacherClassPage(&m_classRooms, &m_students, dpi, this);
    m_studentPage = new TeacherStudentPage(&m_students, &m_classRooms, dpi, this);
    m_problemPage = new TeacherProblemPage(&m_problems, &m_cloudTags, networkManager, dpi, this);
    m_examPage = new TeacherExamPage(&m_problems, &m_classRooms, networkManager, dpi, this);
    m_dashboardWidget = new ClassDashboardWidget(this);
    m_analysisTestWidget = new AnalysisTestWidget(this);
    m_analysisProbWidget = new AnalysisProblemWidget(this);
    m_gradingPage = new TeacherGradingPage(dpi, networkManager, this);

    // 🚀 核心修复 1：把所有的网络接口都接管到 TeacherPage 中枢
    connect(networkManager, &NetworkManager::classListReceived, this, &TeacherPage::onCloudClassListReceived);
    connect(networkManager, &NetworkManager::studentListReceived, this, &TeacherPage::onCloudStudentListReceived);
    connect(networkManager, &NetworkManager::allRecordsReceived, this, &TeacherPage::onCloudAllRecordsReceived);
    connect(networkManager, &NetworkManager::dashboardStatsReceived, m_dashboardWidget, &ClassDashboardWidget::updateDashboardData);
    connect(networkManager, &NetworkManager::examListReceived, this, &TeacherPage::onCloudExamListReceived); // 接管考试列表

    connect(m_classPage, &TeacherClassPage::classListUpdated, this, &TeacherPage::syncClassesToCloud);
    connect(m_classPage, &TeacherClassPage::studentListUpdated, this, &TeacherPage::syncStudentsToCloud);
    connect(m_studentPage, &TeacherStudentPage::studentListUpdated, this, &TeacherPage::syncStudentsToCloud);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_sidebarWebView = new QWebEngineView(this);
    m_sidebarWebView->setFixedWidth(qRound(240 * dpi));
    m_sidebarWebView->setStyleSheet("background-color: #F8FAFC; border-right: 1px solid #E2E8F0;");

    m_sidebarChannel = new QWebChannel(this);
    m_sidebarChannel->registerObject(QStringLiteral("teacherSidebarBridge"), this);
    m_sidebarWebView->page()->setWebChannel(m_sidebarChannel);

    m_sidebarWebView->setUrl(QUrl("qrc:///www/sidebar.html"));

    QVBoxLayout *rightContentLayout = new QVBoxLayout();
    rightContentLayout->setContentsMargins(0, 0, 0, 0);
    rightContentLayout->setSpacing(0);

    QWidget *topHeaderBar = new QWidget(this);
    topHeaderBar->setFixedHeight(qRound(65 * dpi));
    topHeaderBar->setStyleSheet("QWidget { background: white; border-bottom: 1px solid #E2E8F0; }");
    QHBoxLayout *headerLayout = new QHBoxLayout(topHeaderBar);
    headerLayout->setContentsMargins(30, 0, 30, 0);

    welcomeLabel = new QLabel("👋 老师好，欢迎使用教研端", topHeaderBar);
    welcomeLabel->setFont(QFont("Microsoft YaHei", qRound(15 / dpi), QFont::Bold));
    welcomeLabel->setStyleSheet("color: #1E293B; border: none; background: transparent;");

    logoutBtn = new CapzeraButton("退出登录", CapzeraButton::Danger, topHeaderBar);
    logoutBtn->setFixedSize(qRound(160 * dpi), qRound(38 * dpi));
    connect(logoutBtn, &QPushButton::clicked, this, &TeacherPage::logoutRequested);
    logoutBtn->setShortcut(QKeySequence(Qt::Key_Escape)); // 按ESC键可以退出登录

    headerLayout->addWidget(welcomeLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutBtn);

    m_pageStack = new QStackedWidget(this);
    m_pageStack->setStyleSheet("background-color: #F8FAFC;");

    m_pageStack->insertWidget(PAGE_SETTING, m_settingPage->getView());
    m_pageStack->insertWidget(PAGE_STUDENT_OVERVIEW, m_studentPage->getOverviewPage());
    m_pageStack->insertWidget(PAGE_STUDENT_MANAGE, m_studentPage->getManagePage());
    m_pageStack->insertWidget(PAGE_STUDENT_ADD, m_studentPage->getAddPage());
    m_pageStack->insertWidget(PAGE_CLASS_OVERVIEW, m_classPage->getOverviewPage());
    m_pageStack->insertWidget(PAGE_CLASS_MANAGE, m_classPage->getManagePage());
    m_pageStack->insertWidget(PAGE_CLASS_ADD, m_classPage->getAddPage());
    m_pageStack->insertWidget(PAGE_PROBLEM_MANAGE, m_problemPage->getView());
    m_pageStack->insertWidget(PAGE_EXAM, m_examPage->getView());

    m_pageStack->insertWidget(PAGE_DASHBOARD, m_dashboardWidget);
    m_pageStack->insertWidget(PAGE_ANALYSIS_PROBLEM, m_analysisProbWidget);
    m_pageStack->insertWidget(PAGE_ANALYSIS_TEST, m_analysisTestWidget);
    m_pageStack->insertWidget(PAGE_GRADING, m_gradingPage);

    rightContentLayout->addWidget(topHeaderBar);
    rightContentLayout->addWidget(m_pageStack, 1);

    mainLayout->addWidget(m_sidebarWebView);
    mainLayout->addLayout(rightContentLayout, 1);

    m_pageStack->setCurrentIndex(PAGE_CLASS_MANAGE);
}

void TeacherPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    qDebug() << "【TeacherPage】触发核心全局云端同步信号下发...";
    networkManager->requestClassList();
    networkManager->requestStudentList();
    networkManager->requestExamList();
    networkManager->requestAllRecords();
}

void TeacherPage::onCloudClassListReceived(const QJsonArray &classes) {
    m_classRooms.clear();
    for (int i = 0; i < classes.size(); ++i) {
        m_classRooms.append(classes[i].toString());
    }
    m_classPage->refreshOverviewStats();
    m_classPage->refreshTable();
    m_studentPage->updateClassFilterCombo();
    m_examPage->updateClassFilterCombo();
}

void TeacherPage::onCloudStudentListReceived(const QJsonArray &students) {
    m_students.clear();
    for (int i = 0; i < students.size(); ++i) {
        QJsonObject obj = students[i].toObject();
        StudentInfo s;
        s.username = obj["realName"].toString();
        s.className = obj["className"].toString();
        s.regTime = obj["registerTime"].toString();
        s.account = obj["account"].toString();
        s.lastLoginTime = obj["lastLoginTime"].toString();
        s.lastSubmitTime = obj["lastSubmitTime"].toString();
        s.phone = obj["phone"].toString();
        s.email = obj["email"].toString();
        s.idCard = obj["idCard"].toString();
        s.motto = obj["motto"].toString();
        s.gender = obj["gender"].toString();
        s.realName = obj["realName"].toString();
        m_students.append(s);
    }
    m_classPage->refreshOverviewStats();
    m_studentPage->refreshOverviewStats();
    m_studentPage->refreshTable();

    // 收到学生数据后，尝试分发给分析面板
    dispatchAnalysisData();
}

// 🚀 核心修复 2：统一接管考试列表
void TeacherPage::onCloudExamListReceived(const QJsonArray &exams, const QJsonObject &myRecord) {
    Q_UNUSED(myRecord);
    m_cloudExams = exams;

    // 收到考试数据后，尝试分发给分析面板
    dispatchAnalysisData();
}

void TeacherPage::onCloudAllRecordsReceived(const QJsonObject &records) {
    m_allRecords = records;
    m_studentPage->updateRealData(m_cloudExams, m_allRecords);

    // 收到答卷数据后，尝试分发给分析面板
    dispatchAnalysisData();
}

// 🚀 核心修复 3：数据安全分发中枢。只有当必需的数据都非空时，才更新面板！
void TeacherPage::dispatchAnalysisData() {
    if (m_cloudExams.isEmpty() || m_allRecords.isEmpty()) {
        return; // 数据没准备好，拒绝下发！避免下拉框被清空
    }

    // 只有所有数据就位，才统一推送
    m_analysisTestWidget->updateRealData(m_cloudExams, m_allRecords, m_students);
    m_analysisProbWidget->updateRealData(m_cloudExams, m_allRecords, m_students);
    m_gradingPage->updateRealData(m_cloudExams, m_allRecords, m_students);
}

void TeacherPage::syncClassesToCloud() {
    QJsonArray arr;
    for (const QString &c : m_classRooms) arr.append(c);
    networkManager->syncClasses(arr);
}

void TeacherPage::syncStudentsToCloud() {
    QJsonArray arr;
    for (const StudentInfo &s : m_students) {
        QJsonObject obj;
        obj["realName"] = s.username;
        obj["className"] = s.className;
        obj["registerTime"] = s.regTime;
        obj["account"] = s.account;
        arr.append(obj);
    }
    networkManager->syncStudents(arr);
}

void TeacherPage::setTeacherAccount(const QString &account) {
    m_teacherAccount = account;
}

void TeacherPage::setTeacherName(const QString &name) {
    welcomeLabel->setText(QString("👋 %1 老师好，欢迎使用教研端").arg(name));
    m_settingPage->setTeacherName(name);
}

void TeacherPage::webSignalSwitchPage(int targetPage) {
    if (targetPage == PAGE_CLASS_MANAGE) {
        m_classPage->refreshTable();
    }
    else if (targetPage == PAGE_PROBLEM_MANAGE) {
        m_problemPage->fetchCloudData();
    }
    else if (targetPage == PAGE_EXAM) {
        m_examPage->fetchCloudData();
    }
    else if (targetPage == PAGE_GRADING) {
        networkManager->requestExamList();
        networkManager->requestAllRecords();
    }
    else if (targetPage == PAGE_DASHBOARD || targetPage == PAGE_ANALYSIS_PROBLEM || targetPage == PAGE_ANALYSIS_TEST) {
        // 强行刷新一次所有数据，返回时会自动触发 dispatchAnalysisData
        networkManager->requestExamList();
        networkManager->requestAllRecords();
        if (targetPage == PAGE_DASHBOARD) {
            networkManager->requestDashboardStats();
        }
    }

    QWidget *nextPage = m_pageStack->widget(targetPage);
    if (nextPage) {
        m_pageStack->setCurrentIndex(targetPage);
    }
}

void TeacherPage::initMockData() {
    m_cloudTags << "基础语法" << "递推/递归" << "贪心" << "动态规划" << "数据结构";
}