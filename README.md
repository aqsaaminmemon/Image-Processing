# BMP Image Filter Tool

## Description

A simple command-line program written in C that lets you open a BMP image, apply a filter to it (like brighten, invert, sepia, blur, or rotate), and save the result as a new BMP image. Built as a learning project to practice file I/O, structs, pointers, and dynamic memory in C.

## Features

- Read and write 24-bit BMP image files
- 5 color/tone filters: Brighten, Invert, Greyscale, Sepia, Contrast
- 3 rotation filters: Rotate 90°, Rotate 180°, Rotate 270°
- Box Blur filter
- Simple menu-based terminal interface
- Apply multiple filters in one session without restarting the program

## Technologies Used

- **Language:** C
- **Compiler:** GCC
- **Concepts used:** structs, pointers, dynamic memory allocation (`malloc`/`free`), file I/O (`fopen`/`fread`/`fwrite`), header files, multi-file compilation

## Project Structure

```
.
├── main.c        # main function — filenames, menu, loop
├── bmp.h         # Pixel struct, BMP file headers, function declarations
├── bmp.c         # clamp, apply_filter, read_bmp, write_bmp
├── filters.h     # filter function declarations
├── filters.c     # all filter functions (brighten, invert, sepia, rotate, blur, etc.)
└── README.md     # this file
```

| File | What it has |
|---|---|
| `main.c` | The `main` function — asks for filenames, shows the filter menu, runs the loop |
| `bmp.h` / `bmp.c` | Pixel struct, BMP file headers, `clamp`, `apply_filter`, `read_bmp`, `write_bmp` |
| `filters.h` / `filters.c` | All the filter functions (brighten, invert, sepia, rotate, blur, etc.) |

## How to Compile

Open a terminal in this folder and run:

```bash
gcc -o image_editor main.c bmp.c filters.c
```

This creates a program called `image_editor` (or `image_editor.exe` on Windows).

## How to Run

```bash
./image_editor
```

On Windows terminal, use:

```bash
image_editor.exe
```

## How to Use

1. Enter the input BMP filename (the image you want to edit).
2. Enter the output BMP filename (what the new image will be saved as).
3. Pick a filter number from the menu.
4. The filtered image is saved to your output filename.
5. Choose `y` to apply another filter, or `n` to quit.

**Note:** The input file must be a `.bmp` file (not `.png` or `.jpg`). Every filter run reads fresh from the input file — filters don't stack on top of each other.

## Filter Menu

| Number | Filter | What it does |
|---|---|---|
| 1 | Brighten | Makes the image lighter |
| 2 | Invert | Flips all colors (like a photo negative) |
| 3 | Greyscale | Removes color, makes it black and white |
| 4 | Sepia | Adds a warm, vintage brown tone |
| 5 | Contrast | Makes light areas lighter and dark areas darker |
| 6 | Rotate 90 | Rotates the image 90° clockwise |
| 7 | Rotate 180 | Rotates the image 180° |
| 8 | Rotate 270 | Rotates the image 270° clockwise (90° counter-clockwise) |
| 9 | Box Blur | Blurs the image by averaging nearby pixels |

## Requirements

- A C compiler (like `gcc`)
- A `.bmp` image file to test with (24-bit, uncompressed BMP works best)
