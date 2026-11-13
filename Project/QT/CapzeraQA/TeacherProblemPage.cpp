#include <QWebEngineSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebChannel>
#include "TeacherProblemPage.h"
#include "CapzeraDialog.h"

TeacherProblemPage::TeacherProblemPage(QVector<ProblemInfo> *problems, QStringList *cloudTags, NetworkManager *net, double dpi, QObject *parent)
    : QObject(parent), m_problems(problems), m_cloudTags(cloudTags), m_networkManager(net), m_dpi(dpi) {

    // 🚀 彻底抹除 QSettings 单机读取，静默初始化等待云端覆盖
    m_customDiffs.clear();

    // 网络层核心通信信号绑定
    connect(m_networkManager, &NetworkManager::tagsReceived, this, &TeacherProblemPage::onTagsReceived);
    connect(m_networkManager, &NetworkManager::syncTagsResult, this, &TeacherProblemPage::onSyncTagsResult);
    connect(m_networkManager, &NetworkManager::diffsReceived, this, &TeacherProblemPage::onDiffsReceived);           // 🚀 新增
    connect(m_networkManager, &NetworkManager::syncDiffsResult, this, &TeacherProblemPage::onSyncDiffsResult);       // 🚀 新增
    connect(m_networkManager, &NetworkManager::problemListReceived, this, &TeacherProblemPage::onProblemListReceived);
    connect(m_networkManager, &NetworkManager::addProblemResult, this, &TeacherProblemPage::onAddProblemResult);
    connect(m_networkManager, &NetworkManager::updateProblemResult, this, &TeacherProblemPage::onUpdateProblemResult);
    connect(m_networkManager, &NetworkManager::deleteProblemResult, this, &TeacherProblemPage::onDeleteProblemResult);

    m_view = createCardContainer();
    QHBoxLayout *horizontalMaster = new QHBoxLayout(m_view);
    horizontalMaster->setContentsMargins(20, 20, 20, 20);
    horizontalMaster->setSpacing(20);

    initUI();
}

QWidget* TeacherProblemPage::getView() { return m_view; }

