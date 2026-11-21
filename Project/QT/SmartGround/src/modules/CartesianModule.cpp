#include "CartesianModule.h"
#include "core/CartesianExpression.h"
#include "ui/UiFonts.h"
#include <QGraphicsScene>
#include <cmath>

CartesianModule::CartesianModule() {
    m_engine = new CartesianEngine(this);
    connect(m_engine, &CartesianEngine::objectsChanged, this, [this]() {
        emit objectsChanged();
    });
    connect(m_engine, &CartesianEngine::statusMessage, this, [this](const QString& text) {
        emit statusMessage(text);
    });
    connect(m_engine, &CartesianEngine::pointDoubleClicked, this, [this](int index) {
        emit requestPointEditor(index);
    });
    connect(m_engine, &CartesianEngine::segmentDoubleClicked, this, [this](int index) {
        emit requestSegmentEditor(index);
    });
    connect(m_engine, &CartesianEngine::curveDoubleClicked, this, [this](int index) {
        emit requestFunctionEditor(index);
    });
}

QString CartesianModule::getModuleName() const { return "自由结构 - 笛卡尔坐标系"; }

QString CartesianModule::getDefaultInput() const { return QString(); }

QString CartesianModule::getInputHint() const {
    return "笛卡尔坐标系是一个可以直接作图的自由画布：\n"
           "  滚轮缩放：最小分度尺会在 1.0 / 0.5 / 0.2 / 0.1 / 0.05 / 0.02 / 0.01 之间自适应\n"
           "  画点工具：单击落点，长按拖动可实时跟随并吸附到网格交点\n"
           "  连线工具：奇数次选点高亮起点，偶数次选点连成线段，双击线段看一次方程\n"
           "  左侧列表：可录入 f(x) = 表达式，直接绘制一次 / 二次 / 三次函数图像";
}

QString CartesianModule::moduleSettingsTitle() const { return "坐标系设置"; }

QVector<ModuleField> CartesianModule::moduleSettingsFields() const {
    QVector<ModuleField> fields;
    const CartesianEngine::Settings& settings = m_engine->settings();
    fields.append(ModuleField::makeToggle("grid", "显示网格", settings.showGrid));
    fields.append(ModuleField::makeToggle("axis", "显示刻度数字", settings.showAxisNumber));
    fields.append(ModuleField::makeToggle("pointName", "描点时显示名字", settings.showPointName));
    fields.append(ModuleField::makeToggle("equation", "显示线段与函数方程", settings.showEquation));
    return fields;
}

void CartesianModule::applyModuleSetting(const QString& key, const ModuleField& field) {
    CartesianEngine::Settings settings = m_engine->settings();
    if (key == "grid") settings.showGrid = field.boolValue;
    else if (key == "axis") settings.showAxisNumber = field.boolValue;
    else if (key == "pointName") settings.showPointName = field.boolValue;
    else if (key == "equation") settings.showEquation = field.boolValue;
    m_engine->setSettings(settings);
}

void CartesianModule::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;

    if (!m_engine) m_engine = new CartesianEngine(this);
    m_engine->attachScene(scene);
    m_engine->setEnabled(true);
    m_engine->updateViewport(m_viewportRect);
    Q_UNUSED(inputData);
}

std::vector<AnimFrame> CartesianModule::runAlgorithm() {
    m_frames.clear();
    return m_frames;
}

void CartesianModule::renderFrame(const AnimFrame&) {}

void CartesianModule::reset() {
    FreeStructureBase::reset();
    if (m_engine) {
        m_engine->clearObjects();
        m_engine->setEnabled(false);
        m_engine->detachScene();
    }
}

void CartesianModule::releaseSceneItems() {
    if (m_engine) {
        m_engine->setEnabled(false);
        m_engine->clearObjects();
        m_engine->detachScene();
    }
    m_mathText = nullptr;
}

void CartesianModule::setViewportRect(const QRectF& rect) {
    m_viewportRect = rect;
    if (m_engine) m_engine->updateViewport(rect);
}

void CartesianModule::setViewScale(qreal scale) {
    if (!m_engine) return;
    m_engine->setScale(scale);
}
