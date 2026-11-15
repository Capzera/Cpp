#include "CanvasToolBar.h"
#include "ToolIconButton.h"
#include "MyButton.h"
#include "MyColorPicker.h"
#include "MySlider.h"
#include "MyTextInput.h"
#include "ui/UiTheme.h"
#include "canvas/ToolController.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QStackedWidget>
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

void CanvasToolBar::buildMainBar() {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(9, 8, 9, 8);
    layout->setSpacing(2);

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

void CanvasToolBar::showToolPage(int toolIndex) {
    if (!m_toolPages) return;
    const int count = m_toolPages->count();
    for (int i = 0; i < count; ++i) {
        QWidget* page = m_toolPages->widget(i);
        if (!page) continue;
        if (page->objectName() == QString("canvasToolPage_%1").arg(toolIndex)) {
            m_toolPages->setCurrentIndex(i);
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

    const int barW = sizeHint().width();
    const int barH = sizeHint().height();
    const int x = qMax(8, (areaW - barW) / 2);
    const int y = 16;
    setGeometry(x, y, barW, barH);

    if (m_moreOpen && m_morePanel) placePanel(m_morePanel, x, barW, y, barH);
    if (m_toolPanelOpen && m_toolPanel) placeToolPanelUnderButton();
}

bool CanvasToolBar::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_host && event->type() == QEvent::Resize) {
        reposition();
        return false;
    }
    return QWidget::eventFilter(obj, event);
}
