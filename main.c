#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include "bmp24.h"
#include <direct.h>

int main() {
    int type, filtre, valeur;
    char fichier[256];

    while (1) {
        // Menu principal
        printf("\n--- MENU ---\n");
        printf("0. Quitter\n");
        printf("1. Image noir et blanc\n");
        printf("2. Image couleur\n");
        printf("Choix : ");
        scanf("%d", &type);
        getchar();

        if (type == 0) break;

        // Demande du nom du fichier image
        printf("Nom du fichier : ");
        fgets(fichier, sizeof(fichier), stdin);
        fichier[strcspn(fichier, "\n")] = 0; // Suppression du \n

        if (type == 1) {
            // Traitement d'une image 8 bits
            t_bmp8 *img = bmp8_loadImage(fichier);
            if (!img) {
                printf("Fichier introuvable.\n");
                continue;
            }

            // Menu des filtres 8 bits
            printf("1. Négatif\n");
            printf("2. Luminosité\n");
            printf("3. Seuil\n");
            printf("Choix : ");
            scanf("%d", &filtre);

            if (filtre == 1) {
                bmp8_negative(img);
                bmp8_saveImage("negatif.bmp", img);
            } else if (filtre == 2) {
                printf("Valeur luminosité : ");
                scanf("%d", &valeur);
                bmp8_brightness(img, valeur);
                bmp8_saveImage("lumineux.bmp", img);
            } else if (filtre == 3) {
                printf("Seuil : ");
                scanf("%d", &valeur);
                bmp8_threshold(img, valeur);
                bmp8_saveImage("seuil.bmp", img);
            } else {
                printf("Option invalide.\n");
            }

            bmp8_free(img);
        }

        else if (type == 2) {
            // Traitement d'une image 24 bits
            t_bmp24 *img = bmp24_loadImage(fichier);
            if (!img) {
                printf("Fichier introuvable.\n");
                continue;
            }

            // Menu des filtres 24 bits
            printf("1. Négatif\n");
            printf("2. Niveaux de gris\n");
            printf("3. Luminosité\n");
            printf("4. Effets\n");
            printf("Choix : ");
            scanf("%d", &filtre);

            if (filtre == 1) {
                bmp24_negative(img);
                bmp24_saveImage("negatif.bmp", img);
            } else if (filtre == 2) {
                bmp24_grayscale(img);
                bmp24_saveImage("gris.bmp", img);
            } else if (filtre == 3) {
                printf("Valeur luminosité : ");
                scanf("%d", &valeur);
                bmp24_brightness(img, valeur);
                bmp24_saveImage("lumineux.bmp", img);
            } else if (filtre == 4) {
                // Menu des effets spéciaux (convolutions)
                printf("1. Flou simple\n");
                printf("2. Flou doux\n");
                printf("3. Contours\n");
                printf("4. Relief\n");
                printf("5. Netteté\n");
                printf("Choix : ");
                int effet;
                scanf("%d", &effet);

                if (effet == 1) {
                    bmp24_boxBlur(img);
                    bmp24_saveImage("flou.bmp", img);
                } else if (effet == 2) {
                    bmp24_gaussianBlur(img);
                    bmp24_saveImage("doux.bmp", img);
                } else if (effet == 3) {
                    bmp24_outline(img);
                    bmp24_saveImage("contours.bmp", img);
                } else if (effet == 4) {
                    bmp24_emboss(img);
                    bmp24_saveImage("relief.bmp", img);
                } else if (effet == 5) {
                    bmp24_sharpen(img);
                    bmp24_saveImage("net.bmp", img);
                } else {
                    printf("Option invalide.\n");
                }
            } else {
                printf("Option invalide.\n");
            }

            bmp24_free(img);
        }

        else {
            printf("Choix invalide.\n");
        }
    }

    return 0;
}