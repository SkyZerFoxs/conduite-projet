#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string.h>
#include <map.h>

#define MAX_SPRITE_FRAME 20
#define LAYER_SPRITE_MAP 4

/**
 * \file sprite.h
 * \brief Header Gestion Sprite
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 04 mars 2023
 *
 * Header Gestion Sprite:
 * \n structure sprite_type_t
 * \n structure sprite_type_liste_t
 * \n structure sprite_t
 * \n Signature des fonction externe de sprite.c
 */

/**
 * \typedef sprite_type_s sprite_type_t Structure Sprite Type
 * \struct sprite_type_t
 * \brief Structure avec pour attributs, les informations sur un type de sprite
 * 
 * Contient:
 * \n spriteName    : Nom du type de sprite
 * \n frameNumber   : Frame finale/maximal du sprite
 * \n spriteSheet   : Chemin du spriteSheet 
 * \n spriteSize    : Taille en pixel du sprite
 * \n spriteLine    : Indice de ligne du sprite dans la spriteSheet
 * 
*/
typedef struct sprite_type_s {
    // Nom du type de sprite
    char * spriteName;

    //Son frameNumber
    int frameNumber;
    
    // Metadonnées Texture Sprite
    char * spriteSheet;
    int spriteSize;
    int spriteLine;

}sprite_type_t;

/**
 * \typedef sprite_type_liste_s sprite_type_liste_t Structure Sprite Type Liste
 * \struct sprite_type_liste_t
 * \brief Liste des types de sprites
 * 
 * Contient:
 * \n typeListe     : Liste des types de sprite
 * \n nbElem;       : Nombre d'éléments dans la liste
 * 
*/
typedef struct sprite_type_liste_s {
    sprite_type_t ** typeListe;
    int nbElem;

}sprite_type_liste_t;

/**
 * \typedef sprite_s sprite_t Structure Sprite
 * \struct sprite_t
 * \brief Structure des Sprites
 * 
 * Contient:
 * \n x             : Coordonnées x du sprite
 * \n y             : Coordonnées y du sprite
 * \n frame         : Frame courante du sprite
 * \n spriteTypeId  : Id du type de sprite à afficheer (sprite_type_liste_t)
 * 
 */
typedef struct sprite_s {
    //Coordonnées
    int x;
    int y;

    //Sa frame courante
    int frame;

    //Son type
    int spriteTypeId;

}sprite_t;


extern void Detruire_Liste_Sprite_Type(sprite_type_liste_t ** liste) ;
extern sprite_type_liste_t * Load_Sprite_Type(const char * nom_fichier) ;

extern sprite_t * Load_Sprite(int x, int y, int frame, int spriteTypeId, sprite_type_liste_t * liste, map_t * map) ;
extern void Detruire_Sprite( sprite_t ** sprite) ;

extern sprite_t *** Load_SpriteMap(sprite_type_liste_t *listeType, map_t * map) ;
extern void Detruire_SpriteMap(sprite_t **** spriteMap, map_t * map) ;

#endif