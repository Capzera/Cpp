#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H
#include <QObject>
#include <QColor>
#include <QPointF>
#include <QVector>
#include <QSet>
#include <QHash>
#include <QGraphicsItem>
#include <QMouseEvent>

#include "AnnotationItem.h"

class AnnotationScene;
class AnnotationItem;
class StrokeItem;
class ShapeItem;
class TextBoxItem;
class MarkdownItem;
class LatexItem;
class CanvasTableItem;
class CanvasView;
class SelectionOverlay;
class CartesianEngine;
class CartesianPointItem;

class ToolController : public QObject {
    Q_OBJECT
public:
    enum Tool {
        Select,
        AnnotationSelect,
        Pen,
        Highlighter,
        Eraser,
        Line,
        Rect,
        Circle,
        Arrow,
        Triangle,
        Mosaic,
        TextBox,
        Markdown,
        Latex,
        Table,
        CartPoint,
        CartLine
    };
    Q_ENUM(Tool)

    struct ToolStyle {
        QColor color = QColor(31, 39, 51);
        qreal width = 3.0;
        qreal opacity = 1.0;
        qreal eraserRadius = 14.0;
        bool eraserSquare = false;
        int mosaicCell = 14;
        int tableRows = 3;
        int tableCols = 3;
        int textSize = 12;
        bool filled = false;
    };

    ToolController(AnnotationScene* scene, CanvasView* view, QObject* parent = nullptr);
    ~ToolController() override;

    Tool currentTool() const { return m_tool; }
    void setTool(Tool tool);
    bool isDrawingTool() const;

    QColor penColor() const { return m_styles.value(m_tool).color; }
    void setPenColor(const QColor& c);
    qreal penWidth() const { return m_styles.value(m_tool).width; }
    void setPenWidth(qreal w);
    qreal opacity() const { return m_styles.value(m_tool).opacity; }
    void setOpacity(qreal v);
    qreal eraserRadius() const { return m_styles.value(m_tool).eraserRadius; }
    void setEraserRadius(qreal r);
    bool eraserSquare() const { return m_styles.value(m_tool).eraserSquare; }
    void setEraserSquare(bool on);
    bool shapeFilled() const { return m_styles.value(m_tool).filled; }
    void setShapeFilled(bool on);
    bool rightClickEraseEnabled() const { return m_rightClickErase; }
    void setRightClickEraseEnabled(bool on);
    static bool rightClickErasePreference();
    static void setRightClickErasePreference(bool on);
    int mosaicCellSize() const { return m_styles.value(m_tool).mosaicCell; }
    void setMosaicCellSize(int px);
    int tableRows() const { return m_styles.value(m_tool).tableRows; }
    int tableCols() const { return m_styles.value(m_tool).tableCols; }
    void setTableSize(int rows, int cols);
    int textSize() const { return m_styles.value(m_tool).textSize; }
    void setTextSize(int px);

    ToolStyle styleFor(Tool tool) const;
    void setStyleFor(Tool tool, const ToolStyle& style);

    bool undo();
    bool redo();
    bool canUndo() const { return !m_undoStack.isEmpty(); }
    bool canRedo() const { return !m_redoStack.isEmpty(); }

    void clearAll();
    bool exportToFile(const QString& path, QString* error);

    void setLatexSource(const QString& src);
    QString latexSource() const { return m_latexSource; }

    bool hasClipboard() const { return !m_clipboardItems.isEmpty(); }
    bool copySelection();
    bool cutSelection();
    bool pasteAt(const QPointF& scenePos);
    bool pasteAtCursor();
    void clearClipboard();

    QVector<AnnotationItem*> selectedItems() const { return m_selected; }
    QRectF marqueeRect() const;

    bool annotationsVisible() const;
    void setAnnotationsVisible(bool on);
    bool annotationsLocked() const;
    void setAnnotationsLocked(bool on);

    QPointF eraserCursorPos() const { return m_eraserCursorPos; }
    bool eraserCursorActive() const { return m_eraserActive; }

    void selectAll();
    void deselectAll();
    void deleteSelected();
    void scaleSelected(qreal factor);
    void scaleSelectionAbout(const QPointF& scenePivot, qreal sx, qreal sy);
    void scaleSelectionFramesAbout(const QHash<const AnnotationItem*, AnnotationFrame>& startFrames,
                                   const QPointF& scenePivot, qreal sx, qreal sy);

    SelectionOverlay* selectionOverlay() const { return m_selectionOverlay; }
    AnnotationItem* editingItem() const { return m_editingItem; }
    bool isInlineEditing() const { return m_editingItem && m_editingItem->isInlineEditing(); }
    CanvasTableItem* activeTable() const { return m_activeTable; }

    bool handleKeyPress(QKeyEvent* event);

