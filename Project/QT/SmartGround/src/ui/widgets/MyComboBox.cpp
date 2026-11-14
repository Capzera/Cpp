#include "MyComboBox.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QScreen>
#include <QGuiApplication>
#include <QEvent>
#include <QApplication>
#include <QFontMetrics>
#include <QLinearGradient>
#include <algorithm>

namespace {

QColor mixColor(const QColor& a, const QColor& b, qreal t) {
    return QColor::fromRgbF(a.redF() + (b.redF() - a.redF()) * t,
                            a.greenF() + (b.greenF() - a.greenF()) * t,
                            a.blueF() + (b.blueF() - a.blueF()) * t);
}

QColor sectionAccent(int index) {
    static const QColor palette[] = {
        QColor(59, 125, 221), QColor(0, 150, 136), QColor(123, 97, 255),
        QColor(219, 112, 47), QColor(214, 61, 61), QColor(38, 162, 105),
        QColor(196, 106, 178)
    };
    return palette[index % 7];
}

}  // namespace

ComboOption::ComboOption(const QString& text, QWidget* parent) : QWidget(parent), m_text(text) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(32);
    setFont(UiTheme::uiFont(9));
}

void ComboOption::setSelected(bool on) {
    if (m_selected == on) return;
    m_selected = on;
    update();
}

void ComboOption::setTileMode(bool on) {
    if (m_tile == on) return;
    m_tile = on;
    setFixedHeight(on ? 34 : 30);
    update();
}

void ComboOption::paintTile(QPainter& painter, const QRectF& box) {
    const QColor accent = UiTheme::accent();
    QPainterPath path;
    path.addRoundedRect(box, 8.0, 8.0);

    QLinearGradient grad(box.topLeft(), box.bottomLeft());
    if (m_selected) {
        grad.setColorAt(0.0, accent.lighter(135));
        grad.setColorAt(1.0, accent.lighter(112));
    } else if (m_hover) {
        grad.setColorAt(0.0, QColor(255, 255, 255));
        grad.setColorAt(1.0, QColor(228, 238, 254));
    } else {
        grad.setColorAt(0.0, QColor(249, 251, 255));
        grad.setColorAt(1.0, QColor(236, 242, 251));
    }
    painter.setBrush(grad);
    if (m_selected) {
        painter.setPen(QPen(accent, 1.5));
    } else if (m_hover) {
        painter.setPen(QPen(accent.lighter(140), 1.4));
    } else {
        painter.setPen(QPen(QColor(216, 225, 239), 1));
    }
    painter.drawPath(path);

    if (m_hover && !m_selected) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(accent.red(), accent.green(), accent.blue(), 52));
        painter.drawRoundedRect(QRectF(box.left() + 1.0, box.top() + 4.0, 2.5, box.height() - 8.0), 1.2, 1.2);
    }

    if (m_selected) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255, 120));
        painter.drawRoundedRect(QRectF(box.left() + 1.0, box.top() + 1.0, box.width() - 2.0, 2.0), 1.0, 1.0);
    }

    painter.setFont(font());
    painter.setPen(m_selected ? accent.darker(145) : UiTheme::textSecondary());
    painter.drawText(box.adjusted(6, 0, -6, 0), Qt::AlignVCenter | Qt::AlignHCenter, m_text);
}

void ComboOption::paintRow(QPainter& painter, const QRectF& box) {
    if (m_selected || m_hover) {
        QPainterPath path;
        path.addRoundedRect(box, UiTheme::radiusSm(), UiTheme::radiusSm());
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_selected ? UiTheme::accentSoft() : UiTheme::surfaceHover());
        painter.drawPath(path);
    }
    painter.setFont(font());
    painter.setPen(m_selected ? UiTheme::accent().darker(118) : UiTheme::textPrimary());
    painter.drawText(box.adjusted(10, 0, -10, 0), Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void ComboOption::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    if (m_tile) paintTile(painter, box);
    else paintRow(painter, box);
}

void ComboOption::enterEvent(QEnterEvent* event) {
    m_hover = true;
    update();
    QWidget::enterEvent(event);
}

void ComboOption::leaveEvent(QEvent* event) {
    m_hover = false;
    update();
    QWidget::leaveEvent(event);
}

void ComboOption::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit picked();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

ComboSectionHeader::ComboSectionHeader(const QString& text, const QColor& accent, QWidget* parent)
    : QWidget(parent), m_text(text), m_accent(accent) {
    setFixedHeight(34);
    setFont(UiTheme::uiFont(12, true));
}

