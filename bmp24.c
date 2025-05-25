#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    unsigned char header[54];
    fread(header, 1, 54, file);
    if (header[0] != 'B' || header[1] != 'M') {
        fclose(file);
        return NULL;
    }

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bpp = *(short*)&header[28];
    if (bpp != 24) {
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = malloc(sizeof(t_bmp24));
    img->width = width;
    img->height = height;
    img->colorDepth = 24;
    img->pixels = malloc(width * height * sizeof(t_pixel));

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char *row = malloc(row_padded);

    for (int y = height - 1; y >= 0; y--) {
        fread(row, 1, row_padded, file);
        for (int x = 0; x < width; x++) {
            int i = y * width + x;
            img->pixels[i].blue = row[x * 3];
            img->pixels[i].green = row[x * 3 + 1];
            img->pixels[i].red = row[x * 3 + 2];
        }
    }

    free(row);
    fclose(file);
    return img;
}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int size) {
    int offset = size / 2;
    float r = 0, g = 0, b = 0;

    for (int j = -offset; j <= offset; j++) {
        for (int i = -offset; i <= offset; i++) {
            int px = x + i;
            int py = y + j;
            t_pixel p = img->pixels[py * img->width + px];
            float coeff = kernel[j + offset][i + offset];
            r += p.red * coeff;
            g += p.green * coeff;
            b += p.blue * coeff;
        }
    }

    t_pixel res;
    res.red = fminf(fmaxf(r, 0), 255);
    res.green = fminf(fmaxf(g, 0), 255);
    res.blue = fminf(fmaxf(b, 0), 255);
    return res;
}

void bmp24_applyFilter(t_bmp24 *img, float **kernel, int size) {
    int n = size / 2;
    t_pixel *temp = malloc(img->width * img->height * sizeof(t_pixel));
    if (!temp) return;

    for (int y = n; y < img->height - n; y++)
        for (int x = n; x < img->width - n; x++)
            temp[y * img->width + x] = bmp24_convolution(img, x, y, kernel, size);

    for (int y = n; y < img->height - n; y++)
        for (int x = n; x < img->width - n; x++)
            img->pixels[y * img->width + x] = temp[y * img->width + x];

    free(temp);
}

void bmp24_saveImage(const char *filename, t_bmp24 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    int row_padded = (img->width * 3 + 3) & (~3);
    int data_size = row_padded * img->height;

    unsigned char header[54] = {
        0x42, 0x4D,
        0,0,0,0,
        0,0,0,0,
        54,0,0,0,
        40,0,0,0
    };

    *(int *)&header[2] = 54 + data_size;
    *(int *)&header[18] = img->width;
    *(int *)&header[22] = img->height;
    *(short *)&header[26] = 1;
    *(short *)&header[28] = 24;

    fwrite(header, 1, 54, file);

    unsigned char *row = malloc(row_padded);
    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            t_pixel p = img->pixels[y * img->width + x];
            row[x * 3] = p.blue;
            row[x * 3 + 1] = p.green;
            row[x * 3 + 2] = p.red;
        }
        fwrite(row, 1, row_padded, file);
    }

    free(row);
    fclose(file);
}

void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    free(img->pixels);
    free(img);
}

void bmp24_printInfo(t_bmp24 *img) {
    if (!img) return;
    printf("Largeur : %d\nHauteur : %d\nCouleur : %d bits\n", img->width, img->height, img->colorDepth);
}

void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->pixels[y * img->width + x];
            p->red = 255 - p->red;
            p->green = 255 - p->green;
            p->blue = 255 - p->blue;
        }
}

void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->pixels[y * img->width + x];
            unsigned char g = (p->red + p->green + p->blue) / 3;
            p->red = p->green = p->blue = g;
        }
}

void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->pixels[y * img->width + x];
            int r = p->red + value;
            int g = p->green + value;
            int b = p->blue + value;
            p->red = r > 255 ? 255 : (r < 0 ? 0 : r);
            p->green = g > 255 ? 255 : (g < 0 ? 0 : g);
            p->blue = b > 255 ? 255 : (b < 0 ? 0 : b);
        }
}

void bmp24_equalize(t_bmp24 *img) {
    if (!img || !img->pixels) return;
    int w = img->width, h = img->height, size = w * h;
    unsigned int hist[256] = {0}, cdf[256] = {0};

    for (int i = 0; i < size; i++) {
        t_pixel *px = &img->pixels[i];
        uint8_t Y = 0.299 * px->red + 0.587 * px->green + 0.114 * px->blue;
        hist[Y]++;
    }

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) cdf[i] = cdf[i - 1] + hist[i];

    float cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    float scale = 255.0f / (size - cdf_min);
    uint8_t map[256];
    for (int i = 0; i < 256; i++)
        map[i] = (uint8_t)(round((cdf[i] - cdf_min) * scale));

    for (int i = 0; i < size; i++) {
        t_pixel *px = &img->pixels[i];
        float Y = 0.299 * px->red + 0.587 * px->green + 0.114 * px->blue;
        float U = -0.168736 * px->red - 0.331264 * px->green + 0.5 * px->blue;
        float V = 0.5 * px->red - 0.418688 * px->green - 0.081312 * px->blue;
        uint8_t Y_eq = map[(uint8_t)Y];

        int R = Y_eq + 1.402 * V;
        int G = Y_eq - 0.344136 * U - 0.714136 * V;
        int B = Y_eq + 1.772 * U;

        px->red = R < 0 ? 0 : (R > 255 ? 255 : R);
        px->green = G < 0 ? 0 : (G > 255 ? 255 : G);
        px->blue = B < 0 ? 0 : (B > 255 ? 255 : B);
    }
}

void bmp24_boxBlur(t_bmp24 *img) {
    float kernel[3][3] = {
        {1.0f/9, 1.0f/9, 1.0f/9},
        {1.0f/9, 1.0f/9, 1.0f/9},
        {1.0f/9, 1.0f/9, 1.0f/9}
    };
    float *kptr[3] = { kernel[0], kernel[1], kernel[2] };
    bmp24_applyFilter(img, (float **)kptr, 3);
}

void bmp24_gaussianBlur(t_bmp24 *img) {
    float kernel[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };
    float *kptr[3] = { kernel[0], kernel[1], kernel[2] };
    bmp24_applyFilter(img, (float **)kptr, 3);
}

void bmp24_outline(t_bmp24 *img) {
    float kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    float *kptr[3] = { kernel[0], kernel[1], kernel[2] };
    bmp24_applyFilter(img, (float **)kptr, 3);
}

void bmp24_emboss(t_bmp24 *img) {
    float kernel[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    float *kptr[3] = { kernel[0], kernel[1], kernel[2] };
    bmp24_applyFilter(img, (float **)kptr, 3);
}

void bmp24_sharpen(t_bmp24 *img)
{
    float kernel[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    float *kptr[3] = { kernel[0], kernel[1], kernel[2] };
    bmp24_applyFilter(img, (float **)kptr, 3);
}