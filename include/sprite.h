#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <map.h>

/**
 * \file affichage.h
 * \brief Header Gestion Sprite
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 03 mars 2023
 *
 * Header Gestion Sprite:
 * \n structure sprite_t
 * \n Signature des fonction externe de sprite.c
 */


/**
 * \typedef sprite_s sprite_t Structure Sprite
 * \struct sprite_t
 * \brief Structure des Sprites
 * 
 * Contient:
 * \n x             : Coordonnées x du sprite
 * \n y             : Coordonnées y du sprite
 * \n frame         : Frame courante du sprite
 * \n frameNumber   : Frame finale/maximal du sprite
 * \n spriteSheet   : Chemin du spriteSheet 
 * \n spriteSize    : Taille en pixel du sprite
 * \n spriteLine    : Indice de ligne du sprite dans la spriteSheet
 * 
 */
typedef struct sprite_s {
    //Coordonnées
    int x;
    int y;

    //Sa frame courante
    int frame;

    //Son frameNumber
    int frameNumber;

    // Metadonnées Texture Sprite
    char * spriteSheet;
    int spriteSize;
    int spriteLine;

}sprite_t;

typedef struct sprite_type_s {
    char * spriteName;
    int frameNumber;
    char * spriteSheet;
    int spriteSize;
    int spriteLine;
}sprite_type_t;

typedef struct sprite_liste_s {
    sprite_type_t ** liste;
    int nbElem;
}sprite_liste_t;



extern sprite_t * Load_Sprite(int x, int y, int frame, int frameNumber, char * spriteSheet, int spriteSize, int spriteLine);
extern void Detruire_Sprite( sprite_t ** sprite);

#endif