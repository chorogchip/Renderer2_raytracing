#pragma once

#include<iostream>

#include "Utils.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Camera.h"
#include "PPM_Exporter.h"

class Tests {
public:
    static void test1_uv();

    static void test2_basic_hit();
    static Color ray1_background(Ray const &);
    static Color ray2_sphere(Ray const &);

    static void test3_world();
    static Color ray3_world(Ray const &, Hittable const &world);
    
    static void test4_antialiasing();

    static void test5_diffuse();
    static Color ray4_diffuse(Ray const &, Hittable const &world, int depth);

    static void test6_reflect();
    static Color ray5_reflect(Ray const &, Hittable const &world, int depth);

    static void test7_refract();
    static Color ray6_refract(Ray const &, Hittable const &world, int depth);

    static void test8_cam_move();
    
    static void test9_cam_defocus();

    static void test10_final();

};

