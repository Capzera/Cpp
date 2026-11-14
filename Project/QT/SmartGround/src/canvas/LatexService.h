#ifndef LATEXSERVICE_H
#define LATEXSERVICE_H
#include <QObject>
#include <QProcess>
#include <QHash>
#include <QString>

class LatexService : public QObject {
    Q_OBJECT
public:
    static LatexService* instance();

    bool isAvailable() const;
    void request(const QString& tex);
    QString lastError() const { return m_lastError; }

signals:
    void rendered(const QString& tex, const QString& svg, double width, double height);
    void renderFailed(const QString& tex, const QString& error);

private:
    explicit LatexService(QObject* parent = nullptr);
    void ensureStarted();
    void handleLine(const QString& line);

    QProcess* m_process = nullptr;
    int m_nextId = 1;
    QHash<int, QString> m_pending;
    QString m_lastError;
    bool m_failed = false;
};

#endif
