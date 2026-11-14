#include "ToolController.h"
#include "AnnotationScene.h"
#include "AnnotationItem.h"
#include "StrokeItem.h"
#include "ShapeItem.h"
#include "TextBoxItem.h"
#include "MarkdownItem.h"
#include "LatexItem.h"
#include "CanvasTableItem.h"
#include "CanvasView.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QFile>
#include <cmath>

ToolController::ToolController(AnnotationScene* scene, CanvasView* view, QObject* parent)
    : QObject(parent), m_scene(scene), m_view(view) {
    if (m_view) m_view->viewport()->installEventFilter(this);
}

ToolController::~ToolController() {
    m_undoStack.clear();
    m_redoStack.clear();
}

void ToolController::setTool(Tool tool) {
    if (m_tool == tool) return;
    if (m_drawing) {
        if (m_tool == Pen || m_tool == Highlighter) finishStroke();
        else if (m_tool == Mosaic) finishMosaic();
        else if (m_tool == Eraser) finishEraseDrag();
        else if (m_activeShape) finishShape();
    }
    m_tool = tool;
    m_eraserActive = (tool == Eraser);
    if (m_view && m_view->toolController() == this) {
        m_view->setDragMode(m_tool == Select ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);
        m_view->viewport()->setCursor(m_tool == Select ? Qt::OpenHandCursor : Qt::CrossCursor);
    }
    emit toolChanged(m_tool);
}

void ToolController::setEraserRadius(qreal r) {
    m_eraserRadius = qMax(2.0, r);
    if (m_view) m_view->refreshOverlay();
}

bool ToolController::isDrawingTool() const {
    switch (m_tool) {
    case AnnotationSelect:
    case Pen:
    case Highlighter:
    case Eraser:
    case Line:
    case Rect:
    case Circle:
    case Arrow:
    case Triangle:
    case Mosaic:
    case TextBox:
    case Markdown:
    case Latex:
    case Table:
        return true;
    default:
        return false;
    }
}

void ToolController::setPenColor(const QColor& c) {
    if (m_penColor == c) return;
    m_penColor = c;
    emit styleChanged();
}

void ToolController::setPenWidth(qreal w) {
    if (qFuzzyCompare(m_penWidth, w)) return;
    m_penWidth = w;
    emit styleChanged();
}

void ToolController::setOpacity(qreal v) {
    if (qFuzzyCompare(m_opacity, v)) return;
    m_opacity = v;
    emit styleChanged();
}

void ToolController::setTableSize(int rows, int cols) {
    m_tableRows = qMax(1, rows);
    m_tableCols = qMax(1, cols);
}

void ToolController::setLatexSource(const QString& src) {
    m_latexSource = src;
}

void ToolController::pushCommand(const Command& cmd) {
    m_undoStack.push_back(cmd);
    while (m_undoStack.size() > 120) {
        m_undoStack.removeFirst();
    }
    m_redoStack.clear();
    emit historyChanged(canUndo(), canRedo());
}
bool ToolController::undo() {
    if (m_undoStack.isEmpty()) return false;
    const Command cmd = m_undoStack.takeLast();
    if (cmd.kind == Command::Add) {
        for (int i = cmd.items.size() - 1; i >= 0; --i) {
            AnnotationItem* item = cmd.items.at(i);
            if (!item) continue;
            if (item->scene()) m_scene->removeItem(item, false);
        }
    } else {
        for (AnnotationItem* item : cmd.items) {
            if (item && !item->scene()) m_scene->addItem(item);
        }
    }
    m_redoStack.push_back(cmd);
    emit historyChanged(canUndo(), canRedo());
    return true;
}

bool ToolController::redo() {
    if (m_redoStack.isEmpty()) return false;
    const Command cmd = m_redoStack.takeLast();
    if (cmd.kind == Command::Add) {
        for (AnnotationItem* item : cmd.items) {
            if (item && !item->scene()) m_scene->addItem(item);
        }
    } else {
        for (int i = cmd.items.size() - 1; i >= 0; --i) {
            AnnotationItem* item = cmd.items.at(i);
            if (!item) continue;
            if (item->scene()) m_scene->removeItem(item, false);
        }
    }
    m_undoStack.push_back(cmd);
    emit historyChanged(canUndo(), canRedo());
    return true;
}

