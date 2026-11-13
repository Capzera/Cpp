#include "MyTextEdit.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QTextDocument>
#include <QAbstractScrollArea>
#include <QEvent>

MyTextEdit::MyTextEdit(QWidget* parent) : QTextEdit(parent) {
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setViewportMargins(0, 0, 0, 0);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setFont(UiTheme::monoFont(10));
    setLineWrapMode(QTextEdit::NoWrap);
    setTabChangesFocus(true);
    viewport()->setAutoFillBackground(false);
    document()->setDocumentMargin(6);

    QPalette pal = viewport()->palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    pal.setColor(QPalette::Text, UiTheme::textPrimary());
    pal.setColor(QPalette::Highlight, UiTheme::accentSoft());
    pal.setColor(QPalette::HighlightedText, UiTheme::textPrimary());
    pal.setColor(QPalette::PlaceholderText, UiTheme::textMuted());
    viewport()->setPalette(pal);

    const QString scrollStyle =
        "QScrollBar { background:transparent; }"
        "QScrollBar::handle { background:rgba(140,152,172,110); border-radius:4px; }"
        "QScrollBar::handle:hover { background:rgba(118,131,154,165); }"
        "QScrollBar::add-line, QScrollBar::sub-line { width:0px; height:0px; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background:transparent; }";

    QScrollBar* v = new QScrollBar(Qt::Vertical, this);
    v->setStyleSheet(scrollStyle);
    v->setFixedWidth(11);
    setVerticalScrollBar(v);

    QScrollBar* h = new QScrollBar(Qt::Horizontal, this);
    h->setStyleSheet(scrollStyle);
    h->setFixedHeight(11);
    setHorizontalScrollBar(h);

    connect(this, &QTextEdit::textChanged, this, &MyTextEdit::onTextChanged);
}

void MyTextEdit::renderBackground() {
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(viewport()->rect(), UiTheme::radiusMd(), UiTheme::radiusMd());
    painter.setPen(Qt::NoPen);
    painter.setBrush(UiTheme::surface());
    painter.drawPath(path);
}

void MyTextEdit::renderBorder() {
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(viewport()->rect(), UiTheme::radiusMd(), UiTheme::radiusMd());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(m_focused ? UiTheme::borderFocus() : UiTheme::borderSoft(), m_focused ? 1.8 : 1.4));
    painter.drawPath(path);
}

void MyTextEdit::renderPlaceholder() {
    if (!toPlainText().isEmpty()) return;
    if (m_hint.isEmpty()) return;
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(UiTheme::textMuted());
    painter.setFont(font());
    QRectF area = QRectF(viewport()->geometry());
    area.translate(viewport()->pos());
    area = area.adjusted(7, 5, -7, -5);
    const QStringList lines = m_hint.split('\n');
    const qreal lineHeight = painter.fontMetrics().lineSpacing();
    for (int i = 0; i < lines.size(); ++i) {
        const qreal y = area.top() + lineHeight * i;
        if (y > area.bottom()) break;
        painter.drawText(QRectF(area.left(), y, area.width(), lineHeight),
                         Qt::AlignLeft | Qt::AlignVCenter, lines[i]);
    }
}

void MyTextEdit::setPlaceholderText(const QString& text) {
    if (m_hint == text) return;
    m_hint = text;
    update();
}

void MyTextEdit::paintEvent(QPaintEvent* event) {
    renderBackground();
    QTextEdit::paintEvent(event);
    renderPlaceholder();
    renderBorder();
}

void MyTextEdit::focusInEvent(QFocusEvent* event) {
    m_focused = true;
    update();
    QTextEdit::focusInEvent(event);
}

void MyTextEdit::focusOutEvent(QFocusEvent* event) {
    m_focused = false;
    update();
    QTextEdit::focusOutEvent(event);
}

void MyTextEdit::resizeEvent(QResizeEvent* event) {
    QTextEdit::resizeEvent(event);
    update();
}

void MyTextEdit::onTextChanged() {
    update();
}
