import os

ROOT = r'd:\Program\Cpp\Project\QT\SmartGround'
PATH = os.path.join(ROOT, 'src', 'modules', 'IModule.cpp')

BLOCK = '''
bool IModule::s_scriptTextVisible = false;

void IModule::setScriptTextVisible(bool on) {
    s_scriptTextVisible = on;
}

bool IModule::scriptTextVisible() {
    return s_scriptTextVisible;
}

QGraphicsTextItem* IModule::createScriptText(QGraphicsScene* scene, const QString& text) {
    if (!scene) return nullptr;
    QGraphicsTextItem* item = scene->addText(text);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
    }
    return item;
}

QGraphicsTextItem* IModule::createStatusText(QGraphicsScene* scene, const QString& text) {
    if (!scene) return nullptr;
    QGraphicsTextItem* item = scene->addText(text);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return item;
}

void IModule::setScriptText(QGraphicsTextItem* item, const QString& text) {
    if (!item) return;
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
        return;
    }
    item->setVisible(true);
    item->setPlainText(text);
}

void IModule::setScriptText(QGraphicsTextItem* item, const QString& text, const QColor& color) {
    if (!item) return;
    if (!s_scriptTextVisible) {
        item->setPlainText(QString());
        item->setVisible(false);
        return;
    }
    item->setVisible(true);
    item->setDefaultTextColor(color);
    item->setPlainText(text);
}

void IModule::clearScriptText(QGraphicsTextItem* item) {
    if (!item) return;
    item->setPlainText(QString());
}

void IModule::setStatusText(QGraphicsTextItem* item, const QString& text) {
    if (!item) return;
    item->setVisible(true);
    item->setPlainText(text);
}

void IModule::playTextFlightTo(QGraphicsTextItem* target, const QString& text, QPointF start,
                                int duration, std::function<void()> onArrive) {
    if (!target || !m_scene) {
        if (onArrive) onArrive();
        return;
    }
    const QFont font = target->font();
    const QPointF dst = target->pos();
    if (!m_animated) {
        target->setPlainText(text);
        target->setPos(dst);
        if (onArrive) onArrive();
        return;
    }
    QPointer<QGraphicsTextItem> safeTarget = target;
    QGraphicsTextItem* clone = m_scene->addText(text);
    clone->setFont(font);
    clone->setDefaultTextColor(target->defaultTextColor());
    clone->setPos(start);
    clone->setZValue(target->zValue() + 1.0);
    QVariantAnimation* anim = new QVariantAnimation(clone);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [clone, start, dst](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!clone) return;
        clone->setPos(start.x() + (dst.x() - start.x()) * t,
                      start.y() + (dst.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [clone, safeTarget, text, dst, onArrive]() {
        if (clone) {
            if (clone->scene()) clone->scene()->removeItem(clone);
            delete clone;
        }
        if (safeTarget) {
            safeTarget->setPlainText(text);
            safeTarget->setPos(dst);
        }
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IModule::playTextFlightFragment(QGraphicsTextItem* target, const QString& prefix, const QString& fragment,
                                     QPointF start, int duration, std::function<void()> onArrive) {
    if (!target || !m_scene) {
        if (onArrive) onArrive();
        return;
    }
    const QFont font = target->font();
    const QPointF dst = target->pos();
    const QFontMetrics fm(font);
    const double prefixWidth = fm.horizontalAdvance(prefix);
    if (!m_animated) {
        if (onArrive) onArrive();
        return;
    }
    QPointer<QGraphicsTextItem> safeTarget = target;
    QGraphicsTextItem* clone = m_scene->addText(fragment);
    clone->setFont(font);
    clone->setDefaultTextColor(target->defaultTextColor());
    clone->setPos(start);
    clone->setZValue(target->zValue() + 1.0);
    QVariantAnimation* anim = new QVariantAnimation(clone);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    const QPointF fragDst = dst + QPointF(prefixWidth, 0.0);
    QObject::connect(anim, &QVariantAnimation::valueChanged, [clone, start, fragDst](const QVariant& progress) {
        const qreal t = progress.toReal();
        if (!clone) return;
        clone->setPos(start.x() + (fragDst.x() - start.x()) * t,
                      start.y() + (fragDst.y() - start.y()) * t);
    });
    QObject::connect(anim, &QVariantAnimation::finished, [clone, safeTarget, dst, onArrive]() {
        if (clone) {
            if (clone->scene()) clone->scene()->removeItem(clone);
            delete clone;
        }
        if (safeTarget) safeTarget->setPos(dst);
        if (onArrive) onArrive();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
'''

INCLUDES = '''#include <QGraphicsTextItem>
#include <QFontMetrics>
#include <QColor>
'''


def main():
    text = open(PATH, encoding='utf-8', newline='').read().replace('\r\n', '\n')
    if '#include <QGraphicsTextItem>' not in text:
        text = text.replace('#include <cmath>\n', '#include <cmath>\n' + INCLUDES, 1)
    if 'bool IModule::s_scriptTextVisible' not in text:
        text = text.rstrip() + '\n' + BLOCK
    open(PATH, 'w', encoding='utf-8', newline='').write(text)
    print('ok', len(text.splitlines()), 'lines')


if __name__ == '__main__':
    main()
