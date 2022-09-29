#include "HittableList.h"

HittableList::HittableList() {}
HittableList::HittableList(Hittable_ptr object) {
    this->add(object);
}

void HittableList::clear() {
    this->objects.clear();
}
void HittableList::add(Hittable_ptr object) {
    this->objects.push_back(object);
}
bool HittableList::hit(Ray const& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord rec_tmp;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (auto const& object : objects) {
        if (object->hit(r, t_min, closest_so_far, rec_tmp)) {
            hit_anything = true;
            closest_so_far = rec_tmp.t;
            rec = rec_tmp;
        }
    }
    
    return hit_anything;
}
bool HittableList::bounding_box(double time0, double time1, AABB& output_box) const {
    if (objects.empty()) return false;
    AABB temp_box;
    bool first_box = true;
    for (auto const& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : output_box.joint(temp_box);
        first_box = false;
    }
    return true;
}