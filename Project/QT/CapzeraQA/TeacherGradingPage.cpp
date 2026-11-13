#include "TeacherGradingPage.h"
#include "CapzeraDialog.h"
#include <QJsonDocument>
#include <QDir>
#include <QCoreApplication>
#include <algorithm>

TeacherGradingPage::TeacherGradingPage(double dpi, NetworkManager *net, QWidget *parent)
    : QWidget(parent), m_dpi(dpi), m_networkManager(net) {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    initUI();

    connect(m_networkManager, &NetworkManager::examListReceived, this, [this](const QJsonArray &exams, const QJsonObject &) {
        m_cloudExams = exams; refreshTaskList();
    });
    connect(m_networkManager, &NetworkManager::allRecordsReceived, this, [this](const QJsonObject &records) {
        m_allRecords = records; refreshTaskList();
    });
}

void TeacherGradingPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    m_networkManager->requestExamList();
    m_networkManager->requestAllRecords();
}

void TeacherGradingPage::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; margin: 0 5px; width: 2px; }");

    // ================= 左侧：Web 表格任务大厅 =================
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel); leftLayout->setContentsMargins(0, 0, 0, 0);

    m_tableWebView = new QWebEngineView(leftPanel);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *tableChannel = new QWebChannel(m_tableWebView);
    tableChannel->registerObject("gradingTableBridge", this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/grading_table.html"));

    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        m_isTableLoaded = ok;
        if (ok) refreshTaskList();
    });
    leftLayout->addWidget(m_tableWebView, 1);

    // ================= 右侧：批阅工作核心区 =================
    m_middlePanel = new QWidget();
    QVBoxLayout *middleLayout = new QVBoxLayout(m_middlePanel); middleLayout->setContentsMargins(0, 0, 0, 0);

    m_titleLabel = new QLabel("👈 请在左侧选择一份待批阅的任务...");
    m_titleLabel->setStyleSheet("color:#1E293B; font-weight:bold; font-size:18px; margin-bottom: 10px; border:none; background: transparent;");
    middleLayout->addWidget(m_titleLabel);

    QSplitter *vSplitter = new QSplitter(Qt::Vertical, m_middlePanel);
    vSplitter->setHandleWidth(2);
    vSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; }");

    m_stemWebView = new QWebEngineView(vSplitter);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_stemWebView->setUrl(QUrl("qrc:///www/analysis_markdown.html"));
    connect(m_stemWebView, &QWebEngineView::loadFinished, this, [this](bool ok) { m_isStemLoaded = ok; });
    vSplitter->addWidget(m_stemWebView);

    m_ansWebView = new QWebEngineView(vSplitter);
    m_ansWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_ansWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_ansWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_ansWebView->setUrl(QUrl("qrc:///www/analysis_markdown.html"));
    connect(m_ansWebView, &QWebEngineView::loadFinished, this, [this](bool ok) { m_isAnsLoaded = ok; });
    vSplitter->addWidget(m_ansWebView);

    vSplitter->setSizes({400, 400});
    middleLayout->addWidget(vSplitter, 1);

    // ================= 🚀 底部：仿 Web 现代控制台 =================
    QWidget *controlBar = new QWidget();
    controlBar->setStyleSheet("background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; margin-top: 10px;");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(20, 15, 20, 15);

    QLabel *lblScoreTip = new QLabel("✏️ 快速打分：");
    lblScoreTip->setStyleSheet("color: #475569; font-weight: bold; font-size: 16px; border: none; background: transparent;");

    QFrame *scoreBox = new QFrame();
    scoreBox->setStyleSheet("QFrame { border: 2px solid #CBD5E1; border-radius: 2px; background: white; }");
    scoreBox->setFixedSize(180, 60);
    QHBoxLayout *sbL = new QHBoxLayout(scoreBox);
    sbL->setContentsMargins(0, 0, 0, 0);
    sbL->setSpacing(0);

    m_btnMinus = new CapzeraButton("-", CapzeraButton::Primary);
    m_btnMinus->setFixedSize(40, 40);
    m_btnMinus->setFontSize(17);
    connect(m_btnMinus, &QPushButton::clicked, this, &TeacherGradingPage::onMinusClicked);

    m_scoreEdit = new QLineEdit("0");
    m_scoreEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 16px; font-weight: bold; color: #2563EB; }");
    m_scoreEdit->setAlignment(Qt::AlignCenter);

    connect(m_scoreEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        int val = text.toInt();
        if (val > m_currentMaxScore) m_scoreEdit->setText(QString::number(m_currentMaxScore));
        if (val < 0) m_scoreEdit->setText("0");
    });

    m_btnPlus = new CapzeraButton("+", CapzeraButton::Primary);
    m_btnPlus->setFixedSize(40, 40);
    m_btnPlus->setFontSize(17);
    connect(m_btnPlus, &QPushButton::clicked, this, &TeacherGradingPage::onPlusClicked);

    sbL->addWidget(m_btnMinus);
    sbL->addWidget(m_scoreEdit, 1);
    sbL->addWidget(m_btnPlus);

    m_btnSubmit = new CapzeraButton("✅ 提交得分", CapzeraButton::Primary);
    m_btnSubmit->setFixedSize(220, 45);
    connect(m_btnSubmit, &QPushButton::clicked, this, &TeacherGradingPage::onSubmitGrade);

    controlLayout->addWidget(lblScoreTip);
    controlLayout->addWidget(scoreBox);
    controlLayout->addStretch();
    controlLayout->addWidget(m_btnSubmit);

    middleLayout->addWidget(controlBar, 0);

    m_middlePanel->hide();

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(m_middlePanel);
    mainSplitter->setSizes(QList<int>() << 500 << 800);
    mainLayout->addWidget(mainSplitter);
}