void ComboSectionHeader::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect());

    const double barW = 4.0;
    const double barH = 17.0;
    QRectF bar(box.left() + 12.0, box.center().y() - barH / 2.0, barW, barH);
    QPainterPath barPath;
    barPath.addRoundedRect(bar, 2.0, 2.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_accent);
    painter.drawPath(barPath);

    QFontMetrics fm(font());
    const double textW = fm.horizontalAdvance(m_text);
    const double textH = fm.height();

    QColor titleColor = m_accent.darker(155);
    if (titleColor.lightnessF() > 0.42) titleColor = UiTheme::textPrimary();
    painter.setFont(font());
    painter.setPen(titleColor);
    painter.drawText(QRectF(bar.right() + 9.0, box.top(), textW + 4.0, box.height()),
                     Qt::AlignVCenter | Qt::AlignLeft, m_text);

    QPainterPath tail;
    const double lineY = box.center().y() + 1.0;
    const double tailStart = bar.right() + 15.0 + textW;
    const double tailEnd = box.right() - 14.0;
    if (tailEnd > tailStart) {
        tail.addRoundedRect(QRectF(tailStart, lineY, tailEnd - tailStart, 1.4), 0.7, 0.7);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 46));
        painter.drawPath(tail);
    }
    Q_UNUSED(textH);
}

ComboCard::ComboCard(QWidget* parent) : QFrame(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void ComboCard::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, 12.0, 12.0);
    QLinearGradient grad(box.topLeft(), box.bottomLeft());
    grad.setColorAt(0.0, QColor(255, 255, 255, 250));
    grad.setColorAt(1.0, QColor(240, 244, 252, 250));
    painter.setBrush(grad);
    painter.setPen(QPen(QColor(214, 222, 236, 200), 1));
    painter.drawPath(path);
}

ComboPopupFrame::ComboPopupFrame(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void ComboPopupFrame::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    if (!m_card) return;
    const QRectF cardRect(m_card->geometry());
    const qreal radius = 12.0;
    const qreal offsetY = 4.0;
    const int rings = 12;
    const qreal step = 1.35;
    for (int i = rings; i >= 1; --i) {
        const qreal spread = i * step;
        QPainterPath outer;
        outer.addRoundedRect(cardRect.adjusted(-spread, -spread + offsetY, spread, spread + offsetY),
                             radius + spread, radius + spread);
        const qreal innerSpread = spread - step;
        QPainterPath inner;
        inner.addRoundedRect(cardRect.adjusted(-innerSpread, -innerSpread + offsetY, innerSpread, innerSpread + offsetY),
                             radius + innerSpread, radius + innerSpread);
        outer.setFillRule(Qt::OddEvenFill);
        outer.addPath(inner);
        painter.setBrush(QColor(24, 38, 64, 2 + i));
        painter.drawPath(outer);
    }
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
    m_indexByText.insert(text, m_items.size() - 1);
    if (m_current < 0) m_current = 0;
    updateGeometry();
    update();
}

void MyComboBox::addItems(const QStringList& texts) {
    for (const QString& t : texts) {
        m_items.push_back(t);
        m_indexByText.insert(t, m_items.size() - 1);
    }
    if (m_current < 0 && !m_items.isEmpty()) m_current = 0;
    updateGeometry();
    update();
}

void MyComboBox::addGroup(const QString& groupName, const QStringList& texts) {
    if (texts.isEmpty()) return;
    Group group;
    group.name = groupName;
    for (const QString& t : texts) {
        m_items.push_back(t);
        m_indexByText.insert(t, m_items.size() - 1);
        group.indices.push_back(m_items.size() - 1);
        if (m_current < 0) m_current = m_items.size() - 1;
    }
    m_groups.push_back(group);
    updateGeometry();
    update();
}

QString MyComboBox::groupOf(int index) const {
    for (const Group& g : m_groups) {
        if (g.indices.contains(index)) return g.name;
    }
    return QString();
}

QStringList MyComboBox::groupNames() const {
    QStringList names;
    for (const Group& g : m_groups) names << g.name;
    return names;
}

QStringList MyComboBox::groupItems(int groupIndex) const {
    QStringList out;
    if (groupIndex < 0 || groupIndex >= m_groups.size()) return out;
    for (int idx : m_groups[groupIndex].indices) out << m_items[idx];
    return out;
}

