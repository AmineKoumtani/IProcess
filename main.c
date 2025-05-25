#include <stdio.h>
#include "bmp8.h"

int main() {
    const char *cheminImage = "barbara_gray.bmp";
    const char *cheminCopie = "copie.bmp";

    // tentative de chargement de l'image.
    t_bmp8 *img = bmp8_loadImage(cheminImage);

    if (img) {
        bmp8_printInfo(img);
        bmp8_saveImage(cheminCopie, img);
        bmp8_free(img);
    } else {
        printf("raté\n"); // image non chargée (fichier absent ?)
    }

    return 0;
}