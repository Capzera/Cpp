#include "AnalysisTestWidget.h"
#include <QHeaderView>
#include <QJsonDocument>
#include <QWebEngineSettings>
#include <QDir>
#include <QCoreApplication>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QPageLayout>
#include "CapzeraDialog.h"

AnalysisTestWidget::AnalysisTestWidget(QWidget *parent) : QWidget(parent), m_currentQuestionIndex(-1) {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    initUI();
}

void AnalysisTestWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; margin: 0 5px; width: 2px; }");

    // ================= 左侧 (原生下拉框已彻底被 Web 吞并) =================
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel); leftLayout->setContentsMargins(0,0,0,0);

    m_tableWebView = new QWebEngineView(leftPanel);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *tableChannel = new QWebChannel(m_tableWebView);
    tableChannel->registerObject("analysisTestTableBridge", this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/analysis_test_table.html"));

    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        m_isTableLoaded = ok;
        if (ok && m_cloudExams.size() > 0) {
            syncWebCombo();
            onExamIndexChanged(0);
        }
    });
    leftLayout->addWidget(m_tableWebView, 1);

    // ================= 中间：试卷展示区 (Splitter双擎架构) =================
    m_middlePanel = new QWidget();
    QVBoxLayout *middleLayout = new QVBoxLayout(m_middlePanel); middleLayout->setContentsMargins(0,0,0,0);
    m_paperTitleLabel = new QLabel("请在左侧选择一位学员...");
    m_paperTitleLabel->setStyleSheet("color:#1E293B; font-weight:bold; font-size:18px; margin-bottom: 10px; border:none; background: transparent;");
    middleLayout->addWidget(m_paperTitleLabel);

    QSplitter *vSplitter = new QSplitter(Qt::Vertical, m_middlePanel);
    vSplitter->setHandleWidth(2);
    vSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; }");

    // 1. 题干区
    m_stemWebView = new QWebEngineView(vSplitter);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_stemWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_stemWebView->setUrl(QUrl("qrc:///www/analysis_markdown.html"));
    connect(m_stemWebView, &QWebEngineView::loadFinished, this, [this](bool ok) { m_isStemLoaded = ok; });
    vSplitter->addWidget(m_stemWebView);

    // 2. 答案区 (Stacked)
    QWidget *lowerHalf = new QWidget();
    QVBoxLayout *lowerLayout = new QVBoxLayout(lowerHalf);
    lowerLayout->setContentsMargins(0, 10, 0, 0);

    m_bottomStack = new QStackedWidget();
    m_optionsWebView = new QWebEngineView();
    m_optionsWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_optionsWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_optionsWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_optionsWebView->setUrl(QUrl("qrc:///www/analysis_markdown.html"));
    connect(m_optionsWebView, &QWebEngineView::loadFinished, this, [this](bool ok) { m_isOptionsLoaded = ok; });
    m_bottomStack->addWidget(m_optionsWebView);

    m_codeWebView = new QWebEngineView();
    m_codeWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_codeWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_codeWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_codeWebView->setUrl(QUrl("qrc:///www/student_code_editor.html"));
    m_bottomStack->addWidget(m_codeWebView);

    m_answerContainer = new QWidget();
    m_answerLayout = new QVBoxLayout(m_answerContainer);
    m_answerLayout->setContentsMargins(10, 10, 10, 10);
    m_answerContainer->setStyleSheet("background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px;");
    m_bottomStack->addWidget(m_answerContainer);

    lowerLayout->addWidget(m_bottomStack, 1);
    m_feedbackWidget = new QWidget();
    m_feedbackWidget->setLayout(new QVBoxLayout());
    m_feedbackWidget->layout()->setContentsMargins(0, 5, 0, 0);
    lowerLayout->addWidget(m_feedbackWidget, 0);

    vSplitter->addWidget(lowerHalf);
    vSplitter->setSizes({400, 400});
    middleLayout->addWidget(vSplitter, 1);
    m_middlePanel->hide();

    // ================= 右侧：导航面板 =================
    QWidget *rightPanel = new QWidget();
    rightPanel->setFixedWidth(280);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel); rightLayout->setContentsMargins(15, 0, 0, 0);

    m_btnExportPdf = new CapzeraButton("📄 导出当前学生卷面", CapzeraButton::Secondary);
    m_btnExportPdf->setFixedHeight(40);
    rightLayout->addWidget(m_btnExportPdf);

    QLabel *navTitle = new QLabel("📑 试卷导航");
    navTitle->setStyleSheet("color:#1E293B; font-weight:bold; font-size:16px; margin-top: 15px; border:none; background: transparent;");
    rightLayout->addWidget(navTitle);

    m_navGridWidget = new QListWidget();
    m_navGridWidget->setViewMode(QListView::IconMode);
    m_navGridWidget->setResizeMode(QListView::Adjust);
    m_navGridWidget->setSpacing(8);
    m_navGridWidget->setMovement(QListView::Static);
    m_navGridWidget->setStyleSheet("QListWidget { background: transparent; border: none; outline: none; } QListWidget::item { background: transparent; outline: none; }");
    rightLayout->addWidget(m_navGridWidget, 1);

    QLabel *legend = new QLabel("🟢 正确  🔴 错误  🟡 半对\n🔵 主观题  ⚪ 未答");
    legend->setStyleSheet("color: #64748B; font-size:13px; line-height: 1.5; border:none; background: transparent;"); legend->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(legend);

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(m_middlePanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes(QList<int>() << 350 << 600 << 280);
    mainLayout->addWidget(mainSplitter);
}

