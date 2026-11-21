#include "CartesianPanel.h"
#include "render/CartesianElements.h"
#include "core/CartesianExpression.h"
#include "ui/UiTheme.h"
#include "ui/UiFonts.h"
#include "MyButton.h"
#include "MyTextInput.h"
#include "MyToggleSwitch.h"
#include "MySlider.h"
#include "MySegmentedControl.h"
#include "MyColorPicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QMenu>
#include <QWidgetAction>
#include <QDialogButtonBox>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <functional>
#include <cmath>

namespace {

QLabel* makeSectionTitle(const QString& text, QWidget* parent) {
    QLabel* label = new QLabel(text, parent);
    label->setObjectName("sectionLabel");
    label->setFont(UiTheme::uiFont(10, true));
    return label;
}

QString formatCoord(double value) {
    QString text = QString::number(value, 'f', 4);
    if (text.contains('.')) {
        while (text.endsWith('0')) text.chop(1);
        if (text.endsWith('.')) text.chop(1);
    }
    if (text == "-0") text = "0";
    return text;
}

class MenuButton : public QWidget {
public:
    explicit MenuButton(QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(22, 22);
        setCursor(Qt::PointingHandCursor);
    }

    void setOnClick(std::function<void()> callback) { m_onClick = std::move(callback); }

    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(UiTheme::textMuted());
        const QPointF c(width() / 2.0, height() / 2.0);
        const double d = 2.2;
        const double gap = 5.0;
        for (int i = -1; i <= 1; ++i) {
            painter.drawEllipse(QPointF(c.x() + i * gap, c.y()), d, d);
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (m_onClick) m_onClick();
            event->accept();
            return;
        }
        QWidget::mousePressEvent(event);
    }

private:
    std::function<void()> m_onClick;
};

}  // namespace

CartesianPanel::CartesianPanel(QWidget* parent) : QWidget(parent) {
    setObjectName("cartesianPanel");
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(8);

    root->addWidget(makeSectionTitle("对象列表", this));

    m_listHost = new QWidget(this);
    m_listHost->setObjectName("cartesianListHost");
    QVBoxLayout* listLayout = new QVBoxLayout(m_listHost);
    listLayout->setContentsMargins(0, 0, 0, 0);
    listLayout->setSpacing(4);
    root->addWidget(m_listHost);

    root->addWidget(makeSectionTitle("函数绘制", this));

    QHBoxLayout* functionRow = new QHBoxLayout();
    functionRow->setSpacing(6);
    QLabel* prefix = new QLabel("f(x) =", this);
    prefix->setObjectName("fieldLabel");
    prefix->setFont(UiFonts::mono(10, true));
    functionRow->addWidget(prefix);

    m_functionInput = new MyTextInput(this);
    m_functionInput->setObjectName("cartesianFunctionInput");
    m_functionInput->setPlaceholderText("3x^2 - 8x + 7");
    functionRow->addWidget(m_functionInput, 1);

    m_functionAddButton = new MyButton("+", this);
    m_functionAddButton->setObjectName("cartesianFunctionAdd");
    m_functionAddButton->setVariant(MyButton::Soft);
    m_functionAddButton->setCompact(true);
    m_functionAddButton->setFixedWidth(34);
    functionRow->addWidget(m_functionAddButton);
    root->addLayout(functionRow);

    QLabel* hint = new QLabel("支持一次 / 二次 / 三次函数，双击曲线可改表达式", this);
    hint->setObjectName("hintLabel");
    hint->setWordWrap(true);
    root->addWidget(hint);

    connect(m_functionAddButton, &MyButton::clicked, this, [this]() {
        if (!m_engine) return;
        const QString expression = m_functionInput->text().trimmed();
        if (expression.isEmpty()) {
            emit statusMessage("请先输入函数表达式，例如 3x^2 - 8x + 7");
            return;
        }
        const CartesianExpression::Result result = CartesianExpression::parse(expression);
        if (!result.ok) {
            emit statusMessage("函数表达式无法解析：" + result.error);
            return;
        }
        emit functionAddRequested(expression);
        m_functionInput->setText("");
    });
}
CartesianPanel::~CartesianPanel() {}

void CartesianPanel::setEngine(CartesianEngine* engine) {
    if (m_engine == engine) return;
    m_engine = engine;
    refresh();
}

void CartesianPanel::refresh() {
    if (m_refreshing) return;
    m_refreshing = true;
    rebuildList();
    m_refreshing = false;
}

