#include "StudentExamWidget.h"
#include "CapzeraDialog.h"
#include <QJsonDocument>
#include <QDateTime>

StudentExamWidget::StudentExamWidget(double dpi, NetworkManager *net, QWidget *parent)
    : QWidget(parent), m_dpi(dpi), m_networkManager(net), currentQuestionIndex(-1), m_radioGroup(nullptr) {
    examTimer = new QTimer(this);
    connect(examTimer, &QTimer::timeout, this, &StudentExamWidget::updateCountdown);
    initUI();
}

void StudentExamWidget::initUI() {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *topBar = new QWidget();
    topBar->setStyleSheet("background: white; border-bottom: 1px solid #E2E8F0;");
    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 15, 20, 15);

    examTitleLabel = new QLabel("正在准备考试...");
    examTitleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #1E293B; border: none;");

    btnLogoutExam = new CapzeraButton("🚪 暂存并退出", CapzeraButton::Secondary);
    btnLogoutExam->setFixedSize(160, 40);
    connect(btnLogoutExam, &QPushButton::clicked, this, [this]() {
        if(CapzeraDialog::question(this, "退出确认", "确定要中途退出吗？答题进度会被安全暂存。")) {
            onSaveTemporarily();
        }
    });

    topLayout->addWidget(examTitleLabel);
    topLayout->addStretch();
    topLayout->addWidget(btnLogoutExam);
    mainLayout->addWidget(topBar);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; width: 2px; }");

    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
    leftSplitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; height: 2px; }");

    m_questionWebView = new QWebEngineView();
    m_questionWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_questionWebView->setUrl(QUrl("qrc:///www/student_markdown.html"));
    leftSplitter->addWidget(m_questionWebView);

    m_bottomStack = new QStackedWidget();

    m_optionsWebView = new QWebEngineView();
    m_optionsWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_optionsWebView->setUrl(QUrl("qrc:///www/student_markdown.html"));
    connect(m_optionsWebView, &QWebEngineView::loadFinished, this, [this](bool ok){ m_isOptionsLoaded = ok; });
    connect(m_optionsWebView, &QWebEngineView::titleChanged, this, &StudentExamWidget::onOptionsTitleChanged);

    // 🚀 新增：注入 Web 现代代码引擎！
    m_codeWebView = new QWebEngineView();
    m_codeWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_codeWebView->setUrl(QUrl("qrc:///www/student_code_editor.html"));
    connect(m_codeWebView, &QWebEngineView::loadFinished, this, [this](bool ok){ m_isCodeLoaded = ok; });
    connect(m_codeWebView, &QWebEngineView::titleChanged, this, &StudentExamWidget::onCodeTitleChanged); // 绑定实时护盾

    answerAreaContainer = new QWidget();
    answerAreaContainer->setStyleSheet("QWidget { background: #FAFAFA; }");
    answerAreaLayout = new QVBoxLayout(answerAreaContainer);
    answerAreaLayout->setContentsMargins(25, 20, 25, 20);

    m_bottomStack->addWidget(m_optionsWebView);
    m_bottomStack->addWidget(m_codeWebView); // 注入底层栈
    m_bottomStack->addWidget(answerAreaContainer);

    leftSplitter->addWidget(m_bottomStack);
    leftSplitter->setSizes({500, 300});
    mainSplitter->addWidget(leftSplitter);

    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet("background: white;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(20, 20, 20, 20);

    m_bigTimerLabel = new QLabel("00:00:00");
    m_bigTimerLabel->setAlignment(Qt::AlignCenter);
    m_bigTimerLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: #EF4444; font-family: Consolas; border: none; padding: 10px; background: #FEF2F2; border-radius: 12px; margin-bottom: 10px;");
    rightLayout->addWidget(m_bigTimerLabel);

    m_navGridWidget = new QListWidget();
    m_navGridWidget->setViewMode(QListView::IconMode);
    m_navGridWidget->setResizeMode(QListView::Adjust);
    m_navGridWidget->setSpacing(8);
    m_navGridWidget->setMovement(QListView::Static);
    m_navGridWidget->setStyleSheet("QListWidget { border: none; background: transparent; outline: none; } QListWidget::item { background: transparent; outline: none; }");
    rightLayout->addWidget(m_navGridWidget, 1);

    QHBoxLayout *btnL1 = new QHBoxLayout();
    btnPrevQuestion = new CapzeraButton("⬅ 上一题", CapzeraButton::Secondary);
    btnPrevQuestion->setShortcut(QKeySequence(Qt::Key_Left));
    btnNextQuestion = new CapzeraButton("下一题 ➡", CapzeraButton::Primary);
    btnNextQuestion->setShortcut(QKeySequence(Qt::Key_Right));
    connect(btnPrevQuestion, &QPushButton::clicked, this, &StudentExamWidget::onPrevQuestion);
    connect(btnNextQuestion, &QPushButton::clicked, this, &StudentExamWidget::onNextQuestion);
    btnL1->addWidget(btnPrevQuestion); btnL1->addWidget(btnNextQuestion);
    rightLayout->addLayout(btnL1);

    QHBoxLayout *btnL2 = new QHBoxLayout();
    btnSaveTemporarily = new CapzeraButton("💾 暂存进度", CapzeraButton::Warning);
    btnSubmitExam = new CapzeraButton("✅ 提交试卷", CapzeraButton::Success);
    connect(btnSaveTemporarily, &QPushButton::clicked, this, &StudentExamWidget::onSaveTemporarily);
    connect(btnSubmitExam, &QPushButton::clicked, this, &StudentExamWidget::onSubmitExam);
    btnL2->addWidget(btnSaveTemporarily); btnL2->addWidget(btnSubmitExam);
    rightLayout->addLayout(btnL2);

    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes({800, 350});
    mainLayout->addWidget(mainSplitter, 1);
}

