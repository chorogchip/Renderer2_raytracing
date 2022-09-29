#pragma once
#include "Vec3.h"
#include "Ray.h"

class AABB {
public:
    Point3 minimum;
    Point3 maximum;

    AABB();
    AABB(Point3 const &a, Point3 const &b);

    bool hit(Ray const& r, double t_min, double t_max) const;
    AABB joint(AABB const &aabb) const;

};

