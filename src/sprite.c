#include <stdio.h>
#include <stdlib.h>

#include <sprite.h>


/**
 * \file sprite.c
 * \brief Gestion des sprites
 * \author Yamis MANFALOTI
 * \version 2.2
 * \date 12 mars 2023
 *
 * Gestion des sprites:
 * \n Chargement des données des types de sprite depuis un fichier
 * \n Chargement de la listes des types de sprites
 * \n Chargement d'un sprite
 * \n Destruction de la liste des types de sprites
 * \n Destruction d'un sprite
 */

/**
 * \fn int Charger_Sprite_Type(const char * nom_fichier, sprite_type_liste_t * liste)
 * \brief Fonction externe qui charge les données de sprites depuis un fichier
 * 
 * \param nom_fichier Le nom du fichier à lire
 * \param liste Pointeur sur une structure sprite_type_liste_t où stocker les données des sprites
 * \return Un entier qui caractérise la réussite ou non de la fonction
 */
static int Charger_Sprite_Type(const char * nom_fichier, sprite_type_liste_t * liste) {
    // initialisation des variables
    FILE *fichier;
    char ligne[256];
    char *token;
    int nb_elements = 0;

    // Ouverture du fichier en mode lecture
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Echec ouverture fichier dans Charger_Sprite_Type()\n");
        return 1;
    }

    // Comptage du nombre de lignes du fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nb_elements++;
    }

    // Allocation de la liste des types de sprites
    liste->typeListe = (sprite_type_t**) malloc(nb_elements * sizeof(sprite_type_t*));
    if (liste->typeListe == NULL) {
        printf("Erreur : Echec malloc(liste->typeListe) dans Charger_Sprite_Type()\n");
        fclose(fichier);
        return 1;
    }
    liste->nbElem = nb_elements;

    // Retour au début du fichier
    rewind(fichier);

    // Remplissage de la liste de sprites
    int i = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        sprite_type_t *sprite = malloc(sizeof(sprite_type_t));
        if (sprite == NULL) {
            printf("Erreur : Echec malloc(sprite) dans Charger_Sprite_Type() \n");
            fclose(fichier);
            return 1;
        }

        token = strtok(ligne, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite);
            return 1;
        }
        
        sprite->spriteName = malloc((strlen(token)+1) * sizeof(char));
        if (sprite->spriteName == NULL) {
            printf("Erreur : Echec malloc(sprite->spriteName) dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite);
            return 1;
        }
        strcpy(sprite->spriteName, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite);
            return 1;
        }
        sprite->frameNumber = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite);
            return 1;
        }

        sprite->spriteSheet = malloc((strlen(token)+1) * sizeof(char));
        if (sprite->spriteSheet == NULL) {
            printf("Erreur : Echec malloc(sprite->spriteSheet) dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        strcpy(sprite->spriteSheet, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->spriteSize = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->spriteLine = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : Mauvais format dans le fichier dans Charger_Sprite_Type()\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->frameCat = atoi(token);

        sprite->textureId = -1;

        liste->typeListe[i] = sprite;
        i++;
    }

    // Fermeture du fichier
    fclose(fichier);

    return 0;
}

/**
 * \fn void Detruire_Liste_Sprite_Type(sprite_type_liste_t * liste)
 * \brief Fonction qui détruit la liste des types de sprites
 * \param liste Pointeur de pointeur sur la structure sprite_type_liste_t à détruire
 * \return Aucun retours
 */
extern void Detruire_Liste_Sprite_Type(sprite_type_liste_t ** liste) {
    if (liste == NULL || *liste == NULL) {
        printf("Erreur : Liste de types de sprites vide dans Detruire_Liste_Sprite_Type()\n");
        return;
    }

    if ((*liste)->typeListe == NULL) {
        printf("Erreur : Liste vide dans Detruire_Liste_Sprite_Type()\n");
        return;
    }

    if ( (*liste)->nbElem <= 0 ) {
        printf("Erreur : Nombre element <= 0 dans Detruire_Liste_Sprite_Type()\n");
        return;
    }

    // parcourt de la liste des types
    for (int i = 0; i < (*liste)->nbElem; i++) {
        sprite_type_t * sprite = (*liste)->typeListe[i];
        // si l'element de la liste des types existe
        if (sprite != NULL) {
            if ( sprite->spriteName != NULL ) {
                free(sprite->spriteName);
            }
            if ( sprite->spriteSheet != NULL ) {
                free(sprite->spriteSheet);
            }
            free(sprite);
        }
    }
    // free de la liste des types
    free((*liste)->typeListe);
    (*liste)->typeListe = NULL;
        
    // reset de nbElem
    (*liste)->nbElem = 0;

    // free de la liste
    free((*liste));
    (*liste) = NULL;
    
}


