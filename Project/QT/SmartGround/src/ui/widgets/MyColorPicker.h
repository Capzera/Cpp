#ifndef MYCOLORPICKER_H
#define MYCOLORPICKER_H
#include <QWidget>
#include <QColor>
#include <functional>

class MySlider;
class MyTextInput;

class ColorField : public QWidget {
    Q_OBJECT
public:
    explicit ColorField(QWidget* parent = nullptr);

    void setColor(const QColor& c);
    QColor color() const { return m_color; }
    void setOnChanged(std::function<void()> cb) { m_onChanged = std::move(cb); }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    QRectF svRect() const;
    QRectF hueRect() const;
    void applyAt(const QPointF& pos);
    void updateFromPoint(const QPointF& pos, bool onHue);

    QColor m_color = QColor(31, 39, 51);
    std::function<void()> m_onChanged;
    bool m_dragging = false;
};

class ColorPreview : public QWidget {
    Q_OBJECT
public:
    explicit ColorPreview(QWidget* parent = nullptr);
    void setColor(const QColor& c);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QColor m_color = QColor(31, 39, 51);
};

class MyColorPicker : public QWidget {
    Q_OBJECT
public:
    explicit MyColorPicker(QWidget* parent = nullptr);

    QColor color() const { return m_color; }
    void setColor(const QColor& c);

signals:
    void colorPicked(const QColor& c);

private:
    void emitColor();
    void syncInputs();
    void applyHex(const QString& text);
    void applyRgb(int r, int g, int b);
    void onFieldChanged();

    QColor m_color = QColor(31, 39, 51);
    ColorField* m_field = nullptr;
    ColorPreview* m_preview = nullptr;
    MySlider* m_hue = nullptr;
    MySlider* m_sat = nullptr;
    MySlider* m_val = nullptr;
    MyTextInput* m_hex = nullptr;
    MyTextInput* m_red = nullptr;
    MyTextInput* m_green = nullptr;
    MyTextInput* m_blue = nullptr;
    bool m_syncing = false;
};

#endif
