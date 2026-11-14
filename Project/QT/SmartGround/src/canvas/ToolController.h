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

class AnnotationScene;
class AnnotationItem;
class StrokeItem;
class ShapeItem;
class TextBoxItem;
class MarkdownItem;
class LatexItem;
class CanvasTableItem;
class CanvasView;

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
        Table
    };
    Q_ENUM(Tool)

    ToolController(AnnotationScene* scene, CanvasView* view, QObject* parent = nullptr);
    ~ToolController() override;

    Tool currentTool() const { return m_tool; }
    void setTool(Tool tool);
    bool isDrawingTool() const;

    QColor penColor() const { return m_penColor; }
    void setPenColor(const QColor& c);
    qreal penWidth() const { return m_penWidth; }
    void setPenWidth(qreal w);
    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal v);

    bool undo();
    bool redo();
    bool canUndo() const { return !m_undoStack.isEmpty(); }
    bool canRedo() const { return !m_redoStack.isEmpty(); }

    void clearAll();
    bool exportToFile(const QString& path, QString* error);

    void setTableSize(int rows, int cols);
    void setLatexSource(const QString& src);
    QString latexSource() const { return m_latexSource; }

    QVector<AnnotationItem*> selectedItems() const { return m_selected; }
    QRectF marqueeRect() const;

    bool annotationsVisible() const;
    void setAnnotationsVisible(bool on);
    bool annotationsLocked() const;
    void setAnnotationsLocked(bool on);

    qreal eraserRadius() const { return m_eraserRadius; }
    void setEraserRadius(qreal r);
    QPointF eraserCursorPos() const { return m_eraserCursorPos; }
    bool eraserCursorActive() const { return m_eraserActive; }

    void selectAll();
    void deselectAll();
    void deleteSelected();
    void scaleSelected(qreal factor);

    bool applyColorShortcut(QKeyEvent* event);

    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void toolChanged(Tool tool);
    void styleChanged();
    void historyChanged(bool canUndo, bool canRedo);
    void statusMessage(const QString& text);
    void selectionChanged(int count);

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
    void onKeyPress(QKeyEvent* event);

    void beginStroke(const QPointF& pos, bool highlighter = false);
    void updateStroke(const QPointF& pos);
    void finishStroke();
    void beginShape(const QPointF& pos);
    void updateShape(const QPointF& pos);
    void finishShape();
    void beginMosaic(const QPointF& pos);
    void updateMosaic(const QPointF& pos);
    void finishMosaic();
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
    void setSelectedFlag(AnnotationItem* item, bool on);
    AnnotationItem* itemAt(const QPointF& pos);

public:
    void setSelectedTextFamily(const QString& family);
    void setSelectedTextSize(int pointSize);
    void toggleSelectedTextBold();
    void toggleSelectedTextItalic();
    void toggleSelectedTextUnderline();
    void setSelectedTextColor(const QColor& c);
    bool hasSelectedText() const;

    QPointF constrainPoint(const QPointF& pos, const QPointF& origin) const;
    AnnotationItem* createItemForTool(const QPointF& pos);

    AnnotationScene* m_scene = nullptr;
    CanvasView* m_view = nullptr;
    Tool m_tool = Select;
    QColor m_penColor = QColor(31, 39, 51);
    qreal m_penWidth = 3.0;
    qreal m_opacity = 1.0;
    int m_tableRows = 3;
    int m_tableCols = 3;
    QString m_latexSource = "E = mc^2";
    Qt::KeyboardModifiers m_modifiers = Qt::NoModifier;

    StrokeItem* m_activeStroke = nullptr;
    ShapeItem* m_activeShape = nullptr;
    QPointF m_pressPos;
    QPointF m_currentPos;
    bool m_drawing = false;
    QVector<QPolygonF> m_mosaicCells;

    QVector<AnnotationItem*> m_selected;
    QPointF m_marqueeStart;
    QPointF m_marqueeEnd;
    QRectF m_marqueeRect;
    bool m_marqueeActive = false;
    QPointF m_moveLastPos;
    QHash<const AnnotationItem*, QPointF> m_moveOrigins;

    bool m_eraseDragging = false;
    QPointF m_eraseLastPos;
    QVector<AnnotationItem*> m_eraseRemoved;
    QPointF m_eraserCursorPos;
    bool m_eraserActive = false;
    qreal m_eraserRadius = 14.0;

    QVector<Command> m_undoStack;
    QVector<Command> m_redoStack;
};

#endif