void TeacherGradingPage::updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students) {
    m_cloudExams = exams;
    m_allRecords = allRecords;
    m_students = students;
    if (m_isTableLoaded) {
        refreshTaskList();
    }
}

void TeacherGradingPage::refreshTaskList() {
    m_tasks.clear();
    QMap<QString, QString> validExams;

    for (int e = 0; e < m_cloudExams.size(); ++e) {
        QJsonObject exam = m_cloudExams[e].toObject();
        QString examId = exam["examId"].toString();
        QString examTitle = exam["title"].toString();
        QJsonArray questions = exam["questions"].toArray();

        for (const auto &stu : m_students) {
            QString account = stu.account;
            QString name = stu.realName.isEmpty() ? account : stu.realName;

            if (m_allRecords.contains(account)) {
                QJsonObject uRec = m_allRecords[account].toObject();
                if (uRec.contains(examId) && uRec[examId].toObject()["status"].toString() == "SUBMITTED") {
                    QJsonObject rec = uRec[examId].toObject();
                    QJsonValue msVal = rec["manual_scores"];
                    QJsonObject msObj;
                    QJsonArray msArr;
                    bool isMsArray = msVal.isArray();

                    if (isMsArray) msArr = msVal.toArray();
                    else msObj = msVal.toObject();

                    for (int q = 0; q < questions.size(); ++q) {
                        QJsonObject qObj = questions[q].toObject();
                        QString type = qObj["type"].toString();

                        if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
                            GradingTask task;
                            task.account = account;
                            task.name = name;
                            task.examId = examId;
                            task.examTitle = examTitle;
                            task.qIndex = q;
                            task.type = type;
                            task.maxScore = qObj["score"].toInt();
                            task.updateTime = rec["updateTime"].toString();
                            QString qKey = QString::number(q);
                            if (isMsArray) {
                                task.isGraded = (q >= 0 && q < msArr.size() && !msArr[q].isNull() && !msArr[q].isUndefined());
                                task.currentScore = task.isGraded ? msArr[q].toInt() : 0;
                            } else {
                                task.isGraded = msObj.contains(qKey);
                                task.currentScore = task.isGraded ? msObj[qKey].toInt() : 0;
                            }
                            if (task.isGraded) continue;

                            m_tasks.append(task);
                            validExams.insert(examId, examTitle);
                        }
                    }
                }
            }
        }
    }

    std::sort(m_tasks.begin(), m_tasks.end(), [](const GradingTask &a, const GradingTask &b) {
        if (a.isGraded != b.isGraded) return !a.isGraded;
        if (a.examId != b.examId) return a.examId > b.examId;
        return a.account < b.account;
    });

    syncWebCombo(validExams);
    renderTableData();
}

void TeacherGradingPage::syncWebCombo(const QMap<QString, QString>& validExams) {
    QJsonArray arr;
    QJsonObject allObj;
    allObj["id"] = "ALL";
    allObj["title"] = "全部批阅任务";
    arr.append(allObj);

    for (auto it = validExams.begin(); it != validExams.end(); ++it) {
        QJsonObject obj;
        obj["id"] = it.key();
        obj["title"] = it.value();
        arr.append(obj);
    }
    QByteArray bytes = QJsonDocument(arr).toJson(QJsonDocument::Compact).toBase64();
    if (m_isTableLoaded) {
        m_tableWebView->page()->runJavaScript(QString("loadExamCombo('%1');").arg(QString(bytes)));
    }
}

// 🚀 Web 下拉框回传的筛选信号
void TeacherGradingPage::onExamFilterChanged(const QString &examId) {
    m_currentFilterExamId = examId;
    renderTableData();
}

// 🚀 根据筛选状态渲染表格
void TeacherGradingPage::renderTableData() {
    QJsonArray tableData;
    for (int i = 0; i < m_tasks.size(); ++i) {
        const auto &task = m_tasks[i];

        // 核心：基于前端传回的 examId 进行剔除，注意保留 absolute index！
        if (m_currentFilterExamId != "ALL" && task.examId != m_currentFilterExamId) {
            continue;
        }

        QJsonObject rowObj;
        rowObj["examTitle"] = task.examTitle;
        rowObj["name"] = task.name;
        rowObj["qIndex"] = QString("第 %1 题").arg(task.qIndex + 1);
        rowObj["status"] = task.isGraded ? "已批阅" : "待批阅";
        rowObj["score"] = task.isGraded ? QString("%1 / %2").arg(task.currentScore).arg(task.maxScore) : QString("-- / %1").arg(task.maxScore);
        rowObj["_index"] = i;
        tableData.append(rowObj);
    }

    if (m_isTableLoaded) {
        QByteArray bytes = QJsonDocument(tableData).toJson(QJsonDocument::Compact).toBase64();
        m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(bytes)));
    }

    m_middlePanel->hide();
    m_currentTaskIndex = -1;
}

