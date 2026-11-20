#include "BubbleSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QPointer>

BubbleSortModule::BubbleSortModule() {}

QString BubbleSortModule::getModuleName() const {
    return "冒泡排序";
}

QString BubbleSortModule::getDefaultInput() const {
    return "8\n3 5 1 9 2 7 4 6";
}
QString BubbleSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组";
}

void BubbleSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_data.clear();
    m_blocks.clear();
    m_frames.clear();
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) {
        m_data.push_back(items[i].toInt());
    }
    n = m_data.size();
    int startX = 400 - (n * 60) / 2;
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        m_scene->addItem(block);
        block->setPos(startX + i * 60, 300);
        m_blocks.push_back(block);
        registerArraySlot(block, "a", i);
    }
}

std::vector<AnimFrame> BubbleSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;
    bool swapped = false;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            m_frames.push_back({ActionType::AR_HIGHLIGHT, j, j + 1, 0, 0, {},
                QString("比较 arr[%1] 和 arr[%2]").arg(j).arg(j+1)});
            if (arr[j] > arr[j + 1]) {
                m_frames.push_back({ActionType::AR_SWAP, j, j + 1, 0, 0, {},
                    QString("交换 arr[%1] 和 arr[%2]").arg(j).arg(j+1)});
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
            m_frames.push_back({ActionType::AR_RESET, j, j + 1, 0, 0, {}, QString("完成比较")});
        }
        m_frames.push_back({ActionType::AR_DONE, n - i - 1, 0, 0, 0, {},
            QString("元素 %1 已锁定在最终位置").arg(arr[n - i - 1])});
        if (!swapped) {
            for (int k = 0; k < n - i - 1; ++k) {
                m_frames.push_back({ActionType::AR_DONE, k, 0, 0, 0, {},
                    QString("未发生交换，元素 %1 提前锁定").arg(arr[k])});
            }
            break;
        }
    }
    if (n > 0 && swapped) {
        m_frames.push_back({ActionType::AR_DONE, 0, 0, 0, 0, {}, QString("排序完全结束")});
    }
    return m_frames;
}

void BubbleSortModule::renderFrame(const AnimFrame& step) {
    if (step.type == ActionType::AR_HIGHLIGHT) {
        m_blocks[step.u]->color = QColor(255, 200, 100);
        m_blocks[step.u]->setZValue(2);
        m_blocks[step.u]->update();
        m_blocks[step.v]->color = QColor(255, 200, 100);
        m_blocks[step.v]->setZValue(2);
        m_blocks[step.v]->update();
        if (m_animated) {
            playScalePulse(m_blocks[step.u], 1.15, 200);
            playScalePulse(m_blocks[step.v], 1.15, 200);
            playCellRipple(m_blocks[step.u], QColor(255, 190, 90), 0.85, 440);
            playCellRipple(m_blocks[step.v], QColor(255, 190, 90), 0.85, 440);
        }
    } else if (step.type == ActionType::AR_SWAP) {
        ArrayBlock* b1 = m_blocks[step.u];
        ArrayBlock* b2 = m_blocks[step.v];
        int val1 = b1->value;
        int val2 = b2->value;
        QPointer<ArrayBlock> safeB1 = b1;
        QPointer<ArrayBlock> safeB2 = b2;
        if (m_animated) {
            b1->showValue = false;
            b1->update();
            b2->showValue = false;
            b2->update();
            QPointer<FloatingText> t1 = new FloatingText(QString::number(b1->value));
            QPointer<FloatingText> t2 = new FloatingText(QString::number(b2->value));
            t1->setPos(b1->pos());
            t2->setPos(b2->pos());
            m_scene->addItem(t1.data());
            m_scene->addItem(t2.data());
            QVariantAnimation *anim = new QVariantAnimation(this);
            anim->setDuration(500);
            anim->setStartValue(0.0);
            anim->setEndValue(1.0);
            anim->setEasingCurve(QEasingCurve::InOutQuad);
            QPointF p1 = b1->pos();
            QPointF p2 = b2->pos();
            connect(anim, &QVariantAnimation::valueChanged, [t1, t2, p1, p2](const QVariant &value) {
                qreal t = value.toReal();
                qreal arc = std::sin(t * M_PI) * 40.0;
                if (t1) t1->setPos(p1.x() + (p2.x() - p1.x()) * t, p1.y() - arc);
                if (t2) t2->setPos(p2.x() + (p1.x() - p2.x()) * t, p2.y() + arc);
            });
            connect(anim, &QVariantAnimation::finished, [safeB1, safeB2, t1, t2, val1, val2]() {
                if (safeB1) { safeB1->value = val2; safeB1->showValue = true; safeB1->update(); }
                if (safeB2) { safeB2->value = val1; safeB2->showValue = true; safeB2->update(); }
                if (t1) { if (t1->scene()) t1->scene()->removeItem(t1.data()); delete t1.data(); }
                if (t2) { if (t2->scene()) t2->scene()->removeItem(t2.data()); delete t2.data(); }
            });
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        } else {
            b1->value = val2;
            b2->value = val1;
            b1->showValue = true;
            b1->update();
            b2->showValue = true;
            b2->update();
        }
    } else if (step.type == ActionType::AR_RESET) {
        m_blocks[step.u]->color = Qt::white;
        m_blocks[step.u]->setZValue(1);
        m_blocks[step.u]->update();
        m_blocks[step.v]->color = Qt::white;
        m_blocks[step.v]->setZValue(1);
        m_blocks[step.v]->update();
    } else if (step.type == ActionType::AR_DONE) {
        m_blocks[step.u]->color = QColor(150, 255, 150);
        m_blocks[step.u]->setZValue(1);
        m_blocks[step.u]->update();
        if (m_animated) {
            playScalePulse(m_blocks[step.u], 1.2, 300);
            playCellRipple(m_blocks[step.u], QColor(120, 220, 140), 1.05, 520);
        }
    }
}

void BubbleSortModule::reset() {
    for (auto block : m_blocks) {
        if (!block) continue;
        block->color = Qt::white;
        block->showValue = true;
        block->setZValue(1);
        block->update();
    }
}
void BubbleSortModule::releaseSceneItems() {
    m_blocks.clear();
}

