#ifndef CLASSDASHBOARDWIDGET_H
#define CLASSDASHBOARDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebEngineView>

class ClassDashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit ClassDashboardWidget(QWidget *parent = nullptr);

public slots:
    void updateDashboardData(const QJsonObject &stats);

private:
    void initUI();
    QFrame* createDataCard(const QString &title, const QString &value, const QString &color, const QString &bgColor, QLabel *&valLabelRef);

    QLabel *lblTotalExams;
    QLabel *lblTotalSubmissions;
    QLabel *lblAvgScore;
    QLabel *lblTopScore;

    QWebEngineView *m_webView;

    // 🚀 核心缓冲盾牌：防白屏
    bool m_isPageLoaded = false;
    QJsonObject m_cachedStats;
};

#endif // CLASSDASHBOARDWIDGET_H