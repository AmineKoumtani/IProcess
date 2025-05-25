#ifndef BMP24_H
#define BMP24_H

#include <stdint.h>  // pour uint8_t, uint16_t, uint32_t
#include <stdio.h>
#include <stdlib.h>

// STRUCTURES DE BASE

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

// ALLOC / LIBÉRATION

t_pixel ** bmp24_allocateDataPixels(int width, int height);
void bmp24_freeDataPixels(t_pixel **pixels, int height);

t_bmp24 * bmp24_allocate(int width, int height, int colorDepth);
void bmp24_free(t_bmp24 *img);

#endif // BMP24_H
//
// Created by koumt on 20/05/2025.
//

#ifndef BMP24_H
#define BMP24_H

#endif //BMP24_H
