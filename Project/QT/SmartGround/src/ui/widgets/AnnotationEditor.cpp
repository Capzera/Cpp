#include "AnnotationEditor.h"
#include "ui/UiTheme.h"
#include "canvas/LatexService.h"
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QTextDocument>
#include <QTextBrowser>
#include <QPushButton>
#include <QSvgRenderer>
#include <QTimer>

AnnotationEditor::AnnotationEditor(Mode mode, const QString& initial, QWidget* parent)
    : QDialog(parent), m_mode(mode) {
    setWindowTitle(mode == LatexMode ? "编辑 LaTeX 公式"
                                     : (mode == MarkdownMode ? "编辑 Markdown 笔记" : "编辑文本"));
    setModal(true);
    resize(620, mode == LatexMode ? 400 : 460);
    buildUi();
    m_editor->setPlainText(initial);
    applyStyle();

    connect(m_editor, &QPlainTextEdit::textChanged, this, [this]() {
        if (m_preview) {
            if (m_mode == MarkdownMode) {
                m_preview->setMarkdown(m_editor->toPlainText());
            } else if (m_mode == TextMode) {
                m_preview->setPlainText(m_editor->toPlainText());
            }
        }
        if (m_mode == LatexMode) refreshLatexPreview();
        if (m_live) emit contentChanged(m_editor->toPlainText());
    });

    if (m_mode == LatexMode) refreshLatexPreview();
}

void AnnotationEditor::setLiveMode(bool on) {
    m_live = on;
    setModal(!on);
    if (m_okButton) {
        m_okButton->setText(on ? "关闭" : "确定");
        if (on) {
            m_okButton->setDefault(false);
            connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
        }
    }
}

void AnnotationEditor::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 16);
    root->setSpacing(10);

    QLabel* title = new QLabel(this);
    title->setObjectName("editorTitle");
    title->setText(m_mode == LatexMode ? "输入 LaTeX 源码，右侧实时渲染公式"
                                       : (m_mode == MarkdownMode ? "支持 GitHub 风格 Markdown 渲染"
                                                                : "输入文本内容"));
    root->addWidget(title);

    auto* body = new QHBoxLayout();
    body->setSpacing(12);

    m_editor = new QPlainTextEdit(this);
    m_editor->setObjectName("editorInput");
    m_editor->setFont(UiTheme::monoFont(11));
    m_editor->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    m_editor->setMinimumWidth(280);
    body->addWidget(m_editor, 1);

    if (m_mode == LatexMode) {
        m_latexPreview = new QLabel(this);
        m_latexPreview->setObjectName("editorLatexPreview");
        m_latexPreview->setMinimumWidth(260);
        m_latexPreview->setAlignment(Qt::AlignCenter);
        m_latexPreview->setStyleSheet("background:#fbfcfe; border:1px solid #d5dde9; border-radius:6px;");
        body->addWidget(m_latexPreview, 1);

        m_latexRenderer = new QSvgRenderer();
        connect(LatexService::instance(), &LatexService::rendered, this,
                [this](const QString& tex, const QString& svg, double w, double h) {
                    if (tex != m_latexPending) return;
                    renderLatexPreview(tex, svg, w, h);
                });
    } else {
        m_preview = new QTextBrowser(this);
        m_preview->setObjectName("editorPreview");
        m_preview->setMinimumWidth(220);
        if (m_mode == MarkdownMode) {
            m_preview->setMarkdown(m_editor->toPlainText());
            applyMarkdownPreviewStyle();
        } else {
            m_preview->setFont(UiTheme::monoFont(11));
            m_preview->setPlainText(m_editor->toPlainText());
        }
        body->addWidget(m_preview, 1);
    }

    root->addLayout(body, 1);

    m_hint = new QLabel(this);
    m_hint->setObjectName("editorHint");
    m_hint->setText(m_mode == LatexMode
                        ? "示例：\\frac{a}{b} + \\sum_{i=1}^{n} x_i"
                        : (m_mode == MarkdownMode ? "示例：# 标题\\n- 列表\\n```cpp\\nint main() {}\\n```"
                                                  : "示例：双击画布上的文本框可再次编辑"));
    root->addWidget(m_hint);

    auto* actions = new QHBoxLayout();
    actions->setSpacing(8);
    actions->addStretch(1);

    auto* cancel = new QPushButton("取消", this);
    cancel->setObjectName("editorCancel");
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
    actions->addWidget(cancel);

    m_okButton = new QPushButton("确定", this);
    m_okButton->setObjectName("editorOk");
    m_okButton->setDefault(true);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    actions->addWidget(m_okButton);

    root->addLayout(actions);
}

