#include "AnalysisProblemWidget.h"
#include <QHeaderView>
#include <QJsonDocument>
#include <QWebEngineSettings>
#include <QDir>
#include <QCoreApplication>
#include <algorithm>

AnalysisProblemWidget::AnalysisProblemWidget(QWidget *parent) : QWidget(parent) {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    initUI();
}

void AnalysisProblemWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; margin: 0 5px; width: 2px; }");

    // ================= 左侧 (彻底移入 Web 引擎) =================
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel); leftLayout->setContentsMargins(0, 0, 0, 0);

    m_tableWebView = new QWebEngineView(leftPanel);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *tableChannel = new QWebChannel(m_tableWebView);
    tableChannel->registerObject("analysisProblemTableBridge", this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/analysis_problem_table.html"));

    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        m_isTableLoaded = ok;
        if (ok && m_cloudExams.size() > 0) {
            syncWebCombo();
            onExamIndexChanged(0);
        }
    });
    leftLayout->addWidget(m_tableWebView, 1);

    // ================= 右侧：分析详情 =================
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel); rightLayout->setContentsMargins(0, 0, 0, 0);

    m_analysisTitleLabel = new QLabel("👈 请在左侧选择一道试题以调阅雷达分析数据...");
    m_analysisTitleLabel->setStyleSheet("color:#1E293B; font-weight:bold; font-size:18px; margin-bottom: 10px; border:none; background: transparent;");
    rightLayout->addWidget(m_analysisTitleLabel);

    m_webView = new QWebEngineView(rightPanel);
    m_webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    QWebChannel *channel = new QWebChannel(m_webView);
    channel->registerObject("problemAnalysisBridge", this);
    m_webView->page()->setWebChannel(channel);

    m_webView->setUrl(QUrl("qrc:///www/problem_analysis.html"));
    rightLayout->addWidget(m_webView, 1);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes(QList<int>() << 450 << 750);
    mainLayout->addWidget(splitter);
}

void AnalysisProblemWidget::updateRealData(const QJsonArray &exams, const QJsonObject &allRecords, const QVector<StudentInfo> &students) {
    m_cloudExams = exams;
    m_allRecords = allRecords;
    m_students = students;

    if (m_isTableLoaded) {
        syncWebCombo();
        if (m_cloudExams.size() > 0) onExamIndexChanged(0);
    }
}

void AnalysisProblemWidget::syncWebCombo() {
    QJsonArray titles;
    for (int i = 0; i < m_cloudExams.size(); ++i) titles.append(m_cloudExams[i].toObject()["title"].toString());
    QByteArray bytes = QJsonDocument(titles).toJson(QJsonDocument::Compact).toBase64();
    m_tableWebView->page()->runJavaScript(QString("loadExamCombo('%1');").arg(QString(bytes)));
}

bool AnalysisProblemWidget::checkAnswerCorrectness(const QString &type, const QVariant &stdAns, const QVariant &stuAns) {
    if (type == "single") return stdAns.toInt() == stuAns.toInt();
    if (type == "multi") return stdAns.toJsonArray() == stuAns.toJsonArray();
    if (type == "fill") {
        if (stuAns.type() == QVariant::String) return stdAns.toString() == stuAns.toString();
        else return stdAns.toJsonArray() == stuAns.toJsonArray();
    }
    return false;
}

