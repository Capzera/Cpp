#ifndef TOOLICONBUTTON_H
#define TOOLICONBUTTON_H
#include <QAbstractButton>
#include <QColor>

class ToolIconButton : public QAbstractButton {
    Q_OBJECT
public:
    enum Kind {
        Select,
        Marquee,
        Pen,
        Highlighter,
        Eraser,
        Line,
        Rect,
        Circle,
        Arrow,
        Triangle,
        Mosaic,
        Text,
        Markdown,
        Latex,
        Table,
        Style,
        More,
        Undo,
        Redo,
        CartPoint,
        CartLine,
        CartFunction
    };
    Q_ENUM(Kind)

    explicit ToolIconButton(Kind kind, const QString& tip, QWidget* parent = nullptr);

    Kind kind() const { return m_kind; }
    void setActive(bool on);
    bool isActive() const { return m_active; }
    void setBadge(const QString& text);
    QString badge() const { return m_badge; }

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    static void paintIcon(QPainter& painter, Kind kind, const QRectF& box, const QColor& color);

    Kind m_kind = Select;
    bool m_active = false;
    bool m_hover = false;
    bool m_pressed = false;
    QString m_badge;
};

#endif
