#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "filters.h"

// clamp helper function
uint8_t clamp(int value)
{
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)value;
}

// applies the simple per-pixel filters (rotate and blur are handled
// separately in main.c since they need the whole image at once)
void apply_filter(Pixel* pixels, int width, int height, int choice)
{
    for (int i = 0; i < width * height; i++) {
        if (choice == 1) {
            pixels[i] = brighten(pixels[i], 50);
        } else if (choice == 2) {
            pixels[i] = invert(pixels[i]);
        } else if (choice == 3) {
            pixels[i] = greyscale(pixels[i]);
        } else if (choice == 4) {
            pixels[i] = sepia(pixels[i]);
        } else if (choice == 5) {
            pixels[i] = contrast(pixels[i], 1.5);
        }
    }
}

// Read BMP
Pixel* read_bmp(const char* filename, int* width, int* height)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    *width = infoHeader.biwidth;
    *height = infoHeader.biheight;

    Pixel* pixels = (Pixel*)malloc(sizeof(Pixel) * (*width) * (*height));
    if (!pixels) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Move to where actual pixel data starts
    fseek(file, fileHeader.bfoffbits, SEEK_SET);

    // BMP rows are padded to a multiple of 4 bytes
    int row_padded = (*width * 3 + 3) & (~3);
    uint8_t* row_data = (uint8_t*)malloc(row_padded);
    if (!row_data) {
        perror("Error allocating memory");
        free(pixels);
        fclose(file);
        return NULL;
    }

    // BMP stores rows bottom-to-top, so we flip while reading
    for (int i = 0; i < *height; i++) {
        fread(row_data, sizeof(uint8_t), row_padded, file);
        for (int j = 0; j < *width; j++) {
            int idx = j * 3;
            pixels[(*height - 1 - i) * (*width) + j].b = row_data[idx];
            pixels[(*height - 1 - i) * (*width) + j].g = row_data[idx + 1];
            pixels[(*height - 1 - i) * (*width) + j].r = row_data[idx + 2];
        }
    }

    free(row_data);
    fclose(file);
    return pixels;
}

// Write BMP
int write_bmp(const char* filename, Pixel* pixels, int width, int height)
{
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    int row_padded = (width * 3 + 3) & (~3);
    uint8_t* row_data = (uint8_t*)malloc(row_padded);

    BITMAPFILEHEADER fileHeader = {0};
    BITMAPINFOHEADER infoHeader = {0};

    fileHeader.bftype = 0x4D42;
    fileHeader.bfoffbits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfsize = fileHeader.bfoffbits + row_padded * height;

    infoHeader.bisize = sizeof(BITMAPINFOHEADER);
    infoHeader.biwidth = width;
    infoHeader.biheight = height;
    infoHeader.biplanes = 1;
    infoHeader.bibitcount = 24;
    infoHeader.bicompression = 0;
    infoHeader.bisizeimage = row_padded * height;

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel p = pixels[(height - 1 - i) * width + j];
            row_data[j * 3] = p.b;
            row_data[j * 3 + 1] = p.g;
            row_data[j * 3 + 2] = p.r;
        }
        fwrite(row_data, 1, row_padded, file);
    }

    free(row_data);
    fclose(file);
    return 1;
}