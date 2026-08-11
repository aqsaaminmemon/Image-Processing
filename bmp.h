#ifndef BMP_H
#define BMP_H

#include <stdint.h>

typedef struct {
    uint8_t r, g, b;
} Pixel;

#pragma pack(push, 1)

// BMP header structures
typedef struct {
    uint16_t bftype;
    uint32_t bfsize;
    uint16_t bfreserved1;
    uint16_t bfreserved2;
    uint32_t bfoffbits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t bisize;
    int32_t biwidth;
    int32_t biheight;
    uint16_t biplanes;
    uint16_t bibitcount;
    uint32_t bicompression;
    uint32_t bisizeimage;
    int32_t bixppm;
    int32_t biyppm;
    uint32_t biclrused;
    uint32_t biclrimportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

// clamp helper: keeps a value between 0 and 255
uint8_t clamp(int value);

// applies one of the simple per-pixel filters (choice 1-5)
void apply_filter(Pixel* pixels, int width, int height, int choice);

// reads a BMP file into a Pixel array
Pixel* read_bmp(const char* filename, int* width, int* height);

// writes a Pixel array out as a BMP file
int write_bmp(const char* filename, Pixel* pixels, int width, int height);

#endif