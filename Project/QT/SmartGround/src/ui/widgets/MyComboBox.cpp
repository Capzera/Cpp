#include "MyComboBox.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QGuiApplication>
#include <QEvent>
#include <QApplication>

ComboRow::ComboRow(const QString& text, QWidget* parent) : QWidget(parent), m_text(text) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(30);
    setFont(UiTheme::uiFont(10));
}

void ComboRow::setSelected(bool on) {
    if (m_selected == on) return;
    m_selected = on;
    update();
}

void ComboRow::setHovered(bool on) {
    if (m_hover == on) return;
    m_hover = on;
    update();
}

void ComboRow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (m_selected || m_hover) {
        QPainterPath path;
        path.addRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), UiTheme::radiusSm(), UiTheme::radiusSm());
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_selected ? UiTheme::accentSoft() : UiTheme::surfaceHover());
        painter.drawPath(path);
    }
    painter.setFont(font());
    painter.setPen(m_selected ? UiTheme::accent().darker(118) : UiTheme::textPrimary());
    painter.drawText(QRectF(10, 0, width() - 20, height()), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void ComboRow::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QWidget::enterEvent(event);
}

void ComboRow::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QWidget::leaveEvent(event);
}

void ComboRow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit picked();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

MyComboBox::MyComboBox(QWidget* parent) : QAbstractButton(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFont(UiTheme::uiFont(10));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(34);
}

MyComboBox::~MyComboBox() {
    if (qApp) qApp->removeEventFilter(this);
    if (m_popup) m_popup->close();
}

void MyComboBox::addItem(const QString& text) {
    m_items.push_back(text);
    if (m_current < 0) m_current = 0;
    updateGeometry();
    update();
}

void MyComboBox::addItems(const QStringList& texts) {
    for (const QString& t : texts) m_items.push_back(t);
    if (m_current < 0 && !m_items.isEmpty()) m_current = 0;
    updateGeometry();
    update();
}

void MyComboBox::clear() {
    closePopup();
    m_items.clear();
    m_current = -1;
    updateGeometry();
    update();
}

void MyComboBox::setCurrentIndex(int index) {
    if (index < -1 || index >= m_items.size()) return;
    if (m_current == index) {
        closePopup();
        return;
    }
    m_current = index;
    update();
    emit currentIndexChanged(index);
}

QString MyComboBox::currentText() const {
    if (m_current < 0 || m_current >= m_items.size()) return QString();
    return m_items[m_current];
}

QString MyComboBox::itemText(int index) const {
    if (index < 0 || index >= m_items.size()) return QString();
    return m_items[index];
}

QSize MyComboBox::sizeHint() const {
    return QSize(160, 34);
}

QSize MyComboBox::minimumSizeHint() const {
    return QSize(110, 34);
}

void MyComboBox::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF box = QRectF(this->rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, UiTheme::radiusMd(), UiTheme::radiusMd());

    painter.setPen(QPen(m_hover ? UiTheme::borderStrong() : UiTheme::border(), 1));
    painter.setBrush(hasFocus() ? UiTheme::surface() : UiTheme::surfaceAlt());
    painter.drawPath(path);

    painter.setPen(UiTheme::textPrimary());
    painter.setFont(font());
    QRectF textRect = box.adjusted(11, 0, -30, 0);
    QString text = currentText();
    if (text.isEmpty()) {
        painter.setPen(UiTheme::textMuted());
        text = "请选择...";
    }
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    painter.setPen(QPen(UiTheme::textSecondary(), 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPointF arrow(box.right() - 15, box.center().y());
    painter.drawLine(arrow + QPointF(-4, -2), arrow);
    painter.drawLine(arrow + QPointF(4, -2), arrow);
}

void MyComboBox::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QAbstractButton::enterEvent(event);
}

void MyComboBox::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void MyComboBox::rebuildRows() {
    if (!m_popupLayout) return;
    for (ComboRow* row : m_rows) {
        m_popupLayout->removeWidget(row);
        row->deleteLater();
    }
    m_rows.clear();
    for (int i = 0; i < m_items.size(); ++i) {
        ComboRow* row = new ComboRow(m_items[i], m_popupContent);
        row->setSelected(i == m_current);
        connect(row, &ComboRow::picked, this, [this, i]() {
            setCurrentIndex(i);
            closePopup();
        });
        m_popupLayout->addWidget(row);
        m_rows.push_back(row);
    }
}

