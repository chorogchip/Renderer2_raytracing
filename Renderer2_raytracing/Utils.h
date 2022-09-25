#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

class Vec3;

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

constexpr double INF_DBL = std::numeric_limits<double>::infinity();
constexpr double PI = 3.114159265358979323846264;

namespace util {

    constexpr inline double degree_to_radian(double degree) {
        return degree * PI / 180.0;
    }

};

namespace util_rand {

    double random_double();
    double random_double(double min, double max);
    Vec3 random_unit_vector();
    Vec3 random_unit_vector(double min, double max);
    Vec3 random_vector_in_unit_sphere();
    Vec3 random_vector_on_unit_sphere();
    Vec3 random_vector_in_hemisphere(Vec3 const &normal);
    Vec3 random_vector_in_unit_disk();

};