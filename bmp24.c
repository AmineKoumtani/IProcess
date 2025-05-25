#include "bmp24.h"

// allocation mémoire pour les pixels
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) return NULL;

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            bmp24_freeDataPixels(pixels, i);
            return NULL;
        }
    }
    return pixels;
}

// libère la mémoire des pixels
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return;
    for (int i = 0; i < height; i++) free(pixels[i]);
    free(pixels);
}

// alloue la structure de l'image
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) return NULL;

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height);

    if (!img->data) {
        free(img);
        return NULL;
    }
    return img;
}

// libère toute l'image
void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    if (img->data) bmp24_freeDataPixels(img->data, img->height);
    free(img);
}

// charge une image BMP 24 bits
t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    unsigned char header[54];
    fread(header, 1, 54, file);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    if (*(short*)&header[28] != 24) {
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = bmp24_allocate(width, height, 24);
    if (!img) {
        fclose(file);
        return NULL;
    }

    fseek(file, *(int*)&header[10], SEEK_SET);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fread(&img->data[height - 1 - y][x], sizeof(t_pixel), 1, file);
        }
    }

    fclose(file);
    return img;
}

// convertit en niveau de gris
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            unsigned char gray = (p->red + p->green + p->blue) / 3;
            p->red = p->green = p->blue = gray;
        }
    }
}

// ajuste la luminosité
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];

            p->red = (p->red + value > 255) ? 255 : (p->red + value < 0) ? 0 : p->red + value;
            p->green = (p->green + value > 255) ? 255 : (p->green + value < 0) ? 0 : p->green + value;
            p->blue = (p->blue + value > 255) ? 255 : (p->blue + value < 0) ? 0 : p->blue + value;
        }
    }
}

// applique le négatif
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            p->red = 255 - p->red;
            p->green = 255 - p->green;
            p->blue = 255 - p->blue;
        }
    }
}

// sauvegarde l'image BMP 24 bits
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    unsigned char header[54] = {0};
    header[0] = 'B'; header[1] = 'M';
    *(int*)&header[2] = 54 + img->width * img->height * 3;
    *(int*)&header[10] = 54;
    *(int*)&header[14] = 40;
    *(int*)&header[18] = img->width;
    *(int*)&header[22] = img->height;
    *(short*)&header[26] = 1;
    *(short*)&header[28] = 24;

    fwrite(header, 1, 54, file);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            fwrite(&img->data[img->height - 1 - y][x], sizeof(t_pixel), 1, file);
        }
    }

    fclose(file);
}
