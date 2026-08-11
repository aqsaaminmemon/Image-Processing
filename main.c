#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "filters.h"

int main()
{
    char input_filename[100], output_filename[100];
    int choice;
    char ans = 'n';
    Pixel* pixels = NULL;

    printf("Enter input BMP filename: ");
    scanf("%s", input_filename);
    printf("Enter output BMP filename: ");
    scanf("%s", output_filename);

    do {
        printf("Choose filter:\n1. Brighten\n2. Invert\n3. Greyscale\n4. Sepia\n5. Contrast\n6. Rotate 90\n7. Rotate 180\n8. Rotate 270\n9. Box Blur\n");
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

        if (choice == 6 || choice == 8) {
            // rotate90 / rotate270 swap width and height
            Pixel* rotated = (choice == 6) ? rotate90(pixels, width, height)
                                            : rotate270(pixels, width, height);
            if (rotated == NULL) {
                free(pixels);
                return 1;
            }
            free(pixels);
            pixels = rotated;
            int temp = width;
            width = height;
            height = temp;
        } else if (choice == 7) {
            // rotate180 keeps dimensions the same
            Pixel* rotated = rotate180(pixels, width, height);
            if (rotated == NULL) {
                free(pixels);
                return 1;
            }
            free(pixels);
            pixels = rotated;
        } else if (choice == 9) {
            // box_blur keeps dimensions the same
            Pixel* blurred = box_blur(pixels, width, height);
            if (blurred == NULL) {
                free(pixels);
                return 1;
            }
            free(pixels);
            pixels = blurred;
        } else {
            apply_filter(pixels, width, height, choice);
        }

        if (write_bmp(output_filename, pixels, width, height)) {
            printf("Image Saved successfully\n");
        } else {
            printf("Failed to save image\n");
        }

        printf("Do you want to apply another filter? (y/n): ");
        scanf(" %c", &ans);
    } while (ans == 'y' || ans == 'Y');

    free(pixels);
    return 0;
}