#include "Ray.h"

Ray::Ray():
    origin{},
    direction{}
{}
Ray::Ray(Point3 const &origin, Vec3 const &direction):
    origin{origin},
    direction{direction}
{}