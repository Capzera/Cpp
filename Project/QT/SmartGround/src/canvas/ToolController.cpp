#include "ToolController.h"
#include "AnnotationScene.h"
#include "AnnotationItem.h"
#include "StrokeItem.h"
#include "ShapeItem.h"
#include "TextBoxItem.h"
#include "MarkdownItem.h"
#include "LatexItem.h"
#include "CanvasTableItem.h"
#include "SelectionOverlay.h"
#include "CanvasView.h"
#include "render/CartesianElements.h"
#include "core/TextEditGuard.h"
#include "ui/widgets/AnnotationEditor.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSettings>
#include <QJsonDocument>
#include <QFile>
#include <QDialog>
#include <QCursor>
#include <QPainterPath>
#include <cmath>

ToolController::ToolController(AnnotationScene* scene, CanvasView* view, QObject* parent)
    : QObject(parent), m_scene(scene), m_view(view) {
    m_selectionOverlay = new SelectionOverlay(this, this);
    for (int t = Select; t <= Table; ++t) {
        ToolStyle style;
        if (t == Highlighter) {
            style.color = QColor(240, 190, 40);
            style.width = 9.0;
            style.opacity = 1.0;
        } else if (t == Eraser) {
            style.color = QColor(31, 39, 51);
            style.width = 3.0;
        } else if (t == Mosaic) {
            style.color = QColor(31, 39, 51);
            style.width = 3.0;
            style.mosaicCell = 14;
        }
        m_styles.insert(t, style);
    }
    m_rightClickErase = rightClickErasePreference();
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
    if (m_editingItem) endInlineEdit(true);
    if (m_tool == AnnotationSelect && tool != AnnotationSelect) {
        if (m_activeTable) {
            m_activeTable = nullptr;
            emit tableActivated(nullptr);
        }
    }
    m_tool = tool;
    m_eraserActive = (tool == Eraser);
    if (m_cartesianEngine) {
        if (tool == CartPoint) m_cartesianEngine->setActiveTool(CartesianEngine::Point);
        else if (tool == CartLine) m_cartesianEngine->setActiveTool(CartesianEngine::Line);
        else m_cartesianEngine->setActiveTool(CartesianEngine::Select);
    }
    if (m_view && m_view->toolController() == this) {
        m_view->setDragMode(m_tool == Select ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);
        m_view->viewport()->setCursor(m_tool == Select ? Qt::OpenHandCursor : Qt::CrossCursor);
    }
    emit toolChanged(m_tool);
}

ToolController::ToolStyle ToolController::styleFor(Tool tool) const {
    return m_styles.value(tool);
}

void ToolController::setStyleFor(Tool tool, const ToolStyle& style) {
    m_styles.insert(tool, style);
    emit styleChanged();
    if (tool == m_tool) {
        applySelectionStyle();
        applyActiveStyle();
    }
}

void ToolController::setEraserRadius(qreal r) {
    ToolStyle style = m_styles.value(m_tool);
    style.eraserRadius = qMax(2.0, r);
    m_styles.insert(m_tool, style);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::setEraserSquare(bool on) {
    ToolStyle style = m_styles.value(m_tool);
    style.eraserSquare = on;
    m_styles.insert(m_tool, style);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::setShapeFilled(bool on) {
    ToolStyle style = m_styles.value(m_tool);
    style.filled = on;
    m_styles.insert(m_tool, style);
    emit styleChanged();
}

void ToolController::setRightClickEraseEnabled(bool on) {
    m_rightClickErase = on;
    setRightClickErasePreference(on);
    emit styleChanged();
}

bool ToolController::rightClickErasePreference() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("SmartGround"), QStringLiteral("SmartGround"));
    return settings.value("canvas/rightClickErase", true).toBool();
}

void ToolController::setRightClickErasePreference(bool on) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("SmartGround"), QStringLiteral("SmartGround"));
    settings.setValue("canvas/rightClickErase", on);
    settings.sync();
}

void ToolController::setMosaicCellSize(int px) {
    ToolStyle style = m_styles.value(m_tool);
    style.mosaicCell = qBound(4, px, 60);
    m_styles.insert(m_tool, style);
}

void ToolController::setTextSize(int px) {
    ToolStyle style = m_styles.value(m_tool);
    style.textSize = qBound(7, px, 72);
    m_styles.insert(m_tool, style);
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
    ToolStyle style = m_styles.value(m_tool);
    if (style.color != c) {
        style.color = c;
        m_styles.insert(m_tool, style);
        emit styleChanged();
    }
    applySelectionStyle();
    applyActiveStyle();
}

void ToolController::setPenWidth(qreal w) {
    ToolStyle style = m_styles.value(m_tool);
    if (!qFuzzyCompare(style.width, w)) {
        style.width = w;
        m_styles.insert(m_tool, style);
        emit styleChanged();
    }
    applySelectionStyle();
    applyActiveStyle();
}