void MyComboBox::clear() {
    closePopup();
    m_items.clear();
    m_groups.clear();
    m_indexByText.clear();
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
    const QRectF box = QRectF(this->rect()).adjusted(0.5, 0.5, -0.5, -0.5);
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
    for (ComboOption* option : m_options) {
        m_popupLayout->removeWidget(option);
        option->deleteLater();
    }
    m_options.clear();
    for (QWidget* w : m_popupWidgets) {
        m_popupLayout->removeWidget(w);
        w->deleteLater();
    }
    m_popupWidgets.clear();

    if (m_groups.isEmpty()) {
        for (int i = 0; i < m_items.size(); ++i) {
            ComboOption* option = new ComboOption(m_items[i], m_popupContent);
            option->setSelected(i == m_current);
            connect(option, &ComboOption::picked, this, [this, i]() {
                setCurrentIndex(i);
                closePopup();
            });
            m_popupLayout->addWidget(option);
            m_options.push_back(option);
        }
        return;
    }

    for (int g = 0; g < m_groups.size(); ++g) {
        const Group& group = m_groups[g];
        ComboSectionHeader* header = new ComboSectionHeader(group.name, sectionAccent(g), m_popupContent);
        m_popupLayout->addWidget(header);
        m_popupWidgets.push_back(header);

        QGridLayout* grid = new QGridLayout();
        grid->setContentsMargins(12, 2, 12, 10);
        grid->setHorizontalSpacing(8);
        grid->setVerticalSpacing(8);
        const int columns = 3;
        int cell = 0;
        for (int index : group.indices) {
            ComboOption* option = new ComboOption(m_items[index], m_popupContent);
            option->setTileMode(true);
            option->setSelected(index == m_current);
            connect(option, &ComboOption::picked, this, [this, index]() {
                setCurrentIndex(index);
                closePopup();
            });
            grid->addWidget(option, cell / columns, cell % columns);
            ++cell;
        }
        m_popupLayout->addLayout(grid);
    }
}

void MyComboBox::openPopup() {
    if (m_popup) {
        m_popup->close();
        m_popup->deleteLater();
        m_popup = nullptr;
        m_popupLayout = nullptr;
    }

    ComboPopupFrame* frame = new ComboPopupFrame(nullptr);
    frame->setAttribute(Qt::WA_ShowWithoutActivating, false);
    frame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    QVBoxLayout* outer = new QVBoxLayout(frame);
    outer->setContentsMargins(18, 18, 18, 22);
    outer->setSpacing(0);

    ComboCard* card = new ComboCard(frame);
    frame->setCardWidget(card);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(4, 4, 4, 4);
    cardLayout->setSpacing(0);

    QWidget* content = new QWidget();
    content->setAttribute(Qt::WA_TranslucentBackground, true);
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
    scroll->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);
    scroll->setStyleSheet(
        "QScrollArea#comboScroll { background:transparent; border:none; }"
        "QScrollArea#comboScroll > QWidget > QWidget { background:transparent; border:none; }"
        "QScrollBar:vertical { background:transparent; width:8px; margin:0px 2px 0px 0px; border:none; }"
        "QScrollBar::handle:vertical { background:rgba(150,162,182,120); border-radius:4px; min-height:24px; border:none; }"
        "QScrollBar::handle:vertical:hover { background:rgba(120,133,156,175); }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; width:0px; border:none; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; border:none; }"
    );
    scroll->setWidget(content);

    cardLayout->addWidget(scroll);
    outer->addWidget(card);

    scroll->setMinimumHeight(60);
    scroll->setMaximumHeight(430);

    m_popup = frame;
    rebuildRows();

    const int popupWidth = m_groups.isEmpty() ? qMax(width(), 200) : qMax(width(), 540);
    card->setFixedWidth(popupWidth);
    frame->setFixedWidth(popupWidth + 36);
    frame->adjustSize();

    QPoint origin = mapToGlobal(QPoint(0, height() + 4));
    const int popupHeight = frame->sizeHint().height();
    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    if (origin.y() + popupHeight > screen.bottom()) {
        origin.setY(mapToGlobal(QPoint(0, 0)).y() - popupHeight - 4);
    }
    if (origin.x() + popupWidth + 36 > screen.right()) {
        origin.setX(screen.right() - popupWidth - 36);
    }
    frame->move(origin);

    connect(frame, &QWidget::destroyed, this, [this]() {
        m_popup = nullptr;
        m_popupLayout = nullptr;
        m_options.clear();
        qApp->removeEventFilter(this);
    });
    frame->show();
    frame->raise();
    frame->activateWindow();

    qApp->installEventFilter(this);
}

bool MyComboBox::isPopupChild(QObject* obj) const {
    QWidget* w = qobject_cast<QWidget*>(obj);
    if (!w) return false;
    if (w == m_popup) return true;
    if (m_popup && m_popup->isAncestorOf(w)) return true;
    for (ComboOption* option : m_options) {
        if (w == option || option->isAncestorOf(w)) return true;
    }
    for (QWidget* child : m_popupWidgets) {
        if (w == child || child->isAncestorOf(w)) return true;
    }
    return false;
}

bool MyComboBox::eventFilter(QObject* obj, QEvent* event) {
    if (m_popup && event->type() == QEvent::MouseButtonPress) {
        QWidget* w = qobject_cast<QWidget*>(obj);
        if (w != m_popup && !isAncestorOf(w) && !isPopupChild(w)) {
            closePopup();
            qApp->removeEventFilter(this);
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
    m_options.clear();
    m_popupWidgets.clear();
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
