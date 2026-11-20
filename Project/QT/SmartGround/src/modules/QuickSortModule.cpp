#include "QuickSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QPointer>
#include <QFont>

QuickSortModule::QuickSortModule() : m_mathText(nullptr), m_startX(0), m_ptrI(nullptr), m_ptrJ(nullptr), m_pivotBlock(nullptr) {}

QString QuickSortModule::getModuleName() const { return "快速排序"; }
QString QuickSortModule::getDefaultInput() const { return "10\n6 1 2 7 9 3 4 5 10 8"; }
QString QuickSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组";
}

void QuickSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_ptrJ = nullptr; m_pivotBlock = nullptr;

    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) m_data.push_back(items[i].toInt());
    n = m_data.size();
    m_startX = 400 - (n * 60) / 2;

    m_mathText = createScriptText(m_scene, "");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 50);

    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        m_scene->addItem(block);
        block->setPos(m_startX + i * 60, 300);
        m_blocks[i] = block;
        registerArraySlot(block, "a", i);
    }

    m_pivotBlock = new ArrayBlock(1000, 0, "pivot");
    m_pivotBlock->setPos(400, 150);
    m_pivotBlock->showValue = false;
    m_scene->addItem(m_pivotBlock);
    m_blocks[1000] = m_pivotBlock;

    m_ptrI = new PointerBlock("i", QColor(220, 50, 50));
    m_ptrI->setPos(m_startX + 30, 350);
    m_ptrI->setOpacity(0);
    m_ptrI->setProperty("logical_pos", -1);
    m_scene->addItem(m_ptrI);

    m_ptrJ = new PointerBlock("j", QColor(50, 150, 220));
    m_ptrJ->setPos(m_startX + 30, 350);
    m_ptrJ->setOpacity(0);
    m_ptrJ->setProperty("logical_pos", -2);
    m_scene->addItem(m_ptrJ);
}
std::vector<AnimFrame> QuickSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;

    m_frames.push_back({ActionType::AR_RESET, -1, 0, 0, 0, {}, "初始化序列"});
    quickSort(arr, 0, n - 1, 1);
    m_frames.push_back({ActionType::AR_RESET, -2, 0, 0, 0, {}, "快速排序完成！"});

    return m_frames;
}

void QuickSortModule::quickSort(std::vector<int>& arr, int l, int r, int depth) {
    if (l > r) return;
    if (l == r) {
        m_frames.push_back({ActionType::AR_DONE, l, 0, 0, 0, {}, QString("区间仅剩一个元素，位置 %1 已锁定").arg(l)});
        return;
    }

    m_frames.push_back({ActionType::AR_HIGHLIGHT, l, r, 0, 0, {}, QString("递归层 %1: 处理子区间 [%2, %3]，非当前区间进入阴影").arg(depth).arg(l).arg(r)});

    int pivot = arr[l];
    m_frames.push_back({ActionType::AR_SCAN_MIN, l, 1000, pivot, 0, {}, QString("选取 arr[%1]=%2 作为基准值 (pivot)").arg(l).arg(pivot)});

    int i = l, j = r;
    m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, i, 0, 0, {}, "初始化左游标 i"});
    m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, j, 0, 0, {}, "初始化右游标 j"});

    while (i < j) {
        while (i < j && arr[j] >= pivot) {
            m_frames.push_back({ActionType::AR_COMPARE_SHIFT, j, 0, 0, 0, {}, QString("arr[%1]=%2 >= pivot，j 左移").arg(j).arg(arr[j])});
            j--;
            m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, j, 0, 0, {}, QString("j 移动到 %1").arg(j)});
        }
        if (i < j) {
            m_frames.push_back({ActionType::AR_COMPARE_SHIFT, j, 0, 0, 0, {}, QString("发现 arr[%1]=%2 < pivot，j 停下").arg(j).arg(arr[j])});
        }

        while (i < j && arr[i] <= pivot) {
            m_frames.push_back({ActionType::AR_COMPARE_SHIFT, i, 0, 0, 0, {}, QString("arr[%1]=%2 <= pivot，i 右移").arg(i).arg(arr[i])});
            i++;
            m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, i, 0, 0, {}, QString("i 移动到 %1").arg(i)});
        }
        if (i < j) {
            m_frames.push_back({ActionType::AR_COMPARE_SHIFT, i, 0, 0, 0, {}, QString("发现 arr[%1]=%2 > pivot，i 停下").arg(i).arg(arr[i])});
            m_frames.push_back({ActionType::AR_SWAP, i, j, arr[i], arr[j], {}, QString("发生跨界互换：交换 arr[%1] 和 arr[%2]").arg(i).arg(j)});
            std::swap(arr[i], arr[j]);
        }
    }

    m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, i, 0, 0, {}, QString("i 和 j 在位置 %1 发生相撞").arg(i)});
    if (l != i) {
        m_frames.push_back({ActionType::AR_SWAP, l, i, arr[l], arr[i], {}, QString("基准值归位：原点 arr[%1] 飞向最终相遇点 arr[%2]").arg(l).arg(i)});
        std::swap(arr[l], arr[i]);
        m_frames.push_back({ActionType::AR_RESET, l, l, 0, 0, {}, ""}); // 原点褪去橙色
    }
    m_frames.push_back({ActionType::AR_DONE, i, 0, 0, 0, {}, QString("基准值 %1 已永久锁定在最终位置 %2").arg(pivot).arg(i)});

    quickSort(arr, l, i - 1, depth + 1);
    quickSort(arr, i + 1, r, depth + 1);
}

void QuickSortModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) setScriptText(m_mathText, step.mathText);

    if (step.type == ActionType::AR_RESET) {
        if (step.u == -1) {
            for (auto& p : m_blocks) {
                p.second->color = Qt::white;
                p.second->setOpacity(1.0);
                if (p.first < 1000) {
                    p.second->value = m_data[p.first];
                    p.second->showValue = true;
                } else {
                    p.second->showValue = false;
                }
                p.second->update();
            }
            if (m_ptrI) { m_ptrI->setOpacity(0); m_ptrI->setProperty("logical_pos", -1); }
            if (m_ptrJ) { m_ptrJ->setOpacity(0); m_ptrJ->setProperty("logical_pos", -2); }
        } else if (step.u == -2) {
            if (m_ptrI) { m_ptrI->setOpacity(0); m_ptrI->setProperty("logical_pos", -1); }
            if (m_ptrJ) { m_ptrJ->setOpacity(0); m_ptrJ->setProperty("logical_pos", -2); }
            if (m_blocks.count(1000)) {
                m_blocks[1000]->showValue = false;
                m_blocks[1000]->update();
            }
            for (auto& p : m_blocks) {
                if (p.first < 1000) {
                    p.second->setOpacity(1.0);
                    p.second->update();
                }
            }
        } else if (step.u >= 0) {
            for (int k = step.u; k <= step.v; ++k) {
                if (m_blocks.count(k) && m_blocks[k]->color != QColor(150, 255, 150)) {
                    m_blocks[k]->color = Qt::white;
                    m_blocks[k]->update();
                }
            }
        }
        return;
    }

    if (step.type == ActionType::AR_HIGHLIGHT) {
        for (auto& p : m_blocks) {
            if (p.first < 1000) {
                if (p.second->color != QColor(150, 255, 150)) {
                    if (p.first >= step.u && p.first <= step.v) {
                        p.second->setOpacity(1.0);
                        p.second->color = Qt::white;
                    } else {
                        p.second->setOpacity(0.3);
                    }
                    p.second->update();
                }
            }
        }
        std::vector<ArrayBlock*> range;
        for (int k = step.u; k <= step.v; ++k) {
            if (m_blocks.count(k) && k < 1000) range.push_back(m_blocks[k]);
        }
        if (!range.empty()) playCellSweep(range, QColor(150, 190, 245), 46, 280);
    } else if (step.type == ActionType::AR_SCAN_MIN) {
        ArrayBlock* src = m_blocks[step.u];
        ArrayBlock* dst = m_blocks[step.v];
        int val = step.val1;
        QPointer<ArrayBlock> safeDst = dst;
        if (m_animated) {
            src->color = QColor(255, 170, 0);
            src->update();
            QPointer<FloatingText> ft = new FloatingText(QString::number(val));
            ft->setPos(src->pos());
            m_scene->addItem(ft.data());
            QPropertyAnimation* anim = new QPropertyAnimation(ft.data(), "pos");
            anim->setEndValue(dst->pos());
            anim->setDuration(400);
            anim->setEasingCurve(QEasingCurve::InOutQuad);
            connect(anim, &QPropertyAnimation::finished, [this, safeDst, ft, val]() {
                if (ft) {
                    if (ft->scene()) ft->scene()->removeItem(ft.data());
                    delete ft.data();
                }
                if (!safeDst) return;
                safeDst->value = val;
                safeDst->showValue = true;
                safeDst->update();
            });
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        } else {
            src->color = QColor(255, 170, 0);
            src->update();
            dst->value = val;
            dst->showValue = true;
            dst->update();
        }
    } else if (step.type == ActionType::SQ_POINTER_MOVE) {
        PointerBlock* ptr = (step.u == 0) ? m_ptrI : m_ptrJ;
        if (ptr) {
            if (ptr->opacity() < 1) ptr->setOpacity(1);
            ptr->setProperty("logical_pos", step.v);
            playPointersSync({m_ptrI, m_ptrJ}, m_startX);
        }
    } else if (step.type == ActionType::AR_COMPARE_SHIFT) {
        ArrayBlock* b = m_blocks[step.u];
        if (b) {
            playScaleAnim(b);
            playCellRipple(b, QColor(255, 190, 90), 1.6, 460);
        }
    } else if (step.type == ActionType::AR_SWAP) {
        ArrayBlock* b1 = m_blocks[step.u];
        ArrayBlock* b2 = m_blocks[step.v];
        playSwapAnim(b1, b2, step.val1, step.val2);
        if (b1 && b2) {
            playCellLanding(b1, QColor(255, 170, 0), 340);
            playCellLanding(b2, QColor(255, 170, 0), 340);
        }
    } else if (step.type == ActionType::AR_DONE) {
        if (m_blocks.count(step.u)) {
            m_blocks[step.u]->color = QColor(150, 255, 150);
            m_blocks[step.u]->setOpacity(1.0);
            m_blocks[step.u]->update();
            playScaleAnim(m_blocks[step.u]);
            playCellRipple(m_blocks[step.u], QColor(150, 255, 150), 1.9, 560);
        }
    }
}

void QuickSortModule::reset() {
    m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_ptrJ = nullptr; m_pivotBlock = nullptr;
}
void QuickSortModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_pivotBlock = nullptr;
    m_mathText = nullptr;
}