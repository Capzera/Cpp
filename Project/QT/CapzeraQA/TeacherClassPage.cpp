#include "TeacherClassPage.h"
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include "CapzeraButton.h"
#include "CapzeraDialog.h"

TeacherClassPage::TeacherClassPage(QStringList *classRooms, QVector<StudentInfo> *students, double dpi, QObject *parent)
    : QObject(parent), m_classRooms(classRooms), m_students(students), m_dpi(dpi), m_selectedClassName(""), m_isTableLoaded(false) {

    m_overviewView = createCardContainer();
    m_manageView = createCardContainer();
    m_addView = createCardContainer();

    // ================= 1. 构建【班级总览】 =================
    QVBoxLayout *overviewL = new QVBoxLayout(m_overviewView);
    overviewL->setContentsMargins(40, 40, 40, 40);
    overviewL->setSpacing(30);
    QLabel *lblTitle = new QLabel("📊 班级运营总览", m_overviewView);
    lblTitle->setFont(QFont("Microsoft YaHei", qRound(18 / m_dpi), QFont::Bold));
    lblTitle->setStyleSheet("color: #1E293B; border:none;");
    overviewL->addWidget(lblTitle);

    QHBoxLayout *cardsRow = new QHBoxLayout();
    auto createMetricCard = [this](const QString & title, const QString & color, QLabel* &lblTarget, QWidget * parent) -> QWidget* {
        QWidget *card = new QWidget(parent);
        card->setStyleSheet("QWidget { background: #F8FAFC; border: 1px solid #E2E8F0; border-radius: 16px; }");
        QVBoxLayout *v = new QVBoxLayout(card); v->setContentsMargins(20, 20, 20, 20);
        QLabel *t = new QLabel(title, card); t->setStyleSheet("color: #64748B; font-weight:bold; font-size:14px; border:none;");
        lblTarget = new QLabel("0", card); lblTarget->setFont(QFont("Consolas", qRound(28 / m_dpi), QFont::Bold));
        lblTarget->setStyleSheet(QString("color: %1; border:none;").arg(color));
        v->addWidget(t); v->addWidget(lblTarget);
        return card;
    };
    cardsRow->addWidget(createMetricCard("机构活跃班级总数", "#3B82F6", lblTotalClasses, m_overviewView));
    cardsRow->addWidget(createMetricCard("在册受训学员总数", "#10B981", lblTotalStudents, m_overviewView));
    cardsRow->addWidget(createMetricCard("平均标准班额大小", "#8B5CF6", lblAvgClassSize, m_overviewView));
    cardsRow->addWidget(createMetricCard("待分班新生雷达数", "#F59E0B", lblUnassignedCount, m_overviewView));
    overviewL->addLayout(cardsRow);
    overviewL->addStretch();

    // ================= 2. 构建【班级管理】(100% 满屏高度贴合版) =================
    QHBoxLayout *manageL = new QHBoxLayout(m_manageView);
    manageL->setContentsMargins(20, 20, 20, 20);
    manageL->setSpacing(20);

    // --- 左侧：高定离线 Web 数据网格 ---
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);

    m_tableWebView = new QWebEngineView(m_manageView);
    m_tableWebView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableWebView->setStyleSheet("background: transparent;");
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    QWebChannel *tableChannel = new QWebChannel(this);
    tableChannel->registerObject(QStringLiteral("classTableBridge"), this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/class_table.html"));

    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool ok){
        if (ok) {
            m_isTableLoaded = true;
            if (!m_cachedTableData.isEmpty()) {
                QString b64 = QJsonDocument(m_cachedTableData).toJson(QJsonDocument::Compact).toBase64();
                m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(b64));
            }
        }
    });
    leftLayout->addWidget(m_tableWebView);

    // --- 右侧：原班级控制中枢面板 ---
    QWidget *rightPanel = new QWidget(m_manageView);
    rightPanel->setFixedWidth(320);
    rightPanel->setStyleSheet("QWidget { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }");
    QVBoxLayout *rightL = new QVBoxLayout(rightPanel);
    rightL->setContentsMargins(20, 25, 20, 25); rightL->setSpacing(15);

    m_detailTitleLabel = new QLabel("👈 请先在左侧选择班级", rightPanel);
    m_detailTitleLabel->setFont(QFont("Microsoft YaHei", qRound(16 / m_dpi), QFont::Bold));
    m_detailTitleLabel->setStyleSheet("color: #1E293B; border:none;");
    m_detailCountLabel = new QLabel("班级学员总数：-- 人", rightPanel);
    m_detailCountLabel->setStyleSheet("color: #64748B; font-size:14px; border:none;");

    m_transferStudentCombo = new CapzeraComboBox(rightPanel);
    m_transferTargetCombo = new CapzeraComboBox(rightPanel);
    btnTransfer = new CapzeraButton("⚡ 确认将该生跨班转移", CapzeraButton::Primary, rightPanel);
    btnRemove = new CapzeraButton("❌ 从该班级开除学员", CapzeraButton::Secondary, rightPanel);
    btnMergeClass = new CapzeraButton("💥 启动合班数据迁移引擎", CapzeraButton::Danger, rightPanel);

    rightL->addWidget(m_detailTitleLabel); rightL->addWidget(m_detailCountLabel);
    QFrame *splitLine = new QFrame(rightPanel); splitLine->setFrameShape(QFrame::HLine); splitLine->setStyleSheet("color:#E2E8F0;"); rightL->addWidget(splitLine);
    QLabel *lblT1 = new QLabel("选择班内学员：");
    lblT1->setStyleSheet("color: #475569; font-weight: bold; font-size: 14px; border:none;");
    rightL->addWidget(lblT1); rightL->addWidget(m_transferStudentCombo);

    QLabel *lblT2 = new QLabel("目标调度班级：");
    lblT2->setStyleSheet("color: #475569; font-weight: bold; font-size: 14px; border:none;");
    rightL->addWidget(lblT2); rightL->addWidget(m_transferTargetCombo);
    rightL->addWidget(btnTransfer); rightL->addWidget(btnRemove);
    rightL->addStretch(); rightL->addWidget(btnMergeClass);

    manageL->addLayout(leftLayout, 7); manageL->addWidget(rightPanel, 3);

    // 状态禁用兜底
    btnTransfer->setEnabled(false); btnRemove->setEnabled(false);

    connect(btnTransfer, &QPushButton::clicked, this, &TeacherClassPage::onTransferStudent);
    connect(btnRemove, &QPushButton::clicked, this, &TeacherClassPage::onRemoveStudentFromClass);
    connect(btnMergeClass, &QPushButton::clicked, this, &TeacherClassPage::onMergeClassAction);

    // ================= 3. 构建【新增班级】 =================
    QVBoxLayout *addL = new QVBoxLayout(m_addView);
    addL->setContentsMargins(60, 60, 60, 60); addL->setSpacing(25);
    QLabel *addTitle = new QLabel("➕ 建立全新信奥教学班级", m_addView);
    addTitle->setFont(QFont("Microsoft YaHei", qRound(18 / m_dpi), QFont::Bold)); addTitle->setStyleSheet("color: #1E293B; border:none;");
    addL->addWidget(addTitle);

    m_newClassEdit = new QLineEdit(m_addView); m_newClassEdit->setPlaceholderText("在此输入全新班级的标识名称 (例如：2026C++提高组模拟班)..."); m_newClassEdit->setMinimumHeight(45);
    m_newClassEdit->setStyleSheet("QLineEdit { padding: 0 15px; border: 2px solid #E2E8F0; border-radius: 8px; font-size: 15px; background: white; } QLineEdit:focus { border-color: #3B82F6; }");
    CapzeraButton *btnCreateClassRun = new CapzeraButton("🚀 批准并创建该班级架构", CapzeraButton::Primary, m_addView); btnCreateClassRun->setFixedSize(220, 45);

    QHBoxLayout *addHL = new QHBoxLayout(); addHL->addWidget(m_newClassEdit); addHL->addWidget(btnCreateClassRun);
    addL->addLayout(addHL); addL->addStretch();
    connect(btnCreateClassRun, &QPushButton::clicked, this, &TeacherClassPage::onAddNewClassRun);
}