void AnalysisTestWidget::updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students) {
    m_cloudExams = exams;
    m_allRecords = allRecords;
    m_students = students;

    if (m_isTableLoaded) {
        syncWebCombo();
        if (m_cloudExams.size() > 0) onExamIndexChanged(0);
    }
}

// 🚀 核心：同步更新 Web 下拉框
void AnalysisTestWidget::syncWebCombo() {
    QJsonArray titles;
    for (int i = 0; i < m_cloudExams.size(); ++i) titles.append(m_cloudExams[i].toObject()["title"].toString());
    QByteArray bytes = QJsonDocument(titles).toJson(QJsonDocument::Compact).toBase64();
    m_tableWebView->page()->runJavaScript(QString("loadExamCombo('%1');").arg(QString(bytes)));
}

int AnalysisTestWidget::calculateDynamicScore(const QJsonArray &questions, const QJsonObject &answers, const QJsonObject &manualScores) {
    int score = 0;
    for (int i = 0; i < questions.size(); ++i) {
        QJsonObject q = questions[i].toObject();
        QString type = q["type"].toString();
        QString k = QString::number(i);

        if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
            if (manualScores.contains(k)) score += manualScores[k].toInt();
            continue;
        }

        if (!answers.contains(k)) continue;

        if (type == "single" && answers[k].toInt() == q["answer"].toInt()) score += q["score"].toInt();
        else if (type == "multi" && answers[k].toArray() == q["answer"].toArray()) score += q["score"].toInt();
        else if (type == "fill") {
            if (answers[k].isString() && answers[k].toString() == q["answer"].toString()) score += q["score"].toInt();
            else if (answers[k].isArray() && answers[k].toArray() == q["answer"].toArray()) score += q["score"].toInt();
        }
    }
    return score;
}
void AnalysisTestWidget::onExamIndexChanged(int index) {
    if (index < 0 || index >= m_cloudExams.size()) return;
    m_currentExamIndex = index; // 锁定内存

    QJsonObject examsObj = m_cloudExams[index].toObject();
    QString examId = examsObj["examId"].toString();
    QString targetClass = examsObj["targetClass"].toString();
    m_currentQuestions = examsObj["questions"].toArray();
    m_currentStudentAnswers = QJsonObject();

    buildNavGrid();

    QJsonArray tableData;
    for (int i = 0; i < m_students.size(); ++i) {
        QString u = m_students[i].account;
        QString name = m_students[i].realName.isEmpty() ? u : m_students[i].realName;
        QString cName = m_students[i].className;

        if (targetClass != "全部班级" && cName != targetClass) continue;

        QJsonObject rowObj;
        rowObj["account"] = u;
        rowObj["name"] = name;

        if (m_allRecords.contains(u) && m_allRecords[u].toObject().contains(examId)) {
            QJsonObject rec = m_allRecords[u].toObject()[examId].toObject();
            QJsonObject manualScores = rec["manual_scores"].toObject();
            int score = calculateDynamicScore(m_currentQuestions, rec["answers"].toObject(), manualScores);
            QString statusStr = "批阅完毕";

            if (rec["status"].toString() == "SUBMITTED") {
                statusStr = "待批阅";
                bool hasSubjective = false;
                bool isFullyGraded = true;

                for (int q = 0; q < m_currentQuestions.size(); ++q) {
                    QString t = m_currentQuestions[q].toObject()["type"].toString();
                    if (t == "oi" || t == "code" || t == "code_completion" || t == "code_snippet") {
                        hasSubjective = true;
                        if (!manualScores.contains(QString::number(q))) {
                            isFullyGraded = false;
                        }
                    }
                }
                if (!hasSubjective || isFullyGraded) statusStr = "已完成";

                rowObj["score"] = QString::number(score);
                rowObj["status"] = statusStr;
                rowObj["time"] = rec["updateTime"].toString().mid(5, 11);
            } else if (rec["status"].toString() == "SAVED") {
                rowObj["score"] = "--";
                rowObj["status"] = "答题中";
                rowObj["time"] = "--";
            } else {
                rowObj["score"] = "--";
                rowObj["status"] = "未参加";
                rowObj["time"] = "--";
            }
        } else {
            rowObj["score"] = "--";
            rowObj["status"] = "未参加";
            rowObj["time"] = "--";
        }
        tableData.append(rowObj);
    }

    if (m_isTableLoaded) {
        QByteArray bytes = QJsonDocument(tableData).toJson(QJsonDocument::Compact).toBase64();
        m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(bytes)));
    }

    m_middlePanel->hide();
    m_paperTitleLabel->setText("请在左侧选择一位学员...");
}

