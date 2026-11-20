#ifndef TEXTEDITGUARD_H
#define TEXTEDITGUARD_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QApplication>
#include <QAbstractSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QWidget>

namespace TextEditGuard {

inline bool sceneHasEditor(QGraphicsScene* scene) {
    if (!scene) return false;
    QGraphicsItem* focus = scene->focusItem();
    if (!focus) return false;
    auto* text = qgraphicsitem_cast<QGraphicsTextItem*>(focus);
    if (!text) return false;
    return text->textInteractionFlags().testFlag(Qt::TextEditorInteraction);
}

inline bool widgetHasEditor(QWidget* widget) {
    if (!widget) return false;
    if (qobject_cast<QTextEdit*>(widget)) return true;
    if (qobject_cast<QLineEdit*>(widget)) return true;
    if (qobject_cast<QAbstractSpinBox*>(widget)) return true;
    return false;
}

inline bool active(QGraphicsScene* scene) {
    if (widgetHasEditor(QApplication::focusWidget())) return true;
    return sceneHasEditor(scene);
}

}  // namespace TextEditGuard
#endif
