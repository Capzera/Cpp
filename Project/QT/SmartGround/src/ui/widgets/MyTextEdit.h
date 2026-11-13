#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H
#include <QTextEdit>
#include <QPainter>

class MyTextEdit : public QTextEdit {
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget* parent = nullptr);

    QString placeholderText() const { return m_hint; }
    void setPlaceholderText(const QString& text);

protected:
    void paintEvent(QPaintEvent*) override;
    void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private slots:
    void onTextChanged();

private:
    void renderBackground();
    void renderBorder();
    void renderPlaceholder();

    QString m_hint;
    bool m_focused = false;
};

#endif
