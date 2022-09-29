#include "Hittable.h"

HittableSphere::HittableSphere():
    center{Point3{0.0, 0.0, 0.0}},
    radius{0.0}
{}
HittableSphere::HittableSphere(Point3 center, double radius, std::shared_ptr<Material> material):
    center{center},
    radius{radius},
    mat_ptr{material}
{}

bool HittableSphere::hit(Ray const& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 const oc = r.origin - this->center;
    double a = r.direction.length_squared();
    double b_half = r.direction.dot(oc);
    double c = oc.dot(oc) - this->radius * this->radius;

    double discriminant = b_half * b_half - a * c;
    if (discriminant < 0.0) return false;
    double sqrt_dcm = sqrt(discriminant);

    double root = (-b_half - sqrt_dcm) / a;
    if (root < t_min || t_max < root) {
        root = (-b_half + sqrt_dcm) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.at(root);
    Vec3 outward_normal = (rec.point - this->center) / this->radius;
    r.set_hit_recodrd_normal(outward_normal, rec);
    rec.mat_ptr = mat_ptr;
    return true;
}

HittableSphereMoving::HittableSphereMoving():
    center0{},
    center1{},
    time0{},
    time1{},
    radius{},
    mat_ptr{}
{}
HittableSphereMoving::HittableSphereMoving(Point3 cen0, Point3 cen1, double _time0, double _time1,
                                           double radius, std::shared_ptr<Material> material):
    center0{cen0},
    center1{cen1},
    time0{_time0},
    time1{_time1},
    radius{radius},
    mat_ptr{material}
{}

bool Hittable::bounding_box(double time0, double time1, AABB &output_box) const {
    return true;
}

bool HittableSphereMoving::hit(Ray const& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 const oc = r.origin - this->center(r.tm);
    double a = r.direction.length_squared();
    double b_half = r.direction.dot(oc);
    double c = oc.dot(oc) - this->radius * this->radius;

    double discriminant = b_half * b_half - a * c;
    if (discriminant < 0.0) return false;
    double sqrt_dcm = sqrt(discriminant);

    double root = (-b_half - sqrt_dcm) / a;
    if (root < t_min || t_max < root) {
        root = (-b_half + sqrt_dcm) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.at(root);
    Vec3 outward_normal = (rec.point - this->center(r.tm)) / this->radius;
    r.set_hit_recodrd_normal(outward_normal, rec);
    rec.mat_ptr = mat_ptr;
    return true;
}
bool HittableSphereMoving::bounding_box(double _time0, double _time1, AABB& output_box) const {
    AABB box0{
        this->center(_time0) - Vec3{radius, radius, radius},
        this->center(_time0) + Vec3{radius, radius, radius},
    };
    AABB box1{
        this->center(_time1) - Vec3{radius, radius, radius},
        this->center(_time1) + Vec3{radius, radius, radius},
    };
    output_box = box0.joint(box1);
    return true;
}

Point3 HittableSphereMoving::center(double time) const {
    return this->center0 + ((time - this->time0) / (this->time1 - this->time0)) * (this->center1 - this->center0);
}

