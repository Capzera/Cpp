#ifndef TEACHERSTUDENTPAGE_H
#define TEACHERSTUDENTPAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWebEngineView>
#include <QDialog>
#include <QTextEdit>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include "CommonTypes.h"
#include "CapzeraComboBox.h"
#include "CapzeraButton.h"

class TeacherStudentPage : public QObject {
    Q_OBJECT
public:
    explicit TeacherStudentPage(QVector<StudentInfo> *students, QStringList *classRooms, double dpi, QObject *parent = nullptr);

    QWidget* getOverviewPage();
    QWidget* getManagePage();
    QWidget* getAddPage();

    void updateRealData(const QJsonArray &exams, const QJsonObject &allRecords);
    void refreshOverviewStats();
    void refreshTable();

public slots:
    void updateClassFilterCombo();
    void onWebRowClicked(int index);

    // 右侧高定网页表单传回 C++ 的强力通讯接口
    void webSignalChangeClass(const QString &targetClass);
    void webSignalResetPwd();
    void webSignalDeleteStudent();

signals:
    void studentListUpdated();

private slots:
    void onAddSingleStudent();
    void onOpenBatchImportDialog();

private:
    QWidget* createCardContainer();

    QVector<StudentInfo> *m_students;
    QStringList *m_classRooms;
    double m_dpi;

    QJsonObject m_allRecords;
    QJsonArray m_cloudExams; // 缓存试卷信息，用于精确映射折线图标题
    int m_totalExams = 0;

    QWidget *m_overviewView;
    QWidget *m_manageView;
    QWidget *m_addView;

    // 总览标签
    QLabel *m_lblTotalStudents;
    QLabel *m_lblActiveStudents;
    QLabel *m_lblCompletionRate;
    QLabel *m_lblLoginStats;

    // 管理顶部过滤
    CapzeraComboBox *m_classFilterCombo;
    QLineEdit *m_searchEdit;

    // 核心网格视图
    QWebEngineView *m_tableWebView;
    QWebEngineView *m_detailWebView;

    bool m_isTableLoaded;
    bool m_isDetailLoaded;
    int m_selectedStudentIdx;

    QLineEdit *m_singleStudentEdit;
};

#endif // TEACHERSTUDENTPAGE_H