void StudentExamWidget::startExam(const QJsonObject &examObj, const QJsonObject &myRecord) {
    m_currentExamId = examObj["examId"].toString();
    examTitleLabel->setText("📖 正在考试：" + examObj["title"].toString());
    m_currentQuestions = examObj["questions"].toArray();
    m_answersCache.clear();
    m_questionSequence.clear();

    m_isOptionShuffled = examObj["isOptionShuffled"].toBool();

    for (int i = 0; i < m_currentQuestions.size(); ++i) m_questionSequence.append(i);

    if (myRecord.contains(m_currentExamId) && myRecord[m_currentExamId].toObject()["status"].toString() == "SAVED") {
        QJsonObject savedAns = myRecord[m_currentExamId].toObject()["answers"].toObject();
        for (auto it = savedAns.begin(); it != savedAns.end(); ++it) {
            QJsonValue jVal = it.value();
            if (jVal.isArray()) m_answersCache[it.key().toInt()] = jVal.toArray();
            else m_answersCache[it.key().toInt()] = jVal.toVariant();
        }
    }

    QString endTimeStr = examObj["endTime"].toString();
    QDateTime endTime = QDateTime::fromString(endTimeStr, "yyyy-MM-dd HH:mm");
    remainingSeconds = QDateTime::currentDateTime().secsTo(endTime);
    if (remainingSeconds < 0) remainingSeconds = 0;

    updateCountdown();
    examTimer->start(1000);

    buildNavGrid();
    currentQuestionIndex = -1;
    loadQuestion(0);
}

void StudentExamWidget::buildNavGrid() {
    m_navGridWidget->clear();
    m_navButtons.clear();

    for (int i = 0; i < m_currentQuestions.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(m_navGridWidget);
        item->setSizeHint(QSize(45, 45));
        CapzeraButton *btn = new CapzeraButton(QString::number(i + 1), CapzeraButton::Secondary);
        btn->setFixedSize(45, 45);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, i]() { loadQuestion(i); });
        m_navButtons.append(btn);
        m_navGridWidget->setItemWidget(item, btn);
    }
    updateNavGrid();
}