void AnalysisTestWidget::onWebRowClicked(const QString &account) {
    m_currentAccount = account;
    if (m_currentExamIndex < 0 || m_currentExamIndex >= m_cloudExams.size()) return;
    QString examId = m_cloudExams[m_currentExamIndex].toObject()["examId"].toString();

    QString studentName = account;
    for (const auto& s : m_students) {
        if (s.account == account) { studentName = s.realName.isEmpty() ? account : s.realName; break; }
    }
    m_paperTitleLabel->setText(QString("%1 的答卷").arg(studentName));

    if (!m_allRecords.contains(account) || !m_allRecords[account].toObject().contains(examId)) {
        m_currentStudentAnswers = QJsonObject();
        m_middlePanel->hide();
        updateNavGrid();
        return;
    }

    QJsonObject rec = m_allRecords[account].toObject()[examId].toObject();
    m_currentStudentAnswers = rec["answers"].toObject();

    updateNavGrid();
    m_middlePanel->show();
    if (m_currentQuestions.size() > 0) loadQuestion(0);
}

void AnalysisTestWidget::buildNavGrid() {
    m_navGridWidget->clear(); m_navButtons.clear();
    for (int i = 0; i < m_currentQuestions.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(m_navGridWidget);
        item->setSizeHint(QSize(44, 44));
        CapzeraButton *btn = new CapzeraButton(QString::number(i + 1), CapzeraButton::Secondary);
        btn->setFontSize(12);
        btn->setFixedSize(44, 44);
        connect(btn, &QPushButton::clicked, this, [this, i]() { loadQuestion(i); });
        m_navGridWidget->setItemWidget(item, btn);
        m_navButtons.append(btn);
    }
    updateNavGrid();
}

