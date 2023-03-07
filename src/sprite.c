#include <stdio.h>
#include <stdlib.h>

#include <sprite.h>


/**
 * \file sprite.c
 * \brief Gestion des sprites
 * \author Yamis MANFALOTI
 * \version 1.5
 * \date 07 mars 2023
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
        printf("Erreur : lors de l'ouverture du fichier\n");
        return 1;
    }

    // Comptage du nombre de lignes du fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nb_elements++;
    }

    // Allocation de la liste des types de sprites
    liste->typeListe = (sprite_type_t**) malloc(nb_elements * sizeof(sprite_type_t*));
    if (liste->typeListe == NULL) {
        printf("Erreur : d'allocation mémoire\n");
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
            printf("Erreur : d'allocation mémoire\n");
            fclose(fichier);
            return 1;
        }

        token = strtok(ligne, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite);
            return 1;
        }
        
        sprite->spriteName = malloc((strlen(token)+1) * sizeof(char));
        if (sprite->spriteName == NULL) {
            printf("Erreur : d'allocation mémoire\n");
            fclose(fichier);
            free(sprite);
            return 1;
        }
        strcpy(sprite->spriteName, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite);
            return 1;
        }
        sprite->frameNumber = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite);
            return 1;
        }

        sprite->spriteSheet = malloc((strlen(token)+1) * sizeof(char));
        if (sprite->spriteSheet == NULL) {
            printf("Erreur : d'allocation mémoire\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        strcpy(sprite->spriteSheet, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->spriteSize = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->spriteLine = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Erreur : de format dans le fichier\n");
            fclose(fichier);
            free(sprite->spriteName);
            free(sprite->spriteSheet);
            free(sprite);
            return 1;
        }
        sprite->frameCat = atoi(token);

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
    // si la structure liste existe
    if ((*liste) != NULL) {
        // si la liste des types existe
        if ((*liste)->typeListe != NULL) {
            // parcourt de la liste des types
            for (int i = 0; i < (*liste)->nbElem; i++) {
                sprite_type_t *sprite = (*liste)->typeListe[i];
                // si l'element de la liste des types existe
                if (sprite != NULL) {
                    free(sprite->spriteName);
                    free(sprite->spriteSheet);
                    free(sprite);
                }
            }
            // free de la liste des types
            free((*liste)->typeListe);
            (*liste)->typeListe = NULL;
        }
        // reset de nbElem
        (*liste)->nbElem = 0;
        // free de la liste
        free((*liste));
        (*liste) = NULL;
    }
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
        printf("Erreur : d'allocation mémoire\n");
        return(NULL);
    }

    if (Charger_Sprite_Type(nom_fichier, liste) != 0) {
        printf("Erreur : lors du chargement des sprites\n");
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
        printf("x Invalide\n");
        free(sprite);
        return NULL;
    }
    if ( y >= 0 && y < map->height ) {
        sprite->y = y;
    }
    else {
        printf("y Invalide\n");
        free(sprite);
        return NULL;
    }

    //Initialisation de la variables de frame courante
    if ( frame >= 0 && frame < MAX_SPRITE_FRAME ) {
        sprite->frame = frame;
    }
    else {
        printf("frame Invalide\n");
        free(sprite);
        return NULL;
    }

    //Initialisation de la variable de type du sprite
    if ( spriteTypeId >= 0 && spriteTypeId < liste->nbElem ) {
        sprite->spriteTypeId = spriteTypeId;
    }
    else {
        printf("spriteTypeId Invalide\n");
        free(sprite);
        return NULL;
    }

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
extern void Detruire_SpriteMap(sprite_t **** spriteMap, map_t * map) {
    // si le pointeur en paramètre est correcte
    if (spriteMap != NULL ) {
        // si la spriteMap existe
        if ( (*spriteMap) != NULL ) {
            // parcourt de la spriteMap
            for (int y = 0; y < map->height; y++) {
                for (int x = 0; x < map->width; x++) {
                    // destruction des sprites stockés dans la spriteMap
                    if ( (*spriteMap)[y][x] != NULL ) {
                        Detruire_Sprite(  &((*spriteMap)[y][x]) );
                        (*spriteMap)[y][x] = NULL;
                    }
                }
                // destruction des lignes de la matrice spriteMap
                free((*spriteMap)[y]);
                (*spriteMap)[y] = NULL;
            }
            // destruction de la matrice spriteMap
            free((*spriteMap));
            (*spriteMap) = NULL;
        }
        else {
            printf("Erreur : La spriteMap n'existe pas\n");
            return;
        }
    }
    else {
        printf("Erreur : Pointeur Passé En Paramètre Invalide\n");
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
extern sprite_t *** Load_SpriteMap(sprite_type_liste_t *listeType, map_t * map) {
    // Initialisation matrice spriteMap
    sprite_t ***spriteMap = (sprite_t ***) malloc(map->height * sizeof(sprite_t **));
    if ( spriteMap != NULL ) {
        for (int i = 0; i < map->height; i++) {
            spriteMap[i] = (sprite_t **) malloc(map->width * sizeof(sprite_t *));
            if ( spriteMap[i] == NULL ) {
                printf("Erreur : Echec Malloc sprite_t ***spriteMap[%d]\n",i);
                Detruire_SpriteMap(&spriteMap,map);
                return NULL;
            }
        }
    }
    else {
        printf("Erreur : Echec Malloc sprite_t ***spriteMap\n");
        return NULL;
    }

    // Chargement matrice spriteMap
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (map->matrice[LAYER_SPRITE_MAP][y][x] >= 0) {
                sprite_t * sprite = Load_Sprite(x, y, 0, map->matrice[LAYER_SPRITE_MAP][y][x], listeType, map);
                if (sprite != NULL) {
                    spriteMap[y][x] = sprite;
                } else {
                    printf("Erreur : Echec Load_Sprite matrice[4][%d][%d]\n",y,x);
                    Detruire_SpriteMap(&spriteMap,map);
                    return NULL;
                }
            }
            else {
                spriteMap[y][x] = NULL;
            }
        }
    }

    return spriteMap;
}