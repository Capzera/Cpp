#include "SettingsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadSettings();
}

void SettingsWidget::setupUI() {
    QFont normalFont("Microsoft YaHei", 14);
    QFont boldFont("Microsoft YaHei", 14, QFont::Bold);
    QFont titleFont("Microsoft YaHei", 28, QFont::Bold);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // 标题
    QLabel *titleLabel = new QLabel("练习设置", this);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(60);

    // 表单布局
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(30);
    formLayout->setAlignment(Qt::AlignHCenter);

    // 全屏开关
    fullscreenCheckBox = new QCheckBox("开启全屏模式 (保存后立即生效)");
    fullscreenCheckBox->setFont(normalFont);
    formLayout->addWidget(fullscreenCheckBox);

    // 默认开启线槽
    practiceModeCheckBox = new QCheckBox("无尽模式：默认开启「复盘槽位与引导线」");
    practiceModeCheckBox->setFont(normalFont);
    formLayout->addWidget(practiceModeCheckBox);

    // 默认难度
    QHBoxLayout *levelLayout = new QHBoxLayout();
    QLabel *levelLabel = new QLabel("默认出题难度：", this);
    levelLabel->setFont(normalFont);
    levelLayout->addWidget(levelLabel);

    defaultLevelCombo = new QComboBox();
    defaultLevelCombo->addItems({"2位 × 2位", "3位 × 2位", "3位 × 3位", "4位 × 2位", "4位 × 3位", "4位 × 4位"});
    defaultLevelCombo->setFont(normalFont);
    levelLayout->addWidget(defaultLevelCombo);
    levelLayout->addStretch();
    formLayout->addLayout(levelLayout);

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(80);

    // 按钮组
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignHCenter);

    applyButton = new QPushButton("💾 保存设置");
    applyButton->setFont(boldFont);
    applyButton->setFixedSize(160, 50);
    applyButton->setStyleSheet("background-color: #1976d2; color: white; border-radius: 5px;");
    connect(applyButton, &QPushButton::clicked, this, &SettingsWidget::applySettings);

    defaultButton = new QPushButton("🔄 恢复默认");
    defaultButton->setFont(normalFont);
    defaultButton->setFixedSize(160, 50);
    connect(defaultButton, &QPushButton::clicked, this, &SettingsWidget::restoreDefaults);

    clearDataButton = new QPushButton("🗑️ 清除所有记录");
    clearDataButton->setFont(normalFont);
    clearDataButton->setFixedSize(180, 50);
    clearDataButton->setStyleSheet("background-color: #d32f2f; color: white; border-radius: 5px;");
    connect(clearDataButton, &QPushButton::clicked, this, &SettingsWidget::clearLocalData);

    btnLayout->addWidget(applyButton);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(defaultButton);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(clearDataButton);

    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch(); // 往下推

    // 返回按钮
    backButton = new QPushButton("⬅ 返回主菜单");
    backButton->setFont(boldFont);
    backButton->setFixedSize(200, 50);
    connect(backButton, &QPushButton::clicked, this, &SettingsWidget::backToMenuRequested);
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
}

void SettingsWidget::loadSettings() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    fullscreenCheckBox->setChecked(settings.value("Settings_Fullscreen", false).toBool());
    practiceModeCheckBox->setChecked(settings.value("Settings_PracticeDefault", false).toBool());
    defaultLevelCombo->setCurrentIndex(settings.value("Settings_DefaultLevel", 0).toInt());
}

void SettingsWidget::applySettings() {
    QSettings settings("TrachtenbergApp", "SpeedMath");
    settings.setValue("Settings_Fullscreen", fullscreenCheckBox->isChecked());
    settings.setValue("Settings_PracticeDefault", practiceModeCheckBox->isChecked());
    settings.setValue("Settings_DefaultLevel", defaultLevelCombo->currentIndex());

    // 触发全屏信号，让外层的 MainWindow 去执行
    emit fullscreenToggled(fullscreenCheckBox->isChecked());

    QMessageBox::information(this, "设置已保存", "设置已成功保存！");
}

void SettingsWidget::restoreDefaults() {
    fullscreenCheckBox->setChecked(false);
    practiceModeCheckBox->setChecked(false);
    defaultLevelCombo->setCurrentIndex(0);
}

void SettingsWidget::clearLocalData() {
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "危险操作",
                                                             "这将清除排行榜上所有的历史最高分和游玩记录！\n此操作不可逆，确定要清除吗？",
                                                             QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSettings settings("TrachtenbergApp", "SpeedMath");

        // 我们只希望清除分数，所以要把刚才的系统设置先备份出来
        bool fs = settings.value("Settings_Fullscreen", false).toBool();
        bool pd = settings.value("Settings_PracticeDefault", false).toBool();
        int dl = settings.value("Settings_DefaultLevel", 0).toInt();

        settings.clear(); // 核心：暴力清空整个注册表/配置文件里的数据

        // 把系统设置再写回去
        settings.setValue("Settings_Fullscreen", fs);
        settings.setValue("Settings_PracticeDefault", pd);
        settings.setValue("Settings_DefaultLevel", dl);

        QMessageBox::information(this, "清理完成", "本地游戏记录已全部清除，排行榜已重置。");
    }
}