/**
 * \fn sprite_type_liste_t * Load_Sprite_Type(const char * nom_fichier)
 * \brief Fonction qui charge les données de sprites depuis un fichier et retourne un pointeur sur la liste de sprites chargée.
 * \param nom_fichier Le nom du fichier à lire
 * \return Pointeur sur une structure sprite_type_liste_t où sont stockées les données des sprites
*/
extern sprite_type_liste_t * Load_Sprite_Type(const char * nom_fichier) {
    sprite_type_liste_t *liste = malloc(sizeof(sprite_type_liste_t));
    if (liste == NULL) {
        printf("Erreur : Echec malloc(liste) dans Load_Sprite_Type()\n");
        return(NULL);
    }

    if (Charger_Sprite_Type(nom_fichier, liste) != 0) {
        printf("Erreur : Echec chargement sprites dans Load_Sprite_Type()\n");
        Detruire_Liste_Sprite_Type(&liste);
        return(NULL);
    }

    return liste;
}

/**
 * \fn sprite_t * Load_Sprite(int x, int y, int frame, int frameNumber, char * spriteSheet, int spriteSize, int spriteLine)
 * \brief Fonction externe qui initialise une structure sprite_t
 * 
 * \param x Position x du sprite dans la vue du joueur
 * \param y Position y du sprite dans la vue du joueur
 * \param frame Frame courante du sprite
 * \param spriteTypeId Id du type de sprite à afficher
 * \return Return un pointeur sur la structure sprite_t formée avec les paramètres passées
 */
extern sprite_t * Load_Sprite(int x, int y, int frame, int spriteTypeId, sprite_type_liste_t * liste, map_t * map) {
    // Allocation en mémoire
    sprite_t * sprite;
    sprite = malloc( sizeof(sprite_t) );

    //Initialisation des variables de position
    if ( x >= 0 && x < map->width ) {
        sprite->x = x;
    }
    else {
        printf("Erreur : x Invalide dans Load_Sprite()\n");
        free(sprite);
        return NULL;
    }
    if ( y >= 0 && y < map->height ) {
        sprite->y = y;
    }
    else {
        printf("Erreur : y Invalide dans Load_Sprite()\n");
        free(sprite);
        return NULL;
    }

    //Initialisation de la variables de frame courante
    if ( frame >= 0 && frame < MAX_SPRITE_FRAME ) {
        sprite->frame = frame;
    }
    else {
        printf("Erreur : frame Invalide dans Load_Sprite()\n");
        free(sprite);
        return NULL;
    }

    //Initialisation de la variable de type du sprite
    if ( spriteTypeId >= 0 && spriteTypeId < liste->nbElem ) {
        sprite->spriteTypeId = spriteTypeId;
    }
    else {
        printf("Erreur : spriteTypeId Invalide dans Load_Sprite()\n");
        free(sprite);
        return NULL;
    }

    sprite->monstre = NULL;

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
    if ( sprite == NULL ) {
        printf("Erreur : aucun sprite passé en paramètre dans Detruire_Sprite()\n");
        return;
    }

    if ( (*sprite) != NULL ) {
        free((*sprite));
        (*sprite) = NULL;
    }
}

/**
 * \fn void Detruire_SpriteMap(sprite_t **** spriteMap, map_t * map)
 * \brief Fonction externe qui libère la mémoire allouée pour la matrice de sprites.
 * 
 * \param spriteMap Pointeur de pointeur de pointeur de pointeur sur sprite_t, Le pointeur vers la matrice de sprites à libérer.
 * \param map Pointeur sur map_t, la carte correspondante.
 * 
 * \return Aucun retour effectué en fin de fonction.
 */
void Detruire_SpriteMap(sprite_t *****spriteMap, map_t *map) {
    if (spriteMap != NULL || *spriteMap == NULL ) {
        for (int i = 0; i < 2; i++) {
            if ((*spriteMap)[i] != NULL) {
                for (int y = 0; y < map->height; y++) {
                    if ((*spriteMap)[i][y] != NULL) {
                        for (int x = 0; x < map->width; x++) {
                            if ((*spriteMap)[i][y][x] != NULL) {
                                Detruire_Sprite(&((*spriteMap)[i][y][x]));
                            }
                        }
                        free((*spriteMap)[i][y]);
                        (*spriteMap)[i][y] = NULL;
                    }
                }
                free((*spriteMap)[i]);
                (*spriteMap)[i] = NULL;
            }
            else {
                printf("Erreur : La spriteMap n'existe pas dans Detruire_SpriteMap()\n");
                return;
            }
        }
        free(*spriteMap);
        *spriteMap = NULL;
    }
    else {
        printf("Erreur : Pointeur Passé En Paramètre Invalide dans Detruire_SpriteMap()\n");
        return;
    }
}

