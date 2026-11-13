#ifndef TEACHERSETTINGPAGE_H
#define TEACHERSETTINGPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class TeacherSettingPage : public QObject {
    Q_OBJECT
public:
    explicit TeacherSettingPage(double dpi, QObject *parent = nullptr);
    QWidget* getView();
    void setTeacherName(const QString &name);

signals:
    // 🚀 核心升级：发射真实的资料更新信号（携带密码）
    void profileUpdateRequested(const QString &newName, const QString &newPassword);

private slots:
    void onSaveSettings();

private:
    QWidget *m_view;
    double m_dpi;

    QLineEdit *m_nameEdit;
    QLineEdit *m_passwordEdit;
};

#endif // TEACHERSETTINGPAGE_H