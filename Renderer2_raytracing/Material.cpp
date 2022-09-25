#include "Material.h"

MaterialLambertian::MaterialLambertian(Color const &color):
    albedo{color}
{}
bool MaterialLambertian::scatter(Ray const& r_in, HitRecord const& rec,
                                 Color& attenuation, Ray& scattered) const {
    Vec3 scatter_direction = rec.normal + util_rand::random_unit_vector();
    if (scatter_direction.is_near_zero())
        scatter_direction = rec.normal;
    scattered = Ray{rec.point, scatter_direction};
    attenuation = albedo;
    return true;
}

MaterialMetal::MaterialMetal(Color const &color, double fuzz):
    albedo{color},
    fuzz{std::min(fuzz, 1.0)}
{}
bool MaterialMetal::scatter(Ray const& r_in, HitRecord const& rec,
                                 Color& attenuation, Ray& scattered) const {
    Vec3 reflected = r_in.direction.reflect(rec.normal);
    scattered = Ray{rec.point, reflected
        + this->fuzz * util_rand::random_vector_in_unit_sphere()};
    attenuation = this->albedo;
    return (scattered.direction.dot(rec.normal) > 0.0);
}

MaterialDielectric::MaterialDielectric(double index_of_refraction):
    ir{index_of_refraction}
{}
bool MaterialDielectric::scatter(Ray const& r_in, HitRecord const& rec,
                                 Color& attenuation, Ray& scattered) const {
    attenuation = Color{1.0, 1.0, 1.0};
    double refration_ratio = rec.is_hit_front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = r_in.direction;
    unit_direction.normalize();

    double cos_theta = std::min(rec.normal.dot(-unit_direction), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_reflect = refration_ratio * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_reflect ||
        MaterialDielectric::reflectance(cos_theta, refration_ratio) >
            util_rand::random_double()) {
        direction = unit_direction.reflect(rec.normal);
    } else {
        direction = unit_direction.refract(rec.normal, refration_ratio);
    }

    scattered = Ray{rec.point, direction};
    return true;
}
double MaterialDielectric::reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
}
