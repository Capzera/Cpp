#include "MyComboBox.h"
#include "ui/UiTheme.h"
#include "ui/UiFonts.h"
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
#include <QRadialGradient>
#include <QEasingCurve>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QList>
#include <algorithm>

namespace {

ComboVisualStyle g_globalStyle = ComboVisualStyle::VueLight;
QList<MyComboBox*> g_instances;

constexpr int kOptionFontSize = 10;
constexpr int kHeaderFontSize = 10;
constexpr int kButtonFontSize = 11;

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

QColor mixAlpha(const QColor& c, int alpha) {
    return QColor(c.red(), c.green(), c.blue(), alpha);
}

}  // namespace

ComboOption::ComboOption(const QString& text, QWidget* parent) : QWidget(parent), m_text(text) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(40);
    setFont(UiFonts::forText(text, kOptionFontSize));
    m_accent = UiTheme::accent();
}

void ComboOption::setAccent(const QColor& color) {
    if (m_accent == color) return;
    m_accent = color;
    update();
}

void ComboOption::setVisualStyle(ComboVisualStyle style) {
    if (m_style == style) return;
    m_style = style;
    update();
}

void ComboOption::setHoverProgress(qreal v) {
    if (qFuzzyCompare(m_hoverProgress, v)) return;
    m_hoverProgress = v;
    update();
}

void ComboOption::setPressProgress(qreal v) {
    if (qFuzzyCompare(m_pressProgress, v)) return;
    m_pressProgress = v;
    update();
}

void ComboOption::syncAnimations() {
    const qreal hoverTarget = m_hover ? 1.0 : 0.0;
    const qreal pressTarget = m_pressed ? 1.0 : 0.0;

    if (!m_hoverAnim) {
        m_hoverAnim = new QPropertyAnimation(this, "hoverProgress", this);
        m_hoverAnim->setDuration(160);
        m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);
    }
    if (!qFuzzyCompare(m_hoverAnim->endValue().toReal(), hoverTarget)) {
        m_hoverAnim->stop();
        m_hoverAnim->setStartValue(m_hoverProgress);
        m_hoverAnim->setEndValue(hoverTarget);
        m_hoverAnim->start();
    }

    if (!m_pressAnim) {
        m_pressAnim = new QPropertyAnimation(this, "pressProgress", this);
        m_pressAnim->setDuration(110);
        m_pressAnim->setEasingCurve(QEasingCurve::OutQuad);
    }
    if (!qFuzzyCompare(m_pressAnim->endValue().toReal(), pressTarget)) {
        m_pressAnim->stop();
        m_pressAnim->setStartValue(m_pressProgress);
        m_pressAnim->setEndValue(pressTarget);
        m_pressAnim->start();
    }
}

void ComboOption::setSelected(bool on) {
    if (m_selected == on) return;
    m_selected = on;
    update();
}

void ComboOption::setTileMode(bool on) {
    if (m_tile == on) return;
    m_tile = on;
    setFixedHeight(on ? 40 : 32);
    update();
}

int ComboOption::preferredWidth() const {
    const QFont f = font();
    const QFontMetrics fm(f);
    return fm.horizontalAdvance(m_text) + 40;
}

