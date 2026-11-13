#include "MainWindow.h"
#include "UiTheme.h"
#include "modules/TarjanModule.h"
#include "modules/BubbleSortModule.h"
#include "modules/SelectionSortModule.h"
#include "modules/InsertionSortModule.h"
#include "modules/QuickSortModule.h"
#include "modules/MergeSortModule.h"
#include "modules/HeapBasicsModule.h"
#include "modules/MonotonicStackModule.h"
#include "modules/DSUBasicModule.h"
#include "modules/PrefixSumModule.h"
#include "modules/DifferenceModule.h"
#include "modules/StackModule.h"
#include "modules/QueueModule.h"
#include "modules/BinarySearchClosedModule.h"
#include "modules/BinarySearchHalfOpenModule.h"
#include "modules/TopologicalSortModule.h"
#include "modules/DijkstraNaiveModule.h"
#include "modules/KruskalModule.h"
#include "modules/BFSModule.h"
#include "modules/Knapsack01Module.h"
#include "modules/LISModule.h"
#include "modules/KMPNextModule.h"
#include "modules/KMPMatchModule.h"
#include "modules/SegmentTreeBuildModule.h"
#include "modules/SegmentTreeTagModule.h"
#include "modules/SegmentTreeQueryModule.h"
#include "modules/SandboxArrayModule.h"
#include "modules/SandboxMatrixModule.h"
#include "modules/SandboxTreeModule.h"
#include "modules/SandboxGraphModule.h"
#include "modules/SandboxLatexModule.h"
#include "render/LatexBlock.h"
#include "render/EditableModel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QWheelEvent>
#include <QGridLayout>
#include <QStyle>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <cmath>
#include "render/SceneGrid.h"
#include "core/ViewportFitter.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentModule(nullptr), m_frameIndex(0), m_layoutBuilt(false), m_graphPanelInLayout(false), m_playing(false), m_pendingPause(false) {
    setWindowTitle("| SmartGround - 信奥赛沙盒演示 | By : Capzera | VX : Capzera18655458613 |"); resize(1200, 800);
    setupUI();
    LatexBlock::loadLibrary(QApplication::applicationDirPath() + "/latex_formulas.json");
    m_playTimer = new QTimer(this);
    m_playTimer->setSingleShot(true);
    connect(m_playTimer, &QTimer::timeout, this, &MainWindow::advanceFrame);
    m_settleTimer = new QTimer(this);
    m_settleTimer->setSingleShot(true);
    connect(m_settleTimer, &QTimer::timeout, this, &MainWindow::onFrameSettled);
    connect(frameSlider, &MySlider::valueChanged, this, &MainWindow::onFrameSliderMoved);
    connect(frameSlider, &MySlider::sliderReleased, this, &MainWindow::onFrameSliderReleased);

    // 全他妈模块
    registeredModules.push_back(new BubbleSortModule());
    registeredModules.push_back(new SelectionSortModule());
    registeredModules.push_back(new InsertionSortModule());
    registeredModules.push_back(new QuickSortModule());
    registeredModules.push_back(new MergeSortModule());
    registeredModules.push_back(new HeapBasicsModule());
    registeredModules.push_back(new PrefixSumModule());
    registeredModules.push_back(new DifferenceModule());
    registeredModules.push_back(new StackModule());
    registeredModules.push_back(new QueueModule());
    registeredModules.push_back(new MonotonicStackModule());
    registeredModules.push_back(new BinarySearchClosedModule());
    registeredModules.push_back(new BinarySearchHalfOpenModule());
    registeredModules.push_back(new DSUBasicModule());
    registeredModules.push_back(new TarjanModule());
    registeredModules.push_back(new TopologicalSortModule());
    registeredModules.push_back(new DijkstraNaiveModule());
    registeredModules.push_back(new KruskalModule());
    registeredModules.push_back(new BFSModule());
    registeredModules.push_back(new Knapsack01Module());
    registeredModules.push_back(new LISModule());
    registeredModules.push_back(new KMPNextModule());
    registeredModules.push_back(new KMPMatchModule());
    registeredModules.push_back(new SegmentTreeBuildModule());
    registeredModules.push_back(new SegmentTreeTagModule());
    registeredModules.push_back(new SegmentTreeQueryModule());
    registeredModules.push_back(new SandboxArrayModule());
    registeredModules.push_back(new SandboxMatrixModule());
    registeredModules.push_back(new SandboxTreeModule());
    registeredModules.push_back(new SandboxGraphModule());
    registeredModules.push_back(new SandboxLatexModule());

    for(auto m : registeredModules) comboModules->addItem(m->getModuleName());
    for (auto m : registeredModules) {
        m->setLiveEditCallback([this]() {
            if (m_liveEditPending) return;
            m_liveEditPending = true;
            QTimer::singleShot(0, this, [this]() {
                m_liveEditPending = false;
                applyLiveEditsAndReplay();
            });
        });
    }

    SceneGrid::Config gridCfg;
    gridCfg.enabled = true;
    gridCfg.showGuides = true;
    gridCfg.cell = 56.0;
    gridCfg.magnetRange = 18.0;
    SceneGrid::attach(graphicsScene, gridCfg);
}
MainWindow::~MainWindow() {
    for (auto module : registeredModules) delete module;
    registeredModules.clear();
}
void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("appRoot");
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(16, 16, 14, 16);
    leftLayout->setSpacing(12);

    QLabel *labelTitle = new QLabel("算法模块");
    labelTitle->setObjectName("sectionLabel");
    comboModules = new MyComboBox();
    comboModules->addItem("请选择...");
    comboModules->setMinimumHeight(36);

    QLabel *labelInput = new QLabel("数据输入");
    labelInput->setObjectName("sectionLabel");
    textInput = new MyTextEdit();
    textInput->setPlaceholderText("请在上方选择算法模块，这里会显示对应的输入格式说明");

    graphPanelHost = new QWidget();
    graphPanelHost->setObjectName("graphPanelHost");
    QVBoxLayout *graphPanelLayout = new QVBoxLayout(graphPanelHost);
    graphPanelLayout->setContentsMargins(0, 0, 0, 0);
    graphPanelLayout->setSpacing(0);

    QWidget *graphContent = new QWidget();
    graphContent->setAttribute(Qt::WA_TranslucentBackground, true);
    QVBoxLayout *graphContentLayout = new QVBoxLayout(graphContent);
    graphContentLayout->setContentsMargins(14, 0, 14, 14);
    graphContentLayout->setSpacing(10);

    QWidget *directionRow = new QWidget();
    directionRow->setAttribute(Qt::WA_TranslucentBackground, true);
    QHBoxLayout *directionLayout = new QHBoxLayout(directionRow);
    directionLayout->setContentsMargins(0, 0, 0, 0);
    directionLayout->setSpacing(10);
    toggleDirected = new MyToggleSwitch();
    toggleDirected->setOnText("无向图");
    toggleDirected->setOffText("有向图");
    toggleDirected->setOnColor(UiTheme::success());
    toggleDirected->setChecked(false);
    QLabel *directionLabel = new QLabel("图的类型");
    directionLabel->setObjectName("fieldLabel");
    directionLayout->addWidget(directionLabel);
    directionLayout->addStretch(1);
    directionLayout->addWidget(toggleDirected);

    checkBidirectional = new MyCheckBox("无向边加双向箭头");
    checkBidirectional->setEnabled(false);

    labelWeightHint = new QLabel("边权按输入自动判定：某行写了 3 个数即为带权图");
    labelWeightHint->setObjectName("hintLabel");
    labelWeightHint->setWordWrap(true);

    graphContentLayout->addWidget(directionRow);
    graphContentLayout->addWidget(checkBidirectional);
    graphContentLayout->addWidget(labelWeightHint);

    graphPanel = new MyCollapsible();
    graphPanel->setHeaderText("图结构选项");
    graphPanel->setContentWidget(graphContent);
    graphPanelLayout->addWidget(graphPanel);

    checkGrid = new MyCheckBox("启用对齐网格");
    checkGrid->setChecked(true);
    checkGrid->setObjectName("gridToggle");

    btnBuild = new MyButton("绘制可视化效果");
    btnStepBack = new MyButton("上一步");
    btnStep = new MyButton("下一步");
    btnAuto = new MyButton("自动播放");

    btnBuild->setVariant(MyButton::Solid);
    btnStepBack->setVariant(MyButton::Ghost);
    btnStep->setVariant(MyButton::Ghost);
    btnAuto->setVariant(MyButton::Soft);
    btnStepBack->setCompact(true);
    btnStep->setCompact(true);

    QHBoxLayout *playLayout = new QHBoxLayout();
    playLayout->setSpacing(8);
    playLayout->addWidget(btnStepBack);
    playLayout->addWidget(btnStep);

    leftLayout->addWidget(labelTitle);
    leftLayout->addWidget(comboModules);
    leftLayout->addWidget(graphPanelHost);
    leftLayout->addWidget(labelInput);
    leftLayout->addWidget(textInput, 1);
    leftLayout->addWidget(checkGrid);
    leftLayout->addWidget(btnBuild);
    leftLayout->addLayout(playLayout);
    leftLayout->addWidget(btnAuto);

    QWidget *canvasHost = new QWidget();
    canvasHost->setObjectName("canvasHost");
    QVBoxLayout *rightLayout = new QVBoxLayout(canvasHost);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    graphicsScene = new QGraphicsScene(this);
    graphicsScene->setBackgroundBrush(UiTheme::canvasBg());
    graphicsView = new QGraphicsView(graphicsScene);
    graphicsView->setObjectName("canvasView");
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setRenderHint(QPainter::TextAntialiasing);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->viewport()->installEventFilter(this);

    labelStatus = new QLabel("就绪。");
    labelStatus->setObjectName("statusLabel");

    QWidget* statusBar = new QWidget();
    statusBar->setObjectName("statusBar");
    QHBoxLayout* statusLayout = new QHBoxLayout(statusBar);
    statusLayout->setContentsMargins(14, 6, 14, 6);
    statusLayout->setSpacing(12);
    statusLayout->addWidget(labelStatus, 1);

    frameSlider = new MySlider();
    frameSlider->setObjectName("frameSlider");
    frameSlider->setRange(0, 0);
    frameSlider->setFixedWidth(210);
    frameSlider->setEnabled(false);

    labelFrameBadge = new QLabel("0 / 0");
    labelFrameBadge->setObjectName("frameBadge");
    labelFrameBadge->setFixedWidth(74);
    labelFrameBadge->setAlignment(Qt::AlignCenter);

    statusLayout->addWidget(frameSlider);
    statusLayout->addWidget(labelFrameBadge);

    rightLayout->addWidget(graphicsView, 1);
    rightLayout->addWidget(statusBar);

    mainLayout->addLayout(leftLayout, 0);
    mainLayout->addWidget(canvasHost, 1);
    setCentralWidget(centralWidget);

    graphPanelHost->setVisible(false);

    connect(comboModules, &MyComboBox::currentIndexChanged, this, &MainWindow::onModuleChanged);
    connect(btnBuild, &MyButton::clicked, this, &MainWindow::onBtnBuildClicked);
    connect(btnStepBack, &MyButton::clicked, this, &MainWindow::onBtnStepBackClicked);
    connect(btnStep, &MyButton::clicked, this, &MainWindow::onBtnStepClicked);
    connect(btnAuto, &MyButton::clicked, this, &MainWindow::onBtnAutoClicked);
    connect(toggleDirected, &MyToggleSwitch::toggled, this, &MainWindow::onGraphOptionChanged);
    connect(checkBidirectional, &MyCheckBox::toggled, this, &MainWindow::onGraphOptionChanged);
    connect(checkGrid, &MyCheckBox::toggled, this, &MainWindow::onGridToggled);
    applyStyleSheet();
}

