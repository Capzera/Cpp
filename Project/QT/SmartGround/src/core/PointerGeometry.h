#ifndef POINTERGEOMETRY_H
#define POINTERGEOMETRY_H
#include <QPointF>

namespace PointerGeometry {

constexpr double kGap = 2.0;

inline QPointF aboveCell(double cellW, double cellH) {
    Q_UNUSED(cellH);
    return QPointF(cellW / 2.0, -kGap);
}

inline QPointF belowCell(double cellW, double cellH) {
    Q_UNUSED(cellH);
    return QPointF(cellW / 2.0, cellH + kGap);
}

inline QPointF leftOfCell(double cellW, double cellH) {
    Q_UNUSED(cellW);
    return QPointF(-kGap, cellH / 2.0);
}

inline QPointF rightOfCell(double cellW, double cellH) {
    Q_UNUSED(cellW);
    return QPointF(cellW + kGap, cellH / 2.0);
}

inline QPointF outsideNode(double radius, bool above) {
    return above ? QPointF(0.0, -(radius + kGap)) : QPointF(0.0, radius + kGap);
}

inline QPointF leftOfNode(double radius) {
    return QPointF(-(radius + kGap), 0.0);
}

inline QPointF rightOfNode(double radius) {
    return QPointF(radius + kGap, 0.0);
}

}

#endif
