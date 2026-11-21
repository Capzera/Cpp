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

    void setCartesianMode(bool on);
    bool isCartesianMode() const { return m_cartesianMode; }
    void setCartesianPointNames(bool on);
    bool cartesianPointNames() const { return m_cartesianPointNames; }

    bool isMorePanelOpen() const { return m_moreOpen; }
    bool isToolPanelOpen() const { return m_toolPanelOpen; }
    QWidget* morePanel() const { return m_morePanel; }
    QWidget* toolPanel() const { return m_toolPanel; }
    int mainBarWidth() const;
    int mainRowWidth() const;
    int mainRowHeight() const;

    ToolIconButton* toolButtonFor(int tool) const;
    ToolIconButton* cartesianButtonFor(int kind) const;

signals:
    void exportRequested();
    void clearRequested();
    void cartesianToolChanged(int kind);
    void cartesianPointNamesChanged(bool on);

protected:
    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void buildMainBar();
    void buildCartesianRow();
    void buildMorePanel();
    void buildToolPanel();
    QWidget* createToolPage(int tool);
    QWidget* createCartesianPage(int kind);
    void showToolPage(int tool);
    void refreshToolPage(int tool);
    void applyToolPageSize();
    void toggleMorePanel();
    void toggleToolPanel();
    void closeFlyouts();
    void placePanel(QWidget* panel, int barX, int barW, int barY, int barH);
    void placeToolPanelUnderButton();
    void syncToolButtons();
    void syncCartesianButtons(int tool);
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

    QWidget* m_cartesianRow = nullptr;
    QVector<ToolIconButton*> m_cartesianButtons;
    bool m_cartesianMode = false;
    bool m_cartesianPointNames = true;
    bool m_cartesianPanelOpen = false;
};

#endif
