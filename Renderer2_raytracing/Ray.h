#pragma once

#include "Vec3.h"

class Ray {
public:
    Point3 origin;
    Vec3 direction;

    Ray();
    explicit Ray(Point3 const &origin, Vec3 const &direction);

    Point3 at(double t) const {
        return origin + t * direction;
    }
};

