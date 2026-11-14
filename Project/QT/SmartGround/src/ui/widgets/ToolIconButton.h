#ifndef TOOLICONBUTTON_H
#define TOOLICONBUTTON_H
#include <QAbstractButton>
#include <QColor>
#include <QPainter>

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
        Redo
    };
    Q_ENUM(Kind)

    explicit ToolIconButton(Kind kind, const QString& tip, QWidget* parent = nullptr);

    Kind kind() const { return m_kind; }
    void setActive(bool on);
    bool isActive() const { return m_active; }

    static void paintIcon(QPainter& painter, Kind kind, const QRectF& box, const QColor& color);

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    Kind m_kind;
    bool m_active = false;
    bool m_hover = false;
    bool m_pressed = false;
};

#endif
