#ifndef MOCA_UTILS_H
#define MOCA_UTILS_H

#include <limits>
#include <armadillo>

static const double M_PTM_RATIO = 16.0; // 1m = 16px

inline double MOCA_PIXELS_TO_METERS(double pixels) {
    return pixels / M_PTM_RATIO;
}

inline bool MOCA_GREATER_THAN_ZERO(double value) {
    return value > std::numeric_limits<double>::epsilon();
}

inline arma::vec3 MOCA_MAKE_VEC3(double x, double y, double z) {
    arma::vec3 ret;
    ret[0] = x;
    ret[1] = y;
    ret[2] = z;

    return ret;
}

#endif // MOCA_UTILS_H
