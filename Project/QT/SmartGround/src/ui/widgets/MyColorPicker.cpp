#include "MyColorPicker.h"
#include "MyTextInput.h"
#include "MySlider.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <algorithm>
#include <functional>

namespace {

constexpr int kPresetCount = 9;

int normalizedHue(const QColor& c) {
    const int h = c.hue();
    return h < 0 ? 0 : qBound(0, h, 359);
}

const QColor& gradientAt(int index) {
    static const QColor palette[] = {
        QColor(229, 72, 77), QColor(240, 140, 40), QColor(240, 196, 40),
        QColor(96, 196, 84), QColor(32, 178, 170), QColor(59, 125, 221),
        QColor(123, 97, 255), QColor(214, 90, 168), QColor(90, 100, 120)
    };
    static const int count = 9;
    return palette[index % count];
}

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

}  // namespace

ColorField::ColorField(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(112);
    setCursor(Qt::CrossCursor);
    setAttribute(Qt::WA_Hover, true);
}

QSize ColorField::sizeHint() const {
    return QSize(220, 112);
}

QSize ColorField::minimumSizeHint() const {
    return QSize(180, 112);
}

QRectF ColorField::svRect() const {
    const qreal hueH = 14.0;
    return QRectF(0.0, 0.0, width(), height() - hueH - 4.0);
}

QRectF ColorField::hueRect() const {
    const qreal hueH = 14.0;
    return QRectF(0.0, height() - hueH, width(), hueH);
}

void ColorField::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    update();
}

void ColorField::applyAt(const QPointF& pos) {
    updateFromPoint(pos, false);
}

void ColorField::updateFromPoint(const QPointF& pos, bool onHue) {
    if (onHue) {
        const QRectF hr = hueRect();
        qreal t = (pos.x() - hr.left()) / qMax<qreal>(1.0, hr.width());
        t = qBound(0.0, t, 1.0);
        const int h = qBound(0, static_cast<int>(t * 360.0), 359);
        QColor c = QColor::fromHsv(h, m_color.saturation(), m_color.value());        m_color = c;
        update();
        if (m_onChanged) m_onChanged();
        return;
    }

    const QRectF sr = svRect();
    qreal sx = qBound(0.0, pos.x() - sr.left(), sr.width());
    qreal sy = qBound(0.0, pos.y() - sr.top(), sr.height());
    const qreal sat = sr.width() > 0.0 ? sx / sr.width() : 0.0;
    const qreal val = sr.height() > 0.0 ? 1.0 - sy / sr.height() : 0.0;
    QColor c = QColor::fromHsv(normalizedHue(m_color), qBound(0, static_cast<int>(sat * 255.0), 255),
                               qBound(0, static_cast<int>(val * 255.0), 255));
    m_color = c;
    update();
    if (m_onChanged) m_onChanged();
}

void ColorField::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    const QPointF pos = event->position();
    m_dragging = true;
    updateFromPoint(pos, hueRect().contains(pos));
    event->accept();
}

void ColorField::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging) {
        const QPointF pos = event->position();
        updateFromPoint(pos, hueRect().contains(pos));
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void ColorField::mouseReleaseEvent(QMouseEvent* event) {
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ColorField::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF sr = svRect();
    const int h = normalizedHue(m_color);
    const QColor pure = QColor::fromHsv(h, 255, 255);

    QLinearGradient whiteToPure(0, 0, sr.width(), 0);
    whiteToPure.setColorAt(0.0, QColor(255, 255, 255));
    whiteToPure.setColorAt(1.0, pure);
    painter.setPen(Qt::NoPen);
    painter.setBrush(whiteToPure);
    painter.drawRect(sr);

    QLinearGradient blackToClear(0, 0, 0, sr.height());
    blackToClear.setColorAt(0.0, QColor(0, 0, 0, 0));
    blackToClear.setColorAt(1.0, QColor(0, 0, 0, 255));
    painter.setBrush(blackToClear);
    painter.drawRect(sr);

    const qreal sat = m_color.saturationF();
    const qreal val = m_color.valueF();
    const QPointF marker(sr.left() + sat * sr.width(), sr.top() + (1.0 - val) * sr.height());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255, 255, 255, 235), 2.0));
    painter.drawEllipse(marker, 6.0, 6.0);
    painter.setPen(QPen(QColor(0, 0, 0, 120), 1.0));
    painter.drawEllipse(marker, 7.2, 7.2);

    const QRectF hr = hueRect();
    QLinearGradient hueGradient(hr.left(), 0, hr.right(), 0);
    for (int i = 0; i < 12; ++i) {
        hueGradient.setColorAt(i / 12.0, QColor::fromHsv(i * 30, 235, 240));
    }
    hueGradient.setColorAt(1.0, QColor::fromHsv(359, 235, 240));
    painter.setPen(Qt::NoPen);
    painter.setBrush(hueGradient);
    painter.drawRoundedRect(hr, 5, 5);

    const qreal hueT = normalizedHue(m_color) / 359.0;
    const qreal hx = hr.left() + hr.width() * hueT;
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255, 255, 255, 240), 2.0));
    painter.drawEllipse(QPointF(hx, hr.center().y()), 6.0, 6.0);
    painter.setPen(QPen(QColor(0, 0, 0, 110), 1.0));
    painter.drawEllipse(QPointF(hx, hr.center().y()), 7.2, 7.2);
}

