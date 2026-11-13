#ifndef TEACHERPAGE_H
#define TEACHERPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QPushButton>
#include <QListWidget>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QShortcut>
#include <QGraphicsOpacityEffect>
#include <QWebEngineView>
#include <QWebChannel>
#include "NetworkManager.h"
#include "CommonTypes.h"
#include "TeacherSettingPage.h"
#include "TeacherClassPage.h"
#include "TeacherStudentPage.h"
#include "TeacherProblemPage.h"
#include "TeacherExamPage.h"
#include "ClassDashboardWidget.h"
#include "AnalysisTestWidget.h"
#include "AnalysisProblemWidget.h"
#include "CapzeraButton.h"
#include "TeacherGradingPage.h"

class TeacherPage : public QWidget {
    Q_OBJECT
public:
    explicit TeacherPage(QWidget *parent = nullptr, NetworkManager *net = nullptr);
    ~TeacherPage() {}
    void setTeacherAccount(const QString &account);
    void setTeacherName(const QString &name);

protected:
    void showEvent(QShowEvent *event) override;

private:
    enum PageIndex {
        PAGE_SETTING = 0,
        PAGE_STUDENT_OVERVIEW = 1,
        PAGE_STUDENT_MANAGE = 2,
        PAGE_STUDENT_ADD = 3,
        PAGE_CLASS_OVERVIEW = 4,
        PAGE_CLASS_MANAGE = 5,
        PAGE_CLASS_ADD = 6,
        PAGE_PROBLEM_MANAGE = 7,
        PAGE_EXAM = 8,
        PAGE_DASHBOARD = 9,           // 对应 9: 大屏总览
        PAGE_ANALYSIS_PROBLEM = 10,   // 对应 10: 题目分析
        PAGE_ANALYSIS_TEST = 11,      // 对应 11: 测试分析
        PAGE_GRADING = 12,            // 对应 12: 批阅中心
        PAGE_VERSION = 13             // 对应 13: 版本日志
    };
    enum MenuItemType { ITEM_MAIN = 0, ITEM_PARENT, ITEM_SUB };

    void initUi(QVBoxLayout *mainLayout);
    void initStyles();
    void initMockData();

    void addMainItem(int row, const QString &text, int targetPage, int itemType);
    void switchPageAnimation(int targetPage, QListWidgetItem *item);
    void dispatchAnalysisData();

private slots:
    //void onNavigationClicked(QListWidgetItem *item);
    void onCloudClassListReceived(const QJsonArray &classes);
    void onCloudStudentListReceived(const QJsonArray &students);
    void onCloudAllRecordsReceived(const QJsonObject &records);
    void onCloudExamListReceived(const QJsonArray &exams, const QJsonObject &myRecord);
    void syncClassesToCloud();
    void syncStudentsToCloud();
public slots:
    void webSignalSwitchPage(int targetPage);

signals:
    void logoutRequested();

private:
    NetworkManager *networkManager;
    double dpi;

    QString m_teacherAccount;
    QLabel *titleLabel;
    QLabel *welcomeLabel;
    //QListWidget *m_navMenu;
    QStackedWidget *m_pageStack;
    // *m_navIndicator;
    CapzeraButton *logoutBtn;

    bool m_studentExpanded;
    bool m_classExpanded;
    bool m_analysisExpanded;
    QString m_lastActiveParentText;

    QVector<StudentInfo> m_students;
    QStringList m_classRooms;
    QVector<ProblemInfo> m_problems;
    QStringList m_cloudTags;

    TeacherSettingPage *m_settingPage;
    TeacherClassPage *m_classPage;
    TeacherStudentPage *m_studentPage;
    TeacherProblemPage *m_problemPage;
    TeacherExamPage *m_examPage;
    TeacherGradingPage *m_gradingPage;
    ClassDashboardWidget *m_dashboardWidget;
    AnalysisTestWidget *m_analysisTestWidget;
    AnalysisProblemWidget *m_analysisProbWidget;

    QJsonArray m_cloudExams;   // 缓存云端试卷
    QJsonObject m_allRecords;  // 缓存全班答卷

    QWebEngineView *m_sidebarWebView;
    QWebChannel *m_sidebarChannel;
};

#endif // TEACHERPAGE_H