void ComboOption::paintVueTile(QPainter& painter, const QRectF& box) {
    const QColor accent = m_accent;
    const qreal hp = m_hoverProgress;
    const qreal pp = m_pressProgress;

    const double radius = 10.0;
    QRectF r = box.adjusted(1.5, 1.5, -1.5, -1.5);
    const double sink = 1.0 * pp;
    r.translate(0, sink);

    QPainterPath path;
    path.addRoundedRect(r, radius, radius);

    QColor fill;
    if (m_selected) {
        fill = QColor(accent.red(), accent.green(), accent.blue(), 34);
    } else {
        fill = mixAlpha(accent, 0);
        fill.setAlpha(static_cast<int>(18 * hp));
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);
    painter.drawPath(path);

    if (m_selected) {
        painter.setPen(QPen(mixAlpha(accent, 120), 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }

    const double barH = r.height() - 16.0;
    const QRectF bar(r.left() + 3.0, r.top() + 8.0, 3.0, barH);
    if (m_selected) {
        QPainterPath barPath;
        barPath.addRoundedRect(bar, 1.5, 1.5);
        painter.setPen(Qt::NoPen);
        painter.setBrush(accent);
        painter.drawPath(barPath);
    } else if (hp > 0.02) {
        QPainterPath barPath;
        barPath.addRoundedRect(bar, 1.5, 1.5);
        painter.setPen(Qt::NoPen);
        painter.setBrush(mixAlpha(accent, static_cast<int>(150 * hp)));
        painter.drawPath(barPath);
    }

    QColor textColor;
    if (m_selected) {
        textColor = accent.darker(126);
    } else {
        textColor = mixColor(UiTheme::textPrimary(), accent.darker(120), hp * 0.75);
    }
    painter.setFont(font());
    painter.setPen(textColor);
    QRectF textRect = r.adjusted(14.0, 0, -10.0, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_text);
}

void ComboOption::paintTile(QPainter& painter, const QRectF& box) {
    paintVueTile(painter, box);
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
    syncAnimations();
    update();
    QWidget::enterEvent(event);
}

void ComboOption::leaveEvent(QEvent* event) {
    m_hover = false;
    m_pressed = false;
    syncAnimations();
    update();
    QWidget::leaveEvent(event);
}

void ComboOption::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        syncAnimations();
        update();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void ComboOption::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        const bool wasPressed = m_pressed;
        m_pressed = false;
        syncAnimations();
        update();
        if (wasPressed && rect().contains(event->pos())) {
            emit picked();
        }
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ComboOption::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}

ComboSectionHeader::ComboSectionHeader(const QString& text, const QColor& accent, QWidget* parent)
    : QWidget(parent), m_text(text), m_accent(accent) {
    setFixedHeight(32);
    setFont(UiFonts::forText(text, kHeaderFontSize, true));
}

void ComboSectionHeader::setVisualStyle(ComboVisualStyle style) {
    if (m_style == style) return;
    m_style = style;
    update();
}

void ComboSectionHeader::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect());

    const double barW = 4.0;
    const double barH = 15.0;
    const QRectF bar(box.left() + 10.0, box.center().y() - barH / 2.0, barW, barH);
    QPainterPath barPath;
    barPath.addRoundedRect(bar, 2.0, 2.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_accent);
    painter.drawPath(barPath);

    QFontMetrics fm(font());
    const double textW = fm.horizontalAdvance(m_text);
    QColor titleColor = m_accent.darker(150);
    if (titleColor.lightnessF() > 0.40) titleColor = UiTheme::textPrimary();
    painter.setFont(font());
    painter.setPen(titleColor);
    painter.drawText(QRectF(bar.right() + 9.0, box.top(), textW + 4.0, box.height()),
                     Qt::AlignVCenter | Qt::AlignLeft, m_text);

    const double lineY = box.center().y();
    const double tailStart = bar.right() + 15.0 + textW;
    const double tailEnd = box.right() - 14.0;
    if (tailEnd > tailStart) {
        QLinearGradient tg(tailStart, lineY, tailEnd, lineY);
        tg.setColorAt(0.0, QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 46));
        tg.setColorAt(1.0, QColor(m_accent.red(), m_accent.green(), m_accent.blue(), 6));
        painter.setPen(Qt::NoPen);
        painter.setBrush(tg);
        painter.drawRect(QRectF(tailStart, lineY, tailEnd - tailStart, 1.0));
    }
}

ComboCard::ComboCard(QWidget* parent) : QFrame(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void ComboCard::setVisualStyle(ComboVisualStyle style) {
    if (m_style == style) return;
    m_style = style;
    update();
}

void ComboCard::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);

    QPainterPath path;
    path.addRoundedRect(box, 16.0, 16.0);
    QLinearGradient grad(box.topLeft(), box.bottomLeft());
    grad.setColorAt(0.0, QColor(255, 255, 255, 255));
    grad.setColorAt(1.0, QColor(249, 251, 254, 255));
    painter.setBrush(grad);
    painter.setPen(QPen(QColor(226, 232, 242), 1.0));
    painter.drawPath(path);
}

ComboPopupFrame::ComboPopupFrame(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_StyledBackground, false);
    setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);
}

void ComboPopupFrame::setVisualStyle(ComboVisualStyle style) {
    if (m_style == style) return;
    m_style = style;
    update();
}

