#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QScreen>
#include <QFile>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    void connectToServer(const QString &ip, quint16 port);
    void sendLoginRequest(const QString &username, const QString &password);
    void sendAnswer(const QString &examId, const QJsonObject &answers);
    void saveExamProgress(const QString &examId, const QJsonObject &answers);
    void updateTeacherProfile(const QString &account, const QString &newName, const QString &newPwd);
    void updateStudentProfile(const QString &account, const QString &newName, const QString &newPwd, const QString &newMotto, const QString &phone, const QString &email, const QString &gender, const QString &idCard);
    void requestClassList();
    void syncClasses(const QJsonArray &classesArray);
    void requestStudentList();
    void syncStudents(const QJsonArray &studentsArray);

    void requestProblemList();
    void submitNewProblem(const QJsonObject &problem);
    void requestUpdateProblem(const QJsonObject &problem);
    void requestDeleteProblem(const QString &id);
    void requestTags();
    void syncTags(const QJsonArray &tagsArray);
    void requestDiffs();
    void syncDiffs(const QJsonArray &diffsArray);
    void requestExamList();
    void submitNewExam(const QJsonObject &exam);
    void requestUpdateExam(const QJsonObject &exam);
    void requestDeleteExam(const QString &examId);
    void requestDashboardStats();
    void requestAllRecords();
    void updateManualScore(const QString &account, const QString &examId, int qIndex, int score);

    void checkUpdate(const QString &currentVersion);

    // 🚀 OTA 新增：请求开始下载更新文件
    void requestUpdateFile();

    QTcpSocket *tcpSocket;

signals:
    void connectedSuccessfully();
    void loginResult(bool success, const QString &message, const QString &realName);
    void newExamReceived(const QJsonObject &examData);
    void questionReceived(const QJsonObject &questionData);
    void teacherProfileUpdated(bool success, const QString &message);
    void studentProfileUpdated(bool success, const QString &message, const QJsonObject &updatedData);

    void classListReceived(const QJsonArray &classes);
    void syncClassesResult(bool success, const QString &message);
    void studentListReceived(const QJsonArray &students);
    void syncStudentsResult(bool success, const QString &message);

    void problemListReceived(const QJsonArray &problems);
    void addProblemResult(bool success, const QString &message);
    void updateProblemResult(bool success, const QString &message);
    void deleteProblemResult(bool success, const QString &message);
    void tagsReceived(const QJsonArray &tagsArray);
    void syncTagsResult(bool success, const QString &message);
    void diffsReceived(const QJsonArray &diffsArray);
    void syncDiffsResult(bool success, const QString &message);
    void studentInfoReceived(const QJsonObject &studentInfo);
    void examListReceived(const QJsonArray &exams, const QJsonObject &myRecord);
    void createExamResult(bool success, const QString &message);
    void updateExamResult(bool success, const QString &message);
    void deleteExamResult(bool success, const QString &message);
    void dashboardStatsReceived(const QJsonObject &stats);
    void allRecordsReceived(const QJsonObject &records);

    void updateAvailable(bool isForced, const QString &latestVersion, const QString &releaseNotes);

    // 🚀 OTA 新增：文件下载进度与完成信号
    void updateDownloadProgress(int percent);
    void updateDownloadFinished(bool success);

private slots:
    void onReadyRead();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    void sendJson(const QJsonObject &json);

    // 🚀 OTA 文件句柄
    QFile m_updateFile;
};
#endif // NETWORKMANAGER_H