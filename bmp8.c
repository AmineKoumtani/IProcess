//
// Created by koumt on 20/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Probléme d'ouverture du fichier %s",filename);
        return NULL;
    }
    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Probléme d'allocation mémoire du fichier %s",filename);
        fclose(file);
        return NULL;
    }
    fread(img->header, sizeof(unsigned char), 54, file);
    fread(img->colorTable, sizeof(unsigned char), 1024, file);
    img->width = *(unsign    ed int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned int *)(img->header + 28);
    img->dataSize = *(unsigned int *)(img->header + 34);

    //Vérification de la profondeur de couleur de l'image
    if (img->colorDepth != 8) {
        printf(stderr, "Probléme de profondeur de couleur différente de 8 bits\n");
        free(img);
        fclose(file);
        return NULL;
    }

    //Allocation image pour l'ensemble des données de l'image
    img->data = malloc(img->dataSize);

    if (!img->data) {
        perror("Erreur d'allocation mémoire pour les données");
        free(img);
        fclose(file);
        return NULL;
    }
    //Lecture des données et déplacement de celle-ci vers data
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}
void bmp8_saveImage(const char *filename, t_bmp8 *img) { //Cette fonction permet d’écrire une image en niveaux de gris dans un fichier BMP dont le nom (chemin)
                                                         //est renseigné par le paramètre filename.
    FILE *file = fopen(filename, "wb"); // fopen avec "wb" pour ouverture d'un nouveau fichier en binaire
    if (!file) {
        printf("Probléme de création du fichier %s\n", filename);
        return;
    }
    // Ecriture de l'en tete , la table de couleurs et les données de l'image
    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file); //fermeture du fichier
    printf("L'image à été enregistrée dans %s\n", filename);
}
void bmp8_free(t_bmp8 *img) { // Cette fonction permet de libérer la mémoire allouée pour stocker une image de type t_bmp8.
    if (img) { // Vérification facultative au cas ou le pointeur est invalide
        if (img->data) {
            free(img->data); // Vérification que "img" pointe bien vers un bloc mémoire valide
        }
        free(img);
    }
}
void bmp8_printInfo(t_bmp8 *img) {
    if (!img) // Vérification que img ne soit pas nul {
        printf("Erreur : image non chargée.\n");
        return;
    }
//Rappel que l'utilisation de %u est pour l'affichage d'un entier non signé
    printf("Image Info:\n");
    printf("  Width: %u\n", img->width);
    printf("  Height: %u\n", img->height);
    printf("  Color Depth: %u\n", img->colorDepth);
    printf("  Data Size: %u\n", img->dataSize);
}