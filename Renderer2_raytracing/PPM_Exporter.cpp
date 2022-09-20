#include "PPM_Exporter.h"

void PPM_Exporter::test1() {
    int const image_width = 256;
    int const image_height = 256;

    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{
                static_cast<double>(i) / static_cast<double>(image_width-1),
                static_cast<double>(j) / static_cast<double>(image_height-1),
                0.25
            };
            PPM_Exporter::write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}

void PPM_Exporter::write_color(std::ostream& out, Color pixel_clr) {
    out << static_cast<int>(CLR_MAX * pixel_clr.x()) << ' '
        << static_cast<int>(CLR_MAX * pixel_clr.y()) << ' '
        << static_cast<int>(CLR_MAX * pixel_clr.z()) << '\n';
}