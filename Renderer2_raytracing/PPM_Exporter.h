#pragma once
#include <iostream>
#include <algorithm>

#include "Vec3.h"

class PPM_Exporter {
private:
    static double constexpr CLR_MAX = 255.999;
public:
    static void write_color1_basic(std::ostream &out, Color color);
    static void write_color2_antialiasing(std::ostream &out, Color color,
                                          int samples_per_pixel);
};