void MyComboBox::openPopup() {
    if (m_popup) {
        m_popup->close();
        m_popup->deleteLater();
        m_popup = nullptr;
        m_popupLayout = nullptr;
    }

    QFrame* frame = new QFrame(nullptr, Qt::Popup | Qt::FramelessWindowHint);
    frame->setAttribute(Qt::WA_TranslucentBackground, true);
    frame->setAttribute(Qt::WA_ShowWithoutActivating, false);
    frame->setFrameShape(QFrame::NoFrame);
    frame->setStyleSheet("QFrame { background:transparent; border:none; }");

    QVBoxLayout* outer = new QVBoxLayout(frame);
    outer->setContentsMargins(10, 8, 10, 10);
    outer->setSpacing(0);

    QFrame* card = new ComboCard(frame);
    card->setObjectName("comboCard");
    card->setFrameShape(QFrame::NoFrame);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(4, 4, 4, 4);
    cardLayout->setSpacing(0);

    QWidget* content = new QWidget();
    content->setAttribute(Qt::WA_TranslucentBackground, true);
    content->setObjectName("comboContent");
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    m_popupLayout = contentLayout;
    m_popupContent = content;

    QScrollArea* scroll = new QScrollArea();
    scroll->setObjectName("comboScroll");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setStyleSheet(
        "QScrollArea#comboScroll { background:transparent; border:none; }"
        "QScrollArea#comboScroll > QWidget > QWidget { background:transparent; }"
        "QScrollBar:vertical { background:transparent; width:8px; margin:0px 2px 0px 0px; }"
        "QScrollBar::handle:vertical { background:rgba(150,162,182,120); border-radius:4px; min-height:24px; }"
        "QScrollBar::handle:vertical:hover { background:rgba(120,133,156,175); }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; width:0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; }"
    );
    scroll->setWidget(content);

    cardLayout->addWidget(scroll);
    outer->addWidget(card);

    scroll->setMinimumHeight(60);
    scroll->setMaximumHeight(268);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(24, 36, 58, 70));
    card->setGraphicsEffect(shadow);

    m_popup = frame;
    rebuildRows();

    int popupWidth = qMax(width(), 180);
    card->setFixedWidth(popupWidth);
    frame->setFixedWidth(popupWidth + 20);
    frame->adjustSize();

    QPoint origin = mapToGlobal(QPoint(0, height() + 4));
    int popupHeight = frame->sizeHint().height();
    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    if (origin.y() + popupHeight > screen.bottom()) {
        origin.setY(mapToGlobal(QPoint(0, 0)).y() - popupHeight - 4);
    }
    if (origin.x() + popupWidth + 20 > screen.right()) {
        origin.setX(screen.right() - popupWidth - 20);
    }
    frame->move(origin);

    connect(frame, &QWidget::destroyed, this, [this]() {
        m_popup = nullptr;
        m_popupLayout = nullptr;
        m_rows.clear();
        qApp->removeEventFilter(this);
    });
    frame->show();
    frame->raise();
    frame->activateWindow();

    qApp->installEventFilter(this);
}

bool MyComboBox::eventFilter(QObject* obj, QEvent* event) {
    if (m_popup && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        QWidget* w = qobject_cast<QWidget*>(obj);
        if (w != m_popup && !isAncestorOf(w)) {
            bool insidePopup = false;
            for (ComboRow* row : m_rows) {
                if (w == row || (w && row->isAncestorOf(w))) { insidePopup = true; break; }
            }
            if (!insidePopup) {
                closePopup();
                qApp->removeEventFilter(this);
                Q_UNUSED(me);
            }
        }
    }
    if (m_popup && event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Escape) {
            closePopup();
            qApp->removeEventFilter(this);
            return true;
        }
    }
    return QAbstractButton::eventFilter(obj, event);
}

void MyComboBox::closePopup() {
    if (!m_popup) return;
    QPointer<QWidget> popup = m_popup;
    m_popup = nullptr;
    m_popupLayout = nullptr;
    m_rows.clear();
    if (!popup) return;
    popup->close();
    popup->deleteLater();
}

void MyComboBox::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QAbstractButton::mousePressEvent(event);
        return;
    }
    if (m_popup && m_popup->isVisible()) {
        closePopup();
        event->accept();
        return;
    }
    openPopup();
    event->accept();
}

void MyComboBox::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Down) {
        setCurrentIndex(m_current + 1 < m_items.size() ? m_current + 1 : 0);
        return;
    }
    if (event->key() == Qt::Key_Up) {
        setCurrentIndex(m_current - 1 >= 0 ? m_current - 1 : m_items.size() - 1);
        return;
    }
    if (event->key() == Qt::Key_Escape) {
        closePopup();
        return;
    }
    QAbstractButton::keyPressEvent(event);
}
