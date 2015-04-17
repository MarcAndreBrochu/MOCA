#include "Solid.h"

Solid::Solid(bool isHollow) {
    _isHollow = isHollow;
}
Solid::~Solid() {}

void Solid::setAngularPosition(double tx, double ty, double tz) {
    _angularPosition[0] = tx;
    _angularPosition[1] = ty;
    _angularPosition[2] = tz;
}

void Solid::setAngularVelocity(double wx, double wy, double wz) {
    _angularVelocity[0] = wx;
    _angularVelocity[1] = wy;
    _angularVelocity[2] = wz;
}
