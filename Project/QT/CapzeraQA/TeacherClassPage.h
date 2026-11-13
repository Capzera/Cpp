#ifndef TEACHERCLASSPAGE_H
#define TEACHERCLASSPAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebEngineView>
#include <QJsonObject>
#include <QJsonArray>
#include "CommonTypes.h"
#include "CapzeraComboBox.h"

class TeacherClassPage : public QObject {
    Q_OBJECT
public:
    explicit TeacherClassPage(QStringList *classRooms, QVector<StudentInfo> *students, double dpi, QObject *parent = nullptr);

    QWidget* getOverviewPage();
    QWidget* getManagePage();
    QWidget* getAddPage();

    void refreshOverviewStats();
    void refreshTable();

public slots:
    // 🚀 前端高级表格传回的信号通道
    void onWebRowClicked(const QString &className);

signals:
    void classListUpdated();
    void studentListUpdated();

private slots:
    void onAddNewClassRun();
    void onTransferStudent();
    void onRemoveStudentFromClass();
    void onMergeClassAction();

private:
    QWidget* createCardContainer();
    void refreshClassDetailPanel();

    QStringList *m_classRooms;
    QVector<StudentInfo> *m_students;
    double m_dpi;

    QWidget *m_overviewView;
    QWidget *m_manageView;
    QWidget *m_addView;

    // 总览标签群
    QLabel *lblTotalClasses;
    QLabel *lblTotalStudents;
    QLabel *lblAvgClassSize;
    QLabel *lblUnassignedCount;

    // 🚀 表格全权升级为 WebEngineView
    QWebEngineView *m_tableWebView;
    bool m_isTableLoaded = false;
    QJsonArray m_cachedTableData;

    QString m_selectedClassName;
    QLabel *m_detailTitleLabel;
    QLabel *m_detailCountLabel;
    CapzeraComboBox *m_transferStudentCombo;
    CapzeraComboBox *m_transferTargetCombo;
    QPushButton *btnTransfer;
    QPushButton *btnRemove;
    QPushButton *btnMergeClass;

    QLineEdit *m_newClassEdit;
};

#endif // TEACHERCLASSPAGE_H