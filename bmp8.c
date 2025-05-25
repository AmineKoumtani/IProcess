#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        fclose(file);
        return NULL;
    }

    fread(img->header, 1, 54, file);
    fread(img->colorTable, 1, 1024, file);

    img->width = *(unsigned int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned int *)(img->header + 28);
    img->dataSize = img->width * img->height;

    if (img->colorDepth != 8) {
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = malloc(img->dataSize);
    if (!img->data) {
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->data, 1, img->dataSize, file);
    fclose(file);
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    fwrite(img->header, 1, 54, file);
    fwrite(img->colorTable, 1, 1024, file);
    fwrite(img->data, 1, img->dataSize, file);

    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    if (!img) return;
    free(img->data);
    free(img);
}

void bmp8_printInfo(t_bmp8 *img) {
    if (!img) return;
    printf("Largeur : %u\n", img->width);
    printf("Hauteur : %u\n", img->height);
    printf("Profondeur : %u bits\n", img->colorDepth);
    printf("Taille des données : %u octets\n", img->dataSize);
}

void bmp8_negative(t_bmp8 *img) {
    if (!img || !img->data) return;
    for (unsigned int i = 0; i < img->dataSize; i++)
        img->data[i] = 255 - img->data[i];
}

void bmp8_brightness(t_bmp8 *img, int value) {
    if (!img || !img->data) return;
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int val = img->data[i] + value;
        if (val > 255) val = 255;
        if (val < 0) val = 0;
        img->data[i] = (unsigned char)val;
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    if (!img || !img->data) return;
    for (unsigned int i = 0; i < img->dataSize; i++)
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (!img || !img->data) return;

    int n = kernelSize / 2;
    unsigned char *temp = malloc(img->dataSize);
    if (!temp) return;

    memcpy(temp, img->data, img->dataSize);

    for (int y = n; y < img->height - n; y++) {
        for (int x = n; x < img->width - n; x++) {
            float sum = 0.0;

            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    int val = img->data[(y + i) * img->width + (x + j)];
                    float coeff = kernel[i + n][j + n];
                    sum += val * coeff;
                }
            }

            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            temp[y * img->width + x] = (unsigned char)sum;
        }
    }

    memcpy(img->data, temp, img->dataSize);
    free(temp);
}