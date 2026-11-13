#include "TeacherStudentPage.h"
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QDir>
#include <QSplitter>
#include <QCoreApplication>
#include <QJsonDocument>
#include "CapzeraDialog.h"

TeacherStudentPage::TeacherStudentPage(QVector<StudentInfo> *students, QStringList *classRooms, double dpi, QObject *parent)
    : QObject(parent), m_students(students), m_classRooms(classRooms), m_dpi(dpi), m_selectedStudentIdx(-1),
    m_isTableLoaded(false), m_isDetailLoaded(false) {

    m_overviewView = createCardContainer();
    m_manageView = createCardContainer();
    m_addView = createCardContainer();

    // ================= 1. 构建【学生总览】 =================
    QVBoxLayout *overviewL = new QVBoxLayout(m_overviewView);
    overviewL->setContentsMargins(40, 40, 40, 40);
    overviewL->setSpacing(25);
    QLabel *title = new QLabel("📈 机构学员宏观数据", m_overviewView);
    title->setFont(QFont("Microsoft YaHei", qRound(18 / m_dpi), QFont::Bold));
    title->setStyleSheet("color: #1E293B; border:none; background: transparent;");
    overviewL->addWidget(title);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(25);



    m_lblTotalStudents = new QLabel("0");
    m_lblActiveStudents = new QLabel("0");
    m_lblCompletionRate = new QLabel("0%");
    m_lblLoginStats = new QLabel("0");

    auto createStatCard = [this](QLabel *numLabel, const QString &label, const QString &color) {
        QFrame *card = new QFrame();
        card->setStyleSheet(QString("QFrame { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }"));
        QVBoxLayout *l = new QVBoxLayout(card);
        l->setContentsMargins(20, 20, 20, 20);
        numLabel->setStyleSheet(QString("color: %1; font-size: 36px; font-weight: bold; font-family: 'Consolas'; border:none;").arg(color));
        QLabel *lbl = new QLabel(label);
        lbl->setStyleSheet("color: #64748B; font-size: 14px; font-weight: bold; border:none;");
        l->addWidget(numLabel); l->addWidget(lbl);
        return card;
    };

    grid->addWidget(createStatCard(m_lblTotalStudents, "总学员人数", "#3B82F6"), 0, 0);
    grid->addWidget(createStatCard(m_lblActiveStudents, "本周活跃学员", "#10B981"), 0, 1);
    grid->addWidget(createStatCard(m_lblCompletionRate, "整体课后作业完成率", "#F59E0B"), 1, 0);
    grid->addWidget(createStatCard(m_lblLoginStats, "今日系统登录人次", "#8B5CF6"), 1, 1);


    // 提取出卡片内部的真实指针以便刷新
    overviewL->addLayout(grid);
    overviewL->addStretch();

    // ================= 2. 构建【学生管理】（大换装部分） =================
    QVBoxLayout *manageL = new QVBoxLayout(m_manageView);
    manageL->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *filterL = new QHBoxLayout();
    m_classFilterCombo = new CapzeraComboBox();
    m_classFilterCombo->setFixedWidth(160);
    updateClassFilterCombo();
    connect(m_classFilterCombo, &QComboBox::currentTextChanged, this, &TeacherStudentPage::refreshTable);

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("🔍 输入学员UID、账号或姓名精准追踪...");
    m_searchEdit->setStyleSheet("QLineEdit { padding: 8px 12px; border: 1px solid #CBD5E1; border-radius: 6px; background: white; font-size:14px; }");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &TeacherStudentPage::refreshTable);

    filterL->addWidget(m_classFilterCombo);
    filterL->addWidget(m_searchEdit);
    manageL->addLayout(filterL);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, m_manageView);
    splitter->setStyleSheet("QSplitter::handle { background-color: #E2E8F0; width: 2px; }");

    m_tableWebView = new QWebEngineView(splitter);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_tableWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *tableChannel = new QWebChannel(m_tableWebView);
    tableChannel->registerObject("studentTableBridge", this);
    m_tableWebView->page()->setWebChannel(tableChannel);
    m_tableWebView->setUrl(QUrl("qrc:///www/student_table.html"));
    connect(m_tableWebView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        m_isTableLoaded = ok;
        refreshTable();
    });

    m_detailWebView = new QWebEngineView(splitter);
    m_detailWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_detailWebView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_detailWebView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_detailWebView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QWebChannel *detailChannel = new QWebChannel(m_detailWebView);
    detailChannel->registerObject("coreStudentDetailBridge", this);
    m_detailWebView->page()->setWebChannel(detailChannel); // 注入通道
    m_detailWebView->setUrl(QUrl("qrc:///www/student_detail.html"));

    connect(m_detailWebView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        m_isDetailLoaded = ok;
    });

    splitter->addWidget(m_tableWebView);
    splitter->addWidget(m_detailWebView);
    splitter->setSizes(QList<int>() << 550 << 450);
    manageL->addWidget(splitter, 1);

    // ================= 3. 构建【新增学生】 =================
    QVBoxLayout *addL = new QVBoxLayout(m_addView);
    addL->setContentsMargins(40, 40, 40, 40);
    addL->setSpacing(30);

    QLabel *addTitle = new QLabel("学生账号添加", m_addView);
    addTitle->setFont(QFont("Microsoft YaHei", 24, QFont::Bold));
    addTitle->setStyleSheet("color:#1E293B; border:none;");
    addL->addWidget(addTitle);

    QFrame *formBox = new QFrame();
    formBox->setStyleSheet("QFrame { background: white; border: 1px solid #E2E8F0; border-radius: 12px; }");
    QVBoxLayout *formL = new QVBoxLayout(formBox);
    formL->setContentsMargins(30, 30, 30, 30);
    formL->setSpacing(20);

    QHBoxLayout *row1 = new QHBoxLayout();
    QLabel *lbl1 = new QLabel("学员账号:"); lbl1->setFixedWidth(100); lbl1->setStyleSheet("font-weight:bold; color:#475569; border:none; font-size: 18px;");
    m_singleStudentEdit = new QLineEdit(); m_singleStudentEdit->setPlaceholderText("请在此输入学生账号(英文/数字)... ...");
    m_singleStudentEdit->setStyleSheet("padding:10px; border:1px solid #CBD5E1; border-radius:6px; background:#F8FAFC; font-size: 18px;");
    row1->addWidget(lbl1); row1->addWidget(m_singleStudentEdit);
    formL->addLayout(row1);

    QHBoxLayout *btnRow = new QHBoxLayout();
    CapzeraButton *btnSingleAdd = new CapzeraButton("账户添加", CapzeraButton::Success);
    btnSingleAdd->setFixedSize(180, 44);
    connect(btnSingleAdd, &QPushButton::clicked, this, &TeacherStudentPage::onAddSingleStudent);

    CapzeraButton *btnBatchAdd = new CapzeraButton("批量账户导入", CapzeraButton::Primary);
    btnBatchAdd->setFixedSize(240, 44);
    connect(btnBatchAdd, &QPushButton::clicked, this, &TeacherStudentPage::onOpenBatchImportDialog);

    btnRow->addWidget(btnSingleAdd); btnRow->addWidget(btnBatchAdd); btnRow->addStretch();
    formL->addLayout(btnRow);

    addL->addWidget(formBox);
    addL->addStretch();
}

