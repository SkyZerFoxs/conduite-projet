#include <stdio.h>
#include <stdlib.h>
#include <sprite.h>

/**
 * \fn sprite_t * Load_Sprite(int x, int y, int frame, int frameNumber, char * spriteSheet, int spriteSize, int spriteLine)
 * \brief Fonction externe qui initialise une structure sprite_t
 * 
 * \param x Position x du sprite dans la vue du joueur
 * \param y Position y du sprite dans la vue du joueur
 * \param frame Frame courante du sprite
 * \param frameNumber Frame Final du sprite
 * \param spriteSheet Chemin du spriteSheet
 * \param spriteSize Taille en pixel du sprite
 * \param spriteLine Indice de ligne du sprite dans la spriteSheet
 * \return Return un pointeur sur la structure sprite_t formée avec les paramètres passées
 */
extern sprite_t * Load_Sprite(int x, int y, int frame, int frameNumber, char * spriteSheet, int spriteSize, int spriteLine) {
    // Allocation en mémoire
    sprite_t * sprite;
    sprite = malloc( sizeof(sprite_t) );

    //Initialisation des variables de position
    sprite->x = x;
    sprite->y = y;

    //Initialisation des variables d'animation
    sprite->frame = frame;
    sprite->frameNumber = frameNumber;

    // Initialisation des metadonnées des texture du sprite
    sprite->spriteSheet = spriteSheet;
    sprite->spriteSize = spriteSize;
    sprite->spriteLine = spriteLine;

    // Return un pointeur sur la structure sprite_t
    return sprite;
}

/**
 * \fn void Detruire_Sprite( sprite_t ** sprite)
 * \brief Fonction externe qui détruit en mémoire la structure sprite_t passé en paramètre
 * 
 * \param sprite Pointeur de pointeur sur l'objet SDL_Texture
 * \return Aucun retour effectué en fin de fonction
 */
extern void Detruire_Sprite( sprite_t ** sprite) {
    free((*sprite));
    (*sprite) = NULL;
}

extern sprite_type_t * Load_Sprite_Type(int frameNumber, char * spriteSheet, int spriteSize, int spriteLine) {
    // Allocation en mémoire
    sprite_type_t * spriteType;
    spriteType = malloc( sizeof(sprite_type_t) );

    //Initialisation des variables d'animation
    spriteType->frameNumber = frameNumber;

    // Initialisation des metadonnées des texture du spriteType
    spriteType->spriteSheet = spriteSheet;
    spriteType->spriteSize = spriteSize;
    spriteType->spriteLine = spriteLine;

    // Return un pointeur sur la structure spriteType
    return spriteType;
}

extern void Detruire_Sprite_Type( sprite_type_t ** spriteType) {
    free((*spriteType));
    (*spriteType) = NULL;
}