#ifndef MOCA_UTILS_H
#define MOCA_UTILS_H

#include <limits>

static const double M_PTM_RATIO = 8; // 1m = 32px

inline double MOCA_METERS_TO_PIXELS(double meters) {
    return meters * M_PTM_RATIO;
}

inline bool MOCA_GREATER_THAN_ZERO(double value) {
    return value > std::numeric_limits<double>::epsilon();
}

#endif // MOCA_UTILS_H