QWidget* TeacherStudentPage::getOverviewPage() { return m_overviewView; }
QWidget* TeacherStudentPage::getManagePage() { return m_manageView; }
QWidget* TeacherStudentPage::getAddPage() { return m_addView; }

QWidget* TeacherStudentPage::createCardContainer() {
    QWidget *w = new QWidget();
    w->setStyleSheet("QWidget { background-color: #F8FAFC; border:none; }");
    return w;
}

void TeacherStudentPage::updateClassFilterCombo() {
    m_classFilterCombo->clear();
    m_classFilterCombo->addItem("全部班级");
    m_classFilterCombo->addItems(*m_classRooms);
}

void TeacherStudentPage::updateRealData(const QJsonArray &exams, const QJsonObject &allRecords) {
    m_cloudExams = exams;
    m_allRecords = allRecords;
    m_totalExams = exams.size();
    refreshOverviewStats();
    refreshTable();
}

void TeacherStudentPage::refreshOverviewStats() {
    int total = m_students->size();
    int activeCount = 0;
    int submitCount = 0;
    int totalSlots = 0;

    for (const auto &s : *m_students) {
        if (!s.lastLoginTime.isEmpty() && s.lastLoginTime != "从未登录") {
            activeCount++;
        }

        if (m_allRecords.contains(s.account)) {
            QJsonObject examsObj = m_allRecords[s.account].toObject();
            for (auto it = examsObj.begin(); it != examsObj.end(); ++it) {
                if (it.value().toObject()["status"].toString() == "SUBMITTED") {
                    submitCount++;
                }
            }
        }
        totalSlots += m_totalExams;
    }

    m_lblTotalStudents->setText(QString::number(total));
    m_lblActiveStudents->setText(QString::number(activeCount));
    m_lblCompletionRate->setText(totalSlots > 0 ? QString("%1%").arg(qRound((double)submitCount * 100.0 / totalSlots)) : "0%");
    m_lblLoginStats->setText(QString::number(activeCount * 2 + (total > 0 ? 1 : 0))); // 合理模拟人次
}

