#include "BinarySearchClosedModule.h"
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFont>
BinarySearchClosedModule::BinarySearchClosedModule() : m_target(0), m_n(0), m_ptrL(nullptr), m_ptrR(nullptr), m_ptrMid(nullptr) {}
QString BinarySearchClosedModule::getModuleName() const {
	return "二分查找 (闭区间)";
}
QString BinarySearchClosedModule::getDefaultInput() const {
    return "10 8\n1 3 5 7 9 11 13 15 17 19";
}
QString BinarySearchClosedModule::getInputHint() const {
    return "第一行输入两个正整数：数组长度 n、待查找的目标值 target\n"
           "第二行输入 n 个整数，且必须按升序排列\n"
           "本模块查找闭区间 [0, n-1]";
}
void BinarySearchClosedModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    m_blocks.clear();
    m_frames.clear();
    m_data.clear();
    m_ptrL = nullptr;
    m_ptrR = nullptr;
    m_ptrMid = nullptr;
    QStringList lines_raw = inputData.split('\n', Qt::SkipEmptyParts);
    if (lines_raw.isEmpty()) return;
    const QStringList lines = lines_raw; // 加上 const 避免 detach

    QStringList header_raw = lines[0].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (header_raw.size() < 2) return;
    const QStringList header = header_raw;

    m_n = header[0].toInt();
    m_target = header[1].toInt();

    if (lines.size() > 1) {
        const QStringList arrStrs = lines[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for (int i = 0; i < m_n && i < arrStrs.size(); ++i) {
            m_data.push_back(arrStrs[i].toInt());
        }
    }
    m_n = m_data.size();
    m_startX = 400 - ((m_n + 1) * 60) / 2;
    QGraphicsTextItem *label = scene->addText("[L, R]  ( L <= R, R = mid - 1 )");
    label->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    label->setPos(m_startX - 120, 250);
    m_mathText = scene->addText("");
    m_mathText->setFont(QFont("Consolas", 18, QFont::Bold));
    m_mathText->setDefaultTextColor(QColor(180, 50, 50));
    m_mathText->setPos(300, 100);
    for (int i = 1; i <= m_n; ++i) {
        ArrayBlock* b = new ArrayBlock(i, m_data[i - 1], QString::number(i));
        b->setPos(m_startX + i * 60, 300);
        m_scene->addItem(b);
        m_blocks[i] = b;
    }
    m_ptrL = new PointerBlock("L", QColor(50, 150, 220));
    m_ptrL->setPos(m_startX + 1 * 60 + 15, 350);
    m_scene->addItem(m_ptrL);
    m_ptrR = new PointerBlock("R", QColor(220, 50, 50));
    m_ptrR->setPos(m_startX + m_n * 60 + 45, 350);
    m_scene->addItem(m_ptrR);
    m_ptrMid = new PointerBlock("mid", QColor(50, 200, 50));
    m_ptrMid->setPos(m_startX + 1 * 60 + 30, 350);
    m_ptrMid->setOpacity(0);
    m_scene->addItem(m_ptrMid);
    m_ptrL->setProperty("slot", 1);
    m_ptrR->setProperty("slot", m_n);
    m_ptrMid->setProperty("slot", 1);
}
std::vector<AnimFrame> BinarySearchClosedModule::runAlgorithm() {
	m_frames.clear();
	int L = 1, R = m_n;
	m_frames.push_back({ActionType::PD_MATH_TEXT, 0, 0, 0, 0, {}, QString("目标值 target = %1").arg(m_target), ""});
	bool found = false;
	while (L <= R) {
		int mid = L + (R - L) / 2;
		m_frames.push_back({ActionType::BS_POINTER_MOVE, 2, mid, 0, 0, {}, QString("计算 mid = %1").arg(mid), QString("mid = L + (R - L) / 2 = %1").arg(mid)});
		m_frames.push_back({ActionType::BS_HIGHLIGHT_MID, mid, 0, 0, 0, {}, QString("比较 a[%1]=%2 与 target=%3").arg(mid).arg(m_data[mid - 1]).arg(m_target), ""});
		if (m_data[mid - 1] == m_target) {
			m_frames.push_back({ActionType::BS_FOUND, mid, 0, 0, 0, {}, "找到目标值！", ""});
			found = true;
			break;
		} else if (m_data[mid - 1] < m_target) {
			m_frames.push_back({ActionType::BS_DISCARD_RANGE, L, mid, 0, 0, {}, QString("a[mid] < target，淘汰左侧 [%1, %2]").arg(L).arg(mid), ""});
			L = mid + 1;
			m_frames.push_back({ActionType::BS_POINTER_MOVE, 0, L, 0, 0, {}, QString("L 指针右移至 %1").arg(L), QString("L = mid + 1 = %1").arg(L)});
		} else {
			m_frames.push_back({ActionType::BS_DISCARD_RANGE, mid, R, 0, 0, {}, QString("a[mid] > target，淘汰右侧 [%1, %2]").arg(mid).arg(R), ""});
			R = mid - 1;
			m_frames.push_back({ActionType::BS_POINTER_MOVE, 1, R, 0, 0, {}, QString("R 指针左移至 %1").arg(R), QString("R = mid - 1 = %1").arg(R)});
		}
	}
	if (!found) m_frames.push_back({ActionType::BS_NOT_FOUND, 0, 0, 0, 0, {}, "未找到目标值，循环结束", ""});
	return m_frames;
}
ArrayBlock* BinarySearchClosedModule::getBlock(int id) {
	return m_blocks.count(id) ? m_blocks[id] : nullptr;
}
void BinarySearchClosedModule::renderFrame(const AnimFrame& step) {
    if (!step.mathText.isEmpty()) m_mathText->setPlainText(step.mathText);
    if (step.type == ActionType::BS_POINTER_MOVE) {
        if (step.u == 0) m_ptrL->setProperty("slot", step.v);
        else if (step.u == 1) m_ptrR->setProperty("slot", step.v);
        else if (step.u == 2) {
            m_ptrMid->setOpacity(1.0);
            m_ptrMid->setProperty("slot", step.v);
        }
        std::vector<PointerBlock*> ptrs;
        std::vector<QGraphicsItem*> anchors;
        for (PointerBlock* p : {m_ptrL, m_ptrR, m_ptrMid}) {
            if (!p || p->opacity() < 0.05) continue;
            int slot = p->property("slot").toInt();
            ArrayBlock* anchor = getBlock(slot);
            if (!anchor) continue;
            ptrs.push_back(p);
            anchors.push_back(anchor);
        }
        playPointerArrange(ptrs, anchors);
    } else if (step.type == ActionType::BS_HIGHLIGHT_MID) {
        ArrayBlock* b = getBlock(step.u);
        if (b) {
            b->color = QColor(255, 255, 100);
            b->update();
            playScalePulse(b, 1.3, 300);
        }
    } else if (step.type == ActionType::BS_DISCARD_RANGE) {
        for (int i = step.u; i <= step.v; ++i) {
            ArrayBlock* b = getBlock(i);
            if (!b) continue;
            b->color = QColor(200, 200, 200);
            b->update();
            if (!m_animated) {
                b->setOpacity(0.3);
                b->setPos(b->pos() + QPointF(0, 30));
                continue;
            }
            QPropertyAnimation* opa = acquireAnim(b, "opacity", 400, QEasingCurve::InOutQuad);
            if (opa) {
                opa->setStartValue(b->opacity());
                opa->setEndValue(0.3);
                opa->start();
            }
            QPropertyAnimation* mv = acquireAnim(b, "pos", 400, QEasingCurve::InOutQuad);
            if (mv) {
                mv->setStartValue(b->pos());
                mv->setEndValue(b->pos() + QPointF(0, 30));
                mv->start();
            }
        }
    } else if (step.type == ActionType::BS_FOUND) {
        ArrayBlock* b = getBlock(step.u);
        if (b) {
            b->color = QColor(100, 255, 100);
            b->update();
            playScalePulse(b, 1.4, 400);
        }
    } else if (step.type == ActionType::BS_NOT_FOUND) {
        if (m_ptrMid) m_ptrMid->setOpacity(0);
    }
}
void BinarySearchClosedModule::reset() {
	m_data.clear();
	m_blocks.clear();
	m_frames.clear();
	m_ptrL = nullptr;
	m_ptrR = nullptr;
	m_ptrMid = nullptr;
}
void BinarySearchClosedModule::releaseSceneItems() {
    m_blocks.clear();
    m_ptrL = nullptr;
    m_ptrR = nullptr;
    m_ptrMid = nullptr;
    m_mathText = nullptr;
}
