#include <stdio.h>
#include <stdlib.h>

#include <sprite.h>
#include <map.h>

/**
 * \file test_sprite.c
 * \brief Test des fonctionnalité des sprites
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 04 mars 2023
 *
 * Test sprites: 
 * \n Chargement de la listes des types de sprites
 * \n Chargement d'un sprite
 * \n Destruction de la liste des types de sprites
 * \n Destruction d'un sprite
 * \n Affichage des informations stockées
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la du main
 */
int main() {
    printf("------------------------------ Test Load Sprite Type ------------------------------\n");
    // chargement de la liste des types de sprite
    sprite_type_liste_t *listeType = Load_Sprite_Type("asset/sprite/spriteType.txt");

    for (int i = 0; i < listeType->nbElem; i++) {
        printf("ID=%d, Nom=%s, Nombre de frames=%d, Sprite sheet=%s, Taille=%d, Ligne=%d\n", i, 
        listeType->typeListe[i]->spriteName,  listeType->typeListe[i]->frameNumber,
        listeType->typeListe[i]->spriteSheet, listeType->typeListe[i]->spriteSize,
        listeType->typeListe[i]->spriteLine);
    }
    printf("\nnbElem=%d\n",listeType->nbElem);
    printf("----------------------------- Test Chargemment Sprite -----------------------------\n");
    // chargment de la map necessaire a Load_Sprite
    map_t * continent = Initialiser_Map("asset/map/map.txt");
    // chargement du sprite
    sprite_t * slime = Load_Sprite(0,0,0,0,listeType,continent);
    // affichage du sprite si il existe
    if ( slime != NULL ) {
        int i = slime->spriteTypeId;
        printf("x=%d, y=%d, frame=%d\n", slime->x, slime->y, slime->frame);
        printf("ID=%d, Nom=%s, Nombre de frames=%d, Sprite sheet=%s, Taille=%d, Ligne=%d\n", i, 
        listeType->typeListe[i]->spriteName,  listeType->typeListe[i]->frameNumber,
        listeType->typeListe[i]->spriteSheet, listeType->typeListe[i]->spriteSize,
        listeType->typeListe[i]->spriteLine);
    }
    Detruire_Sprite(&slime);

    /**/
    printf("---------------------------- Test Chargemment Sprite Map ---------------------------\n");

    sprite_t ***spriteMap = Load_SpriteMap(listeType,continent);

    
    
    // Affichage id sprite type depuis matrice spriteMap
    for (int y = 0; y < continent->height; y++) {
        for (int x = 0; x < continent->width; x++) {
            if ( spriteMap[y][x] != NULL ) {
                printf("%02d,",(spriteMap[y][x])->spriteTypeId);
            }
            else {
                printf("-1,");
            }
        }
        printf("\n");
    }

    Detruire_SpriteMap(&spriteMap,continent);


    Detruire_Map(continent);
    Detruire_Liste_Sprite_Type(&listeType);
    
    return 0;
}
