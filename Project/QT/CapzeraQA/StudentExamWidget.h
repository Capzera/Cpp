#ifndef STUDENTEXAMWIDGET_H
#define STUDENTEXAMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTimer>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QSettings>
#include <random>
#include "NetworkManager.h"
#include "CapzeraButton.h"

class StudentExamWidget : public QWidget {
    Q_OBJECT
public:
    explicit StudentExamWidget(double dpi, NetworkManager *net, QWidget *parent = nullptr);
    void startExam(const QJsonObject &examObj, const QJsonObject &myRecord);

signals:
    void returnToDashboardRequested();

private slots:
    void updateCountdown();
    void onPrevQuestion();
    void onNextQuestion();
    void onSaveTemporarily();
    void onSubmitExam();
    void onOptionsTitleChanged(const QString &title);
    void onCodeTitleChanged(const QString &title);

private:
    void initUI();
    void buildNavGrid();
    void updateNavGrid();

    void loadQuestion(int index);
    void saveCurrentAnswer(); // 🚀 恢复你最强大的原生同步存取器

    double m_dpi;
    NetworkManager *m_networkManager;

    // --- 考试底层状态锁 ---
    QString m_currentExamId;
    QJsonArray m_currentQuestions;
    QVector<int> m_questionSequence;
    QMap<int, QVariant> m_answersCache;
    int currentQuestionIndex;
    int remainingSeconds;
    QTimer *examTimer;

    bool m_isOptionShuffled = false;
    QMap<int, QVector<int>> m_optionMapping;

    // --- 核心 UI 控件 ---
    QLabel *examTitleLabel;
    QWebEngineView *m_questionWebView;

    QWidget *answerAreaContainer;
    QVBoxLayout *answerAreaLayout;
    QStackedWidget *m_bottomStack;
    QWebEngineView *m_optionsWebView;
    bool m_isOptionsLoaded = false;
    QWebEngineView *m_codeWebView;
    bool m_isCodeLoaded = false;

    QButtonGroup *m_radioGroup;
    QVector<QCheckBox*> m_checkBoxes;
    QLineEdit *m_fillEdit;

    QListWidget *m_navGridWidget;
    QVector<CapzeraButton*> m_navButtons;
    QLabel *m_bigTimerLabel;

    CapzeraButton *btnPrevQuestion;
    CapzeraButton *btnNextQuestion;
    CapzeraButton *btnSaveTemporarily;
    CapzeraButton *btnSubmitExam;
    CapzeraButton *btnLogoutExam;
};

#endif // STUDENTEXAMWIDGET_H