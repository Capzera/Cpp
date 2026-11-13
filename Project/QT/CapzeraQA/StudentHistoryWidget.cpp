#include "StudentHistoryWidget.h"
#include <QHeaderView>
#include "CapzeraButton.h"

StudentHistoryWidget::StudentHistoryWidget(double dpi, QWidget *parent)
    : QWidget(parent), m_dpi(dpi), m_currentHistoryAnswerWidget(nullptr) {
    initUI();
}

void StudentHistoryWidget::initUI() {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; }");
    QVBoxLayout *mainL = new QVBoxLayout(this);
    mainL->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topL = new QHBoxLayout();
    CapzeraButton *btnBack = new CapzeraButton("⬅ 返回大厅", CapzeraButton::Secondary);
    btnBack->setFixedSize(140, 42);
    connect(btnBack, &QPushButton::clicked, this, &StudentHistoryWidget::backToDashboardRequested);

    QLabel *title = new QLabel(" 📜 我的历史战绩与错题本");
    title->setFont(QFont("Microsoft YaHei", qRound(20/m_dpi), QFont::Bold));
    title->setStyleSheet("color:#1E293B; border:none;");
    topL->addWidget(btnBack); topL->addWidget(title); topL->addStretch();
    mainL->addLayout(topL);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; width: 4px; }");

    QWidget *leftP = new QWidget();
    QVBoxLayout *leftL = new QVBoxLayout(leftP); leftL->setContentsMargins(0, 10, 0, 0);

    // 🚀 这里替换了你的原生表格，挂载 WebEngine
    m_historyTableWebView = new QWebEngineView();
    m_historyTableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_historyTableWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *channel = new QWebChannel(m_historyTableWebView);
    channel->registerObject("studentHistoryBridge", this);
    m_historyTableWebView->page()->setWebChannel(channel);

    connect(m_historyTableWebView, &QWebEngineView::loadFinished, this, [this](bool ok){
        m_isTableLoaded = ok;
        if(ok && !m_cachedTableData.isEmpty()) {
            QByteArray jsonBytes = QJsonDocument(m_cachedTableData).toJson(QJsonDocument::Compact).toBase64();
            m_historyTableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(jsonBytes)));
        }
    });

    m_historyTableWebView->setUrl(QUrl("qrc:///www/student_history_table.html"));
    leftL->addWidget(m_historyTableWebView);
    mainSplitter->addWidget(leftP);

    // 🚀 这里修复了昨天丢失的层级嵌套
    m_historyDetailPanel = new QWidget();
    QHBoxLayout *detailL = new QHBoxLayout(m_historyDetailPanel);
    detailL->setContentsMargins(15, 0, 0, 0);

    QSplitter *innerSplitter = new QSplitter(Qt::Horizontal);
    innerSplitter->setStyleSheet("QSplitter::handle { background-color: transparent; width: 10px; }");

    QWidget *centerP = new QWidget();
    QVBoxLayout *centerL = new QVBoxLayout(centerP);
    centerL->setContentsMargins(0, 0, 0, 0);

    m_historyTitleLabel = new QLabel("👈 请在左侧选择一份已完成的答卷");
    m_historyTitleLabel->setFont(QFont("HYWenHei-85W", qRound(18/m_dpi), QFont::Bold));
    m_historyTitleLabel->setStyleSheet("color:#1E293B; border:none; margin-bottom: 10px;");
    centerL->addWidget(m_historyTitleLabel);

    QSplitter *vSplitter = new QSplitter(Qt::Vertical);
    vSplitter->setHandleWidth(2);

    // 1. 题干容器
    m_historyQuestionWebView = new QWebEngineView();
    m_historyQuestionWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_historyQuestionWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_historyQuestionWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_historyQuestionWebView->setStyleSheet("background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px;");
    m_historyQuestionWebView->setUrl(QUrl("qrc:///www/student_markdown.html"));
    m_historyQuestionWebView->setZoomFactor(1.2);
    vSplitter->addWidget(m_historyQuestionWebView);

    // 2. 底部综合容器 (包含堆叠答题区 + 批改反馈区)
    QWidget *lowerHalf = new QWidget();
    QVBoxLayout *lowerHalfLayout = new QVBoxLayout(lowerHalf);
    lowerHalfLayout->setContentsMargins(0, 10, 0, 0);

    m_historyBottomStack = new QStackedWidget();

    // 2.1 选项容器
    m_historyOptionsWebView = new QWebEngineView();
    m_historyOptionsWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_historyOptionsWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_historyOptionsWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_historyOptionsWebView->setStyleSheet("background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px;");
    m_historyOptionsWebView->setUrl(QUrl("qrc:///www/analysis_markdown.html"));
    m_historyOptionsWebView->setZoomFactor(1.2);
    m_historyBottomStack->addWidget(m_historyOptionsWebView);

    // 2.2 代码容器
    m_historyCodeWebView = new QWebEngineView();
    m_historyCodeWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_historyCodeWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_historyCodeWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_historyCodeWebView->setStyleSheet("background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px;");
    m_historyCodeWebView->setUrl(QUrl("qrc:///www/student_code_editor.html"));
    m_historyCodeWebView->setZoomFactor(1.2);
    m_historyBottomStack->addWidget(m_historyCodeWebView);

    // 2.3 原生填空区
    m_historyAnswerContainer = new QWidget();
    m_historyAnswerLayout = new QVBoxLayout(m_historyAnswerContainer);
    m_historyAnswerLayout->setContentsMargins(10, 10, 10, 10);
    m_historyAnswerContainer->setStyleSheet("background: #F8FAFC; border: 1px solid #E2E8F0; border-radius: 8px;");
    m_historyBottomStack->addWidget(m_historyAnswerContainer);

    lowerHalfLayout->addWidget(m_historyBottomStack, 1);

    // 3. 底部反馈区容器
    m_currentHistoryAnswerWidget = new QWidget();
    m_currentHistoryAnswerWidget->setLayout(new QVBoxLayout());
    m_currentHistoryAnswerWidget->layout()->setContentsMargins(0, 5, 0, 0);
    lowerHalfLayout->addWidget(m_currentHistoryAnswerWidget, 0);

    vSplitter->addWidget(lowerHalf);
    vSplitter->setSizes({400, 400});

    centerL->addWidget(vSplitter);
    innerSplitter->addWidget(centerP);

    QWidget *rightP = new QWidget();
    rightP->setStyleSheet("QWidget#HRight { background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; }");
    rightP->setObjectName("HRight");
    QVBoxLayout *rightL = new QVBoxLayout(rightP); rightL->setContentsMargins(15, 20, 15, 20); rightL->setSpacing(20);

    QLabel *navTitle = new QLabel("📑 错题追踪导航"); navTitle->setFont(QFont("Microsoft YaHei", 16, QFont::Bold)); navTitle->setStyleSheet("color:#1E293B; border:none;");

    m_historyNavWidget = new QListWidget();
    m_historyNavWidget->setViewMode(QListView::IconMode);
    m_historyNavWidget->setResizeMode(QListView::Adjust);
    m_historyNavWidget->setSpacing(8);
    m_historyNavWidget->setMovement(QListView::Static);
    m_historyNavWidget->setGridSize(QSize(62, 62));
    m_historyNavWidget->setStyleSheet("QListWidget { background: transparent; border: none; outline: none; } QListWidget::item { background: transparent; outline: none; }");

    QLabel *scoreTip = new QLabel("本次测试总得分"); scoreTip->setAlignment(Qt::AlignCenter); scoreTip->setStyleSheet("color:#64748B; font-weight:bold; font-size:14px; border:none;");
    m_historyScoreLabel = new QLabel("--"); m_historyScoreLabel->setAlignment(Qt::AlignCenter);
    m_historyScoreLabel->setStyleSheet("color: #10B981; font-size: 48px; font-weight: bold; font-family: 'Consolas'; border: none;");

    QLabel *legend = new QLabel("🟢 正确  🔴 错误\n🟡 待批  🔵 已批");
    legend->setStyleSheet("color: #94A3B8; font-size:13px; font-weight:bold; line-height: 1.5; border:none;"); legend->setAlignment(Qt::AlignCenter);

    rightL->addWidget(navTitle); rightL->addWidget(m_historyNavWidget, 1);
    QFrame *tmpFrame = new QFrame(); tmpFrame->setFrameShape(QFrame::HLine); rightL->addWidget(tmpFrame);
    rightL->addWidget(legend); rightL->addWidget(scoreTip); rightL->addWidget(m_historyScoreLabel);

    innerSplitter->addWidget(rightP); innerSplitter->setSizes(QList<int>() << 700 << 250);
    detailL->addWidget(innerSplitter);
    mainSplitter->addWidget(m_historyDetailPanel);
    m_historyDetailPanel->hide();

    mainL->addWidget(mainSplitter, 1);
}

