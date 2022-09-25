#pragma once

#include "Utils.h"
#include "Vec3.h"
#include "Ray.h"

class Material {
public:
    virtual bool scatter(Ray const &r_in, HitRecord const &rec,
                         Color &attenuation, Ray &scattered) const = 0;
};

class MaterialLambertian : public Material {
public:
    Color albedo;

    MaterialLambertian(Color const &color);

    virtual bool scatter(Ray const &r_in, HitRecord const &rec,
                         Color &attenuation, Ray &scattered) const override;
};
class MaterialMetal : public Material {
public:
    Color albedo;
    double fuzz;

    MaterialMetal(Color const &color, double fuzz);

    virtual bool scatter(Ray const &r_in, HitRecord const &rec,
                         Color &attenuation, Ray &scattered) const override;
};
class MaterialDielectric : public Material {
public:
    double ir;  // index of refraction

    MaterialDielectric(double index_of_refraction);

    virtual bool scatter(Ray const &r_in, HitRecord const &rec,
                         Color &attenuation, Ray &scattered) const override;

private:
    static double reflectance(double cosine, double ref_idx);
};