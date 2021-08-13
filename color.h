#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
using namespace std;

void write_color(ostream &out, color pixel_color, int samples_per_pixel) // color is another name for vec3 class
{   
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // dividing the color based on the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // instead of using the original loop, we are reducing the code
    out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif