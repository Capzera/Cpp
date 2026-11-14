#include "LatexBlock.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSvgRenderer>
#include <QPainter>
#include <QPainterPath>

namespace {
QVector<LatexBlock::Formula> g_library;
bool g_loaded = false;
}

bool LatexBlock::loadLibrary(const QString& jsonPath) {
    QByteArray data;
    QFile file(jsonPath);
    if (file.open(QIODevice::ReadOnly)) {
        data = file.readAll();
        file.close();
    }
    if (data.isEmpty()) {
        QFile res(":/render/latex_formulas.json");
        if (res.open(QIODevice::ReadOnly)) {
            data = res.readAll();
            res.close();
        }
    }
    if (data.isEmpty()) return false;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return false;
    QJsonArray arr = doc.object().value("formulas").toArray();
    g_library.clear();
    for (const QJsonValue& v : arr) {
        QJsonObject o = v.toObject();
        Formula f;
        f.tex = o.value("tex").toString();
        f.svg = o.value("svg").toString();
        f.width = o.value("width").toDouble();
        f.height = o.value("height").toDouble();
        g_library.push_back(f);
    }
    g_loaded = true;
    return !g_library.isEmpty();
}

const QVector<LatexBlock::Formula>& LatexBlock::library() {
    return g_library;
}

LatexBlock::LatexBlock(QGraphicsItem* parent) : QGraphicsObject(parent) {
    setZValue(2);
    setFlag(ItemIsMovable, true);
}

void LatexBlock::setFormulaIndex(int index) {
    if (index < 0 || index >= g_library.size()) return;
    m_index = index;
    prepareGeometryChange();
    update();
}

void LatexBlock::setScaleFactor(qreal s) {
    if (qFuzzyCompare(m_scale, s)) return;
    m_scale = s;
    prepareGeometryChange();
    update();
}

void LatexBlock::setTargetHeight(qreal h) {
    if (qFuzzyCompare(m_targetHeight, h)) return;
    m_targetHeight = h;
    prepareGeometryChange();
    update();
}

void LatexBlock::setTint(const QColor& color) {
    m_tint = color;
    update();
}

QRectF LatexBlock::boundingRect() const {
    if (m_index < 0 || m_index >= g_library.size()) return QRectF(0, 0, 10, 10);
    const Formula& f = g_library[m_index];
    double w = f.width / 1000.0 * m_scale;
    double h = f.height / 1000.0 * m_scale;
    if (m_targetHeight > 0.0 && f.height > 0.0) {
        double factor = m_targetHeight / f.height;
        w = f.width * factor;
        h = m_targetHeight;
    }
    return QRectF(-w / 2.0, -h / 2.0, w + 4, h + 4);
}

void LatexBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (m_index < 0 || m_index >= g_library.size()) return;
    const Formula& f = g_library[m_index];
    if (f.svg.isEmpty()) return;
    QString markup = f.svg;
    if (m_tint.isValid() && markup.contains("currentColor")) {
        markup.replace("currentColor", m_tint.name());
    }
    QSvgRenderer renderer(markup.toUtf8());
    if (!renderer.isValid()) return;
    QRectF target = boundingRect().adjusted(2, 2, -2, -2);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(target.left(), target.top());
    renderer.render(painter, QRectF(0, 0, target.width(), target.height()));
    painter->restore();
}