/**
 * \fn sprite_t *** Load_SpriteMap(sprite_type_liste_t *listeType, map_t * map)
 * \brief Fonction externe qui charge la matrice de sprites à partir d'une liste de types des sprites et de la map.
 * 
 * \param listeType Pointeur sur sprite_type_liste_t , la liste des types de sprites.
 * \param map Pointeur sur map_t, la map à partir de laquelle charger la matrice de sprites.
 * 
 * \return Un pointeur vers la matrice de sprites chargée, ou NULL en cas d'erreur.
 */
extern sprite_t **** Load_SpriteMap(sprite_type_liste_t *listeType, map_t * map) {
    // Initialisation matrice spriteMap
    sprite_t ****spriteMap = (sprite_t ****) malloc( sizeof(sprite_t ***) * 2);
    if ( spriteMap != NULL ) {
        for (int i = 0; i < 2; i++ ) {
            spriteMap[i] = (sprite_t ***) malloc( map->height * sizeof(sprite_t **) );
            if ( spriteMap[i] == NULL ) {
                printf("Erreur : Echec Malloc sprite_t ***spriteMap[%d] dans Load_SpriteMap()\n",i);
                Detruire_SpriteMap(&spriteMap,map);
                if ( i == 1 ) {
                    free(spriteMap[1]);
                }
                free(spriteMap);
                return NULL;
            }
            for (int y = 0; y < map->height; y++) {
                spriteMap[i][y] = (sprite_t **) malloc(map->width * sizeof(sprite_t *));
                if ( spriteMap[i][y] == NULL ) {
                    printf("Erreur : Echec Malloc sprite_t ***spriteMap[%d][%d] dans Load_SpriteMap()\n",i,y);
                    Detruire_SpriteMap(&spriteMap,map);
                    free(spriteMap[1]);
                    free(spriteMap);
                    return NULL;
                }
            }
        }
    }
    else {
        printf("Erreur : Echec Malloc sprite_t ***spriteMap dans Load_SpriteMap()\n");
        return NULL;
    }

    // Chargement matrice spriteMap
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (map->matrice[SPRITEMAP_LAYER][y][x] >= BORNE_PERSO_SPRITE) {
                sprite_t * sprite = Load_Sprite(x, y, 0, map->matrice[SPRITEMAP_LAYER][y][x], listeType, map);
                if (sprite != NULL) {
                    spriteMap[0][y][x] = sprite;
                } else {
                    printf("Erreur : Echec Load_Sprite matrice[4][%d][%d] dans Load_SpriteMap()\n",y,x);
                    Detruire_SpriteMap(&spriteMap,map);
                    return NULL;
                }
            }
            else {
                spriteMap[0][y][x] = NULL;
            }
            spriteMap[1][y][x] = NULL;
        }
    }

    return spriteMap;
}

/*
extern monstre_t *** Load_MapMonstre(sprite_t **** spriteMap, sprite_type_liste_t *listeType, map_t * map) {

    monstre_t ***mat = (monstre_t ***) malloc(map->height * sizeof(monstre_t **));
    if (mat == NULL) {
        printf("Erreur : Echec malloc(mat) dans Load_MapMonstre()\n");
        return NULL;
    }

    for (int y = 0; y < map->height; y++) {
        mat[y] = (monstre_t **) malloc(map->width * sizeof(monstre_t *));
        if (mat[y] == NULL) {
            printf("Erreur : Echec malloc(mat[%d]) dans Load_MapMonstre()\n",y);
            return NULL;
        }

        for (int x = 0; x < map->width; x++) {
            mat[y][x] = NULL;
            if ( spriteMap[0][y][x] != NULL ) {
                mat[y][x] = creer_monstre(listeType->typeListe[spriteMap[0][y][x]->spriteTypeId]->spriteName,1,y,x);
                if ( mat[y][x] == NULL ) {
                    printf("Erreur : Echec creer_monstre() dans Load_MapMonstre()\n");
                    return NULL;
                }
            }
        }
    }
    return mat;
}
*/

