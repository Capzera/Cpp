#ifndef STUDENTSETTINGWIDGET_H
#define STUDENTSETTINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include "CommonTypes.h"
#include "NetworkManager.h"

class StudentSettingWidget : public QWidget {
    Q_OBJECT
public:
    explicit StudentSettingWidget(double dpi, NetworkManager *net, QWidget *parent = nullptr);
    void updateStudentInfo(const StudentInfo &info);

public slots:
    void webSaveProfile(const QString &name, const QString &gender, const QString &phone, const QString &email, const QString &idCard, const QString &motto);
    void webChangePassword(const QString &newPwd);

private slots:
    void onProfileUpdated(bool success, const QString &message, const QJsonObject &updatedData);

private:
    void initUI();
    double m_dpi;
    NetworkManager *m_networkManager;
    StudentInfo m_currentInfo;
    QWebEngineView *m_webView;
    QWebChannel *m_webChannel;
    bool m_isWebLoaded = false;
};

#endif // STUDENTSETTINGWIDGET_H