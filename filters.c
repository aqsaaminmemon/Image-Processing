#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "filters.h"

Pixel brighten(Pixel p, int value)
{
    Pixel result;
    result.r = clamp(p.r + value);
    result.g = clamp(p.g + value);
    result.b = clamp(p.b + value);
    return result;
}

Pixel invert(Pixel p)
{
    Pixel result;
    result.r = 255 - p.r;
    result.g = 255 - p.g;
    result.b = 255 - p.b;
    return result;
}

Pixel greyscale(Pixel p)
{
    uint8_t gray = clamp((int)(0.299 * p.r + 0.587 * p.g + 0.114 * p.b));
    p.r = p.g = p.b = gray;
    return p;
}

Pixel contrast(Pixel p, double factor)
{
    Pixel result;
    result.r = clamp((int)((p.r - 128) * factor + 128));
    result.g = clamp((int)((p.g - 128) * factor + 128));
    result.b = clamp((int)((p.b - 128) * factor + 128));
    return result;
}

// warm sepia tone
Pixel sepia(Pixel p)
{
    Pixel result;
    int r = (int)(0.450 * p.r + 0.820 * p.g + 0.200 * p.b);
    int g = (int)(0.380 * p.r + 0.720 * p.g + 0.170 * p.b);
    int b = (int)(0.250 * p.r + 0.480 * p.g + 0.120 * p.b);

    // extra warmth boost
    r += 15;
    b -= 10;

    result.r = clamp(r);
    result.g = clamp(g);
    result.b = clamp(b);
    return result;
}

// rotate 90 degrees clockwise (width and height swap)
Pixel* rotate90(Pixel* pixels, int width, int height)
{
    Pixel* output = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (!output) {
        perror("Error allocating memory");
        return NULL;
    }
    int new_width = height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int new_x = height - 1 - y;
            int new_y = x;
            output[new_y * new_width + new_x] = pixels[y * width + x];
        }
    }
    return output;
}

// rotate 180 degrees (width and height stay the same)
Pixel* rotate180(Pixel* pixels, int width, int height)
{
    Pixel* output = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (!output) {
        perror("Error allocating memory");
        return NULL;
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            output[(height - 1 - y) * width + (width - 1 - x)] = pixels[y * width + x];
        }
    }
    return output;
}

// rotate 270 degrees clockwise / 90 counter-clockwise (width and height swap)
Pixel* rotate270(Pixel* pixels, int width, int height)
{
    Pixel* output = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (!output) {
        perror("Error allocating memory");
        return NULL;
    }
    int new_width = height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int new_x = y;
            int new_y = width - 1 - x;
            output[new_y * new_width + new_x] = pixels[y * width + x];
        }
    }
    return output;
}

// blurs the image by averaging each pixel with its neighbors
Pixel* box_blur(Pixel* pixels, int width, int height)
{
    Pixel* output = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (!output) {
        perror("Error allocating memory");
        return NULL;
    }

    int radius = 2; // bigger radius = more blur

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumR = 0, sumG = 0, sumB = 0, count = 0;
            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        Pixel p = pixels[ny * width + nx];
                        sumR += p.r; sumG += p.g; sumB += p.b;
                        count++;
                    }
                }
            }
            output[y * width + x].r = sumR / count;
            output[y * width + x].g = sumG / count;
            output[y * width + x].b = sumB / count;
        }
    }
    return output;
}