void TeacherGradingPage::onWebRowClicked(int index) {
    if (index < 0 || index >= m_tasks.size()) return;
    m_currentTaskIndex = index;
    GradingTask task = m_tasks[index];

    m_titleLabel->setText(QString("正在批阅：%1 - %2 (第 %3 题)").arg(task.examTitle).arg(task.name).arg(task.qIndex + 1));

    QJsonObject examObj;
    for (int i = 0; i < m_cloudExams.size(); ++i) {
        if (m_cloudExams[i].toObject()["examId"].toString() == task.examId) {
            examObj = m_cloudExams[i].toObject();
            break;
        }
    }
    QJsonObject qObj = examObj["questions"].toArray()[task.qIndex].toObject();

    QJsonObject stemPayload;
    QString stemMd = qObj.contains("description") ? qObj["description"].toString() : qObj["stem"].toString();
    stemMd = QString("\n\n(%1 分)  ").arg(task.maxScore) + stemMd;
    if (task.type == "oi" || task.type == "code" || task.type == "code_completion" || task.type == "code_snippet") {
        stemMd += QString("\n\n#### 📥 输入样例\n```text\n%1\n```\n#### 📤 输出样例\n```text\n%2\n```\n").arg(qObj["inputSample"].toString()).arg(qObj["outputSample"].toString());
    }
    stemMd += QString("\n\n---\n### 🎯 官方参考代码 / 题解：\n```cpp\n%1\n```").arg(qObj["answer"].toString());
    stemPayload["md"] = stemMd;
    QByteArray stemB64 = QJsonDocument(stemPayload).toJson(QJsonDocument::Compact).toBase64();
    if (m_isStemLoaded) m_stemWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(stemB64)));

    QJsonObject ansPayload;
    QString stuAns = "// ⚠ 该生未提交任何代码";
    if (m_allRecords.contains(task.account)) {
        QJsonObject uRec = m_allRecords[task.account].toObject()[task.examId].toObject();
        if (uRec["answers"].toObject().contains(QString::number(task.qIndex))) {
            stuAns = uRec["answers"].toObject()[QString::number(task.qIndex)].toString();
        }
    }
    QString ansMd = QString("### 💻 学员提交的代码：\n```cpp\n%1\n```").arg(stuAns);
    ansPayload["md"] = ansMd;
    QByteArray ansB64 = QJsonDocument(ansPayload).toJson(QJsonDocument::Compact).toBase64();
    if (m_isAnsLoaded) m_ansWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(ansB64)));

    // 🚀 解锁打分面板状态
    m_currentMaxScore = task.maxScore;
    m_scoreEdit->setText(QString::number(task.currentScore));

    m_middlePanel->show();
}

// 🚀 自定义步进器逻辑
void TeacherGradingPage::onMinusClicked() {
    int current = m_scoreEdit->text().toInt();
    if (current > 0) m_scoreEdit->setText(QString::number(current - 1));
}

void TeacherGradingPage::onPlusClicked() {
    int current = m_scoreEdit->text().toInt();
    if (current < m_currentMaxScore) m_scoreEdit->setText(QString::number(current + 1));
}

void TeacherGradingPage::onSubmitGrade() {
    if (m_currentTaskIndex < 0 || m_currentTaskIndex >= m_tasks.size()) return;
    GradingTask task = m_tasks[m_currentTaskIndex];
    int score = m_scoreEdit->text().toInt();

    m_networkManager->updateManualScore(task.account, task.examId, task.qIndex, score);

    QJsonObject uRec = m_allRecords[task.account].toObject();
    QJsonObject examRec = uRec[task.examId].toObject();

    QJsonValue msVal = examRec["manual_scores"];
    if (msVal.isArray()) {
        QJsonArray msArr = msVal.toArray();
        while (msArr.size() <= task.qIndex) msArr.append(QJsonValue::Null);
        msArr[task.qIndex] = score;
        examRec["manual_scores"] = msArr;
    } else {
        QJsonObject msObj = msVal.toObject();
        msObj[QString::number(task.qIndex)] = score;
        examRec["manual_scores"] = msObj;
    }

    uRec[task.examId] = examRec;
    m_allRecords[task.account] = uRec;

    CapzeraDialog::information(this, "批阅成功", QString("【%1】的代码已批阅：%2 分！").arg(task.name).arg(score));

    m_middlePanel->hide();
    m_currentTaskIndex = -1;
    refreshTaskList();
}