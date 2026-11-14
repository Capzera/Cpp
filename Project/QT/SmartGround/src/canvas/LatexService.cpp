#include "LatexService.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

namespace {
QString findHelperScript() {
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        appDir + "/tools/latex/serve.js",
        appDir + "/../tools/latex/serve.js",
        appDir + "/../../tools/latex/serve.js",
        appDir + "/../../../tools/latex/serve.js",
        QDir::current().absoluteFilePath("tools/latex/serve.js")
    };
    for (const QString& path : candidates) {
        if (QFileInfo::exists(path)) return QFileInfo(path).absoluteFilePath();
    }
    return QString();
}

QString findNodeExe() {
    const QString local = QCoreApplication::applicationDirPath() + "/node.exe";
    if (QFileInfo::exists(local)) return local;
    return QStringLiteral("node");
}
}

LatexService* LatexService::instance() {
    static LatexService* s_instance = new LatexService();
    return s_instance;
}

LatexService::LatexService(QObject* parent) : QObject(parent) {}

bool LatexService::isAvailable() const {
    if (m_failed) return false;
    return !findHelperScript().isEmpty();
}

void LatexService::ensureStarted() {
    if (m_process) return;
    const QString script = findHelperScript();
    if (script.isEmpty()) {
        m_failed = true;
        m_lastError = "未找到 tools/latex/serve.js";
        return;
    }
    m_process = new QProcess(this);
    m_process->setWorkingDirectory(QFileInfo(script).absolutePath());
    connect(m_process, &QProcess::readyReadStandardOutput, this, [this]() {
        while (m_process->canReadLine()) {
            handleLine(QString::fromUtf8(m_process->readLine()).trimmed());
        }
    });
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError) {
        m_failed = true;
        m_lastError = "Node 渲染助手启动失败";
    });
    connect(m_process, &QProcess::finished, this, [this]() {
        if (m_process->exitStatus() != QProcess::NormalExit) {
            m_failed = true;
            m_lastError = "Node 渲染助手异常退出";
        }
    });
    m_process->start(findNodeExe(), {script});
    if (!m_process->waitForStarted(4000)) {
        m_failed = true;
        m_lastError = "无法启动 Node 渲染助手";
    }
}

void LatexService::handleLine(const QString& line) {
    if (line.isEmpty()) return;
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) return;
    const QJsonObject obj = doc.object();
    const int id = obj.value("id").toInt();
    const QString tex = m_pending.value(id);
    if (tex.isEmpty()) return;
    m_pending.remove(id);
    if (!obj.value("ok").toBool()) {
        emit renderFailed(tex, obj.value("error").toString());
        return;
    }
    emit rendered(tex,
                  obj.value("svg").toString(),
                  obj.value("width").toDouble(),
                  obj.value("height").toDouble());
}

void LatexService::request(const QString& tex) {
    if (tex.trimmed().isEmpty()) return;
    ensureStarted();
    if (!m_process || m_process->state() != QProcess::Running) {
        emit renderFailed(tex, m_lastError.isEmpty() ? QString("LaTeX 渲染服务不可用") : m_lastError);
        return;
    }
    const int id = m_nextId++;
    m_pending.insert(id, tex);
    QJsonObject req;
    req["id"] = id;
    req["src"] = tex;
    m_process->write(QJsonDocument(req).toJson(QJsonDocument::Compact) + "\n");
}