void StudentExamWidget::updateNavGrid() {
    for (int i = 0; i < m_navButtons.size(); ++i) {
        m_navButtons[i]->setStyleSheet(m_navButtons[i]->styleSheet().replace("border: 2px solid #1E293B;", "border: none;"));

        if (i == currentQuestionIndex) {
            m_navButtons[i]->setTheme(CapzeraButton::Warning);
            m_navButtons[i]->setStyleSheet(m_navButtons[i]->styleSheet() + "QPushButton { border: 2px solid #1E293B; }");
        } else {
            bool isAnswered = false;
            int realIdx = m_questionSequence[i];

            if (m_answersCache.contains(realIdx)) {
                QVariant ans = m_answersCache[realIdx];
                QString type = m_currentQuestions[realIdx].toObject()["type"].toString();
                if (type == "single" && ans.toInt() != -1) isAnswered = true;
                else if (type == "multi" && !ans.toJsonArray().isEmpty()) isAnswered = true;
                // 🚀 核心过滤：检测代码是否仅仅为未动过的初始模板
                else if (!ans.toString().trimmed().isEmpty() && !ans.toString().contains("请将代码复制粘贴至此处")) isAnswered = true;
            }
            if (isAnswered) m_navButtons[i]->setTheme(CapzeraButton::Success);
            else m_navButtons[i]->setTheme(CapzeraButton::Secondary);
        }
    }
}

// 🚀 代码题和选项题均已由 JS 实时接管，本函数专职兜底填空题
void StudentExamWidget::saveCurrentAnswer() {
    if (currentQuestionIndex < 0) return;
    int realIdx = m_questionSequence[currentQuestionIndex];
    QString type = m_currentQuestions[realIdx].toObject()["type"].toString();

    if (type == "fill" && m_fillEdit) {
        if (!m_fillEdit->text().trimmed().isEmpty()) m_answersCache[realIdx] = m_fillEdit->text().trimmed();
        else m_answersCache.remove(realIdx);
    }
}