void StudentHistoryWidget::updateData(const QJsonArray &cloudExams, const QJsonObject &myRecord) {
    m_cloudExams = cloudExams;
    m_myRecord = myRecord;
}

int StudentHistoryWidget::calculateStudentHistoryScore(const QJsonArray &qs, const QJsonObject &ans, const QJsonObject &mScores) {
    int score = 0;
    for (int i = 0; i < qs.size(); ++i) {
        QJsonObject q = qs[i].toObject();
        QString type = q["type"].toString();
        QString k = QString::number(i);
        if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
            if (mScores.contains(k)) score += mScores[k].toInt();
            continue;
        }
        if (!ans.contains(k)) continue;
        if (type == "single" && ans[k].toInt() == q["answer"].toInt()) score += q["score"].toInt();
        else if (type == "multi" && ans[k].toArray() == q["answer"].toArray()) score += q["score"].toInt();
        else if (type == "fill") {
            if (ans[k].isString() && ans[k].toString() == q["answer"].toString()) score += q["score"].toInt();
            else if (ans[k].isArray() && ans[k].toArray() == q["answer"].toArray()) score += q["score"].toInt();
        }
    }
    return score;
}

void StudentHistoryWidget::showHistory() {
    m_cachedTableData = QJsonArray();

    for (int i = 0; i < m_cloudExams.size(); ++i) {
        QJsonObject ex = m_cloudExams[i].toObject();
        QString examId = ex["examId"].toString();
        if (m_myRecord.contains(examId) && m_myRecord[examId].toObject()["status"].toString() == "SUBMITTED") {
            QJsonObject uRec = m_myRecord[examId].toObject();
            int score = calculateStudentHistoryScore(ex["questions"].toArray(), uRec["answers"].toObject(), uRec["manual_scores"].toObject());

            QString submitTime = uRec["updateTime"].toString();
            if(submitTime.isEmpty()) submitTime = "--";

            bool fullyGraded = true;
            QJsonArray qs = ex["questions"].toArray();
            for(int j=0; j<qs.size(); ++j) {
                QString t = qs[j].toObject()["type"].toString();
                if(t == "oi" || t == "code_snippet" || t == "code") {
                    if(!uRec["manual_scores"].toObject().contains(QString::number(j))) fullyGraded = false;
                }
            }

            QJsonObject rowObj;
            rowObj["examId"] = examId;
            rowObj["title"] = ex["title"].toString();
            rowObj["score"] = QString::number(score) + " 分";
            rowObj["time"] = submitTime;
            rowObj["status"] = fullyGraded ? "✅ 成绩有效" : "⏳ 等待批阅";

            m_cachedTableData.append(rowObj);
        }
    }

    if (m_isTableLoaded) {
        QByteArray jsonBytes = QJsonDocument(m_cachedTableData).toJson(QJsonDocument::Compact).toBase64();
        m_historyTableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(jsonBytes)));
    }

    m_historyDetailPanel->hide();
    if (m_currentHistoryAnswerWidget) m_currentHistoryAnswerWidget->hide();
    m_historyQuestionWebView->page()->runJavaScript("renderMarkdown('');");
    m_historyScoreLabel->setText("--");
    m_historyTitleLabel->setText("👈 请在左侧选择一份已完成的答卷");
}

