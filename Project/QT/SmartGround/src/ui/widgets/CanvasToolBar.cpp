#include "CanvasToolBar.h"
#include "ToolIconButton.h"
#include "MyButton.h"
#include "MyColorSwatch.h"
#include "MySlider.h"
#include "ui/UiTheme.h"
#include "canvas/ToolController.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QScrollArea>
#include <QStackedWidget>
#include <QCursor>
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
        "指针 / 平移 (V)", "选择批注 (A)", "画笔 (B) — 点击可调颜色与粗细", "荧光笔 (H)",
        "橡皮 (E) — 点击可调大小", "直线 (L, Shift 锁定 45°)", "矩形 (R, Shift 正方)",
        "圆形 (O, Shift 正圆)", "箭头 (D)", "三角形 (T, Shift 等腰)", "马赛克 (M)",
        "文本框 (N) — 点击可调字号", "Markdown 笔记 (G)", "LaTeX 公式 (X)",
        "表格 (I) — 点击可调行列"
    };

    for (int i = 0; i < 15; ++i) {
        ToolIconButton* button = new ToolIconButton(
            static_cast<ToolIconButton::Kind>(ToolIconButton::Select + i), tips[i], this);
        const ToolController::Tool tool = tools[i];
        button->setObjectName(QString("canvasTool_%1").arg(static_cast<int>(tool)));
        connect(button, &QAbstractButton::clicked, this, [this, button, tool]() {
            const bool alreadyActive = (m_controller->currentTool() == tool);
            m_controller->setTool(tool);
            button->setActive(true);
            if (alreadyActive) {
                toggleToolPanel();
            } else {
                closeFlyouts();
            }
        });
        m_toolButtons.push_back(button);
        layout->addWidget(button);
    }

    layout->addWidget(new ToolDivider(this));

    m_btnStyle = new ToolIconButton(ToolIconButton::Style, "全局样式 (颜色 / 粗细 / 不透明度)", this);
    m_btnStyle->setObjectName("canvasStyleToggle");
    connect(m_btnStyle, &QAbstractButton::clicked, this, [this]() { toggleStylePanel(); });
    layout->addWidget(m_btnStyle);

    m_btnMore = new ToolIconButton(ToolIconButton::More, "更多工具 (撤销 / 复制 / 导出等)", this);
    m_btnMore->setObjectName("canvasMoreToggle");
    connect(m_btnMore, &QAbstractButton::clicked, this, [this]() { toggleMorePanel(); });
    layout->addWidget(m_btnMore);

    connect(m_controller, &ToolController::toolChanged, this, [this](ToolController::Tool tool) {
        for (ToolIconButton* button : m_toolButtons) {
            if (!button) continue;
            button->setActive(static_cast<int>(tool) == static_cast<int>(button->kind()));
        }
        if (m_toolPanelOpen) showToolPage(static_cast<int>(tool));
    });
}