void ToolController::setOpacity(qreal v) {
    ToolStyle style = m_styles.value(m_tool);
    if (!qFuzzyCompare(style.opacity, v)) {
        style.opacity = v;
        m_styles.insert(m_tool, style);
        emit styleChanged();
    }
    applySelectionStyle();
    applyActiveStyle();
}

void ToolController::setTableSize(int rows, int cols) {
    ToolStyle style = m_styles.value(m_tool);
    style.tableRows = qMax(1, rows);
    style.tableCols = qMax(1, cols);
    m_styles.insert(m_tool, style);
}

void ToolController::setLatexSource(const QString& src) {
    m_latexSource = src;
}

bool ToolController::copySelection() {
    if (m_selected.isEmpty()) return false;
    m_clipboardItems = m_selected;
    QRectF bounds;
    for (AnnotationItem* item : m_clipboardItems) {
        const QRectF r = item->mapRectToScene(item->contentBounds());
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
    m_clipboardAnchor = bounds.isNull() ? QPointF() : bounds.topLeft();
    emit statusMessage(QString("已复制 %1 个批注元素").arg(m_clipboardItems.size()));
    return true;
}

bool ToolController::cutSelection() {
    if (m_selected.isEmpty()) return false;
    if (!copySelection()) return false;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = m_selected;
    for (AnnotationItem* item : m_selected) {
        m_scene->removeItem(item, false);
    }
    m_selected.clear();
    refreshSelectionOverlay();
    emit selectionChanged(0);
    pushCommand(cmd);
    emit statusMessage(QString("已剪切 %1 个批注元素").arg(cmd.items.size()));
    return true;
}

bool ToolController::pasteAt(const QPointF& scenePos) {
    if (m_clipboardItems.isEmpty()) return false;
    QRectF bounds;
    for (AnnotationItem* item : m_clipboardItems) {
        const QRectF r = item->mapRectToScene(item->contentBounds());
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
    const QPointF sourceAnchor = bounds.isNull() ? QPointF() : bounds.topLeft();
    const QPointF delta = scenePos - sourceAnchor;

    QVector<AnnotationItem*> created;
    for (AnnotationItem* source : m_clipboardItems) {
        AnnotationItem* clone = source->cloneItem();
        if (!clone) continue;
        clone->setPos(source->pos() + delta);
        m_scene->addItem(clone);
        created.push_back(clone);
    }
    if (created.isEmpty()) return false;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items = created;
    pushCommand(cmd);
    deselectAll();
    for (AnnotationItem* item : created) {
        setSelectedFlag(item, true);
        m_selected.push_back(item);
    }
    refreshSelectionOverlay();
    emit selectionChanged(m_selected.size());
    emit statusMessage(QString("已粘贴 %1 个批注元素").arg(created.size()));
    return true;
}

bool ToolController::pasteAtCursor() {
    if (!m_view) return false;
    const QPoint local = m_view->viewport()->mapFromGlobal(QCursor::pos());
    return pasteAt(m_view->mapToScene(local));
}

void ToolController::clearClipboard() {
    m_clipboardItems.clear();
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
    refreshSelectionOverlay();
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
    refreshSelectionOverlay();
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
    deselectAll();
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
        return isDrawingTool();    case QEvent::MouseMove:
        onMouseMove(static_cast<QMouseEvent*>(event));
        return isDrawingTool();
    case QEvent::MouseButtonRelease:
        onMouseRelease(static_cast<QMouseEvent*>(event));
        return isDrawingTool();
    case QEvent::MouseButtonDblClick: {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        const QPointF pos = m_view->mapToSceneF(mouseEvent->position());
        if (m_cartesianEngine && m_cartesianEngine->isEnabled()) {
            cartesianDoubleClick(pos);
            event->accept();
            return true;
        }
        const AnnotationItem* hit = itemAt(pos);
        if (hit) {
            AnnotationItem* item = const_cast<AnnotationItem*>(hit);
            if (item->supportsInlineEdit()) {
                if (item->isInlineEditing()) {
                    endInlineEdit(true);
                } else {
                    beginInlineEdit(item);
                }
                if (m_view) m_view->refreshOverlay();
                event->accept();
                return true;
            }
            openEditorFor(item);
            event->accept();
            return true;
        }
        return false;
    }
    case QEvent::KeyPress: {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        return handleKeyPress(key);
    }
    case QEvent::Wheel: {
        QWheelEvent* wheel = static_cast<QWheelEvent*>(event);
        const int delta = wheel->angleDelta().y();
        if (delta == 0) return false;
        if (m_tool == Eraser) {
            setEraserRadius(eraserRadius() + (delta > 0 ? 2.0 : -2.0));
            return true;
        }
        if (m_tool == Pen || m_tool == Highlighter) {
            setPenWidth(qBound(1.0, penWidth() + (delta > 0 ? 0.5 : -0.5), 40.0));
            emit statusMessage(QString("画笔粗细 %1").arg(penWidth(), 0, 'f', 1));
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
    const QPointF pos = m_view->mapToSceneF(event->position());
    m_modifiers = event->modifiers();

    if (event->button() == Qt::RightButton) {
        const bool rightErase = m_rightClickErase
            && (m_tool == Pen || m_tool == Highlighter);
        if (rightErase) {
            beginRightErase(pos);
            event->accept();
            return;
        }
        auto* table = dynamic_cast<CanvasTableItem*>(itemAt(pos));
        if (table) {
            const QPointF local = table->mapFromScene(pos);
            const qreal cell = table->cellSize();
            const int col = static_cast<int>(std::floor(local.x() / cell));
            const int row = static_cast<int>(std::floor((local.y() - table->headerHeight()) / cell));
            const bool valid = (row >= 0 && row < table->rows() && col >= 0 && col < table->cols());
            if (m_activeTable != table) {
                m_activeTable = table;
                emit tableActivated(table);
            }
            emit tableContextRequested(table, valid ? row : -1, valid ? col : -1,
                                       event->globalPosition().toPoint());
            event->accept();
            return;
        }
        if (m_activeTable) {
            m_activeTable = nullptr;
            emit tableActivated(nullptr);
        }
    }

    if (m_editingItem) {
        const AnnotationItem* hit = itemAt(pos);
        if (!hit || hit != m_editingItem) {
            endInlineEdit(true);
        } else {
            if (m_editingItem->placeCaretAt(pos)) {
                if (m_view) m_view->refreshOverlay();
                event->accept();
                return;
            }
        }
    }

    if (m_tool == AnnotationSelect) {
        const qreal tol = 9.0 / qMax(0.2, m_view->transform().m11());
        if (!m_selected.isEmpty()) {
            const SelectionOverlay::Handle handle = m_selectionOverlay->handleAt(pos, tol);
            if (handle != SelectionOverlay::None) {
                m_selectionOverlay->beginScale(pos, handle);
                event->accept();
                return;
            }
        }

        auto* table = dynamic_cast<CanvasTableItem*>(const_cast<AnnotationItem*>(itemAt(pos)));
        if (table) {
            const QPointF local = table->mapFromScene(pos);
            const qreal cell = table->cellSize();
            const int col = static_cast<int>(std::floor(local.x() / cell));
            const int row = static_cast<int>(std::floor((local.y() - table->headerHeight()) / cell));
            const bool inGrid = (row >= 0 && row < table->rows() && col >= 0 && col < table->cols());
            const qreal edge = qMax(6.0, cell * 0.16);
            const QRectF area = table->mapRectToScene(table->contentBounds());
            const bool onBorder = area.adjusted(-edge, -edge, edge, edge).contains(pos)
                && !area.adjusted(edge, edge, -edge, -edge).contains(pos);
            if (!inGrid || onBorder) {
                if (!m_selected.contains(table)) {
                    if (!event->modifiers().testFlag(Qt::ShiftModifier)) deselectAll();
                    setSelectedFlag(table, true);
                    m_selected.push_back(table);
                    emit selectionChanged(m_selected.size());
                    refreshSelectionOverlay();
                }
                beginMoveSelection(pos);
                event->accept();
                return;
            }
            handleTableClick(pos);
            event->accept();
            return;
        }

        const AnnotationItem* hit = itemAt(pos);
        if (hit) {
            AnnotationItem* item = const_cast<AnnotationItem*>(hit);
            if (item->supportsInlineEdit() && !item->isLocked() && item->prefersCaretPlacement(pos)) {
                beginInlineEdit(item);
                if (item->placeCaretAt(pos)) {
                    if (m_view) m_view->refreshOverlay();
                    event->accept();
                    return;
                }
            }
            if (!m_selected.contains(item)) {
                if (!event->modifiers().testFlag(Qt::ShiftModifier)) deselectAll();
                setSelectedFlag(item, true);
                m_selected.push_back(item);
                emit selectionChanged(m_selected.size());
                refreshSelectionOverlay();
            }
            beginMoveSelection(pos);
        } else if (pointInsideSelection(pos)) {
            beginMoveSelection(pos);
        } else {
            const AnnotationItem* borderHit = borderItemAt(pos);
            if (borderHit) {
                AnnotationItem* item = const_cast<AnnotationItem*>(borderHit);
                if (!m_selected.contains(item)) {
                    if (!event->modifiers().testFlag(Qt::ShiftModifier)) deselectAll();
                    setSelectedFlag(item, true);
                    m_selected.push_back(item);
                    emit selectionChanged(m_selected.size());
                    refreshSelectionOverlay();
                }
                beginMoveSelection(pos);
            } else if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
                deselectAll();
                beginMarquee(pos);
            } else {
                beginMarquee(pos);
            }
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
    case CartPoint:
        beginCartesianPoint(pos);
        break;
    case CartLine:
        beginCartesianLink(pos);
        break;
    default:
        break;
    }
    event->accept();
}

void ToolController::setCartesianEngine(CartesianEngine* engine) {
    if (m_cartesianEngine == engine) return;
    m_cartesianEngine = engine;
    m_cartesianPending = nullptr;
    m_cartesianLinkStart = nullptr;
}

void ToolController::beginCartesianPoint(const QPointF& pos) {
    if (!m_cartesianEngine) return;

    CartesianPointItem* existing = m_cartesianEngine->pointAtScene(pos);
    if (existing) {
        m_cartesianPending = existing;
        m_cartesianDragging = true;
        m_cartesianPressPos = pos;
        m_cartesianEngine->setDraggingPoint(existing);
        return;
    }

    const QPointF snapped = m_cartesianEngine->snapScene(pos);
    const QPointF math = m_cartesianEngine->toMath(snapped);
    const int index = m_cartesianEngine->addPoint(math.x(), math.y(), QString());
    if (index < 0) return;
    CartesianPointItem* created = m_cartesianEngine->pointAt(index);
    if (!created) return;
    m_cartesianPending = created;
    m_cartesianDragging = true;
    m_cartesianPressPos = pos;
    m_cartesianEngine->setDraggingPoint(created);
    emit statusMessage(QString("已描点 %1（%2, %3）")
                           .arg(created->name())
                           .arg(CartesianExpression::formatNumber(created->x()))
                           .arg(CartesianExpression::formatNumber(created->y())));
}

void ToolController::updateCartesianPoint(const QPointF& pos) {
    if (!m_cartesianEngine || !m_cartesianPending) return;
    const QPointF snapped = m_cartesianEngine->snapScene(pos);
    const QPointF math = m_cartesianEngine->toMath(snapped);
    m_cartesianPending->setMathPos(math.x(), math.y());
    emit statusMessage(QString("%1（%2, %3）")
                           .arg(m_cartesianPending->name(),
                                CartesianExpression::formatNumber(math.x()),
                                CartesianExpression::formatNumber(math.y())));
}

void ToolController::finishCartesianPoint() {
    if (!m_cartesianEngine) return;
    if (m_cartesianPending) {
        m_cartesianPending->setHighlighted(false);
        m_cartesianPending = nullptr;
    }
    m_cartesianEngine->setDraggingPoint(nullptr);
    m_cartesianDragging = false;
}

void ToolController::beginCartesianLink(const QPointF& pos) {
    if (!m_cartesianEngine) return;

    CartesianPointItem* target = m_cartesianEngine->pointAtScene(pos);
    if (!target) {
        emit statusMessage("连线工具：请在已有的点上单击选择端点");
        return;
    }

    CartesianPointItem* start = m_cartesianEngine->linkStart();
    if (!start) {
        m_cartesianEngine->setLinkStart(target);
        emit statusMessage(QString("已选择起点 %1，请再点一个点连成线段").arg(target->name()));
        return;
    }

    if (start == target) {
        m_cartesianEngine->setLinkStart(nullptr);
        emit statusMessage("已取消起点选择");
        return;
    }

    CartesianEngine* engine = m_cartesianEngine;
    engine->setLinkStart(nullptr);

    const int index = engine->linkPoints(start, target);
    if (index < 0) {
        emit statusMessage("线段创建失败");
        return;
    }
    emit statusMessage(QString("已连接 %1 与 %2，双击线段可查看一次方程")
                           .arg(start->name(), target->name()));
}

void ToolController::cartesianDoubleClick(const QPointF& pos) {
    if (!m_cartesianEngine) return;

    CartesianPointItem* point = m_cartesianEngine->pointAtScene(pos);
    if (point) {
        emit m_cartesianEngine->pointDoubleClicked(point->index());
        return;
    }

    for (CartesianSegmentItem* segment : m_cartesianEngine->segments()) {
        if (segment->hitTest(pos)) {
            emit m_cartesianEngine->segmentDoubleClicked(segment->index());
            return;
        }
    }

    for (CartesianCurveItem* curve : m_cartesianEngine->curves()) {
        if (curve->hitTest(pos)) {
            emit m_cartesianEngine->curveDoubleClicked(curve->index());
            return;
        }
    }
}

void ToolController::onMouseMove(QMouseEvent* event) {
    if (m_rightEraseDragging) {
        const QPointF pos = m_view->mapToSceneF(event->position());
        m_eraserCursorPos = pos;
        updateRightErase(pos);
        event->accept();
        return;
    }
    if (m_tool == Eraser) {
        const QPointF pos = m_view->mapToSceneF(event->position());
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
        const QPointF pos = m_view->mapToSceneF(event->position());
        if (m_selectionOverlay->isScaling()) {
            m_selectionOverlay->updateScale(pos);
            if (m_view) m_view->refreshOverlay();
            event->accept();
            return;
        }
        if (m_marqueeActive) {
            updateMarquee(pos);
        } else if (m_drawing && !m_selected.isEmpty()) {
            updateMoveSelection(pos);
        }
        event->accept();
        return;
    }
    if (m_tool == CartPoint) {
        if (m_cartesianDragging) {
            const QPointF pos = m_view->mapToSceneF(event->position());
            updateCartesianPoint(pos);
            event->accept();
            return;
        }
        if (m_view) m_view->refreshOverlay();
        event->accept();
        return;
    }
    if (m_tool == CartLine) {
        event->accept();
        return;
    }
    if (!m_drawing) return;
    m_modifiers = event->modifiers();
    const QPointF pos = m_view->mapToSceneF(event->position());
    m_currentPos = pos;
    if (m_activeStroke) updateStroke(pos);
    if (m_activeShape && m_tool != Mosaic) updateShape(pos);
    if (m_tool == Mosaic) updateMosaic(pos);
    event->accept();
}

void ToolController::onMouseRelease(QMouseEvent* event) {
    if (m_rightEraseDragging) {
        finishRightErase();
        event->accept();
        return;
    }
    if (m_tool == Eraser) {
        if (m_eraseDragging) finishEraseDrag();
        event->accept();
        return;
    }
    if (m_tool == AnnotationSelect) {
        if (m_selectionOverlay->isScaling()) {
            m_selectionOverlay->finishScale();
            if (m_view) m_view->refreshOverlay();
        } else if (m_marqueeActive) {
            finishMarquee();
        } else if (m_drawing && !m_selected.isEmpty()) {
            finishMoveSelection();
        }
        event->accept();
        return;
    }
    if (m_tool == CartPoint) {
        finishCartesianPoint();
        event->accept();
        return;
    }
    if (m_tool == CartLine) {
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

bool ToolController::handleKeyPress(QKeyEvent* event) {
    if (!event) return false;
    return onKeyPress(event);
}

bool ToolController::onKeyPress(QKeyEvent* event) {
    if (m_editingItem) {
        if (!m_editingItem->isInlineEditing()) {
            m_editingItem = nullptr;
        } else {
            m_editingItem->handleEditKey(event);
            event->accept();
            return true;
        }
    }

    const bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
    if (ctrl) {
        if (event->key() == Qt::Key_Z) {
            if (event->modifiers().testFlag(Qt::ShiftModifier)) redo();
            else undo();
            event->accept();
            return true;
        }
        if (event->key() == Qt::Key_Y) {
            redo();
            event->accept();
            return true;
        }
        if (event->key() == Qt::Key_C) {
            copySelection();
            event->accept();
            return true;
        }
        if (event->key() == Qt::Key_X) {
            cutSelection();
            event->accept();
            return true;
        }
        if (event->key() == Qt::Key_V) {
            pasteAtCursor();
            event->accept();
            return true;
        }
        if (event->key() == Qt::Key_A) {
            selectAll();
            event->accept();
            return true;
        }
        return false;
    }

    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        if (!m_selected.isEmpty()) {
            deleteSelected();
            event->accept();
            return true;
        }
    }

    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        return false;
    }

    if (applyColorShortcut(event)) {
        event->accept();
        return true;
    }
    return false;
}

bool ToolController::applyColorShortcut(QKeyEvent* event) {
    if (!event) return false;
    if (isInlineEditing()) return false;
    if (TextEditGuard::active(m_scene)) return false;
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
    m_activeStroke->setPenColor(penColor());
    m_activeStroke->setPenWidth(penWidth());
    m_activeStroke->setFillOpacity(opacity());
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
    m_activeShape->setPenColor(penColor());
    m_activeShape->setPenWidth(penWidth());
    m_activeShape->setFillOpacity(opacity());
    m_activeShape->setFilled(shapeFilled());
    m_activeShape->setPos(0, 0);
    m_activeShape->setGeometry(pos, pos);
    m_scene->addItem(m_activeShape);
    m_drawing = true;
    m_pressPos = pos;
    m_currentPos = pos;
    if (m_view) m_view->refreshOverlay();
}

void ToolController::updateShape(const QPointF& pos) {
    if (!m_activeShape) return;
    m_activeShape->setGeometry(m_pressPos, constrainPoint(pos, m_pressPos));
    if (m_view) m_view->refreshOverlay();
}

void ToolController::finishShape() {
    if (!m_activeShape) return;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(m_activeShape);
    m_activeShape = nullptr;
    m_drawing = false;
    pushCommand(cmd);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::beginMosaic(const QPointF& pos) {
    m_activeShape = new ShapeItem(ShapeItem::Mosaic);
    m_activeShape->setPenColor(penColor());
    m_activeShape->setPenWidth(penWidth());
    m_activeShape->setPos(0, 0);
    m_mosaicCells.clear();
    m_activeShape->setGeometry(pos, pos);
    m_scene->addItem(m_activeShape);
    m_drawing = true;
    m_pressPos = pos;
    m_currentPos = pos;
    if (m_view) m_view->refreshOverlay();
}

void ToolController::updateMosaic(const QPointF& pos) {
    if (!m_activeShape) return;
    const qreal cell = static_cast<qreal>(mosaicCellSize());
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
    const QRectF region(x0 * cell, y0 * cell, (x1 - x0 + 1) * cell, (y1 - y0 + 1) * cell);
    m_activeShape->setMosaicCells(m_mosaicCells);
    m_activeShape->setGeometry(region.topLeft(), region.bottomRight());
    captureMosaic(region);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::captureMosaic(const QRectF& region) {
    if (!m_activeShape || !m_view) return;
    QPixmap shot = m_view->captureSceneRegion(region);
    if (shot.isNull()) return;

    const int cell = qMax(2, mosaicCellSize());
    const int cols = qMax(1, shot.width() / cell);
    const int rows = qMax(1, shot.height() / cell);
    const int outW = cols * cell;
    const int outH = rows * cell;

    QImage small(cols, rows, QImage::Format_ARGB32);
    small.fill(Qt::transparent);
    const QImage source = shot.toImage();
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            const QPoint sample(cell * x + cell / 2, cell * y + cell / 2);
            if (sample.x() >= source.width() || sample.y() >= source.height()) continue;
            small.setPixelColor(x, y, source.pixelColor(sample));
        }
    }

    QImage blocky(outW, outH, QImage::Format_ARGB32);
    blocky.fill(Qt::transparent);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            const QColor c = small.pixelColor(x, y);
            for (int dy = 0; dy < cell; ++dy) {
                for (int dx = 0; dx < cell; ++dx) {
                    const int px = x * cell + dx;
                    const int py = y * cell + dy;
                    if (px < outW && py < outH) blocky.setPixelColor(px, py, c);
                }
            }
        }
    }
    m_activeShape->setMosaicImage(blocky, QRectF(region.topLeft(), QSizeF(outW, outH)));
}

void ToolController::finishMosaic() {
    if (!m_activeShape) return;
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(m_activeShape);
    m_activeShape = nullptr;
    m_drawing = false;
    pushCommand(cmd);
    if (m_view) m_view->refreshOverlay();
}

void ToolController::eraseAt(const QPointF& pos) {
    const qreal radius = eraserRadius();
    QVector<AnnotationItem*> hits;
    for (AnnotationItem* item : m_scene->annotationItems()) {
        if (item->isLocked()) continue;
        if (eraserSquare()) {
            const QRectF area(pos.x() - radius, pos.y() - radius, radius * 2.0, radius * 2.0);
            const QPainterPath region = item->sceneShape();
            if (!region.isEmpty() && region.intersects(area)) {
                hits.push_back(item);
                continue;
            }
            const QRectF bounds = item->mapRectToScene(item->boundingRect());
            if (bounds.intersects(area)) hits.push_back(item);
            continue;
        }
        if (item->hitTest(pos, radius)) hits.push_back(item);
    }
    if (hits.isEmpty()) return;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = hits;
    for (AnnotationItem* item : hits) {
        m_scene->removeItem(item, false);
    }
    for (AnnotationItem* item : hits) {
        m_selected.removeAll(item);
    }
    refreshSelectionOverlay();
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
    const qreal step = qMax(2.0, eraserRadius() * 0.4);
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

bool ToolController::beginRightErase(const QPointF& pos) {
    m_rightEraseDragging = true;
    m_eraseLastPos = pos;
    m_eraserCursorPos = pos;
    m_eraserActive = true;
    m_eraseRemoved.clear();
    eraseAt(pos);
    if (m_view) m_view->refreshOverlay();
    return true;
}

void ToolController::updateRightErase(const QPointF& pos) {
    if (!m_rightEraseDragging) return;
    updateEraseDrag(pos);
}

void ToolController::finishRightErase() {
    if (!m_rightEraseDragging) return;
    m_rightEraseDragging = false;
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
    for (AnnotationItem* item : all) {
        if (item->isLocked()) continue;
        if (item->hitTest(pos, 5.0)) return item;
    }
    return nullptr;
}

AnnotationItem* ToolController::borderItemAt(const QPointF& pos) {
    const qreal tol = 7.0 / qMax(0.2, m_view ? m_view->transform().m11() : 1.0);
    const QVector<AnnotationItem*> all = m_scene->annotationItems();
    AnnotationItem* best = nullptr;
    qreal bestArea = 0;
    for (AnnotationItem* item : all) {
        if (item->isLocked()) continue;
        if (m_selected.contains(item)) continue;
        if (!item->borderHitTest(pos, tol)) continue;
        const QRectF area = item->mapRectToScene(item->contentBounds());
        const qreal a = area.width() * area.height();
        if (!best || a < bestArea) {
            best = item;
            bestArea = a;
        }
    }
    return best;
}

bool ToolController::pointInsideSelection(const QPointF& pos) const {
    if (m_selected.isEmpty()) return false;
    QRectF bounds;
    for (AnnotationItem* item : m_selected) {
        if (!item) continue;
        const QRectF r = item->mapRectToScene(item->contentBounds());
        bounds = bounds.isNull() ? r : bounds.united(r);
    }
    return !bounds.isNull() && bounds.contains(pos);
}

void ToolController::setSelectedFlag(AnnotationItem* item, bool on) {
    if (!item) return;
    item->setSelected(on);
    item->update();
}

void ToolController::deselectAll() {
    for (AnnotationItem* item : m_selected) setSelectedFlag(item, false);
    m_selected.clear();
    refreshSelectionOverlay();
    emit selectionChanged(0);
}

void ToolController::refreshSelectionOverlay() {
    if (!m_selectionOverlay) return;
    m_selectionOverlay->setItems(m_selected);
    if (m_view) m_view->refreshOverlay();
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
    refreshSelectionOverlay();
    emit selectionChanged(m_selected.size());
}

void ToolController::deleteSelected() {
    if (m_selected.isEmpty()) return;
    Command cmd;
    cmd.kind = Command::Remove;
    cmd.items = m_selected;
    if (m_editingItem && m_selected.contains(m_editingItem)) endInlineEdit(false);
    for (AnnotationItem* item : m_selected) {
        if (!item) continue;
        if (item == m_activeTable) {
            m_activeTable = nullptr;
            emit tableActivated(nullptr);
        }
        setSelectedFlag(item, false);
        m_scene->removeItem(item, false);
    }
    m_selected.clear();
    refreshSelectionOverlay();
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
    scaleSelectionAbout(bounds.center(), factor, factor);
}

void ToolController::scaleSelectionAbout(const QPointF& scenePivot, qreal sx, qreal sy) {
    if (m_selected.isEmpty()) return;
    for (AnnotationItem* item : m_selected) {
        item->scaleAbout(scenePivot, sx, sy);
    }
    if (m_view) m_view->refreshOverlay();
}

void ToolController::scaleSelectionFramesAbout(const QHash<const AnnotationItem*, AnnotationFrame>& startFrames,
                                                const QPointF& scenePivot, qreal sx, qreal sy) {
    if (m_selected.isEmpty()) return;
    for (AnnotationItem* item : m_selected) {
        const auto it = startFrames.constFind(item);
        if (it == startFrames.constEnd()) {
            item->scaleAbout(scenePivot, sx, sy);
        } else {
            item->scaleFrameAbout(it.value(), scenePivot, sx, sy);
        }
    }
    if (m_view) m_view->refreshOverlay();
}

void ToolController::applyActiveStyle() {
    if (m_activeStroke) {
        m_activeStroke->setPenColor(penColor());
        m_activeStroke->setPenWidth(penWidth());
        m_activeStroke->setFillOpacity(opacity());
    }
    if (m_activeShape) {
        m_activeShape->setPenColor(penColor());
        m_activeShape->setPenWidth(penWidth());
        m_activeShape->setFillOpacity(opacity());
    }
}

void ToolController::applySelectionStyle() {
    if (m_selected.isEmpty()) return;
    for (AnnotationItem* item : m_selected) {
        item->setPenColor(penColor());
        item->setPenWidth(penWidth());
        item->setFillOpacity(opacity());
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
        refreshSelectionOverlay();
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
    m_movePressPos = pos;
    m_moveLastPos = pos;
    m_moveOrigins.clear();
    for (AnnotationItem* item : m_selected) {
        if (item) m_moveOrigins.insert(item, item->pos());
    }
}

void ToolController::updateMoveSelection(const QPointF& pos) {
    const QPointF step = pos - m_moveLastPos;
    if (step.isNull()) return;
    m_moveLastPos = pos;
    const QPointF total = pos - m_movePressPos;
    for (AnnotationItem* item : m_selected) {
        if (!item) continue;
        const QPointF origin = m_moveOrigins.value(item, item->pos());
        item->setPos(origin + total);
    }
    if (m_view) m_view->refreshOverlay();
}

void ToolController::finishMoveSelection() {
    m_drawing = false;
    m_moveOrigins.clear();
    m_movePressPos = QPointF();
    m_moveLastPos = QPointF();
}

void ToolController::openEditorFor(AnnotationItem* item) {
    if (!item || item->isLocked()) return;
    if (auto* box = dynamic_cast<TextBoxItem*>(item)) {
        beginInlineEdit(box);
        emit statusMessage("文本框已进入编辑状态，直接在画布上输入。");
        return;
    }
    if (auto* md = dynamic_cast<MarkdownItem*>(item)) {
        beginInlineEdit(md);
        emit statusMessage("笔记已进入编辑状态，直接在画布上输入 Markdown 源码。");
        return;
    }
    if (auto* latex = dynamic_cast<LatexItem*>(item)) {
        openLatexEditor(latex);
        return;
    }
}

void ToolController::beginInlineEdit(AnnotationItem* item) {
    if (!item || !item->supportsInlineEdit()) return;
    if (m_editingItem && m_editingItem != item) endInlineEdit(true);
    m_editingItem = item;
    item->beginInlineEdit();
    if (m_view) {
        m_view->viewport()->setFocus(Qt::OtherFocusReason);
        m_view->refreshOverlay();
    }
}

void ToolController::endInlineEdit(bool commit) {
    if (!m_editingItem) return;
    AnnotationItem* item = m_editingItem;
    m_editingItem = nullptr;
    if (commit) item->commitInlineEdit();
    else item->cancelInlineEdit();
    if (m_view) m_view->refreshOverlay();
}

void ToolController::openLatexEditor(LatexItem* item) {
    if (!item) return;
    AnnotationEditor* editor = new AnnotationEditor(AnnotationEditor::LatexMode, item->tex(), nullptr);
    editor->setAttribute(Qt::WA_DeleteOnClose);
    editor->setLiveMode(true);
    editor->setWindowTitle("编辑 LaTeX 公式");
    connect(editor, &AnnotationEditor::contentChanged, item, [item](const QString& text) {
        item->setTex(text);
    });
    connect(editor, &QDialog::finished, editor, &QObject::deleteLater);
    editor->show();
    editor->raise();
}

void ToolController::handleTableClick(const QPointF& scenePos) {
    auto* table = dynamic_cast<CanvasTableItem*>(itemAt(scenePos));
    if (!table) return;
    if (m_activeTable != table) {
        m_activeTable = table;
        emit tableActivated(table);
    }
    const QPointF local = table->mapFromScene(scenePos);
    const qreal cell = table->cellSize();
    const int col = static_cast<int>(std::floor(local.x() / cell));
    const int row = static_cast<int>(std::floor((local.y() - table->headerHeight()) / cell));

    if (row < 0 || row >= table->rows() || col < 0 || col >= table->cols()) {
        table->clearCellSelection();
        if (m_view) m_view->refreshOverlay();
        return;
    }

    const int idx = row * table->cols() + col;
    QSet<int> cells = table->selectedCells();
    if (!m_modifiers.testFlag(Qt::ShiftModifier)) cells.clear();
    cells.insert(idx);
    table->selectCells(cells);
    if (m_view) m_view->refreshOverlay();
}

AnnotationItem* ToolController::createItemForTool(const QPointF& pos) {
    AnnotationItem* item = nullptr;
    bool openEditor = false;
    bool latexEditor = false;
    if (m_tool == TextBox) {
        auto* box = new TextBoxItem();
        box->setPenColor(penColor());
        box->setPlainText("");
        item = box;
        openEditor = true;
    } else if (m_tool == Markdown) {
        auto* md = new MarkdownItem();
        md->setPenColor(penColor());
        md->setMarkdown("");
        item = md;
        openEditor = true;
    } else if (m_tool == Latex) {
        auto* latex = new LatexItem();
        latex->setPenColor(penColor());
        latex->setTex(m_latexSource.isEmpty() ? QString("E = mc^2") : m_latexSource);
        item = latex;
        latexEditor = true;
    } else if (m_tool == Table) {
        auto* table = new CanvasTableItem(tableRows(), tableCols());
        table->setPenColor(penColor());
        item = table;
    }
    if (!item) return nullptr;
    item->setPos(pos);
    m_scene->addItem(item);
    Command cmd;
    cmd.kind = Command::Add;
    cmd.items.push_back(item);
    pushCommand(cmd);

    const bool wasTable = (m_tool == Table);
    setTool(AnnotationSelect);
    if (latexEditor) {
        openLatexEditor(dynamic_cast<LatexItem*>(item));
    } else if (openEditor) {
        beginInlineEdit(item);
    } else if (wasTable) {
        m_activeTable = dynamic_cast<CanvasTableItem*>(item);
        emit tableActivated(m_activeTable);
    }
    return item;
}
