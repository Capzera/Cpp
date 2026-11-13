#include "TeacherExamPage.h"
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPageLayout>
#include <QTimer>

TeacherExamPage::TeacherExamPage(QVector<ProblemInfo> *problems, QStringList *classRooms, NetworkManager *net, double dpi, QObject *parent)
    : QObject(parent), m_problems(problems), m_classRooms(classRooms), m_networkManager(net), m_dpi(dpi), m_isEditingExam(false)
{
    connect(m_networkManager, &NetworkManager::examListReceived, this, &TeacherExamPage::onExamListReceived);
    connect(m_networkManager, &NetworkManager::createExamResult, this, &TeacherExamPage::onCreateExamResult);
    connect(m_networkManager, &NetworkManager::updateExamResult, this, &TeacherExamPage::onUpdateExamResult);
    connect(m_networkManager, &NetworkManager::deleteExamResult, this, &TeacherExamPage::onDeleteExamResult);
    connect(m_networkManager, &NetworkManager::problemListReceived, this, [this](const QJsonArray &) {
        refreshBankTable();
    });

    m_view = createCardContainer();
    QVBoxLayout *layout = new QVBoxLayout(m_view);
    layout->setContentsMargins(20, 20, 20, 20); layout->setSpacing(20);

    // 🚀 全局 SaaS 级皮肤引擎注入
    QString saasSkin = R"(
        QWidget { font-family: 'DengXian', 'Microsoft YaHei'; color: #1E293B; }
        QLabel { font-weight: bold; color: #475569; font-size: 15px; border: none; background: transparent; }
        QLineEdit { padding: 10px 14px; border: 2px solid #E2E8F0; border-radius: 8px; background: #F8FAFC; font-size: 15px; color: #1E293B; }
        QLineEdit:focus { border-color: #3B82F6; background: #FFFFFF; }
        QTabWidget::pane { border: 1px solid #E2E8F0; border-radius: 12px; background: #FFFFFF; top: -1px; }
        QTabBar::tab { background: #F8FAFC; color: #64748B; border: 1px solid #E2E8F0; padding: 12px 25px; border-top-left-radius: 10px; border-top-right-radius: 10px; font-weight: bold; font-size: 16px; margin-right: 4px; border-bottom: none; }
        QTabBar::tab:selected { background: #FFFFFF; color: #2563EB; }
    )";
    m_view->setStyleSheet(saasSkin);

    m_examTabWidget = new QTabWidget(m_view);

    // ================== Tab 0: 试卷总览 ==================
    QWidget *overviewTab = new QWidget();
    QVBoxLayout *overviewL = new QVBoxLayout(overviewTab); overviewL->setContentsMargins(20, 20, 20, 20);

    m_overviewWebView = new QWebEngineView(overviewTab);
    m_overviewWebView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_overviewWebView->setStyleSheet("background: transparent; border-radius: 14px;");
    m_overviewWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_overviewWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    QWebChannel *overviewChannel = new QWebChannel(this);
    overviewChannel->registerObject(QStringLiteral("examOverviewBridge"), this);
    m_overviewWebView->page()->setWebChannel(overviewChannel);
    m_overviewWebView->setUrl(QUrl("qrc:///www/exam_overview_table.html"));

    connect(m_overviewWebView, &QWebEngineView::loadFinished, this, [this](bool ok){
        if (ok) { m_isOverviewLoaded = true; if (!m_cachedOverview.isEmpty()) { m_overviewWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(QJsonDocument(m_cachedOverview).toJson(QJsonDocument::Compact).toBase64()))); } }
    });
    overviewL->addWidget(m_overviewWebView);
    m_examTabWidget->addTab(overviewTab, " 📜 已发布试卷大厅 ");

    // ================== Tab 1: 智能组卷面板 ==================
    QWidget *buildTab = new QWidget();
    QHBoxLayout *buildL = new QHBoxLayout(buildTab); buildL->setContentsMargins(20, 20, 20, 20); buildL->setSpacing(25);

    // --- 左侧：题库备选库 ---
    QVBoxLayout *bankL = new QVBoxLayout(); bankL->setSpacing(15);
    QHBoxLayout *filterL = new QHBoxLayout();
    m_searchTagCombo = new CapzeraComboBox(); m_searchTagCombo->addItem("全部标签"); m_searchTagCombo->addItem("基础语法"); m_searchTagCombo->addItem("数据结构");
    m_searchDiffCombo = new CapzeraComboBox(); m_searchDiffCombo->addItems({"全部难度", "入门", "普及-", "普及/提高-", "提高+/省选"});
    m_searchKeywordEdit = new QLineEdit(); m_searchKeywordEdit->setPlaceholderText("搜题号...");
    filterL->addWidget(m_searchTagCombo); filterL->addWidget(m_searchDiffCombo); filterL->addWidget(m_searchKeywordEdit);
    bankL->addLayout(filterL);

    m_bankWebView = new QWebEngineView(buildTab);
    m_bankWebView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_bankWebView->setStyleSheet("background: transparent; border-radius: 12px;");
    m_bankWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    QWebChannel *bankChannel = new QWebChannel(this);
    bankChannel->registerObject(QStringLiteral("examBankBridge"), this);
    m_bankWebView->page()->setWebChannel(bankChannel);
    m_bankWebView->setUrl(QUrl("qrc:///www/exam_bank_table.html"));

    connect(m_bankWebView, &QWebEngineView::loadFinished, this, [this](bool ok){
        if (ok) { m_isBankLoaded = true; if (!m_cachedBank.isEmpty()) m_bankWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(QJsonDocument(m_cachedBank).toJson(QJsonDocument::Compact).toBase64()))); }
    });
    bankL->addWidget(m_bankWebView);

    // --- 右侧：表单 + 组卷篮 ---
    QVBoxLayout *rightL = new QVBoxLayout(); rightL->setSpacing(15);
    QGridLayout *formG = new QGridLayout(); formG->setVerticalSpacing(15); formG->setHorizontalSpacing(15);
    m_examIdEdit = new QLineEdit(); m_examIdEdit->setReadOnly(true); m_examIdEdit->setStyleSheet("QLineEdit{ background:#E2E8F0; color:#64748B; border-radius:8px; padding:10px; font-weight:bold; }");
    m_examTitleEdit = new QLineEdit(); m_examTitleEdit->setPlaceholderText("例如: 2026年夏季普及组第一次模拟赛");
    m_examClassCombo = new CapzeraComboBox(); m_examClassCombo->addItem("全部班级"); m_examClassCombo->addItems(*m_classRooms);

    // 💡 完美保留底层组件
    m_examStartEdit = new DateTimePickerWidget();
    m_examEndEdit = new DateTimePickerWidget();

    formG->addWidget(new QLabel("📝 试卷流水号:"), 0, 0); formG->addWidget(m_examIdEdit, 0, 1);
    formG->addWidget(new QLabel("📌 试卷主标题:"), 1, 0); formG->addWidget(m_examTitleEdit, 1, 1);
    formG->addWidget(new QLabel("🎯 分发班级:"), 2, 0); formG->addWidget(m_examClassCombo, 2, 1);
    formG->addWidget(new QLabel("⏳ 开考时间:"), 3, 0); formG->addWidget(m_examStartEdit, 3, 1);
    formG->addWidget(new QLabel("🛑 截止时间:"), 4, 0); formG->addWidget(m_examEndEdit, 4, 1);



    QLabel *lblShuffle = new QLabel("试题乱序");
    QLabel *lblOptShuffle = new QLabel("选项乱序");
    btnExportPdf = new CapzeraButton("导出PDF", CapzeraButton::Danger);
    connect(btnExportPdf, &QPushButton::clicked, this, &TeacherExamPage::onExportPdfClicked);
    lblShuffle->setStyleSheet("color: #475569; font-weight: bold; font-size: 15px;");
    lblOptShuffle->setStyleSheet("color: #475569; font-weight: bold; font-size: 15px;");
    m_toggleShuffle = new CapzeraToggle(buildTab);
    m_toggleOptionShuffle = new CapzeraToggle(buildTab);
    QHBoxLayout *shuffleL = new QHBoxLayout();
    shuffleL->addWidget(lblShuffle);
    shuffleL->addWidget(m_toggleShuffle);
    QHBoxLayout *shuffleR = new QHBoxLayout();
    shuffleR->addWidget(lblOptShuffle);
    shuffleR->addWidget(m_toggleOptionShuffle);
    shuffleR->addStretch(1);
    shuffleR->addWidget(btnExportPdf);
    shuffleR->addStretch();
    formG->addLayout(shuffleL, 5, 0);
    formG->addLayout(shuffleR, 5, 1);

    rightL->addLayout(formG);

    m_basketWebView = new QWebEngineView(buildTab);
    m_basketWebView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_basketWebView->setStyleSheet("background: transparent; border-radius: 12px;");
    m_basketWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    QWebChannel *basketChannel = new QWebChannel(this);
    basketChannel->registerObject(QStringLiteral("examBasketBridge"), this);
    m_basketWebView->page()->setWebChannel(basketChannel);
    m_basketWebView->setUrl(QUrl("qrc:///www/exam_basket_table.html"));

    connect(m_basketWebView, &QWebEngineView::loadFinished, this, [this](bool ok){
        if (ok) { m_isBasketLoaded = true; if (!m_cachedBasket.isEmpty()) m_basketWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(QJsonDocument(m_cachedBasket).toJson(QJsonDocument::Compact).toBase64()))); }
    });
    rightL->addWidget(m_basketWebView);

    QHBoxLayout *bottomL = new QHBoxLayout();
    QLabel *sLbl = new QLabel("当前卷面总分:"); sLbl->setStyleSheet("font-size:16px; color:#475569;");
    m_lblTotalScore = new QLabel("0"); m_lblTotalScore->setStyleSheet("font-size:26px; font-weight:bold; color:#EF4444; font-family:'Consolas';");
    QLabel *cLbl = new QLabel("已选题目:"); cLbl->setStyleSheet("font-size:16px; color:#475569; margin-left:20px;");
    m_lblProbCount = new QLabel("0"); m_lblProbCount->setStyleSheet("font-size:26px; font-weight:bold; color:#3B82F6; font-family:'Consolas';");

    btnCancelEditAction = new CapzeraButton("✖ 退出编辑", CapzeraButton::Secondary); btnCancelEditAction->hide();
    btnSubmitExamAction = new CapzeraButton("🚀 发布试卷", CapzeraButton::Primary); btnSubmitExamAction->setFixedHeight(45);

    m_btnDeleteExam = new CapzeraButton("🗑️ 删除该测试", CapzeraButton::Danger); m_btnDeleteExam->setFixedSize(qRound(150 * m_dpi), qRound(38 * m_dpi)); m_btnDeleteExam->hide();
    connect(m_btnDeleteExam, &QPushButton::clicked, this, &TeacherExamPage::onDeleteExamClicked);

    bottomL->addWidget(sLbl); bottomL->addWidget(m_lblTotalScore); bottomL->addWidget(cLbl); bottomL->addWidget(m_lblProbCount);
    bottomL->addStretch(); bottomL->addWidget(m_btnDeleteExam); bottomL->addWidget(btnCancelEditAction); bottomL->addWidget(btnSubmitExamAction);
    rightL->addLayout(bottomL);

    buildL->addLayout(bankL, 5); buildL->addLayout(rightL, 5);
    m_examTabWidget->addTab(buildTab, " ⚙️ 智能穿梭组卷系统 ");

    layout->addWidget(m_examTabWidget);

    connect(m_searchTagCombo, &QComboBox::currentTextChanged, this, &TeacherExamPage::refreshBankTable);
    connect(m_searchDiffCombo, &QComboBox::currentTextChanged, this, &TeacherExamPage::refreshBankTable);
    connect(m_searchKeywordEdit, &QLineEdit::textChanged, this, &TeacherExamPage::refreshBankTable);
    connect(btnSubmitExamAction, &QPushButton::clicked, this, &TeacherExamPage::onSubmitNewExam);
    connect(btnCancelEditAction, &QPushButton::clicked, this, &TeacherExamPage::resetExamForm);
    connect(m_examTabWidget, &QTabWidget::currentChanged, this, &TeacherExamPage::onExamTabSwitchClicked);

    resetExamForm();
}

