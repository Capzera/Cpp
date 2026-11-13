#include "ClassDashboardWidget.h"
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QWebEngineSettings>

ClassDashboardWidget::ClassDashboardWidget(QWidget *parent) : QWidget(parent) {
    this->setStyleSheet("QWidget { background-color: #F8FAFC; font-family: 'Microsoft YaHei'; }");
    initUI();
}

void ClassDashboardWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(25);
    cardsLayout->addWidget(createDataCard("🚀 累计发布测验", "-- 场", "#3B82F6", "#EFF6FF", lblTotalExams));
    cardsLayout->addWidget(createDataCard("📝 累计收到答卷", "-- 份", "#10B981", "#ECFDF5", lblTotalSubmissions));
    cardsLayout->addWidget(createDataCard("📊 全班平均分", "-- 分", "#F59E0B", "#FFFBEB", lblAvgScore));
    cardsLayout->addWidget(createDataCard("🏆 最高纪录分", "-- 分", "#EF4444", "#FEF2F2", lblTopScore));
    mainLayout->addLayout(cardsLayout, 1);

    QFrame *chartFrame = new QFrame();
    QVBoxLayout *chartLayout = new QVBoxLayout(chartFrame);
    chartLayout->setContentsMargins(0, 0, 0, 0);

    m_webView = new QWebEngineView(chartFrame);

    // 🚀 核心修复：建立本地加载完成信号连接，让 m_isPageLoaded 正确变为 true 并无缝补充渲染数据
    connect(m_webView, &QWebEngineView::loadFinished, this, [this](bool success) {
        if (success) {
            m_isPageLoaded = true;
            if (!m_cachedStats.isEmpty()) {
                updateDashboardData(m_cachedStats);
            }
        }
    });

    m_webView->setUrl(QUrl("qrc:///www/dashboard.html"));
    chartLayout->addWidget(m_webView);

    mainLayout->addWidget(chartFrame, 3);
}

QFrame* ClassDashboardWidget::createDataCard(const QString &title, const QString &value, const QString &color, const QString &bgColor, QLabel *&valLabelRef) {
    QFrame *card = new QFrame();
    card->setStyleSheet(QString("QFrame { background-color: %1; border-radius: 12px; border: 1px solid #E2E8F0; }").arg(bgColor));
    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 15, 20, 15);

    QLabel *lblTitle = new QLabel(title);
    lblTitle->setStyleSheet("color: #64748B; font-size: 14px; border: none; background: transparent;");

    valLabelRef = new QLabel(value);
    valLabelRef->setStyleSheet(QString("color: %1; font-size: 28px; font-weight: bold; border: none; background: transparent; font-family: 'Consolas', sans-serif;").arg(color));

    layout->addWidget(lblTitle);
    layout->addWidget(valLabelRef);
    return card;
}

void ClassDashboardWidget::updateDashboardData(const QJsonObject &stats) {
    m_cachedStats = stats;

    lblTotalExams->setText(QString::number(stats["totalExams"].toInt()) + " 场");
    lblTotalSubmissions->setText(QString::number(stats["totalSubmissions"].toInt()) + " 份");
    lblAvgScore->setText(QString::number(stats["avgScore"].toDouble(), 'f', 1) + " 分");
    lblTopScore->setText(QString::number(stats["topScore"].toInt()) + " 分");

    if (!m_isPageLoaded) return;

    QJsonArray categoriesArr;
    QJsonArray valuesArr;
    QJsonArray recent = stats["recentExams"].toArray();
    for(int i = 0; i < recent.size(); ++i) {
        QJsonObject ex = recent[i].toObject();
        categoriesArr.append(ex["name"].toString());
        valuesArr.append(ex["avg"].toDouble());
    }
    QJsonObject barDataObj;
    barDataObj["categories"] = categoriesArr;
    barDataObj["values"] = valuesArr;

    QJsonObject pr = stats["passRate"].toObject();
    QJsonArray pieDataArr;
    auto appendPie = [&pieDataArr](const QString& name, int value) {
        QJsonObject obj; obj["name"] = name; obj["value"] = value; pieDataArr.append(obj);
    };
    appendPie("优秀 (90+)", pr["excellent"].toInt());
    appendPie("良好 (75+)", pr["good"].toInt());
    appendPie("及格 (60+)", pr["pass"].toInt());
    appendPie("不及格", pr["fail"].toInt());

    QJsonObject finalPayloadObj;
    finalPayloadObj["barData"] = barDataObj;
    finalPayloadObj["pieData"] = pieDataArr;

    QByteArray base64Data = QJsonDocument(finalPayloadObj).toJson(QJsonDocument::Compact).toBase64();
    QString jsCommand = QString("updateDashboard('%1');").arg(QString(base64Data));

    m_webView->page()->runJavaScript(jsCommand);
}