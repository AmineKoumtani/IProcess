#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Problème d'ouverture du fichier %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Problème d'allocation mémoire du fichier %s\n", filename);
        fclose(file);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), 54, file);
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->width = *(unsigned int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned int *)(img->header + 28);
    img->dataSize = img->width * img->height;

    if (img->colorDepth != 8) {
        fprintf(stderr, "Problème : profondeur de couleur différente de 8 bits\n");
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = malloc(img->dataSize);
    if (!img->data) {
        perror("Erreur d'allocation mémoire pour les données");
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Problème de création du fichier %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    printf("L'image a été enregistrée dans %s\n", filename);
}

void bmp8_free(t_bmp8 *img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

void bmp8_printInfo(t_bmp8 *img) {
    if (!img) {
        printf("Erreur : image non chargée.\n");
        return;
    }

    printf("Image Info:\n");
    printf("  Width: %u\n", img->width);
    printf("  Height: %u\n", img->height);
    printf("  Color Depth: %u\n", img->colorDepth);
    printf("  Data Size: %u\n", img->dataSize);
}

void bmp8_negative(t_bmp8 *img) {
    if (!img || !img->data) {
        printf("Problème d'image invalide pour l'inversion.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 *img, int value) {
    if (!img || !img->data) {
        printf("Problème d'image invalide pour ajuster la luminosité.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        int nouvelleVal = img->data[i] + value;
        if (nouvelleVal > 255) {
            img->data[i] = 255;
        } else if (nouvelleVal < 0) {
            img->data[i] = 0;
        } else {
            img->data[i] = (unsigned char)nouvelleVal;
        }
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    if (!img || !img->data) {
        printf("Problème d'image invalide pour seuillage.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (!img || !img->data) {
        printf("Problème d'image invalide pour filtre.\n");
        return;
    }

    int n = kernelSize / 2;
    unsigned char *newData = malloc(img->dataSize);

    if (!newData) {
        printf("Problème d'allocation mémoire pour le filtrage.\n");
        return;
    }

    memcpy(newData, img->data, img->dataSize);

    for (int y = n; y < img->height - n; y++) {
        for (int x = n; x < img->width - n; x++) {
            float sum = 0.0;

            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    int pixel = img->data[(y + i) * img->width + (x + j)];
                    float coeff = kernel[i + n][j + n];
                    sum += pixel * coeff;
                }
            }

            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            newData[y * img->width + x] = (unsigned char)sum;
        }
    }

    memcpy(img->data, newData, img->dataSize);
    free(newData);
}