QWidget* TeacherExamPage::getView() {
    return m_view;
}

void TeacherExamPage::fetchCloudData() {
    m_networkManager->requestExamList();
    m_networkManager->requestProblemList();
}

QWidget* TeacherExamPage::createCardContainer() {
    QWidget *card = new QWidget(); card->setStyleSheet("QWidget { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }"); return card;
}

void TeacherExamPage::updateClassFilterCombo() {
    QString cur = m_examClassCombo->currentText();
    m_examClassCombo->blockSignals(true); m_examClassCombo->clear();
    m_examClassCombo->addItem("全部班级"); m_examClassCombo->addItems(*m_classRooms);
    m_examClassCombo->setCurrentText(cur); m_examClassCombo->blockSignals(false);
}

void TeacherExamPage::refreshOverviewTable() {
    if (!m_isOverviewLoaded) return;
    QJsonArray arr;
    for (int i = 0; i < m_cloudExams.size(); ++i) {
        QJsonObject ex = m_cloudExams[i].toObject();
        QJsonObject row;
        row["examId"] = ex["examId"];
        row["title"] = ex["title"];
        row["targetClass"] = ex["targetClass"];
        row["status"] = ex["status"];
        row["qCount"] = ex["questions"].toArray().size();
        row["startTime"] = ex["startTime"];
        row["endTime"] = ex["endTime"];
        arr.append(row);
    }

    QByteArray base64Data = QJsonDocument(arr).toJson(QJsonDocument::Compact).toBase64();
    m_overviewWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(base64Data)));
}

