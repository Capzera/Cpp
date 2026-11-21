#include "CartesianInfoPanel.h"
#include "ui/UiTheme.h"
#include "ui/UiFonts.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>

namespace {

class InfoCard : public QWidget {
public:
    explicit InfoCard(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF box = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(box, 12, 12);
        painter.setPen(QPen(QColor(223, 228, 236), 1));
        painter.setBrush(QColor(255, 255, 255, 252));
        painter.drawPath(path);
        QPainterPath inner;
        inner.addRoundedRect(box.adjusted(1, 1, -1, -1), 11, 11);
        painter.setPen(QPen(QColor(255, 255, 255, 190), 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(inner);
    }
};

QString formatCoord(double value) {
    QString text = QString::number(value, 'f', 4);
    if (text.contains('.')) {
        while (text.endsWith('0')) text.chop(1);
        if (text.endsWith('.')) text.chop(1);
    }
    if (text == "-0") text = "0";
    return text;
}

}  // namespace

CartesianInfoPanel::CartesianInfoPanel(QWidget* parent) : QWidget(parent) {
    setObjectName("cartesianInfoPanel");
    setVisible(false);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    m_host = new QWidget(this);
    m_host->setObjectName("cartesianInfoHost");
    QVBoxLayout* hostLayout = new QVBoxLayout(m_host);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->setSpacing(0);
    root->addWidget(m_host);

    m_host->installEventFilter(this);
}

QWidget* CartesianInfoPanel::buildCard() {
    InfoCard* card = new InfoCard(m_host);
    card->setObjectName("cartesianInfoCard");
    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(8);

    m_title = new QLabel(card);
    m_title->setObjectName("cartesianInfoTitle");
    m_title->setFont(UiTheme::uiFont(10, true));
    layout->addWidget(m_title);

    m_body = new QLabel(card);
    m_body->setObjectName("cartesianInfoBody");
    m_body->setFont(UiFonts::mono(10, false));
    m_body->setWordWrap(true);
    m_body->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(m_body);

    m_hint = new QLabel(card);
    m_hint->setObjectName("hintLabel");
    m_hint->setFont(UiTheme::uiFont(9));
    m_hint->setWordWrap(true);
    layout->addWidget(m_hint);

    return card;
}

void CartesianInfoPanel::showCard(QWidget* card) {
    if (m_card) {
        m_host->layout()->removeWidget(m_card);
        m_card->deleteLater();
    }
    m_card = card;
    m_host->layout()->addWidget(card);
    m_card->show();
    setVisible(true);
    raise();
}

void CartesianInfoPanel::showPoint(const QString& name, double x, double y, const QColor& color,
                                   const QString& extra) {
    QWidget* card = buildCard();
    m_title->setText(QString("点 %1").arg(name));
    m_title->setStyleSheet(QString("color:%1;").arg(color.darker(135).name()));
    m_body->setText(QString("x = %1\ny = %2").arg(formatCoord(x), formatCoord(y)));
    m_hint->setText(extra.isEmpty() ? QString("双击画布上的点可随时再次查看此信息。")
                                    : extra);
    showCard(card);
}

void CartesianInfoPanel::showSegment(const QString& equation, const QString& detail) {
    QWidget* card = buildCard();
    m_title->setText("线段");
    m_title->setStyleSheet(QString("color:%1;").arg(UiTheme::textPrimary().name()));
    m_body->setText(equation.isEmpty() ? QString("（该线段退化为一个点）") : equation);
    m_hint->setText(detail);
    showCard(card);
}

void CartesianInfoPanel::showCurve(const QString& name, const QString& expression, const QColor& color) {
    QWidget* card = buildCard();
    m_title->setText(QString("函数 %1").arg(name));
    m_title->setStyleSheet(QString("color:%1;").arg(color.darker(135).name()));
    m_body->setText(QString("y = %1").arg(expression));
    m_hint->setText("可在左侧「对象列表」中修改表达式或删除该函数。");
    showCard(card);
}

void CartesianInfoPanel::clearInfo() {
    if (m_card) {
        m_host->layout()->removeWidget(m_card);
        m_card->deleteLater();
        m_card = nullptr;
    }
    setVisible(false);
}

QString CartesianInfoPanel::infoTitle() const { return m_title ? m_title->text() : QString(); }

QString CartesianInfoPanel::infoBody() const { return m_body ? m_body->text() : QString(); }

QString CartesianInfoPanel::infoHint() const { return m_hint ? m_hint->text() : QString(); }

void CartesianInfoPanel::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QWidget::paintEvent(event);
}
