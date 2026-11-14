#include "AnnotationEditor.h"
#include "ui/UiTheme.h"
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QTextDocument>
#include <QTextBrowser>
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>

AnnotationEditor::AnnotationEditor(Mode mode, const QString& initial, QWidget* parent)
    : QDialog(parent), m_mode(mode) {
    setWindowTitle(mode == LatexMode ? "编辑 LaTeX 公式"
                                     : (mode == MarkdownMode ? "编辑 Markdown 笔记" : "编辑文本"));
    setModal(true);
    resize(560, mode == LatexMode ? 340 : 460);
    buildUi();
    m_editor->setPlainText(initial);
    applyStyle();
    connect(m_editor, &QPlainTextEdit::textChanged, this, [this]() {
        if (!m_preview) return;
        if (m_mode == MarkdownMode) {
            m_preview->setMarkdown(m_editor->toPlainText());
        } else if (m_mode == LatexMode) {
            m_preview->setPlainText(m_editor->toPlainText());
        }
    });
}

void AnnotationEditor::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 16);
    root->setSpacing(10);

    QLabel* title = new QLabel(this);
    title->setObjectName("editorTitle");
    title->setText(m_mode == LatexMode ? "输入 LaTeX 源码，右侧实时预览"
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

    if (m_mode != TextMode) {
        m_preview = new QTextBrowser(this);
        m_preview->setObjectName("editorPreview");
        m_preview->setMinimumWidth(220);
        if (m_mode == MarkdownMode) {
            m_preview->setMarkdown(m_editor->toPlainText());
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
        "QPushButton { font-family:'Microsoft YaHei UI'; font-size:11px; padding:7px 20px; border-radius:%6px;"
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

void AnnotationEditor::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(UiTheme::surface());
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), UiTheme::radiusLg(), UiTheme::radiusLg());
}
