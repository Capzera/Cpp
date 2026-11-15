#include "MyColorPicker.h"
#include "MyTextInput.h"
#include "MySlider.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <algorithm>
#include <functional>

namespace {

class ColorChip : public QWidget {
public:
    explicit ColorChip(const QColor& color, QWidget* parent = nullptr) : QWidget(parent), m_color(color) {
        setFixedSize(20, 20);
        setCursor(Qt::PointingHandCursor);
    }

    void setPickedCallback(std::function<void()> callback) { m_callback = std::move(callback); }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 5, 5);
        painter.setPen(QPen(QColor(0, 0, 0, 40), 1));
        painter.setBrush(m_color);
        painter.drawPath(path);
    }

    void mousePressEvent(QMouseEvent*) override {
        if (m_callback) m_callback();
    }

private:
    QColor m_color;
    std::function<void()> m_callback;
};

const QColor& gradientAt(int index) {
    static const QColor palette[] = {
        QColor(229, 72, 77), QColor(240, 140, 40), QColor(240, 196, 40),
        QColor(96, 196, 84), QColor(32, 178, 170), QColor(59, 125, 221),
        QColor(123, 97, 255), QColor(214, 90, 168), QColor(90, 100, 120)
    };
    static const int count = 9;
    return palette[index % count];
}

}  // namespace

MyColorPicker::MyColorPicker(QWidget* parent) : QWidget(parent) {
    setFixedWidth(244);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(8);

    QWidget* preview = new QWidget(this);
    preview->setObjectName("colorPreview");
    preview->setFixedHeight(34);
    preview->setStyleSheet("QWidget#colorPreview { border:1px solid #dbe1ea; border-radius:8px; background:#1f2733; }");
    root->addWidget(preview);

    QLabel* hint = new QLabel("拖动色相条无极取色", this);
    hint->setFont(UiTheme::uiFont(8));
    hint->setStyleSheet("color:#8b95a6; background:transparent;");
    root->addWidget(hint);

    auto addSlider = [this, root](const QString& label, int value) {
        QWidget* row = new QWidget(this);
        QHBoxLayout* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(7);
        QLabel* text = new QLabel(label, row);
        text->setFixedWidth(26);
        text->setFont(UiTheme::uiFont(8));
        text->setStyleSheet("color:#7a8494; background:transparent;");
        rowLayout->addWidget(text);
        MySlider* slider = new MySlider(row);
        slider->setRange(0, 255);
        slider->setValue(value);
        slider->setFixedHeight(18);
        rowLayout->addWidget(slider, 1);
        root->addWidget(row);
        return slider;
    };

    m_hue = addSlider("色相", m_color.hue());
    m_sat = addSlider("饱和", m_color.saturation());
    m_val = addSlider("明度", m_color.value());

    QWidget* hexRow = new QWidget(this);
    QHBoxLayout* hexLayout = new QHBoxLayout(hexRow);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    hexLayout->setSpacing(7);
    QLabel* hexLabel = new QLabel("色值", hexRow);
    hexLabel->setFixedWidth(26);
    hexLabel->setFont(UiTheme::uiFont(8));
    hexLabel->setStyleSheet("color:#7a8494; background:transparent;");
    hexLayout->addWidget(hexLabel);
    m_hex = new MyTextInput(hexRow);
    m_hex->setPlaceholderText("#1F2733");
    m_hex->setMaxLength(7);
    hexLayout->addWidget(m_hex, 1);
    root->addWidget(hexRow);

    QWidget* rgbRow = new QWidget(this);
    QHBoxLayout* rgbLayout = new QHBoxLayout(rgbRow);
    rgbLayout->setContentsMargins(0, 0, 0, 0);
    rgbLayout->setSpacing(5);
    auto addNumber = [this, rgbLayout, rgbRow](const QString& label, int value) {
        QLabel* text = new QLabel(label, rgbRow);
        text->setFont(UiTheme::uiFont(8));
        text->setStyleSheet("color:#7a8494; background:transparent;");
        rgbLayout->addWidget(text);
        MyTextInput* input = new MyTextInput(rgbRow);
        input->setText(QString::number(value));
        input->setMaxLength(3);
        input->setFixedWidth(48);
        rgbLayout->addWidget(input);
        return input;
    };
    m_red = addNumber("R", m_color.red());
    m_green = addNumber("G", m_color.green());
    m_blue = addNumber("B", m_color.blue());
    root->addWidget(rgbRow);

    QLabel* presetLabel = new QLabel("渐变预设", this);
    presetLabel->setFont(UiTheme::uiFont(8));
    presetLabel->setStyleSheet("color:#7a8494; background:transparent;");
    root->addWidget(presetLabel);

    QWidget* presetRow = new QWidget(this);
    QHBoxLayout* presetLayout = new QHBoxLayout(presetRow);
    presetLayout->setContentsMargins(0, 0, 0, 0);
    presetLayout->setSpacing(4);
    for (int i = 0; i < 9; ++i) {
        const QColor c = gradientAt(i);
        ColorChip* chip = new ColorChip(c, presetRow);
        chip->setPickedCallback([this, c]() {
            setColor(c);
            emitColor();
        });
        presetLayout->addWidget(chip);
    }
    root->addWidget(presetRow);

    connect(m_hue, &MySlider::valueChanged, this, [this](int v) {
        QColor c = m_color;
        c.setHsv(qBound(0, v, 359), m_color.saturation(), m_color.value());
        m_color = c;
        emitColor();
    });
    connect(m_sat, &MySlider::valueChanged, this, [this](int v) {
        QColor c = m_color;
        c.setHsv(m_color.hue(), qBound(0, v, 255), m_color.value());
        m_color = c;
        emitColor();
    });
    connect(m_val, &MySlider::valueChanged, this, [this](int v) {
        QColor c = m_color;
        c.setHsv(m_color.hue(), m_color.saturation(), qBound(0, v, 255));
        m_color = c;
        emitColor();
    });
    connect(m_hex, &MyTextInput::editingFinished, this, [this]() { applyHex(m_hex->text()); });
    connect(m_red, &MyTextInput::editingFinished, this, [this]() {
        applyRgb(m_red->text().toInt(), m_green->text().toInt(), m_blue->text().toInt());
    });
    connect(m_green, &MyTextInput::editingFinished, this, [this]() {
        applyRgb(m_red->text().toInt(), m_green->text().toInt(), m_blue->text().toInt());
    });
    connect(m_blue, &MyTextInput::editingFinished, this, [this]() {
        applyRgb(m_red->text().toInt(), m_green->text().toInt(), m_blue->text().toInt());
    });

    syncInputs();
}

