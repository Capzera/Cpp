#ifndef MODULEFIELDS_H
#define MODULEFIELDS_H
#include <QString>
#include <QStringList>
#include <QVector>

struct ModuleField {
    enum Type { Toggle, Segment, IntInput, TextInput, SizeInput };

    Type type = Toggle;
    QString key;
    QString label;
    QStringList options;
    bool boolValue = false;
    int intValue = 0;
    int minValue = 1;
    int maxValue = 99;
    QString textValue;
    QString hint;
    int secondaryIntValue = 0;
    int secondaryMinValue = 1;
    int secondaryMaxValue = 99;

    static ModuleField makeToggle(const QString& key, const QString& label, bool value);
    static ModuleField makeSegment(const QString& key, const QString& label, const QStringList& options, int index);
    static ModuleField makeInt(const QString& key, const QString& label, int value, int minValue, int maxValue);
    static ModuleField makeText(const QString& key, const QString& label, const QString& value);
    static ModuleField makeSize(const QString& key, const QString& label,
                               int width, int widthMin, int widthMax,
                               int height, int heightMin, int heightMax);
};

#endif
