#include <stdio.h>
#include <stdlib.h>
#include <map.h>

/**
 * \file map.c
 * \brief Gestion map
 * \author Yamis MANFALOTI
 * \version 2.1
 * \date 9 mars 2023
 *
 * Gestion des map:
 * \n Initialisation en mémoire
 * \n Chargement des données de la map depuis les assets
 * \n Destruction en mémoire
 */

/**
 * \fn int Load_Map(char * fichier, map_t * map)
 * \brief Fonction interne qui charge la map depuis le fichier map sélectionné
 * 
 * \param fichier Chemin du fichier map
 * \param map Structure map_t où sont stockées les informations de la map
 * \return Int qui caractérise la réussite de la fonction
 */
int Load_Map(char * fichier, map_t * map) {
    FILE *file;
    int tile;

    // ouverture du fichier en mode lecture
    file = fopen(fichier, "r");

    // vérifie si l'ouverture du fichier a échoué
    if (file == NULL) {
        printf("Erreur : Echec ouverture fichier dans Load_Map()\n");
        return 1;
    }

    // récupération des métadonnées (largeur map, longueur map, taille des tiles, nombre de layer)
    fscanf(file, "%d", &(map->width));
    fscanf(file, "%d", &(map->height));
    fscanf(file, "%d", &(map->tileSize));
    fscanf(file, "%d", &(map->layer));

    // génération de la matriceMap map depuis les métadonées du fichier
    map->matrice = (int***)malloc( map->layer * sizeof(int**));
    for (int n = 0; n < map->layer; n++) {
        map->matrice[n] = (int**) malloc( map->height * sizeof(int*) );
        for ( int i = 0; i < map->height; i++ ) {
            map->matrice[n][i] = (int*) malloc( map->width * sizeof(int) );
        }
    }

    // remplissage de la matriceMap map depuis les données du fichier
    for (int n = 0; n < map->layer; n++) {
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->width; j++) {
                fscanf(file, "%d,", &tile);
                map->matrice[n][i][j] = tile;
            }
        }
    }

    // fermeture du fichier
    fclose(file);
    return 0;
}

/**
 * \fn map_t * Initialiser_Map(char * fichier)
 * \brief Fonction externe qui charge la map depuis le fichier map sélectionné
 * 
 * \param fichier Chemin du fichier map
 * \return Pointeur sur une structure map_t qui correspond à la map qui vient d'être créée
 */
extern map_t * Initialiser_Map(char * fichier) {
    // initialisation et allocation mémoire de la structure map_t
    map_t * map = malloc( sizeof(map_t) );
    // génération de la matriceMap en appellant Load_Map
    if ( Load_Map(fichier, map) ) {
        printf("Erreur : Echec Load_Map() dans Initialiser_Map()\n");
        return NULL;
    }
    // renvoie de la map final
    return map;
}

/**
 * \brief Fonction qui permet de détruire en mémoire la structure map_t passée en paramètre ainsi que ses dépendances
 * 
 * \param map Pointeur sur la structure map_t qu'on souhaite détruire
 * \return Aucun retour effectué en fin de fonction
 */
extern void Detruire_Map(map_t ** map) {
    if ( map == NULL ) {
        printf("Erreur : map inexsitant dans Detruire_Map() ");
        return;
    }
    // destruction des lignes dans chaques layers de la matrice
    for (int n = 0; n < (*map)->layer; n++) {
        for ( int i = 0; i < (*map)->height; i++ ) {
            free((*map)->matrice[n][i]);
            (*map)->matrice[n][i] = NULL;
        }
    }
    // destruction de chaque layers dans la matrice
    for (int n = 0; n < (*map)->layer; n++) {
        free((*map)->matrice[n]);
        (*map)->matrice[n] = NULL;
    }
    // destruction de la matrice
    free((*map)->matrice);
    (*map)->matrice = NULL;
    // destruction de la structure map_t
    free((*map));
    (*map) = NULL;
}

/**
 * \fn int Colision(map_t * map, int y, int x)
 * \brief Fonction externe qui verifie si il y a colision
 * 
 * \param map Structure map_t où sont stockées les informations de la map
 * \param y Coordonée y dans la map
 * \param x Coordonée x dans la map
 * \return 1 collision || 0 pas de colision | -1 Faill 
*/
extern int Colision(map_t * map, int y, int x) {
    if ( map == NULL ) {
       printf("Erreur : La Map n'est pas chargé dans Colision()\n");
       return -1;
    }

    if ( y < 0 || x < 0 || y >= map->height || x >= map->width ) {
        printf("Erreur : Coordonnées Invalide dans Colision()\n");
        return 1;
    }

    if ( map->matrice[COLISION_LAYER][y][x] > 0) {
        return 1;
    }
    else {
        return 0;
    }
}