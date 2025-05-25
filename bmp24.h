#ifndef BMP24_H
#define BMP24_H

#include <stdint.h>

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_pixel;

typedef struct {
    int width;
    int height;
    int colorDepth;
    t_pixel *pixels;
} t_bmp24;

t_bmp24 *bmp24_loadImage(const char *filename);
void bmp24_saveImage(const char *filename, t_bmp24 *img);
void bmp24_free(t_bmp24 *img);
void bmp24_printInfo(t_bmp24 *img);
void bmp24_negative(t_bmp24 *img);
void bmp24_grayscale(t_bmp24 *img);
void bmp24_brightness(t_bmp24 *img, int value);
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize);
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize);
void bmp24_equalize(t_bmp24 *img);
void bmp24_boxBlur(t_bmp24 *img);
void bmp24_gaussianBlur(t_bmp24 *img);
void bmp24_outline(t_bmp24 *img);
void bmp24_emboss(t_bmp24 *img);
void bmp24_sharpen(t_bmp24 *img);

#endif // BMP24_H