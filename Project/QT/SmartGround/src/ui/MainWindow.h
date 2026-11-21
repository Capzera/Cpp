#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include "modules/IModule.h"
#include "ui/widgets/MyButton.h"
#include "ui/widgets/MyComboBox.h"
#include "ui/widgets/MyTextEdit.h"
#include "ui/widgets/MyToggleSwitch.h"
#include "ui/widgets/MyCheckBox.h"
#include "ui/widgets/MyCollapsible.h"
#include "ui/widgets/MySlider.h"
#include "ui/widgets/MySegmentedControl.h"
#include "modules/ModuleFields.h"
#include "canvas/ToolController.h"

class CanvasView;
class AnnotationScene;
class ToolController;
class CanvasToolBar;
class CanvasBrushSlider;
class TableToolBar;
class CanvasTableItem;
class CartesianModule;
class CartesianEngine;
class CartesianPanel;
class CartesianInfoPanel;
class MyTextEdit;
class MySegmentedControl;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
private slots:
    void onModuleChanged(int index);
    void onBtnBuildClicked();
    void onBtnStepClicked();
    void onBtnStepBackClicked();
    void onBtnAutoClicked();
    void onGraphOptionChanged();
    void onFrameSettled();
    void onGridToggled(bool on);
    void onScriptTextToggled(bool on);
    void updateCollapsibleRatio();
    void animateSubOptionVisibility();
    void onFrameSliderMoved(int value);
    void onFrameSliderPressed(int value);
    void onFrameSliderReleased(int value);
    void seekToFrame(int index);
    void syncFrameSlider();
    void onExportPdf();
    void onClearAnnotations();
    void onAnnotationVisibilityToggled();
    void onAnnotationLockToggled();
    void onCartesianPointEdited(int index, const QString& name, double x, double y, bool nameHidden);
    void onCartesianPointStyleEdited(int index, const QColor& color, qreal size, int shape, bool nameHidden);
    void onCartesianFunctionStyleEdited(const QString& name, const QColor& color);
    void onCartesianFunctionAdded(const QString& expression);
    void onCartesianFunctionRemoved(const QString& name);
    void onCartesianToolChanged(int kind);
    void onCartesianPointNamesChanged(bool on);
    void onCartesianPointDoubleClicked(int index);
    void onCartesianSegmentDoubleClicked(int index);
    void onCartesianCurveDoubleClicked(int index);
    void refreshCartesianViewport();
    void positionCartesianInfo();
