#pragma once

#include <cmath>
#include <iostream>

class alignas(sizeof(double)*4) Vec3 {
public:
    double e[3];

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    double &operator[](int i) { return e[i]; }
    double const &operator[](int i) const { return e[i]; }

    Vec3();
    explicit Vec3(double e0, double e1, double e2);

    Vec3 &operator+=(Vec3 const &v);
    Vec3 &operator*=(double d);
    Vec3 &operator/=(double d);
    double length_squared() const;
    double length() const;
    double dot(Vec3 const &v) const;
    Vec3 cross(Vec3 const &v) const;
    Vec3 &normalize();
    Vec3 sqrt() const;
    Vec3 &make_sqrt();
    bool is_near_zero() const;
    Vec3 reflect(Vec3 const& n) const;
    Vec3 refract(Vec3 const &normal, double etai_over_etat) const;

    Vec3 operator-() const;
    Vec3 operator+(Vec3 const &v) const;
    Vec3 operator-(Vec3 const &v) const;
    Vec3 operator*(Vec3 const &v) const;
    Vec3 operator*(double d) const;
    Vec3 operator/(double d) const;
};

std::ostream& operator<<(std::ostream& out, Vec3 const& v);
Vec3 operator*(double d, Vec3 const &v);

typedef Vec3 Point3;
typedef Vec3 Color;