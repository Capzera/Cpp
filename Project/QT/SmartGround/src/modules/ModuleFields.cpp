#include "ModuleFields.h"

ModuleField ModuleField::makeToggle(const QString& key, const QString& label, bool value) {
    ModuleField f;
    f.type = Toggle;
    f.key = key;
    f.label = label;
    f.boolValue = value;
    return f;
}

ModuleField ModuleField::makeSegment(const QString& key, const QString& label, const QStringList& options, int index) {
    ModuleField f;
    f.type = Segment;
    f.key = key;
    f.label = label;
    f.options = options;
    f.intValue = index;
    return f;
}

ModuleField ModuleField::makeInt(const QString& key, const QString& label, int value, int minValue, int maxValue) {
    ModuleField f;
    f.type = IntInput;
    f.key = key;
    f.label = label;
    f.intValue = value;
    f.minValue = minValue;
    f.maxValue = maxValue;
    return f;
}

ModuleField ModuleField::makeText(const QString& key, const QString& label, const QString& value) {
    ModuleField f;
    f.type = TextInput;
    f.key = key;
    f.label = label;
    f.textValue = value;
    return f;
}

ModuleField ModuleField::makeSize(const QString& key, const QString& label,
                                  int width, int widthMin, int widthMax,
                                  int height, int heightMin, int heightMax) {
    ModuleField f;
    f.type = SizeInput;
    f.key = key;
    f.label = label;
    f.intValue = width;
    f.minValue = widthMin;
    f.maxValue = widthMax;
    f.secondaryIntValue = height;
    f.secondaryMinValue = heightMin;
    f.secondaryMaxValue = heightMax;
    return f;
}
