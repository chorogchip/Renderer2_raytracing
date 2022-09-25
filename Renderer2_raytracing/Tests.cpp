#include "Tests.h"


void Tests::test1_uv() {

    int const image_width = 256;
    int const image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{
                static_cast<double>(i) / static_cast<double>(image_width-1),
                static_cast<double>(j) / static_cast<double>(image_height-1),
                0.25
            };
            PPM_Exporter::write_color1_basic(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}

void Tests::test2_basic_hit() {
    
    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    Point3 origin{0.0, 0.0, 0.0};
    Vec3 horizontal{viewport_width, 0.0, 0.0};
    Vec3 vertical{0.0, viewport_height, 0.0};
    Point3 lower_left_corner =
        origin - horizontal/2.0 - vertical/2.0 - Vec3{0.0, 0.0, focal_length};

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            double u = static_cast<double>(i) / static_cast<double>(image_width-1);
            double v = static_cast<double>(j) / static_cast<double>(image_height-1);
            Ray r{origin, lower_left_corner + u*horizontal + v*vertical - origin};
            //Color pixel_color = ray1_background(r);
            Color pixel_color = ray2_sphere(r);

            PPM_Exporter::write_color1_basic(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

Color Tests::ray1_background(Ray const &r) {

    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color{1.0, 1.0, 1.0} + t * Color{0.5, 0.7, 1.0};
}

Color Tests::ray2_sphere(Ray const& r) {

    HittableSphere sphere{Point3{0.0, 0.0, -1.0}, 0.5,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})};
    
    if (HitRecord rec; sphere.hit(r, 0.0, 100.0, rec)) {
        Vec3 normal = r.at(rec.t) - sphere.center;
        normal.normalize();
        return 0.5 * Color{normal.x() + 1.0, normal.y() + 1.0, normal.z() + 1.0};
    }
    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double interpolate = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - interpolate) * Color{1.0, 1.0, 1.0} + interpolate * Color{0.5, 0.7, 1.0};
}

