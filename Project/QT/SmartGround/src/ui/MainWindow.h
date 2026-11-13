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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void onModuleChanged(int index);
    void onBtnBuildClicked();
    void onBtnStepClicked();
    void onBtnStepBackClicked();
    void onBtnAutoClicked();
    void onGraphOptionChanged();
    void onFrameSettled();
    void onGridToggled(bool on);
    void updateCollapsibleRatio();
    void animateSubOptionVisibility();
    void onFrameSliderMoved(int value);
    void onFrameSliderReleased(int value);
    void seekToFrame(int index);
    void syncFrameSlider();
private:
    void setupUI();
    void applyStyleSheet();
    void clearScene();
    void adjustSceneBoundary(bool centerView = false);
    void rebuildCurrentLayout(const QString& inputData, bool animated, bool centerView);
    void computeFrames();
    void applyLiveEditsAndReplay();
    void updateGraphOptionPanel();
    void applyGraphOptions();
    void updateInputHint();
    void updateGridOption();
    void startAutoPlay();
    void requestPause();
    void finishAutoPlay();
    void advanceFrame();
    int frameDelayMs(const AnimFrame& frame) const;

    MyComboBox *comboModules;
    MyTextEdit *textInput;
    MyButton *btnBuild, *btnStep, *btnStepBack, *btnAuto;
    MySlider *frameSlider;
    QLabel *labelStatus;
    QLabel *labelFrameBadge;
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QWidget *graphPanelHost;
    MyCollapsible *graphPanel;
    MyToggleSwitch *toggleDirected;
    MyCheckBox *checkBidirectional;
    MyCheckBox *checkGrid;
    QLabel *labelWeightHint;
    QVBoxLayout *leftLayout;
    std::vector<IModule*> registeredModules;
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

    friend class UiBehaviorProbe;
    friend class GroupAProbe;

public:
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
};
#endif
