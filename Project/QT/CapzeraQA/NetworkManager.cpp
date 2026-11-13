#include "NetworkManager.h"
#include <QDebug>
#include <QCoreApplication> // 用于获取当前路径

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), tcpSocket(new QTcpSocket(this)) {
    connect(tcpSocket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &NetworkManager::onErrorOccurred);
}
NetworkManager::~NetworkManager() { tcpSocket->disconnectFromHost(); }
void NetworkManager::connectToServer(const QString &ip, quint16 port) { tcpSocket->connectToHost(ip, port); }
void NetworkManager::onConnected() { emit connectedSuccessfully(); }
void NetworkManager::sendJson(const QJsonObject &json) {
    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    data.append('\n');
    tcpSocket->write(data);
}

void NetworkManager::checkUpdate(const QString &currentVersion) { QJsonObject json; json["type"] = "CHECK_VERSION"; json["currentVersion"] = currentVersion; sendJson(json); }

// 🚀 发起文件下载请求
void NetworkManager::requestUpdateFile() {
    QJsonObject json;
    json["type"] = "REQUEST_UPDATE_FILE";
    sendJson(json);
}

void NetworkManager::sendLoginRequest(const QString &user, const QString &pass) { QJsonObject json; json["type"] = "LOGIN"; json["username"] = user; json["password"] = pass; sendJson(json); }
void NetworkManager::sendAnswer(const QString &examId, const QJsonObject &answers) { QJsonObject json; json["type"] = "SUBMIT_EXAM"; json["examId"] = examId; json["answers"] = answers; sendJson(json); }
void NetworkManager::saveExamProgress(const QString &examId, const QJsonObject &answers) { QJsonObject json; json["type"] = "SAVE_ANSWERS"; json["examId"] = examId; json["answers"] = answers; sendJson(json); }
void NetworkManager::requestClassList() { QJsonObject json; json["type"] = "GET_CLASS_LIST"; sendJson(json); }
void NetworkManager::syncClasses(const QJsonArray &classesArray) { QJsonObject json; json["type"] = "SYNC_CLASSES"; json["classes"] = classesArray; sendJson(json); }
void NetworkManager::requestStudentList() { QJsonObject json; json["type"] = "GET_STUDENT_LIST"; sendJson(json); }
void NetworkManager::syncStudents(const QJsonArray &studentsArray) { QJsonObject json; json["type"] = "SYNC_STUDENTS"; json["students"] = studentsArray; sendJson(json); }
void NetworkManager::requestProblemList() { QJsonObject json; json["type"] = "GET_PROBLEM_LIST"; sendJson(json); }
void NetworkManager::submitNewProblem(const QJsonObject &problem) { QJsonObject json; json["type"] = "ADD_PROBLEM"; json["problem"] = problem; sendJson(json); }
void NetworkManager::requestUpdateProblem(const QJsonObject &problem) { QJsonObject json; json["type"] = "UPDATE_PROBLEM"; json["problem"] = problem; sendJson(json); }
void NetworkManager::requestDeleteProblem(const QString &id) { QJsonObject json; json["type"] = "DELETE_PROBLEM"; json["id"] = id; sendJson(json); }
void NetworkManager::requestTags() { QJsonObject json; json["type"] = "GET_TAGS"; sendJson(json); }
void NetworkManager::syncTags(const QJsonArray &tagsArray) { QJsonObject json; json["type"] = "SYNC_TAGS"; json["tags"] = tagsArray; sendJson(json); }
void NetworkManager::requestExamList() { QJsonObject json; json["type"] = "GET_EXAM_LIST"; sendJson(json); }
void NetworkManager::submitNewExam(const QJsonObject &exam) { QJsonObject json; json["type"] = "CREATE_EXAM"; json["exam"] = exam; sendJson(json); }
void NetworkManager::requestUpdateExam(const QJsonObject &exam) { QJsonObject json; json["type"] = "UPDATE_EXAM"; json["exam"] = exam; sendJson(json); }
void NetworkManager::requestDeleteExam(const QString &examId) { QJsonObject json; json["type"] = "DELETE_EXAM"; json["examId"] = examId; sendJson(json); }
void NetworkManager::requestDashboardStats() { QJsonObject json; json["type"] = "REQUEST_DASHBOARD_STATS"; sendJson(json); }
void NetworkManager::updateTeacherProfile(const QString &account, const QString &newName, const QString &newPwd) { QJsonObject json; json["type"] = "UPDATE_PROFILE"; json["account"] = account; json["realName"] = newName; json["password"] = newPwd; sendJson(json); }
void NetworkManager::requestAllRecords() { QJsonObject json; json["type"] = "GET_ALL_RECORDS"; sendJson(json); }