void CartesianPanel::rebuildList() {
    QVBoxLayout* hostLayout = qobject_cast<QVBoxLayout*>(m_listHost->layout());
    if (!hostLayout) return;
    while (QLayoutItem* item = hostLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) widget->deleteLater();
        delete item;
    }
    m_rows.clear();
    if (!m_engine) return;

    const QVector<CartesianPointItem*> points = m_engine->points();
    if (points.isEmpty()) {
        QLabel* empty = new QLabel("尚未描点", m_listHost);
        empty->setObjectName("hintLabel");
        hostLayout->addWidget(empty);
    } else {
        for (int i = 0; i < points.size(); ++i) {
            hostLayout->addWidget(buildPointRow(points.at(i), i));
        }
    }

    for (int i = 0; i < m_engine->curveCount(); ++i) {
        CartesianCurveItem* curve = m_engine->curveAt(i);
        if (!curve) continue;
        hostLayout->addWidget(buildFunctionRow(curve, i));
    }
    hostLayout->addStretch(1);
}
bool CartesianPanel::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if (widget && widget->parent() == m_listHost) {
                const QVariant value = widget->property("cartesianPointIndex");
                if (value.isValid()) {
                    emit pointSelected(value.toInt());
                    return true;
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

QWidget* CartesianPanel::buildPointRow(CartesianPointItem* point, int index) {
    QWidget* row = new QWidget(m_listHost);
    row->setObjectName("cartesianObjectRow");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 4, 4, 4);
    layout->setSpacing(8);

    QLabel* swatch = new QLabel(row);
    swatch->setFixedSize(12, 12);
    swatch->setStyleSheet(QString("background:%1; border-radius:6px;").arg(point->color().name()));
    layout->addWidget(swatch);

    QLabel* nameLabel = new QLabel(point->name(), row);
    nameLabel->setObjectName("fieldLabel");
    nameLabel->setFont(UiFonts::forText(point->name(), 10, true));
    nameLabel->setFixedWidth(34);
    layout->addWidget(nameLabel);

    QLabel* coordLabel = new QLabel(QString("(%1, %2)").arg(formatCoord(point->x()), formatCoord(point->y())), row);
    coordLabel->setObjectName("hintLabel");
    coordLabel->setFont(UiFonts::mono(9, false));
    layout->addWidget(coordLabel, 1);

    MenuButton* menu = new MenuButton(row);
    layout->addWidget(menu);

    menu->setOnClick([this, point, index, menu]() {
        showPointMenu(point, index, menu->mapToGlobal(QPoint(0, menu->height())));
    });

    row->installEventFilter(this);
    row->setProperty("cartesianPointIndex", index);
    row->setCursor(Qt::PointingHandCursor);

    m_rows.push_back(row);
    return row;
}

QWidget* CartesianPanel::buildFunctionRow(CartesianCurveItem* curve, int index) {
    QWidget* row = new QWidget(m_listHost);
    row->setObjectName("cartesianObjectRow");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 4, 4, 4);
    layout->setSpacing(8);

    QLabel* swatch = new QLabel(row);
    swatch->setFixedSize(12, 12);
    swatch->setStyleSheet(QString("background:%1; border-radius:6px;").arg(curve->color().name()));
    layout->addWidget(swatch);

    QLabel* nameLabel = new QLabel(curve->name(), row);
    nameLabel->setObjectName("fieldLabel");
    nameLabel->setFont(UiFonts::forText(curve->name(), 10, true));
    nameLabel->setFixedWidth(34);
    layout->addWidget(nameLabel);

    QLabel* exprLabel = new QLabel(curve->expression(), row);
    exprLabel->setObjectName("hintLabel");
    exprLabel->setFont(UiFonts::mono(9, false));
    layout->addWidget(exprLabel, 1);

    MenuButton* menu = new MenuButton(row);
    layout->addWidget(menu);

    menu->setOnClick([this, curve, index, menu]() {
        showFunctionMenu(curve, index, menu->mapToGlobal(QPoint(0, menu->height())));
    });

    m_rows.push_back(row);
    return row;}

void CartesianPanel::showPointMenu(CartesianPointItem* point, int index, const QPoint& globalPos) {
    QMenu menu(this);
    QAction* editAction = menu.addAction("编辑名称与坐标");
    QAction* styleAction = menu.addAction("设置");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction("删除");
    Q_UNUSED(point);

    QAction* chosen = menu.exec(globalPos);
    if (!chosen) return;
    if (chosen == editAction) {
        emit pointSelected(index);
    } else if (chosen == styleAction) {
        showPointStyleEditor(point, index, globalPos);
    } else if (chosen == deleteAction) {
        if (m_engine) m_engine->removePoint(index);
        emit statusMessage("已删除该点");
    }
}

