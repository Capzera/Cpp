#ifndef TEACHEREXAMPAGE_H
#define TEACHEREXAMPAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QWebEngineView>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QPageLayout>
#include "CommonTypes.h"
#include "NetworkManager.h"
#include "DateTimePickerWidget.h"
#include "CapzeraDialog.h"
#include "CapzeraToggle.h"
#include "CapzeraComboBox.h"
#include "CapzeraButton.h"

class TeacherExamPage : public QObject {
    Q_OBJECT
public:
    explicit TeacherExamPage(QVector<ProblemInfo> *problems, QStringList *classRooms, NetworkManager *net, double dpi, QObject *parent = nullptr);

    QWidget* getView();
    void fetchCloudData();

public slots:
    void updateClassFilterCombo();
    void refreshBankTable();

    // 🚀 Web 桥接大核心
    void onWebOverviewRowClicked(const QString &examId);
    void onWebBankRowClicked(const QString &probId);
    void onWebBasketRowClicked(int index);
    void onExportPdfClicked();
    void webSignalToggleExamStatus(const QString &examId, bool isEnabled);
    void webSignalUpdateBasketScore(const QString &probId, int newScore);

private slots:
    void onExamTabSwitchClicked(int index);
    void onDeleteExamClicked();
    void onSubmitNewExam();
    void resetExamForm();
    void onExamListReceived(const QJsonArray &exams);
    void onCreateExamResult(bool success, const QString &message);
    void onUpdateExamResult(bool success, const QString &message);
    void onDeleteExamResult(bool success, const QString &message);

private:
    QWidget* createCardContainer();
    void refreshOverviewTable();
    void refreshBasketTable();
    void recalculateTotalScore();

    QVector<ProblemInfo> *m_problems;
    QStringList *m_classRooms;
    NetworkManager *m_networkManager;
    double m_dpi;

    QWidget *m_view;
    QTabWidget *m_examTabWidget;

    // 🚀 核心替换：抛弃 QTableWidget，拥抱大厂 Web 数据网格
    QWebEngineView *m_overviewWebView;
    QWebEngineView *m_bankWebView;
    QWebEngineView *m_basketWebView;

    bool m_isOverviewLoaded = false;
    bool m_isBankLoaded = false;
    bool m_isBasketLoaded = false;
    QJsonArray m_cachedOverview;
    QJsonArray m_cachedBank;
    QJsonArray m_cachedBasket;

    QLineEdit *m_examIdEdit;
    QLineEdit *m_examTitleEdit;
    CapzeraComboBox *m_examClassCombo;

    DateTimePickerWidget *m_examStartEdit;
    DateTimePickerWidget *m_examEndEdit;

    CapzeraToggle *m_toggleShuffle;
    CapzeraToggle *m_toggleOptionShuffle;

    CapzeraComboBox *m_searchTagCombo;
    CapzeraComboBox *m_searchDiffCombo;
    QLineEdit *m_searchKeywordEdit;

    QLabel *m_lblTotalScore;
    QLabel *m_lblProbCount;
    CapzeraButton *btnSubmitExamAction;
    CapzeraButton *btnCancelEditAction;
    CapzeraButton *m_btnDeleteExam;
    CapzeraButton *btnExportPdf;

    QJsonArray m_cloudExams;

    QVector<ProblemInfo> m_basketProblems;
    QMap<QString, int> m_basketScores;

    bool m_isEditingExam;
    QString m_editingExamId;
    bool m_isSilentUpdate = false;
};

#endif // TEACHEREXAMPAGE_H