void TeacherProblemPage::initUI() {
    QHBoxLayout *horizontalMaster = qobject_cast<QHBoxLayout*>(m_view->layout());
    if (!horizontalMaster) return;

    QFont yaheiBoldFont("Microsoft YaHei", qRound(12 / m_dpi), QFont::Bold);

    // ==================== 左侧面板 ====================
    QWidget *leftPanel = new QWidget(m_view);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0); leftLayout->setSpacing(15);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(12);

    m_searchTagCombo = new CapzeraComboBox(leftPanel);
    m_searchTagCombo->setFixedWidth(qRound(150 * m_dpi));

    m_searchKeywordEdit = new QLineEdit(leftPanel);
    m_searchKeywordEdit->setPlaceholderText("🔍 输入题目名称或编号搜索...");
    m_searchKeywordEdit->setStyleSheet("QLineEdit { border: 2px solid #E2E8F0; border-radius: 8px; padding: 6px 12px; min-height: 38px; font-family:'Microsoft YaHei'; background: #FFFFFF; } QLineEdit:focus { border-color: #3B82F6; }");

    CapzeraButton *btnSearch = new CapzeraButton("筛选题库", CapzeraButton::Primary, leftPanel);
    btnSearch->setFixedSize(qRound(100 * m_dpi), qRound(38 * m_dpi));
    connect(btnSearch, &QPushButton::clicked, this, &TeacherProblemPage::refreshTable);

    CapzeraButton *btnAddNewProb = new CapzeraButton("➕ 新增试题", CapzeraButton::Success, leftPanel);
    btnAddNewProb->setFixedSize(qRound(120 * m_dpi), qRound(38 * m_dpi));
    connect(btnAddNewProb, &QPushButton::clicked, this, &TeacherProblemPage::onAddNewProblemRun);

    searchLayout->addWidget(m_searchTagCombo);
    searchLayout->addWidget(m_searchKeywordEdit, 1);
    searchLayout->addWidget(btnSearch);
    searchLayout->addWidget(btnAddNewProb);
    leftLayout->addLayout(searchLayout);

    m_tableWebView = new QWebEngineView(leftPanel);
    m_tableWebView->setStyleSheet("background: transparent; border-radius: 14px;");

    QWebChannel *tableChannel = new QWebChannel(this);
    tableChannel->registerObject(QStringLiteral("tableBridge"), this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/problem_table.html"));

    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool success) {
        if (success) {
            m_isTableLoaded = true;
            refreshTable();
        }
    });

    leftLayout->addWidget(m_tableWebView, 1);
    horizontalMaster->addWidget(leftPanel, 11);

    // ==================== 右侧面板：双WebView ====================
    QVBoxLayout *rightLayout = new QVBoxLayout(); rightLayout->setSpacing(0);
    m_rightTabWidget = new QTabWidget(m_view); m_rightTabWidget->setFont(yaheiBoldFont);
    m_rightTabWidget->setStyleSheet("QTabWidget::pane { border: 1px solid #E2E8F0; border-radius: 12px; background: #FFFFFF; top: -1px; } QTabBar::tab { background: #F8FAFC; border: 1px solid #E2E8F0; padding: 12px 25px; border-top-left-radius: 10px; border-top-right-radius: 10px; color: #64748B; margin-right: 4px; border-bottom: none; } QTabBar::tab:selected { background: #FFFFFF; color: #2563EB; } QTabBar::tab:hover:!selected { background: #F1F5F9; }");

    // ---- Tab 1: 试题新增 ----
    QWidget *tabEdit = new QWidget();
    QVBoxLayout *tabEditLayout = new QVBoxLayout(tabEdit);
    tabEditLayout->setContentsMargins(0, 0, 0, 0); tabEditLayout->setSpacing(0);

    m_addWebView = new QWebEngineView(tabEdit);
    m_addWebView->setStyleSheet("background: #FFFFFF; border-radius: 10px;");

    QWebChannel *addChannel = new QWebChannel(this);
    addChannel->registerObject(QStringLiteral("problemAddBridge"), this);
    m_addWebView->page()->setWebChannel(addChannel);
    m_addWebView->setUrl(QUrl("qrc:///www/problem_add.html"));

    tabEditLayout->addWidget(m_addWebView);
    m_rightTabWidget->addTab(tabEdit, " 📝 试题新增 ");

    // ---- Tab 2: 试题详情 ----
    QWidget *tabPreview = new QWidget();
    QVBoxLayout *previewLayout = new QVBoxLayout(tabPreview);
    previewLayout->setContentsMargins(0, 0, 0, 0); previewLayout->setSpacing(0);

    m_detailWebView = new QWebEngineView(tabPreview);
    m_detailWebView->setStyleSheet("background: #FAFAFA; border-radius: 10px;");

    QWebChannel *detailChannel = new QWebChannel(this);
    detailChannel->registerObject(QStringLiteral("problemDetailBridge"), this);
    m_detailWebView->page()->setWebChannel(detailChannel);
    m_detailWebView->setUrl(QUrl("qrc:///www/problem_detail.html"));

    QWidget *actionContainer = new QWidget();
    actionContainer->setStyleSheet("background: #FAFAFA;");
    QHBoxLayout *actionLayout = new QHBoxLayout(actionContainer);
    actionLayout->setContentsMargins(25, 10, 25, 25);

    btnEditProb = new CapzeraButton("✏️ 编辑此题", CapzeraButton::Primary, actionContainer);
    btnEditProb->setEnabled(false); btnEditProb->setMinimumHeight(48);
    btnDeleteProb = new CapzeraButton("🗑️ 物理删除", CapzeraButton::Danger, actionContainer);
    btnDeleteProb->setEnabled(false); btnDeleteProb->setMinimumHeight(48);

    connect(btnEditProb, &QPushButton::clicked, this, &TeacherProblemPage::onEditProblemTriggered);
    connect(btnDeleteProb, &QPushButton::clicked, this, &TeacherProblemPage::onDeleteProblemTriggered);

    actionLayout->addStretch();
    actionLayout->addWidget(btnEditProb, 1);
    actionLayout->addWidget(btnDeleteProb, 1);

    previewLayout->addWidget(m_detailWebView, 1);
    previewLayout->addWidget(actionContainer);
    m_rightTabWidget->addTab(tabPreview, " 👁️ 试题详情 ");

    rightLayout->addWidget(m_rightTabWidget, 10);
    horizontalMaster->addLayout(rightLayout, 9);
}

