#pragma once
#include <iostream>

#include "Vec3.h"

class PPM_Exporter {
private:
    static double constexpr CLR_MAX = 255.999;
public:
    static void test1();
    static void write_color(std::ostream &out, Color color);
};