void MyColorPicker::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    syncInputs();
    update();
}

void MyColorPicker::syncInputs() {
    const bool hue = m_hue && m_hue->value() != m_color.hue();
    const bool sat = m_sat && m_sat->value() != m_color.saturation();
    const bool val = m_val && m_val->value() != m_color.value();
    if (hue) m_hue->setValue(m_color.hue());
    if (sat) m_sat->setValue(m_color.saturation());
    if (val) m_val->setValue(m_color.value());
    if (m_hex) m_hex->setText(m_color.name().toUpper());
    if (m_red) m_red->setText(QString::number(m_color.red()));
    if (m_green) m_green->setText(QString::number(m_color.green()));
    if (m_blue) m_blue->setText(QString::number(m_color.blue()));
}

void MyColorPicker::applyHex(const QString& text) {
    QString t = text.trimmed();
    if (!t.startsWith('#')) t.prepend('#');
    QColor c(t);
    if (!c.isValid()) {
        syncInputs();
        return;
    }
    m_color = c;
    syncInputs();
    emitColor();
}

void MyColorPicker::applyRgb(int r, int g, int b) {
    QColor c(qBound(0, r, 255), qBound(0, g, 255), qBound(0, b, 255));
    m_color = c;
    syncInputs();
    emitColor();
}

void MyColorPicker::emitColor() {
    update();
    emit colorPicked(m_color);
}

int MyColorPicker::hueAt(int x) const {
    const int w = qMax(1, width());
    return qBound(0, static_cast<int>(x * 360.0 / w), 359);
}

void MyColorPicker::mousePressEvent(QMouseEvent* event) {
    const int y = event->position().y();
    if (y < 34) {
        m_dragging = true;
        return;
    }
    QWidget::mousePressEvent(event);
}

void MyColorPicker::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging) {
        const int h = hueAt(static_cast<int>(event->position().x()));
        QColor c = m_color;
        c.setHsv(h, m_color.saturation(), m_color.value());
        m_color = c;
        syncInputs();
        emitColor();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void MyColorPicker::mouseReleaseEvent(QMouseEvent* event) {
    if (m_dragging) {
        m_dragging = false;
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void MyColorPicker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF bar(0, 0, width(), 34);
    QLinearGradient gradient(0, 0, width(), 0);
    for (int i = 0; i <= 12; ++i) {
        const QColor c = QColor::fromHsv(i * 30, 220, 235);
        gradient.setColorAt(i / 12.0, c);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawRoundedRect(bar, 8, 8);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(0, 0, 0, 40), 1));
    painter.drawRoundedRect(bar.adjusted(0.5, 0.5, -0.5, -0.5), 8, 8);

    const qreal markerX = bar.left() + bar.width() * (m_color.hue() / 359.0);
    painter.setPen(QPen(QColor(255, 255, 255, 230), 2));
    painter.setBrush(QColor(31, 39, 51));
    painter.drawEllipse(QPointF(markerX, bar.center().y()), 6.5, 6.5);
    painter.setPen(QPen(QColor(255, 255, 255, 235), 1.2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPointF(markerX, bar.center().y()), 8.5, 8.5);
}
