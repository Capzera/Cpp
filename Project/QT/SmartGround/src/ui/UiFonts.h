#ifndef UIFONTS_H
#define UIFONTS_H
#include <QFont>
#include <QFontMetrics>
#include <QString>
#include <QStringList>

namespace UiFonts {

inline const QString& cjkFamily() {
    static const QString family = QStringLiteral("Microsoft YaHei");
    return family;
}

inline const QString& monoFamily() {
    static const QString family = QStringLiteral("Consolas");
    return family;
}

inline QFont make(const QString& family, int pointSize, bool bold = false) {
    QFont f(family);
    f.setPointSize(pointSize);
    f.setWeight(bold ? QFont::DemiBold : QFont::Normal);
    f.setHintingPreference(QFont::PreferFullHinting);
    f.setStyleStrategy(QFont::PreferQuality);
    return f;
}

inline QFont cjk(int pointSize = 10, bool bold = false) {
    return make(cjkFamily(), pointSize, bold);
}

inline QFont mono(int pointSize = 10, bool bold = false) {
    return make(monoFamily(), pointSize, bold);
}

inline QFont ui(int pointSize = 10, bool bold = false) {
    return cjk(pointSize, bold);
}

inline QFont cjkItalic(int pointSize = 10, bool bold = false) {
    QFont f = cjk(pointSize, bold);
    f.setItalic(true);
    return f;
}

inline QFont mixed(int pointSize = 10, bool bold = false) {
    QFont f = mono(pointSize, bold);
    f.setFamilies({monoFamily(), cjkFamily()});
    return f;
}

inline bool needsCjk(const QString& text) {
    for (const QChar& ch : text) {
        if (ch.unicode() >= 0x2E80) return true;
    }
    return false;
}

inline QFont forText(const QString& text, int pointSize = 10, bool bold = false) {
    return needsCjk(text) ? mixed(pointSize, bold) : mono(pointSize, bold);
}

inline int cjkTextWidth(const QString& text, int pointSize = 10, bool bold = false) {
    return QFontMetrics(cjk(pointSize, bold)).horizontalAdvance(text);
}

inline int monoTextWidth(const QString& text, int pointSize = 10, bool bold = false) {
    return QFontMetrics(mono(pointSize, bold)).horizontalAdvance(text);
}

}  // namespace UiFonts
#endif