ColorPreview::ColorPreview(QWidget* parent) : QWidget(parent) {
    setFixedHeight(34);
}

QSize ColorPreview::sizeHint() const {
    return QSize(220, 34);
}

void ColorPreview::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    update();
}

void ColorPreview::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, 8, 8);
    painter.setPen(QPen(QColor(0, 0, 0, 45), 1));
    painter.setBrush(m_color);
    painter.drawPath(path);

    QPainterPath inner;
    inner.addRoundedRect(box.adjusted(1, 1, -1, -1), 7, 7);
    painter.setPen(QPen(QColor(255, 255, 255, 170), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(inner);
}

MyColorPicker::MyColorPicker(QWidget* parent) : QWidget(parent) {
    setFixedWidth(244);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(8);

    m_preview = new ColorPreview(this);
    m_preview->setObjectName("colorPreview");
    m_preview->setColor(m_color);
    root->addWidget(m_preview);

    m_field = new ColorField(this);
    m_field->setObjectName("colorField");
    m_field->setColor(m_color);
    m_field->setOnChanged([this]() { onFieldChanged(); });
    root->addWidget(m_field);

    QLabel* hint = new QLabel("拖动色相条无极取色", this);
    hint->setFont(UiTheme::uiFont(8));
    hint->setStyleSheet("color:#8b95a6; background:transparent;");
    root->addWidget(hint);

    auto addSlider = [this, root](const QString& label, int min, int max, int value) {
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
        slider->setRange(min, max);
        slider->setValue(value);
        slider->setFixedHeight(18);
        rowLayout->addWidget(slider, 1);
        root->addWidget(row);
        return slider;
    };

    m_hue = addSlider("色相", 0, 359, m_color.hue());
    m_sat = addSlider("饱和", 0, 255, m_color.saturation());
    m_val = addSlider("明度", 0, 255, m_color.value());

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

    QLabel* presetLabel = new QLabel("预设", this);
    presetLabel->setFont(UiTheme::uiFont(8));
    presetLabel->setStyleSheet("color:#7a8494; background:transparent;");
    root->addWidget(presetLabel);

    QWidget* presetRow = new QWidget(this);
    QHBoxLayout* presetLayout = new QHBoxLayout(presetRow);
    presetLayout->setContentsMargins(0, 0, 0, 0);
    presetLayout->setSpacing(4);
    for (int i = 0; i < kPresetCount; ++i) {
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
        if (m_syncing) return;
        QColor c = m_color;
        c.setHsv(qBound(0, v, 359), m_color.saturation(), m_color.value());
        m_color = c;
        syncInputs();
        emitColor();
    });    connect(m_sat, &MySlider::valueChanged, this, [this](int v) {
        if (m_syncing) return;
        QColor c = m_color;
        c.setHsv(m_color.hue(), qBound(0, v, 255), m_color.value());
        m_color = c;
        syncInputs();
        emitColor();
    });
    connect(m_val, &MySlider::valueChanged, this, [this](int v) {
        if (m_syncing) return;
        QColor c = m_color;
        c.setHsv(m_color.hue(), m_color.saturation(), qBound(0, v, 255));
        m_color = c;
        syncInputs();
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

void MyColorPicker::onFieldChanged() {
    if (m_syncing) return;
    m_color = m_field->color();
    syncInputs();
    emitColor();
}

void MyColorPicker::setColor(const QColor& c) {
    if (m_color == c) return;
    m_color = c;
    syncInputs();
    update();
}

void MyColorPicker::syncInputs() {
    m_syncing = true;
    const int hue = normalizedHue(m_color);
    const bool hueDiff = m_hue && m_hue->value() != hue;
    const bool sat = m_sat && m_sat->value() != m_color.saturation();
    const bool val = m_val && m_val->value() != m_color.value();
    if (hueDiff) m_hue->setValue(hue);
    if (sat) m_sat->setValue(m_color.saturation());
    if (val) m_val->setValue(m_color.value());
    if (m_hex) m_hex->setText(m_color.name().toUpper());
    if (m_red) m_red->setText(QString::number(m_color.red()));
    if (m_green) m_green->setText(QString::number(m_color.green()));
    if (m_blue) m_blue->setText(QString::number(m_color.blue()));
    if (m_field) m_field->setColor(m_color);
    if (m_preview) m_preview->setColor(m_color);
    m_syncing = false;
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