void AnalysisTestWidget::updateNavGrid() {
    for (int i = 0; i < m_navButtons.size(); ++i) {
        m_navButtons[i]->setStyleSheet(m_navButtons[i]->styleSheet().replace("border: 2px solid #1E293B;", "border: none;"));
        if (i == m_currentQuestionIndex) m_navButtons[i]->setStyleSheet(m_navButtons[i]->styleSheet() + "QPushButton { border: 2px solid #1E293B; }");

        QString ansKey = QString::number(i);
        if (!m_currentStudentAnswers.contains(ansKey)) { m_navButtons[i]->setTheme(CapzeraButton::Secondary); continue; }

        QJsonObject q = m_currentQuestions[i].toObject();
        QString type = q["type"].toString();

        if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
            bool isGraded = false;
            if (m_allRecords.contains(m_currentAccount)) {
                QJsonObject examRec = m_allRecords[m_currentAccount].toObject()[m_cloudExams[m_currentExamIndex].toObject()["examId"].toString()].toObject();
                if (examRec["manual_scores"].toObject().contains(ansKey)) isGraded = true;
            }
            if (isGraded) m_navButtons[i]->setTheme(CapzeraButton::Success);
            else m_navButtons[i]->setTheme(CapzeraButton::Primary);
        } else {
            bool isRight = false; bool isHalfRight = false;
            if (type == "single") isRight = (m_currentStudentAnswers[ansKey].toInt() == q["answer"].toInt());
            else if (type == "fill") {
                if (m_currentStudentAnswers[ansKey].isString()) isRight = (m_currentStudentAnswers[ansKey].toString() == q["answer"].toString());
                else isRight = (m_currentStudentAnswers[ansKey].toArray() == q["answer"].toArray());
            } else if (type == "multi") {
                QJsonArray rightArr = q["answer"].toArray(); QJsonArray curArr = m_currentStudentAnswers[ansKey].toArray();
                isRight = (curArr == rightArr);
                if (!isRight && !curArr.isEmpty()) {
                    isHalfRight = true;
                    for (int c = 0; c < curArr.size(); ++c) {
                        bool found = false;
                        for (int r = 0; r < rightArr.size(); ++r) { if (curArr[c].toInt() == rightArr[r].toInt()) { found = true; break; } }
                        if (!found) { isHalfRight = false; break; }
                    }
                }
            }
            if (isRight) m_navButtons[i]->setTheme(CapzeraButton::Success);
            else if (isHalfRight) m_navButtons[i]->setTheme(CapzeraButton::Warning);
            else m_navButtons[i]->setTheme(CapzeraButton::Danger);
        }
    }
}

