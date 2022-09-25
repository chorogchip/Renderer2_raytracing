#include "Ray.h"

Ray::Ray():
    origin{},
    direction{},
    tm{}
{}
Ray::Ray(Point3 const &origin, Vec3 const &direction, double time):
    origin{origin},
    direction{direction},
    tm{time}
{}

void Ray::set_hit_recodrd_normal(Vec3 const& outward_normal, HitRecord& rec) const {
    rec.is_hit_front_face = (this->direction.dot(outward_normal) < 0.0);
    rec.normal = rec.is_hit_front_face ? outward_normal : -outward_normal;
}