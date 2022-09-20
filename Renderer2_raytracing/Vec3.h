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
    Vec3(double e0, double e1, double e2);

    Vec3 operator-() const;
    Vec3 &operator+=(Vec3 const &v);
    Vec3 &operator*=(double d);
    Vec3 &operator/=(double d);
    double length_squared() const;
    double length() const;
    double dot(Vec3 const &v);
    Vec3 cross(Vec3 const &v);
    Vec3 &normalize();

    Vec3 operator+(Vec3 const &v);
    Vec3 operator-(Vec3 const &v);
    Vec3 operator*(Vec3 const &v);
    Vec3 operator*(double d);
    Vec3 operator/(double d);
};

std::ostream& operator<<(std::ostream& out, Vec3 const& v);
Vec3 operator*(double d, Vec3 const &v);

typedef Vec3 Point3;
typedef Vec3 Color;