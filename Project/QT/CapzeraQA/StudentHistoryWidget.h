#ifndef STUDENTHISTORYWIDGET_H
#define STUDENTHISTORYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPushButton>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QStackedWidget>
#include "CapzeraButton.h"

class StudentHistoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit StudentHistoryWidget(double dpi, QWidget *parent = nullptr);
    void updateData(const QJsonArray &cloudExams, const QJsonObject &myRecord);
    void showHistory();

signals:
    void backToDashboardRequested();

public slots:
    // 🚀 新增：专门接收 Web 历史表格点击一行的接口
    void onWebRowClicked(const QString &examId);

private slots:
    void loadHistoryQuestion(int index);

private:
    void initUI();
    void buildHistoryNavGrid();
    void updateHistoryNavGrid();
    int calculateStudentHistoryScore(const QJsonArray &qs, const QJsonObject &ans, const QJsonObject &mScores);

    double m_dpi;
    QJsonArray m_cloudExams;
    QJsonObject m_myRecord;

    // 🚀 核心替换：抛弃 QTableWidget，换装 WebEngineView
    QWebEngineView *m_historyTableWebView;
    bool m_isTableLoaded = false;
    QJsonArray m_cachedTableData;

    QWidget *m_historyDetailPanel;
    QLabel *m_historyTitleLabel;

    QWebEngineView *m_historyQuestionWebView;
    QWebEngineView *m_historyOptionsWebView;
    QWebEngineView *m_historyCodeWebView;

    QWidget *m_historyAnswerContainer;
    QVBoxLayout *m_historyAnswerLayout;
    QStackedWidget *m_historyBottomStack;

    QWidget *m_currentHistoryAnswerWidget;

    QLabel *m_historyScoreLabel;
    QListWidget *m_historyNavWidget;
    QVector<CapzeraButton*> m_historyNavButtons;

    QJsonArray m_historyQuestions;
    QJsonObject m_historyAnswers;
    QJsonObject m_historyManualScores;
    QJsonObject m_historyFeedbacks;
    QString m_currentHistoryExamId;
    int m_currentHistoryQuestionIndex;
};

#endif // STUDENTHISTORYWIDGET_H