void StudentExamWidget::loadQuestion(int index) {
    if (index < 0 || index >= m_questionSequence.size()) return;
    saveCurrentAnswer();
    currentQuestionIndex = index;

    if (index > 0) {
        btnPrevQuestion->setEnabled(1);
        btnPrevQuestion->setTheme(CapzeraButton::Primary);
    } else {
        btnPrevQuestion->setEnabled(0);
        btnPrevQuestion->setTheme(CapzeraButton::Secondary);
    }

    if (index < m_questionSequence.size() - 1) {
        btnNextQuestion->setEnabled(1);
        btnNextQuestion->setTheme(CapzeraButton::Primary);
    } else {
        btnNextQuestion->setEnabled(0);
        btnNextQuestion->setTheme(CapzeraButton::Secondary);
    }

    int realIdx = m_questionSequence[index];
    QJsonObject q = m_currentQuestions[realIdx].toObject();
    QString type = q["type"].toString();

    QJsonObject stemPayload;
    stemPayload["type"] = "stem_only";
    stemPayload["md"] = q.contains("description") ? q["description"].toString() : q["stem"].toString();
    if (type == "oi") {
        stemPayload["md"] = stemPayload["md"].toString() + QString("\n\n#### 📥 输入样例\n```text\n%1\n```\n#### 📤 输出样例\n```text\n%2\n```\n").arg(q["inputSample"].toString()).arg(q["outputSample"].toString());
    }
    QByteArray stemB64 = QJsonDocument(stemPayload).toJson(QJsonDocument::Compact).toBase64();
    m_questionWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(stemB64)));

    QLayoutItem *child;
    while ((child = answerAreaLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    m_fillEdit = nullptr;

    if (type == "single" || type == "multi") {
        m_bottomStack->setCurrentWidget(m_optionsWebView);

        QJsonArray opts = q["options"].toArray();
        QVector<int> mapping(opts.size());
        std::iota(mapping.begin(), mapping.end(), 0);

        if (m_isOptionShuffled) {
            QSettings settings("CapzeraEdu", "SaaSOlympic");
            QString account = settings.value("LastSavedUser").toString();
            quint32 seed = qHash(m_currentExamId + QString::number(realIdx) + account);
            std::shuffle(mapping.begin(), mapping.end(), std::default_random_engine(seed));
        }
        m_optionMapping[realIdx] = mapping;

        QJsonArray savedAnsArr;
        if (m_answersCache.contains(realIdx)) {
            QVariant varAns = m_answersCache[realIdx];
            if (varAns.type() == QVariant::List || varAns.canConvert<QJsonArray>()) {
                savedAnsArr = varAns.toJsonArray();
                if(savedAnsArr.isEmpty() && varAns.canConvert<QVariantList>()) {
                    QVariantList vl = varAns.toList();
                    for(auto v : vl) savedAnsArr.append(v.toInt());
                }
            } else {
                savedAnsArr.append(varAns.toInt());
            }
        }

        QJsonObject optPayload;
        optPayload["type"] = type;
        optPayload["md"] = "";
        optPayload["qIndex"] = index;
        QJsonArray optsArr;
        QJsonArray userAnsUiArr;

        for (int i = 0; i < opts.size(); ++i) {
            int originalIdx = mapping[i];
            QJsonObject optObj;
            optObj["id"] = i;
            optObj["displayId"] = QString(QChar('A' + i));
            optObj["md"] = opts[originalIdx].toString();

            bool isSelected = false;
            for (int v = 0; v < savedAnsArr.size(); ++v) {
                if (savedAnsArr[v].toInt() == originalIdx) {
                    isSelected = true;
                    userAnsUiArr.append(i);
                    break;
                }
            }
            optObj["status"] = isSelected ? "selected" : "normal";
            optsArr.append(optObj);
        }
        optPayload["options"] = optsArr;
        optPayload["userAnswer"] = userAnsUiArr;

        if (m_isOptionsLoaded) {
            QByteArray optB64 = QJsonDocument(optPayload).toJson(QJsonDocument::Compact).toBase64();
            m_optionsWebView->page()->runJavaScript(QString("renderMarkdown('%1');").arg(QString(optB64)));
        }
    } else if (type == "code" || type == "oi" || type == "code_completion" || type == "code_snippet") {
        // 🚀 全新 Web 代码引擎分发！
        m_bottomStack->setCurrentWidget(m_codeWebView);

        QString existingCode = "";
        if (m_answersCache.contains(realIdx)) {
            existingCode = m_answersCache[realIdx].toString();
        }
        if (existingCode.trimmed().isEmpty() || existingCode.trimmed() == "```cpp" || existingCode.trimmed() == "```") {
            existingCode = "```cpp\n请将代码复制粘贴至此处...\n```";
        }

        if (m_isCodeLoaded) {
            QByteArray b64Code = existingCode.toUtf8().toBase64();
            m_codeWebView->page()->runJavaScript(QString("initEditor(%1, '%2');").arg(index).arg(QString(b64Code)));
        }
    } else if (type == "fill") {
        m_bottomStack->setCurrentWidget(answerAreaContainer);

        QLabel *tip = new QLabel("✍ 请在下方精准输入你的答案：");
        tip->setStyleSheet("color:#8B5CF6; font-weight:bold; font-size:15px; border:none;");
        answerAreaLayout->addWidget(tip);
        m_fillEdit = new QLineEdit();
        m_fillEdit->setStyleSheet("QLineEdit { border: 2px solid #CBD5E1; border-radius: 8px; padding: 15px; font-size: 18px; color:#1E293B; } QLineEdit:focus { border-color: #3B82F6; }");
        if (m_answersCache.contains(realIdx)) m_fillEdit->setText(m_answersCache[realIdx].toString());
        answerAreaLayout->addWidget(m_fillEdit);
        answerAreaLayout->addStretch();
        connect(m_fillEdit, &QLineEdit::textChanged, this, [this]() { saveCurrentAnswer(); updateNavGrid(); });
    }
    updateNavGrid();
}

void StudentExamWidget::onOptionsTitleChanged(const QString &title) {
    if (title.startsWith("ANS:")) {
        QStringList parts = title.split("|");
        if (parts.size() >= 3) {
            int qIndex = parts[0].mid(4).toInt();
            QString jsonStr = parts[1];
            QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());

            if (doc.isArray() && qIndex >= 0 && qIndex < m_questionSequence.size()) {
                int realIdx = m_questionSequence[qIndex];
                QString type = m_currentQuestions[realIdx].toObject()["type"].toString();
                QJsonArray arr = doc.array();

                QJsonArray finalAns;
                for (int i = 0; i < arr.size(); ++i) {
                    int uiIdx = arr[i].toInt();
                    if (m_optionMapping.contains(realIdx) && uiIdx < m_optionMapping[realIdx].size()) {
                        finalAns.append(m_optionMapping[realIdx][uiIdx]);
                    }
                }

                if (type == "single") {
                    if (!finalAns.isEmpty()) m_answersCache[realIdx] = finalAns[0].toInt();
                    else m_answersCache.remove(realIdx);
                } else if (type == "multi") {
                    if (!finalAns.isEmpty()) m_answersCache[realIdx] = finalAns;
                    else m_answersCache.remove(realIdx);
                }
                updateNavGrid();
            }
        }
    }
}