    bool applyColorShortcut(QKeyEvent* event);

    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void toolChanged(Tool tool);
    void styleChanged();
    void historyChanged(bool canUndo, bool canRedo);
    void statusMessage(const QString& text);
    void selectionChanged(int count);
    void tableActivated(CanvasTableItem* table);
    void tableContextRequested(CanvasTableItem* table, int row, int col, const QPoint& globalPos);

private:
    struct Command {
        enum Kind { Add, Remove };
        Kind kind = Add;
        QVector<AnnotationItem*> items;
    };

    void pushCommand(const Command& cmd);
    void onMousePress(QMouseEvent* event);
    void onMouseMove(QMouseEvent* event);
    void onMouseRelease(QMouseEvent* event);
    bool onKeyPress(QKeyEvent* event);

    void beginStroke(const QPointF& pos, bool highlighter = false);
    void updateStroke(const QPointF& pos);
    void finishStroke();
    bool beginRightErase(const QPointF& pos);
    void updateRightErase(const QPointF& pos);
    void finishRightErase();
    void beginShape(const QPointF& pos);
    void updateShape(const QPointF& pos);
    void finishShape();
    void beginMosaic(const QPointF& pos);
    void updateMosaic(const QPointF& pos);
    void finishMosaic();
    void beginCartesianPoint(const QPointF& pos);
    void updateCartesianPoint(const QPointF& pos);
    void finishCartesianPoint();
    void beginCartesianLink(const QPointF& pos);
    void cartesianDoubleClick(const QPointF& pos);
    void eraseAt(const QPointF& pos);
    void beginEraseDrag(const QPointF& pos);
    void updateEraseDrag(const QPointF& pos);
    void finishEraseDrag();

    void beginMarquee(const QPointF& pos);
    void updateMarquee(const QPointF& pos);
    void finishMarquee();
    void updateMarqueeVisual();
    void beginMoveSelection(const QPointF& pos);
    void updateMoveSelection(const QPointF& pos);
    void finishMoveSelection();
    void applySelectionStyle();
    void applyActiveStyle();
    void setSelectedFlag(AnnotationItem* item, bool on);
    AnnotationItem* itemAt(const QPointF& pos);
    AnnotationItem* borderItemAt(const QPointF& pos);
    bool pointInsideSelection(const QPointF& pos) const;
    void captureMosaic(const QRectF& region);
    void openEditorFor(AnnotationItem* item);
    void openLatexEditor(LatexItem* item);
    void beginInlineEdit(AnnotationItem* item);
    void endInlineEdit(bool commit);
    void refreshSelectionOverlay();
    void handleTableClick(const QPointF& scenePos);

public:
    void setSelectedTextFamily(const QString& family);
    void setSelectedTextSize(int pointSize);
    void toggleSelectedTextBold();
    void toggleSelectedTextItalic();
    void toggleSelectedTextUnderline();
    void setSelectedTextColor(const QColor& c);
    bool hasSelectedText() const;

    void setCartesianEngine(CartesianEngine* engine);
    CartesianEngine* cartesianEngine() const { return m_cartesianEngine; }

    QPointF constrainPoint(const QPointF& pos, const QPointF& origin) const;
    AnnotationItem* createItemForTool(const QPointF& pos);

    AnnotationScene* m_scene = nullptr;
    CanvasView* m_view = nullptr;
    Tool m_tool = Select;
    QHash<int, ToolStyle> m_styles;
    QString m_latexSource = "E = mc^2";
    Qt::KeyboardModifiers m_modifiers = Qt::NoModifier;

    QVector<AnnotationItem*> m_clipboardItems;
    QPointF m_clipboardAnchor;

    StrokeItem* m_activeStroke = nullptr;
    ShapeItem* m_activeShape = nullptr;
    QPointF m_pressPos;
    QPointF m_currentPos;
    bool m_drawing = false;
    QVector<QPolygonF> m_mosaicCells;

    CartesianEngine* m_cartesianEngine = nullptr;
    CartesianPointItem* m_cartesianPending = nullptr;
    CartesianPointItem* m_cartesianLinkStart = nullptr;
    QPointF m_cartesianPressPos;
    bool m_cartesianDragging = false;

    QVector<AnnotationItem*> m_selected;
    QPointF m_marqueeStart;
    QPointF m_marqueeEnd;
    QRectF m_marqueeRect;
    bool m_marqueeActive = false;
    QPointF m_movePressPos;
    QPointF m_moveLastPos;
    QHash<const AnnotationItem*, QPointF> m_moveOrigins;

    bool m_eraseDragging = false;
    bool m_rightEraseDragging = false;
    QPointF m_eraseLastPos;
    QVector<AnnotationItem*> m_eraseRemoved;
    QPointF m_eraserCursorPos;
    bool m_eraserActive = false;
    bool m_rightClickErase = true;

    QVector<Command> m_undoStack;
    QVector<Command> m_redoStack;

    SelectionOverlay* m_selectionOverlay = nullptr;
    AnnotationItem* m_editingItem = nullptr;
    CanvasTableItem* m_activeTable = nullptr;
};

#endif
