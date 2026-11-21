#ifndef MYTEXTINPUT_H
#define MYTEXTINPUT_H
#include <QWidget>
#include <QString>

class QLineEdit;

class MyTextInput : public QWidget {
    Q_OBJECT
public:
    explicit MyTextInput(QWidget* parent = nullptr);

    QString text() const;
    void setText(const QString& text);
    void setPlaceholderText(const QString& text);
    void setMaxLength(int n);
    void selectAll();

signals:
    void textChanged(const QString& text);
    void editingFinished();
    void returnPressed();

protected:
    void paintEvent(QPaintEvent*) override;
    void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    QLineEdit* m_edit = nullptr;
    QString m_placeholder;
    bool m_focused = false;
};

#endif