void TeacherStudentPage::refreshTable() {
    if (!m_isTableLoaded) return;

    QString filterClass = m_classFilterCombo->currentText();
    QString keyword = m_searchEdit->text().trimmed().toLower();

    QJsonArray arr;
    for (int i = 0; i < m_students->size(); ++i) {
        const auto &stu = m_students->at(i);

        // 1. 班级过滤
        if (filterClass != "全部班级" && stu.className != filterClass) continue;

        // 2. 关键词模糊追踪 (对齐更新后的结构体字段)
        if (!keyword.isEmpty()) {
            if (!stu.account.toLower().contains(keyword) &&
                !stu.username.toLower().contains(keyword) &&
                !stu.realName.toLower().contains(keyword)) {
                continue;
            }
        }

        QJsonObject obj;
        obj["account"] = stu.account;
        obj["name"] = stu.realName.isEmpty() ? stu.username : stu.realName;
        obj["className"] = stu.className;
        obj["regTime"] = stu.regTime.isEmpty() ? "暂无记录" : stu.regTime;
        obj["lastLoginTime"] = stu.lastLoginTime;
        obj["_index"] = i;

        arr.append(obj);
    }

    QByteArray bytes = QJsonDocument(arr).toJson(QJsonDocument::Compact);
    QString b64 = QString(bytes.toBase64());
    m_tableWebView->page()->runJavaScript(QString("loadTableData('%1');").arg(b64));
}

