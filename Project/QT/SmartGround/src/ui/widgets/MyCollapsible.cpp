#include "MyCollapsible.h"
#include "ui/UiTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QEasingCurve>

MyCollapsible::MyCollapsible(QWidget* parent) : QWidget(parent), m_accent(UiTheme::accent()) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_anim = new QPropertyAnimation(this, "ratio", this);
    m_anim->setDuration(m_duration);
    m_anim->setEasingCurve(QEasingCurve::InOutCubic);
    connect(m_anim, &QPropertyAnimation::valueChanged, this, [this](const QVariant& v) {
        setRatio(v.toReal());
    });
}

void MyCollapsible::setRatio(qreal v) {
    if (qFuzzyCompare(m_ratio, v)) return;
    m_ratio = v;
    if (m_content) {
        m_content->setMaximumHeight(static_cast<int>(m_contentHeight * m_ratio));
        m_content->setMinimumHeight(static_cast<int>(m_contentHeight * m_ratio));
    }
    updateGeometry();
    update();
}

void MyCollapsible::setContentWidget(QWidget* content) {
    if (m_content) {
        m_content->setParent(nullptr);
        m_content->deleteLater();
    }
    m_content = content;
    if (!m_content) return;
    m_content->setParent(this);
    m_content->setAttribute(Qt::WA_TranslucentBackground, true);
    m_content->show();
    m_content->installEventFilter(this);
    updateGeometryAnimated();
}

void MyCollapsible::setHeaderText(const QString& text) {
    if (m_headerText == text) return;
    m_headerText = text;
    update();
}

void MyCollapsible::setAccentColor(const QColor& c) {
    m_accent = c;
    update();
}

void MyCollapsible::setDuration(int ms) {
    m_duration = ms;
    m_anim->setDuration(ms);
}

void MyCollapsible::setCollapsed(bool collapsed) {
    if (m_collapsed == collapsed) return;
    m_collapsed = collapsed;
    m_anim->stop();
    m_anim->setStartValue(m_ratio);
    m_anim->setEndValue(collapsed ? 0.0 : 1.0);
    m_anim->start();
    emit collapseStateChanged(collapsed);
}

QSize MyCollapsible::sizeHint() const {
    int headerH = 34;
    int contentH = static_cast<int>(m_contentHeight * m_ratio);
    return QSize(220, headerH + contentH);
}

void MyCollapsible::updateGeometryAnimated() {
    if (!m_content) return;
    m_contentHeight = m_content->sizeHint().height();
    if (m_contentHeight <= 0) m_contentHeight = m_content->sizeHint().height();
    m_content->setMaximumHeight(static_cast<int>(m_contentHeight * m_ratio));
    m_content->setMinimumHeight(static_cast<int>(m_contentHeight * m_ratio));
    updateGeometry();
}

void MyCollapsible::resizeEvent(QResizeEvent* event) {
    if (m_content) {
        m_content->setGeometry(0, 34, width(), static_cast<int>(m_contentHeight * m_ratio));
    }
    QWidget::resizeEvent(event);
}

void MyCollapsible::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF box = QRectF(this->rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainterPath path;
    path.addRoundedRect(box, UiTheme::radiusLg(), UiTheme::radiusLg());
    painter.setPen(QPen(UiTheme::border(), 1));
    painter.setBrush(UiTheme::surfaceAlt());
    painter.drawPath(path);

    painter.setPen(m_accent.darker(115));
    painter.setFont(UiTheme::uiFont(10, true));
    painter.drawText(QRectF(14, 0, width() - 40, 34), Qt::AlignVCenter | Qt::AlignLeft, m_headerText);

    painter.setPen(QPen(UiTheme::textSecondary(), 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPointF arrow(width() - 18, 17);
    qreal dir = m_ratio > 0.5 ? 1.0 : -1.0;
    painter.drawLine(arrow + QPointF(-4, -2 * dir), arrow);
    painter.drawLine(arrow + QPointF(4, -2 * dir), arrow);
}
