#ifndef CANVASVIEW_H
#define CANVASVIEW_H
#include <QGraphicsView>
#include <QPoint>

class AnnotationScene;
class ToolController;
class SelectionOverlay;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CanvasView(QGraphicsScene* scene, QWidget* parent = nullptr);

    void setAnnotationScene(AnnotationScene* scene);
    AnnotationScene* annotationScene() const { return m_annotationScene; }

    void setToolController(ToolController* controller);
    ToolController* toolController() const { return m_toolController; }

    void refreshOverlay();

    QPointF mapToSceneF(const QPointF& viewportPos) const;

    QPixmap captureSceneRegion(const QRectF& region) const;

protected:
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;

private:
    void syncDragMode();

    AnnotationScene* m_annotationScene = nullptr;
    ToolController* m_toolController = nullptr;
    bool m_panning = false;
    QPoint m_panOrigin;
};

#endif