void CanvasToolBar::buildStylePanel() {
    m_stylePanel = new ToolCard(m_host ? m_host : this);
    m_stylePanel->setObjectName("canvasStylePanel");
    m_stylePanel->setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* layout = new QVBoxLayout(m_stylePanel);
    layout->setContentsMargins(15, 13, 15, 13);
    layout->setSpacing(8);

    layout->addWidget(makePanelTitle("画笔样式", m_stylePanel));

    QHBoxLayout* colorRow = new QHBoxLayout();
    colorRow->setSpacing(8);
    colorRow->addWidget(makePanelLabel("颜色", m_stylePanel));
    colorRow->addStretch(1);
    MyColorSwatch* swatch = new MyColorSwatch(m_stylePanel);
    swatch->setObjectName("canvasStyleColor");
    swatch->setColor(m_controller->penColor());
    connect(swatch, &MyColorSwatch::colorPicked, this, [this](const QColor& c) {
        m_controller->setPenColor(c);
    });
    colorRow->addWidget(swatch);
    layout->addLayout(colorRow);

    auto addSliderRow = [this, layout](const QString& label, int min, int max, int value,
                                       const QString& objectName,
                                       const std::function<void(int)>& sink) {
        QHBoxLayout* row = new QHBoxLayout();
        row->setSpacing(8);
        QLabel* text = makePanelLabel(label, m_stylePanel);
        text->setFixedWidth(46);
        row->addWidget(text);
        MySlider* slider = new MySlider(m_stylePanel);
        slider->setObjectName(objectName);
        slider->setRange(min, max);
        slider->setValue(value);
        slider->setFixedWidth(132);
        connect(slider, &MySlider::valueChanged, this, [sink](int v) { sink(v); });
        row->addWidget(slider, 1);
        layout->addLayout(row);
        return slider;
    };

    addSliderRow("粗细", 1, 16, static_cast<int>(m_controller->penWidth()),
                 "canvasStyleWidth", [this](int v) { m_controller->setPenWidth(v); });
    addSliderRow("不透明", 10, 100, static_cast<int>(m_controller->opacity() * 100),
                 "canvasStyleOpacity", [this](int v) { m_controller->setOpacity(v / 100.0); });
    addSliderRow("橡皮", 4, 60, static_cast<int>(m_controller->eraserRadius()),
                 "canvasStyleEraser", [this](int v) { m_controller->setEraserRadius(v); });

    layout->addWidget(new ToolDivider(m_stylePanel));
    layout->addWidget(makePanelTitle("文字样式", m_stylePanel));

    QHBoxLayout* textRow = new QHBoxLayout();
    textRow->setSpacing(4);
    MyButton* bold = new MyButton("B", m_stylePanel);
    bold->setObjectName("canvasTextBold");
    bold->setVariant(MyButton::Ghost);
    bold->setCompact(true);
    bold->setFixedWidth(36);
    connect(bold, &MyButton::clicked, this, [this]() { m_controller->toggleSelectedTextBold(); });
    textRow->addWidget(bold);

    MyButton* italic = new MyButton("I", m_stylePanel);
    italic->setObjectName("canvasTextItalic");
    italic->setVariant(MyButton::Ghost);
    italic->setCompact(true);
    italic->setFixedWidth(36);
    connect(italic, &MyButton::clicked, this, [this]() { m_controller->toggleSelectedTextItalic(); });
    textRow->addWidget(italic);

    MyButton* underline = new MyButton("U", m_stylePanel);
    underline->setObjectName("canvasTextUnderline");
    underline->setVariant(MyButton::Ghost);
    underline->setCompact(true);
    underline->setFixedWidth(36);
    connect(underline, &MyButton::clicked, this, [this]() { m_controller->toggleSelectedTextUnderline(); });
    textRow->addWidget(underline);

    MySlider* textSize = new MySlider(m_stylePanel);
    textSize->setObjectName("canvasTextSize");
    textSize->setRange(8, 48);
    textSize->setValue(12);
    textSize->setFixedWidth(84);
    connect(textSize, &MySlider::valueChanged, this, [this](int v) {
        m_controller->setSelectedTextSize(v);
    });
    textRow->addWidget(textSize, 1);
    layout->addLayout(textRow);

    connect(m_controller, &ToolController::selectionChanged, this, [this, bold, italic, underline](int count) {
        const bool hasText = m_controller->hasSelectedText();
        bold->setEnabled(hasText);
        italic->setEnabled(hasText);
        underline->setEnabled(hasText);
        Q_UNUSED(count);
    });
}