void ComboPopupFrame::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    if (!m_card) return;
    const QRectF cardRect(m_card->geometry());
    const double radius = 20.0;
    const int layers = 14;
    const double maxSpreadX = 11.0;
    const double maxSpreadY = 6.0;
    for (int i = layers; i >= 1; --i) {
        const double t = static_cast<double>(i) / layers;
        const double spread = maxSpreadX * t;
        const double spreadY = maxSpreadY * t;
        const QRectF r = cardRect.adjusted(-spread, -spreadY, spread, spreadY);
        const int alpha = static_cast<int>(4.0 * (1.0 - t) + 1.0);
        if (alpha <= 0) continue;
        painter.setBrush(QColor(24, 40, 68, alpha));
        painter.drawRoundedRect(r, radius + spread * 0.5, radius + spread * 0.5);
    }
}
MyComboBox::MyComboBox(QWidget* parent) : QAbstractButton(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFont(UiFonts::forText("请选择...", kButtonFontSize));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(34);
    m_style = g_globalStyle;
    g_instances.append(this);
}

MyComboBox::~MyComboBox() {
    g_instances.removeAll(this);
    if (qApp) qApp->removeEventFilter(this);
    if (m_popup) m_popup->close();
}

void MyComboBox::setGlobalVisualStyle(ComboVisualStyle style) {
    g_globalStyle = style;
    for (MyComboBox* combo : g_instances) {
        if (combo && combo->m_style != style) {
            combo->m_style = style;
            combo->update();
        }
    }
}

ComboVisualStyle MyComboBox::globalVisualStyle() {
    return g_globalStyle;
}

QStringList MyComboBox::styleNames() {
    return {"Vue 扁平"};
}

QString MyComboBox::styleName(ComboVisualStyle style) {
    const QStringList names = styleNames();
    const int idx = styleIndex(style);
    return idx >= 0 && idx < names.size() ? names[idx] : names.first();
}

int MyComboBox::styleIndex(ComboVisualStyle style) {
    Q_UNUSED(style);
    return 0;
}