/**
 * \fn int Deplacement_Sprite(sprite_t **** spriteMap, map_t * map, int y1, int x1, int y2, int x2)
 * \brief Fonction externe qui echange l'emplacement de deux sprite dans la SpriteMap
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur map_t, la carte correspondante.
 * \param y1 Coordonée y dans la spriteMap du sprite N°1
 * \param x1 Coordonée x dans la spriteMap du sprite N°1
 * \param y2 Coordonée y dans la spriteMap du sprite N°2
 * \param x2 Coordonée x dans la spriteMap du sprite N°2
 * \return 0 success || 1 Fail
*/
extern int Deplacement_Sprite(sprite_t **** spriteMap, map_t * map, int y1, int x1, int y2, int x2) {
    if ( spriteMap == NULL ) {
        printf("Erreur : spriteMap[1] Inexistante dans Deplacement_Sprite()\n");
        return 1;
    }

    if ( y1 < 0 || x1 < 0 || y1 >= map->height || x1 >= map->width ) {
        printf("Erreur : Coordonnées N°1 ( Emplacement Source ) Invalide dans Deplacement_Sprite()\n");
        return 1;
    }

    if ( y2 < 0 || x2 < 0 || y2 >= map->height || x2 >= map->width ) {
        printf("Erreur : Coordonnées N°2 ( Emplacement Destination ) Invalide dans Deplacement_Sprite()\n");
        return 1;
    }

    if ( spriteMap[1][y2][x2] != NULL) {
        printf("Erreur : L'emplacement aux coordonnées N°2 ( Emplacement Destination ) n'est pas vide dans Deplacement_Sprite()\n");
        return 1;
    }

    sprite_t * temp;
    temp = spriteMap[1][y1][x1];
    spriteMap[1][y1][x1] = spriteMap[1][y2][x2];
    spriteMap[1][y2][x2] = temp;
    spriteMap[1][y2][x2]->x = x2;
    spriteMap[1][y2][x2]->y = y2;

    return 0;
}


/**
 * \fn int Copy_Sprite(sprite_t **** spriteMap, map_t * map, int y1, int x1, int y2, int x2)
 * \brief Fonction externe qui permet de copier un sprite de la spriteMap vers un emplacement dans la spriteMap
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur map_t, la carte correspondante.
 * \param sprite Pointeur sur le sprite qui remplacera dans la spriteMap.
 * \param y1 Coordonée y1 dans la spriteMap du pointeur de sprite ( source ).
 * \param x1 Coordonée x1 dans la spriteMap du pointeur de sprite ( source ).
 * \param y2 Coordonée y2 dans la spriteMap du pointeur de sprite ( destination ).
 * \param x2 Coordonée x2 dans la spriteMap du pointeur de sprite ( destination ).
* \return 0 success || 1 Fail
*/
extern int Copy_Sprite(sprite_t **** spriteMap, map_t * map, int y1, int x1, int y2, int x2) {
    if ( spriteMap == NULL ) {
        printf("Erreur : spriteMap Inexistante dans Copy_Sprite()\n");
        return 1;
    }

    if ( map == NULL ) {
        printf("Erreur : Map Inexistante dans Copy_Sprite()\n");
        return 1;
    }

    if ( y1 < 0 || x1 < 0 || y1 >= map->height || x1 >= map->width ) {
        printf("Erreur : Coordonnées N°1 ( Emplacement Destination ) Invalide dans Copy_Sprite()\n");
        return 1;
    }

    if ( y2 < 0 || x2 < 0 || y2 >= map->height || x2 >= map->width ) {
        printf("Erreur : Coordonnées N°2 ( Emplacement Source ) Invalide dans Copy_Sprite()\n");
        return 1;
    }

    if ( spriteMap[y1][x1] == NULL) {
        printf("Erreur : L'emplacement aux coordonnées N°1 ( Emplacement Source ) est vide dans Copy_Sprite()\n");
        return 1;
    }

    spriteMap[1][y2][x2] = spriteMap[1][y1][x1];

    return 0;
}


/**
 * \fn int Change_Sprite(sprite_t **** spriteMap, map_t * map, sprite_t * sprite, int y, int x)
 * \brief Fonction externe qui permet de changer le sprite dans la spriteMap au coordonnées y et x
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur map_t, la carte correspondante.
 * \param sprite Pointeur sur le sprite qui remplacera dans la spriteMap.
 * \param y Coordonée y dans la spriteMap du pointeur de sprite.
 * \param x Coordonée x dans la spriteMap du pointeur de sprite.
 * \return 0 success || 1 Fail
*/
extern int Change_Sprite(sprite_t **** spriteMap, map_t * map, sprite_t * sprite, int y, int x) {
    if ( spriteMap == NULL ) {
        printf("Erreur : spriteMap Inexistante dans Change_Sprite()\n");
        return 1;
    }

    if ( map == NULL ) {
        printf("Erreur : Map Inexistante dans Change_Sprite()\n");
        return 1;
    }

    if ( sprite == NULL) {
        printf("Erreur : Le sprite source est vide dans Change_Sprite()\n");
        return 1;
    }

    if ( y < 0 || x < 0 || y >= map->height || x >= map->width ) {
        printf("Erreur : Coordonnées dans la spriteMap ( Emplacement Destination ) Invalide dans Change_Sprite()\n");
        return 1;
    }

    sprite->x = x;
    sprite->y = y;

    spriteMap[1][y][x] = sprite;

    return 0;
}