void AnalysisProblemWidget::onExamIndexChanged(int index) {
    if (index < 0 || index >= m_cloudExams.size()) return;
    m_currentExamIndex = index; // 锁定内存

    QJsonObject examsObj = m_cloudExams[index].toObject();
    QString examId = examsObj["examId"].toString();
    QString targetClass = examsObj["targetClass"].toString();
    m_currentQuestions = examsObj["questions"].toArray();

    QJsonArray tableData;
    for (int i = 0; i < m_currentQuestions.size(); ++i) {
        QJsonObject q = m_currentQuestions[i].toObject();
        QString type = q["type"].toString();

        int totalAttempts = 0;
        int correctCount = 0;
        int totalScore = 0;

        for (int s = 0; s < m_students.size(); ++s) {
            QString u = m_students[s].account;
            QString cName = m_students[s].className;

            if (!targetClass.isEmpty() && targetClass != "全部班级" && cName != targetClass) continue;

            if (m_allRecords.contains(u) && m_allRecords[u].toObject().contains(examId)) {
                QJsonObject rec = m_allRecords[u].toObject()[examId].toObject();
                QJsonObject ansObj = rec["answers"].toObject();
                QString k = QString::number(i);

                if (ansObj.contains(k)) {
                    totalAttempts++;
                    if (type == "single" || type == "multi" || type == "fill") {
                        if (checkAnswerCorrectness(type, q["answer"].toVariant(), ansObj[k].toVariant())) {
                            correctCount++;
                        }
                    }
                }

                if (type == "oi" || type == "code" || type == "code_completion" || type == "code_snippet") {
                    if (rec["manual_scores"].toObject().contains(k)) {
                        totalScore += rec["manual_scores"].toObject()[k].toInt();
                    }
                }
            }
        }

        QString rateStr;
        if (type == "single" || type == "multi" || type == "fill") {
            rateStr = (totalAttempts > 0) ? QString("%1%").arg(qRound(100.0 * correctCount / totalAttempts)) : "0%";
        } else {
            rateStr = (totalAttempts > 0) ? QString::number((double)totalScore / totalAttempts, 'f', 1) + " 分" : "0 分";
        }

        QString tName = (type=="single") ? "单选题" : (type=="multi" ? "多选题" : (type=="fill" ? "填空题" : "代码提交题"));

        QJsonObject rowObj;
        rowObj["id"] = QString("第 %1 题").arg(i + 1);
        rowObj["type"] = tName;
        rowObj["rate"] = rateStr;
        rowObj["_index"] = i;
        tableData.append(rowObj);
    }

    if (m_isTableLoaded) {
        QByteArray bytes = QJsonDocument(tableData).toJson(QJsonDocument::Compact).toBase64();
        m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(QString(bytes)));
    }

    m_webView->page()->runJavaScript("document.getElementById('dynamic-area').style.display='none'; document.getElementById('p-stem').innerHTML=''; document.getElementById('stat-submits').innerText='--'; document.getElementById('stat-correct').innerText='--'; document.getElementById('stat-rate').innerText='--'; document.getElementById('stat-score').innerText='--';");
    m_analysisTitleLabel->setText("👈 请在左侧选择一道试题以调阅雷达分析数据...");
}

void AnalysisProblemWidget::onWebRowClicked(int index) {
    loadProblemAnalysis(index);
}

void AnalysisProblemWidget::onWebPageLoaded() {
    m_isPageLoaded = true;
    if (!m_cachedPayload.isEmpty()) {
        QByteArray base64Data = QJsonDocument(m_cachedPayload).toJson(QJsonDocument::Compact).toBase64();
        m_webView->page()->runJavaScript(QString("updateProblemAnalysis('%1');").arg(QString(base64Data)));
        m_cachedPayload = QJsonObject();
    }
}

