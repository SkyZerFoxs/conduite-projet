#ifndef _MAP_H_
#define _MAP_H_

/*
 * DEFINITION OBJET "MAP"  
 */
typedef struct map_s{
    int width;
    int height;
    int tileSize;
    int **matrice;
}map_t;

/* 
 * FONCTIONS
 */
extern map_t * initialiser_map(char * fichier) ;
extern void detruire_map(map_t * map) ;

#endif