/**
 * \fn int Colision(map_t * map, int y, int x)
 * \brief Fonction externe qui verifie si il y a colision
 * 
 * \param map Structure map_t où sont stockées les informations de la map
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap dans laquelle on travaille
 * \param direction Char qui correspond au sens du deplacement lors de l'appelle de Colision
 * \param y Coordonée y dans la map
 * \param x Coordonée x dans la map
 * \return 1 collision || 0 pas de colision | -1 Fail 
*/
extern int Colision(map_t * map, sprite_t **** spriteMap, char direction, int y, int x) {
    if ( map == NULL ) {
       printf("Erreur : La Map n'est pas chargé dans Colision()\n");
       return -1;
    }

    if ( y < 0 || x < 0 || y >= map->height || x >= map->width ) {
        printf("Erreur : Coordonnées Invalide dans Colision()\n");
        return -1;
    }

    if ( map->matrice[COLISION_LAYER][y][x] > 0) {
        return 1;
    }

    if ( direction != 'z' && direction != 'q' && direction != 's' && direction != 'd' ) {
        printf("Erreur : Direction Incorrecte dans Colision()\n");
        return -1;
    }

    if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
        return 1;
    }

    if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->pnj != NULL ) {
        return 1;
    }

    /*
    switch ( direction) {
        case 'z':
            if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
                return 1;
            }
            break;
        case 'd':
            if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
                return 1;
            }
            break;
        case 'q':
            if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
                return 1;
            }
            break;
        case 's':
            if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
                return 1;
            }
            break;
        default:
            printf("Erreur : Direction Incorrecte dans Colision()\n");
            return -1;
            break;
    }
    */

    return 0;
}

/**
 * \param listeType Pointeur sur sprite_type_liste_t , la liste des types de sprites.
 * \param map Pointeur sur map_t, la map à partir de laquelle charger la matrice de sprites.
 * \param debut Indice du spriteTypeId du premier sprite de la liste
 * \param fin Indice du spriteTypeId du dernier sprite de la liste
 * 
 * \return Un pointeur vers la liste de sprites du perso chargée || fail NULL .
 */
extern sprite_liste_t * Load_PersoSprite_List(sprite_type_liste_t * listeType, map_t * map, int debut, int fin) {
    if (listeType == NULL) {
        printf("Erreur : ListeType Inexistante Dans Load_PersoSprite_List().\n");
        return NULL;
    }

    if (map == NULL) {
        printf("Erreur : La Map est inexistante dans Load_PersoSprite_List().\n");
        return NULL;
    }

    if ( debut < 0 || debut > fin || debut >= listeType->nbElem ) {
        printf("Erreur : L'indice de debut est incorrecte dans Load_PersoSprite_List()\n");
        return NULL;
    }

    if (  fin < 0 || fin >= listeType->nbElem || fin < debut  ) {
        printf("Erreur : L'indice de fin est incorrecte dans Load_PersoSprite_List()\n");
        return NULL;
    }

    sprite_liste_t * liste = malloc(sizeof(sprite_liste_t));
    if (liste == NULL) {
        printf("Erreur : Echec maloc(liste) dans Load_PersoSprite_List().\n");
        return NULL;
    }

    int nbElem = fin - debut + 1;
    liste->nbElem = nbElem;
    liste->spriteListe = malloc(nbElem * sizeof(sprite_t *));
    if (liste->spriteListe == NULL) {
        printf("Erreur : Echec malloc(liste->spriteListe) dans Load_PersoSprite_List().\n");
        free(liste);
        return NULL;
    }

    for (int i = debut; i < fin; i++) {
        sprite_t * sprite = Load_Sprite(0, 0, 0, i, listeType, map);
        if (sprite == NULL) {
            printf("Erreur : Echec Load_Sprite() dans Load_PersoSprite_List().\n");
            for (int j = 0; j < i - debut; j++) {
                free(liste->spriteListe[j]);
            }
            free(liste->spriteListe);
            free(liste);
            return NULL;
        }
        liste->spriteListe[i - debut] = sprite;
    }

    return liste;
}

/**
 * \fn void Detruire_Sprite_Liste(sprite_liste_t **liste)
 * \brief Fonction externe qui détruit en mémoire la structure sprite_liste_t passée en paramètre, ainsi que tous les sprites qu'elle contient
 * 
 * \param liste Pointeur de pointeur vers la structure sprite_liste_t à détruire
 * \return Aucun retour effectué en fin de fonction
 */
