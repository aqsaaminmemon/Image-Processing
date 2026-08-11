#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

typedef struct{
    uint8_t r, g, b;
}Pixel;

#pragma pack(push, 1)

//BMP header structures

 typedef struct{
    uint16_t bftype;
    uint32_t bfsize;
    uint16_t bfreserved1;
    uint16_t bfreserved2;
    uint32_t bfoffbits;
}BITMAPFILEHEADER;

typedef struct{
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
}BITMAPINFOHEADER;
#pragma pack(pop)

//clamp helper function
uint8_t clamp (int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;
    return (uint8_t)value;
}

//filters
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
    p.r =p.g=p.b=gray;
    return p;
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

    fseek(file, fileHeader.bfoffbits, SEEK_SET);

    int row_padded = (*width * 3 + 3) & (~3);
    uint8_t* row_data = (uint8_t*)malloc(row_padded);
    if (!row_data) {
        perror("Error allocating memory");
        free(pixels);
        fclose(file);
        return NULL;
    }

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
//write BMP
 int write_bmp(const char* filename, Pixel* pixels, int width, int height)
 {
    FILE *file = fopen(filename, "wb");
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

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
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

void apply_filter(Pixel* pixels, int width, int height, Pixel (*filter)(Pixel), int choice)
{
    for(int i = 0; i < width * height; i++)
    {
        if(choice == 1) {
            pixels[i] = brighten(pixels[i], 50);
        } else if(choice == 2) {
            pixels[i] = invert(pixels[i]);
        } else if(choice == 3) {
            pixels[i] = greyscale(pixels[i]);
        }
    }
}
int main()
{
    char input_filename[100], output_filename[100];
    int choice;
    char ans = 'n';
    Pixel *pixels = NULL;

    do {
        printf("Enter input BMP filename: ");
        scanf("%s", input_filename);
        printf("Enter output BMP filename: ");
        scanf("%s", output_filename);

        printf("Choose filter:\n1. Brighten\n2. Invert\n3. Greyscale\n");
        scanf("%d", &choice);

        int width, height;
        if (pixels != NULL) {
            free(pixels);
            pixels = NULL;
        }

        pixels = read_bmp(input_filename, &width, &height);
        if (pixels == NULL) {
            return 1;
        }

        apply_filter(pixels, width, height, NULL, choice);
        if (write_bmp(output_filename, pixels, width, height)) {
            printf("Image Saved successfully\n");
        } else {
            printf("Failed to save image\n");
        }

        printf("Do you want to process another image? (y/n): ");
        scanf(" %c", &ans);
    } while (ans == 'y' || ans == 'Y');

    free(pixels);
    return 0;
}
