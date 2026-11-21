#include "CanvasToolBar.h"
#include "ToolIconButton.h"
#include "MyButton.h"
#include "MyColorPicker.h"
#include "MySlider.h"
#include "MySegmentedControl.h"
#include "MyTextInput.h"
#include "MyToggleSwitch.h"
#include "ui/UiTheme.h"
#include "canvas/ToolController.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QStackedWidget>
#include <QApplication>
#include <QMouseEvent>
#include <functional>

namespace {

class ToolCard : public QWidget {
public:
    explicit ToolCard(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(box, 13, 13);
        painter.setPen(QPen(QColor(0, 0, 0, 34), 1));
        painter.setBrush(QColor(255, 255, 255, 253));
        painter.drawPath(path);
        QPainterPath inner;
        inner.addRoundedRect(box.adjusted(1, 1, -1, -1), 12, 12);
        painter.setPen(QPen(QColor(255, 255, 255, 190), 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(inner);
    }
};

class ToolDivider : public QWidget {
public:
    explicit ToolDivider(QWidget* parent = nullptr) : QWidget(parent) {
        setFixedWidth(1);
        setFixedHeight(20);
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setPen(QPen(QColor(214, 221, 233), 1));
        painter.drawLine(0, 2, 0, height() - 2);
    }
};

QLabel* makePanelTitle(const QString& text, QWidget* parent) {
    QLabel* label = new QLabel(text, parent);
    label->setObjectName("canvasStyleTitle");
    label->setFont(UiTheme::uiFont(10, true));
    label->setStyleSheet("color:#5c6675; background:transparent;");
    return label;
}

QLabel* makePanelLabel(const QString& text, QWidget* parent) {
    QLabel* label = new QLabel(text, parent);
    label->setObjectName("canvasStyleLabel");
    label->setFont(UiTheme::uiFont(9));
    label->setStyleSheet("color:#7a8494; background:transparent;");
    return label;
}

}  // namespace

CanvasToolBar::CanvasToolBar(ToolController* controller, QWidget* parent)
    : QWidget(parent), m_controller(controller) {
    setObjectName("canvasToolSuite");
    setAttribute(Qt::WA_TranslucentBackground, true);
    buildMainBar();
    qApp->installEventFilter(this);
}

CanvasToolBar::~CanvasToolBar() {
    if (qApp) qApp->removeEventFilter(this);
}

int CanvasToolBar::mainBarWidth() const {
    return sizeHint().width();
}

ToolIconButton* CanvasToolBar::toolButtonFor(int tool) const {
    for (ToolIconButton* button : m_toolButtons) {
        if (!button) continue;
        if (static_cast<int>(button->kind()) == tool) return button;
    }
    return nullptr;
}

ToolIconButton* CanvasToolBar::cartesianButtonFor(int kind) const {
    for (ToolIconButton* button : m_cartesianButtons) {
        if (!button) continue;
        if (static_cast<int>(button->kind()) == kind) return button;
    }
    return nullptr;
}

int CanvasToolBar::mainRowWidth() const {
    QWidget* mainRow = findChild<QWidget*>("canvasToolMainRow");
    if (!mainRow) return sizeHint().width();
    return mainRow->sizeHint().width();
}

int CanvasToolBar::mainRowHeight() const {
    QWidget* mainRow = findChild<QWidget*>("canvasToolMainRow");
    if (!mainRow) return sizeHint().height();
    return mainRow->sizeHint().height();
}

void CanvasToolBar::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, 13, 13);
    painter.setPen(QPen(QColor(0, 0, 0, 28), 1));
    painter.setBrush(QColor(255, 255, 255, 252));
    painter.drawPath(path);
    QPainterPath inner;
    inner.addRoundedRect(box.adjusted(1, 1, -1, -1), 12, 12);
    painter.setPen(QPen(QColor(255, 255, 255, 190), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(inner);
}

void CanvasToolBar::buildCartesianRow() {
    m_cartesianRow = new QWidget(this);
    m_cartesianRow->setObjectName("canvasCartesianRow");
    m_cartesianRow->setAttribute(Qt::WA_TranslucentBackground, true);

    QHBoxLayout* layout = new QHBoxLayout(m_cartesianRow);
    layout->setContentsMargins(9, 3, 9, 3);
    layout->setSpacing(2);

    struct Entry {
        ToolIconButton::Kind kind;
        const char* tip;
    };
    const Entry entries[] = {
        {ToolIconButton::CartPoint, "画点工具 (P) — 再次点击展开描点设置"},
        {ToolIconButton::CartLine, "连线工具 (S) — 奇数次选点为起点，偶数次选点连线"},
        {ToolIconButton::CartFunction, "函数绘制 — 在左侧列表输入 f(x) = 表达式"}
    };

    for (const Entry& entry : entries) {
        ToolIconButton* button = new ToolIconButton(entry.kind, entry.tip, m_cartesianRow);
        button->setObjectName(QString("canvasCartTool_%1").arg(static_cast<int>(entry.kind)));
        connect(button, &QAbstractButton::clicked, this, [this, button, entry]() {
            const bool alreadyActive = button->isActive();
            if (alreadyActive) {
                m_cartesianPanelOpen = !m_cartesianPanelOpen;
                if (m_toolPanel) {
                    if (m_cartesianPanelOpen) {
                        showToolPage(-1 - static_cast<int>(entry.kind));
                        m_toolPanel->adjustSize();
                        m_toolPanel->layout()->activate();
                        m_toolPanel->show();
                        m_toolPanel->raise();
                    } else {
                        m_toolPanel->hide();
                    }
                }
                reposition();
                return;
            }
            for (ToolIconButton* other : m_cartesianButtons) other->setActive(false);
            button->setActive(true);
            m_cartesianPanelOpen = false;
            if (m_toolPanel) m_toolPanel->hide();
            emit cartesianToolChanged(static_cast<int>(entry.kind));
            reposition();
        });
        m_cartesianButtons.push_back(button);
        layout->addWidget(button);
    }

    layout->addWidget(new ToolDivider(this));

    MyButton* namesButton = new MyButton("描点显示名字", m_cartesianRow);
    namesButton->setObjectName("canvasCartPointNames");
    namesButton->setVariant(MyButton::Ghost);
    namesButton->setCompact(true);
    connect(namesButton, &MyButton::clicked, this, [this, namesButton]() {
        m_cartesianPointNames = !m_cartesianPointNames;
        namesButton->setText(m_cartesianPointNames ? "描点显示名字" : "描点隐藏名字");
        emit cartesianPointNamesChanged(m_cartesianPointNames);
    });
    layout->addWidget(namesButton);

    m_cartesianRow->setVisible(false);
}

void CanvasToolBar::setCartesianMode(bool on) {
    if (m_cartesianMode == on) return;
    m_cartesianMode = on;
    if (!m_cartesianRow) buildCartesianRow();
    if (m_cartesianRow) {
        m_cartesianRow->setVisible(on);
        if (!on) {
            m_cartesianPanelOpen = false;
            for (ToolIconButton* button : m_cartesianButtons) button->setActive(false);
            if (m_toolPanel) m_toolPanel->hide();
        }
    }
    updateGeometry();
    reposition();
}

void CanvasToolBar::setCartesianPointNames(bool on) {
    m_cartesianPointNames = on;
    if (MyButton* button = m_cartesianRow ? m_cartesianRow->findChild<MyButton*>("canvasCartPointNames") : nullptr) {
        button->setText(on ? "描点显示名字" : "描点隐藏名字");
    }
}

void CanvasToolBar::buildMainBar() {
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    QWidget* mainRow = new QWidget(this);
    mainRow->setObjectName("canvasToolMainRow");
    mainRow->setAttribute(Qt::WA_TranslucentBackground, true);
    QHBoxLayout* layout = new QHBoxLayout(mainRow);
    layout->setContentsMargins(9, 8, 9, 8);
    layout->setSpacing(2);
    root->addWidget(mainRow);

    const ToolController::Tool tools[] = {
        ToolController::Select, ToolController::AnnotationSelect, ToolController::Pen,
        ToolController::Highlighter, ToolController::Eraser, ToolController::Line,
        ToolController::Rect, ToolController::Circle, ToolController::Arrow,
        ToolController::Triangle, ToolController::Mosaic,
        ToolController::TextBox, ToolController::Markdown,
        ToolController::Latex, ToolController::Table
    };
    static const QString tips[] = {
        "指针 / 平移 (V)", "选择批注 (A)", "画笔 (B) — 再次点击展开画笔设置",
        "荧光笔 (H) — 再次点击展开荧光笔设置", "橡皮 (E) — 再次点击展开橡皮设置",
        "直线 (L, Shift 锁定 45°)", "矩形 (R, Shift 正方)",
        "圆形 (O, Shift 正圆)", "箭头 (D)", "三角形 (T, Shift 等腰)", "马赛克 (M)",
        "文本框 (N) — 再次点击展开文本设置", "Markdown 笔记 (G) — 再次点击展开笔记设置",
        "LaTeX 公式 (X)", "表格 (I) — 再次点击展开表格设置"
    };

    for (int i = 0; i < 15; ++i) {
        ToolIconButton* button = new ToolIconButton(
            static_cast<ToolIconButton::Kind>(ToolIconButton::Select + i), tips[i], this);
        const ToolController::Tool tool = tools[i];
        button->setObjectName(QString("canvasTool_%1").arg(static_cast<int>(tool)));
        connect(button, &QAbstractButton::clicked, this, [this, button, tool]() {
            const bool alreadyActive = (m_controller->currentTool() == tool);
            if (alreadyActive) {
                toggleToolPanel();
                return;
            }
            m_controller->setTool(tool);
            button->setActive(true);
            closeFlyouts();
        });
        m_toolButtons.push_back(button);
        layout->addWidget(button);
    }

    layout->addWidget(new ToolDivider(this));

    m_btnMore = new ToolIconButton(ToolIconButton::More, "更多 (显隐 / 锁定 / 导出 / 清空)", this);
    m_btnMore->setObjectName("canvasMoreToggle");
    connect(m_btnMore, &QAbstractButton::clicked, this, [this]() { toggleMorePanel(); });
    layout->addWidget(m_btnMore);

    connect(m_controller, &ToolController::toolChanged, this, [this](ToolController::Tool tool) {
        syncToolButtons();
        if (m_toolPanelOpen) {
            refreshToolPage(static_cast<int>(tool));
            if (m_toolPanel) {
                m_toolPanel->adjustSize();
                m_toolPanel->layout()->activate();
            }
            placeToolPanelUnderButton();
        }
    });
}

void CanvasToolBar::syncToolButtons() {
    const int current = static_cast<int>(m_controller->currentTool());
    for (ToolIconButton* button : m_toolButtons) {
        if (!button) continue;
        button->setActive(static_cast<int>(button->kind()) == current);
    }
}

void CanvasToolBar::buildMorePanel() {
    m_morePanel = new ToolCard(m_host ? m_host : this);
    m_morePanel->setObjectName("canvasMorePanel");
    m_morePanel->setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* layout = new QVBoxLayout(m_morePanel);
    layout->setContentsMargins(15, 13, 15, 13);
    layout->setSpacing(8);

    layout->addWidget(makePanelTitle("图层与文件", m_morePanel));

    QHBoxLayout* layerRow = new QHBoxLayout();
    layerRow->setSpacing(4);

    MyButton* visible = new MyButton("隐藏批注", m_morePanel);
    visible->setObjectName("canvasVisibilityButton");
    visible->setVariant(MyButton::Ghost);
    visible->setCompact(true);
    connect(visible, &MyButton::clicked, this, [this, visible]() {
        m_controller->setAnnotationsVisible(!m_controller->annotationsVisible());
        visible->setText(m_controller->annotationsVisible() ? "隐藏批注" : "显示批注");
    });
    layerRow->addWidget(visible);

    MyButton* lock = new MyButton("锁定批注", m_morePanel);
    lock->setObjectName("canvasLockButton");
    lock->setVariant(MyButton::Ghost);
    lock->setCompact(true);
    connect(lock, &MyButton::clicked, this, [this, lock]() {
        m_controller->setAnnotationsLocked(!m_controller->annotationsLocked());
        lock->setText(m_controller->annotationsLocked() ? "解锁批注" : "锁定批注");
    });
    layerRow->addWidget(lock);
    layout->addLayout(layerRow);

    QHBoxLayout* fileRow = new QHBoxLayout();
    fileRow->setSpacing(4);

    MyButton* exportBtn = new MyButton("导出 PDF", m_morePanel);
    exportBtn->setObjectName("canvasExportButton");
    exportBtn->setVariant(MyButton::Soft);
    exportBtn->setCompact(true);
    connect(exportBtn, &MyButton::clicked, this, [this]() { emit exportRequested(); });
    fileRow->addWidget(exportBtn);

    MyButton* clearBtn = new MyButton("清空批注", m_morePanel);
    clearBtn->setObjectName("canvasClearButton");
    clearBtn->setVariant(MyButton::Ghost);
    clearBtn->setCompact(true);
    connect(clearBtn, &MyButton::clicked, this, [this]() { emit clearRequested(); });
    fileRow->addWidget(clearBtn);
    layout->addLayout(fileRow);

    QLabel* hint = makePanelLabel("复制 / 粘贴 / 剪切 / 删除 / 全选 / 撤销 / 重做 均使用键盘快捷键", m_morePanel);
    hint->setWordWrap(true);
    layout->addWidget(hint);
}

void CanvasToolBar::buildToolPanel() {
    m_toolPanel = new ToolCard(m_host ? m_host : this);
    m_toolPanel->setObjectName("canvasToolPanel");
    m_toolPanel->setAttribute(Qt::WA_TranslucentBackground, true);

    m_toolPages = new QStackedWidget(m_toolPanel);
    m_toolPages->setObjectName("canvasToolPages");
    m_toolPages->setStyleSheet("background:transparent;");

    QVBoxLayout* outer = new QVBoxLayout(m_toolPanel);
    outer->setContentsMargins(15, 13, 15, 13);
    outer->setSpacing(8);
    outer->addWidget(m_toolPages);

    const ToolController::Tool tools[] = {
        ToolController::Pen, ToolController::Highlighter, ToolController::Line,
        ToolController::Rect, ToolController::Circle, ToolController::Arrow,
        ToolController::Triangle, ToolController::Eraser, ToolController::Mosaic,
        ToolController::TextBox, ToolController::Markdown, ToolController::Latex,
        ToolController::Table
    };
    for (ToolController::Tool tool : tools) {
        m_toolPages->addWidget(createToolPage(static_cast<int>(tool)));
    }

    m_toolPages->addWidget(createCartesianPage(static_cast<int>(ToolIconButton::CartPoint)));
    m_toolPages->addWidget(createCartesianPage(static_cast<int>(ToolIconButton::CartLine)));
    m_toolPages->addWidget(createCartesianPage(static_cast<int>(ToolIconButton::CartFunction)));
}

QWidget* CanvasToolBar::createCartesianPage(int kind) {
    QWidget* page = new QWidget(m_toolPages);
    page->setObjectName(QString("canvasToolPage_cart_%1").arg(kind));
    page->setAttribute(Qt::WA_TranslucentBackground, true);
    page->setStyleSheet("background:transparent;");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    if (kind == static_cast<int>(ToolIconButton::CartPoint)) {
        layout->addWidget(makePanelTitle("画点设置", page));
        QHBoxLayout* nameRow = new QHBoxLayout();
        nameRow->setSpacing(8);
        QLabel* nameLabel = makePanelLabel("描点名字", page);
        nameLabel->setFixedWidth(52);
        nameRow->addWidget(nameLabel);
        MyToggleSwitch* nameSwitch = new MyToggleSwitch(page);
        nameSwitch->setObjectName("canvasCartPointNameSwitch");
        nameSwitch->setOnText("显示");
        nameSwitch->setOffText("隐藏");
        nameSwitch->setChecked(m_cartesianPointNames);
        connect(nameSwitch, &MyToggleSwitch::toggled, this, [this](bool on) {
            m_cartesianPointNames = on;
            emit cartesianPointNamesChanged(on);
        });
        nameRow->addWidget(nameSwitch);
        nameRow->addStretch(1);
        layout->addLayout(nameRow);

        QLabel* hint = makePanelLabel("单击落点，长按拖动可让圆点跟随鼠标并吸附到网格交点", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
    } else if (kind == static_cast<int>(ToolIconButton::CartLine)) {
        layout->addWidget(makePanelTitle("连线设置", page));
        QLabel* hint = makePanelLabel("奇数次选点为起点（高亮显示），偶数次选点即连成线段；双击线段查看一次方程", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
    } else {
        layout->addWidget(makePanelTitle("函数绘制", page));
        QLabel* hint = makePanelLabel("在左侧「函数绘制」输入 f(x) = 表达式，支持一次 / 二次 / 三次函数，可同时绘制多条曲线", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
    }
    return page;
}

QWidget* CanvasToolBar::createToolPage(int toolIndex) {
    const ToolController::Tool tool = static_cast<ToolController::Tool>(toolIndex);
    QWidget* page = new QWidget(m_toolPages);
    page->setObjectName(QString("canvasToolPage_%1").arg(static_cast<int>(tool)));
    page->setAttribute(Qt::WA_TranslucentBackground, true);
    page->setStyleSheet("background:transparent;");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    const ToolController::ToolStyle style = m_controller->styleFor(tool);

    auto addSliderRow = [this, layout, page](const QString& label, int min, int max, int value,
                                             const QString& objectName,
                                             const std::function<void(int)>& sink) {
        QHBoxLayout* row = new QHBoxLayout();
        row->setSpacing(8);
        QLabel* text = makePanelLabel(label, page);
        text->setFixedWidth(46);
        row->addWidget(text);
        MySlider* slider = new MySlider(page);
        slider->setObjectName(objectName);
        slider->setRange(min, max);
        slider->setValue(value);
        slider->setFixedWidth(132);
        connect(slider, &MySlider::valueChanged, this, [sink](int v) { sink(v); });
        row->addWidget(slider, 1);
        layout->addLayout(row);
        return slider;
    };

    auto addColorRow = [this, layout, page, tool](const QColor& initial) {
        MyColorPicker* picker = new MyColorPicker(page);
        picker->setObjectName("canvasToolColorPicker");
        picker->setColor(initial);
        connect(picker, &MyColorPicker::colorPicked, this, [this, tool](const QColor& c) {
            ToolController::ToolStyle s = m_controller->styleFor(tool);
            s.color = c;
            m_controller->setStyleFor(tool, s);
            m_controller->setPenColor(c);
        });
        layout->addWidget(picker);
        return picker;
    };

    switch (tool) {
    case ToolController::Pen:
    case ToolController::Highlighter: {
        layout->addWidget(makePanelTitle(tool == ToolController::Highlighter
                                             ? "荧光笔设置" : "画笔设置", page));
        addColorRow(style.color);
        addSliderRow("粗细", 1, 40, static_cast<int>(style.width),
                     "canvasToolWidth", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.width = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setPenWidth(v);
                     });
        addSliderRow("不透明", 10, 100, static_cast<int>(style.opacity * 100),
                     "canvasToolOpacity", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.opacity = v / 100.0;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setOpacity(v / 100.0);
                     });
        break;
    }
    case ToolController::Line:
    case ToolController::Rect:
    case ToolController::Circle:
    case ToolController::Arrow:
    case ToolController::Triangle: {
        layout->addWidget(makePanelTitle("形状设置", page));
        if (tool == ToolController::Rect || tool == ToolController::Circle || tool == ToolController::Triangle) {
            QHBoxLayout* fillRow = new QHBoxLayout();
            fillRow->setSpacing(8);
            QLabel* fillLabel = makePanelLabel("样式", page);
            fillLabel->setFixedWidth(46);
            fillRow->addWidget(fillLabel);
            MySegmentedControl* fillPicker = new MySegmentedControl({"空心", "实心"}, page);
            fillPicker->setObjectName("canvasToolShapeFill");
            if (tool == ToolController::Rect) fillPicker->setShapeIcon(MySegmentedControl::RectIcon);
            else if (tool == ToolController::Circle) fillPicker->setShapeIcon(MySegmentedControl::CircleIcon);
            else fillPicker->setShapeIcon(MySegmentedControl::TriangleIcon);
            fillPicker->setCurrentIndex(style.filled ? 1 : 0);
            connect(fillPicker, &MySegmentedControl::currentIndexChanged, this, [this, tool](int idx) {
                ToolController::ToolStyle s = m_controller->styleFor(tool);
                s.filled = (idx == 1);
                m_controller->setStyleFor(tool, s);
                m_controller->setShapeFilled(idx == 1);
            });
            fillRow->addWidget(fillPicker, 1);
            layout->addLayout(fillRow);
        }
        addColorRow(style.color);
        addSliderRow("粗细", 1, 40, static_cast<int>(style.width),
                     "canvasToolWidth", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.width = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setPenWidth(v);
                     });
        addSliderRow("不透明", 10, 100, static_cast<int>(style.opacity * 100),
                     "canvasToolOpacity", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.opacity = v / 100.0;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setOpacity(v / 100.0);
                     });
        break;
    }
    case ToolController::Eraser: {
        layout->addWidget(makePanelTitle("橡皮设置", page));
        addSliderRow("大小", 4, 60, static_cast<int>(style.eraserRadius),
                     "canvasToolEraser", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.eraserRadius = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setEraserRadius(v);
                     });
        QHBoxLayout* shapeRow = new QHBoxLayout();
        shapeRow->setSpacing(8);
        QLabel* shapeLabel = makePanelLabel("形状", page);
        shapeLabel->setFixedWidth(46);
        shapeRow->addWidget(shapeLabel);
        MySegmentedControl* shapePicker = new MySegmentedControl({"圆形", "方形"}, page);
        shapePicker->setObjectName("canvasToolEraserShape");
        shapePicker->setCurrentIndex(style.eraserSquare ? 1 : 0);
        connect(shapePicker, &MySegmentedControl::currentIndexChanged, this, [this, tool](int idx) {
            ToolController::ToolStyle s = m_controller->styleFor(tool);
            s.eraserSquare = (idx == 1);
            m_controller->setStyleFor(tool, s);
            m_controller->setEraserSquare(idx == 1);
        });
        shapeRow->addWidget(shapePicker, 1);
        layout->addLayout(shapeRow);
        QHBoxLayout* rightRow = new QHBoxLayout();
        rightRow->setSpacing(8);
        QLabel* rightLabel = makePanelLabel("右键擦除", page);
        rightLabel->setFixedWidth(46);
        rightRow->addWidget(rightLabel);
        MyToggleSwitch* rightSwitch = new MyToggleSwitch(page);
        rightSwitch->setObjectName("canvasToolRightClickErase");
        rightSwitch->setChecked(m_controller->rightClickEraseEnabled());
        rightSwitch->setToolTip("画笔 / 荧光笔按住右键即为橡皮擦除");
        connect(rightSwitch, &MyToggleSwitch::toggled, this, [this](bool on) {
            m_controller->setRightClickEraseEnabled(on);
        });
        rightRow->addWidget(rightSwitch);
        rightRow->addStretch(1);
        layout->addLayout(rightRow);
        break;
    }
    case ToolController::Mosaic: {
        layout->addWidget(makePanelTitle("马赛克设置", page));
        addSliderRow("格子", 4, 48, style.mosaicCell,
                     "canvasToolMosaic", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.mosaicCell = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setMosaicCellSize(v);
                     });
        break;
    }
    case ToolController::Table: {
        layout->addWidget(makePanelTitle("表格设置", page));
        addSliderRow("行数", 1, 12, style.tableRows,
                     "canvasToolTableRows", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.tableRows = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setTableSize(v, s.tableCols);
                     });
        addSliderRow("列数", 1, 12, style.tableCols,
                     "canvasToolTableCols", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.tableCols = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setTableSize(s.tableRows, v);
                     });
        break;
    }
    case ToolController::TextBox: {
        layout->addWidget(makePanelTitle("文本框设置", page));
        addSliderRow("字号", 8, 48, style.textSize,
                     "canvasToolTextSize", [this, tool](int v) {
                         ToolController::ToolStyle s = m_controller->styleFor(tool);
                         s.textSize = v;
                         m_controller->setStyleFor(tool, s);
                         m_controller->setTextSize(v);
                     });
        QLabel* hint = makePanelLabel("插入后可直接在画布上键入，双击已有文本框可继续编辑", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
        break;
    }
    case ToolController::Latex: {
        layout->addWidget(makePanelTitle("LaTeX 公式", page));
        QLabel* hint = makePanelLabel("点击画布插入公式后可在弹窗中编写源码并实时预览，拖动公式卡片可移动位置", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
        break;
    }
    case ToolController::Markdown: {
        layout->addWidget(makePanelTitle("Markdown 笔记", page));
        QLabel* hint = makePanelLabel("点击画布插入笔记后可直接键入 Markdown 源码，拖动笔记卡片可移动位置", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
        break;
    }
    default:
        break;
    }
    return page;
}

void CanvasToolBar::applyToolPageSize() {
    if (!m_toolPages) return;
    QWidget* page = m_toolPages->currentWidget();
    if (!page) return;
    if (page->layout()) page->layout()->activate();
    page->adjustSize();
    const QSize hint = page->sizeHint();
    if (hint.width() > 0 && hint.height() > 0) {
        m_toolPages->setFixedSize(hint);
    }
    if (m_toolPanel) {
        m_toolPanel->adjustSize();
        m_toolPanel->layout()->activate();
    }
}

void CanvasToolBar::showToolPage(int toolIndex) {
    if (!m_toolPages) return;
    if (toolIndex < 0) {
        const QString target = QString("canvasToolPage_cart_%1").arg(-toolIndex - 1);
        const int count = m_toolPages->count();
        for (int i = 0; i < count; ++i) {
            QWidget* page = m_toolPages->widget(i);
            if (!page) continue;
            if (page->objectName() == target) {
                m_toolPages->setCurrentIndex(i);
                applyToolPageSize();
                return;
            }
        }
        return;
    }
    const int count = m_toolPages->count();
    for (int i = 0; i < count; ++i) {
        QWidget* page = m_toolPages->widget(i);
        if (!page) continue;
        if (page->objectName() == QString("canvasToolPage_%1").arg(toolIndex)) {
            m_toolPages->setCurrentIndex(i);
            applyToolPageSize();
            return;
        }
    }
}

void CanvasToolBar::refreshToolPage(int toolIndex) {
    if (!m_toolPages) return;
    QWidget* page = m_toolPages->currentWidget();
    if (!page) return;
    if (page->objectName() != QString("canvasToolPage_%1").arg(toolIndex)) {
        showToolPage(toolIndex);
        return;
    }
    const int index = m_toolPages->indexOf(page);
    if (index < 0) return;
    QWidget* fresh = createToolPage(toolIndex);
    m_toolPages->removeWidget(page);
    page->deleteLater();
    m_toolPages->insertWidget(index, fresh);
    m_toolPages->setCurrentIndex(index);
    applyToolPageSize();
}

void CanvasToolBar::toggleMorePanel() {
    m_moreOpen = !m_moreOpen;
    if (m_toolPanelOpen) {
        m_toolPanelOpen = false;
        if (m_toolPanel) m_toolPanel->hide();
    }
    if (m_morePanel) {
        if (m_moreOpen) {
            m_morePanel->show();
            m_morePanel->raise();
        } else {
            m_morePanel->hide();
        }
    }
    reposition();
}

void CanvasToolBar::toggleToolPanel() {
    m_toolPanelOpen = !m_toolPanelOpen;
    if (m_moreOpen) {
        m_moreOpen = false;
        if (m_morePanel) m_morePanel->hide();
    }
    if (m_toolPanel) {
        if (m_toolPanelOpen) {
            showToolPage(static_cast<int>(m_controller->currentTool()));
            m_toolPanel->layout()->activate();
            m_toolPanel->adjustSize();
            m_toolPanel->show();
            m_toolPanel->raise();
        } else {
            m_toolPanel->hide();
        }
    }
    reposition();
}

void CanvasToolBar::closeFlyouts() {
    m_moreOpen = false;
    m_toolPanelOpen = false;
    if (m_morePanel) m_morePanel->hide();
    if (m_toolPanel) m_toolPanel->hide();
    reposition();
}

void CanvasToolBar::setHostWidget(QWidget* host) {
    m_host = host;
    if (m_host) {
        m_host->installEventFilter(this);
        buildMorePanel();
        buildToolPanel();
        if (m_morePanel) m_morePanel->hide();
        if (m_toolPanel) m_toolPanel->hide();
    }
    reposition();
}

bool CanvasToolBar::isInsidePanel(QWidget* panel, const QPoint& globalPos) const {
    if (!panel || !panel->isVisible()) return false;
    return panel->geometry().contains(panel->parentWidget()
                                         ? panel->parentWidget()->mapFromGlobal(globalPos)
                                         : panel->mapFromGlobal(globalPos));
}

bool CanvasToolBar::isInsideToolbar(const QPoint& globalPos) const {
    if (!isVisible()) return false;
    QWidget* parent = parentWidget();
    if (!parent) return false;
    return geometry().contains(parent->mapFromGlobal(globalPos));
}

bool CanvasToolBar::shouldCloseOnPress(QObject* obj, QEvent* event) const {
    if (!m_toolPanelOpen && !m_moreOpen) return false;
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if (mouseEvent->button() != Qt::LeftButton) return false;
    const QPoint globalPos = mouseEvent->globalPosition().toPoint();
    if (isInsidePanel(m_toolPanel, globalPos)) return false;
    if (isInsidePanel(m_morePanel, globalPos)) return false;
    if (isInsideToolbar(globalPos)) return false;
    return true;
}

void CanvasToolBar::placePanel(QWidget* panel, int barX, int barW, int barY, int barH) {
    if (!panel || !m_host) return;
    panel->adjustSize();
    const int panelW = panel->sizeHint().width();
    const int panelH = panel->sizeHint().height();
    const int areaW = m_host->width();
    const int areaH = m_host->height();
    int px = barX + barW - panelW;
    if (px + panelW > areaW - 8) px = areaW - panelW - 8;
    if (px < 8) px = 8;
    int py = barY + barH + 10;
    if (py + panelH > areaH - 8) py = barY - panelH - 10;
    if (py < 8) py = 8;
    panel->setGeometry(px, py, panelW, panelH);
}

void CanvasToolBar::placeToolPanelUnderButton() {
    if (!m_toolPanel || !m_host) return;
    const int tool = static_cast<int>(m_controller->currentTool());
    ToolIconButton* button = toolButtonFor(tool);
    if (!button) {
        placePanel(m_toolPanel, 0, 0, 0, 0);
        return;
    }
    m_toolPanel->adjustSize();
    const int panelW = m_toolPanel->sizeHint().width();
    const int panelH = m_toolPanel->sizeHint().height();
    const QPoint buttonGlobalCenter = button->mapToGlobal(QPoint(button->width() / 2, 0));
    const QPoint buttonGlobalLeft = button->mapToGlobal(QPoint(0, 0));
    const QPoint hostLocal = m_host->mapFromGlobal(buttonGlobalLeft);

    int px = hostLocal.x() + (button->width() - panelW) / 2;
    const int areaW = m_host->width();
    if (px + panelW > areaW - 8) px = areaW - panelW - 8;
    if (px < 8) px = 8;

    int py = hostLocal.y() + button->height() + 8;
    const int areaH = m_host->height();
    if (py + panelH > areaH - 8) py = hostLocal.y() - panelH - 8;
    if (py < 8) py = 8;

    m_toolPanel->setGeometry(px, py, panelW, panelH);
}

void CanvasToolBar::reposition() {
    if (!m_host) return;
    const int areaW = m_host->width();
    const int areaH = m_host->height();

    int barW = mainRowWidth();
    int barH = m_cartesianRow && m_cartesianRow->isVisible()
                   ? mainRowHeight() + m_cartesianRow->sizeHint().height()
                   : mainRowHeight();

    int x = qMax(8, (areaW - barW) / 2);
    int y = 16;
    setGeometry(x, y, barW, barH);

    if (m_cartesianRow && m_cartesianRow->isVisible()) {
        const QPoint rowPos = mapTo(m_cartesianRow, QPoint(0, 0));
        m_cartesianRow->setGeometry(rowPos.x(), rowPos.y(), barW, m_cartesianRow->sizeHint().height());
        m_cartesianRow->raise();
    }

    if (m_moreOpen && m_morePanel) placePanel(m_morePanel, x, barW, y, barH);
    if (m_toolPanelOpen && m_toolPanel) placeToolPanelUnderButton();
}

bool CanvasToolBar::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_host && event->type() == QEvent::Resize) {
        reposition();
        return false;
    }
    if (event->type() == QEvent::MouseButtonPress && shouldCloseOnPress(obj, event)) {
        closeFlyouts();
        return false;
    }
    return QWidget::eventFilter(obj, event);
}
