#ifndef TEACHERPROBLEMPAGE_H
#define TEACHERPROBLEMPAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDialog>
#include <QTextEdit>
#include <QTextBrowser>
#include <QStackedWidget>
#include <QTabWidget>
#include <QScrollArea>
#include <QCheckBox>
#include <QListWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QFormLayout>
#include <QWebEngineView>
#include <QDir>
#include "CommonTypes.h"
#include "NetworkManager.h"
#include "CapzeraComboBox.h"
#include "CapzeraButton.h"

class TeacherProblemPage : public QObject {
    Q_OBJECT
public:
    explicit TeacherProblemPage(QVector<ProblemInfo> *problems, QStringList *cloudTags, NetworkManager *net, double dpi, QObject *parent = nullptr);
    void initUI();
    QWidget* getView();
    void fetchCloudData();
    void refreshTable();

signals:
    void problemBankUpdated();

public slots:
    void onDetailWebLoaded();
    void onWebTableRowClicked(const QString &probId);

    // 开放给网页调用的三个中枢接口
    void openManageDiffsFromWeb();
    void openManageTagsFromWeb();
    void onAddWebLoaded();
    void submitProblemFromWeb(const QString &b64Data);
    void cancelEditFromWeb();

private slots:
    void onManageTagsClicked();
    void resetProblemForm();
    void onAddNewProblemRun();
    void onEditProblemTriggered();
    void onDeleteProblemTriggered();

    // 云端数据包回调中枢
    void onTagsReceived(const QJsonArray &tagsArray);
    void onSyncTagsResult(bool success, const QString &message);
    void onDiffsReceived(const QJsonArray &diffsArray);
    void onSyncDiffsResult(bool success, const QString &message);
    void onProblemListReceived(const QJsonArray &problemsArray);
    void onAddProblemResult(bool success, const QString &message);
    void onUpdateProblemResult(bool success, const QString &message);
    void onDeleteProblemResult(bool success, const QString &message);

private:
    QWidget* createCardContainer();

    QVector<ProblemInfo> *m_problems;
    QStringList *m_cloudTags;
    NetworkManager *m_networkManager;
    double m_dpi;

    QWidget *m_view;

    QWebEngineView *m_tableWebView;
    bool m_isTableLoaded = false;
    QJsonArray m_cachedTableData;
    CapzeraComboBox *m_searchTagCombo;
    QLineEdit *m_searchKeywordEdit;

    QTabWidget *m_rightTabWidget;
    QWebEngineView *m_addWebView;
    bool m_isAddLoaded = false;
    bool m_isEditingProb = false;
    QString m_cachedAddId;
    QJsonObject m_cachedEditPayload;
    QWebEngineView *m_detailWebView;

    bool m_isDetailLoaded = false;
    QJsonObject m_cachedDetailPayload;
    QString m_currentSelectedProbId;
    CapzeraButton *btnEditProb;
    CapzeraButton *btnDeleteProb;
    QStringList m_customDiffs; // 🚀 升级：现在作为云端难度在内存中的动态镜像
};

#endif // TEACHERPROBLEMPAGE_H