void TeacherExamPage::refreshBankTable() {
    if (!m_isBankLoaded) return;
    QJsonArray arr;
    for (int i = 0; i < m_problems->size(); ++i) {
        ProblemInfo p = m_problems->at(i);
        QJsonObject row;
        row["id"] = p.id;
        row["type"] = p.type;
        row["title"] = p.title;
        row["tag"] = p.tag;
        arr.append(row);
    }

    QByteArray base64Data = QJsonDocument(arr).toJson(QJsonDocument::Compact).toBase64();
    m_bankWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(base64Data)));
}

void TeacherExamPage::refreshBasketTable() {
    QJsonArray arr;
    for (int i = 0; i < m_basketProblems.size(); ++i) {
        const auto& prob = m_basketProblems[i];
        QJsonObject row;
        row["index"] = i;
        row["id"] = prob.id; row["title"] = prob.title;
        // 🚀 从独立的映射中读取分数，默认 10 分
        row["score"] = m_basketScores.value(prob.id, 10);
        row["tag"] = prob.tag;
        row["type"] = prob.type == "single" ? "单选题" : (prob.type == "multi" ? "多选题" : (prob.type == "fill" ? "填空题" : (prob.type == "code_snippet" ? "代码提交题" : "OI题")));
        arr.append(row);
    }
    m_cachedBasket = arr;
    if (m_isBasketLoaded) m_basketWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(QJsonDocument(m_cachedBasket).toJson(QJsonDocument::Compact).toBase64())));
    recalculateTotalScore();
}

