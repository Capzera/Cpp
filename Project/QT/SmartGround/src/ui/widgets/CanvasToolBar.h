#ifndef CANVASTOOLBAR_H
#define CANVASTOOLBAR_H
#include <QWidget>
#include <QVector>

class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;

class ToolController;
class ToolIconButton;
class MyButton;
class MySlider;
class MyColorPicker;
class MyTextInput;

class CanvasToolBar : public QWidget {
    Q_OBJECT
public:
    explicit CanvasToolBar(ToolController* controller, QWidget* parent = nullptr);
    ~CanvasToolBar() override;

    void setHostWidget(QWidget* host);
    void reposition();

    bool isMorePanelOpen() const { return m_moreOpen; }
    bool isToolPanelOpen() const { return m_toolPanelOpen; }
    QWidget* morePanel() const { return m_morePanel; }
    QWidget* toolPanel() const { return m_toolPanel; }
    int mainBarWidth() const;

    ToolIconButton* toolButtonFor(int tool) const;

signals:
    void exportRequested();
    void clearRequested();

protected:
    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void buildMainBar();
    void buildMorePanel();
    void buildToolPanel();
    QWidget* createToolPage(int tool);
    void showToolPage(int tool);
    void refreshToolPage(int tool);
    void applyToolPageSize();
    void toggleMorePanel();
    void toggleToolPanel();
    void closeFlyouts();
    void placePanel(QWidget* panel, int barX, int barW, int barY, int barH);
    void placeToolPanelUnderButton();
    void syncToolButtons();
    bool isInsidePanel(QWidget* panel, const QPoint& globalPos) const;
    bool isInsideToolbar(const QPoint& globalPos) const;
    bool shouldCloseOnPress(QObject* obj, QEvent* event) const;

    ToolController* m_controller = nullptr;
    QWidget* m_host = nullptr;
    QWidget* m_morePanel = nullptr;
    QWidget* m_toolPanel = nullptr;
    QStackedWidget* m_toolPages = nullptr;
    QVector<ToolIconButton*> m_toolButtons;
    ToolIconButton* m_btnMore = nullptr;
    bool m_moreOpen = false;
    bool m_toolPanelOpen = false;
    int m_activeToolIndex = -1;
};

#endif
