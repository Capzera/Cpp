#ifndef MYCOLORPICKER_H
#define MYCOLORPICKER_H
#include <QWidget>
#include <QColor>

class QSlider;
class MyTextInput;
class MySlider;

class MyColorPicker : public QWidget {
    Q_OBJECT
public:
    explicit MyColorPicker(QWidget* parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor& c);

signals:
    void colorPicked(const QColor& c);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    void emitColor();
    int hueAt(int x) const;
    void syncInputs();
    void applyHex(const QString& text);
    void applyRgb(int r, int g, int b);

    QColor m_color = QColor(31, 39, 51);
    MySlider* m_hue = nullptr;
    MySlider* m_sat = nullptr;
    MySlider* m_val = nullptr;
    MyTextInput* m_hex = nullptr;
    MyTextInput* m_red = nullptr;
    MyTextInput* m_green = nullptr;
    MyTextInput* m_blue = nullptr;
    bool m_dragging = false;
};

#endif