void TeacherExamPage::recalculateTotalScore() {
    int total = 0;
    for(const auto& p : m_basketProblems) total += m_basketScores.value(p.id, 10);
    m_lblTotalScore->setText(QString::number(total));
    m_lblProbCount->setText(QString::number(m_basketProblems.size()));
}

void TeacherExamPage::onExamTabSwitchClicked(int index) { if (index == 1) { refreshBankTable(); updateClassFilterCombo(); } }

void TeacherExamPage::onWebBankRowClicked(const QString &probId) {
    for (int i = 0; i < m_cachedBasket.size(); ++i) {
        if (m_cachedBasket[i].toObject()["id"].toString() == probId) {
            return; // 发现重复题号，直接中断函数！
        }
    }
    for (const auto& p : *m_problems) {
        if (p.id == probId) {
            m_basketProblems.append(p);
            // 🚀 如果这道题之前没进过篮子，赋一个默认分
            if (!m_basketScores.contains(p.id)) {
                int defScore = 10;
                if (p.type == "single" || p.type == "multi") defScore = 5;
                m_basketScores[p.id] = defScore;
            }
            refreshBasketTable();
            break;
        }
    }
}


void TeacherExamPage::onExportPdfClicked() {
    if (m_basketProblems.isEmpty()) {
        CapzeraDialog::warning(m_view, "无法导出", "当前组卷篮中没有任何题目，请先添加试题！");
        return;
    }

    // 1. 生成文件名
    QString examTitle = m_examTitleEdit->text().trimmed();
    if (examTitle.isEmpty()) { examTitle = "未命名教研试卷"; }

    QString defaultName = QString("%1_%2.pdf").arg(examTitle).arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmm"));
    QString filePath = QFileDialog::getSaveFileName(m_view, "将当前组卷导出为可打印的 PDF",
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + defaultName,
                                                    "PDF 文件 (*.pdf)");

    if (filePath.isEmpty()) return;

    // 2. 组装考卷 JSON 数据 (精简版，专注渲染)
    QJsonArray qArray;
    for (int i = 0; i < m_basketProblems.size(); ++i) {
        const auto& prob = m_basketProblems[i];
        QJsonObject qObj;
        qObj["stem"] = prob.description;

        if (!prob.options.isEmpty()) {
            QJsonArray opts;
            for (const QString& opt : prob.options) opts.append(opt);
            qObj["options"] = opts;
        }
        qArray.append(qObj);
    }

    QByteArray base64Data = QJsonDocument(qArray).toJson(QJsonDocument::Compact).toBase64();
    QWebEngineView *printView = new QWebEngineView();
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // 🚀 核心修改 1：捕捉 JS 发出的渲染完毕暗号
    connect(printView, &QWebEngineView::titleChanged, m_view, [printView, filePath, layout, this](const QString &title) {
        if (title == "RENDER_COMPLETE") {
            printView->page()->printToPdf(filePath, layout);
        }
    });

    // 🚀 核心修改 2：网页加载完成后，只传数据，不再使用死等定时器
    connect(printView, &QWebEngineView::loadFinished, m_view, [printView, examTitle, base64Data, this](bool success) {
        if (success) {
            QString jsCommand = QString("renderPaper('%1', '%2');").arg(examTitle).arg(QString(base64Data));
            printView->page()->runJavaScript(jsCommand);
        } else {
            CapzeraDialog::critical(m_view, "渲染失败", "离线试卷排版引擎加载失败！");
            printView->deleteLater();
        }
    });

    connect(printView->page(), &QWebEnginePage::pdfPrintingFinished, m_view, [printView, this](const QString &path, bool success) {
        if (success) {
            CapzeraDialog::information(m_view, "试卷导出成功", "PDF 已生成！\n\n文件保存至：\n" + path);
        } else {
            CapzeraDialog::critical(m_view, "导出失败", "PDF 生成失败，请检查文件是否正被其他程序打开占用！");
        }
        printView->deleteLater();
    });

    // 4. 加载独立封装好的排版模板
    printView->setUrl(QUrl("qrc:///www/export_paper.html"));
}


