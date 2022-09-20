#include "Vec3.h"

Vec3::Vec3():
    e{0.0, 0.0, 0.0}
{}
Vec3::Vec3(double e0, double e1, double e2):
    e{e0, e1, e2}
{}

Vec3 Vec3::operator-() const {
    return Vec3(-e[0], -e[1], -e[2]);
}
Vec3& Vec3::operator+=(Vec3 const& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}
Vec3& Vec3::operator*=(double d) {
    e[0] *= d;
    e[1] *= d;
    e[2] *= d;
    return *this;
}
Vec3& Vec3::operator/=(double d) {
    *this *= (1.0 / d);
    return *this;
}

double Vec3::length_squared() const {
    return sqrt(this->length_squared());
}
double Vec3::length() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}
double Vec3::dot(Vec3 const& v) {
    return e[0]*v.e[0] + e[1]*v.e[1] + e[2]*v.e[2];
}
Vec3 Vec3::cross(Vec3 const& v) {
    return Vec3{
        e[1]*v.e[2] - e[2]*v.e[1],
        e[2]*v.e[0] - e[0]*v.e[2],
        e[0]*v.e[1] - e[1]*v.e[0]
    };
}
Vec3& Vec3::normalize() {
    *this /= this->length();
    return *this;
}

Vec3 Vec3::operator+(Vec3 const& v) {
    return Vec3{ e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2] };
}
Vec3 Vec3::operator-(Vec3 const &v) {
    return Vec3{ e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2] };
}
Vec3 Vec3::operator*(Vec3 const &v) {
    return Vec3{ e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2] };
}
Vec3 Vec3::operator*(double d) {
    return Vec3{ e[0] * d, e[1] * d, e[2] * d };
}
Vec3 Vec3::operator/(double d) {
    return Vec3{ e[0] / d, e[1] / d, e[2] / d };
}

std::ostream& operator<<(std::ostream& out, Vec3 const& v) {
    out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    return out;
}
Vec3 operator*(double d, Vec3 const& v) {
    return Vec3{ v.e[0] / d, v.e[1] / d, v.e[2] / d };
}