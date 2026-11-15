#include "LatexItem.h"
#include "LatexService.h"
#include "ui/widgets/AnnotationEditor.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>

LatexItem::LatexItem(QGraphicsItem* parent) : AnnotationItem(parent) {
    setZValue(10);
    setAcceptedMouseButtons(Qt::LeftButton);
    setToolTip("双击可重新输入 LaTeX 源码，拖动可移动位置");
    connect(LatexService::instance(), &LatexService::rendered,
            this, &LatexItem::handleRendered);
}

void LatexItem::setTex(const QString& tex) {
    if (m_tex == tex) return;
    m_tex = tex;
    setToolTip(tex.isEmpty() ? "双击可重新输入 LaTeX 源码" : tex);
    requestRender();
    update();
}

bool LatexItem::requestRender() {
    if (m_tex.trimmed().isEmpty()) return false;
    if (!LatexService::instance()->isAvailable()) return false;
    m_pending = true;
    ++m_requestToken;
    LatexService::instance()->request(m_tex);
    return true;
}

void LatexItem::handleRendered(const QString& tex, const QString& svg, double width, double height) {
    if (tex != m_tex) return;
    m_pending = false;
    setSvg(svg, width, height);
}

void LatexItem::setSvg(const QString& svg, double width, double height) {
    m_svg = svg;
    m_srcWidth = width;
    m_srcHeight = height;
    if (!svg.isEmpty()) {
        m_renderer.load(QByteArray::fromStdString(svg.toStdString()));
    }
    prepareGeometryChange();
    update();
}

void LatexItem::setTargetHeight(qreal h) {
    if (qFuzzyCompare(m_targetHeight, h)) return;
    m_targetHeight = h;
    prepareGeometryChange();
    update();
}

QRectF LatexItem::contentBounds() const {
    if (m_svg.isEmpty() || m_srcWidth <= 0.0 || m_srcHeight <= 0.0) return QRectF();
    const qreal factor = m_targetHeight / m_srcHeight;
    const qreal w = m_srcWidth * factor;
    return QRectF(-w / 2.0, -m_targetHeight / 2.0, w, m_targetHeight);
}

QRectF LatexItem::boundingRect() const {
    return contentBounds().adjusted(-6, -6, 6, 6);
}

void LatexItem::applyStyle() {
    setAcceptedMouseButtons(Qt::LeftButton);
}

void LatexItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    const QRectF r = contentBounds();
    if (m_svg.isEmpty()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(QColor(150, 160, 178), 1.2, Qt::DashLine));
        painter->setBrush(QColor(250, 251, 253, 220));
        painter->drawRoundedRect(r.adjusted(-4, -4, 4, 4), 6, 6);
        painter->setPen(QColor(140, 150, 168));
        painter->drawText(r.adjusted(-4, -4, 4, 4), Qt::AlignCenter, "LaTeX");
        painter->restore();
        drawSelectionFrame(painter);
        return;
    }
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    if (m_renderer.isValid()) {
        m_renderer.render(painter, r);
    }
    painter->restore();
    drawSelectionFrame(painter);
}

void LatexItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_locked) {
        AnnotationEditor* editor = new AnnotationEditor(AnnotationEditor::LatexMode, m_tex, nullptr);
        editor->setAttribute(Qt::WA_DeleteOnClose);
        editor->setLiveMode(true);
        editor->setWindowTitle("编辑 LaTeX 公式");
        connect(editor, &AnnotationEditor::contentChanged, this, [this](const QString& text) {
            setTex(text);
        });
        connect(editor, &QDialog::finished, editor, &QObject::deleteLater);
        editor->show();
        editor->raise();
        event->accept();
        return;
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}

AnnotationItem* LatexItem::cloneItem() const {
    auto* copy = new LatexItem();
    copy->setTex(m_tex);
    copy->setSvg(m_svg, m_srcWidth, m_srcHeight);
    copy->setTargetHeight(m_targetHeight);
    copy->setPenColor(m_penColor);
    copy->setPenWidth(m_penWidth);
    copy->setFillOpacity(m_fillOpacity);
    copy->setLocked(m_locked);
    return copy;
}

QJsonObject LatexItem::toJson() const {
    QJsonObject obj = AnnotationItem::toJson();
    obj["tex"] = m_tex;
    obj["svg"] = m_svg;
    obj["srcWidth"] = m_srcWidth;
    obj["srcHeight"] = m_srcHeight;
    obj["targetHeight"] = m_targetHeight;
    return obj;
}

void LatexItem::fromJson(const QJsonObject& obj) {
    AnnotationItem::fromJson(obj);
    m_tex = obj.value("tex").toString();
    m_targetHeight = obj.value("targetHeight").toDouble(m_targetHeight);
    setSvg(obj.value("svg").toString(),
           obj.value("srcWidth").toDouble(),
           obj.value("srcHeight").toDouble());
    setToolTip(m_tex.isEmpty() ? "双击可重新输入 LaTeX 源码" : m_tex);
}
