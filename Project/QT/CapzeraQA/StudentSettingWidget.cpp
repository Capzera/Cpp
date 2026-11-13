#include "StudentSettingWidget.h"
#include "CapzeraDialog.h"
#include <QDebug>

StudentSettingWidget::StudentSettingWidget(double dpi, NetworkManager *net, QWidget *parent)
    : QWidget(parent), m_dpi(dpi), m_networkManager(net) {
    initUI();
    connect(m_networkManager, &NetworkManager::studentProfileUpdated, this, &StudentSettingWidget::onProfileUpdated);
}

void StudentSettingWidget::initUI() {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_webView = new QWebEngineView(this);
    m_webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    m_webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    m_webChannel = new QWebChannel(m_webView);
    m_webChannel->registerObject("studentSettingBridge", this);
    m_webView->page()->setWebChannel(m_webChannel);

    connect(m_webView, &QWebEngineView::loadFinished, this, [this](bool ok){
        m_isWebLoaded = ok;
        if (ok && !m_currentInfo.account.isEmpty()) {
            updateStudentInfo(m_currentInfo);
        }
    });

    m_webView->setUrl(QUrl("qrc:///www/student_setting.html"));
    mainLayout->addWidget(m_webView);
}

void StudentSettingWidget::updateStudentInfo(const StudentInfo &info) {
    m_currentInfo = info;
    if (!m_isWebLoaded) return;

    QJsonObject obj;
    obj["account"] = info.account;
    obj["className"] = info.className;
    obj["regTime"] = info.regTime;
    obj["realName"] = info.realName;
    obj["gender"] = info.gender;
    obj["phone"] = info.phone;
    obj["email"] = info.email;
    obj["idCard"] = info.idCard;
    obj["motto"] = info.motto;

    QByteArray jsonBytes = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    QString jsCode = QString("renderProfile(%1);").arg(QString(jsonBytes));
    m_webView->page()->runJavaScript(jsCode);
}

void StudentSettingWidget::webSaveProfile(const QString &name, const QString &gender, const QString &phone, const QString &email, const QString &idCard, const QString &motto) {
    m_networkManager->updateStudentProfile(
        m_currentInfo.account, name, "", motto,
        phone, email, gender, idCard
        );
}

void StudentSettingWidget::webChangePassword(const QString &newPwd) {
    m_networkManager->updateStudentProfile(
        m_currentInfo.account, m_currentInfo.realName, newPwd, m_currentInfo.motto,
        m_currentInfo.phone, m_currentInfo.email, m_currentInfo.gender, m_currentInfo.idCard
        );
}

void StudentSettingWidget::onProfileUpdated(bool success, const QString &message, const QJsonObject &updatedData) {
    if (success) {
        m_currentInfo.realName = updatedData["realName"].toString();
        m_currentInfo.password = updatedData["password"].toString();
        m_currentInfo.motto = updatedData["motto"].toString();
        m_currentInfo.phone = updatedData["phone"].toString();
        m_currentInfo.email = updatedData["email"].toString();
        m_currentInfo.gender = updatedData["gender"].toString();
        m_currentInfo.idCard = updatedData["idCard"].toString();

        updateStudentInfo(m_currentInfo); // 再次用新数据刷一遍 UI，恢复按钮状态
        CapzeraDialog::information(this, "成功", message);
    } else {
        CapzeraDialog::critical(this, "更新失败", message);
        if (m_isWebLoaded) {
            m_webView->page()->runJavaScript("let b=document.getElementById('btnSave'); if(b){b.innerText='💾 保存修改'; b.disabled=false;}");
        }
    }
}