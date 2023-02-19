#include <stdio.h>
#include <stdlib.h>
#include <map.h>

/**
 * \file map.c
 * \brief Gestion map
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 14 février 2023
 *
 * Gestion des map:
 * \n Initialisation en mémoire
 * \n Chargement des données de la map depuis les assets
 * \n Destruction en mémoire
 */

/**
 * \fn int load_map(char * fichier, map_t * map)
 * \brief Fonction interne qui charge la map depuis le fichier map sélectionné
 * 
 * \param fichier Chemin du fichier map
 * \param map Structure map_t où sont stockées les informations de la map
 * \return Int qui caractérise la réussite de la fonction
 */
int load_map(char * fichier, map_t * map) {
    FILE *file;
    int tile;

    // ouverture du fichier en mode lecture
    file = fopen(fichier, "r");

    // vérifie si l'ouverture du fichier a échoué
    if (file == NULL) {
        printf("Error opening file!");
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
 * \fn map_t * initialiser_map(char * fichier)
 * \brief Fonction externe qui charge la map depuis le fichier map sélectionné
 * 
 * \param fichier Chemin du fichier map
 * \return Pointeur sur une structure map_t qui correspond à la map qui vient d'être créée
 */
extern map_t * initialiser_map(char * fichier) {
    // initialisation et allocation mémoire de la structure map_t
    map_t * map = malloc( sizeof(map_t) );
    // génération de la matriceMap en appellant load_map
    load_map(fichier, map);
    // renvoie de la map final
    return map;
}

/**
 * \brief Fonction qui permet de détruire en mémoire la structure map_t passée en paramètre ainsi que ses dépendances
 * 
 * \param map Pointeur sur la structure map_t qu'on souhaite détruire
 * \return Aucun retour effectué en fin de fonction
 */
extern void detruire_map(map_t * map) {
    // destruction des lignes dans chaques layers de la matrice
    for (int n = 0; n < map->layer; n++) {
        for ( int i = 0; i < map->height; i++ ) {
            free(map->matrice[n][i]);
            map->matrice[n][i] = NULL;
        }
    }
    // destruction de chaque layers dans la matrice
    for (int n = 0; n < map->layer; n++) {
        free(map->matrice[n]);
        map->matrice[n] = NULL;
    }
    // destruction de la matrice
    free(map->matrice);
    map->matrice = NULL;
    // destruction de la structure map_t
    free(map);
    map = NULL;
}



    
