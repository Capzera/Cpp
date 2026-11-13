#ifndef CAPZERABUTTON_H
#define CAPZERABUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QColor>
#include <QEnterEvent>

class CapzeraButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)
public:

    enum Theme { Primary, Success, Warning, Danger, Secondary };
    explicit CapzeraButton(const QString &text, Theme theme = Primary, QWidget *parent = nullptr);

    QColor bgColor() const;
    void setBgColor(const QColor &color);
    void setTheme(Theme n_theme);
    void setFontSize(int x);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    Theme m_theme;
    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_currentColor;
    QPropertyAnimation *m_anim;
};

#endif // CAPZERABUTTON_H