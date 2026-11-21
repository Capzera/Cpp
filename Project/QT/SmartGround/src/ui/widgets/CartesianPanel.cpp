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
#include <QScrollArea>
#include <QFrame>
#include <functional>
#include <cmath>

namespace {

QLabel* makeSectionTitle(const QString& text, QWidget* parent) {
    QLabel* label = new QLabel(text, parent);
    label->setObjectName("sectionLabel");
    label->setFont(UiTheme::uiFont(10, true));
    return label;
}

class GuideCard : public QWidget {
public:
    explicit GuideCard(QWidget* parent = nullptr) : QWidget(parent) {
        setObjectName("cartesianGuideCard");
    }

    void addStep(int index, const QString& title, const QString& detail) {
        QWidget* row = new QWidget(this);
        row->setAttribute(Qt::WA_TranslucentBackground, true);
        QHBoxLayout* layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(7);

        QLabel* badge = new QLabel(QString::number(index), row);
        badge->setFixedSize(16, 16);
        badge->setAlignment(Qt::AlignCenter);
        badge->setFont(UiTheme::uiFont(8, true));
        badge->setStyleSheet(QString("color:#ffffff; background:%1; border-radius:8px;")
                                 .arg(UiTheme::accent().name()));
        layout->addWidget(badge, 0, Qt::AlignTop);

        QLabel* text = new QLabel(QString("<b>%1</b><br><span style='color:#7a8494;'>%2</span>")
                                      .arg(title.toHtmlEscaped(), detail.toHtmlEscaped()),
                                  row);
        text->setObjectName("hintLabel");
        text->setWordWrap(true);
        text->setFont(UiTheme::uiFont(9));
        layout->addWidget(text, 1);
        m_rows.push_back(row);
    }

    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(box, 9, 9);
        painter.setPen(QPen(QColor(196, 216, 243), 1));
        painter.setBrush(QColor(233, 241, 253, 220));
        painter.drawPath(path);
    }

private:
    QVector<QWidget*> m_rows;
};

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

    root->addWidget(buildGuideCard(this));
    root->addWidget(makeSectionTitle("对象列表", this));

    m_listScroll = new QScrollArea(this);
    m_listScroll->setObjectName("cartesianListScroll");
    m_listScroll->setWidgetResizable(true);
    m_listScroll->setFrameShape(QFrame::NoFrame);
    m_listScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_listScroll->viewport()->setObjectName("cartesianListScrollViewport");
    m_listScroll->viewport()->setAutoFillBackground(false);
    m_listScroll->setStyleSheet(
        "QScrollArea#cartesianListScroll { background:transparent; border:none; }"
        "QScrollArea#cartesianListScroll > QWidget > QWidget { background:transparent; }"
        "QScrollBar:vertical { background:transparent; width:8px; margin:0px; }"
        "QScrollBar::handle:vertical { background:#c6ced9; border-radius:4px; min-height:24px; }"
        "QScrollBar::handle:vertical:hover { background:#a8b2c0; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; }");

    m_listHost = new QWidget(m_listScroll);
    m_listHost->setObjectName("cartesianListHost");
    m_listHost->setAttribute(Qt::WA_TranslucentBackground, true);
    QVBoxLayout* listLayout = new QVBoxLayout(m_listHost);
    listLayout->setContentsMargins(0, 0, 6, 0);
    listLayout->setSpacing(4);
    m_listScroll->setWidget(m_listHost);
    root->addWidget(m_listScroll, 1);
}

CartesianPanel::~CartesianPanel() {}

void CartesianPanel::setEngine(CartesianEngine* engine) {
    if (m_engine == engine) return;
    m_engine = engine;
    refresh();
}

