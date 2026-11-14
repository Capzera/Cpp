#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H
#include <QLayout>
#include <QList>
#include <QRect>

class FlowLayout : public QLayout {
    Q_OBJECT
public:
    explicit FlowLayout(QWidget* parent, int margin = -1);
    ~FlowLayout() override;

    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    void setHorizontalSpacing(int spacing);
    void setVerticalSpacing(int spacing);

    QSize sizeHint() const override;
    QSize minimumSize() const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QLayoutItem* takeAt(int index) override;
    Qt::Orientations expandingDirections(const QRect& rect) const;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;
    int minimumHeightForWidth(int width) const override;
    void setGeometry(const QRect& rect) override;

private:
    int doLayout(const QRect& rect, bool testOnly) const;
    int itemSpacing(const QWidget* widget, bool horizontal) const;

    QList<QLayoutItem*> m_items;
    int m_hSpace = 8;
    int m_vSpace = 8;
};

#endif