void MyComboBox::setVisualStyle(ComboVisualStyle style) {
    if (m_style == style) return;
    m_style = style;
    update();
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

void MyComboBox::setChevronProgress(qreal v) {
    if (qFuzzyCompare(m_chevronProgress, v)) return;
    m_chevronProgress = v;
    update();
}

void MyComboBox::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(this->rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    const double radius = 10.0;
    path.addRoundedRect(box, radius, radius);

    QLinearGradient grad(box.topLeft(), box.bottomLeft());
    if (hasFocus()) {
        grad.setColorAt(0.0, QColor(255, 255, 255));
        grad.setColorAt(1.0, QColor(250, 252, 255));
    } else {
        grad.setColorAt(0.0, m_hover ? QColor(255, 255, 255) : QColor(252, 253, 255));
        grad.setColorAt(1.0, m_hover ? QColor(244, 248, 253) : QColor(248, 250, 253));
    }
    painter.setBrush(grad);
    QColor border = m_hover || hasFocus() ? UiTheme::borderFocus() : UiTheme::border();
    painter.setPen(QPen(border, 1));
    painter.drawPath(path);

    QString text = currentText();
    QColor textColor = UiTheme::textPrimary();
    if (text.isEmpty()) {
        textColor = UiTheme::textMuted();
        text = "请选择...";
    }
    painter.setPen(textColor);
    painter.setFont(UiFonts::forText(text, kButtonFontSize));
    QRectF textRect = box.adjusted(12, 0, -34, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    const QPointF arrow(box.right() - 16, box.center().y());
    const bool open = isPopupOpen();
    QColor arrowColor = m_hover || open ? UiTheme::accent() : UiTheme::textSecondary();
    painter.setPen(QPen(arrowColor, 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    const double shift = m_chevronProgress * 1.6;
    const double dir = m_chevronProgress > 0.5 ? -1.0 : 1.0;
    painter.drawLine(arrow + QPointF(-4.5, -2.0 + shift * dir), arrow);
    painter.drawLine(arrow + QPointF(4.5, -2.0 + shift * dir), arrow);
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

void MyComboBox::computeColumnWidth() {
    m_columnWidth = 140;
    if (m_groups.isEmpty()) return;
    const int columns = 3;
    std::vector<int> widest(columns, 0);
    for (const Group& group : m_groups) {
        int cell = 0;
        for (int index : group.indices) {
            if (index < 0 || index >= m_items.size()) { ++cell; continue; }
            const QFont f = UiFonts::forText(m_items[index], kOptionFontSize);
            const QFontMetrics fm(f);
            const int need = fm.horizontalAdvance(m_items[index]) + 40;
            const int col = cell % columns;
            if (need > widest[col]) widest[col] = need;
            ++cell;
        }
    }
    int total = 0;
    for (int w : widest) total += w;
    if (total <= 0) return;
    m_columnWidth = widest.empty() ? 140 : widest[0];
    for (int col = 1; col < columns; ++col) {
        if (widest[col] > m_columnWidth) m_columnWidth = widest[col];
    }
    m_columnWidth = qBound(140, m_columnWidth, 300);
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
            option->setVisualStyle(m_style);
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
        const QColor accent = sectionAccent(g);
        ComboSectionHeader* header = new ComboSectionHeader(group.name, accent, m_popupContent);
        header->setVisualStyle(m_style);
        m_popupLayout->addWidget(header);
        m_popupWidgets.push_back(header);

        QGridLayout* grid = new QGridLayout();
        grid->setContentsMargins(10, 2, 10, 10);
        grid->setHorizontalSpacing(6);
        grid->setVerticalSpacing(6);
        const int columns = 3;
        int cell = 0;
        for (int index : group.indices) {
            ComboOption* option = new ComboOption(m_items[index], m_popupContent);
            option->setTileMode(true);
            option->setVisualStyle(m_style);
            option->setAccent(accent);
            option->setSelected(index == m_current);
            option->setMinimumWidth(option->preferredWidth());
            connect(option, &ComboOption::picked, this, [this, index]() {
                setCurrentIndex(index);
                closePopup();
            });
            grid->addWidget(option, cell / columns, cell % columns);
            ++cell;
        }
        for (int col = 0; col < columns; ++col) grid->setColumnMinimumWidth(col, m_columnWidth);
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
    frame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    frame->setVisualStyle(m_style);

    QVBoxLayout* outer = new QVBoxLayout(frame);
    outer->setContentsMargins(18, 12, 18, 14);
    outer->setSpacing(0);

    ComboCard* card = new ComboCard(frame);
    card->setVisualStyle(m_style);
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
        "QScrollBar::handle:vertical { background:rgba(150,162,182,110); border-radius:4px; min-height:24px; border:none; }"
        "QScrollBar::handle:vertical:hover { background:rgba(120,133,156,160); }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; width:0px; border:none; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; border:none; }"
    );
    scroll->setWidget(content);

    cardLayout->addWidget(scroll);
    outer->addWidget(card);

    scroll->setMinimumHeight(60);
    scroll->setMaximumHeight(430);

    m_popup = frame;
    computeColumnWidth();
    rebuildRows();

    const int popupWidth = m_groups.isEmpty() ? qMax(width(), 200)
                                              : qMax(width(), m_columnWidth * 3 + 44);
    card->setFixedWidth(popupWidth);
    frame->setFixedWidth(popupWidth + 36);
    frame->adjustSize();

    QPoint origin = mapToGlobal(QPoint(0, height() + 6));
    const int popupHeight = frame->sizeHint().height();
    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    if (origin.y() + popupHeight > screen.bottom()) {
        origin.setY(mapToGlobal(QPoint(0, 0)).y() - popupHeight - 6);
    }
    if (origin.x() + popupWidth + 36 > screen.right()) {
        origin.setX(screen.right() - popupWidth - 36);
    }
    if (origin.x() < screen.left()) origin.setX(screen.left());
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

    QPropertyAnimation* fade = new QPropertyAnimation(frame, "windowOpacity", frame);
    fade->setDuration(140);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->setEasingCurve(QEasingCurve::OutCubic);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    if (!m_chevronAnim) {
        m_chevronAnim = new QPropertyAnimation(this, "chevronProgress", this);
        m_chevronAnim->setDuration(180);
        m_chevronAnim->setEasingCurve(QEasingCurve::OutCubic);
    }
    m_chevronAnim->stop();
    m_chevronAnim->setStartValue(m_chevronProgress);
    m_chevronAnim->setEndValue(1.0);
    m_chevronAnim->start();

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
    if (m_chevronAnim) {
        m_chevronAnim->stop();
        m_chevronAnim->setStartValue(m_chevronProgress);
        m_chevronAnim->setEndValue(0.0);
        m_chevronAnim->start();
    }
    if (!popup) return;
    QPropertyAnimation* fade = new QPropertyAnimation(popup, "windowOpacity", popup);
    fade->setDuration(110);
    fade->setStartValue(popup->windowOpacity());
    fade->setEndValue(0.0);
    fade->setEasingCurve(QEasingCurve::InCubic);
    fade->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(130, popup, [popup]() {
        if (!popup) return;
        popup->close();
        popup->deleteLater();
    });
    update();
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
