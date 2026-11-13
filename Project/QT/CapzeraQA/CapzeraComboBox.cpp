#include "CapzeraComboBox.h"
#include <QAbstractItemView>
#include <QPainterPath>

// ================ 渲染器实现 ================
void CapzeraComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);

    if (option.state & QStyle::State_Selected || option.state & QStyle::State_MouseOver) {
        painter->setBrush(QColor("#EFF6FF"));
    } else {
        painter->setBrush(Qt::white);
    }

    QRect bgRect = option.rect.adjusted(4, 2, -4, -2);
    painter->drawRoundedRect(bgRect, 6, 6);

    painter->setPen(QColor("#1E293B"));
    painter->setFont(QFont("Microsoft YaHei", 14));
    QRect textRect = bgRect.adjusted(12, 0, 0, 0);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
}

QSize CapzeraComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option); Q_UNUSED(index);
    return QSize(200, 38);
}

// ================ 下拉框本体实现 ================
CapzeraComboBox::CapzeraComboBox(QWidget *parent) : QComboBox(parent) {
    this->setItemDelegate(new CapzeraComboBoxDelegate(this));

    this->setStyleSheet(
        "QComboBox { "
        "   border: 2px solid #E2E8F0; border-radius: 8px; "
        "   padding: 5px 15px; min-height: 38px; background: white; "
        "   color: #1E293B; font-family: 'Microsoft YaHei'; font-size: 14px; font-weight: bold; "
        "}"
        "QComboBox:focus { border-color: #3B82F6; }"
        "QComboBox::drop-down { "
        "   width: 36px; border: none; background: transparent; "
        "}"
        "QComboBox::down-arrow { image: none; }"
        "QComboBox QAbstractItemView { "
        "   border: 1px solid #E2E8F0; border-radius: 8px; "
        "   background: white; outline: none; selection-background-color: transparent; "
        "}"
        );

    this->setCursor(Qt::PointingHandCursor);
    this->view()->setCursor(Qt::PointingHandCursor);

    this->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
}

// 🚀 终极绘制引擎：调整尺寸、比例与颜色！
void CapzeraComboBox::paintEvent(QPaintEvent *event) {
    QComboBox::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 🚀 缩小正方形：边缘呼吸距从 6 调大到 8
    int margin = 8;
    int sqSize = height() - margin * 2;
    QRect rightRect(width() - sqSize - margin, margin, sqSize, sqSize);

    QPainterPath path;
    path.addRoundedRect(rightRect, 5, 5); // 内部圆角稍微收一点，适配更小的正方形

    // 2. 🚀 调浅颜色：使用更加柔和的次级蓝 #60A5FA
    painter.fillPath(path, QColor("#60A5FA"));

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);

    int cx = rightRect.center().x();
    int cy = rightRect.center().y() + 1;

    // 3. 🚀 放大三角形：从 4 调大到 5，视觉上更加清晰饱满
    int size = 5;

    QPolygon polygon;
    polygon << QPoint(cx - size, cy - size + 1)
            << QPoint(cx + size, cy - size + 1)
            << QPoint(cx, cy + size);

    painter.drawPolygon(polygon);
}