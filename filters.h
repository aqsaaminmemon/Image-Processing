#ifndef FILTERS_H
#define FILTERS_H

#include "bmp.h"

// simple per-pixel filters
Pixel brighten(Pixel p, int value);
Pixel invert(Pixel p);
Pixel greyscale(Pixel p);
Pixel sepia(Pixel p);
Pixel contrast(Pixel p, double factor);

// whole-image filters (these need every pixel, not just one)
Pixel* rotate90(Pixel* pixels, int width, int height);
Pixel* rotate180(Pixel* pixels, int width, int height);
Pixel* rotate270(Pixel* pixels, int width, int height);
Pixel* box_blur(Pixel* pixels, int width, int height);

#endif