void ToolController::clearAll() {
    const QVector<AnnotationItem*> all = m_scene->annotationItems();
    if (all.isEmpty()) return;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = all;
    for (AnnotationItem* item : all) {
        m_scene->removeItem(item, false);
    }
    pushCommand(cmd);
    emit statusMessage("已清空批注层");
}

bool ToolController::exportToFile(const QString& path, QString* error) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (error) *error = "无法写入文件: " + path;
        return false;
    }
    file.write(QJsonDocument(m_scene->toJson()).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool ToolController::eventFilter(QObject* obj, QEvent* event) {
    if (!m_view || obj != m_view->viewport()) return false;

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        onMousePress(static_cast<QMouseEvent*>(event));
        return isDrawingTool();
    case QEvent::MouseMove:
        onMouseMove(static_cast<QMouseEvent*>(event));
        return isDrawingTool();
    case QEvent::MouseButtonRelease:
        onMouseRelease(static_cast<QMouseEvent*>(event));
        return isDrawingTool();
    case QEvent::KeyPress: {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        onKeyPress(key);
        return key->isAccepted();
    }
    case QEvent::Wheel: {
        QWheelEvent* wheel = static_cast<QWheelEvent*>(event);
        const int delta = wheel->angleDelta().y();
        if (delta == 0) return false;
        if (m_tool == Eraser) {
            setEraserRadius(m_eraserRadius + (delta > 0 ? 2.0 : -2.0));
            return true;
        }
        if (m_tool == Pen || m_tool == Highlighter) {
            setPenWidth(qBound(1.0, m_penWidth + (delta > 0 ? 0.5 : -0.5), 40.0));
            emit statusMessage(QString("画笔粗细 %1").arg(m_penWidth, 0, 'f', 1));
            return true;
        }
        return false;
    }
    default:
        break;
    }
    return false;
}

void ToolController::onMousePress(QMouseEvent* event) {
    if (!m_scene) return;
    const QPointF pos = m_view->mapToScene(event->position().toPoint());
    m_modifiers = event->modifiers();

    if (m_tool == AnnotationSelect) {
        const AnnotationItem* hit = itemAt(pos);
        if (hit) {
            if (!m_selected.contains(const_cast<AnnotationItem*>(hit))) {
                if (!event->modifiers().testFlag(Qt::ShiftModifier)) deselectAll();
                setSelectedFlag(const_cast<AnnotationItem*>(hit), true);
                m_selected.push_back(const_cast<AnnotationItem*>(hit));
                emit selectionChanged(m_selected.size());
            }
            beginMoveSelection(pos);
        } else {
            if (!event->modifiers().testFlag(Qt::ShiftModifier)) deselectAll();
            beginMarquee(pos);
        }
        event->accept();
        return;
    }

    switch (m_tool) {
    case Pen:
        beginStroke(pos);
        break;
    case Highlighter:
        beginStroke(pos, true);
        break;
    case Eraser:
        beginEraseDrag(pos);
        break;
    case Line:
    case Rect:
    case Circle:
    case Arrow:
    case Triangle:
        beginShape(pos);
        break;
    case Mosaic:
        beginMosaic(pos);
        break;
    case TextBox:
    case Markdown:
    case Latex:
    case Table:
        createItemForTool(pos);
        break;
    default:
        break;
    }
    event->accept();
}

void ToolController::onMouseMove(QMouseEvent* event) {
    if (m_tool == Eraser) {
        const QPointF pos = m_view->mapToScene(event->position().toPoint());
        m_eraserCursorPos = pos;
        if (m_eraseDragging) {
            updateEraseDrag(pos);
        } else if (m_view) {
            m_view->refreshOverlay();
        }
        event->accept();
        return;
    }
    if (m_tool == AnnotationSelect) {
        const QPointF pos = m_view->mapToScene(event->position().toPoint());
        if (m_marqueeActive) {
            updateMarquee(pos);
        } else if (m_drawing && !m_selected.isEmpty()) {
            updateMoveSelection(pos);
        }
        event->accept();
        return;
    }
    if (!m_drawing) return;
    m_modifiers = event->modifiers();
    const QPointF pos = m_view->mapToScene(event->position().toPoint());
    m_currentPos = pos;
    if (m_activeStroke) updateStroke(pos);
    if (m_activeShape && m_tool != Mosaic) updateShape(pos);
    if (m_tool == Mosaic) updateMosaic(pos);
    event->accept();
}

