#ifndef APPVERSION_H
#define APPVERSION_H
#include <QString>

namespace AppVersion {

inline const char* kBaseVersion = "0.7";
inline const char* kFallbackStamp = "0";

#if __has_include("AppVersionGenerated.h")
#include "AppVersionGenerated.h"
inline const char* kBuildStamp = AppVersionBuild::kStamp;
#else
inline const char* kBuildStamp = kFallbackStamp;
#endif

inline QString display() {
    return QString::fromLatin1(kBaseVersion) + QLatin1String(".")
         + QString::fromLatin1(kBuildStamp);
}

inline QString tooltip() {
    return QStringLiteral("SmartGround v") + display();
}

}

#endif
