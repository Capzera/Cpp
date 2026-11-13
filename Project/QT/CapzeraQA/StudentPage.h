#ifndef STUDENTPAGE_H
#define STUDENTPAGE_H

#include <QWidget>
#include <QStackedWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "NetworkManager.h"
#include "CommonTypes.h"
#include "StudentDashboardWidget.h"
#include "StudentExamWidget.h"
#include "StudentHistoryWidget.h"
#include "StudentSettingWidget.h"

class StudentPage : public QWidget {
    Q_OBJECT
public:
    explicit StudentPage(QWidget *parent = nullptr, NetworkManager *net = nullptr);
    ~StudentPage() = default;

    // 🚀 完美继承原有外部接口：设置登录学生的底层画像
    void setStudentAccount(const QString &account);
    void setStudentInfo(const StudentInfo &info);

public slots:
    void refreshDashboard();
    void resetToDashboard();

    // 🚀 Web 侧边栏专属路由桥接槽函数
    void webSignalSwitchPage(int targetPage);
    void webSignalLogout();

signals:
    void logoutRequested();

private slots:
    void onExamListReceived(const QJsonArray &exams, const QJsonObject &myRecord);
    void onStartExam(const QString &examId); // 响应开考信号

private:
    void initUi();

    NetworkManager *networkManager;
    double dpi;

    // 当前登录学生的核心状态
    QString m_studentAccount;
    StudentInfo m_studentInfo;
    QJsonArray m_cachedExams;
    QJsonObject m_cachedMyRecord;

    // 🚀 路由中枢三大核心支柱
    QWidget *m_sidebarContainer;
    QWebEngineView *m_sidebarWebView;
    QStackedWidget *mainStack;

    // 🚀 独立解耦的四大子兵团指针
    StudentDashboardWidget *m_dashboardWidget;
    StudentExamWidget      *m_examWidget;
    StudentHistoryWidget   *m_historyWidget;
    StudentSettingWidget   *m_settingWidget;

    // 页面索引枚举
    enum PageIndex {
        PAGE_DASHBOARD = 0,
        PAGE_HISTORY   = 1,
        PAGE_SETTING   = 2,
        PAGE_EXAM      = 3
    };
};

#endif // STUDENTPAGE_H