void StudentHistoryWidget::onWebRowClicked(const QString &examId) {
    QJsonObject ex;
    for(int i=0; i<m_cloudExams.size(); i++) {
        if(m_cloudExams[i].toObject()["examId"].toString() == examId) {
            ex = m_cloudExams[i].toObject();
            break;
        }
    }
    if(ex.isEmpty()) return;

    m_currentHistoryExamId = examId;
    m_historyQuestions = ex["questions"].toArray();

    QJsonObject uRec = m_myRecord[examId].toObject();
    m_historyAnswers = uRec["answers"].toObject();
    m_historyManualScores = uRec["manual_scores"].toObject();
    m_historyFeedbacks = uRec["feedbacks"].toObject();

    int score = calculateStudentHistoryScore(m_historyQuestions, m_historyAnswers, m_historyManualScores);
    m_historyScoreLabel->setText(QString::number(score));
    m_historyTitleLabel->setText(QString("📑 %1 (深度解析)").arg(ex["title"].toString()));

    m_historyDetailPanel->setVisible(true);
    buildHistoryNavGrid();
    loadHistoryQuestion(0);
}

void StudentHistoryWidget::buildHistoryNavGrid() {
    m_historyNavWidget->clear(); m_historyNavButtons.clear();
    for (int i = 0; i < m_historyQuestions.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(m_historyNavWidget); item->setSizeHint(QSize(54, 54));
        CapzeraButton *btn = new CapzeraButton(QString::number(i + 1), CapzeraButton::Secondary); btn->setFixedSize(48, 48); btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, i]() { loadHistoryQuestion(i); });
        m_historyNavWidget->setItemWidget(item, btn); m_historyNavButtons.append(btn);
    }
    updateHistoryNavGrid();
}

