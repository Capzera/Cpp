#include "MyColorSwatch.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

namespace {
const QColor& paletteAt(int index) {
    static const QColor palette[] = {
        QColor(31, 39, 51), QColor(59, 125, 221), QColor(219, 112, 47),
        QColor(52, 199, 89), QColor(229, 72, 77), QColor(123, 97, 255),
        QColor(0, 150, 136), QColor(196, 106, 178), QColor(255, 214, 140)
    };
    static const int count = 9;
    return palette[index % count];
}
}

MyColorSwatch::MyColorSwatch(QWidget* parent) : QWidget(parent) {
    setFixedSize(30, 30);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setToolTip("点击选择画笔颜色");
}

void MyColorSwatch::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    update();
}

void MyColorSwatch::setTitle(const QString& title) {
    if (m_title == title) return;
    m_title = title;
    update();
}

void MyColorSwatch::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(1.5, 1.5, -1.5, -1.5);
    QPainterPath path;
    path.addRoundedRect(box, 8, 8);
    painter.setPen(QPen(m_hover ? UiTheme::accent() : UiTheme::borderStrong(), m_hover ? 2.0 : 1.2));
    painter.setBrush(m_color);
    painter.drawPath(path);
}

void MyColorSwatch::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    emit clicked();
    QMenu menu(this);
    menu.setObjectName("colorMenu");
    QAction* action = nullptr;
    for (int i = 0; i < 9; ++i) {
        action = menu.addAction(QString("颜色 %1").arg(i + 1));
        action->setIcon(QIcon());
        const QColor c = paletteAt(i);
        QPixmap pm(14, 14);
        pm.fill(c);
        action->setIcon(QIcon(pm));
        connect(action, &QAction::triggered, this, [this, c]() {
            setColor(c);
            emit colorPicked(c);
        });
    }
    menu.exec(event->globalPosition().toPoint());
    event->accept();
}

void MyColorSwatch::enterEvent(QEnterEvent*) {
    m_hover = true;
    update();
}

void MyColorSwatch::leaveEvent(QEvent*) {
    m_hover = false;
    update();
}