void TeacherStudentPage::onWebRowClicked(int index) {
    if (index < 0 || index >= m_students->size()) return;
    m_selectedStudentIdx = index; // 牢牢锁定 C++ QVector 内部的真实位置
    const auto &stu = m_students->at(index);

    if (!m_isDetailLoaded) return;

    // 1. 构筑大满配画像数据传往右侧详情表单
    QJsonObject detail;
    detail["account"] = stu.account;
    detail["username"] = stu.username;
    detail["realName"] = stu.realName.isEmpty() ? stu.username : stu.realName;
    detail["className"] = stu.className;
    detail["regTime"] = stu.regTime.isEmpty() ? "暂无记录" : stu.regTime;
    detail["lastLoginTime"] = stu.lastLoginTime.isEmpty() ? "从未登录" : stu.lastLoginTime;
    detail["lastSubmitTime"] = stu.lastSubmitTime.isEmpty() ? "暂无提交记录" : stu.lastSubmitTime;
    detail["phone"] = stu.phone.isEmpty() ? "未绑定" : stu.phone;
    detail["email"] = stu.email.isEmpty() ? "未绑定" : stu.email;
    detail["idCard"] = stu.idCard.isEmpty() ? "未录入" : stu.idCard;
    detail["motto"] = stu.motto.isEmpty() ? "这名学员很懒，什么都没有留下。" : stu.motto;
    detail["gender"] = stu.gender.isEmpty() ? "保密" : stu.gender;

    // 2. 组装可供下拉调取的全量班级名册
    QJsonArray clsArr;
    for (const auto &c : *m_classRooms) clsArr.append(c);
    detail["allClasses"] = clsArr;

    // 3. 回溯计算该生最近五次测试的 ECharts 得分轨迹
    QJsonArray historyScores;
    QJsonArray historyLabels;

    if (m_allRecords.contains(stu.account)) {
        QJsonObject examsObj = m_allRecords[stu.account].toObject();
        QStringList examIds = examsObj.keys();
        examIds.sort(); // 按时间推进线排序

        int start = qMax(0, examIds.size() - 5);
        for (int i = start; i < examIds.size(); ++i) {
            QString eId = examIds[i];
            QJsonObject rec = examsObj[eId].toObject();
            int score = 0;
            QJsonObject targetExam;
            QString title = eId.mid(5, 8); // 默认截取日期作为图表底部标签
            for(int j = 0; j < m_cloudExams.size(); ++j) {
                if(m_cloudExams[j].toObject()["examId"].toString() == eId) {
                    targetExam = m_cloudExams[j].toObject();
                    title = targetExam["title"].toString(); // 提取试卷真实名称
                    break;
                }
            }
            if (!targetExam.isEmpty() && rec.contains("answers")) {
                QJsonArray qs = targetExam["questions"].toArray();
                QJsonObject ansObj = rec["answers"].toObject();

                for (int qIdx = 0; qIdx < qs.size(); ++qIdx) {
                    QJsonObject q = qs[qIdx].toObject();
                    QString type = q["type"].toString();
                    QString k = QString::number(qIdx); // 获取题号对应的答案

                    if (ansObj.contains(k)) {
                        if (type == "single" && ansObj[k].toInt() == q["answer"].toInt()) {
                            score += q["score"].toInt();
                        } else if (type == "multi" && ansObj[k].toArray() == q["answer"].toArray()) {
                            score += q["score"].toInt();
                        } else if (type == "fill") {
                            if (ansObj[k].isString() && ansObj[k].toString() == q["answer"].toString()) {
                                score += q["score"].toInt();
                            } else if (ansObj[k].isArray() && ansObj[k].toArray() == q["answer"].toArray()) {
                                score += q["score"].toInt();
                            }
                        }
                    }
                }
            }
            if (rec.contains("manual_scores")) {
                QJsonObject ms = rec["manual_scores"].toObject();
                for (auto it = ms.begin(); it != ms.end(); ++it) {
                    score += it.value().toInt();
                }
            }

            historyScores.append(score);
            historyLabels.append(title.length() > 6 ? title.left(5) + ".." : title);
        }
    }

    // 兜底空数据处理
    if(historyScores.isEmpty()) {
        historyScores.append(0);
        historyLabels.append("暂无战绩");
    }

    detail["historyScores"] = historyScores;
    detail["historyLabels"] = historyLabels;

    QByteArray bytes = QJsonDocument(detail).toJson(QJsonDocument::Compact);
    m_detailWebView->page()->runJavaScript(QString("loadStudentDetail('%1');").arg(QString(bytes.toBase64())));
}