QWidget* CartesianPanel::buildGuideCard(QWidget* parent) {
    GuideCard* card = new GuideCard(parent);
    card->addStep(1, "画点", "点画布上方「画点」按钮（或按 P），在画布上单击即可落点。");
    card->addStep(2, "连线", "按 S 切到连线工具，奇数次选点为起点，偶数次选点即连成线段。");
    card->addStep(3, "函数", "点顶部「函数」按钮展开下拉菜单，在其中输入 f(x) = 表达式后回车。");
    card->addStep(4, "管理", "双击列表中的点可直接改坐标；点、线、函数都在「对象列表」中管理。");
    return card;
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
    const QVector<CartesianCurveItem*> curves = m_engine->curves();

    if (points.isEmpty() && curves.isEmpty()) {
        QLabel* empty = new QLabel("尚未描点。用上方「画点」工具在画布上单击即可落下第一个点。",
                                   m_listHost);
        empty->setObjectName("hintLabel");
        empty->setWordWrap(true);
        hostLayout->addWidget(empty);
    }

    for (int i = 0; i < points.size(); ++i) {
        hostLayout->addWidget(buildPointRow(points.at(i), i));
    }

    const QVector<CartesianSegmentItem*> segments = m_engine->segments();
    for (int i = 0; i < segments.size(); ++i) {
        CartesianSegmentItem* segment = segments.at(i);
        if (!segment) continue;
        hostLayout->addWidget(buildSegmentRow(segment, i));
    }

    for (int i = 0; i < curves.size(); ++i) {
        CartesianCurveItem* curve = curves.at(i);
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
                const QVariant pointValue = widget->property("cartesianPointIndex");
                if (pointValue.isValid()) {
                    emit pointSelected(pointValue.toInt());
                    return true;
                }
                const QVariant segmentValue = widget->property("cartesianSegmentIndex");
                if (segmentValue.isValid()) {
                    emit segmentSelected(segmentValue.toInt());
                    return true;
                }
            }
        }
    }
    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if (widget && widget->parent() == m_listHost) {
                const QVariant value = widget->property("cartesianPointIndex");
                if (value.isValid()) {
                    const int index = value.toInt();
                    CartesianPointItem* point = m_engine ? m_engine->pointAt(index) : nullptr;
                    if (point) {
                        editPointCoordinates(point, index);
                        return true;
                    }
                }
                const QVariant segmentValue = widget->property("cartesianSegmentIndex");
                if (segmentValue.isValid()) {
                    emit segmentSelected(segmentValue.toInt());
                    return true;
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CartesianPanel::editPointCoordinates(CartesianPointItem* point, int index) {
    if (!point) return;
    CartesianPointEditor editor(this);
    editor.setPointData(point->name(), point->x(), point->y(), point->nameHidden());
    if (editor.exec() != QDialog::Accepted) return;
    const double x = editor.pointX();
    const double y = editor.pointY();
    emit pointEdited(index, editor.pointName(), x, y, editor.nameHidden());
    emit statusMessage(QString("点 %1 的坐标已更新为 (%2, %3)")
                           .arg(point->name(),
                                CartesianExpression::formatNumber(x, 4),
                                CartesianExpression::formatNumber(y, 4)));
}

QWidget* CartesianPanel::buildPointRow(CartesianPointItem* point, int index) {
    QWidget* row = new QWidget(m_listHost);
    row->setObjectName("cartesianObjectRow");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 5, 4, 5);
    layout->setSpacing(8);

    QLabel* swatch = new QLabel(row);
    swatch->setFixedSize(13, 13);
    swatch->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    swatch->setStyleSheet(QString("background:%1; border-radius:6px;").arg(point->color().name()));
    layout->addWidget(swatch);

    QLabel* nameLabel = new QLabel(point->name(), row);
    nameLabel->setObjectName("cartesianObjectName");
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    nameLabel->setFont(UiFonts::forText(point->name(), 12, true));
    nameLabel->setFixedWidth(40);
    layout->addWidget(nameLabel);

    QLabel* coordLabel = new QLabel(QString("(%1, %2)").arg(formatCoord(point->x()), formatCoord(point->y())), row);
    coordLabel->setObjectName("cartesianObjectValue");
    coordLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    coordLabel->setFont(UiFonts::mono(11, false));
    layout->addWidget(coordLabel, 1);

    MenuButton* menu = new MenuButton(row);
    layout->addWidget(menu);

    menu->setOnClick([this, point, index, menu]() {
        showPointMenu(point, index, menu->mapToGlobal(QPoint(0, menu->height())));
    });

    row->installEventFilter(this);
    row->setProperty("cartesianPointIndex", index);
    row->setToolTip("双击该行可直接修改点的坐标");
    row->setCursor(Qt::PointingHandCursor);

    m_rows.push_back(row);
    return row;
}

QWidget* CartesianPanel::buildSegmentRow(CartesianSegmentItem* segment, int index) {
    QWidget* row = new QWidget(m_listHost);
    row->setObjectName("cartesianObjectRow");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 5, 4, 5);
    layout->setSpacing(8);

    QLabel* swatch = new QLabel(row);
    swatch->setFixedSize(13, 13);
    swatch->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    swatch->setStyleSheet(QString("background:%1; border-radius:6px;").arg(segment->color().name()));
    layout->addWidget(swatch);

    const QString label = segment->start() && segment->end()
                              ? QString("%1—%2").arg(segment->start()->name(), segment->end()->name())
                              : QString("线段");
    QLabel* nameLabel = new QLabel(label, row);
    nameLabel->setObjectName("cartesianObjectName");
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    nameLabel->setFont(UiFonts::forText(label, 12, true));
    nameLabel->setFixedWidth(56);
    layout->addWidget(nameLabel);

    QLabel* eqLabel = new QLabel(segment->equationText(), row);
    eqLabel->setObjectName("cartesianObjectValue");
    eqLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    eqLabel->setFont(UiFonts::mono(11, false));
    layout->addWidget(eqLabel, 1);

    MenuButton* menu = new MenuButton(row);
    layout->addWidget(menu);

    menu->setOnClick([this, segment, index, menu]() {
        showSegmentMenu(segment, index, menu->mapToGlobal(QPoint(0, menu->height())));
    });

    row->installEventFilter(this);
    row->setProperty("cartesianSegmentIndex", index);
    row->setCursor(Qt::PointingHandCursor);

    m_rows.push_back(row);
    return row;
}

