#include "bmp24.h"

t_pixel ** bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        printf("Erreur : allocation de lignes échouée.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Erreur : allocation de colonnes échouée à la ligne %d.\n", i);
            bmp24_freeDataPixels(pixels, i); // libérer ce qui a été alloué
            return NULL;
        }
    }

    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return;
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) {
        printf("Erreur : allocation de structure image échouée.\n");
        return NULL;
    }

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

void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    if (img->data) {
        bmp24_freeDataPixels(img->data, img->height);
    }
    free(img);
}
//
// Created by koumt on 20/05/2025.
//
