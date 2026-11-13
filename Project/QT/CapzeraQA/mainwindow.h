#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAIN_VERSION 0
#define SECOND_VERSION 15
#define LAST_VERSION 5
#define VERSION_SUFFIX "Alpha"

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressDialog>
#include "NetworkManager.h"
#include "TeacherPage.h"
#include "StudentPage.h"
#include "CapzeraButton.h"

class MyButton : public QPushButton {
public:
    MyButton(const QString& str, QWidget *parent) : QPushButton(str, parent) {}
};

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public slots:
    void loginTriggle();
    void handleLoginResult(bool success, const QString &message, const QString &realName);

    void handleUpdateAvailable(bool isForced, const QString &latestVersion, const QString &releaseNotes);
    void startOtaDownload();

    void onUpdateDownloadProgress(int percent);
    void onUpdateDownloadFinished(bool success);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Init();

private:
    void applyUpdateAndRestart();

    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    double dpi;
    QString VersionNumber = "V" + QString::number(MAIN_VERSION) + "." + QString::number(SECOND_VERSION) + "." + QString::number(LAST_VERSION) + "-" + VERSION_SUFFIX;

    QStackedWidget *stackedWidget;

    QWidget *loginPage;
    QLineEdit *usernameLineEdit, *passwordLineEdit;
    MyButton *mainLogin, *Quit;

    QProgressDialog *m_updateProgressDlg = nullptr; // 🚀 极简悬浮进度条

    StudentPage *studentPage;
    TeacherPage *teacherPage;
};
#endif // MAINWINDOW_H