void StudentHistoryWidget::updateHistoryNavGrid() {
    for (int i = 0; i < m_historyNavButtons.size(); ++i) {
        m_historyNavButtons[i]->setStyleSheet(m_historyNavButtons[i]->styleSheet().replace("border: 2px solid #1E293B;", "border: none;"));
        if (i == m_currentHistoryQuestionIndex) {
            m_historyNavButtons[i]->setStyleSheet(m_historyNavButtons[i]->styleSheet() + "QPushButton { border: 2px solid #1E293B; }");
        }

        QString key = QString::number(i);
        if (!m_historyAnswers.contains(key)) {
            m_historyNavButtons[i]->setTheme(CapzeraButton::Secondary);
            continue;
        }

        QJsonObject q = m_historyQuestions[i].toObject();
        QString type = q["type"].toString();

        if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
            if (m_historyManualScores.contains(key)) m_historyNavButtons[i]->setTheme(CapzeraButton::Success);
            else m_historyNavButtons[i]->setTheme(CapzeraButton::Primary);
        } else {
            bool isRight = false;
            bool isHalfRight = false;
            if (type == "single") isRight = (m_historyAnswers[key].toInt() == q["answer"].toInt());
            else if (type == "fill") {
                if (m_historyAnswers[key].isString()) isRight = (m_historyAnswers[key].toString() == q["answer"].toString());
                else isRight = (m_historyAnswers[key].toArray() == q["answer"].toArray());
            } else if (type == "multi") {
                QJsonArray rightArr = q["answer"].toArray();
                QJsonArray curArr = m_historyAnswers[key].toArray();
                isRight = (curArr == rightArr);
                if (!isRight && !curArr.isEmpty()) {
                    isHalfRight = true;
                    for (int c = 0; c < curArr.size(); ++c) {
                        bool found = false;
                        for (int r = 0; r < rightArr.size(); ++r) {
                            if (curArr[c].toInt() == rightArr[r].toInt()) { found = true; break; }
                        }
                        if (!found) { isHalfRight = false; break; }
                    }
                }
            }
            if (isRight) m_historyNavButtons[i]->setTheme(CapzeraButton::Success);
            else if (isHalfRight) m_historyNavButtons[i]->setTheme(CapzeraButton::Warning);
            else m_historyNavButtons[i]->setTheme(CapzeraButton::Danger);
        }
    }
}
void StudentHistoryWidget::loadHistoryQuestion(int index) {
    if (index < 0 || index >= m_historyQuestions.size()) return;
    m_currentHistoryQuestionIndex = index;
    updateHistoryNavGrid();

    QJsonObject q = m_historyQuestions[index].toObject();
    QString type = q["type"].toString();
    QString key = QString::number(index);

    // ================= 1. 下发题干 (上半部分) =================
    QJsonObject payload;
    payload["md"] = q.contains("description") ? q["description"].toString() : q["stem"].toString();
    payload["md"] = QString("\n\n(%1 分)  ").arg(q["score"].toInt()) + payload["md"].toString();

    if (type == "oi") {
        payload["md"] = payload["md"].toString() + QString("\n\n#### 📥 输入样例\n```text\n%1\n```\n#### 📤 输出样例\n```text\n%2\n```\n").arg(q["inputSample"].toString()).arg(q["outputSample"].toString());
    }

    QByteArray base64Data = QJsonDocument(payload).toJson(QJsonDocument::Compact).toBase64();
    m_historyQuestionWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(base64Data)));

    // ================= 2. 清理下半部分原生布局残影 =================
    QLayoutItem *child;
    while ((child = m_historyAnswerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    // ================= 3. 解析学员答案与老师评分 =================
    QString userAnsStr = "未作答";
    bool hasUserAnswer = m_historyAnswers.contains(key);

    if (hasUserAnswer) {
        QJsonValue val = m_historyAnswers[key];
        if (type == "single") userAnsStr = QString(QChar('A' + val.toInt()));
        else if (type == "multi") {
            QJsonArray aArr = val.toArray(); QStringList sl;
            for(int i=0; i<aArr.size(); ++i) sl << QString(QChar('A' + aArr[i].toInt()));
            userAnsStr = sl.join(", ");
        } else if (type == "fill") {
            if (val.isArray()) {
                QJsonArray aArr = val.toArray(); QStringList sl;
                for(int i=0; i<aArr.size(); ++i) sl << aArr[i].toString();
                userAnsStr = sl.join(", ");
            } else userAnsStr = val.toString();
        }
    }

    // ================= 4. 路由分发给各种题型引擎 =================
    if (type == "single" || type == "multi") {
        m_historyBottomStack->setCurrentWidget(m_historyOptionsWebView);
        QJsonObject optPayload;
        optPayload["type"] = type;
        QJsonArray optsArr;
        QJsonArray qOptions = q["options"].toArray();

        QJsonArray stdAnsArr;
        if (type == "single") stdAnsArr.append(q["answer"].toInt());
        else stdAnsArr = q["answer"].toArray();

        QJsonArray stuAnsArr;
        if (hasUserAnswer) {
            if (type == "single") stuAnsArr.append(m_historyAnswers[key].toInt());
            else stuAnsArr = m_historyAnswers[key].toArray();
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

        QByteArray optB64 = QJsonDocument(optPayload).toJson(QJsonDocument::Compact).toBase64();
        m_historyOptionsWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(optB64)));

    } else if (type == "code" || type == "oi" || type == "code_completion" || type == "code_snippet") {
        m_historyBottomStack->setCurrentWidget(m_historyCodeWebView);
        QString existingCode = "```cpp\n未提交任何有效代码\n```";
        if (hasUserAnswer) {
            existingCode = m_historyAnswers[key].toString();
            if (!existingCode.startsWith("```")) existingCode = "```cpp\n" + existingCode + "\n```";
        }
        QByteArray b64Code = existingCode.toUtf8().toBase64();
        m_historyCodeWebView->page()->runJavaScript(QString("initEditor(%1, '%2');").arg(index).arg(QString(b64Code)));

        if (m_historyManualScores.contains(key)) {
            userAnsStr = QString("%1 分 (满分 %2 分)").arg(m_historyManualScores[key].toInt()).arg(q["score"].toInt());
        } else {
            userAnsStr = "暂无评分 (待批阅)";
        }

    } else if (type == "fill") {
        m_historyBottomStack->setCurrentWidget(m_historyAnswerContainer);
        int fillCount = q["options"].toArray().size();
        if(fillCount == 0) fillCount = 1;
        QJsonArray userAnsArr;
        if (hasUserAnswer) userAnsArr = m_historyAnswers[key].toArray();

        for (int i=0; i<fillCount; ++i) {
            QHBoxLayout *h = new QHBoxLayout();
            QLabel *l = new QLabel(QString("你的答案【%1】:").arg(i+1));
            l->setStyleSheet("color:#475569; font-weight:bold; font-family: HYWenHei65W; font-size: 16px");
            QLineEdit *le = new QLineEdit();
            le->setStyleSheet("font-weight:bold; font-family: HYWenHei65W; font-size: 16px");
            le->setText(q["answer"].toString());
            le->setReadOnly(true);
            le->setStyleSheet("padding: 8px; border: 1px solid #CBD5E1; border-radius: 4px; background: #F1F5F9; color:#1E293B;");
            if (i < userAnsArr.size()) le->setText(userAnsArr[i].toString());
            h->addWidget(l); h->addWidget(le);
            m_historyAnswerLayout->addLayout(h);
        }
        m_historyAnswerLayout->addStretch();
    }

    // ================= 5. 构建底部核对卡片 =================
    QFrame *feedbackBox = new QFrame();
    QVBoxLayout *fbL = new QVBoxLayout(feedbackBox); fbL->setContentsMargins(15, 15, 15, 15);

    bool isGradedByTeacher = m_historyManualScores.contains(key);
    feedbackBox->setStyleSheet(isGradedByTeacher
                                   ? "QFrame { background: #ECFDF5; border: 1px solid #10B981; border-radius: 8px; }"
                                   : "QFrame { background: #FFFBEB; border: 1px solid #F59E0B; border-radius: 8px; }");

    QString correctAnsStr;
    if (type == "single") correctAnsStr = QString(QChar('A' + q["answer"].toInt()));
    else if (type == "multi") {
        QJsonArray aArr = q["answer"].toArray(); QStringList sl;
        for(int i=0; i<aArr.size(); ++i) sl << QString(QChar('A' + aArr[i].toInt()));
        correctAnsStr = sl.join(", ");
    } else if (type == "fill") correctAnsStr = q["answer"].toString();
    else correctAnsStr = "主观题/代码题，请参考详细解析或老师点评";

    QString labelText = QString("<b>标准答案：</b><span style='color:#059669;'>%1</span> &nbsp;&nbsp;&nbsp;&nbsp; ").arg(correctAnsStr);
    if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
        labelText += QString("<b>教师评分：</b><span style='color:#2563EB; font-weight:bold;'>%1</span>").arg(userAnsStr);
    } else {
        labelText += QString("<b>你的答案：</b><span style='color:#475569;'>%1</span>").arg(userAnsStr);
    }

    QLabel *lblAns = new QLabel(labelText);
    lblAns->setStyleSheet("color:#1E293B; font-size:15px; border:none; background: transparent;");
    lblAns->setWordWrap(true);
    fbL->addWidget(lblAns);

    if (isGradedByTeacher && m_historyFeedbacks.contains(key) && !m_historyFeedbacks[key].toString().isEmpty()) {
        QFrame *line2 = new QFrame(); line2->setFrameShape(QFrame::HLine); line2->setStyleSheet("color: #A7F3D0; border:none; height:1px; margin-top:5px; margin-bottom:5px;");
        fbL->addWidget(line2);
        QLabel *lblFeedback = new QLabel(QString("💬 <b>老师评语：</b><span style='color:#047857;'>%1</span>").arg(m_historyFeedbacks[key].toString()));
        lblFeedback->setStyleSheet("color:#065F46; font-size:14px; border:none; background: transparent;");
        lblFeedback->setWordWrap(true);
        fbL->addWidget(lblFeedback);
    }

    m_historyAnswerLayout->addWidget(feedbackBox);
}