void ToolController::onMouseRelease(QMouseEvent* event) {
    if (m_tool == Eraser) {
        if (m_eraseDragging) finishEraseDrag();
        event->accept();
        return;
    }
    if (m_tool == AnnotationSelect) {
        if (m_marqueeActive) {
            finishMarquee();
        } else if (m_drawing && !m_selected.isEmpty()) {
            finishMoveSelection();
        }
        event->accept();
        return;
    }
    if (!m_drawing) return;
    m_modifiers = event->modifiers();
    if (m_tool == Pen || m_tool == Highlighter) finishStroke();
    else if (m_tool == Mosaic) finishMosaic();
    else if (m_activeShape) finishShape();
    event->accept();
}

void ToolController::onKeyPress(QKeyEvent* event) {
    const bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
    if (ctrl) {
        if (event->key() == Qt::Key_Z) {
            if (event->modifiers().testFlag(Qt::ShiftModifier)) redo();
            else undo();
            event->accept();
            return;
        }
        if (event->key() == Qt::Key_Y) {
            redo();
            event->accept();
            return;
        }
        return;
    }

    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        return;
    }

    if (applyColorShortcut(event)) {
        event->accept();
        return;
    }
}

bool ToolController::applyColorShortcut(QKeyEvent* event) {
    QColor color;
    if (event->modifiers().testFlag(Qt::AltModifier)) {
        switch (event->key()) {
        case Qt::Key_R: color = QColor(229, 72, 77); break;
        case Qt::Key_G: color = QColor(46, 168, 106); break;
        case Qt::Key_B: color = QColor(59, 125, 221); break;
        case Qt::Key_Y: color = QColor(240, 190, 40); break;
        case Qt::Key_O: color = QColor(232, 132, 42); break;
        case Qt::Key_P: color = QColor(214, 90, 168); break;
        case Qt::Key_K: color = QColor(31, 39, 51); break;
        default: return false;
        }
    } else {
        switch (event->key()) {
        case Qt::Key_1: color = QColor(229, 72, 77); break;
        case Qt::Key_2: color = QColor(46, 168, 106); break;
        case Qt::Key_3: color = QColor(59, 125, 221); break;
        case Qt::Key_4: color = QColor(240, 190, 40); break;
        case Qt::Key_5: color = QColor(232, 132, 42); break;
        case Qt::Key_6: color = QColor(214, 90, 168); break;
        case Qt::Key_7: color = QColor(31, 39, 51); break;
        default: return false;
        }
    }
    setPenColor(color);
    emit statusMessage(QString("画笔颜色 %1").arg(color.name()));
    return true;
}

QPointF ToolController::constrainPoint(const QPointF& pos, const QPointF& origin) const {
    const bool shift = m_modifiers.testFlag(Qt::ShiftModifier);
    if (!shift) return pos;

    const QPointF delta = pos - origin;
    const qreal len = std::hypot(delta.x(), delta.y());
    if (len < 0.001) return pos;

    switch (m_tool) {
    case Line:
    case Arrow: {
        const qreal angle = std::atan2(delta.y(), delta.x());
        const qreal step = M_PI / 4.0;
        const qreal snapped = std::round(angle / step) * step;
        return origin + QPointF(std::cos(snapped), std::sin(snapped)) * len;
    }
    case Rect:
    case Circle: {
        const qreal side = std::max(std::abs(delta.x()), std::abs(delta.y()));
        return origin + QPointF(delta.x() >= 0 ? side : -side, delta.y() >= 0 ? side : -side);
    }
    case Triangle: {
        const qreal side = std::max(std::abs(delta.x()), std::abs(delta.y()));
        const qreal h = side * std::sqrt(3.0) / 2.0;
        QPointF apex = origin;
        if (delta.y() <= 0) apex = origin + QPointF(0, -h);
        else apex = origin + QPointF(0, h);
        return apex;
    }
    default:
        return pos;
    }
}

