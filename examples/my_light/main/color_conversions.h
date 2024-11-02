#pragma once

#include <math.h>

static void xyToRGB(float x, float y, uint8_t *pr, uint8_t *pg, uint8_t *pb)
{
    float X = x * (80. / y);
    float Y = 80.;
    float Z = (1 - x - y) * (80. / y);

    float r = X * 3.2404542 + Y * -1.5371385 + Z * -0.4985314;
    float g = X * -0.9692660 + Y * 1.8760108 + Z * 0.0415560;
    float b = X * 0.0556434 + Y * -0.2040259 + Z * 1.0572252;

    float min = std::min(std::min(r, g), b);
    if (min < 0)
    {
        r -= min;
        g -= min;
        b -= min;
    }

    r = ((r > 0.0031308) ? (1.055 * pow(r, 1 / 2.4) - 0.055) : (12.92 * r));
    g = ((g > 0.0031308) ? (1.055 * pow(g, 1 / 2.4) - 0.055) : (12.92 * g));
    b = ((b > 0.0031308) ? (1.055 * pow(b, 1 / 2.4) - 0.055) : (12.92 * b));

    float max = std::max(std::max(r, g), b);

    r = (r / max) * 255.;
    g = (g / max) * 255.;
    b = (b / max) * 255.;

    *pr = r;
    *pg = g;
    *pb = b;
}
