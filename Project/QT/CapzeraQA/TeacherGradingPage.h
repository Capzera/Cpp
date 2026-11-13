#ifndef TEACHERGRADINGPAGE_H
#define TEACHERGRADINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>
#include "CommonTypes.h"
#include "NetworkManager.h"
#include "CapzeraButton.h"

struct GradingTask {
    QString account;
    QString name;
    QString examId;
    QString examTitle;
    int qIndex;
    QString type;
    int maxScore;
    int currentScore;
    bool isGraded;
    QString updateTime;
};

class TeacherGradingPage : public QWidget {
    Q_OBJECT
public:
    explicit TeacherGradingPage(double dpi, NetworkManager *net, QWidget *parent = nullptr);
    void updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students);

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void onWebRowClicked(int index);
    void onExamFilterChanged(const QString &examId);

private slots:
    void onSubmitGrade();
    void onMinusClicked();
    void onPlusClicked();

private:
    void initUI();
    void refreshTaskList();
    void syncWebCombo(const QMap<QString, QString>& validExams);
    void renderTableData();

    double m_dpi;
    NetworkManager *m_networkManager;

    QWebEngineView *m_tableWebView;
    bool m_isTableLoaded = false;

    QWidget *m_middlePanel;
    QLabel *m_titleLabel;
    QWebEngineView *m_stemWebView;
    QWebEngineView *m_ansWebView;
    bool m_isStemLoaded = false;
    bool m_isAnsLoaded = false;

    // 🚀 仿 Web 现代控制台
    CapzeraButton *m_btnMinus, *m_btnPlus;
    QLineEdit *m_scoreEdit;
    CapzeraButton *m_btnSubmit;

    QJsonArray m_cloudExams;
    QJsonObject m_allRecords;
    QVector<StudentInfo> m_students;
    QVector<GradingTask> m_tasks;

    int m_currentTaskIndex = -1;
    int m_currentMaxScore = 0;
    QString m_currentFilterExamId = "ALL";
};

#endif // TEACHERGRADINGPAGE_H