extern void Detruire_Sprite_Liste(sprite_liste_t ** liste) {
    if ( liste == NULL || (*liste) == NULL ) {
        printf("Erreur : aucune liste de sprite passé en paramètre dans Detruire_Sprite_Liste()\n");
        return;
    }

    if ((*liste)->spriteListe == NULL) {
        printf("Erreur : la liste est vide dans Detruire_Sprite_Liste()\n");
        return;
    }

    if ((*liste)->nbElem <= 0) {
        printf("Erreur : le nombre d'élément est <= 0 dans Detruire_Sprite_Liste()\n");
        return;
    }


    for (int i = 0; i < (*liste)->nbElem; i++) {
        if ((*liste)->spriteListe[i] != NULL) {
            Detruire_Sprite(&(*liste)->spriteListe[i]);
        }
    }
    free((*liste)->spriteListe);
    
    (*liste)->nbElem = 0;

    free((*liste));
    (*liste) = NULL;
}


extern pnj_liste_t * Load_Pnj(map_t* map, sprite_t**** spriteMap, liste_type_pnj_t * liste_type_pnj) {

    if (map == NULL) {
        printf("Erreur : La Map est inexistante dans Load_Pnj().\n");
        return NULL;
    }

    if (spriteMap == NULL) {
        printf("Erreur : spriteMap Inexistante dans Load_Pnj()\n");
        return NULL;
    }

    if ( liste_type_pnj == NULL ) {
        printf("Erreur : liste_type_pnj passé en paramètre invalide dans Load_Pnj()\n");
        return NULL;
    }

    if (liste_type_pnj->liste == NULL) {
        printf("Erreur : la liste de type de pnj est vide dans Load_Pnj()\n");
        return NULL;
    }

    if (liste_type_pnj->nbElem <= 0) {
        printf("Erreur : le nombre d'élément est <= 0 dans Load_Pnj()\n");
        return NULL;
    }

    // Création de la structure liste de pnj
    pnj_liste_t * liste = malloc(sizeof(pnj_liste_t));
    if (liste == NULL) {
        printf("Erreur : Echec malloc(liste) dans Load_Pnj()\n");
        return NULL;
    }

    // Création tableau de pointeur sur pnj
    liste->tabPnj = malloc(map->height * map->width * sizeof(pnj_t*));
    if (liste->tabPnj == NULL) {
        printf("Erreur : Echec malloc(liste->tabPnj) dans Load_Pnj()\n");
        return NULL;
    }
    liste->nbElem = 0;

    // Parcours de la matrice de sprites
    int id;
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            sprite_t* sprite = spriteMap[0][i][j];
            // Si le sprite n'est pas déja associé a une structure de data
            if (sprite != NULL && sprite->monstre == NULL && sprite->pnj == NULL ) {
                // Si le sprite est sous la forme d'un pnj ( 2 x 2)
                if ( i < map->height - 1 && j < map->width - 1 && spriteMap[0][i][j+1] != NULL && spriteMap[0][i+1][j] != NULL && spriteMap[0][i+1][j+1] != NULL ) 
                {   
                    // Si le id du type de sprite correspond bien a un sprite de PNJ
                    if ( sprite->spriteTypeId >= BORNE_PERSO_SPRITE &&  sprite->spriteTypeId < BORNE_PNJ_SPRITE) {
                        // calcule id pnj ( commence a BORNE_PERSO_SPRITE et 4 sprite par pnj )
                        id = (sprite->spriteTypeId-BORNE_PERSO_SPRITE) / 4;
                        // Création du nouveau pnj détecté
                        pnj_t * newPnj = creer_pnj(id,i,j,liste_type_pnj);
                        if ( newPnj == NULL ) {
                            printf("Erreur : Echec creer_pnj(%d,%d,%d,liste_type_pnj) dans Load_Pnj()\n",id,i,j);
                            return NULL;
                        }
                        // Chargement informations pnj
                        newPnj->pos_y = i; // Position en y du pnj
                        newPnj->pos_x = j; // Position en x du pnj
                        sprite->pnj = newPnj; // Liaison du pnj au sprite
                        // Ajout du pnj vers le tableau de pnj_t
                        liste->tabPnj[liste->nbElem] = newPnj;  
                        liste->nbElem++;
                        // pnj en 2 x 2 donc on relie les autres sprite du pnj au pnj crée
                        spriteMap[0][i][j+1]->pnj = newPnj;
                        spriteMap[0][i+1][j]->pnj = newPnj;
                        spriteMap[0][i+1][j+1]->pnj = newPnj;
                    }
                } 
            }
        }
    }

    return liste;
}

