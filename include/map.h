#ifndef _MAP_H_
#define _MAP_H_

/**
 * \file map.h
 * \brief Header Gestion map
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 03 février 2023
 *
 * Header Gestion des map:
 * \n Structure map : map_t 
 * \n Signature des fonction externe de map.c
 */

/**
 * \typedef map_s map_t Struct de la map
 * \struct map_t
 * \brief Structure map
 * 
 * Contient:
 * \n width : largeur de la map
 * \n  height : hauteur de la map
 * \n  tileSize : dimension en pixel des tiles concernées
 * \n  matriceMap : Matrice qui correspond au tiles de la map principal
 * \n  matriceDecor :  Matrice qui correspond au tiles de la map des décors
 * 
 */
typedef struct map_s{
    int width;
    int height;
    int tileSize;
    int **matriceMap;
    int **matriceDecor;

}map_t;


extern map_t * initialiser_map(char * fichier) ;
extern void detruire_map(map_t * map) ;

#endif