#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

class Hittable {
public:
    virtual bool hit(Ray const &r, double t_min, double t_max, HitRecord &rec) const = 0;
};

class HittableSphere : public Hittable {
public:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr;

    HittableSphere();
    HittableSphere(Point3 center, double radius, std::shared_ptr<Material> material);

    virtual bool hit(Ray const &r, double t_min, double t_max, HitRecord &rec) const override;
};


class HittableSphereMoving : public Hittable {
public:
    Point3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<Material> mat_ptr;

    HittableSphereMoving();
    HittableSphereMoving(Point3 cen0, Point3 cen1, double _time0, double _time1,
                         double radius, std::shared_ptr<Material> material);

    virtual bool hit(Ray const &r, double t_min, double t_max, HitRecord &rec) const override;
    Point3 center(double time) const;
};
