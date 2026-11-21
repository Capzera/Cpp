#ifndef CARTESIANINFOPANEL_H
#define CARTESIANINFOPANEL_H
#include <QWidget>
#include <QString>
#include <QColor>

class QLabel;
class QVBoxLayout;

class CartesianInfoPanel : public QWidget {
    Q_OBJECT
public:
    explicit CartesianInfoPanel(QWidget* parent = nullptr);

    void showPoint(const QString& name, double x, double y, const QColor& color,
                   const QString& extra);
    void showSegment(const QString& equation, const QString& detail);
    void showCurve(const QString& name, const QString& expression, const QColor& color);
    void clearInfo();

    QString infoTitle() const;
    QString infoBody() const;
    QString infoHint() const;

signals:
    void closed();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QWidget* buildCard();
    void showCard(QWidget* card);

    QWidget* m_host = nullptr;
    QWidget* m_card = nullptr;
    QLabel* m_title = nullptr;
    QLabel* m_body = nullptr;
    QLabel* m_hint = nullptr;
};

#endif
