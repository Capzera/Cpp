#include "IModule.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPointer>

namespace IModuleHelpers {

inline void releaseItem(QPointer<QGraphicsItem>& p) {
    p = nullptr;
}

}
