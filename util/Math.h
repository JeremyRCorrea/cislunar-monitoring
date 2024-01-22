#pragma once
#include "Cartesian.h"

double sgn(double x) {
    if (x < 0.0) {
        return -1.0;
    }
    return 1.0;
} // sgn

double angle(double3 vec1, double3 vec2) {
    double small, magv1, magv2, temp;
    small = 0.00000001;



    magv1 = magnitude(vec1);
    magv2 = magnitude(vec2);

    if (magv1 * magv2 > small * small)
    {
        temp = dot(vec1, vec2) / (magv1 * magv2);
        if (fabs(temp) > 1.0)
            temp = sgn(temp) * 1.0;
        return acos(temp);
    }
    else
        return 99.9;
}  // angle