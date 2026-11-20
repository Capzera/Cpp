#ifndef CANVASBRUSHSLIDER_H
#define CANVASBRUSHSLIDER_H
#include <QWidget>

class ToolController;

class CanvasBrushSlider : public QWidget {
    Q_OBJECT
public:
    explicit CanvasBrushSlider(ToolController* controller, QWidget* parent = nullptr);

    void setHostWidget(QWidget* host);
    void reposition();
    void syncFromController();

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    int currentTool() const;
    bool isVisibleForTool() const;
    qreal valueFromY(qreal y) const;
    void applyValue(int value);

    ToolController* m_controller = nullptr;
    QWidget* m_host = nullptr;
    bool m_dragging = false;
    qreal m_hover = 0.0;
};

#endif