void ToolController::beginStroke(const QPointF& pos, bool highlighter) {
    m_activeStroke = new StrokeItem();
    m_activeStroke->setMode(highlighter ? StrokeItem::Highlighter : StrokeItem::Ink);
    m_activeStroke->setPenColor(m_penColor);
    m_activeStroke->setPenWidth(m_penWidth);
    m_activeStroke->setFillOpacity(m_opacity);
    m_activeStroke->setPos(0, 0);
    m_activeStroke->addPoint(pos);
    m_scene->addItem(m_activeStroke);
    m_drawing = true;
    m_pressPos = pos;
    m_currentPos = pos;
}

void ToolController::updateStroke(const QPointF& pos) {
    if (!m_activeStroke) return;
    m_activeStroke->addPoint(pos);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::finishStroke() {
    if (!m_activeStroke) return;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(m_activeStroke);
    m_activeStroke = nullptr;
    m_drawing = false;
    pushCommand(cmd);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::beginShape(const QPointF& pos) {
    ShapeItem::Kind kind = ShapeItem::Line;
    switch (m_tool) {
    case Line: kind = ShapeItem::Line; break;
    case Rect: kind = ShapeItem::Rectangle; break;
    case Circle: kind = ShapeItem::Ellipse; break;
    case Arrow: kind = ShapeItem::Arrow; break;
    case Triangle: kind = ShapeItem::Triangle; break;
    default: break;
    }
    m_activeShape = new ShapeItem(kind);
    m_activeShape->setPenColor(m_penColor);
    m_activeShape->setPenWidth(m_penWidth);
    m_activeShape->setFillOpacity(m_opacity);
    m_activeShape->setPos(0, 0);
    m_activeShape->setGeometry(pos, pos);
    m_scene->addItem(m_activeShape);
    m_drawing = true;
    m_pressPos = pos;
    m_currentPos = pos;
}

void ToolController::updateShape(const QPointF& pos) {
    if (!m_activeShape) return;
    m_activeShape->setGeometry(m_pressPos, constrainPoint(pos, m_pressPos));
}

void ToolController::finishShape() {
    if (!m_activeShape) return;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(m_activeShape);
    m_activeShape = nullptr;
    m_drawing = false;
    pushCommand(cmd);
}

void ToolController::beginMosaic(const QPointF& pos) {
    m_activeShape = new ShapeItem(ShapeItem::Mosaic);
    m_activeShape->setPenColor(m_penColor);
    m_activeShape->setPenWidth(m_penWidth);
    m_activeShape->setPos(0, 0);
    m_mosaicCells.clear();
    m_activeShape->setGeometry(pos, pos);
    m_scene->addItem(m_activeShape);
    m_drawing = true;
    m_pressPos = pos;
    m_currentPos = pos;
}

void ToolController::updateMosaic(const QPointF& pos) {
    if (!m_activeShape) return;
    const qreal cell = 14.0;
    const int x0 = static_cast<int>(std::floor(m_pressPos.x() / cell));
    const int y0 = static_cast<int>(std::floor(m_pressPos.y() / cell));
    const int x1 = static_cast<int>(std::floor(pos.x() / cell));
    const int y1 = static_cast<int>(std::floor(pos.y() / cell));
    m_mosaicCells.clear();
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            QPolygonF poly;
            poly << QPointF(x * cell, y * cell)
                 << QPointF((x + 1) * cell, y * cell)
                 << QPointF((x + 1) * cell, (y + 1) * cell)
                 << QPointF(x * cell, (y + 1) * cell);
            m_mosaicCells.push_back(poly);
        }
    }
    m_activeShape->setMosaicCells(m_mosaicCells);
    m_activeShape->setGeometry(QPointF(x0 * cell, y0 * cell), QPointF((x1 + 1) * cell, (y1 + 1) * cell));
}

void ToolController::finishMosaic() {
    if (!m_activeShape) return;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(m_activeShape);
    m_activeShape = nullptr;
    m_drawing = false;
    pushCommand(cmd);
}

void ToolController::eraseAt(const QPointF& pos) {
    const qreal radius = m_eraserRadius;
    QVector<AnnotationItem*> hits;
    for (AnnotationItem* item : m_scene->annotationItems()) {
        if (item->isLocked()) continue;
        if (item->hitTest(pos, radius)) hits.push_back(item);
    }
    if (hits.isEmpty()) return;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = hits;
    for (AnnotationItem* item : hits) {
        m_scene->removeItem(item, false);
    }
    pushCommand(cmd);
}