// 🚀 新增：毫秒级监听学生写代码动作！完全防丢！
void StudentExamWidget::onCodeTitleChanged(const QString &title) {
    if (title.startsWith("CODE:")) {
        QStringList parts = title.split("|");
        if (parts.size() >= 2) {
            int qIndex = parts[0].mid(5).toInt();
            QString safeValue = parts[1];

            if (qIndex >= 0 && qIndex < m_questionSequence.size()) {
                int realIdx = m_questionSequence[qIndex];
                QByteArray utf8Bytes = QByteArray::fromBase64(safeValue.toUtf8());
                QString codeStr = QString::fromUtf8(utf8Bytes);

                if (!codeStr.trimmed().isEmpty() && !codeStr.contains("请将代码复制粘贴至此处")) {
                    m_answersCache[realIdx] = codeStr;
                } else {
                    m_answersCache.remove(realIdx);
                }
                updateNavGrid();
            }
        }
    }
}

void StudentExamWidget::onPrevQuestion() { loadQuestion(currentQuestionIndex - 1); }
void StudentExamWidget::onNextQuestion() { loadQuestion(currentQuestionIndex + 1); }

void StudentExamWidget::onSaveTemporarily() {
    saveCurrentAnswer();
    QJsonObject finalAnswers;
    for (auto it = m_answersCache.begin(); it != m_answersCache.end(); ++it) finalAnswers[QString::number(it.key())] = QJsonValue::fromVariant(it.value());
    m_networkManager->saveExamProgress(m_currentExamId, finalAnswers);
    CapzeraDialog::information(this, "暂存成功", "当前答题进度已安全保存云端！");
    emit returnToDashboardRequested();
}

void StudentExamWidget::onSubmitExam() {
    if (CapzeraDialog::question(this, "最后确认", "确定要提交考卷吗？一旦提交将无法修改！")) {
        saveCurrentAnswer();
        examTimer->stop();
        QJsonObject finalAnswers;
        for (auto it = m_answersCache.begin(); it != m_answersCache.end(); ++it) finalAnswers[QString::number(it.key())] = QJsonValue::fromVariant(it.value());
        m_networkManager->sendAnswer(m_currentExamId, finalAnswers);
        CapzeraDialog::information(this, "交卷成功", "试卷已成功上传！");
        emit returnToDashboardRequested();
    }
}

void StudentExamWidget::updateCountdown() {
    if (remainingSeconds > 0) {
        remainingSeconds--;
        QString timeStr = QString("%1:%2:%3").arg(remainingSeconds / 3600, 2, 10, QChar('0')).arg((remainingSeconds % 3600) / 60, 2, 10, QChar('0')).arg(remainingSeconds % 60, 2, 10, QChar('0'));
        m_bigTimerLabel->setText(timeStr);
        if (remainingSeconds <= 300) m_bigTimerLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: white; background: #EF4444; border-radius: 12px; padding: 10px; font-family: Consolas;");
    } else {
        examTimer->stop();
        CapzeraDialog::warning(this, "时间到", "考试时间已到，系统正在自动强制交卷！");
        saveCurrentAnswer();
        QJsonObject finalAnswers;
        for (auto it = m_answersCache.begin(); it != m_answersCache.end(); ++it) finalAnswers[QString::number(it.key())] = QJsonValue::fromVariant(it.value());
        m_networkManager->sendAnswer(m_currentExamId, finalAnswers);
        emit returnToDashboardRequested();
    }
}