QWidget* TeacherClassPage::getOverviewPage() { return m_overviewView; }
QWidget* TeacherClassPage::getManagePage() { return m_manageView; }
QWidget* TeacherClassPage::getAddPage() { return m_addView; }

QWidget* TeacherClassPage::createCardContainer() {
    QWidget *card = new QWidget(); card->setStyleSheet("QWidget { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }"); return card;
}

void TeacherClassPage::refreshOverviewStats() {
    if (!m_classRooms || !m_students) return;
    int cSize = m_classRooms->size(); int sSize = m_students->size();
    lblTotalClasses->setText(QString::number(cSize) + " 个"); lblTotalStudents->setText(QString::number(sSize) + " 人");
    double avg = cSize > 0 ? (double)sSize / cSize : 0.0; lblAvgClassSize->setText(QString::number(avg, 'f', 1) + " 人/班");
    int unassigned = 0; for (const auto &s : *m_students) { if (s.className == "未分班" || s.className.isEmpty()) unassigned++; }
    lblUnassignedCount->setText(QString::number(unassigned) + " 人");
}

void TeacherClassPage::refreshTable() {
    QJsonArray tableData;
    for (const QString &cName : *m_classRooms) {
        int count = 0; for (const auto &s : *m_students) { if (s.className == cName) count++; }
        QJsonObject row; row["className"] = cName; row["studentCount"] = count; tableData.append(row);
    }
    m_cachedTableData = tableData;
    if (m_isTableLoaded) {
        QString b64 = QJsonDocument(m_cachedTableData).toJson(QJsonDocument::Compact).toBase64();
        m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(b64));
    }
    m_selectedClassName = ""; m_detailTitleLabel->setText("👈 请先在左侧选择班级"); m_detailCountLabel->setText("班级学员总数：-- 人");
    m_transferStudentCombo->clear(); btnTransfer->setEnabled(false); btnRemove->setEnabled(false);
    refreshOverviewStats();
}

