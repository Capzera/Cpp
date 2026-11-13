#include "MergeSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QVariantAnimation>

MergeSortModule::MergeSortModule() {}

QString MergeSortModule::getModuleName() const {
    return "归并排序";
}

QString MergeSortModule::getDefaultInput() const {
    return "8\n3 5 1 9 2 7 4 6";
}
QString MergeSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组";
}

void MergeSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_data.clear();
    m_blocks.clear();
    m_auxBlocks.clear();
    m_frames.clear();
    QStringList items = inputData.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (items.isEmpty()) return;
    int n = items[0].toInt();
    for (int i = 1; i <= n && i < items.size(); ++i) {
        m_data.push_back(items[i].toInt());
    }
    n = m_data.size();
    m_startX = 400 - (n * 60) / 2;
    for (int i = 0; i < n; ++i) {
        ArrayBlock* block = new ArrayBlock(i, m_data[i]);
        m_scene->addItem(block);
        block->setPos(m_startX + i * 60, 200);
        m_blocks[i] = block;
        registerArraySlot(block, "a", i);
    }
    for (int i = 0; i < n; ++i) {
        ArrayBlock* auxBlock = new ArrayBlock(i, 0);
        m_scene->addItem(auxBlock);
        auxBlock->setPos(m_startX + i * 60, 400);
        m_auxBlocks[i] = auxBlock;
    }
    m_mathText = m_scene->addText("");
    m_mathText->setPos(100, 50);
    m_mathText->setFont(QFont("Consolas", 12, QFont::Bold));
}

std::vector<AnimFrame> MergeSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;
    mergeSort(arr, 0, n - 1, 0);
    return m_frames;
}

void MergeSortModule::mergeSort(std::vector<int>& arr, int l, int r, int depth) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    m_frames.push_back({ActionType::AR_HIGHLIGHT, l, r, depth, 0, {},
                        QString("递归层 %1: 分治 [%2, %3]").arg(depth).arg(l).arg(r),
                        QString("mergeSort(%1, %2)").arg(l).arg(r)});
    mergeSort(arr, l, mid, depth + 1);
    mergeSort(arr, mid + 1, r, depth + 1);
    merge(arr, l, mid, r);
    m_frames.push_back({ActionType::AR_RESET, l, r, 0, 0, {},
                        QString("递归层 %1: 合并完成 [%2, %3]").arg(depth).arg(l).arg(r)});
}

void MergeSortModule::merge(std::vector<int>& arr, int l, int mid, int r) {
    std::vector<int> aux(arr.begin() + l, arr.begin() + r + 1);
    m_frames.push_back({ActionType::AR_SCAN_MIN, l, r, 0, 0, {},
                        QString("复制子数组到辅助数组 [%1, %2]").arg(l).arg(r)});
    int i = 0, j = mid - l + 1, k = l;
    while (i <= mid - l && j <= r - l) {
        m_frames.push_back({ActionType::AR_COMPARE_SHIFT, l + i, l + j, aux[i], aux[j], {},
                            QString("比较 aux[%1]=%2 与 aux[%3]=%4").arg(i).arg(aux[i]).arg(j).arg(aux[j])});
        if (aux[i] <= aux[j]) {
            m_frames.push_back({ActionType::AR_SWAP, k, l + i, 0, 0, {},
                                QString("将较小元素 %1 放到位置 %2").arg(aux[i]).arg(k)});
            arr[k++] = aux[i++];
        } else {
            m_frames.push_back({ActionType::AR_SWAP, k, l + j, 0, 0, {},
                                QString("将较小元素 %1 放到位置 %2").arg(aux[j]).arg(k)});
            arr[k++] = aux[j++];
        }
    }
    while (i <= mid - l) {
        m_frames.push_back({ActionType::AR_EXTRACT, k, l + i, 0, 0, {},
                            QString("复制剩余左侧元素 %1").arg(aux[i])});
        arr[k++] = aux[i++];
    }
    while (j <= r - l) {
        m_frames.push_back({ActionType::AR_EXTRACT, k, l + j, 0, 0, {},
                            QString("复制剩余右侧元素 %1").arg(aux[j])});
        arr[k++] = aux[j++];
    }
    m_frames.push_back({ActionType::AR_DONE, l, r, 0, 0, {},
                        QString("区间 [%1, %2] 合并完成").arg(l).arg(r)});
}

void MergeSortModule::renderFrame(const AnimFrame& frame) {
    if (!m_mathText) return;
    m_mathText->setPlainText(frame.mathText);
    if (frame.type == ActionType::AR_HIGHLIGHT) {
        for (int i = frame.u; i <= frame.v; ++i) {
            if (m_blocks.count(i)) {
                m_blocks[i]->color = QColor(255, 220, 150);
                m_blocks[i]->update();
            }
        }
    } else if (frame.type == ActionType::AR_SCAN_MIN) {
        for (int i = frame.u; i <= frame.v; ++i) {
            if (m_blocks.count(i)) {
                m_blocks[i]->color = QColor(200, 200, 200);
                m_blocks[i]->update();
            }
        }
    } else if (frame.type == ActionType::AR_COMPARE_SHIFT) {
        if (m_blocks.count(frame.u)) {
            m_blocks[frame.u]->color = QColor(150, 200, 255);
            m_blocks[frame.u]->update();
        }
        if (m_blocks.count(frame.v)) {
            m_blocks[frame.v]->color = QColor(150, 200, 255);
            m_blocks[frame.v]->update();
        }
    } else if (frame.type == ActionType::AR_SWAP) {
        if (m_blocks.count(frame.u)) {
            ArrayBlock* b = m_blocks[frame.u];
            b->color = QColor(200, 255, 150);
            b->update();
        }
    } else if (frame.type == ActionType::AR_EXTRACT) {
        if (m_blocks.count(frame.u)) {
            ArrayBlock* b = m_blocks[frame.u];
            b->color = QColor(255, 200, 100);
            b->update();
        }
    } else if (frame.type == ActionType::AR_DONE) {
        for (int i = frame.u; i <= frame.v; ++i) {
            if (m_blocks.count(i)) {
                m_blocks[i]->color = QColor(150, 255, 150);
                m_blocks[i]->update();
            }
        }
    } else if (frame.type == ActionType::AR_RESET) {
        for (int i = frame.u; i <= frame.v; ++i) {
            if (m_blocks.count(i) && m_blocks[i]->color != QColor(150, 255, 150)) {
                m_blocks[i]->color = Qt::white;
                m_blocks[i]->update();
            }
        }
    }
}

void MergeSortModule::reset() {
    m_data.clear();
    m_blocks.clear();
    m_auxBlocks.clear();
    m_frames.clear();
}
void MergeSortModule::releaseSceneItems() {
    m_blocks.clear();
    m_auxBlocks.clear();
    m_mathText = nullptr;
}