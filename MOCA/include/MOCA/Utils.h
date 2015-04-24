#ifndef MOCA_UTILS_H
#define MOCA_UTILS_H

#include <limits>
#include <armadillo>

static const double M_PTM_RATIO = 8; // 1m = 32px

inline double MOCA_METERS_TO_PIXELS(double meters) {
    return meters * M_PTM_RATIO;
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
