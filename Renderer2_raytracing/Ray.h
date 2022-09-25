#pragma once

#include <memory>

#include "Vec3.h"

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    double t;
    bool is_hit_front_face;
    std::shared_ptr<Material> mat_ptr;
};


class Ray {
public:
    Point3 origin;
    Vec3 direction;
    double tm;

    Ray();
    explicit Ray(Point3 const &origin, Vec3 const &direction, double time = 0.0);

    Point3 at(double t) const {
        return origin + t * direction;
    }

    void set_hit_recodrd_normal(Vec3 const &outward_normal, HitRecord &rec) const;
};