extern void Detruire_Liste_Pnj(pnj_liste_t** liste) {
    if (liste == NULL || *liste == NULL) {
        printf("Erreur : Liste de pnj vide dans Detruire_Liste_Pnj()\n");
        return;
    }

    if ((*liste)->tabPnj == NULL) {
        printf("Erreur : Tableau de monstres vide dans Detruire_Liste_Pnj()\n");
        return;
    }

    // Suppression de chaque monstre dans le tableau
    for (int i = 0; i < (*liste)->nbElem; i++) {
        if ((*liste)->tabPnj[i] != NULL) {
            supprimer_pnj(&((*liste)->tabPnj[i]));
            if ((*liste)->tabPnj[i] != NULL) {
                printf("Erreur : Impossible de supprimer le monstre n°%d dans Detruire_Liste_Pnj()\n", i);
            }
        }
    }

    // free tab Monstres
    if ( (*liste)->tabPnj != NULL ) {
        free((*liste)->tabPnj);
        (*liste)->tabPnj = NULL;
    }

    // clear nb elem
    (*liste)->nbElem = 0;

    // Suppression de la structure liste de monstres
    free(*liste);
    *liste = NULL;
}

/**
 * 
 * \brief Fonction externe qui charge les monstre_t
 * 
 * \param map Structure map_t où sont stockées les informations de la map
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap dans laquelle on travaille
 * \return Un pointeur sur la structure monstre_liste_t || NULL si erreur
 */
extern monstre_liste_t* Load_Monster(map_t* map, sprite_t**** spriteMap) {
    if (map == NULL) {
        printf("Erreur : La Map est inexistante dans Load_Monster().\n");
        return NULL;
    }

    if (spriteMap == NULL) {
        printf("Erreur : spriteMap Inexistante dans Load_Monster()\n");
        return NULL;
    }

    // Création liste monstre
    monstre_liste_t* liste = malloc(sizeof(monstre_liste_t));
    if (liste == NULL) {
        printf("Erreur : Echec malloc(liste) dans Load_Monster()\n");
        return NULL;
    }

    // Création tableau de pointeur sur monstres
    liste->tabMonstres = malloc(map->height * map->width * sizeof(monstre_t*));
    if (liste->tabMonstres == NULL) {
        printf("Erreur : Echec malloc(liste->tabMonstres) dans Load_Monster()\n");
        return NULL;
    }
    liste->nbElem = 0;

    // Parcours de la matrice de sprites
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            sprite_t* sprite = spriteMap[0][i][j];
            // Si le sprite est sous la forme d'un monstre ( 2 x 1 )
            if (sprite != NULL && sprite->monstre == NULL && sprite->pnj == NULL ) {
                // Nouveau monstre détecté
                char nom[20];
                sprintf(nom, "Monstre N°%d", liste->nbElem + 1);
                monstre_t* monstre = creer_monstre(nom, 1, i, j); // Création du monstre
                if ( monstre == NULL ) {
                    printf("Erreur : Echec creer_monstre(%s,%d,%d,%d) dans Load_Monster()\n",nom, 1, i, j);
                    return NULL;
                }
                // Chargement informations monstre
                monstre->pos_x = j; // Position en x du monstre
                monstre->pos_y = i; // Position en y du monstre
                sprite->monstre = monstre; // Liaison du monstre au sprite
                // Ajout du monstre vers le tableau de monstre
                liste->tabMonstres[liste->nbElem] = monstre;
                liste->nbElem++;
                // monstre en 2 x 1 donc on relie les autres sprite du monstre au monstre crée
                if (i < map->height - 1 && spriteMap[0][i+1][j] != NULL) {
                    spriteMap[0][i+1][j]->monstre = monstre;
                }
            }
        }
    }

    return liste;
}

/**
 * \fn void detruire_liste_monstres(monstre_liste_t** liste)
 * \brief Fonction externe qui permet de detuire la liste de monstres
 * 
 * \param liste Un double pointeur sur la structure monstre_liste_t à detruire
 * \return Aucun retours
*/
extern void Detruire_Liste_Monstres(monstre_liste_t** liste) {
    if (liste == NULL || *liste == NULL) {
        printf("Erreur : Liste de monstres vide dans Detruire_Liste_Monstres()\n");
        return;
    }

    if ((*liste)->tabMonstres == NULL) {
        printf("Erreur : Tableau de monstres vide dans Detruire_Liste_Monstres()\n");
        return;
    }

    // Suppression de chaque monstre dans le tableau
    for (int i = 0; i < (*liste)->nbElem; i++) {
        if ((*liste)->tabMonstres[i] != NULL) {
            supprimer_monstre(&((*liste)->tabMonstres[i]));
            if ((*liste)->tabMonstres[i] != NULL) {
                printf("Erreur : Impossible de supprimer le monstre n°%d dans Detruire_Liste_Monstres()\n", i);
            }
        }
    }

    // free tab Monstres
    if ( (*liste)->tabMonstres != NULL ) {
        free((*liste)->tabMonstres);
        (*liste)->tabMonstres = NULL;
    }

    // clear nb elem
    (*liste)->nbElem = 0;

    // Suppression de la structure liste de monstres
    free(*liste);
    *liste = NULL;
}



