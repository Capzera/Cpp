#include "MyTextInput.h"
#include "ui/UiTheme.h"
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QEvent>

MyTextInput::MyTextInput(QWidget* parent) : QWidget(parent) {
    setFixedHeight(28);
    setAttribute(Qt::WA_Hover, true);

    m_edit = new QLineEdit(this);
    m_edit->setFrame(false);
    m_edit->setObjectName("myTextInputLine");
    m_edit->setFont(UiTheme::uiFont(9));
    m_edit->setStyleSheet(
        "QLineEdit#myTextInputLine { background:transparent; border:none; color:#1f2733; padding:0px; }"
        "QLineEdit#myTextInputLine::selection { background:#e9f1fd; color:#1f2733; }");
    m_edit->setAttribute(Qt::WA_TranslucentBackground, true);
    m_edit->move(9, 0);
    m_edit->resize(width() - 18, height());
    m_edit->installEventFilter(this);

    connect(m_edit, &QLineEdit::textChanged, this, [this](const QString& text) {
        emit textChanged(text);
    });
    connect(m_edit, &QLineEdit::editingFinished, this, &MyTextInput::editingFinished);
    connect(m_edit, &QLineEdit::returnPressed, this, &MyTextInput::returnPressed);
}

QString MyTextInput::text() const {
    return m_edit ? m_edit->text() : QString();
}

void MyTextInput::setText(const QString& text) {
    if (m_edit) m_edit->setText(text);
}

void MyTextInput::setPlaceholderText(const QString& text) {
    if (m_placeholder == text) return;
    m_placeholder = text;
    if (m_edit) m_edit->setPlaceholderText(text);
    update();
}

void MyTextInput::setMaxLength(int n) {
    if (m_edit) m_edit->setMaxLength(n);
}

void MyTextInput::selectAll() {
    if (m_edit) m_edit->selectAll();
}

void MyTextInput::focusInEvent(QFocusEvent* event) {
    m_focused = true;
    if (m_edit) m_edit->setFocus();
    update();
    QWidget::focusInEvent(event);
}

void MyTextInput::focusOutEvent(QFocusEvent* event) {
    m_focused = false;
    update();
    QWidget::focusOutEvent(event);
}

void MyTextInput::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 7, 7);
    painter.setPen(QPen(m_focused ? UiTheme::borderFocus() : UiTheme::border(), m_focused ? 1.6 : 1.1));
    painter.setBrush(UiTheme::surface());
    painter.drawPath(path);
}