void ToolController::beginEraseDrag(const QPointF& pos) {
    m_eraseDragging = true;
    m_eraseLastPos = pos;
    m_eraserCursorPos = pos;
    m_eraserActive = true;
    m_eraseRemoved.clear();
    eraseAt(pos);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::updateEraseDrag(const QPointF& pos) {
    if (!m_eraseDragging) return;
    const QPointF from = m_eraseLastPos;
    const QPointF delta = pos - from;
    const qreal dist = std::hypot(delta.x(), delta.y());
    const qreal step = qMax(2.0, m_eraserRadius * 0.4);
    if (dist >= step) {
        const int steps = static_cast<int>(dist / step);
        for (int i = 1; i <= steps; ++i) {
            const qreal t = (step * i) / dist;
            eraseAt(from + delta * t);
        }
        m_eraseLastPos = from + delta * (step * steps / dist);
    }
    m_eraserCursorPos = pos;
    if (m_view) m_view->refreshOverlay();
}

void ToolController::finishEraseDrag() {
    if (!m_eraseDragging) return;
    m_eraseDragging = false;
    m_eraserActive = false;
    if (m_view) m_view->refreshOverlay();
}

AnnotationItem* ToolController::itemAt(const QPointF& pos) {
    const QVector<AnnotationItem*> all = m_scene->annotationItems();
    for (AnnotationItem* item : all) {
        if (item->isLocked()) continue;
        const QPointF local = item->mapFromScene(pos);
        if (item->shape().contains(local)) return item;
    }
    return nullptr;
}

void ToolController::setSelectedFlag(AnnotationItem* item, bool on) {
    if (!item) return;
    item->setSelected(on);
    item->update();
}

void ToolController::deselectAll() {
    for (AnnotationItem* item : m_selected) setSelectedFlag(item, false);
    m_selected.clear();
    emit selectionChanged(0);
}

bool ToolController::annotationsVisible() const {
    return m_scene && m_scene->annotationsVisible();
}

void ToolController::setAnnotationsVisible(bool on) {
    if (!m_scene) return;
    m_scene->setAnnotationsVisible(on);
    if (m_view) m_view->refreshOverlay();
    emit statusMessage(on ? "已显示批注层。" : "已隐藏批注层，下层算法结构不受影响。");
}

bool ToolController::annotationsLocked() const {
    return m_scene && m_scene->annotationsLocked();
}

void ToolController::setAnnotationsLocked(bool on) {
    if (!m_scene) return;
    m_scene->setAnnotationsLocked(on);
    emit statusMessage(on ? "已锁定批注层，无法移动或删除。" : "已解锁批注层。");
}

void ToolController::selectAll() {
    deselectAll();
    for (AnnotationItem* item : m_scene->annotationItems()) {
        if (item->isLocked()) continue;
        setSelectedFlag(item, true);
        m_selected.push_back(item);
    }
    emit selectionChanged(m_selected.size());
}

void ToolController::deleteSelected() {
    if (m_selected.isEmpty()) return;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = m_selected;
    for (AnnotationItem* item : m_selected) {
        m_scene->removeItem(item, false);
    }
    m_selected.clear();
    emit selectionChanged(0);
    pushCommand(cmd);
    emit statusMessage(QString("已删除 %1 个批注元素").arg(cmd.items.size()));
}

void ToolController::scaleSelected(qreal factor) {
    if (m_selected.isEmpty() || factor <= 0.001) return;
    QRectF bounds;
    for (AnnotationItem* item : m_selected) {
        const QRectF r = item->mapRectToScene(item->contentBounds());
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
    const QPointF pivot = bounds.center();
    for (AnnotationItem* item : m_selected) {
        item->scaleBy(factor, pivot);
    }
    emit statusMessage(QString("已缩放选中的 %1 个批注元素").arg(m_selected.size()));
}

void ToolController::applySelectionStyle() {
    if (m_selected.isEmpty()) return;
    for (AnnotationItem* item : m_selected) {
        item->setPenColor(m_penColor);
        item->setPenWidth(m_penWidth);
        item->setFillOpacity(m_opacity);
    }
    emit statusMessage(QString("已更新选中的 %1 个批注元素样式").arg(m_selected.size()));
}

bool ToolController::hasSelectedText() const {
    for (AnnotationItem* item : m_selected) {
        if (dynamic_cast<TextBoxItem*>(item)) return true;
    }
    return false;
}

void ToolController::setSelectedTextFamily(const QString& family) {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setFontFamily(family);
    }
}

void ToolController::setSelectedTextSize(int pointSize) {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setFontSize(pointSize);
    }
}