void TeacherProblemPage::fetchCloudData() {
    m_networkManager->requestTags();
    m_networkManager->requestDiffs(); // 🚀 升级：启动时同步请求云端难度列表
    m_networkManager->requestProblemList();
}

void TeacherProblemPage::refreshTable() {
    if (!m_isTableLoaded) return;
    QString targetTag = m_searchTagCombo->currentText();
    if (targetTag.isEmpty()) targetTag = "全部标签";

    QString keyword = m_searchKeywordEdit->text().trimmed();

    QJsonArray filtered;
    for (const auto& prob : *m_problems) {
        if (targetTag != "全部标签" && prob.tag != targetTag) continue;
        if (!keyword.isEmpty() && !prob.title.contains(keyword, Qt::CaseInsensitive) && !prob.id.contains(keyword, Qt::CaseInsensitive)) continue;

        QJsonObject obj;
        obj["id"] = prob.id;
        obj["title"] = prob.title;
        obj["tag"] = prob.tag;
        obj["type"] = prob.type == "single" ? "单选题" : (prob.type == "multi" ? "多选题" : (prob.type == "fill" ? "填空题" : (prob.type == "code_snippet" ? "代码提交题" : "OI题")));
        filtered.append(obj);
    }

    QString jsonString = QString::fromUtf8(QJsonDocument(filtered).toJson(QJsonDocument::Compact));
    QString jsCommand = QString("if(typeof table !== 'undefined') { table.setData(%1); }").arg(jsonString);
    m_tableWebView->page()->runJavaScript(jsCommand);
}

void TeacherProblemPage::resetProblemForm() {
    int maxId = 1000;
    for (const auto& prob : *m_problems) {
        if (prob.id.startsWith("P", Qt::CaseInsensitive)) {
            int num = prob.id.mid(1).toInt();
            if (num > maxId) maxId = num;
        }
    }
    QString newId = QString("P%1").arg(maxId + 1);
    m_isEditingProb = false;

    if (m_isAddLoaded) {
        m_addWebView->page()->runJavaScript(QString("resetForm('%1');").arg(newId));
    } else {
        m_cachedAddId = newId;
    }
}

void TeacherProblemPage::onAddNewProblemRun() {
    m_rightTabWidget->setCurrentIndex(0);
    resetProblemForm();
}

void TeacherProblemPage::onEditProblemTriggered() {
    QString targetId = m_currentSelectedProbId;
    if (targetId.isEmpty()) return;

    for (const auto& prob : *m_problems) {
        if (prob.id == targetId) {
            m_isEditingProb = true;

            QJsonObject payload;
            payload["id"] = prob.id; payload["title"] = prob.title;
            payload["type"] = prob.type; payload["tag"] = prob.tag;
            payload["difficulty"] = prob.difficulty; payload["stem"] = prob.description;

            if (prob.type == "single" || prob.type == "multi") {
                QJsonArray opts; for(const QString &opt : prob.options) opts.append(opt);
                payload["options"] = opts;
                if (prob.type == "single") {
                    payload["answer"] = prob.standardAnswer.toInt();
                } else {
                    payload["answer"] = QJsonArray::fromVariantList(prob.standardAnswer.toList());
                }
            } else if (prob.type == "fill") {
                payload["answer"] = prob.standardAnswer.toString();
            } else if (prob.type == "oi") {
                payload["inputSample"] = prob.inputSample; payload["outputSample"] = prob.outputSample;
            }

            if (m_isAddLoaded) {
                QByteArray b64 = QJsonDocument(payload).toJson(QJsonDocument::Compact).toBase64();
                m_addWebView->page()->runJavaScript(QString("loadFormData('%1');").arg(QString(b64)));
            } else {
                m_cachedEditPayload = payload;
            }
            m_rightTabWidget->setCurrentIndex(0);
            break;
        }
    }
}

