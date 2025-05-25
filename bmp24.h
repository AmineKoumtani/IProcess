#ifndef BMP24_H
#define BMP24_H

#include <stdint.h>

// Structure représentant un pixel en 24 bits (RGB)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_pixel;

// Structure représentant une image BMP 24 bits
typedef struct {
    int width;
    int height;
    int colorDepth;
    t_pixel *pixels;  // Tableau de pixels (1D)
} t_bmp24;

// Fonctions de gestion de l'image
t_bmp24 *bmp24_loadImage(const char *filename);                  // Chargement
void bmp24_saveImage(const char *filename, t_bmp24 *img);        // Sauvegarde
void bmp24_free(t_bmp24 *img);                                   // Libération mémoire
void bmp24_printInfo(t_bmp24 *img);                              // Affichage des infos

// Transformations simples
void bmp24_negative(t_bmp24 *img);                               // Inversion des couleurs
void bmp24_grayscale(t_bmp24 *img);                              // Niveaux de gris
void bmp24_brightness(t_bmp24 *img, int value);                  // Modification luminosité

// Traitement par convolution
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize);
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize);

// Améliorations d'image
void bmp24_equalize(t_bmp24 *img);                               // Égalisation histogramme
void bmp24_boxBlur(t_bmp24 *img);                                // Flou simple
void bmp24_gaussianBlur(t_bmp24 *img);                           // Flou doux
void bmp24_outline(t_bmp24 *img);                                // Détection des contours
void bmp24_emboss(t_bmp24 *img);                                 // Relief
void bmp24_sharpen(t_bmp24 *img);                                // Netteté

#endif // BMP24_H