#ifndef CAPZERACOMBOBOX_H
#define CAPZERACOMBOBOX_H

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPaintEvent>

// 🚀 专属渲染器：把枯燥的列表变成高级悬浮圆角选项！
class CapzeraComboBoxDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

// 🚀 高定下拉框本体
class CapzeraComboBox : public QComboBox {
    Q_OBJECT
public:
    explicit CapzeraComboBox(QWidget *parent = nullptr);

protected:
    // 🚀 核心大招：拦截绘制事件，亲手画出完美的蓝底白三角！
    void paintEvent(QPaintEvent *event) override;
};

#endif // CAPZERACOMBOBOX_H