#include "FreeStructureBase.h"
#include "ui/UiTheme.h"
#include <QFont>
#include <QRegularExpression>

void FreeStructureBase::buildLayout(QGraphicsScene* scene, const QString& inputData) {
    m_scene = scene;
    reset();
    m_scene = scene;
    Q_UNUSED(inputData);
}

void FreeStructureBase::showHint(const QString& message) {
    Q_UNUSED(message);
}

void FreeStructureBase::parseKeyValues(const QString& inputData, std::vector<QPair<QString, QString>>& out) {
    out.clear();
    QStringList lines = inputData.split(QRegularExpression("[\\r\\n]"), Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        int sep = line.indexOf('=');
        if (sep < 0) continue;
        QString key = line.left(sep).trimmed();
        QString value = line.mid(sep + 1).trimmed();
        if (key.isEmpty()) continue;
        out.push_back(qMakePair(key, value));
    }
}

std::vector<AnimFrame> FreeStructureBase::runAlgorithm() {
    m_frames.clear();
    return m_frames;
}

void FreeStructureBase::renderFrame(const AnimFrame&) {}

void FreeStructureBase::reset() {
    m_frames.clear();
    m_mathText = nullptr;
    m_valid = false;
}

void FreeStructureBase::releaseSceneItems() {
    m_mathText = nullptr;
}