void AnalysisTestWidget::loadQuestion(int index) {
    if (index < 0 || index >= m_currentQuestions.size()) return;
    m_currentQuestionIndex = index;
    updateNavGrid();

    QJsonObject q = m_currentQuestions[index].toObject();
    QString type = q["type"].toString();
    QString ansKey = QString::number(index);

    QJsonObject stemPayload;
    QString md = q.contains("description") ? q["description"].toString() : q["stem"].toString();
    stemPayload["md"] = QString("\n\n(%1 分)  ").arg(q["score"].toInt()) + md;

    if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
        stemPayload["md"] = stemPayload["md"].toString() + QString("\n\n#### 📥 输入样例\n```text\n%1\n```\n#### 📤 输出样例\n```text\n%2\n```\n").arg(q["inputSample"].toString()).arg(q["outputSample"].toString());
    }
    QByteArray stemB64 = QJsonDocument(stemPayload).toJson(QJsonDocument::Compact).toBase64();
    if (m_isStemLoaded) m_stemWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(stemB64)));

    QLayoutItem *child;
    while ((child = m_feedbackWidget->layout()->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    while ((child = m_answerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QString stdAnsStr, stuAnsStr;
    if (type == "single") stdAnsStr = QString(QChar('A' + q["answer"].toInt()));
    else if (type == "multi") {
        QJsonArray aArr = q["answer"].toArray(); QStringList sl;
        for(int i = 0; i < aArr.size(); ++i) sl << QString(QChar('A' + aArr[i].toInt()));
        stdAnsStr = sl.join(", ");
    } else if (type == "fill") stdAnsStr = q["answer"].toString();
    else stdAnsStr = "主观题/代码题，请参考详细解析";

    if (m_currentStudentAnswers.contains(ansKey)) {
        QJsonValue val = m_currentStudentAnswers[ansKey];
        if (type == "single") stuAnsStr = QString(QChar('A' + val.toInt()));
        else if (type == "multi") {
            QJsonArray aArr = val.toArray(); QStringList sl;
            for(int i = 0; i < aArr.size(); ++i) sl << QString(QChar('A' + aArr[i].toInt()));
            stuAnsStr = sl.join(", ");
        } else if (type == "fill") {
            if (val.isArray()) {
                QJsonArray aArr = val.toArray(); QStringList sl;
                for(int i = 0; i < aArr.size(); ++i) sl << aArr[i].toString();
                stuAnsStr = sl.join(", ");
            } else stuAnsStr = val.toString();
        } else stuAnsStr = "已提交代码";
    } else stuAnsStr = "未作答";

    if (type == "single" || type == "multi") {
        m_bottomStack->setCurrentWidget(m_optionsWebView);

        QJsonObject optPayload;
        optPayload["type"] = type;
        QJsonArray optsArr;
        QJsonArray qOptions = q["options"].toArray();

        QJsonArray stdAnsArr;
        if (type == "single") stdAnsArr.append(q["answer"].toInt());
        else stdAnsArr = q["answer"].toArray();

        QJsonArray stuAnsArr;
        if (m_currentStudentAnswers.contains(ansKey)) {
            if (type == "single") stuAnsArr.append(m_currentStudentAnswers[ansKey].toInt());
            else stuAnsArr = m_currentStudentAnswers[ansKey].toArray();
        }

        for (int i = 0; i < qOptions.size(); ++i) {
            QJsonObject optObj;
            optObj["id"] = i;
            optObj["displayId"] = QString(QChar('A' + i));
            optObj["md"] = qOptions[i].toString();

            bool isStd = stdAnsArr.contains(i);
            bool isStu = stuAnsArr.contains(i);

            if (isStu && isStd) optObj["status"] = "selected-correct";
            else if (isStu && !isStd) optObj["status"] = "selected-wrong";
            else if (!isStu && isStd) optObj["status"] = "correct-missed";
            else optObj["status"] = "normal";

            optsArr.append(optObj);
        }
        optPayload["options"] = optsArr;

        if (m_isOptionsLoaded) {
            QByteArray optB64 = QJsonDocument(optPayload).toJson(QJsonDocument::Compact).toBase64();
            m_optionsWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(optB64)));
        }

    } else if (type == "code" || type == "oi" || type == "code_completion" || type == "code_snippet") {
        m_bottomStack->setCurrentWidget(m_codeWebView);
        QString existingCode = "```cpp\n未提交代码\n```";
        if (m_currentStudentAnswers.contains(ansKey)) {
            existingCode = m_currentStudentAnswers[ansKey].toString();
            if (!existingCode.startsWith("```")) existingCode = "```cpp\n" + existingCode + "\n```";
        }
        QByteArray b64Code = existingCode.toUtf8().toBase64();
        m_codeWebView->page()->runJavaScript(QString("initEditor(%1, '%2');").arg(index).arg(QString(b64Code)));

    } else if (type == "fill") {
        m_bottomStack->setCurrentWidget(m_answerContainer);
        int fillCount = q["options"].toArray().size();
        if(fillCount == 0) fillCount = 1;
        QJsonArray userAnsArr;
        if (m_currentStudentAnswers.contains(ansKey)) userAnsArr = m_currentStudentAnswers[ansKey].toArray();

        for (int i=0; i<fillCount; ++i) {
            QHBoxLayout *h = new QHBoxLayout();
            QLabel *l = new QLabel(QString("✍ 填空 %1:").arg(i+1));
            l->setStyleSheet("color:#475569; font-weight:bold;");
            QLineEdit *le = new QLineEdit();
            le->setReadOnly(true);
            le->setText(q["answer"].toString());
            le->setStyleSheet("padding: 8px; border: 1px solid #CBD5E1; border-radius: 4px; background: #F1F5F9; color:#1E293B;");
            if (i < userAnsArr.size()) le->setText(userAnsArr[i].toString());
            h->addWidget(l); h->addWidget(le);
            m_answerLayout->addLayout(h);
        }
        m_answerLayout->addStretch();
    }

    QFrame *feedbackBox = new QFrame();
    QVBoxLayout *fbL = new QVBoxLayout(feedbackBox); fbL->setContentsMargins(15, 15, 15, 15);
    feedbackBox->setStyleSheet("QFrame { background: #FFFBEB; border: 1px solid #F59E0B; border-radius: 8px; }");
    QLabel *lblAns = new QLabel(QString("<b>标准答案：</b><span style='color:#059669;'>%1</span> &nbsp;&nbsp;&nbsp;&nbsp; <b>学员答案：</b><span style='color:#475569;'>%2</span>").arg(stdAnsStr).arg(stuAnsStr));
    lblAns->setStyleSheet("color:#B45309; font-size:15px; border:none; background: transparent;");
    lblAns->setWordWrap(true);
    fbL->addWidget(lblAns);
    m_feedbackWidget->layout()->addWidget(feedbackBox);
}