void AnnotationEditor::refreshLatexPreview() {
    if (m_mode != LatexMode || !m_latexPreview) return;
    const QString tex = m_editor->toPlainText();
    m_latexPending = tex;
    if (tex.trimmed().isEmpty()) {
        m_latexPreview->setText("（输入 LaTeX 源码后此处实时渲染）");
        m_latexPreview->setPixmap(QPixmap());
        return;
    }
    m_latexPreview->setText("渲染中…");
    if (!LatexService::instance()->isAvailable()) {
        m_latexPreview->setText("LaTeX 渲染服务不可用\n请确认 tools/latex/serve.js 与 Node 环境");
        return;
    }
    LatexService::instance()->request(tex);
}

void AnnotationEditor::renderLatexPreview(const QString& tex, const QString& svg, double w, double h) {
    Q_UNUSED(tex);
    if (!m_latexPreview || !m_latexRenderer) return;
    if (svg.isEmpty() || w <= 0.0 || h <= 0.0) {
        m_latexPreview->setText("公式渲染失败");
        return;
    }
    m_latexRenderer->load(QByteArray::fromStdString(svg.toStdString()));
    if (!m_latexRenderer->isValid()) {
        m_latexPreview->setText("公式渲染失败");
        return;
    }
    const int maxW = qMax(80, m_latexPreview->width() - 24);
    const int maxH = qMax(60, m_latexPreview->height() - 24);
    qreal factor = qMin(static_cast<qreal>(maxW) / w, static_cast<qreal>(maxH) / h);
    factor = qMin(factor, 3.0);
    const QSize target(qMax(1, static_cast<int>(w * factor)), qMax(1, static_cast<int>(h * factor)));

    QPixmap pixmap(target);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    m_latexRenderer->render(&painter, QRectF(QPointF(0, 0), QSizeF(target)));
    painter.end();
    m_latexPreview->setPixmap(pixmap);
}

QString AnnotationEditor::content() const {
    return m_editor ? m_editor->toPlainText() : QString();
}

void AnnotationEditor::applyStyle() {
    setStyleSheet(QString(
        "QDialog { background:%1; }"
        "QLabel#editorTitle { font-family:'Microsoft YaHei UI'; font-size:12px; font-weight:600; color:%2; }"
        "QLabel#editorHint { font-family:'Microsoft YaHei UI'; font-size:10px; color:%3; }"
        "QPlainTextEdit#editorInput { background:%4; border:1px solid %5; border-radius:%6px;"
        " padding:10px; color:%2; selection-background-color:%7; }"
        "QPlainTextEdit#editorInput:focus { border:1px solid %8; }"
        "QLabel#editorPreview { background:%4; border:1px solid %5; border-radius:%6px; padding:10px; color:%2; }"
        "QTextBrowser#editorPreview { background:%4; border:1px solid %5; border-radius:%6px; color:%2; }"
        "QPushButton { font-family:'Microsoft YaHei UI'; font-size:11px; padding:7px 20px; border-radius:6px;"
        " border:1px solid %5; background:%4; color:%2; }"
        "QPushButton:hover { background:%9; }"
        "QPushButton#editorOk { background:%7; color:white; border:1px solid %7; }"
        "QPushButton#editorOk:hover { background:%10; }"
    ).arg(UiTheme::surface().name(),
          UiTheme::textPrimary().name(),
          UiTheme::textMuted().name(),
          UiTheme::surfaceAlt().name(),
          UiTheme::border().name(),
          QString::number(UiTheme::radiusMd()),
          UiTheme::accent().name(),
          UiTheme::borderFocus().name(),
          UiTheme::surfaceHover().name(),
          UiTheme::accentHover().name()));
}

void AnnotationEditor::applyMarkdownPreviewStyle() {
    if (!m_preview) return;
    m_preview->document()->setDefaultStyleSheet(
        "h1 { font-size:17px; font-weight:700; color:#1f2733; margin-top:2px; margin-bottom:6px; }"
        "h2 { font-size:15px; font-weight:700; color:#1f2733; margin-top:8px; margin-bottom:4px; }"
        "h3 { font-size:13px; font-weight:700; color:#2b3547; margin-top:6px; margin-bottom:3px; }"
        "p  { font-size:12px; color:#33405a; }"
        "li { font-size:12px; color:#33405a; }"
        "code { font-family:'Cascadia Mono','Consolas'; font-size:11px; background-color:#eef2f9; color:#b8465f; }"
        "pre { font-family:'Cascadia Mono','Consolas'; font-size:11px; background-color:#f4f7fc; color:#26344d;"
        " margin-top:4px; margin-bottom:4px; }"
        "blockquote { font-size:11px; color:#5a6b85; margin-top:4px; margin-bottom:4px; }"
        "table { font-size:11px; color:#33405a; }"
        "b, strong { font-weight:700; color:#1f2733; }"
        "em, i { font-style:italic; color:#45536b; }"
        "hr { color:#d8e0ec; height:1px; }");
}

void AnnotationEditor::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(UiTheme::surface());
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), UiTheme::radiusLg(), UiTheme::radiusLg());
}