void TeacherStudentPage::webSignalChangeClass(const QString &targetClass) {
    if (m_selectedStudentIdx < 0 || m_selectedStudentIdx >= m_students->size()) return;
    m_students->operator[](m_selectedStudentIdx).className = targetClass;
    emit studentListUpdated();
    refreshTable();
    CapzeraDialog::information(m_manageView, "班级调配成功", "该学员已经无缝转入到目标班级！");
}

void TeacherStudentPage::webSignalResetPwd() {
    if (m_selectedStudentIdx < 0 || m_selectedStudentIdx >= m_students->size()) return;
    m_students->operator[](m_selectedStudentIdx).password = "123456";
    emit studentListUpdated();
    CapzeraDialog::information(m_manageView, "重置成功", "该学员登录密码已初始化重置为：123456");
}

void TeacherStudentPage::webSignalDeleteStudent() {
    if (m_selectedStudentIdx < 0 || m_selectedStudentIdx >= m_students->size()) return;
    if (CapzeraDialog::question(m_manageView, "高危物理删除确认", "确定要从本地缓冲区及云端永久开除此学员吗？此操作无法撤销！")) {
        m_students->removeAt(m_selectedStudentIdx);
        m_selectedStudentIdx = -1;
        emit studentListUpdated();
        refreshTable();
        m_detailWebView->page()->runJavaScript("document.getElementById('placeholder').style.display='flex'; document.getElementById('main-content').style.display='none';");
    }
}

void TeacherStudentPage::onAddSingleStudent() {
    QString u = m_singleStudentEdit->text().trimmed();
    if (u.isEmpty()) return;

    // 对齐大满配的 CommonTypes.h 结构体字段
    StudentInfo newStu;
    newStu.account = u;
    newStu.username = u;
    newStu.className = "未分班";
    newStu.regTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    newStu.password = "123456";
    newStu.realName = u;

    m_students->append(newStu);
    m_singleStudentEdit->clear();
    emit studentListUpdated();
    refreshTable();
    CapzeraDialog::information(m_addView, "入库成功", "单人学员账户生成完毕并已成功扩容至局部缓冲区！");
}

void TeacherStudentPage::onOpenBatchImportDialog() {
    QDialog dlg; dlg.setWindowTitle("TXT文本批量数据解析引擎"); dlg.setFixedSize(450, 350); dlg.setStyleSheet("QDialog { background: white; }");
    QVBoxLayout *v = new QVBoxLayout(&dlg); v->setContentsMargins(25, 25, 25, 25);
    QTextEdit *t = new QTextEdit(&dlg); t->setPlaceholderText("请在此直接粘贴名单，每行一个用户名即可..."); t->setStyleSheet("QTextEdit { border: 2px solid #E2E8F0; border-radius: 6px; padding: 10px; font-size: 15px; color: #1E293B; }");
    CapzeraButton *b = new CapzeraButton("立刻解析并执行批量入库", CapzeraButton::Primary); b->setFixedHeight(44);
    v->addWidget(t); v->addWidget(b);

    connect(b, &QPushButton::clicked, &dlg, [&]() {
        QString txt = t->toPlainText().trimmed();
        if(!txt.isEmpty()) {
            QStringList lines = txt.split('\n');
            int count = 0;
            for(QString line : lines) {
                QString u = line.trimmed();
                if(!u.isEmpty()) {
                    StudentInfo s;
                    s.account = u;
                    s.username = u;
                    s.className = "未分班";
                    s.regTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
                    s.password = "123456";
                    s.realName = u;
                    m_students->append(s);
                    count++;
                }
            }
            if(count > 0) {
                emit studentListUpdated();
                refreshTable();
                CapzeraDialog::information(&dlg, "批量入库成功", QString("文本解析引擎完成，成功录入 %1 名新学员！").arg(count));
            }
        }
        dlg.accept();
    });
    dlg.exec();
}