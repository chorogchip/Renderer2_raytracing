#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable {
    typedef std::shared_ptr<Hittable> Hittable_ptr;
public:
    std::vector<Hittable_ptr> objects;

    HittableList();
    HittableList(Hittable_ptr object);

    void clear();
    void add(Hittable_ptr object);
    virtual bool hit(Ray const &r, double t_min, double t_max, HitRecord &rec) const override;
};

