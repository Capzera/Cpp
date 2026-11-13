#ifndef MYCOLLAPSIBLE_H
#define MYCOLLAPSIBLE_H
#include <QWidget>
#include <QPropertyAnimation>
#include <QVector>

class MyCollapsible : public QWidget {
    Q_OBJECT
public:
    explicit MyCollapsible(QWidget* parent = nullptr);

    void setContentWidget(QWidget* content);
    void setCollapsed(bool collapsed);
    bool isCollapsed() const { return m_collapsed; }
    void setHeaderText(const QString& text);
    void setAccentColor(const QColor& c);
    void setDuration(int ms);

signals:
    void collapseStateChanged(bool collapsed);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    QSize sizeHint() const override;

private:
    void updateGeometryAnimated();

    QWidget* m_content = nullptr;
    QWidget* m_header = nullptr;
    QString m_headerText;
    QColor m_accent;
    bool m_collapsed = false;
    int m_duration = 220;
    qreal m_ratio = 1.0;
    QPropertyAnimation* m_anim = nullptr;
    int m_contentHeight = 0;
};

#endif