void CartesianPanel::showPointStyleEditor(CartesianPointItem* point, int index, const QPoint& globalPos) {
    QMenu menu(this);
    menu.setObjectName("cartesianStyleMenu");

    QWidget* card = new QWidget(&menu);
    card->setObjectName("cartesianStyleCard");
    card->setAttribute(Qt::WA_TranslucentBackground, true);
    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(9);

    QLabel* title = new QLabel("点的样式", card);
    title->setFont(UiTheme::uiFont(10, true));
    layout->addWidget(title);

    MyColorPicker* picker = new MyColorPicker(card);
    picker->setObjectName("cartesianPointColorPicker");
    picker->setColor(point->color());
    layout->addWidget(picker);

    QHBoxLayout* sizeRow = new QHBoxLayout();
    sizeRow->setSpacing(8);
    QLabel* sizeLabel = new QLabel("大小", card);
    sizeLabel->setObjectName("fieldLabel");
    sizeLabel->setFixedWidth(40);
    sizeRow->addWidget(sizeLabel);
    MySlider* sizeSlider = new MySlider(card);
    sizeSlider->setObjectName("cartesianPointSize");
    sizeSlider->setRange(3, 20);
    sizeSlider->setValue(static_cast<int>(point->size()));
    sizeSlider->setFixedWidth(132);
    sizeRow->addWidget(sizeSlider, 1);
    layout->addLayout(sizeRow);

    QHBoxLayout* shapeRow = new QHBoxLayout();
    shapeRow->setSpacing(8);
    QLabel* shapeLabel = new QLabel("形状", card);
    shapeLabel->setObjectName("fieldLabel");
    shapeLabel->setFixedWidth(40);
    shapeRow->addWidget(shapeLabel);
    MySegmentedControl* shapePicker = new MySegmentedControl({"圆", "方", "菱", "叉"}, card);
    shapePicker->setObjectName("cartesianPointShape");
    shapePicker->setCurrentIndex(point->pointShape());
    shapeRow->addWidget(shapePicker, 1);
    layout->addLayout(shapeRow);

    QHBoxLayout* hideRow = new QHBoxLayout();
    hideRow->setSpacing(8);
    QLabel* hideLabel = new QLabel("隐藏名字", card);
    hideLabel->setObjectName("fieldLabel");
    hideRow->addWidget(hideLabel);
    hideRow->addStretch(1);
    MyToggleSwitch* hideSwitch = new MyToggleSwitch(card);
    hideSwitch->setObjectName("cartesianPointHideName");
    hideSwitch->setOnText("隐藏");
    hideSwitch->setOffText("显示");
    hideSwitch->setChecked(point->nameHidden());
    hideRow->addWidget(hideSwitch);
    layout->addLayout(hideRow);

    QWidgetAction* action = new QWidgetAction(&menu);
    action->setDefaultWidget(card);
    action->setEnabled(false);
    menu.addAction(action);

    connect(picker, &MyColorPicker::colorPicked, this, [this, index](const QColor& color) {
        if (m_engine) {
            CartesianPointItem* target = m_engine->pointAt(index);
            if (target) target->setColor(color);
        }
        emit pointStyleEdited(index, color, -1, -1, false);
        emit statusMessage("已更新点的颜色");
    });
    connect(sizeSlider, &MySlider::valueChanged, this, [this, index](int value) {
        if (m_engine) {
            CartesianPointItem* target = m_engine->pointAt(index);
            if (target) target->setSize(value);
        }
        emit pointStyleEdited(index, QColor(), value, -1, false);
    });
    connect(shapePicker, &MySegmentedControl::currentIndexChanged, this, [this, index](int shape) {
        if (m_engine) {
            CartesianPointItem* target = m_engine->pointAt(index);
            if (target) target->setShape(shape);
        }
        emit pointStyleEdited(index, QColor(), -1, shape, false);
        emit statusMessage("已更新点的形状");
    });
    connect(hideSwitch, &MyToggleSwitch::toggled, this, [this, index](bool on) {
        if (m_engine) {
            CartesianPointItem* target = m_engine->pointAt(index);
            if (target) target->setNameHidden(on);
        }
        emit pointStyleEdited(index, QColor(), -1, -1, on);
        emit statusMessage(on ? "已隐藏该点的名字" : "已显示该点的名字");
    });

    menu.exec(globalPos);
}

void CartesianPanel::showFunctionMenu(CartesianCurveItem* curve, int index, const QPoint& globalPos) {
    QMenu menu(this);
    QAction* styleAction = menu.addAction("设置");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction("删除");
    Q_UNUSED(curve);

    QAction* chosen = menu.exec(globalPos);
    if (!chosen) return;
    if (chosen == styleAction) {
        showFunctionStyleEditor(curve, index, globalPos);
    } else if (chosen == deleteAction) {
        emit functionRemoveRequested(curve->name());
    }
}