private:
    void setupUI();
    void setupCartesianWiring();
    void applyStyleSheet();
    void clearScene();
    void archiveAnnotations();
    void restoreAnnotations();
    void adjustSceneBoundary(bool centerView = false);
    void rebuildCurrentLayout(const QString& inputData, bool animated, bool centerView);
    void computeFrames();
    void applyLiveEditsAndReplay();
    void updateGraphOptionPanel();
    void applyGraphOptions();
    void rebuildModulePanel();
    void onModuleFieldChanged(const QString& key);
    void updateInputHint();
    void updateGridOption();
    void applyCartesianLayoutMode(bool on);
    void showCartesianHint();
    void startAutoPlay();
    void requestPause();
    void finishAutoPlay();
    void advanceFrame();
    int frameDelayMs(const AnimFrame& frame) const;
    void setupCanvasTools();
    void invalidateCanvas();
    void positionTableToolBar();
    void exportCanvasToPdf(const QString& path);
    void syncAnnotationToolButtons(int tool);
    QString toolName(ToolController::Tool tool) const;

    MyComboBox *comboModules;
    QLabel *labelInput;
    MyTextEdit *textInput;
    MyButton *btnBuild, *btnStep, *btnStepBack, *btnAuto;
    MySlider *frameSlider;
    QLabel *labelStatus;
    QLabel *labelFrameBadge;
    QLabel *labelVersion;
    CanvasView *graphicsView;
    QGraphicsScene *graphicsScene;
    QWidget *graphPanelHost;
    MyCollapsible *graphPanel;
    MyToggleSwitch *toggleDirected;
    MyCheckBox *checkBidirectional;
    MyCheckBox *checkGrid;
    MyCheckBox *checkScriptText;
    QLabel *labelWeightHint;
    QWidget *modulePanelHost;
    MyCollapsible *modulePanel;
    QVBoxLayout *modulePanelContent;
    QHash<QString, ModuleField> m_moduleFieldValues;
    QHash<QString, QWidget*> m_moduleFieldWidgets;
    QVBoxLayout *leftLayout;
    std::vector<IModule*> registeredModules;
    std::vector<int> comboToModule;
    IModule* currentModule;
    std::vector<AnimFrame> m_frames;
    int m_frameIndex;
    QString m_builtInput;
    QString m_recordedInput;
    bool m_layoutBuilt;
    bool m_graphPanelInLayout;
    bool m_playing;
    bool m_pendingPause;
    bool m_seeking = false;
    bool m_liveEditPending = false;
    QTimer *m_playTimer;
    QTimer *m_settleTimer;
    QElapsedTimer m_frameClock;

    AnnotationScene* m_annotationScene = nullptr;
    ToolController* m_toolController = nullptr;
    CanvasToolBar* m_canvasToolBar = nullptr;
    CanvasBrushSlider* m_canvasBrushSlider = nullptr;
    TableToolBar* m_tableToolBar = nullptr;
    MyButton* m_btnExportPdf = nullptr;
    MyButton* m_btnClearAnnotations = nullptr;
    MyButton* m_btnToggleAnnotations = nullptr;
    MyButton* m_btnLockAnnotations = nullptr;
    bool m_annotationsVisible = true;
    bool m_annotationsLocked = false;
    CartesianModule* m_cartesianModule = nullptr;
    CartesianPanel* m_cartesianPanel = nullptr;
    CartesianInfoPanel* m_cartesianInfo = nullptr;
    bool m_cartesianActive = false;
    bool m_cartesianWired = false;
    friend class UiBehaviorProbe;
    friend class GroupAProbe;

public:
    bool probeModulePanelVisible() const { return modulePanelHost && modulePanelHost->isVisible(); }
    QString probeModulePanelTitle() const { return modulePanel ? modulePanel->headerText() : QString(); }
    IModule* probeCurrentModule() const { return currentModule; }
    void probeBuild() { onBtnBuildClicked(); }
    int probeFrameIndex() const { return m_frameIndex; }
    bool probeIsPlaying() const { return m_playing; }
    bool probePendingPause() const { return m_pendingPause; }
    int probeFrameCount() const { return static_cast<int>(m_frames.size()); }
    void probeStartAuto() { startAutoPlay(); }
    void probeRequestPause() { requestPause(); }
    void probeAdvance() { advanceFrame(); }
    void probeSettle() { onFrameSettled(); }
    QStringList probeModuleNames() const {
        QStringList names;
        for (int i = 1; i < comboModules->count(); ++i) names << comboModules->itemText(i);
        return names;
    }
    QStringList probeUniqueModuleNames() const {
        QStringList names;
        for (IModule* m : registeredModules) {
            const QString name = m->getModuleName();
            if (!names.contains(name)) names << name;
        }
        return names;
    }
    void probeSelectModule(const QString& name) {
        for (int i = 1; i < comboModules->count(); ++i) {
            if (comboModules->itemText(i) == name) { comboModules->setCurrentIndex(i); return; }
        }
    }
    int probeDelayFor(const QString& text) const {
        AnimFrame f;
        f.type = ActionType::AR_HIGHLIGHT;
        f.explanation = text;
        f.mathText = "";
        return frameDelayMs(f);
    }
    QGraphicsScene* probeScene() const { return graphicsScene; }
};
#endif