void CanvasToolBar::buildMorePanel() {
    m_morePanel = new ToolCard(m_host ? m_host : this);
    m_morePanel->setObjectName("canvasMorePanel");
    m_morePanel->setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* layout = new QVBoxLayout(m_morePanel);
    layout->setContentsMargins(15, 13, 15, 13);
    layout->setSpacing(8);

    layout->addWidget(makePanelTitle("编辑", m_morePanel));

    QHBoxLayout* clipRow = new QHBoxLayout();
    clipRow->setSpacing(4);

    MyButton* copyBtn = new MyButton("复制", m_morePanel);
    copyBtn->setObjectName("canvasCopyButton");
    copyBtn->setVariant(MyButton::Ghost);
    copyBtn->setCompact(true);
    connect(copyBtn, &MyButton::clicked, this, [this]() { m_controller->copySelection(); });
    clipRow->addWidget(copyBtn);

    MyButton* cutBtn = new MyButton("剪切", m_morePanel);
    cutBtn->setObjectName("canvasCutButton");
    cutBtn->setVariant(MyButton::Ghost);
    cutBtn->setCompact(true);
    connect(cutBtn, &MyButton::clicked, this, [this]() { m_controller->cutSelection(); });
    clipRow->addWidget(cutBtn);

    MyButton* pasteBtn = new MyButton("粘贴", m_morePanel);
    pasteBtn->setObjectName("canvasPasteButton");
    pasteBtn->setVariant(MyButton::Ghost);
    pasteBtn->setCompact(true);
    connect(pasteBtn, &MyButton::clicked, this, [this]() {
        m_controller->pasteAtCursor();
    });
    clipRow->addWidget(pasteBtn);

    MyButton* del = new MyButton("删除", m_morePanel);
    del->setObjectName("canvasDeleteButton");
    del->setVariant(MyButton::Ghost);
    del->setCompact(true);
    connect(del, &MyButton::clicked, this, [this]() { m_controller->deleteSelected(); });
    clipRow->addWidget(del);
    layout->addLayout(clipRow);

    layout->addWidget(new ToolDivider(m_morePanel));

    QHBoxLayout* actionRow = new QHBoxLayout();
    actionRow->setSpacing(4);

    m_btnUndo = new MyButton("撤销", m_morePanel);
    m_btnUndo->setObjectName("canvasUndoButton");
    m_btnUndo->setVariant(MyButton::Ghost);
    m_btnUndo->setCompact(true);
    connect(m_btnUndo, &MyButton::clicked, this, [this]() { m_controller->undo(); });
    actionRow->addWidget(m_btnUndo);

    m_btnRedo = new MyButton("重做", m_morePanel);
    m_btnRedo->setObjectName("canvasRedoButton");
    m_btnRedo->setVariant(MyButton::Ghost);
    m_btnRedo->setCompact(true);
    connect(m_btnRedo, &MyButton::clicked, this, [this]() { m_controller->redo(); });
    actionRow->addWidget(m_btnRedo);

    MyButton* selectAll = new MyButton("全选", m_morePanel);
    selectAll->setObjectName("canvasSelectAllButton");
    selectAll->setVariant(MyButton::Ghost);
    selectAll->setCompact(true);
    connect(selectAll, &MyButton::clicked, this, [this]() { m_controller->selectAll(); });
    actionRow->addWidget(selectAll);
    layout->addLayout(actionRow);

    layout->addWidget(new ToolDivider(m_morePanel));

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

    connect(m_controller, &ToolController::historyChanged, this, [this](bool canUndo, bool canRedo) {
        if (m_btnUndo) m_btnUndo->setEnabled(canUndo);
        if (m_btnRedo) m_btnRedo->setEnabled(canRedo);
    });
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

    auto addSliderRow = [this, layout, page](const QString& label, int min, int max, int value,
                                             const QString& objectName,
                                             const std::function<void(int)>& sink) {
        QHBoxLayout* row = new QHBoxLayout();
        row->setSpacing(8);
        QLabel* text = makePanelLabel(label, page);
        text->setFixedWidth(52);
        row->addWidget(text);
        MySlider* slider = new MySlider(page);
        slider->setObjectName(objectName);
        slider->setRange(min, max);
        slider->setValue(value);
        slider->setFixedWidth(132);
        connect(slider, &MySlider::valueChanged, this, [sink](int v) { sink(v); });
        row->addWidget(slider, 1);
        layout->addLayout(row);
    };

    switch (tool) {
    case ToolController::Pen:
    case ToolController::Highlighter:
    case ToolController::Line:
    case ToolController::Rect:
    case ToolController::Circle:
    case ToolController::Arrow:
    case ToolController::Triangle: {
        layout->addWidget(makePanelTitle(tool == ToolController::Highlighter
                                             ? "荧光笔样式" : "画笔样式", page));
        QHBoxLayout* colorRow = new QHBoxLayout();
        colorRow->setSpacing(8);
        colorRow->addWidget(makePanelLabel("颜色", page));
        colorRow->addStretch(1);
        MyColorSwatch* swatch = new MyColorSwatch(page);
        swatch->setObjectName("canvasToolColor");
        swatch->setColor(m_controller->penColor());
        connect(swatch, &MyColorSwatch::colorPicked, this, [this](const QColor& c) {
            m_controller->setPenColor(c);
        });
        colorRow->addWidget(swatch);
        layout->addLayout(colorRow);
        addSliderRow("粗细", 1, 16, static_cast<int>(m_controller->penWidth()),
                     "canvasToolWidth", [this](int v) { m_controller->setPenWidth(v); });
        addSliderRow("不透明", 10, 100, static_cast<int>(m_controller->opacity() * 100),
                     "canvasToolOpacity", [this](int v) { m_controller->setOpacity(v / 100.0); });
        break;
    }
    case ToolController::Eraser: {
        layout->addWidget(makePanelTitle("橡皮设置", page));
        addSliderRow("大小", 4, 60, static_cast<int>(m_controller->eraserRadius()),
                     "canvasToolEraser", [this](int v) { m_controller->setEraserRadius(v); });
        break;
    }
    case ToolController::Mosaic: {
        layout->addWidget(makePanelTitle("马赛克设置", page));
        addSliderRow("格子", 4, 48, m_controller->mosaicCellSize(),
                     "canvasToolMosaic", [this](int v) { m_controller->setMosaicCellSize(v); });
        break;
    }
    case ToolController::Table: {
        layout->addWidget(makePanelTitle("表格设置", page));
        addSliderRow("行数", 1, 12, m_tableRows,
                     "canvasToolTableRows", [this](int v) {
                         m_tableRows = v;
                         m_controller->setTableSize(v, m_tableCols);
                     });
        addSliderRow("列数", 1, 12, m_tableCols,
                     "canvasToolTableCols", [this](int v) {
                         m_tableCols = v;
                         m_controller->setTableSize(m_tableRows, v);
                     });
        break;
    }
    case ToolController::TextBox: {
        layout->addWidget(makePanelTitle("文本框设置", page));
        addSliderRow("字号", 8, 48, 12,
                     "canvasToolTextSize", [this](int v) {
                         m_controller->setSelectedTextSize(v);
                     });
        break;
    }
    case ToolController::Latex: {
        layout->addWidget(makePanelTitle("LaTeX 公式", page));
        QLabel* hint = makePanelLabel("点击画布插入公式后可直接编辑源码", page);
        hint->setWordWrap(true);
        layout->addWidget(hint);
        break;
    }
    case ToolController::Markdown: {
        layout->addWidget(makePanelTitle("Markdown 笔记", page));
        QLabel* hint = makePanelLabel("点击画布插入笔记后可直接编辑源码", page);
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

void CanvasToolBar::toggleStylePanel() {
    m_styleOpen = !m_styleOpen;
    if (m_moreOpen) {
        m_moreOpen = false;
        if (m_morePanel) m_morePanel->hide();
    }
    if (m_toolPanelOpen) {
        m_toolPanelOpen = false;
        if (m_toolPanel) m_toolPanel->hide();
    }
    if (m_stylePanel) {
        if (m_styleOpen) {
            m_stylePanel->show();
            m_stylePanel->raise();
        } else {
            m_stylePanel->hide();
        }
    }
    reposition();
}

void CanvasToolBar::toggleMorePanel() {
    m_moreOpen = !m_moreOpen;
    if (m_styleOpen) {
        m_styleOpen = false;
        if (m_stylePanel) m_stylePanel->hide();
    }
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
    if (m_styleOpen) {
        m_styleOpen = false;
        if (m_stylePanel) m_stylePanel->hide();
    }
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
    m_styleOpen = false;
    m_moreOpen = false;
    m_toolPanelOpen = false;
    if (m_stylePanel) m_stylePanel->hide();
    if (m_morePanel) m_morePanel->hide();
    if (m_toolPanel) m_toolPanel->hide();
    reposition();
}

void CanvasToolBar::setHostWidget(QWidget* host) {
    m_host = host;
    if (m_host) {
        m_host->installEventFilter(this);
        buildStylePanel();
        buildMorePanel();
        buildToolPanel();
        if (m_stylePanel) m_stylePanel->hide();
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

void CanvasToolBar::reposition() {
    if (!m_host) return;
    const int areaW = m_host->width();
    const int areaH = m_host->height();

    const int barW = sizeHint().width();
    const int barH = sizeHint().height();
    const int x = qMax(8, (areaW - barW) / 2);
    const int y = 16;
    setGeometry(x, y, barW, barH);

    if (m_styleOpen && m_stylePanel) placePanel(m_stylePanel, x, barW, y, barH);
    if (m_moreOpen && m_morePanel) placePanel(m_morePanel, x, barW, y, barH);
    if (m_toolPanelOpen && m_toolPanel) placePanel(m_toolPanel, x, barW, y, barH);
}

bool CanvasToolBar::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_host && event->type() == QEvent::Resize) {
        reposition();
        return false;
    }
    return QWidget::eventFilter(obj, event);
}