void CartesianPanel::showFunctionStyleEditor(CartesianCurveItem* curve, int index, const QPoint& globalPos) {
    QMenu menu(this);
    menu.setObjectName("cartesianStyleMenu");

    QWidget* card = new QWidget(&menu);
    card->setObjectName("cartesianStyleCard");
    card->setAttribute(Qt::WA_TranslucentBackground, true);
    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(9);

    QLabel* title = new QLabel(QString("%1 的样式").arg(curve->name()), card);
    title->setFont(UiTheme::uiFont(10, true));
    layout->addWidget(title);

    MyColorPicker* picker = new MyColorPicker(card);
    picker->setObjectName("cartesianFunctionColorPicker");
    picker->setColor(curve->color());
    layout->addWidget(picker);

    QWidgetAction* action = new QWidgetAction(&menu);
    action->setDefaultWidget(card);
    action->setEnabled(false);
    menu.addAction(action);

    connect(picker, &MyColorPicker::colorPicked, this, [this, curve](const QColor& color) {
        curve->setColor(color);
        emit functionStyleEdited(curve->name(), color);
        emit statusMessage("已更新函数曲线颜色");
    });

    menu.exec(globalPos);
    Q_UNUSED(index);
}

// ---------------------------------------------------------------- editor

CartesianPointEditor::CartesianPointEditor(QWidget* parent) : QDialog(parent) {
    setWindowTitle("编辑点");
    setModal(true);
    setObjectName("cartesianPointEditor");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 18, 20, 16);
    layout->setSpacing(12);

    QLabel* title = new QLabel("设置点的名字与坐标", this);
    title->setFont(UiTheme::uiFont(11, true));
    layout->addWidget(title);

    auto addRow = [&](const QString& label, QWidget* control) {
        QHBoxLayout* row = new QHBoxLayout();
        row->setSpacing(10);
        QLabel* text = new QLabel(label, this);
        text->setObjectName("fieldLabel");
        text->setFixedWidth(72);
        row->addWidget(text);
        row->addWidget(control, 1);
        layout->addLayout(row);
    };

    m_nameInput = new MyTextInput(this);
    m_nameInput->setObjectName("cartesianEditName");
    m_nameInput->setMaxLength(8);
    addRow("名字", m_nameInput);

    m_xInput = new MyTextInput(this);
    m_xInput->setObjectName("cartesianEditX");
    addRow("x 坐标", m_xInput);

    m_yInput = new MyTextInput(this);
    m_yInput->setObjectName("cartesianEditY");
    addRow("y 坐标", m_yInput);

    QHBoxLayout* hideRow = new QHBoxLayout();
    hideRow->setSpacing(10);
    QLabel* hideLabel = new QLabel("隐藏名字", this);
    hideLabel->setObjectName("fieldLabel");
    hideLabel->setFixedWidth(72);
    hideRow->addWidget(hideLabel);
    hideRow->addStretch(1);
    m_hideSwitch = new MyToggleSwitch(this);
    m_hideSwitch->setObjectName("cartesianEditHideName");
    m_hideSwitch->setOnText("隐藏");
    m_hideSwitch->setOffText("显示");
    hideRow->addWidget(m_hideSwitch);
    layout->addLayout(hideRow);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->setSpacing(8);
    buttons->addStretch(1);
    MyButton* cancel = new MyButton("取消", this);
    cancel->setVariant(MyButton::Ghost);
    buttons->addWidget(cancel);
    MyButton* ok = new MyButton("确定", this);
    ok->setVariant(MyButton::Solid);
    buttons->addWidget(ok);
    layout->addLayout(buttons);

    connect(cancel, &MyButton::clicked, this, &QDialog::reject);
    connect(ok, &MyButton::clicked, this, &QDialog::accept);
}

void CartesianPointEditor::setPointData(const QString& name, double x, double y, bool nameHidden) {
    m_nameInput->setText(name);
    m_xInput->setText(formatCoord(x));
    m_yInput->setText(formatCoord(y));
    m_hideSwitch->setChecked(nameHidden);
}

QString CartesianPointEditor::pointName() const { return m_nameInput->text().trimmed(); }

double CartesianPointEditor::pointX() const { return m_xInput->text().trimmed().toDouble(); }

double CartesianPointEditor::pointY() const { return m_yInput->text().trimmed().toDouble(); }

bool CartesianPointEditor::nameHidden() const { return m_hideSwitch->isChecked(); }

void CartesianPointEditor::paintEvent(QPaintEvent* event) {
    QDialog::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(UiTheme::border(), 1));
    painter.setBrush(UiTheme::surface());
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 12, 12);
}