void ToolController::toggleSelectedTextBold() {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setBold(!box->isBold());
    }
}

void ToolController::toggleSelectedTextItalic() {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setItalic(!box->isItalic());
    }
}

void ToolController::toggleSelectedTextUnderline() {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setUnderline(!box->isUnderline());
    }
}

void ToolController::setSelectedTextColor(const QColor& c) {
    for (AnnotationItem* item : m_selected) {
        if (auto* box = dynamic_cast<TextBoxItem*>(item)) box->setTextColor(c);
    }
}

void ToolController::beginMarquee(const QPointF& pos) {
    m_marqueeActive = true;
    m_marqueeStart = pos;
    m_marqueeEnd = pos;
    m_marqueeRect = QRectF(pos, pos);
    updateMarqueeVisual();
}

void ToolController::updateMarquee(const QPointF& pos) {
    if (!m_marqueeActive) return;
    m_marqueeEnd = pos;
    m_marqueeRect = QRectF(m_marqueeStart, m_marqueeEnd).normalized();
    updateMarqueeVisual();
}

void ToolController::finishMarquee() {
    if (!m_marqueeActive) return;
    m_marqueeActive = false;
    const QRectF rect = QRectF(m_marqueeStart, m_marqueeEnd).normalized();
    m_marqueeRect = QRectF();
    if (rect.width() > 4.0 && rect.height() > 4.0) {
        for (AnnotationItem* item : m_scene->annotationItems()) {
            if (item->isLocked()) continue;
            const QRectF r = item->mapRectToScene(item->contentBounds());
            if (!rect.intersects(r)) continue;
            if (m_selected.contains(item)) continue;
            setSelectedFlag(item, true);
            m_selected.push_back(item);
        }
        emit selectionChanged(m_selected.size());
    }
    updateMarqueeVisual();
}

void ToolController::updateMarqueeVisual() {
    if (m_view) m_view->refreshOverlay();
}

QRectF ToolController::marqueeRect() const {
    if (!m_marqueeActive) return QRectF();
    return QRectF(m_marqueeStart, m_marqueeEnd).normalized();
}

void ToolController::beginMoveSelection(const QPointF& pos) {
    m_drawing = true;
    m_moveLastPos = pos;
    m_moveOrigins.clear();
    for (AnnotationItem* item : m_selected) {
        m_moveOrigins.insert(item, item->pos());
    }
}

void ToolController::updateMoveSelection(const QPointF& pos) {
    const QPointF delta = pos - m_moveLastPos;
    m_moveLastPos = pos;
    for (AnnotationItem* item : m_selected) {
        const QPointF origin = m_moveOrigins.value(item, item->pos());
        item->setPos(origin + delta);
    }
    if (m_view) m_view->refreshOverlay();
}

void ToolController::finishMoveSelection() {
    m_drawing = false;
    m_moveOrigins.clear();
}

AnnotationItem* ToolController::createItemForTool(const QPointF& pos) {
    AnnotationItem* item = nullptr;
    if (m_tool == TextBox) {
        auto* box = new TextBoxItem();
        box->setPenColor(m_penColor);
        box->setPlainText("双击编辑文字");
        item = box;
    } else if (m_tool == Markdown) {
        auto* md = new MarkdownItem();
        md->setPenColor(m_penColor);
        md->setMarkdown("# 笔记\n\n支持 **Markdown** 语法：\n\n- 列表\n- 代码块\n\n```cpp\nint main() { return 0; }\n```");
        item = md;
    } else if (m_tool == Latex) {
        auto* latex = new LatexItem();
        latex->setPenColor(m_penColor);
        latex->setTex(m_latexSource.isEmpty() ? QString("E = mc^2") : m_latexSource);
        item = latex;
    } else if (m_tool == Table) {
        auto* table = new CanvasTableItem(m_tableRows, m_tableCols);
        table->setPenColor(m_penColor);
        item = table;
    }
    if (!item) return nullptr;
    item->setPos(pos);
    m_scene->addItem(item);
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(item);
    pushCommand(cmd);
    return item;
}
