#ifndef UITHEME_H
#define UITHEME_H
#include <QColor>
#include <QFont>
#include <QString>

namespace UiTheme {

inline QColor accent()         { return QColor(59, 125, 221); }
inline QColor accentHover()    { return QColor(47, 111, 189); }
inline QColor accentPressed()  { return QColor(38, 92, 158); }
inline QColor accentSoft()     { return QColor(233, 241, 253); }
inline QColor accentSoftDeep() { return QColor(213, 229, 250); }

inline QColor success()        { return QColor(52, 199, 89); }
inline QColor successHover()   { return QColor(44, 178, 78); }
inline QColor successSoft()    { return QColor(231, 248, 236); }

inline QColor surface()        { return QColor(255, 255, 255); }
inline QColor surfaceAlt()     { return QColor(246, 248, 251); }
inline QColor surfaceSunken()  { return QColor(238, 241, 246); }
inline QColor surfaceHover()   { return QColor(241, 245, 251); }

inline QColor border()         { return QColor(223, 228, 236); }
inline QColor borderSoft()     { return QColor(196, 216, 243); }
inline QColor borderStrong()   { return QColor(198, 205, 219); }
inline QColor borderFocus()    { return QColor(120, 165, 232); }

inline QColor textPrimary()    { return QColor(31, 39, 51); }
inline QColor textSecondary()  { return QColor(107, 118, 136); }
inline QColor textMuted()      { return QColor(154, 164, 180); }
inline QColor textOnAccent()   { return QColor(255, 255, 255); }

inline QColor trackOff()       { return QColor(200, 207, 218); }
inline QColor danger()         { return QColor(229, 72, 77); }
inline QColor canvasBg()       { return QColor(251, 252, 254); }
inline QColor canvasGrid()     { return QColor(226, 233, 243); }
inline QColor canvasGridAxis() { return QColor(205, 217, 235); }

inline int radiusSm()   { return 6; }
inline int radiusMd()   { return 9; }
inline int radiusLg()   { return 14; }
inline int radiusPill() { return 999; }

inline int spaceXs() { return 4; }
inline int spaceSm() { return 8; }
inline int spaceMd() { return 12; }
inline int spaceLg() { return 18; }
inline int spaceXl() { return 26; }

inline QFont uiFont(int pointSize = 10, bool bold = false) {
    QFont f("Microsoft YaHei UI");
    if (f.family().isEmpty() || f.family().contains("Sans")) f = QFont("Segoe UI");
    f.setPointSize(pointSize);
    f.setWeight(bold ? QFont::DemiBold : QFont::Normal);
    f.setHintingPreference(QFont::PreferFullHinting);
    f.setStyleStrategy(QFont::PreferQuality);
    return f;
}

inline QFont monoFont(int pointSize = 10, bool bold = false) {
    QFont f("Cascadia Mono");
    if (f.family().isEmpty() || f.family().contains("Sans")) f = QFont("Consolas");
    f.setPointSize(pointSize);
    f.setWeight(bold ? QFont::DemiBold : QFont::Normal);
    f.setStyleStrategy(QFont::PreferQuality);
    return f;
}

}
#endif
