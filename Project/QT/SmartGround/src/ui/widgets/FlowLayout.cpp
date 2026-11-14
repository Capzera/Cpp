#include "FlowLayout.h"
#include <QWidget>
#include <QStyle>

FlowLayout::FlowLayout(QWidget* parent, int margin) : QLayout(parent) {
    if (margin >= 0) setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout() {
    while (QLayoutItem* item = takeAt(0)) {
        delete item;
    }
}

void FlowLayout::addItem(QLayoutItem* item) {
    if (!item) return;
    m_items.append(item);
}

int FlowLayout::horizontalSpacing() const { return m_hSpace; }
int FlowLayout::verticalSpacing() const { return m_vSpace; }

void FlowLayout::setHorizontalSpacing(int spacing) {
    m_hSpace = spacing;
    invalidate();
}

void FlowLayout::setVerticalSpacing(int spacing) {
    m_vSpace = spacing;
    invalidate();
}

int FlowLayout::count() const { return m_items.size(); }

QLayoutItem* FlowLayout::itemAt(int index) const {
    if (index < 0 || index >= m_items.size()) return nullptr;
    return m_items.at(index);
}

QLayoutItem* FlowLayout::takeAt(int index) {
    if (index < 0 || index >= m_items.size()) return nullptr;
    return m_items.takeAt(index);
}

Qt::Orientations FlowLayout::expandingDirections(const QRect& rect) const {
    Q_UNUSED(rect);
    return Qt::Orientations();
}

bool FlowLayout::hasHeightForWidth() const { return true; }

int FlowLayout::heightForWidth(int width) const {
    return doLayout(QRect(0, 0, width, 0), true);
}

int FlowLayout::minimumHeightForWidth(int width) const {
    Q_UNUSED(width);
    return minimumSize().height();
}

int FlowLayout::itemSpacing(const QWidget* widget, bool horizontal) const {
    if (!widget) return horizontal ? m_hSpace : m_vSpace;
    if (horizontal) {
        if (m_hSpace >= 0) return m_hSpace;
        return widget->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
    }
    if (m_vSpace >= 0) return m_vSpace;
    return widget->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
}

int FlowLayout::doLayout(const QRect& rect, bool testOnly) const {
    QRect effective = rect;
    effective.adjust(contentsMargins().left(), contentsMargins().top(),
                     -contentsMargins().right(), -contentsMargins().bottom());
    int x = effective.x();
    int y = effective.y();
    int lineHeight = 0;
    const int maxWidth = effective.width();

    for (int i = 0; i < m_items.size(); ++i) {
        QLayoutItem* item = m_items.at(i);
        if (!item || item->isEmpty()) continue;
        QWidget* widget = item->widget();
        if (widget && widget->isHidden()) continue;

        const int spaceX = qMax(0, itemSpacing(widget, true));
        const int spaceY = qMax(0, itemSpacing(widget, false));
        const QSize itemSize = item->sizeHint();

        int next = x + itemSize.width();
        int nextLine = y + lineHeight + spaceY;
        if (next - effective.x() > maxWidth && lineHeight > 0) {
            x = effective.x();
            y = nextLine;
            next = x + itemSize.width();
            nextLine = y + lineHeight + spaceY;
        }
        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), itemSize));
        }
        x = next + spaceX;
        lineHeight = qMax(lineHeight, itemSize.height());
    }
    return y + lineHeight - rect.y() + contentsMargins().bottom();
}

void FlowLayout::setGeometry(const QRect& rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const {
    return minimumSize();
}

QSize FlowLayout::minimumSize() const {
    QSize size;
    int lineWidth = 0;
    int lineHeight = 0;
    int maxLineWidth = 0;

    for (int i = 0; i < m_items.size(); ++i) {
        QLayoutItem* item = m_items.at(i);
        if (!item || item->isEmpty()) continue;
        QWidget* widget = item->widget();
        if (widget && widget->isHidden()) continue;
        const QSize itemSize = item->sizeHint();
        lineWidth += itemSize.width();
        lineHeight = qMax(lineHeight, itemSize.height());
        maxLineWidth = qMax(maxLineWidth, lineWidth);
        lineWidth += qMax(0, itemSpacing(widget, true));
    }
    size.setWidth(maxLineWidth);
    size.setHeight(lineHeight);
    return size;
}
