#include "AnnotationScene.h"
#include "AnnotationItem.h"
#include "StrokeItem.h"
#include "ShapeItem.h"
#include "TextBoxItem.h"
#include "MarkdownItem.h"
#include "LatexItem.h"
#include "CanvasTableItem.h"
#include <QGraphicsView>

AnnotationScene::AnnotationScene(QObject* parent) : QGraphicsScene(parent) {
    setSceneRect(-4000, -4000, 8000, 8000);
}

AnnotationScene::~AnnotationScene() {
    for (AnnotationItem* item : m_owned) {
        delete item;
    }
    m_owned.clear();
}

void AnnotationScene::setHostView(QGraphicsView* view) {
    m_hostView = view;
}

void AnnotationScene::addItem(AnnotationItem* item) {
    if (!item) return;
    QGraphicsScene::addItem(item);
    if (!m_owned.contains(item)) m_owned.push_back(item);
    connect(item, &AnnotationItem::contentChanged, this, [this]() {
        if (m_hostView) m_hostView->viewport()->update();
    });
    emit itemsChanged();
    if (m_hostView) m_hostView->viewport()->update();
}

void AnnotationScene::removeItem(AnnotationItem* item, bool destroy) {
    if (!item) return;
    QGraphicsScene::removeItem(item);
    if (destroy) {
        m_owned.removeOne(item);
        item->deleteLater();
    }
    emit itemsChanged();
    if (m_hostView) m_hostView->viewport()->update();
}

void AnnotationScene::destroyItem(AnnotationItem* item) {
    if (!item) return;
    if (item->scene()) QGraphicsScene::removeItem(item);
    m_owned.removeOne(item);
    delete item;
    emit itemsChanged();
}

QVector<AnnotationItem*> AnnotationScene::annotationItems() const {
    QVector<AnnotationItem*> out;
    for (QGraphicsItem* item : items()) {
        AnnotationItem* ann = dynamic_cast<AnnotationItem*>(item);
        if (ann) out.push_back(ann);
    }
    return out;
}

AnnotationItem* AnnotationScene::itemByUid(const QString& uid) const {
    for (QGraphicsItem* item : items()) {
        AnnotationItem* ann = dynamic_cast<AnnotationItem*>(item);
        if (ann && ann->uid() == uid) return ann;
    }
    return nullptr;
}

void AnnotationScene::setAnnotationsVisible(bool on) {
    if (m_visible == on) return;
    m_visible = on;
    for (QGraphicsItem* item : items()) {
        item->setVisible(on);
    }
    emit visibilityChanged(on);
}

void AnnotationScene::setAnnotationsLocked(bool on) {
    if (m_locked == on) return;
    m_locked = on;
    for (QGraphicsItem* item : items()) {
        AnnotationItem* ann = dynamic_cast<AnnotationItem*>(item);
        if (ann) ann->setLocked(on);
    }
    emit lockChanged(on);
}

void AnnotationScene::clearAnnotations() {
    const QVector<AnnotationItem*> all = annotationItems();
    for (AnnotationItem* item : all) {
        QGraphicsScene::removeItem(item);
        m_owned.removeOne(item);
        item->deleteLater();
    }
    emit itemsChanged();
}

void AnnotationScene::stashTo(QString bucket) {
    if (bucket.isEmpty()) return;
    if (annotationItems().isEmpty()) {
        m_stash.remove(bucket);
        return;
    }
    m_stash.insert(bucket, toJson());
    clearAnnotations();
}

bool AnnotationScene::restoreFrom(QString bucket) {
    const auto it = m_stash.constFind(bucket);
    if (it == m_stash.constEnd()) return false;
    fromJson(it.value());
    return true;
}

QJsonObject AnnotationScene::toJson() const {
    QJsonObject obj;
    obj["visible"] = m_visible;
    obj["locked"] = m_locked;
    QJsonArray arr;
    for (AnnotationItem* item : annotationItems()) {
        arr.push_back(item->toJson());
    }
    obj["items"] = arr;
    return obj;
}

void AnnotationScene::fromJson(const QJsonObject& obj) {
    clearAnnotations();
    m_visible = obj.value("visible").toBool(true);
    m_locked = obj.value("locked").toBool(false);
    const QJsonArray arr = obj.value("items").toArray();
    for (const QJsonValue& v : arr) {
        QJsonObject o = v.toObject();
        const int type = o.value("type").toInt();
        AnnotationItem* item = nullptr;
        if (type == StrokeItem::Type) {
            item = new StrokeItem();
        } else if (type == ShapeItem::Type) {
            item = new ShapeItem(ShapeItem::Line);
        } else if (type == TextBoxItem::Type) {
            item = new TextBoxItem();
        } else if (type == MarkdownItem::Type) {
            item = new MarkdownItem();
        } else if (type == LatexItem::Type) {
            item = new LatexItem();
        } else if (type == CanvasTableItem::Type) {
            item = new CanvasTableItem(2, 2);
        }
        if (!item) continue;
        item->fromJson(o);
        item->setVisible(m_visible);
        item->setLocked(m_locked);
        addItem(item);
    }
    emit itemsChanged();
}
