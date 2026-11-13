#ifndef MYRADIOBUTTON_H
#define MYRADIOBUTTON_H
#include <QAbstractButton>
#include <QString>

class MyRadioButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    explicit MyRadioButton(const QString& text = QString(), QWidget* parent = nullptr);

    QString text() const { return m_text; }
    void setText(const QString& text);
    bool isChecked() const { return m_checked; }
    void setChecked(bool on);

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
};

#endif