void TeacherExamPage::onDeleteExamClicked() {
    QString examId = m_examIdEdit->text();
    if (examId.isEmpty()) return;
    if (CapzeraDialog::question(m_view, "删除测试确认", "确定要永久删除该测试吗？\n删除后，所有相关的学生答卷和学情数据也将一并清空，且无法恢复！")) {
        m_networkManager->requestDeleteExam(examId);
        resetExamForm();
    }
}

void TeacherExamPage::onWebBasketRowClicked(int index) {
    if (index >= 0 && index < m_basketProblems.size()) { m_basketProblems.removeAt(index); refreshBasketTable(); }
}

void TeacherExamPage::onWebOverviewRowClicked(const QString &examId) {
    for (int i = 0; i < m_cloudExams.size(); ++i) {
        QJsonObject exam = m_cloudExams[i].toObject();
        if (exam["examId"].toString() == examId) {
            m_isEditingExam = true; m_editingExamId = examId;
            m_examIdEdit->setText(examId); m_examTitleEdit->setText(exam["title"].toString());
            m_examClassCombo->setCurrentText(exam["targetClass"].toString());
            m_toggleOptionShuffle->setChecked(exam["isOptionShuffled"].toBool());
            m_basketProblems.clear();
            m_basketScores.clear();
            QJsonArray qs = exam["questions"].toArray();

            for (int j = 0; j < qs.size(); ++j) {
                QJsonObject q = qs[j].toObject(); ProblemInfo p;
                p.id = q["id"].toString(); p.type = q["type"].toString(); p.title = q["title"].toString();
                p.tag = q["tag"].toString(); p.difficulty = q["difficulty"].toString();

                // 🚀 分数单独存进 Map
                m_basketScores[p.id] = q["score"].toInt();

                // 🚀 属性映射矫正：stem -> description, answer -> standardAnswer
                p.description = q["stem"].toString();
                p.standardAnswer = q["answer"].toVariant();

                if(q.contains("inputSample")) p.inputSample = q["inputSample"].toString();
                if(q.contains("outputSample")) p.outputSample = q["outputSample"].toString();
                if(q.contains("options")) {
                    QStringList opts; QJsonArray optArr = q["options"].toArray();
                    for(int k=0; k<optArr.size(); ++k) opts.append(optArr[k].toString());
                    p.options = opts;
                }
                m_basketProblems.append(p);
            }
            refreshBasketTable(); btnSubmitExamAction->setText("💾 保存当前试卷配置"); btnCancelEditAction->show(); m_btnDeleteExam->show();
            m_examTabWidget->setCurrentIndex(1); break;
        }
    }
}

