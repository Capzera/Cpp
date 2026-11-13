#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

signals:
    void backToMenuRequested();
    // 向主窗口发送全屏切换信号
    void fullscreenToggled(bool isFullscreen);

private slots:
    void applySettings();
    void restoreDefaults();
    void clearLocalData();

private:
    void setupUI();
    void loadSettings();

    QCheckBox *fullscreenCheckBox;
    QCheckBox *practiceModeCheckBox;
    QComboBox *defaultLevelCombo;

    QPushButton *applyButton;
    QPushButton *defaultButton;
    QPushButton *clearDataButton;
    QPushButton *backButton;
};

#endif // SETTINGSWIDGET_H