void TeacherProblemPage::onDeleteProblemTriggered() {
    QString targetId = m_currentSelectedProbId;
    if (targetId.isEmpty()) return;
    if (CapzeraDialog::question(m_view, "高危操作", "确定要彻底物理删除题号【" + targetId + "】吗？\n删除后该题目将无法从云端找回！")) {
        m_networkManager->requestDeleteProblem(targetId);
    }
}

void TeacherProblemPage::onWebTableRowClicked(const QString &probId) {
    m_currentSelectedProbId = probId;

    for (const auto& prob : *m_problems) {
        if (prob.id == probId) {
            QString typeStr = prob.type == "single" ? "单选题" : (prob.type == "multi" ? "多选题" : (prob.type == "fill" ? "填空题" : (prob.type == "code_snippet" ? "代码提交题" : "OI题")));

            QJsonObject payload;
            payload["id"] = prob.id; payload["title"] = prob.title; payload["type"] = prob.type;
            payload["typeStr"] = typeStr; payload["tag"] = prob.tag; payload["difficulty"] = prob.difficulty;
            payload["stem"] = prob.description;

            if (prob.type == "single" || prob.type == "multi") {
                QJsonArray opts; for(const QString &opt : prob.options) opts.append(opt);
                payload["options"] = opts;
                QString ansStr = "";
                if (prob.type == "single") {
                    ansStr = QString(QChar('A' + prob.standardAnswer.toInt()));
                } else {
                    QVariantList arr = prob.standardAnswer.toList();
                    for (int i = 0; i < arr.size(); ++i) ansStr += QString(QChar('A' + arr[i].toInt())) + " ";
                }
                payload["ansStr"] = ansStr.trimmed();
            } else if (prob.type == "fill") { payload["ansStr"] = prob.standardAnswer.toString();
            } else if (prob.type == "oi") { payload["inputSample"] = prob.inputSample; payload["outputSample"] = prob.outputSample; }

            QByteArray base64Data = QJsonDocument(payload).toJson(QJsonDocument::Compact).toBase64();
            QString jsCommand = QString("renderDetail('%1');").arg(QString(base64Data));

            if (m_isDetailLoaded) { m_detailWebView->page()->runJavaScript(jsCommand);
            } else { m_cachedDetailPayload = payload; }

            QList<CapzeraButton*> buttons = m_detailWebView->parentWidget()->findChildren<CapzeraButton*>();
            for(auto btn : buttons) btn->setEnabled(true);
            m_rightTabWidget->setCurrentIndex(1);
            break;
        }
    }
}

void TeacherProblemPage::onAddWebLoaded() {
    m_isAddLoaded = true;

    QByteArray tagsB64 = QJsonDocument(QJsonArray::fromStringList(*m_cloudTags)).toJson(QJsonDocument::Compact).toBase64();
    m_addWebView->page()->runJavaScript(QString("updateTags('%1');").arg(QString(tagsB64)));

    // 🚀 升级：将当前动态难度的快照灌给前端下拉框
    QByteArray diffsB64 = QJsonDocument(QJsonArray::fromStringList(m_customDiffs)).toJson(QJsonDocument::Compact).toBase64();
    m_addWebView->page()->runJavaScript(QString("updateDiffs('%1');").arg(QString(diffsB64)));

    if (m_isEditingProb && !m_cachedEditPayload.isEmpty()) {
        QByteArray b64 = QJsonDocument(m_cachedEditPayload).toJson(QJsonDocument::Compact).toBase64();
        m_addWebView->page()->runJavaScript(QString("loadFormData('%1');").arg(QString(b64)));
        m_cachedEditPayload = QJsonObject();
    } else {
        if (!m_cachedAddId.isEmpty()) {
            m_addWebView->page()->runJavaScript(QString("resetForm('%1');").arg(m_cachedAddId));
            m_cachedAddId.clear();
        } else {
            resetProblemForm();
        }
    }
}

