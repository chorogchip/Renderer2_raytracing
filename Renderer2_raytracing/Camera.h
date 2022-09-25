#pragma once

#include "Utils.h"
#include "Vec3.h"
#include "Ray.h"

class Camera {
private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lens_radius;
    double time0, time1;
public:
    Camera();
    Camera(double fov_y_degree, double aspect_ratio);
    Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double fov_y_degree, double aspect_ratio);
    Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double fov_y_degree,
           double aspect_ratio, double aperture, double focus_dist);
    Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double fov_y_degree,
           double aspect_ratio, double aperture, double focus_dist, double _time0, double _time1);

    Ray get_ray(double u, double v) const;
    Ray get_ray_defocus(double s, double t) const;

};

