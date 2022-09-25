#include "Vec3.h"

Vec3::Vec3():
    e{0.0, 0.0, 0.0}
{}
Vec3::Vec3(double e0, double e1, double e2):
    e{e0, e1, e2}
{}

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

double Vec3::length() const {
    return std::sqrt(this->length_squared());
}
double Vec3::length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}
double Vec3::dot(Vec3 const& v) const {
    return e[0]*v.e[0] + e[1]*v.e[1] + e[2]*v.e[2];
}
Vec3 Vec3::cross(Vec3 const& v) const {
    return Vec3{
        e[1]*v.e[2] - e[2]*v.e[1],
        e[2]*v.e[0] - e[0]*v.e[2],
        e[0]*v.e[1] - e[1]*v.e[0]
    };
}
Vec3& Vec3::normalize() {
    double len = this->length();
    *this /= len;
    return *this;
}
Vec3 Vec3::sqrt() const {
    return Vec3 {
        std::sqrt(this->e[0]),
        std::sqrt(this->e[1]),
        std::sqrt(this->e[2])
    };
}
Vec3& Vec3::make_sqrt() {
    this->e[0] = std::sqrt(this->e[0]);
    this->e[1] = std::sqrt(this->e[1]);
    this->e[2] = std::sqrt(this->e[2]);
    return *this;
}
bool Vec3::is_near_zero() const {
    constexpr double s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}
Vec3 Vec3::reflect(Vec3 const& n) const {
    return (*this) - (2.0 * this->dot(n) * n);
}
Vec3 Vec3::refract(Vec3 const& normal, double etai_over_etat) const {
    double cos_theta = std::min(normal.dot(-*this), 1.0);
    Vec3 r_out_perp = etai_over_etat * (*this + cos_theta * normal);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_parallel;
}

Vec3 Vec3::operator-() const {
    return Vec3(-e[0], -e[1], -e[2]);
}
Vec3 Vec3::operator+(Vec3 const& v) const {
    return Vec3{ e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2] };
}
Vec3 Vec3::operator-(Vec3 const &v) const {
    return Vec3{ e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2] };
}
Vec3 Vec3::operator*(Vec3 const &v) const {
    return Vec3{ e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2] };
}
Vec3 Vec3::operator*(double d) const {
    return Vec3{ e[0] * d, e[1] * d, e[2] * d };
}
Vec3 Vec3::operator/(double d) const {
    return Vec3{ e[0] / d, e[1] / d, e[2] / d };
}

std::ostream& operator<<(std::ostream& out, Vec3 const& v) {
    out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    return out;
}
Vec3 operator*(double d, Vec3 const& v) {
    return Vec3{ v.e[0] * d, v.e[1] * d, v.e[2] * d };
}