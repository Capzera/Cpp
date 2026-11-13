#include "widget.h"
#include <QDebug>
#include <QtGui/qkeyevent.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(400, 300);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed:" << event->key();   // 使用 key()
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}