// 🚀 被找回的真·核心图表与数据组装逻辑！
void AnalysisProblemWidget::loadProblemAnalysis(int qIdx) {
    if (qIdx < 0 || qIdx >= m_currentQuestions.size() || m_currentExamIndex < 0) return;

    QJsonObject q = m_currentQuestions[qIdx].toObject();
    QString type = q["type"].toString();

    QJsonObject examsObj = m_cloudExams[m_currentExamIndex].toObject();
    QString examId = examsObj["examId"].toString();
    QString targetClass = examsObj["targetClass"].toString();

    m_analysisTitleLabel->setText(QString("📋 第 %1 题 诊断分析报告").arg(qIdx + 1));

    int submittedCount = 0;
    int correctCount = 0;
    QVector<int> choiceCounts;
    if(type == "single" || type == "multi") {
        choiceCounts.resize(q["options"].toArray().size(), 0);
    }
    QMap<QString, int> fillAnswersMap;

    for (const auto &stu : m_students) {
        if (targetClass != "全部班级" && stu.className != targetClass) continue;

        QString u = stu.account;
        if (!m_allRecords.contains(u)) continue;

        QJsonObject uRec = m_allRecords[u].toObject();
        if (uRec.contains(examId) && uRec[examId].toObject()["status"].toString() == "SUBMITTED") {
            submittedCount++;
            QVariant stuAns = uRec[examId].toObject()["answers"].toObject()[QString::number(qIdx)].toVariant();

            if (checkAnswerCorrectness(type, q["answer"].toVariant(), stuAns)) {
                correctCount++;
            }

            if(type == "single" && !stuAns.isNull()) {
                int ansIdx = stuAns.toInt();
                if (ansIdx >= 0 && ansIdx < choiceCounts.size()) choiceCounts[ansIdx]++;
            }
            else if(type == "multi" && !stuAns.isNull()) {
                QJsonArray arr = stuAns.toJsonArray();
                for(int x = 0; x < arr.size(); ++x) {
                    int ansIdx = arr[x].toInt();
                    if (ansIdx >= 0 && ansIdx < choiceCounts.size()) choiceCounts[ansIdx]++;
                }
            }
            else if(type == "fill" && !stuAns.isNull()) {
                QString fAns;
                if (stuAns.type() == QVariant::List) {
                    QJsonArray arr = stuAns.toJsonArray();
                    QStringList sl;
                    for(int i=0; i<arr.size(); ++i) sl << arr[i].toString();
                    fAns = sl.join(", ");
                } else {
                    fAns = stuAns.toString().trimmed();
                }
                if(!fAns.isEmpty()) fillAnswersMap[fAns]++;
            }
        }
    }

    // ================= 组装发送给 WebEngine 的终极 JSON =================
    QJsonObject payload;
    payload["type"] = type;
    payload["score"] = q["score"].toInt();
    payload["stem"] = q.contains("description") ? q["description"].toString() : q["stem"].toString();

    QJsonObject stats;
    stats["submitted"] = submittedCount;
    stats["correct"] = (type == "oi" || type == "code_snippet") ? -1 : correctCount;
    double rate = submittedCount > 0 ? (correctCount * 100.0) / submittedCount : 0;
    stats["rate"] = (type == "oi" || type == "code_snippet") ? "--" : QString::number(rate, 'f', 1) + "%";

    payload["stats"] = stats;
    payload["totalSubmits"] = submittedCount;
    payload["correctCount"] = correctCount;
    payload["correctRate"] = submittedCount > 0 ? qRound((double)correctCount / submittedCount * 100.0) : 0;

    if (type == "single" || type == "multi") {
        QJsonArray categories;
        QJsonArray values;
        for(int i = 0; i < q["options"].toArray().size(); ++i) {
            QString cat = QString(QChar('A'+i));
            if (type == "single" && q["answer"].toInt() == i) cat += "(✅)";
            else if (type == "multi" && q["answer"].toArray().contains(i)) cat += "(✅)";

            categories.append(cat);
            values.append(choiceCounts[i]);
        }
        QJsonObject chartData;
        chartData["categories"] = categories;
        chartData["values"] = values;
        payload["chartData"] = chartData;

    } else if (type == "fill") {
        QJsonArray topFills;
        QList<QPair<QString, int>> sortedAns;
        for(auto it = fillAnswersMap.begin(); it != fillAnswersMap.end(); ++it) {
            sortedAns.append(qMakePair(it.key(), it.value()));
        }
        std::sort(sortedAns.begin(), sortedAns.end(), [](const QPair<QString,int>& a, const QPair<QString,int>& b){
            return a.second > b.second;
        });

        QString stdFAns;
        if (q["answer"].isArray()) {
            QJsonArray arr = q["answer"].toArray();
            QStringList sl;
            for(int j=0; j<arr.size(); ++j) sl << arr[j].toString();
            stdFAns = sl.join(", ");
        } else {
            stdFAns = q["answer"].toString();
        }

        for(int i=0; i < qMin(3, sortedAns.size()); ++i) {
            QJsonObject f;
            f["ans"] = sortedAns[i].first;
            f["count"] = sortedAns[i].second;
            f["isCorrect"] = (sortedAns[i].first == stdFAns);
            topFills.append(f);
        }

        payload["stdAns"] = stdFAns;
        payload["topFills"] = topFills;
    }

    QByteArray base64Data = QJsonDocument(payload).toJson(QJsonDocument::Compact).toBase64();
    QString jsCommand = QString("updateProblemAnalysis('%1');").arg(QString(base64Data));

    if (m_isPageLoaded) {
        m_webView->page()->runJavaScript(jsCommand);
    } else {
        m_cachedPayload = payload;
    }
}