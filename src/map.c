#include <stdio.h>
#include <stdlib.h>
#include <map.h>


int** load_map(char * fichier, int* width, int* height, int* tileSize) {
    FILE *file;
    int tile;
    int **map;

    // ouverture du fichier en mode lecture
    file = fopen(fichier, "r");

    // vérifie si l'ouverture du fichier a échoué
    if (file == NULL) {
        printf("Error opening file!");
        return NULL;
    }

    // récupération des métadonnées (largeur map, longueur map, taille des tiles)
    fscanf(file, "%d", width);
    fscanf(file, "%d", height);
    fscanf(file, "%d", tileSize);

    // génération de la matrice map depuis le fichier
    map = (int**) malloc( *height * sizeof(int*) );
    for ( int i = 0; i < *height; i++ ) {
        map[i] = (int*) malloc( *width * sizeof(int) );
    }

    // chargement des numéros de tile dans la matrice depuis le fichier
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            fscanf(file, "%d:", &tile);
            map[i][j] = tile;
        }
    }

    // fermeture du fichier
    fclose(file);
    return map;
}

map_t * initialiser_map(char * fichier) {
    // initialisation et allocation mémoire de la structure map_t
    map_t * map = malloc( sizeof(map_t) );
    // génération de la matrice en appellant load_map
    map->matrice = load_map( fichier, &(map->width), &(map->height), &(map->tileSize) );
    // renvoie de la map final
    return map;
}

void detruire_map(map_t * map) {
    // libération de la mémoire de la matrice
    for (int i = 0; i < map->height; i++) {
        free(map->matrice[i]);
    }
    free(map->matrice);
    // libération de la mémoire de structure map
    free(map);
    map = NULL;
}



    