void NetworkManager::onReadyRead() {
    while (tcpSocket->canReadLine()) {
        QByteArray data = tcpSocket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) continue;
        QJsonObject json = doc.object();
        QString type = json["type"].toString();

        if (type == "UPDATE_INFO") {
            if (json["hasUpdate"].toBool()) {
                emit updateAvailable(json["isForced"].toBool(), json["latestVersion"].toString(), json["releaseNotes"].toString());
            }
        }
        // 🚀 OTA 核心协议：接收文件碎片
        else if (type == "DOWNLOAD_CHUNK") {
            int current = json["current"].toInt();
            int total = json["total"].toInt();
            QString base64Data = json["data"].toString();
            QByteArray chunk = QByteArray::fromBase64(base64Data.toUtf8());

            // 如果是第一个包，创建新文件
            if (current == 1) {
                if (m_updateFile.isOpen()) m_updateFile.close();
                QString updatePath = QCoreApplication::applicationDirPath() + "/CapzeraQA_Update.exe";
                m_updateFile.setFileName(updatePath);
                m_updateFile.open(QIODevice::WriteOnly);
            }

            m_updateFile.write(chunk);
            emit updateDownloadProgress((current * 100) / total);

            if (current == total) {
                m_updateFile.close();
                emit updateDownloadFinished(true); // 下载彻底完成！
            } else {
                // 🚀 ACK 握手机制：告诉服务器这块收到了，请发下一块！避免撑爆内存！
                QJsonObject ack;
                ack["type"] = "REQUEST_CHUNK";
                ack["chunkIndex"] = current + 1;
                sendJson(ack);
            }
        }
        else if (type == "LOGIN_RESPONSE") emit loginResult(json["success"].toBool(), json["message"].toString(), json["realName"].toString());
        else if (type == "NEW_EXAM") emit newExamReceived(json);
        else if (type == "QUESTION_RESPONSE") emit questionReceived(json);
        else if (type == "CLASS_LIST_RESPONSE") emit classListReceived(json["classes"].toArray());
        else if (type == "SYNC_CLASSES_RESPONSE") emit syncClassesResult(json["success"].toBool(), json["message"].toString());
        else if (type == "STUDENT_LIST_RESPONSE") emit studentListReceived(json["students"].toArray());
        else if (type == "SYNC_STUDENTS_RESPONSE") emit syncStudentsResult(json["success"].toBool(), json["message"].toString());
        else if (type == "PROBLEM_LIST_RESPONSE") emit problemListReceived(json["problems"].toArray());
        else if (type == "ADD_PROBLEM_RESPONSE") emit addProblemResult(json["success"].toBool(), json["message"].toString());
        else if (type == "UPDATE_PROBLEM_RESPONSE") emit updateProblemResult(json["success"].toBool(), json["message"].toString());
        else if (type == "DELETE_PROBLEM_RESPONSE") emit deleteProblemResult(json["success"].toBool(), json["message"].toString());
        else if (type == "TAG_LIST_RESPONSE") emit tagsReceived(json["tags"].toArray());
        else if (type == "SYNC_TAGS_RESPONSE") emit syncTagsResult(json["success"].toBool(), json["message"].toString());
        else if (type == "EXAM_LIST_RESPONSE") {
            emit examListReceived(json["exams"].toArray(), json["myRecord"].toObject());
            if (json.contains("studentInfo")) emit studentInfoReceived(json["studentInfo"].toObject());
        }
        else if (type == "CREATE_EXAM_RESPONSE") emit createExamResult(json["success"].toBool(), json["message"].toString());
        else if (type == "UPDATE_EXAM_RESPONSE") emit updateExamResult(json["success"].toBool(), json["message"].toString());
        else if (type == "DELETE_EXAM_RESPONSE") emit deleteExamResult(json["success"].toBool(), json["message"].toString());
        else if (type == "DASHBOARD_STATS_RESPONSE") emit dashboardStatsReceived(json);
        else if (type == "ALL_RECORDS_RESPONSE") emit allRecordsReceived(json["records"].toObject());
        else if (type == "UPDATE_PROFILE_RESPONSE") emit teacherProfileUpdated(json["success"].toBool(), json["message"].toString());
        else if (type == "DIFF_LIST_RESPONSE") emit diffsReceived(json["diffs"].toArray());
        else if (type == "SYNC_DIFFS_RESPONSE") emit syncDiffsResult(json["success"].toBool(), json["message"].toString());
        else if (type == "UPDATE_STUDENT_PROFILE_RESPONSE") emit studentProfileUpdated(json["success"].toBool(), json["message"].toString(), json["updatedData"].toObject());
    }
}

void NetworkManager::updateManualScore(const QString &account, const QString &examId, int qIndex, int score) {
    QJsonObject json;
    json["type"] = "UPDATE_MANUAL_SCORE";
    json["account"] = account;
    json["examId"] = examId;
    json["qIndex"] = QString::number(qIndex);
    json["score"] = score;
    sendJson(json);
}

void NetworkManager::requestDiffs() {
    QJsonObject json;
    json["type"] = "GET_DIFFS";
    sendJson(json);
}

void NetworkManager::syncDiffs(const QJsonArray &diffsArray) {
    QJsonObject json;
    json["type"] = "SYNC_DIFFS";
    json["diffs"] = diffsArray;
    sendJson(json);
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "网络错误:" << tcpSocket->errorString();
}

void NetworkManager::updateStudentProfile(const QString &account, const QString &newName, const QString &newPwd, const QString &newMotto, const QString &phone, const QString &email, const QString &gender, const QString &idCard) {
    QJsonObject json;
    json["type"] = "UPDATE_STUDENT_PROFILE";
    json["account"] = account;
    json["newName"] = newName;
    json["newPwd"] = newPwd;
    json["newMotto"] = newMotto;
    json["phone"] = phone;
    json["email"] = email;
    json["gender"] = gender;
    json["idCard"] = idCard;
    sendJson(json);
}