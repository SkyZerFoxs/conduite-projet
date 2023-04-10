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
    if ( map == NULL || (*map) == NULL ) {
        printf("Erreur : map inexsitant dans Detruire_Map() ");
        return;
    }

    if ( (*map)->layer <= 0 || (*map)->height <= 0 ) {
        printf("Erreur : layer ou height <= 0 dans Detruire_Map()\n" );
        return;
    }

    // destruction des lignes dans chaques layers de la matrice
    for (int n = 0; n < (*map)->layer; n++) {
        for ( int i = 0; i < (*map)->height; i++ ) {
            if ( (*map)->matrice[n][i] != NULL ) {
                free((*map)->matrice[n][i]);
                (*map)->matrice[n][i] = NULL;
            }
            
        }
        if ( (*map)->matrice[n] != NULL ) {
            free((*map)->matrice[n]);
            (*map)->matrice[n] = NULL;
        }
    }

    // destruction de la matrice
    if ( (*map)->matrice != NULL ) {
        free((*map)->matrice);
        (*map)->matrice = NULL;
    }
    
    // destruction de la structure map_t
    free((*map));
    (*map) = NULL;
}

/** 
 * \fn int Detecter_Coffre(map_t *map, int y_joueur, int x_joueur, char direction, int distance)
 * \brief Fonction externe qui détecte si un coffre est proche et renvoie un booleen
 *
 * \param map Pointeur sur map_t, la map à partir de laquelle charger la matrice de sprites.
 * \param y_joueur Coordonnée y du joueur sur la carte.
 * \param x_joueur Coordonnée x du joueur sur la carte.
 * \param direction Direction dans laquelle le joueur regarde.
 * \param distance Distance maximale à laquelle chercher les un pnj.
 * \return 1 si un coffre a été trouvé, 0 si aucun coffre n'a été trouvé, -1 en cas d'erreur.
 */
extern int Detecter_Coffre(map_t *map, int y_joueur, int x_joueur, char direction, int distance) {
    // Vérification des paramètres d'entrée
    if (map == NULL) {
        printf("Erreur : Map Inexistante dans Detecter_Coffre()\n");
        return -1;
    }
    if (y_joueur < 0 || x_joueur < 0 || y_joueur >= map->height || x_joueur >= map->width) {
        printf("Erreur : Coordonnées dans la spriteMap ( Emplacement Destination ) Invalide dans Detecter_Coffre()\n");
        return -1;
    }

    // Initialisation des variables de detection
    int dx = 0, dy = 0;
    
    switch (direction) {
        case 'Z':
            dy = -1;
            break;
        case 'Q':
            dx = -1;
            break;
        case 'S':
            dy = 1;
            break;
        case 'D':
            dx = 1;
            break;
        default:
            printf("Erreur : Direction Invalide dans Detecter_Coffre()\n");
            return -1;
    }

    // Variables
    int detectionCoffre = 0, zoneLevel;
    int coffreLevel = 0;
    int x, y;
    for (int d = 1; d <= distance; d++) {
        x = x_joueur + dx * d;
        y = y_joueur + dy * d;
        if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
            printf("Erreur : On est en dehors de la carte dans Detecter_Coffre()\n");
            return -1;
            break;
        }
        if ( map->matrice[2][y][x] == ID_COFFRE_TILE ) {
            // On a trouvé un coffre
            detectionCoffre = 1;
            break;
        }
    }

    if ( detectionCoffre ) {
        // Calcule niveau du coffre
        zoneLevel = map->matrice[6][y][x];
        if ( zoneLevel < 5 || zoneLevel % 5 != 0 ) {
            printf("Erreur : matZoneLevel[%d][%d] incorrecte dans Detecter_Coffre()\n", y, x);
            return -1;
        }
        if ( zoneLevel >= 5 || zoneLevel <= 10 ) {
            coffreLevel = 1;
        }
        else if ( zoneLevel > 10 || zoneLevel <= 20 ) {
            coffreLevel = 2;
        }
        else {
            coffreLevel = 3;
        }
        // Changement tile de coffre ferme vers tile de coffre ouvert
        map->matrice[2][y][x] += 1;
    }

    // Aucun coffre trouvé
    return coffreLevel;
}