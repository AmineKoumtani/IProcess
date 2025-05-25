#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BMP8_H
#define BMP8_H

// Structure représentant une image BMP 8 bits
typedef struct {
    unsigned char header[54];         // En-tête BMP
    unsigned char colorTable[1024];   // Table de couleurs (palette)
    unsigned char *data;              // Données des pixels
    unsigned int width;               // Largeur de l'image
    unsigned int height;              // Hauteur de l'image
    unsigned int colorDepth;          // Profondeur de couleur (normalement 8)
    unsigned int dataSize;            // Taille totale des données d'image
} t_bmp8;

// Chargement d'une image BMP 8 bits
t_bmp8 *bmp8_loadImage(const char *filename);

// Sauvegarde d'une image BMP 8 bits
void bmp8_saveImage(const char *filename, t_bmp8 *img);

// Libération de la mémoire utilisée par une image
void bmp8_free(t_bmp8 *img);

// Affiche les informations de l'image
void bmp8_printInfo(t_bmp8 *img);

// Applique un filtre négatif
void bmp8_negative(t_bmp8 *img);

// Ajuste la luminosité
void bmp8_brightness(t_bmp8 *img, int value);

// Applique un seuil
void bmp8_threshold(t_bmp8 *img, int threshold);

// Applique une convolution
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);

#endif // BMP8_H