void MainWindow::applyStyleSheet() {
    const QString border = UiTheme::border().name();
    const QString textPrimary = UiTheme::textPrimary().name();
    const QString textSecondary = UiTheme::textSecondary().name();
    const QString textMuted = UiTheme::textMuted().name();
    const QString surface = UiTheme::surface().name();
    const QString surfaceAlt = UiTheme::surfaceAlt().name();
    const QString canvasBg = UiTheme::canvasBg().name();
    setStyleSheet(QString(
        "QWidget#appRoot { background:%1; }"
        "QWidget#appRoot QWidget { color:%2; }"
        "QWidget#canvasHost { background:%3; }"
        "QWidget#graphPanelHost { background:transparent; }"
        "QLabel#sectionLabel { font-family:'Microsoft YaHei UI','Segoe UI'; font-size:11px; font-weight:600; color:%4; }"
        "QLabel#fieldLabel { font-family:'Microsoft YaHei UI','Segoe UI'; font-size:10px; color:%5; }"
        "QLabel#hintLabel { font-family:'Microsoft YaHei UI','Segoe UI'; font-size:9px; color:%6; }"
        "QLabel#statusLabel { font-family:'Microsoft YaHei UI','Segoe UI'; font-size:11px; font-weight:600; color:%5;"
        " background:%7; padding:8px 14px; border-top:1px solid %8; }"
        "QWidget#statusBar { background:%7; border-top:1px solid %8; }"
        "QLabel#frameBadge { font-family:'Cascadia Mono','Consolas'; font-size:10px; color:%6;"
        " background:%7; padding:2px 6px; border-radius:6px; }"
    ).arg(surface, textPrimary, canvasBg, textSecondary, textPrimary, textMuted, surfaceAlt, border));
}

