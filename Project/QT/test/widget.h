#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>   // 直接包含完整定义

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // WIDGET_H