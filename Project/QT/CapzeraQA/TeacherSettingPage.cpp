#include "TeacherSettingPage.h"
#include "CapzeraButton.h"

TeacherSettingPage::TeacherSettingPage(double dpi, QObject *parent)
    : QObject(parent), m_dpi(dpi)
{
    m_view = new QWidget();
    m_view->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(m_view);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    QWidget *centerCard = new QWidget();
    centerCard->setFixedWidth(450);
    centerCard->setStyleSheet("QWidget#CenterCard { background: white; border: 1px solid #E2E8F0; border-radius: 16px; }");
    centerCard->setObjectName("CenterCard");

    QVBoxLayout *cardLayout = new QVBoxLayout(centerCard);
    cardLayout->setContentsMargins(40, 50, 40, 50);
    cardLayout->setSpacing(35);

    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->setSpacing(10);

    QLabel *avatar = new QLabel("👨‍🏫");
    avatar->setFixedSize(90, 90);
    avatar->setStyleSheet("background: #EFF6FF; border: 4px solid #DBEAFE; border-radius: 45px; font-size: 45px;");
    avatar->setAlignment(Qt::AlignCenter);

    QLabel *tipTitle = new QLabel("我的个人资料");
    tipTitle->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    tipTitle->setStyleSheet("color: #1E293B; border: none;");
    tipTitle->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(avatar, 0, Qt::AlignCenter);
    headerLayout->addWidget(tipTitle);
    cardLayout->addLayout(headerLayout);

    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(20);

    QString labelQss = "color: #64748B; font-weight: bold; font-size: 14px;";
    QString editQss = "QLineEdit { padding: 12px 15px; border: 2px solid #E2E8F0; border-radius: 8px; background: #F8FAFC; font-size: 15px; color: #1E293B; } QLineEdit:focus{ border-color: #3B82F6; background: #FFFFFF; }";

    QVBoxLayout *nameL = new QVBoxLayout(); nameL->setSpacing(8);
    QLabel *lblName = new QLabel("公开显示名称"); lblName->setStyleSheet(labelQss);
    m_nameEdit = new QLineEdit(); m_nameEdit->setStyleSheet(editQss); m_nameEdit->setPlaceholderText("请输入您的姓名或昵称");
    nameL->addWidget(lblName); nameL->addWidget(m_nameEdit);
    formLayout->addLayout(nameL);

    QVBoxLayout *pwdL = new QVBoxLayout(); pwdL->setSpacing(8);
    QLabel *lblPwd = new QLabel("登录安全密码"); lblPwd->setStyleSheet(labelQss);
    m_passwordEdit = new QLineEdit(); m_passwordEdit->setStyleSheet(editQss);
    m_passwordEdit->setPlaceholderText("如需修改密码请填写，否则留空");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    pwdL->addWidget(lblPwd); pwdL->addWidget(m_passwordEdit);
    formLayout->addLayout(pwdL);

    cardLayout->addLayout(formLayout);

    CapzeraButton *saveBtn = new CapzeraButton("保存资料修改", CapzeraButton::Primary);
    saveBtn->setFixedHeight(48);
    cardLayout->addWidget(saveBtn);

    mainLayout->addStretch();
    mainLayout->addWidget(centerCard, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(saveBtn, &QPushButton::clicked, this, &TeacherSettingPage::onSaveSettings);
}

QWidget* TeacherSettingPage::getView() { return m_view; }

void TeacherSettingPage::setTeacherName(const QString &name) {
    if (name.isEmpty() || !m_nameEdit) return;
    m_nameEdit->setText(name);
}

void TeacherSettingPage::onSaveSettings() {
    QString newName = m_nameEdit->text().trimmed();
    QString newPwd = m_passwordEdit->text().trimmed();
    emit profileUpdateRequested(newName, newPwd);
}