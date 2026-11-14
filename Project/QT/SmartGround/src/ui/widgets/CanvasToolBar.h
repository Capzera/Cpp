#ifndef CANVASTOOLBAR_H
#define CANVASTOOLBAR_H
#include <QWidget>
#include <QVector>

class QVBoxLayout;

class ToolController;
class ToolIconButton;
class MyButton;
class QSlider;
class QStackedWidget;

class CanvasToolBar : public QWidget {
    Q_OBJECT
public:
    explicit CanvasToolBar(ToolController* controller, QWidget* parent = nullptr);

    void setHostWidget(QWidget* host);
    void reposition();

    bool isStylePanelOpen() const { return m_styleOpen; }
    bool isMorePanelOpen() const { return m_moreOpen; }
    bool isToolPanelOpen() const { return m_toolPanelOpen; }
    QWidget* stylePanel() const { return m_stylePanel; }
    QWidget* morePanel() const { return m_morePanel; }
    QWidget* toolPanel() const { return m_toolPanel; }
    int mainBarWidth() const;

signals:
    void exportRequested();
    void clearRequested();

protected:
    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void buildMainBar();
    void buildStylePanel();
    void buildMorePanel();
    void buildToolPanel();
    QWidget* createToolPage(int tool);
    void showToolPage(int tool);
    void toggleStylePanel();
    void toggleMorePanel();
    void toggleToolPanel();
    void closeFlyouts();
    void placePanel(QWidget* panel, int barX, int barW, int barY, int barH);

    ToolController* m_controller = nullptr;
    QWidget* m_host = nullptr;
    QWidget* m_stylePanel = nullptr;
    QWidget* m_morePanel = nullptr;
    QWidget* m_toolPanel = nullptr;
    QStackedWidget* m_toolPages = nullptr;
    QVector<ToolIconButton*> m_toolButtons;
    ToolIconButton* m_btnStyle = nullptr;
    ToolIconButton* m_btnMore = nullptr;
    MyButton* m_btnUndo = nullptr;
    MyButton* m_btnRedo = nullptr;
    bool m_styleOpen = false;
    bool m_moreOpen = false;
    bool m_toolPanelOpen = false;
    int m_tableRows = 3;
    int m_tableCols = 3;
};

#endif
