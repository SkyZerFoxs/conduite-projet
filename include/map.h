#ifndef _MAP_H_
#define _MAP_H_

#define LAST_TILEMAP_LAYER 3
#define COLISION_LAYER 4
#define SPRITEMAP_LAYER 5
#define ID_COFFRE_TILE 10908
#define MAX_NB_COFFRE 30

/**
 * \file map.h
 * \brief Header Gestion map
 * \author Yamis MANFALOTI
 * \version 2.1
 * \date 9 mars 2023
 *
 * Header Gestion des map:
 * \n Structure map : map_t 
 * \n Signature des fonction externe de map.c
 */

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */


/**
 * \typedef map_s map_t Struct de la map
 * \struct map_t
 * \brief Structure map
 * 
 * Contient:
 * \n width : largeur de la map
 * \n  height : hauteur de la map
 * \n  tileSize : dimension en pixel des tiles du tileset
 * \n  matrice : Matrice de map, Matrice[layer][ligne][colone]  
 * 
 */
typedef struct map_s{
    int width;
    int height;
    int tileSize;
    int layer;
    int ***matrice;
}map_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

extern map_t * Initialiser_Map(char * fichier) ;
extern void Detruire_Map(map_t ** map) ;
extern int Detecter_Coffre(map_t *map, int y_joueur, int x_joueur, char direction, int distance) ;

#endif