void TeacherProblemPage::onDetailWebLoaded() {
    m_isDetailLoaded = true;
    if (!m_cachedDetailPayload.isEmpty()) {
        QByteArray base64Data = QJsonDocument(m_cachedDetailPayload).toJson(QJsonDocument::Compact).toBase64();
        m_detailWebView->page()->runJavaScript(QString("renderDetail('%1');").arg(QString(base64Data)));
        m_cachedDetailPayload = QJsonObject();
    }
}

void TeacherProblemPage::openManageTagsFromWeb() {
    onManageTagsClicked();
}

void TeacherProblemPage::onManageTagsClicked() {
    QDialog dialog(m_view); dialog.setWindowTitle("⚙️ 算法标签 management 中心"); dialog.setFixedSize(450, 550);
    dialog.setStyleSheet("QDialog { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    QVBoxLayout *layout = new QVBoxLayout(&dialog); layout->setContentsMargins(25, 25, 25, 25); layout->setSpacing(15);

    QHBoxLayout *addLayout = new QHBoxLayout();
    QLineEdit *tagInput = new QLineEdit(&dialog); tagInput->setPlaceholderText("请输入新的标签名称...");
    tagInput->setStyleSheet("QLineEdit { border: 2px solid #E2E8F0; border-radius: 8px; padding: 10px; background: white; font-size: 15px; color: #1E293B; } QLineEdit:focus { border-color: #3B82F6; }");
    CapzeraButton *btnAdd = new CapzeraButton("➕ 添加", CapzeraButton::Primary, &dialog);
    addLayout->addWidget(tagInput); addLayout->addWidget(btnAdd);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setStyleSheet("QListWidget { border: 2px solid #E2E8F0; border-radius: 8px; background: white; font-size: 15px; padding: 5px; outline: none; } QListWidget::item { padding: 12px; border-bottom: 1px solid #F1F5F9; color: #475569; } QListWidget::item:selected { background: #EFF6FF; color: #2563EB; border-radius: 6px; font-weight: bold; }");
    for (const QString &tag : *m_cloudTags) listWidget->addItem(tag);

    CapzeraButton *btnDel = new CapzeraButton("🗑️ 移除选中的标签", CapzeraButton::Danger, &dialog);
    layout->addLayout(addLayout); layout->addWidget(listWidget); layout->addWidget(btnDel);

    connect(btnAdd, &QPushButton::clicked, [&]() {
        QString newTag = tagInput->text().trimmed(); if (newTag.isEmpty()) return;
        if (m_cloudTags->contains(newTag)) { CapzeraDialog::warning(&dialog, "拒绝添加", "该算法标签已经存在于库中！"); return; }
        listWidget->addItem(newTag); m_cloudTags->append(newTag); tagInput->clear();
    });

    connect(btnDel, &QPushButton::clicked, [&]() {
        QListWidgetItem *item = listWidget->currentItem(); if (!item) return;
        if (CapzeraDialog::question(&dialog, "高危操作", "确认彻底移除标签【" + item->text() + "】吗？\n移除后，使用此标签的历史试题可能失去分类索引！")) {
            m_cloudTags->removeOne(item->text()); delete listWidget->takeItem(listWidget->row(item));
        }
    });

    dialog.exec();

    QJsonArray tagsArr; for (const QString &t : *m_cloudTags) tagsArr.append(t);
    m_networkManager->syncTags(tagsArr);
}

// 🚀 升级：彻底干掉单机 QSettings，改为纯正的分布式云端管理弹窗
void TeacherProblemPage::openManageDiffsFromWeb() {
    QDialog dialog(m_view); dialog.setWindowTitle("⚙️ 难度评级管理中心"); dialog.setFixedSize(450, 550);
    dialog.setStyleSheet("QDialog { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    QVBoxLayout *layout = new QVBoxLayout(&dialog); layout->setContentsMargins(25, 25, 25, 25); layout->setSpacing(15);

    QHBoxLayout *addLayout = new QHBoxLayout();
    QLineEdit *diffInput = new QLineEdit(&dialog); diffInput->setPlaceholderText("请输入新的难度评级...");
    diffInput->setStyleSheet("QLineEdit { border: 2px solid #E2E8F0; border-radius: 8px; padding: 10px; background: white; font-size: 15px; color: #1E293B; } QLineEdit:focus { border-color: #3B82F6; }");
    CapzeraButton *btnAdd = new CapzeraButton("➕ 添加", CapzeraButton::Primary, &dialog);
    addLayout->addWidget(diffInput); addLayout->addWidget(btnAdd);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setStyleSheet("QListWidget { border: 2px solid #E2E8F0; border-radius: 8px; background: white; font-size: 15px; padding: 5px; outline: none; } QListWidget::item { padding: 12px; border-bottom: 1px solid #F1F5F9; color: #475569; } QListWidget::item:selected { background: #EFF6FF; color: #2563EB; border-radius: 6px; font-weight: bold; }");
    for (const QString &diff : m_customDiffs) listWidget->addItem(diff);

    CapzeraButton *btnDel = new CapzeraButton("🗑️ 移除选中的难度", CapzeraButton::Danger, &dialog);
    layout->addLayout(addLayout); layout->addWidget(listWidget); layout->addWidget(btnDel);

    connect(btnAdd, &QPushButton::clicked, [&]() {
        QString newDiff = diffInput->text().trimmed(); if (newDiff.isEmpty()) return;
        if (m_customDiffs.contains(newDiff)) { CapzeraDialog::warning(&dialog, "拦截", "该难度已存在！"); return; }
        listWidget->addItem(newDiff); m_customDiffs.append(newDiff); diffInput->clear();
    });

    connect(btnDel, &QPushButton::clicked, [&]() {
        QListWidgetItem *item = listWidget->currentItem(); if (!item) return;
        if (CapzeraDialog::question(&dialog, "高危操作", "确认移除该难度评级吗？")) {
            m_customDiffs.removeOne(item->text()); delete listWidget->takeItem(listWidget->row(item));
        }
    });

    dialog.exec();

    // 🚀 核心同步动作：将修改后的内存快照打包发送给网络流，请求覆盖云端
    QJsonArray diffsArr; for (const QString &d : m_customDiffs) diffsArr.append(d);
    m_networkManager->syncDiffs(diffsArr);
}

// 🚀 新增：当云端最新的难度数据包安全落地时触发
void TeacherProblemPage::onDiffsReceived(const QJsonArray &diffsArray) {
    m_customDiffs.clear();
    for (int i = 0; i < diffsArray.size(); ++i) {
        m_customDiffs.append(diffsArray[i].toString());
    }

    // 如果前端新增面板已经处于渲染就绪状态，瞬间将其下拉框刷新
    if (m_isAddLoaded) {
        QByteArray diffsB64 = QJsonDocument(diffsArray).toJson(QJsonDocument::Compact).toBase64();
        m_addWebView->page()->runJavaScript(QString("updateDiffs('%1');").arg(QString(diffsB64)));
    }
}

// 🚀 新增：响应云端关于难度同步的操作结果
void TeacherProblemPage::onSyncDiffsResult(bool success, const QString &message) {
    if (success) {
        m_networkManager->requestDiffs(); // 成功后重新拉取全量快照，触发全局广播同步
    } else {
        CapzeraDialog::warning(m_view, "难度同步拦截", message);
    }
}

void TeacherProblemPage::cancelEditFromWeb() { resetProblemForm(); }

void TeacherProblemPage::submitProblemFromWeb(const QString &b64Data) {
    QJsonObject p = QJsonDocument::fromJson(QByteArray::fromBase64(b64Data.toUtf8())).object();
    QJsonObject submission;
    submission["id"] = p["id"]; submission["title"] = p["title"]; submission["type"] = p["type"];
    submission["tag"] = p["tag"]; submission["difficulty"] = p["difficulty"]; submission["description"] = p["description"];
    if (p.contains("options")) submission["options"] = p["options"];
    submission["answer"] = p["answer"];
    if (p.contains("inputSample")) submission["inputSample"] = p["inputSample"];
    if (p.contains("outputSample")) submission["outputSample"] = p["outputSample"];

    if (m_isEditingProb) m_networkManager->requestUpdateProblem(submission);
    else m_networkManager->submitNewProblem(submission);
}

void TeacherProblemPage::onTagsReceived(const QJsonArray &tagsArray) {
    m_cloudTags->clear(); m_searchTagCombo->blockSignals(true); m_searchTagCombo->clear(); m_searchTagCombo->addItem("全部标签");
    for (int i = 0; i < tagsArray.size(); ++i) {
        QString t = tagsArray[i].toString(); m_cloudTags->append(t); m_searchTagCombo->addItem(t);
    }
    m_searchTagCombo->blockSignals(false);

    if (m_isAddLoaded) {
        QByteArray b64 = QJsonDocument(tagsArray).toJson(QJsonDocument::Compact).toBase64();
        m_addWebView->page()->runJavaScript(QString("updateTags('%1');").arg(QString(b64)));
    }
}

void TeacherProblemPage::onSyncTagsResult(bool success, const QString &message) {
    if (success) m_networkManager->requestTags();
    else CapzeraDialog::warning(m_view, "同步失败", message);
}

void TeacherProblemPage::onProblemListReceived(const QJsonArray &problemsArray) {
    m_problems->clear();
    for (int i = 0; i < problemsArray.size(); ++i) {
        QJsonObject obj = problemsArray[i].toObject();
        ProblemInfo p;
        p.id = obj["id"].toString(); p.title = obj["title"].toString(); p.type = obj["type"].toString();
        p.tag = obj["tag"].toString(); p.difficulty = obj["difficulty"].toString(); p.description = obj["description"].toString();
        if (obj.contains("options")) {
            QJsonArray opts = obj["options"].toArray();
            for (int j = 0; j < opts.size(); ++j) p.options.append(opts[j].toString());
        }
        p.standardAnswer = obj["answer"].toVariant();
        p.inputSample = obj["inputSample"].toString(); p.outputSample = obj["outputSample"].toString();
        m_problems->append(p);
    }

    m_cachedTableData = problemsArray;
    refreshTable();
    emit problemBankUpdated();

    if (!m_isEditingProb && m_isAddLoaded) {
        int maxId = 1000;
        for (const auto& prob : *m_problems) {
            if (prob.id.startsWith("P", Qt::CaseInsensitive)) {
                int num = prob.id.mid(1).toInt();
                if (num > maxId) maxId = num;
            }
        }
        QString newId = QString("P%1").arg(maxId + 1);
        m_addWebView->page()->runJavaScript(QString("updateNewId('%1');").arg(newId));
    }
}

void TeacherProblemPage::onAddProblemResult(bool success, const QString &message) {
    if (success) { resetProblemForm(); CapzeraDialog::information(m_view, "入库成功", "新试题已成功存入分布式云端题库！"); fetchCloudData();
    } else CapzeraDialog::warning(m_view, "入库拦截", message);
}

void TeacherProblemPage::onUpdateProblemResult(bool success, const QString &message) {
    if (success) { resetProblemForm(); CapzeraDialog::information(m_view, "更新成功", "试题内容已成功覆盖更新至云端！"); fetchCloudData();
    } else CapzeraDialog::warning(m_view, "更新拦截", message);
}

void TeacherProblemPage::onDeleteProblemResult(bool success, const QString &message) {
    if (success) {
        CapzeraDialog::information(m_view, "物理删除成功", "该试题已从云端数据库中永久抹除。");
        QList<CapzeraButton*> buttons = m_detailWebView->parentWidget()->findChildren<CapzeraButton*>();
        for(auto btn : buttons) btn->setEnabled(false);
        m_currentSelectedProbId = ""; fetchCloudData();
    } else CapzeraDialog::warning(m_view, "删除失败", message);
}

QWidget* TeacherProblemPage::createCardContainer() {
    QWidget *container = new QWidget();
    container->setStyleSheet("QWidget { background-color: #FFFFFF; border-radius: 16px; }");
    return container;
}