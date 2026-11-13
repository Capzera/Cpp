#ifndef ANALYSISTESTWIDGET_H
#define ANALYSISTESTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QSplitter>
#include <QListWidget>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QStackedWidget>
#include <QLineEdit>
#include "CommonTypes.h"
#include "CapzeraButton.h" // ⚠️ 已经移除 CapzeraComboBox

class AnalysisTestWidget : public QWidget {
    Q_OBJECT
public:
    explicit AnalysisTestWidget(QWidget *parent = nullptr);
    void updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students);

public slots:
    void onExamIndexChanged(int index);
    void onWebRowClicked(const QString &account);

private:
    void initUI();
    void syncWebCombo();
    void loadQuestion(int index);
    void buildNavGrid();
    void updateNavGrid();
    int calculateDynamicScore(const QJsonArray &questions, const QJsonObject &answers, const QJsonObject &manualScores);
    QWebEngineView *m_tableWebView;
    bool m_isTableLoaded = false;
    QLabel *m_paperTitleLabel;
    CapzeraButton *m_btnExportPdf;

    QWidget *m_middlePanel;
    QWebEngineView *m_stemWebView;

    QStackedWidget *m_bottomStack;
    QWebEngineView *m_optionsWebView;
    QWebEngineView *m_codeWebView;
    QWidget *m_answerContainer;
    QVBoxLayout *m_answerLayout;

    QWidget *m_feedbackWidget;

    bool m_isStemLoaded = false;
    bool m_isOptionsLoaded = false;

    QJsonObject m_currentStudentAnswers;
    QJsonArray m_currentQuestions;
    QVector<StudentInfo> m_students;
    QJsonArray m_cloudExams;
    QJsonObject m_allRecords;

    int m_currentExamIndex = -1;
    int m_currentQuestionIndex;

    QListWidget *m_navGridWidget;
    QVector<CapzeraButton*> m_navButtons;
    QString m_currentAccount;
};

#endif // ANALYSISTESTWIDGET_H