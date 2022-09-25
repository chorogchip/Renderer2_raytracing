#include "Camera.h"


Camera::Camera(): Camera(90, 16.0 / 9.0)
{}

Camera::Camera(double fov_y_degree, double aspect_ratio) {
    double const theta = util::degree_to_radian(fov_y_degree);
    double const h = std::tan(theta / 2.0);
    double const viewport_height = 2.0 * h;
    double const viewport_width = aspect_ratio * viewport_height;
    double const focal_length = 1.0;

    this->origin = Point3{0.0, 0.0, 0.0};
    this->horizontal = Vec3{viewport_width, 0.0, 0.0};
    this->vertical = Vec3{0.0, viewport_height, 0.0};
    this->lower_left_corner = this->origin - this->horizontal / 2.0 - this->vertical / 2.0
        - Vec3{0.0, 0.0, focal_length};

    this->lens_radius = 0.0;
    this->time0 = 0.0;
    this->time1 = 0.0;
}

Camera::Camera(Point3 look_from, Point3 look_at, Vec3 v_up,
               double fov_y_degree, double aspect_ratio) {
    double const theta = util::degree_to_radian(fov_y_degree);
    double const h = std::tan(theta / 2.0);
    double const viewport_height = 2.0 * h;
    double const viewport_width = aspect_ratio * viewport_height;
   
    Vec3 const w = (look_from - look_at).normalize();
    Vec3 const u = v_up.cross(w).normalize();
    Vec3 const v = w.cross(u);

    this->origin = look_from;
    this->horizontal = viewport_width * u;
    this->vertical = viewport_height * v;
    this->lower_left_corner =
        this->origin - this->horizontal / 2.0 - this->vertical / 2.0 - w;

    this->lens_radius = 0.0;
    this->time0 = 0.0;
    this->time1 = 0.0;
}

Ray Camera::get_ray(double u, double v) const {
    return Ray{this->origin, this->lower_left_corner
        + u * this->horizontal + v * this->vertical - this->origin};
}

Camera::Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double fov_y_degree,
               double aspect_ratio, double aperture, double focus_dist) {
    double const theta = util::degree_to_radian(fov_y_degree);
    double const h = std::tan(theta / 2.0);
    double const viewport_height = 2.0 * h;
    double const viewport_width = aspect_ratio * viewport_height;

    this->w = (look_from - look_at).normalize();
    this->u = v_up.cross(w).normalize();
    this->v = w.cross(u);

    this->origin = look_from;
    this->horizontal = focus_dist * viewport_width * u;
    this->vertical = focus_dist * viewport_height * v;
    this->lower_left_corner =
        this->origin - this->horizontal / 2.0 - this->vertical / 2.0 - focus_dist * w;
    this->lens_radius = aperture / 2.0;

    this->time0 = 0.0;
    this->time1 = 0.0;
}
Ray Camera::get_ray_defocus(double s, double t) const {
    Vec3 rd{lens_radius * util_rand::random_vector_in_unit_disk()};
    Vec3 offset{this->u * rd.x() + this->v * rd.y()};

    return Ray{this->origin + offset, this->lower_left_corner
        + s * this->horizontal + t * this->vertical - this->origin - offset};
}

Camera::Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double fov_y_degree,
               double aspect_ratio, double aperture, double focus_dist, double _time0, double _time1):
    Camera{look_from, look_at, v_up, fov_y_degree, aspect_ratio, aperture, focus_dist}
{
    this->time0 = _time0;
    this->time1 = _time1;
}