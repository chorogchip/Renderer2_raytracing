#include "AABB.h"

AABB::AABB():
    minimum{},
    maximum{}
{}

AABB::AABB(Point3 const &a, Point3 const &b):
    minimum{a},
    maximum{b}
{}

bool AABB::hit(Ray const& r, double t_min, double t_max) const {
    for (int a = 0; a < 3; ++a) {
        double invD = 1.0f / r.direction[a];
        double t0 = (minimum[a] - r.origin[a]) * invD;
        double t1 = (maximum[a] - r.origin[a]) * invD;
        if (invD < 0.0)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

AABB AABB::joint(AABB const& aabb) const {
    return AABB{
        Point3{std::min(this->minimum[0], aabb.minimum[0]),
               std::min(this->minimum[1], aabb.minimum[1]),
               std::min(this->minimum[2], aabb.minimum[2]),
        },
        Point3{std::max(this->maximum[0], aabb.maximum[0]),
               std::max(this->maximum[1], aabb.maximum[1]),
               std::max(this->maximum[2], aabb.maximum[2]),
        }
    };
}