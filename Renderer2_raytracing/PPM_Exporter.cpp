#include "PPM_Exporter.h"

void PPM_Exporter::write_color1_basic(std::ostream& out, Color pixel_clr) {
    out << static_cast<int>(CLR_MAX * pixel_clr.x()) << ' '
        << static_cast<int>(CLR_MAX * pixel_clr.y()) << ' '
        << static_cast<int>(CLR_MAX * pixel_clr.z()) << '\n';
}
void PPM_Exporter::write_color2_antialiasing(std::ostream& out, Color color, int samples_per_pixel) {
    
    double scale = 1.0 / static_cast<double>(samples_per_pixel);
    color *= scale;
    color.make_sqrt();  // gamm correction

    out << static_cast<int>(256.0 * std::clamp(color.x(), 0.0, 0.999)) << ' '
        << static_cast<int>(256.0 * std::clamp(color.y(), 0.0, 0.999)) << ' '
        << static_cast<int>(256.0 * std::clamp(color.z(), 0.0, 0.999)) << '\n';
}