#include <stdio.h>
#include <stdlib.h>
#include <map.h>

/**
 * \file map.c
 * \brief Gestion map
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 03 février 2023
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

    // récupération des métadonnées (largeur map, longueur map, taille des tiles)
    fscanf(file, "%d", &(map->width));
    fscanf(file, "%d", &(map->height));
    fscanf(file, "%d", &(map->tileSize));

    // génération de la matriceMap map depuis le fichier
    map->matriceMap = (int**) malloc( map->height * sizeof(int*) );
    for ( int i = 0; i < map->height; i++ ) {
        map->matriceMap[i] = (int*) malloc( map->width * sizeof(int) );
    }

    // chargement des numéros de tile dans la matriceMap depuis le fichier
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            fscanf(file, "%d,", &tile);
            map->matriceMap[i][j] = tile;
        }
    }

    // génération de la matriceDecor map depuis le fichier
    map->matriceDecor = (int**) malloc( map->height * sizeof(int*) );
    for ( int i = 0; i < map->height; i++ ) {
        map->matriceDecor[i] = (int*) malloc( map->width * sizeof(int) );
    }

    // chargement des numéros de tile dans la matriceDecor depuis le fichier
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            fscanf(file, "%d,", &tile);
            map->matriceDecor[i][j] = tile;
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
    // libération de la mémoire de la matriceMap
    for (int i = 0; i < map->height; i++) {
        free(map->matriceMap[i]);
    }
    free(map->matriceMap);
    // libération de la mémoire de la matriceDecor
    for (int i = 0; i < map->height; i++) {
        free(map->matriceDecor[i]);
    }
    free(map->matriceDecor);
    // libération de la mémoire de structure map
    free(map);
    map = NULL;
}



    