void TeacherClassPage::onWebRowClicked(const QString &className) {
    m_selectedClassName = className;
    refreshClassDetailPanel();
}

void TeacherClassPage::refreshClassDetailPanel() {
    if (m_selectedClassName.isEmpty()) return;
    m_detailTitleLabel->setText("🏫 班级： " + m_selectedClassName);

    m_transferStudentCombo->blockSignals(true); m_transferStudentCombo->clear();
    int count = 0;
    for (const auto &s : *m_students) {
        if (s.className == m_selectedClassName) {
            m_transferStudentCombo->addItem(s.username + " (" + s.account + ")", s.account); count++;
        }
    }
    m_transferStudentCombo->blockSignals(false);
    m_detailCountLabel->setText(QString("班级学员总数： %1 人").arg(count));

    m_transferTargetCombo->blockSignals(true); m_transferTargetCombo->clear();
    for (const auto &c : *m_classRooms) { if (c != m_selectedClassName) m_transferTargetCombo->addItem(c); }
    m_transferTargetCombo->blockSignals(false);

    bool hasStudents = (count > 0);
    btnTransfer->setEnabled(hasStudents); btnRemove->setEnabled(hasStudents);
}

void TeacherClassPage::onAddNewClassRun() {
    QString n = m_newClassEdit->text().trimmed();
    if (n.isEmpty()) return;
    if (m_classRooms->contains(n)) { CapzeraDialog::warning(m_addView, "拒绝", "该班级名称在系统中已存在，请勿重复建立！"); return; }
    m_classRooms->append(n); m_newClassEdit->clear();
    emit classListUpdated(); refreshTable();
    CapzeraDialog::information(m_addView, "创建成功", "新班级物理空间和架构树生成完毕！");
}

void TeacherClassPage::onTransferStudent() {
    QString studentAccount = m_transferStudentCombo->currentData().toString();
    QString targetClass = m_transferTargetCombo->currentText();
    if (studentAccount.isEmpty() || targetClass.isEmpty()) return;
    for (int i = 0; i < m_students->size(); ++i) {
        if ((*m_students)[i].account == studentAccount) { (*m_students)[i].className = targetClass; break; }
    }
    refreshTable(); emit studentListUpdated();
    CapzeraDialog::information(m_manageView, "调度成功", "该学员已成功被转移至目标班级。");
}

void TeacherClassPage::onRemoveStudentFromClass() {
    QString studentAccount = m_transferStudentCombo->currentData().toString();
    if (studentAccount.isEmpty()) return;
    if (CapzeraDialog::question(m_manageView, "移除确认", "确定要把该学生从当前班级移除，并贬为【未分班】留档状态吗？")) {
        for (int i = 0; i < m_students->size(); ++i) {
            if ((*m_students)[i].account == studentAccount) { (*m_students)[i].className = "未分班"; break; }
        }
        refreshTable(); emit studentListUpdated();
    }
}

void TeacherClassPage::onMergeClassAction() {
    QDialog dlg; dlg.setWindowTitle("机构合班迁移引擎"); dlg.setFixedSize(400, 300); dlg.setStyleSheet("QDialog { background: white; }");
    QVBoxLayout *vL = new QVBoxLayout(&dlg); vL->setContentsMargins(25, 25, 25, 25);
    QLabel *lblSource = new QLabel("🔮 选择源融出班级 (迁移后清空)："); CapzeraComboBox *comboSource = new CapzeraComboBox(); comboSource->addItems(*m_classRooms);
    QLabel *lblTarget = new QLabel("🎯 选择目标融入班级 (接收学生)："); CapzeraComboBox *comboTarget = new CapzeraComboBox(); comboTarget->addItems(*m_classRooms);
    vL->addWidget(lblSource); vL->addWidget(comboSource); vL->addWidget(lblTarget); vL->addWidget(comboTarget); vL->addStretch();
    QHBoxLayout *hL = new QHBoxLayout(); CapzeraButton *btnCancel = new CapzeraButton("✖ 取消", CapzeraButton::Secondary, &dlg); CapzeraButton *btnConfirm = new CapzeraButton("🚀 确认合并", CapzeraButton::Danger, &dlg);
    hL->addStretch(); hL->addWidget(btnCancel); hL->addWidget(btnConfirm); vL->addLayout(hL);
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnConfirm, &QPushButton::clicked, [&]() {
        QString src = comboSource->currentText(); QString tgt = comboTarget->currentText();
        if (src == tgt) { CapzeraDialog::warning(&dlg, "逻辑拦截", "融出与融入班级不能为同一架构！"); return; }
        if (CapzeraDialog::question(&dlg, "高危数据变动确认", QString("确定要将【%1】的所有在校学生强行打散合并入【%2】吗？").arg(src, tgt))) {
            for (int i = 0; i < m_students->size(); ++i) { if ((*m_students)[i].className == src) (*m_students)[i].className = tgt; }
            m_classRooms->removeAll(src); dlg.accept(); emit classListUpdated(); emit studentListUpdated(); refreshTable();
        }
    });
    dlg.exec();
}