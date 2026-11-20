#ifndef TABLEICONBUTTON_H
#define TABLEICONBUTTON_H
#include <QAbstractButton>
#include <QColor>

class TableIconButton : public QAbstractButton {
    Q_OBJECT
public:
    enum Icon {
        AlignLeft,
        AlignCenter,
        AlignRight,
        AlignTop,
        SelectCell,
        SelectRow,
        SelectColumn,
        InsertRow,
        InsertColumn,
        DeleteRow,
        DeleteColumn
    };
    Q_ENUM(Icon)

    explicit TableIconButton(Icon icon, const QString& tip, QWidget* parent = nullptr);

    Icon icon() const { return m_icon; }

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    static void paintIcon(QPainter& painter, Icon icon, const QRectF& box, const QColor& color);

    Icon m_icon = AlignLeft;
    bool m_hover = false;
};

#endif
