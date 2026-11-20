#include "InsertionSortModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QFont>

InsertionSortModule::InsertionSortModule() : m_mathText(nullptr), m_startX(0), m_ptrI(nullptr), m_ptrJ(nullptr), m_keyBlock(nullptr) {}

QString InsertionSortModule::getModuleName() const { return "插入排序"; }
QString InsertionSortModule::getDefaultInput() const { return "8\n3 5 1 9 2 7 4 6"; }
QString InsertionSortModule::getInputHint() const {
    return "第一行输入一个正整数 n，表示数组长度\n"
           "第二行输入 n 个整数，即待排序的数组";
}

void InsertionSortModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene; m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_ptrJ = nullptr; m_keyBlock = nullptr;

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

    m_keyBlock = new ArrayBlock(1000, 0, "key");
    m_keyBlock->setPos(400, 150);
    m_keyBlock->showValue = false;
    m_scene->addItem(m_keyBlock);
    m_blocks[1000] = m_keyBlock;

    m_ptrI = new PointerBlock("i", QColor(220, 50, 50));
    m_ptrI->setPos(m_startX + 30, 350);
    m_scene->addItem(m_ptrI);

    m_ptrJ = new PointerBlock("j", QColor(50, 150, 220));
    m_ptrJ->setPos(m_startX, 350);
    m_ptrJ->setOpacity(0); // 初始隐藏
    m_scene->addItem(m_ptrJ);
}

std::vector<AnimFrame> InsertionSortModule::runAlgorithm() {
    m_frames.clear();
    int n = m_data.size();
    for (int i = 0; i < n; ++i) m_data[i] = liveValue("a", i, m_data[i]);
    std::vector<int> arr = m_data;

    m_frames.push_back({ActionType::AR_RESET, -1, 0, 0, 0, {}, "初始化序列"});

    m_frames.push_back({ActionType::AR_DONE, 0, 0, 0, 0, {}, "第一个元素默认在有序区"});

    for (int i = 1; i < n; ++i) {
        m_frames.push_back({ActionType::SQ_POINTER_MOVE, 0, i, 0, 0, {}, QString("第 %1 轮，游标 i 指向准备插入的元素").arg(i)});

        int key = arr[i];
        m_frames.push_back({ActionType::AR_EXTRACT, i, 1000, key, 0, {}, QString("抽出 arr[%1] 放入 key，原位置成为空洞").arg(i)});

        int j = i - 1;
        m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, j, 0, 0, {}, QString("游标 j 从 %1 开始向左扫描寻找插入位置").arg(j)});

        while (j >= 0 && arr[j] > key) {
            m_frames.push_back({ActionType::AR_COMPARE_SHIFT, j, j + 1, arr[j], 0, {}, QString("arr[%1]=%2 > key=%3，将元素后移填补空洞").arg(j).arg(arr[j]).arg(key)});
            arr[j + 1] = arr[j];
            j--;
            if (j >= 0) {
                m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, j, 0, 0, {}, QString("j 继续向左移动到 %1").arg(j)});
            }
        }

        if (j >= 0) {
            m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, QString("arr[%1]=%2 <= key=%3，停止扫描").arg(j).arg(arr[j]).arg(key)});
        } else {
            m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "j 已越界，说明 key 是当前最小元素"});
        }

        arr[j + 1] = key;
        m_frames.push_back({ActionType::AR_INSERT_SHIFT, 1000, j + 1, key, 0, {}, QString("将 key=%1 放入最终空洞 arr[%2] 中").arg(key).arg(j + 1)});
        m_frames.push_back({ActionType::SQ_POINTER_MOVE, 1, -1, 0, 0, {}, "本轮结束"});

        for (int k = 0; k <= i; ++k) {
            m_frames.push_back({ActionType::AR_DONE, k, 0, 0, 0, {}, ""});
        }
    }
    m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, "插入排序完成！"});
    return m_frames;
}

void InsertionSortModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) setScriptText(m_mathText, step.mathText);

    if (step.type == ActionType::AR_RESET) {
        if (step.u == -1) { // 全局回滚 (Frame 0)
            for (auto& p : m_blocks) {
                p.second->color = Qt::white;
                if (p.first < 1000) {
                    p.second->value = m_data[p.first];
                    p.second->showValue = true;
                } else {
                    p.second->showValue = false;
                }
                p.second->update();
            }
            if (m_ptrI) m_ptrI->setOpacity(0);
            if (m_ptrJ) m_ptrJ->setOpacity(0);
        } else { // 被 Cursor 遗漏的单轮扫描结束重置
            for (auto& p : m_blocks) {
                if (p.first < 1000 && p.second->color != QColor(150, 255, 150)) {
                    p.second->color = Qt::white;
                    p.second->update();
                }
            }
        }
        return;
    }

    if (step.type == ActionType::SQ_POINTER_MOVE) {
        PointerBlock* ptr = (step.u == 0) ? m_ptrI : m_ptrJ;
        if (ptr) {
            if (step.v < 0) {
                ptr->setOpacity(0);
            } else {
                if (ptr->opacity() < 1) ptr->setOpacity(1);
                QPointF targetPos(m_startX + step.v * 60 + 30, 350);
                playPointerMove(ptr, targetPos);
            }
        }
    } else if (step.type == ActionType::AR_EXTRACT) {
        ArrayBlock* src = m_blocks[step.u];
        ArrayBlock* dst = m_blocks[step.v];
        playExtractAnim(src, dst, step.val1);
    } else if (step.type == ActionType::AR_COMPARE_SHIFT) {
        ArrayBlock* src = m_blocks[step.u];
        ArrayBlock* dst = m_blocks[step.v];
        playShiftAnim(src, dst, step.val1, QColor(150, 255, 150));
    } else if (step.type == ActionType::AR_INSERT_SHIFT) {
        ArrayBlock* src = m_blocks[step.u];
        ArrayBlock* dst = m_blocks[step.v];
        playShiftAnim(src, dst, step.val1, QColor(150, 255, 150), true);
    } else if (step.type == ActionType::AR_DONE) {
        if (m_blocks.count(step.u)) {
            m_blocks[step.u]->color = QColor(150, 255, 150);
            m_blocks[step.u]->update();
            playScalePulse(m_blocks[step.u], 1.20, 300);
            playCellRipple(m_blocks[step.u], QColor(120, 220, 140), 1.15, 560);
        }
    }
}

void InsertionSortModule::reset() {
    m_blocks.clear(); m_frames.clear(); m_data.clear();
    m_ptrI = nullptr; m_ptrJ = nullptr; m_keyBlock = nullptr;
}
void InsertionSortModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrI = nullptr;
    m_ptrJ = nullptr;
    m_keyBlock = nullptr;
    m_mathText = nullptr;
}