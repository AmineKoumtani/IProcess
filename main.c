#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include "bmp24.h"
#include <direct.h>

int main() {
    int choix_type, choix_filtre, valeur;
    char nom_fichier[256];

    printf("=== EDITEUR D'IMAGES BMP ===\n");

    // Choix du type d'image
    printf("\nType d'image :\n");
    printf("1. Image 8 bits (noir et blanc)\n");
    printf("2. Image 24 bits (couleur)\n");
    printf("Votre choix : ");
    scanf("%d", &choix_type);
    getchar();

    // Demander le nom du fichier
    printf("Nom du fichier (ex: image.bmp) : ");
    fgets(nom_fichier, 256, stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    if (choix_type == 1) {
        // ===== IMAGES 8 BITS =====
        t_bmp8 *img = bmp8_loadImage(nom_fichier);
        if (!img) {
            printf("Erreur : fichier introuvable !\n");
            return 1;
        }

        printf("\nFiltres 8 bits :\n");
        printf("1. Negatif\n");
        printf("2. Luminosite\n");
        printf("3. Seuillage\n");
        printf("Votre choix : ");
        scanf("%d", &choix_filtre);

        if (choix_filtre == 1) {
            bmp8_negative(img);
            bmp8_saveImage("resultat_negatif.bmp", img);
            printf("Image negative sauvee !\n");
        }
        else if (choix_filtre == 2) {
            printf("Valeur luminosite (-100 a 100) : ");
            scanf("%d", &valeur);
            bmp8_brightness(img, valeur);
            bmp8_saveImage("resultat_luminosite.bmp", img);
            printf("Luminosite appliquee !\n");
        }
        else if (choix_filtre == 3) {
            printf("Seuil (0 a 255) : ");
            scanf("%d", &valeur);
            bmp8_threshold(img, valeur);
            bmp8_saveImage("resultat_seuillage.bmp", img);
            printf("Seuillage applique !\n");
        }

        bmp8_free(img);
    }
    else if (choix_type == 2) {
        // ===== IMAGES 24 BITS =====
        t_bmp24 *img = bmp24_loadImage(nom_fichier);
        if (!img) {
            printf("Erreur : fichier introuvable !\n");
            return 1;
        }

        printf("\nFiltres 24 bits :\n");
        printf("1. Negatif\n");
        printf("2. Noir et blanc\n");
        printf("3. Luminosite\n");
        printf("Votre choix : ");
        scanf("%d", &choix_filtre);

        if (choix_filtre == 1) {
            bmp24_negative(img);
            bmp24_saveImage(img, "resultat_negatif.bmp");
            printf("Image negative sauvee !\n");
        }
        else if (choix_filtre == 2) {
            bmp24_grayscale(img);
            bmp24_saveImage(img, "resultat_gris.bmp");
            printf("Image en noir et blanc sauvee !\n");
        }
        else if (choix_filtre == 3) {
            printf("Valeur luminosite (-100 a 100) : ");
            scanf("%d", &valeur);
            bmp24_brightness(img, valeur);
            bmp24_saveImage(img, "resultat_luminosite.bmp");
            printf("Luminosite appliquee !\n");
        }

        bmp24_free(img);
    }
    else {
        printf("Choix invalide !\n");
    }

    printf("\nTermine !\n");
    return 0;
}