QWidget* CartesianPanel::buildFunctionRow(CartesianCurveItem* curve, int index) {
    QWidget* row = new QWidget(m_listHost);
    row->setObjectName("cartesianObjectRow");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 5, 4, 5);
    layout->setSpacing(8);

    QLabel* swatch = new QLabel(row);
    swatch->setFixedSize(13, 13);
    swatch->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    swatch->setStyleSheet(QString("background:%1; border-radius:6px;").arg(curve->color().name()));
    layout->addWidget(swatch);

    QLabel* nameLabel = new QLabel(curve->name(), row);
    nameLabel->setObjectName("cartesianObjectName");
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    nameLabel->setFont(UiFonts::forText(curve->name(), 12, true));
    nameLabel->setFixedWidth(40);
    layout->addWidget(nameLabel);

    QLabel* exprLabel = new QLabel(curve->expression(), row);
    exprLabel->setObjectName("cartesianObjectValue");
    exprLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    exprLabel->setFont(UiFonts::mono(11, false));
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
        editPointCoordinates(point, index);
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

void CartesianPanel::showSegmentMenu(CartesianSegmentItem* segment, int index, const QPoint& globalPos) {
    QMenu menu(this);
    QAction* inspectAction = menu.addAction("查看一次方程");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction("删除");
    Q_UNUSED(segment);

    QAction* chosen = menu.exec(globalPos);
    if (!chosen) return;
    if (chosen == inspectAction) {
        emit segmentSelected(index);
    } else if (chosen == deleteAction) {
        if (m_engine) m_engine->removeSegment(index);
        emit statusMessage("已删除该线段");
    }
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