void TeacherExamPage::resetExamForm() {
    m_isEditingExam = false; m_editingExamId = "";
    m_examIdEdit->setText("EXAM_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    m_examTitleEdit->clear(); m_examClassCombo->setCurrentIndex(0);
    m_basketProblems.clear(); m_basketScores.clear(); m_toggleShuffle->setChecked(false); m_toggleOptionShuffle->setChecked(false);
    btnSubmitExamAction->setText("🚀 发布试卷"); btnCancelEditAction->hide(); m_btnDeleteExam->hide();
    refreshBasketTable();
}

void TeacherExamPage::onSubmitNewExam() {
    if (m_examTitleEdit->text().trimmed().isEmpty() || m_basketProblems.isEmpty()) {
        CapzeraDialog::warning(m_view, "配置不完整", "试卷标题不能为空，且组卷篮中至少需包含1道试题！"); return;
    }
    QJsonObject exam;
    exam["examId"] = m_examIdEdit->text(); exam["title"] = m_examTitleEdit->text(); exam["targetClass"] = m_examClassCombo->currentText();
    exam["startTime"] = m_examStartEdit->dateTime().toString("yyyy-MM-dd HH:mm");
    exam["endTime"] = m_examEndEdit->dateTime().toString("yyyy-MM-dd HH:mm");
    exam["status"] = "启用";
    exam["isOptionShuffled"] = m_toggleOptionShuffle->isChecked();

    QJsonArray qs;
    for(const auto& p : m_basketProblems) {
        QJsonObject q;
        q["id"] = p.id; q["type"] = p.type; q["title"] = p.title; q["tag"] = p.tag; q["difficulty"] = p.difficulty;

        // 🚀 组装时从 Map 中重新取出分数
        q["score"] = m_basketScores.value(p.id, 10);

        // 🚀 组装时映射回云端 JSON 字段
        q["stem"] = p.description;
        q["answer"] = QJsonValue::fromVariant(p.standardAnswer);

        if(!p.options.isEmpty()) { QJsonArray opts; for(const auto& o : p.options) opts.append(o); q["options"] = opts; }
        if(!p.inputSample.isEmpty()) q["inputSample"] = p.inputSample;
        if(!p.outputSample.isEmpty()) q["outputSample"] = p.outputSample;
        qs.append(q);
    }
    exam["questions"] = qs;

    btnSubmitExamAction->setEnabled(false); btnSubmitExamAction->setText("🚀 云端同步中...");
    if (m_isEditingExam) m_networkManager->requestUpdateExam(exam); else m_networkManager->submitNewExam(exam);
}

void TeacherExamPage::onExamListReceived(const QJsonArray &exams) { m_cloudExams = exams; refreshOverviewTable(); }
void TeacherExamPage::onCreateExamResult(bool success, const QString &message) {
    btnSubmitExamAction->setEnabled(true); btnSubmitExamAction->setText("🚀 发布试卷");
    if(success) { CapzeraDialog::information(m_view, "成功", message); resetExamForm(); fetchCloudData(); onExamTabSwitchClicked(0); } else CapzeraDialog::warning(m_view, "拦截", message);
}
void TeacherExamPage::onUpdateExamResult(bool success, const QString &message) {
    btnSubmitExamAction->setEnabled(true);
    btnSubmitExamAction->setText("💾 保存当前试卷配置");

    if(success) {
        if (!m_isSilentUpdate) {
            CapzeraDialog::information(m_view, "更新成功", message);
            onExamTabSwitchClicked(0);
        }
        m_isSilentUpdate = false; // 解锁

        resetExamForm();
        fetchCloudData(); // 让前端表格自己刷新，无感过渡
    } else {
        CapzeraDialog::warning(m_view, "拦截", message);
        m_isSilentUpdate = false; // 报错时也要解锁
    }
}

void TeacherExamPage::onDeleteExamResult(bool success, const QString &message) {
    if (success) {
        fetchCloudData();
    } else {
        CapzeraDialog::warning(m_view, "删除失败", message);
    }
}

void TeacherExamPage::webSignalToggleExamStatus(const QString &examId, bool isEnabled) {
    for (int i = 0; i < m_cloudExams.size(); ++i) {
        QJsonObject exam = m_cloudExams[i].toObject();
        if (exam["examId"].toString() == examId) {
            exam["status"] = isEnabled ? "启用" : "停用";
            m_isSilentUpdate = true; // 开启静默锁，防止弹窗打扰
            m_networkManager->requestUpdateExam(exam);
            break;
        }
    }
}

void TeacherExamPage::webSignalUpdateBasketScore(const QString &probId, int newScore) {
    // 1. 更新底层的 m_basketScores Map 解耦系统
    if (m_basketScores.contains(probId)) {
        m_basketScores[probId] = newScore;
    }

    // 2. 更新 m_cachedBasket 缓存数组，防止页面切换时数据又被还原回去
    for (int i = 0; i < m_cachedBasket.size(); ++i) {
        QJsonObject obj = m_cachedBasket[i].toObject();
        if (obj["id"].toString() == probId) {
            obj["score"] = newScore;
            m_cachedBasket[i] = obj;
            break;
        }
    }

    // 3. 重新计算大红字总分！
    recalculateTotalScore();
}