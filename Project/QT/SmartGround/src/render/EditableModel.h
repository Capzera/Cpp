#ifndef EDITABLEMODEL_H
#define EDITABLEMODEL_H
#include <QString>
#include <QHash>

class EditableModel {
public:
    static EditableModel& instance() {
        static EditableModel s_instance;
        return s_instance;
    }

    void clear() { m_values.clear(); }

    void clearScope(const QString& scope) {
        QString prefix = scope + "|";
        for (auto it = m_values.constBegin(); it != m_values.constEnd(); ++it) {
            if (it.key().startsWith(prefix)) m_values.erase(it);
        }
    }

    void setValue(const QString& scope, const QString& row, int index, int value) {
        if (scope.isEmpty() || row.isEmpty()) return;
        m_values[buildKey(scope, row, index)] = value;
    }

    bool getValue(const QString& scope, const QString& row, int index, int& out) const {
        if (scope.isEmpty() || row.isEmpty()) return false;
        auto it = m_values.constFind(buildKey(scope, row, index));
        if (it == m_values.constEnd()) return false;
        out = it.value();
        return true;
    }

    bool hasValue(const QString& scope, const QString& row, int index) const {
        if (scope.isEmpty() || row.isEmpty()) return false;
        return m_values.contains(buildKey(scope, row, index));
    }

    void removeValue(const QString& scope, const QString& row, int index) {
        m_values.remove(buildKey(scope, row, index));
    }

    QString buildKey(const QString& scope, const QString& row, int index) const {
        return QString("%1|%2#%3").arg(scope, row).arg(index);
    }

private:
    QHash<QString, int> m_values;
};

#endif