void Tests::test3_world() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);

    // World
    HittableList world;
    world.add(make_shared<HittableSphere>(Point3{0.0, 0.0, -1.0}, 0.5,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));
    world.add(make_shared<HittableSphere>(Point3{0.0, -100.5, -1.0}, 100.0,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    Point3 origin{0.0, 0.0, 0.0};
    Vec3 horizontal{viewport_width, 0.0, 0.0};
    Vec3 vertical{0.0, viewport_height, 0.0};
    Point3 lower_left_corner =
        origin - horizontal/2.0 - vertical/2.0 - Vec3{0.0, 0.0, focal_length};

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            double u = static_cast<double>(i) / static_cast<double>(image_width-1);
            double v = static_cast<double>(j) / static_cast<double>(image_height-1);
            Ray r{origin, lower_left_corner + u*horizontal + v*vertical - origin};
            Color pixel_color = ray3_world(r, world);

            PPM_Exporter::write_color1_basic(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
Color Tests::ray3_world(Ray const &r, Hittable const &world) {

    HitRecord rec;
    if (world.hit(r, 0.0, INF_DBL, rec)) {
        return 0.5 * (rec.normal + Color{1.0, 1.0, 1.0});
    }
    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double interpolate = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - interpolate) * Color{1.0, 1.0, 1.0} + interpolate * Color{0.5, 0.7, 1.0};
}


void Tests::test4_antialiasing() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 100;

    // World
    HittableList world;
    world.add(make_shared<HittableSphere>(Point3{0.0, 0.0, -1.0}, 0.5,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));
    world.add(make_shared<HittableSphere>(Point3{0.0, -100.5, -1.0}, 100.0,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray3_world(r, world);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}

void Tests::test5_diffuse() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 100;
    int const max_depth = 50;

    // World
    HittableList world;
    world.add(make_shared<HittableSphere>(Point3{0.0, 0.0, -1.0}, 0.5,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));
    world.add(make_shared<HittableSphere>(Point3{0.0, -100.5, -1.0}, 100.0,
        std::make_shared<MaterialLambertian>(Color{0.5, 0.5, 0.5})));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray4_diffuse(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
Color Tests::ray4_diffuse(Ray const &r, Hittable const& world, int depth) {

    if (depth <= 0)
        return Color{0.0, 0.0, 0.0};

    HitRecord rec;
    if (world.hit(r, 0.001, INF_DBL, rec)) {
        Point3 target = rec.point + rec.normal + util_rand::random_vector_on_unit_sphere();
        return 0.5 * ray4_diffuse(Ray{rec.point, target - rec.point}, world, depth - 1);
    }
    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double interpolate = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - interpolate) * Color{1.0, 1.0, 1.0} + interpolate * Color{0.5, 0.7, 1.0};
}

void Tests::test6_reflect() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 100;
    int const max_depth = 50;

    // World
    HittableList world;
    auto material_ground = make_shared<MaterialLambertian>(Color{0.8, 0.8, 0.0});
    auto material_center = make_shared<MaterialLambertian>(Color{0.7, 0.3, 0.3});
    auto material_left   = make_shared<MaterialMetal>(Color{0.8, 0.8, 0.8}, 0.3);
    auto material_right  = make_shared<MaterialMetal>(Color{0.8, 0.6, 0.2}, 1.0);

    world.add(make_shared<HittableSphere>(Point3{ 0.0, -100.5, -1.0}, 100.0, material_ground));
    world.add(make_shared<HittableSphere>(Point3{ 0.0,    0.0, -1.0},   0.5, material_center));
    world.add(make_shared<HittableSphere>(Point3{-1.0,    0.0, -1.0},   0.5, material_left));
    world.add(make_shared<HittableSphere>(Point3{ 1.0,    0.0, -1.0},   0.5, material_right));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray5_reflect(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
Color Tests::ray5_reflect(Ray const &r, Hittable const& world, int depth) {

    if (depth <= 0)
        return Color{0.0, 0.0, 0.0};

    HitRecord rec;
    if (world.hit(r, 0.001, INF_DBL, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray5_reflect(scattered, world, depth - 1);
        return Color{0.0, 0.0, 0.0};
    }
    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double interpolate = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - interpolate) * Color{1.0, 1.0, 1.0} + interpolate * Color{0.5, 0.7, 1.0};
}

void Tests::test7_refract() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 300;
    int const max_depth = 50;

    // World
    HittableList world;
    auto material_ground = make_shared<MaterialLambertian>(Color{0.8, 0.8, 0.0});
    auto material_center = make_shared<MaterialLambertian>(Color{0.1, 0.2, 0.5});
    auto material_left   = make_shared<MaterialDielectric>(1.5);
    auto material_right  = make_shared<MaterialMetal>(Color{0.8, 0.6, 0.2}, 0.0);

    world.add(make_shared<HittableSphere>(Point3{ 0.0, -100.5, -1.0}, 100.0, material_ground));
    world.add(make_shared<HittableSphere>(Point3{ 0.0,    0.0, -1.0},   0.5, material_center));
    world.add(make_shared<HittableSphere>(Point3{-1.0,    0.0, -1.0},   0.5, material_left));
    world.add(make_shared<HittableSphere>(Point3{-1.0,    0.0, -1.0},  -0.4, material_left));
    world.add(make_shared<HittableSphere>(Point3{ 1.0,    0.0, -1.0},   0.5, material_right));

    // Camera
    Camera cam(90.0, aspect_ratio);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray5_reflect(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
Color Tests::ray6_refract(Ray const &r, Hittable const& world, int depth) {

    if (depth <= 0)
        return Color{0.0, 0.0, 0.0};

    HitRecord rec;
    if (world.hit(r, 0.001, INF_DBL, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray5_reflect(scattered, world, depth - 1);
        return Color{0.0, 0.0, 0.0};
    }
    Vec3 unit_direction = r.direction;
    unit_direction.normalize();
    double interpolate = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - interpolate) * Color{1.0, 1.0, 1.0} + interpolate * Color{0.5, 0.7, 1.0};
}

void Tests::test8_cam_move() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 300;
    int const max_depth = 50;

    // World
    HittableList world;
    auto material_ground = make_shared<MaterialLambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<MaterialLambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<MaterialDielectric>(1.5);
    auto material_right  = make_shared<MaterialMetal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<HittableSphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<HittableSphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<HittableSphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<HittableSphere>(Point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<HittableSphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    Camera cam(Point3(-2,2,1), Point3(0,0,-1), Vec3(0,1,0), 20, aspect_ratio);


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray5_reflect(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}

void Tests::test9_cam_defocus() {

    // Image
    double const aspect_ratio = 16.0 / 9.0;
    int const image_width = 400;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 300;
    int const max_depth = 50;

    // World
    HittableList world;
    auto material_ground = make_shared<MaterialLambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<MaterialLambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<MaterialDielectric>(1.5);
    auto material_right  = make_shared<MaterialMetal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<HittableSphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<HittableSphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<HittableSphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<HittableSphere>(Point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<HittableSphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    Point3 look_from{3.0, 3.0, 2.0};
    Point3 look_at{0.0, 0.0, -1.0};
    Vec3 v_up{0.0, 1.0, 0.0};
    double dist_to_focus = (look_from - look_at).length();
    double aperture = 2.0;

    Camera cam{look_from, look_at, v_up, 20.0, aspect_ratio, aperture, dist_to_focus};


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray_defocus(u, v);
                pixel_color += ray5_reflect(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
void Tests::test10_final() {

    // Image
    double const aspect_ratio = 3.0 / 2.0;
    int const image_width = 1200;
    int const image_height = static_cast<int>(
        static_cast<double>(image_width) / aspect_ratio);
    int const samples_per_pixel = 50;
    int const max_depth = 50;

    // World
    HittableList world;

    auto ground_material = make_shared<MaterialLambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<HittableSphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = util_rand::random_double();
            Point3 center(a + 0.9*util_rand::random_double(), 0.2, b + 0.9*util_rand::random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = util_rand::random_unit_vector() * util_rand::random_unit_vector();
                    sphere_material = make_shared<MaterialLambertian>(albedo);
                    world.add(make_shared<HittableSphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = util_rand::random_unit_vector(0.5, 1.0);  //Color::random(0.5, 1);
                    auto fuzz = util_rand::random_double(0, 0.5);
                    sphere_material = make_shared<MaterialMetal>(albedo, fuzz);
                    world.add(make_shared<HittableSphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<MaterialDielectric>(1.5);
                    world.add(make_shared<HittableSphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<MaterialDielectric>(1.5);
    world.add(make_shared<HittableSphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<MaterialLambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<HittableSphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<MaterialMetal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<HittableSphere>(Point3(4, 1, 0), 1.0, material3));

    Point3 look_from{13.0, 2.0, 3.0};
    Point3 look_at{0.0, 0.0, 0.0};
    Vec3 v_up{0.0, 1.0, 0.0};
    double dist_to_focus = 10.0;
    double aperture = 0.1;

    Camera cam{look_from, look_at, v_up, 20.0, aspect_ratio, aperture, dist_to_focus};


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (static_cast<double>(i) + util_rand::random_double())
                    / static_cast<double>(image_width-1);
                double v = (static_cast<double>(j) + util_rand::random_double())
                    / static_cast<double>(image_height-1);
                Ray r = cam.get_ray_defocus(u, v);
                pixel_color += ray5_reflect(r, world, max_depth);
            }
            PPM_Exporter::write_color2_antialiasing(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}