void MainWindow::onBtnStepBackClicked() {
    if (!currentModule || m_frameIndex <= 0) return;
    finishAutoPlay();
    m_playTimer->stop();
    m_settleTimer->stop();
    m_frameIndex--;
    bool previousAnimationState = currentModule->m_animated;
    currentModule->m_animated = false;
    rebuildCurrentLayout(m_recordedInput, false, false);
    currentModule->refreshLiveSlots();
    for (int i = 0; i < m_frameIndex; ++i) {
        currentModule->renderFrame(m_frames[i]);
    }
    currentModule->m_animated = previousAnimationState;
    adjustSceneBoundary(false);
    syncFrameSlider();
    labelStatus->setText(QString("已撤销，退回第 %1 帧").arg(m_frameIndex));
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == graphicsView->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers()/* & Qt::ControlModifier*/) {
            double angle = wheelEvent->angleDelta().y();
            double factor = std::pow(1.0015, angle);
            double currentScale = graphicsView->transform().m11();
            if ((currentScale < 0.2 && factor < 1.0) || (currentScale > 5.0 && factor > 1.0)) return true;
            graphicsView->scale(factor, factor);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::onModuleChanged(int index) {
    finishAutoPlay();
    m_playTimer->stop();
    m_settleTimer->stop();
    if (index > 0 && index - 1 < registeredModules.size()) {
        clearScene();
        currentModule = registeredModules[index - 1];
        m_frames.clear();
        m_frameIndex = 0;
        m_builtInput.clear();
        m_recordedInput.clear();
        m_layoutBuilt = false;
        textInput->setText(currentModule->getDefaultInput());
        labelStatus->setText("已切换至: " + currentModule->getModuleName());
        graphicsView->resetTransform();
        syncFrameSlider();
        updateInputHint();
        updateGraphOptionPanel();
        applyGraphOptions();
    } else {
        currentModule = nullptr;
        clearScene();
        m_frames.clear();
        m_frameIndex = 0;
        m_builtInput.clear();
        m_recordedInput.clear();
        m_layoutBuilt = false;
        textInput->clear();
        textInput->setPlaceholderText("请在上方选择算法模块，这里会显示对应的输入格式说明");
        graphPanelHost->setVisible(false);
        m_graphPanelInLayout = false;
        graphicsView->resetTransform();
        syncFrameSlider();
        labelStatus->setText("已清空画布，请选择算法模块。");
    }
}
void MainWindow::updateInputHint() {
    if (!currentModule) return;
    textInput->setPlaceholderText(currentModule->getInputHint());
}
void MainWindow::updateGraphOptionPanel() {
    bool supported = currentModule && currentModule->supportsGraphOptions();
    graphPanelHost->setVisible(supported);
    m_graphPanelInLayout = supported;
    if (!supported) return;
    GraphModuleUtils::GraphOptions options = currentModule->getGraphOptions();
    bool canDirected = currentModule->supportsDirected();
    bool canBidirectional = currentModule->supportsBidirectional();

    bool wantUndirected = canDirected ? !options.directed : true;
    if (toggleDirected->isChecked() != wantUndirected) {
        toggleDirected->blockSignals(true);
        toggleDirected->setChecked(wantUndirected);
        toggleDirected->blockSignals(false);
    }
    toggleDirected->setEnabled(canDirected);
    checkBidirectional->setEnabled(canBidirectional && wantUndirected);
    if (!canBidirectional) {
        if (checkBidirectional->isChecked()) {
            checkBidirectional->blockSignals(true);
            checkBidirectional->setChecked(false);
            checkBidirectional->blockSignals(false);
        }
    } else {
        bool wantBi = options.bidirectional && wantUndirected;
        if (checkBidirectional->isChecked() != wantBi) {
            checkBidirectional->blockSignals(true);
            checkBidirectional->setChecked(wantBi);
            checkBidirectional->blockSignals(false);
        }
    }
    updateCollapsibleRatio();
}
void MainWindow::updateCollapsibleRatio() {
    bool subVisible = toggleDirected->isChecked();
    checkBidirectional->setProperty("ratio", subVisible ? 1.0 : 0.0);
    checkBidirectional->style()->unpolish(checkBidirectional);
    checkBidirectional->style()->polish(checkBidirectional);
}
void MainWindow::applyGraphOptions() {
    if (!currentModule || !currentModule->supportsGraphOptions()) return;
    GraphModuleUtils::GraphOptions options = currentModule->getGraphOptions();
    options.directed = !toggleDirected->isChecked() && currentModule->supportsDirected();
    options.bidirectional = !options.directed && checkBidirectional->isChecked() && currentModule->supportsBidirectional();
    currentModule->setGraphOptions(options);
}
void MainWindow::onGraphOptionChanged() {
    if (!currentModule) return;
    applyGraphOptions();
    updateGraphOptionPanel();
    animateSubOptionVisibility();
    finishAutoPlay();
    m_playTimer->stop();
    m_settleTimer->stop();
    m_frames.clear();
    m_frameIndex = 0;
    m_recordedInput.clear();
    std::map<int, QPointF> saved = currentModule->captureNodePositions();
    rebuildCurrentLayout(textInput->toPlainText(), false, false);
    currentModule->restoreNodePositions(saved);
    adjustSceneBoundary(false);
    labelStatus->setText("图结构选项已更新，节点位置已保留，请重新计算动画帧。");
}
void MainWindow::onGridToggled(bool on) {
    SceneGrid::Config cfg = SceneGrid::currentConfig();
    cfg.enabled = on;
    cfg.showGuides = on;
    SceneGrid::setConfig(graphicsScene, cfg);
    labelStatus->setText(on ? "已启用对齐网格：拖动节点时自动对齐，按住 Ctrl 可临时自由拖动。"
                            : "已关闭对齐网格，节点可自由拖动。");
}

void MainWindow::animateSubOptionVisibility() {    bool show = toggleDirected->isChecked();
    QPropertyAnimation* anim = new QPropertyAnimation(checkBidirectional, "ratio", this);
    anim->setDuration(show ? 220 : 260);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    anim->setStartValue(checkBidirectional->property("ratio").toDouble());
    anim->setEndValue(show ? 1.0 : 0.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void MainWindow::onBtnBuildClicked() {
    if (!currentModule) return;
    finishAutoPlay();
    m_playTimer->stop();
    m_settleTimer->stop();
    m_frames.clear();
    m_frameIndex = 0;
    m_recordedInput.clear();
    EditableModel::instance().clearScope(currentModule->liveScope());
    currentModule->clearLiveEditCount();
    rebuildCurrentLayout(textInput->toPlainText(), true, true);
    computeFrames();
    labelStatus->setText(QString("绘制完成，共录制 %1 帧。").arg(m_frames.size()));
}
void MainWindow::computeFrames() {
    if (!currentModule) return;
    m_frames = currentModule->runAlgorithm();
    m_frameIndex = 0;
    m_recordedInput = m_builtInput;
    syncFrameSlider();
}
void MainWindow::applyLiveEditsAndReplay() {
    if (!currentModule || m_frames.empty()) return;
    int target = m_frameIndex;
    if (target > static_cast<int>(m_frames.size())) target = static_cast<int>(m_frames.size());
    bool previousAnimationState = currentModule->m_animated;
    std::map<int, QPointF> saved = currentModule->captureNodePositions();
    currentModule->m_animated = false;
    rebuildCurrentLayout(m_recordedInput, false, false);
    currentModule->restoreNodePositions(saved);
    currentModule->refreshLiveSlots();
    m_frames = currentModule->runAlgorithm();
    if (target > static_cast<int>(m_frames.size())) target = static_cast<int>(m_frames.size());
    for (int i = 0; i < target; ++i) {
        currentModule->renderFrame(m_frames[i]);
    }
    currentModule->m_animated = previousAnimationState;
    m_frameIndex = target;
    syncFrameSlider();
    adjustSceneBoundary(false);
    labelStatus->setText(QString("已根据老师修改重新计算，后续动画按新数值继续。"));
}

void MainWindow::onBtnStepClicked() {
    if (!currentModule || m_frameIndex >= m_frames.size()) { finishAutoPlay(); return; }
    finishAutoPlay();
    m_playTimer->stop();
    m_settleTimer->stop();
    AnimFrame f = m_frames[m_frameIndex++];
    currentModule->beginFrame();
    currentModule->renderFrame(f);
    adjustSceneBoundary(false);
    syncFrameSlider();
    labelStatus->setText(QString("帧 %1/%2 : %3").arg(m_frameIndex).arg(m_frames.size()).arg(f.explanation));
}
void MainWindow::onBtnAutoClicked() {
    if (!currentModule || m_frames.empty()) return;
    if (m_playing) {
        requestPause();
        return;
    }
    startAutoPlay();
}
void MainWindow::startAutoPlay() {
    if (!currentModule || m_frames.empty()) return;
    if (m_frameIndex >= m_frames.size()) {
        m_playTimer->stop();
        rebuildCurrentLayout(m_recordedInput, false, false);
        m_frameIndex = 0;
    }
    m_playing = true;
    m_pendingPause = false;
    btnAuto->setText("暂停");
    btnAuto->setAccentColor(UiTheme::success());
    btnAuto->setVariant(MyButton::Solid);
    labelStatus->setText("自动播放中...");
    m_playTimer->start(frameDelayMs(m_frames[m_frameIndex]));
}
void MainWindow::requestPause() {
    if (!m_playing) return;
    m_pendingPause = true;
    btnAuto->setText("暂停中...");
    labelStatus->setText("当前动画帧播放完毕后将暂停...");
}
void MainWindow::finishAutoPlay() {
    m_playing = false;
    m_pendingPause = false;
    if (!btnAuto) return;
    btnAuto->setText("自动播放");
    btnAuto->setAccentColor(UiTheme::accent());
    btnAuto->setVariant(MyButton::Soft);
}
void MainWindow::advanceFrame() {
    if (!m_playing || !currentModule) { finishAutoPlay(); return; }
    if (m_pendingPause) { finishAutoPlay(); return; }
    if (m_frameIndex >= m_frames.size()) {
        m_playTimer->stop();
        finishAutoPlay();
        labelStatus->setText("播放完毕。");
        return;
    }
    AnimFrame f = m_frames[m_frameIndex++];
    currentModule->beginFrame();
    currentModule->renderFrame(f);
    m_frameClock.start();
    adjustSceneBoundary(false);
    labelStatus->setText(QString("帧 %1/%2 : %3").arg(m_frameIndex).arg(m_frames.size()).arg(f.explanation));
    if (m_pendingPause) {
        m_settleTimer->start(820);
        return;
    }
    if (m_frameIndex >= m_frames.size()) {
        m_settleTimer->start(820);
        return;
    }
    m_playTimer->start(frameDelayMs(f));
}
void MainWindow::onFrameSettled() {
    if (m_pendingPause) {
        finishAutoPlay();
        m_playTimer->stop();
        labelStatus->setText("已暂停。");
        return;
    }
    if (m_playing && m_frameIndex >= m_frames.size()) {
        m_playTimer->stop();
        finishAutoPlay();
        labelStatus->setText("播放完毕。");
    }
}
int MainWindow::frameDelayMs(const AnimFrame& frame) const {
    int textLength = frame.explanation.size() + frame.mathText.size();
    int delay = 520 + textLength * 28;
    if (delay < 900) delay = 900;
    if (delay > 2800) delay = 2800;
    return delay;
}
void MainWindow::syncFrameSlider() {
    if (!frameSlider) return;
    int total = static_cast<int>(m_frames.size());
    frameSlider->setRange(0, total);
    frameSlider->setEnabled(total > 0);
    if (!m_seeking) {
        frameSlider->setValue(m_frameIndex);
    }
    if (labelFrameBadge) {
        labelFrameBadge->setText(QString("%1 / %2").arg(m_frameIndex).arg(total));
    }
}

void MainWindow::seekToFrame(int index) {
    if (!currentModule || m_frames.empty()) return;
    int total = static_cast<int>(m_frames.size());
    if (index < 0) index = 0;
    if (index > total) index = total;
    bool previousAnimationState = currentModule->m_animated;
    currentModule->m_animated = false;
    rebuildCurrentLayout(m_recordedInput, false, false);
    currentModule->refreshLiveSlots();
    for (int i = 0; i < index; ++i) {
        currentModule->renderFrame(m_frames[i]);
    }
    currentModule->m_animated = previousAnimationState;
    m_frameIndex = index;
    adjustSceneBoundary(false);
}

void MainWindow::onFrameSliderMoved(int value) {
    if (!currentModule || m_frames.empty()) return;
    m_seeking = true;
    seekToFrame(value);
    labelStatus->setText(QString("已定位到第 %1 帧").arg(m_frameIndex));
}

void MainWindow::onFrameSliderReleased(int value) {
    m_seeking = false;
    onFrameSliderMoved(value);
    syncFrameSlider();
}

void MainWindow::clearScene() {
    if (currentModule) currentModule->releaseSceneItems();
    graphicsScene->clear();
}
void MainWindow::rebuildCurrentLayout(const QString& inputData, bool animated, bool centerView) {
    if (!currentModule) return;
    bool previousAnimationState = currentModule->m_animated;
    currentModule->m_animated = animated;
    clearScene();
    currentModule->clearLiveSlots();
    currentModule->buildLayout(graphicsScene, inputData);
    currentModule->m_animated = previousAnimationState;
    m_builtInput = inputData;
    m_layoutBuilt = true;
    currentModule->enableLiveEditing();
    adjustSceneBoundary(centerView);
}

void MainWindow::adjustSceneBoundary(bool centerView) {
    ViewportFitter::Options options;
    ViewportFitter::expandSceneRect(graphicsScene, options);
    if (centerView) {
        ViewportFitter::fitToItems(graphicsView, graphicsScene, options);
    }
}
