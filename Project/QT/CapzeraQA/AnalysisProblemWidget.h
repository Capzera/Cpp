#ifndef ANALYSISPROBLEMWIDGET_H
#define ANALYSISPROBLEMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QSplitter>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebChannel>
#include "CommonTypes.h"

class AnalysisProblemWidget : public QWidget {
    Q_OBJECT
public:
    explicit AnalysisProblemWidget(QWidget *parent = nullptr);
    void updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students);

public slots:
    void onExamIndexChanged(int index);
    void onWebPageLoaded();
    void onWebRowClicked(int index);

private:
    void initUI();
    void syncWebCombo();
    void loadProblemAnalysis(int index);
    bool checkAnswerCorrectness(const QString &type, const QVariant &stdAns, const QVariant &stuAns);

    QWebEngineView *m_tableWebView;
    bool m_isTableLoaded = false;
    QLabel *m_analysisTitleLabel;

    QWebEngineView *m_webView;
    bool m_isPageLoaded = false;
    QJsonObject m_cachedPayload;

    QJsonArray m_cloudExams;
    QJsonObject m_allRecords;
    QVector<StudentInfo> m_students;

    int m_currentExamIndex = -1;
    QJsonArray m_currentQuestions;
};

#endif // ANALYSISPROBLEMWIDGET_H