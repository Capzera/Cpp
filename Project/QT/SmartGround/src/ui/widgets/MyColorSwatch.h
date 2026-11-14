#ifndef MYCOLORSWATCH_H
#define MYCOLORSWATCH_H
#include <QWidget>
#include <QColor>

class MyColorSwatch : public QWidget {
    Q_OBJECT
public:
    explicit MyColorSwatch(QWidget* parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor& c);
    void setTitle(const QString& title);

signals:
    void clicked();
    void colorPicked(const QColor& c);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    QColor m_color = QColor(31, 39, 51);
    QString m_title;
    bool m_hover = false;
};

#endif
