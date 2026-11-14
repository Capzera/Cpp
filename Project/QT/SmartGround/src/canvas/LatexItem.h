#ifndef LATEXITEM_H
#define LATEXITEM_H
#include "AnnotationItem.h"
#include <QSvgRenderer>
#include <QString>

class LatexItem : public AnnotationItem {
    Q_OBJECT
public:
    enum { Type = UserType + 25 };
    int type() const override { return Type; }

    explicit LatexItem(QGraphicsItem* parent = nullptr);

    void setTex(const QString& tex);
    QString tex() const { return m_tex; }

    void setSvg(const QString& svg, double width, double height);
    QString svg() const { return m_svg; }
    bool hasRender() const { return !m_svg.isEmpty(); }

    void setTargetHeight(qreal h);
    qreal targetHeight() const { return m_targetHeight; }

    bool requestRender();
    bool isRenderPending() const { return m_pending; }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void applyStyle() override;
    QRectF contentBounds() const override;
    AnnotationItem* cloneItem() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void handleRendered(const QString& tex, const QString& svg, double width, double height);

    QString m_tex;
    QString m_svg;
    QSvgRenderer m_renderer;
    double m_srcWidth = 0.0;
    double m_srcHeight = 0.0;
    qreal m_targetHeight = 46.0;
    bool m_pending = false;
    int m_requestToken = 0;
};

#endif
