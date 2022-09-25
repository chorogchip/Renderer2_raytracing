#include "Utils.h"

#include "Vec3.h"

namespace util {

};

namespace util_rand {

double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
double random_double(double min, double max) {
    return min + (max - min) * random_double();
}
Vec3 random_unit_vector() {
    return Vec3{
        random_double(),
        random_double(),
        random_double()
    };
}
Vec3 random_unit_vector(double min, double max) {
    return Vec3{
        random_double(min, max),
        random_double(min, max),
        random_double(min, max)
    };
}
Vec3 random_vector_in_unit_sphere() {
    while (true) {
        Vec3 vec = random_unit_vector(-1.0, 1.0);
        if (vec.length_squared() < 1.0) return vec;
    }
}
Vec3 random_vector_on_unit_sphere() {
    while (true) {
        Vec3 vec = random_unit_vector(-1.0, 1.0);
        if (vec.length_squared() < 1.0) return vec.normalize();
    }
}
Vec3 random_vector_in_hemisphere(Vec3 const &normal) {
    Vec3 in_unit_sphere = util_rand::random_vector_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}
Vec3 random_vector_in_unit_disk() {
    while (true) {
        Vec3 p{random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0};
        if (p.length_squared() < 1.0)
            return p;
    }
}


};