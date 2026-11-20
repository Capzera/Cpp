#ifndef MYSEGMENTEDCONTROL_H
#define MYSEGMENTEDCONTROL_H
#include <QAbstractButton>
#include <QVector>
#include <QStringList>
#include <QPropertyAnimation>
#include <QPainterPath>

class MySegmentedControl : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(qreal indicatorProgress READ indicatorProgress WRITE setIndicatorProgress)
public:
    enum ShapeIcon { NoIcon, RectIcon, CircleIcon, TriangleIcon };

    explicit MySegmentedControl(const QStringList& options, QWidget* parent = nullptr);

    QStringList options() const { return m_options; }
    int currentIndex() const { return m_current; }
    void setCurrentIndex(int index);
    void setAccentColor(const QColor& color);

    void setShapeIcons(const QVector<int>& icons) { m_shapeIcons = icons; update(); }
    void setShapeIcon(ShapeIcon icon) { m_shapeIcons = QVector<int>(m_options.size(), static_cast<int>(icon)); update(); }
    QVector<int> shapeIcons() const { return m_shapeIcons; }

    static QPainterPath buildIconPath(int icon, const QRectF& box);
    static void paintShapeIcon(QPainter* painter, int icon, const QRectF& box, const QColor& color, bool solid);

    qreal indicatorProgress() const { return m_indicatorProgress; }
    void setIndicatorProgress(qreal v);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    int indexAt(const QPointF& pos) const;
    void animateTo(int index);

    QStringList m_options;
    int m_current = 0;
    int m_hoverIndex = -1;
    QColor m_accent;
    qreal m_indicatorProgress = 0.0;
    QPropertyAnimation* m_indicatorAnim = nullptr;
    QVector<int> m_shapeIcons;
};

#endif
