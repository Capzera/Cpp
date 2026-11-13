#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H
#include <QAbstractButton>
#include <QString>

class MyCheckBox : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(qreal ratio READ ratio WRITE setRatio)
public:
    explicit MyCheckBox(const QString& text = QString(), QWidget* parent = nullptr);

    QString text() const { return m_text; }
    void setText(const QString& text);
    bool isChecked() const { return m_checked; }
    void setChecked(bool on);
    qreal ratio() const { return m_ratio; }
    void setRatio(qreal v);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    QString m_text;
    bool m_checked = false;
    bool m_hover = false;
    qreal m_ratio = 1.0;
};

#endif