/** 
 * \fn 
 * \brief Fonction externe qui détecte si un ennemi est proche et renvoie un pointeur sur le monstre si trouvé
 *
 * \param spriteMap Pointeur sur la matrice de sprites.
 * \param map Pointeur sur map_t, la map à partir de laquelle charger la matrice de sprites.
 * \param y_joueur Coordonnée y du joueur sur la carte.
 * \param x_joueur Coordonnée x du joueur sur la carte.
 * \param direction Direction dans laquelle le joueur regarde.
 * \param distance Distance maximale à laquelle chercher les ennemis.
 * \param[out] monstre Pointeur sur le monstre trouvé, NULL si aucun monstre trouvé.
 * \return 1 si un monstre a été trouvé, 0 si aucun monstre n'a été trouvé, -1 en cas d'erreur.
 */
extern int Detecter_Monstre(sprite_t ****spriteMap, map_t *map, int y_joueur, int x_joueur, char direction, int distance, sprite_t **monstre) {
    // Vérification des paramètres d'entrée
    if (spriteMap == NULL) {
        printf("Erreur : spriteMap Inexistante dans detecter_monstre()\n");
        return -1;
    }
    if (map == NULL) {
        printf("Erreur : Map Inexistante dans detecter_monstre()\n");
        return -1;
    }
    if (y_joueur < 0 || x_joueur < 0 || y_joueur >= map->height || x_joueur >= map->width) {
        printf("Erreur : Coordonnées dans la spriteMap ( Emplacement Destination ) Invalide dans detecter_monstre()\n");
        return -1;
    }

    // Initialisation de la variable monstre à NULL
    *monstre = NULL;
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
            printf("Erreur : Direction Invalide dans detecter_monstre()\n");
            return -1;
    }
    for (int d = 1; d <= distance; d++) {
        int x = x_joueur + dx * d;
        int y = y_joueur + dy * d;
        if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
            // On est en dehors de la carte
            break;
        }
        if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv > 0 ) {
            // On a trouvé un ennemi
            *monstre = spriteMap[0][y][x];
            return 1;
        }
    }

    // Aucun ennemi trouvé
    *monstre = NULL;
    return 0;
}

/** 
 * \fn 
 * \brief Fonction externe qui détecte si un pnj est proche et renvoie un pointeur sur le monstre si trouvé
 *
 * \param spriteMap Pointeur sur la matrice de sprites.
 * \param map Pointeur sur map_t, la map à partir de laquelle charger la matrice de sprites.
 * \param y_joueur Coordonnée y du joueur sur la carte.
 * \param x_joueur Coordonnée x du joueur sur la carte.
 * \param direction Direction dans laquelle le joueur regarde.
 * \param distance Distance maximale à laquelle chercher les un pnj.
 * \param[out] pnj Pointeur sur le pnj trouvé, NULL si aucun pnj trouvé.
 * \return 1 si un monstre a été trouvé, 0 si aucun monstre n'a été trouvé, -1 en cas d'erreur.
 */
extern int Detecter_Pnj(sprite_t ****spriteMap, map_t *map, int y_joueur, int x_joueur, char direction, int distance, sprite_t **pnj) {
    // Vérification des paramètres d'entrée
    if (spriteMap == NULL) {
        printf("Erreur : spriteMap Inexistante dans detecter_monstre()\n");
        return -1;
    }
    if (map == NULL) {
        printf("Erreur : Map Inexistante dans detecter_monstre()\n");
        return -1;
    }
    if (y_joueur < 0 || x_joueur < 0 || y_joueur >= map->height || x_joueur >= map->width) {
        printf("Erreur : Coordonnées dans la spriteMap ( Emplacement Destination ) Invalide dans detecter_monstre()\n");
        return -1;
    }

    // Initialisation de la variable monstre à NULL
    *pnj = NULL;
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
            printf("Erreur : Direction Invalide dans detecter_monstre()\n");
            return -1;
    }
    for (int d = 1; d <= distance; d++) {
        int x = x_joueur + dx * d;
        int y = y_joueur + dy * d;
        if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
            // On est en dehors de la carte
            break;
        }
        if ( spriteMap[0][y][x] != NULL && spriteMap[0][y][x]->pnj != NULL ) {
            // On a trouvé un pnj
            *pnj = spriteMap[0][y][x];
            return 1;
        }
    }

    // Aucun ennemi trouvé
    *pnj = NULL;
    return 0;
}