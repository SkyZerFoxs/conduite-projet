#include <stdio.h>
#include <stdlib.h>
#include <map.h>


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
            fscanf(file, "%d:", &tile);
            map->matriceMap[i][j] = tile;
        }
    }

    // fermeture du fichier
    fclose(file);
    return 0;
}

map_t * initialiser_map(char * fichier) {
    // initialisation et allocation mémoire de la structure map_t
    map_t * map = malloc( sizeof(map_t) );
    // génération de la matriceMap en appellant load_map
    load_map( fichier, map);
    // renvoie de la map final
    return map;
}

void detruire_map(map_t * map) {
    // libération de la mémoire de la matriceMap
    for (int i = 0; i < map->height; i++) {
        free(map->matriceMap[i]);
    }
    free(map->matriceMap);
    // libération de la mémoire de structure map
    free(map);
    map = NULL;
}



    
