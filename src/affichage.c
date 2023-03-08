#include <stdio.h>
#include <stdlib.h>

#include <affichage.h>

/**
 * \file affichage.c
 * \brief Gestion affichage
 * \author Yamis MANFALOTI
 * \version 5.0
 * \date 08 février 2023
 *
 * Gestion de l'affichage:
 * \n Initialisation en mémoire
 * \n Afficher Une IMG
 * \n Obtenir les informations relatives à la fenêtre et/ou utile à l'affichage
 * \n Gestion texture sprite
 * \n Afficher une TileMap
 * \n Afficher une SpriteMap
 * \n Gestion timer
 * \n Destruction en mémoire
 */

/**
 * 
 * \fn void Init_SDL(SDL_Window **window, SDL_Renderer **renderer)
 * \brief Fonction externe qui initialise les composants de SDL
 * 
 * \param window Pointeur de pointeur sur l'objet SDL_Window
 * \param renderer Pointeur de pointeur sur l'objet SDL_Renderer
 * \param width  Largeur de la fenêtre
 * \param height  Hauteur de la fenêtre
 * \return 0 success || 1 fail
 */
extern int Init_SDL(SDL_Window ** window, SDL_Renderer **renderer, int width, int height) {
    // Initialisation library SDL
    if ( SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0 ) {
        printf("Erreur: SDL_Init() à échoué dans Init_SDL : %s\n",SDL_GetError());
        return 1;
    }
    // Création d'une fenêtre
    if ( SDL_CreateWindowAndRenderer(width, height, 0, window, renderer) < 0 ) {
        printf("Erreur: SDL_CreateWindowAndRenderer() à échoué dans Init_SDL : %s\n",SDL_GetError());
        return 1;
    }
	// Initialisation de la library SDL_image
    if ( IMG_Init(IMG_INIT_PNG) == 0 ) {
        printf("Erreur: IMG_Init() à échoué dans Init_SDL : %s\n",IMG_GetError());
        return 1;
    }
    // Initialisation de la library SDL_ttf
    if (TTF_Init() < 0 ) {
        printf("Erreur: TTF_Init() à échoué dans Init_SDL : %s\n",TTF_GetError());
        return 1;
    }
    // return status
    return 0;
}

/**
 * \fn void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer)
 * \brief Fonction externe qui ferme les composants de SDL
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return Aucun retour effectué en fin de fonction
 */
extern void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer) {
    // Fermeture de la library SDL_image
    IMG_Quit();
    // Fermeture de la library SDL_ttf
    TTF_Quit();
    // Destruction en mémoire du Renderer
    SDL_DestroyRenderer(renderer);
    // destruction en mémoire de la Fenètre
    SDL_DestroyWindow(window);
    // Fermeture la librairy SDL_image.
    SDL_Quit();
}

/**
 * 
 * \fn void Detruire_Texture(SDL_Texture *texture)
 * \brief Fonction externe qui détruit en mémoire la texture en paramètres
 * 
 * \param texture Pointeur sur l'objet SDL_Texture
 * \return Aucun retour effectué en fin de fonction
 */
extern void Detruire_Texture(SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
}

/**
 * \fn void getWinInfo(SDL_Window *window, map_t * map, SDL_Rect * view, int * width, int * height, int * dstCoef, int * xBorder, int * yBorder)
 * \brief Fonction externe qui permet d'obtenir les informations de la fenêtre
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param tileSize Taille en pixel des tiles
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param width  Pointeur sur un int, largeur de la fenêtre
 * \param height  Pointeur sur un int, hauteur de la fenêtre
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure à gauche dans la fenêtre
 * \param yBorder Bordure en haut dans la fenêtre
 * \return Aucun retour effectué en fin de fonction
 */
extern void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder) {
    SDL_GetWindowSize(window, width, height);
    // Vérification de map et l'utilisation de dstcCoef et xBorder
    if ( tileSize != 0 ) {
        // calcule du coeficient d'affichage
        if ( dstCoef != NULL && view != NULL ) {
            (*dstCoef) = (*width) / ( (view->w) * (tileSize) ) ;
            // calcule des bordures
            if ( xBorder != NULL && yBorder != NULL ) {
                (*xBorder) = ( (*width) - ( view->w * tileSize * (*dstCoef) )  ) / 2;
                (*yBorder) = ( (*height) - ( view->h * tileSize * (*dstCoef) )  ) / 2;
            }
        }
    }
} 


/**
 * \fn void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window)
 * \brief Fonction externe qui permet de changer la résolution de la fenêtre
 * 
 * \param indiceResolution [ 1: 1280x720 | 2: 1600x900 | 3: 1920x1080 | default: 1600x900 ]
 * \param indiceFullscreen [ 0: fenêtré | 1: plein écran | 2: plein écran fenêtré | default: fenêtré ]
 * \param window Pointeur sur l'objet SDL_Window
 * \return Aucun retour effectué en fin de fonction
 */
extern void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window) {
    switch (indiceResolution) {
    case 1:
        SDL_SetWindowSize(window, 1280, 720);
        break;
    case 2:
        SDL_SetWindowSize(window, 1600, 900);
        break;
    case 3:
        SDL_SetWindowSize(window, 1920, 1080);
        break;
    default:
        SDL_SetWindowSize(window, 1600, 900);
        break;
    }
    switch (indiceFullscreen)
    {
    case 0:
        SDL_SetWindowFullscreen(window, 0);
        break;
    case 1:
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        break;
    case 2:
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
    default:
        SDL_SetWindowFullscreen(window, 0);
        break;
    }
}

/**
 * \fn void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect )
 * \brief Fonction externe qui charge et affiche une image
 * 
 * \param IMG Chemin de l'image à afficher
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param texture Pointeur de pointeur sur l'objet SDL_Texture
 * \param srcrect Pointeur sur l'objet SDL_Rect ( Rectangle Source )
 * \param dstrect Pointeur sur l'objet SDL_Rect ( Rectangle Destination )
 * \return 0 success || 1 fail
 */
extern int Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect ) {
    // Chargement d'une texture avec le Moteur de Rendu Graphique et le fichier de l'image
    if ( (*texture) == NULL ) {
        (*texture) = IMG_LoadTexture(renderer, IMG);
        if ( (*texture) == NULL ) {
            printf("Erreur: IMG_LoadTexture() à échoué dans Afficher_IMG\n");
            return 1;
        }
    }
    // Envoie de la texture vers le Moteur de Rendu Graphique
    if ( SDL_RenderCopy(renderer, (*texture), srcrect, dstrect) < 0) {
        printf("Erreur: SDL_RenderCopy() à échoué dans Afficher_IMG\n");
        return 1;
    }

    return 0;
}

/**
 * \fn Sprite_Texture_Liste_t * Init_Sprite_Texture_Liste()
 * \brief Fonction externe qui initialise la liste des textures des sprites
 * 
 * \param void Aucun paramètre
 * \return Un pointeur sur la structure Sprite_Texture_Liste_t, la liste des textures de sprite || NULL si echec
*/
extern Sprite_Texture_Liste_t * Init_Sprite_Texture_Liste() {
    // Malloc Sprite_Texture_Liste_t
    Sprite_Texture_Liste_t * sprite_texture_liste = malloc(sizeof(Sprite_Texture_Liste_t));
    if (sprite_texture_liste == NULL) {
        fprintf(stderr, "Erreur : echec malloc( sprite_texture_liste ) dans Init_Sprite_Texture_Liste.\n");
        return NULL;
    }
    // Element Tableau A NULL
    for (int i = 0; i < MAX_TEXTURE_NUMBER; i++) {
        sprite_texture_liste->tab[i] = NULL;
    }
    // Nb D'Element A 0
    sprite_texture_liste->nbElem = 0;
    // Return La Structure Est Malloc
    return sprite_texture_liste;
}

/**
 * \fn int Chercher_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet)
 * \brief Fonction externe qui cherche une texture ( char * spriteSheet ) dans la liste des textures de sprite
 * 
 * \param liste Pointeur sur Sprite_Texture_Liste_t, la liste des textures de sprite 
 * \param spriteSheet Chaine de caractères, chemin vers le spriteSheet
 * \return (int) L'indice i de la texture dans la liste || -1 Si la texture n'est pas présente
*/
extern int Chercher_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet) {
    for (int i = 0; i < liste->nbElem; i++) {
        if (strcmp(liste->tab[i]->spriteSheet, spriteSheet) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * \fn int Ajouter_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet, SDL_Texture *texture)
 * \brief Fonction externe qui ajoute une texture dans la liste des textures de sprite si elle n'est pas déja présente 
 *        ou renvoie simplement son indice dans la liste
 *
 * \param liste Pointeur sur Sprite_Texture_Liste_t, La liste des textures de sprite 
 * \param spriteSheet Chaine de caractères, chemin vers le spriteSheet
 * \param texture Pointeur sur SDL_Texture, la texture correspondante au spriteSheet
 * \return (int) L'indice i de la texture dans la liste || -1 Echec de la fonction
*/
extern int Ajouter_Texture(Sprite_Texture_Liste_t *liste, char *spriteSheet, SDL_Texture *texture) {
    // Vérification que le spriteSheet n'est pas déjà dans la liste
    int index = Chercher_Texture(liste, spriteSheet);
    if (index != -1) {
        return index;
    }

    // Si le spriteSheet n'est pas dans la liste, on ajoute la nouvelle texture
    if (liste->nbElem < MAX_TEXTURE_NUMBER) {
        // Allocation de mémoire pour la nouvelle texture
        Sprite_Texture_t *newTexture = (Sprite_Texture_t *) malloc(sizeof(Sprite_Texture_t));
        if (newTexture == NULL) {
            fprintf(stderr, "Erreur : echec malloc( Sprite_Texture_t ) dans Ajouter_Texture.\n");
            return -1;
        }

        // Initialisation de la nouvelle texture
        newTexture->spriteSheet = strdup(spriteSheet);
        if (newTexture->spriteSheet == NULL) {
            fprintf(stderr, "Erreur : echec strdup( spriteSheet ) dans Ajouter_Texture.\n");
            free(newTexture);
            return -1;
        }
        newTexture->spriteSheetTexture = texture;

        // Ajout de la nouvelle texture dans la liste
        liste->tab[liste->nbElem] = newTexture;
        liste->nbElem++;

        return liste->nbElem - 1;
    } else {
        fprintf(stderr, "Erreur : Max Sprite Texture Atteinte dans Ajouter_Texture.\n");
        return -1;
    }
}

/**
 * \fn int Load_Sprite_Texture_Liste(Sprite_Texture_Liste_t *SpriteTexteListe, sprite_type_liste_t * listeType, SDL_Renderer *renderer )
 * \brief Fonction externe qui charge les texture dans SpriteTexteListe depuis listeType
 * 
 * \param SpriteTexteListe Pointeur sur Sprite_Texture_Liste_t, la liste des textures de sprite
 * \param listeType Pointeur sur sprite_type_liste_t, la listes des types de sprite
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return 0 success || 1 Echec de la fonction
*/
extern int Load_Sprite_Texture_Liste(Sprite_Texture_Liste_t *SpriteTexteListe, sprite_type_liste_t * listeType, SDL_Renderer *renderer ) {
    if ( SpriteTexteListe == NULL || listeType == NULL ) {
        fprintf(stderr, "Erreur : SpriteTexteListe Ou listeType Inexistante Dans Load_Sprite_Texture_Liste().\n");
        return 1;
    }

    int id;
    for (int i = 0; i < listeType->nbElem; i++) {
        id = Ajouter_Texture(SpriteTexteListe,listeType->typeListe[i]->spriteSheet,IMG_LoadTexture(renderer,listeType->typeListe[i]->spriteSheet));
        if (  id < 0 ) {
            fprintf(stderr, "Erreur : Echec Ajouter_Texture() dans Load_Sprite_Texture_Liste().\n");
            return 1;
        }
        listeType->typeListe[i]->textureId = id;
    }

    return 0;
}

/**
 * \fn void Detruire_Sprite_Texture_Liste(Sprite_Texture_Liste_t *liste)
 * \brief Fonction externe qui supprime la liste des textures de sprite
 * 
 * \param liste Pointeur sur Sprite_Texture_Liste_t, la liste des textures de sprite à supprimer
 * \return Aucun retour effectué en fin de fonction
*/
extern void Detruire_Sprite_Texture_Liste(Sprite_Texture_Liste_t **liste) {
    if ((*liste) != NULL) {
        // Parcours de la liste
        for (int i = 0; i < (*liste)->nbElem; i++) {
            // Libération de la mémoire allouée pour chaque texture
            free((*liste)->tab[i]->spriteSheet);
            SDL_DestroyTexture((*liste)->tab[i]->spriteSheetTexture);
            free((*liste)->tab[i]);
        }
        // Libération de la mémoire allouée pour la liste
        free((*liste));
        (*liste) = NULL;
    }
}


/**
 * \fn int Afficher_Map(SDL_Texture * texture, map_t * map, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder )
 * \brief Fonction externe qui affiche une map composée des tiles d'un tileSet
 * 
 * \param texture Texture du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure à gauche dans la fenêtre
 * \param yBorder Bordure en haut dans la fenêtre
 * \return 0 success || 1 fail
 */
extern int Afficher_TileMap(SDL_Texture * texture, map_t * map, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder ) {


    // Verification paramètres
    if ( texture == NULL ) {
       printf("Erreur: La texture n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( map == NULL ) {
       printf("Erreur: La Map n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( view == NULL ) {
       printf("Erreur: La View n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur: Le Renderer n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur: Le WinInfo Incorrecte dans afficher_map()\n");
        return 1;
    }

    // Initialisation variables
    int ymin, ymax;
    int xmin, xmax;

    // calcule de xmin et ymax tout en verifiant/corrigeant les sorties de map
    if ( view->y < 0 ) {
        ymin = view->y = 0;
        ymax = view->y + view->h;
    }
    else if (view->y + view->h >= map->height ) {
        view->y = ymin = map->height - view->h ;
        ymax = view->y + view->h;
    }
    else {
        ymin = view->y;
        ymax = view->y + view->h;
    }

    if ( view->x < 0 ) {
        xmin = view->x = 0;
        xmax = view->x + view->w;
    }
    else if ( view->x + view->w >= map->width ) {
        view->x = xmin = map->width - view->w;
        xmax = view->x + view->w;
    }
    else {
        xmin = view->x;
        xmax = view->x + view->w;
    }

    // La Map doit avoir 3 layer de tuile ( meme si les layers sont vide )
    if ( map->layer < 5) {
        printf("Erreur : Le format de la Map est incorrecte .\n");
        return 1;
    }
    int layer = 3;

    // Affichage des tiles de la carte
    for (int n = 0; n < layer; n++ ) {
        for (int y = ymin; y < ymax; y++) {
            for (int x = xmin; x < xmax; x++) {
                int tileNumber = map->matrice[n][y][x];
                if ( tileNumber-1 >= -1 ) {
                    // Rectangle Source ( TileSet )
                    SDL_Rect srcrect;
                    srcrect.y = map->tileSize * ( (tileNumber-1) / 120);
                    srcrect.x = map->tileSize * ( (tileNumber-1) % 120);
                    srcrect.h = map->tileSize;
                    srcrect.w = map->tileSize;

                    // Rectangle Destination ( Renderer )
                    SDL_Rect dstrect;
                    dstrect.x = ( dstCoef * (map->tileSize * (x - view->x)) ) + xBorder;
                    dstrect.y = ( dstCoef * (map->tileSize * (y - view->y)) ) + yBorder; 
                    dstrect.h = dstCoef * map->tileSize;
                    dstrect.w = dstCoef * map->tileSize;

                    // Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer
                    if ( SDL_RenderCopy(renderer, texture, &srcrect, &dstrect) < 0 ) {
                        printf("Erreur: SDL_RenderCopy() à échoué dans afficher_map\n");
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * \fn int Afficher_SpriteMap(Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
 * \brief Fonction externe qui affiche les sprites de la spriteMap qui correspondent à la view sur le renderer.
 * 
 * \param SpriteTextureListe
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param renderer Pointeur sur l'objet SDL_Renderer.
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \return 0 success || 1 fail
 */
extern int Afficher_SpriteMap(Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Verification paramètres
    if ( SpriteTextureListe == NULL ) {
       printf("Erreur: La SpriteTextureListe n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }

    if ( spriteMap == NULL ) {
       printf("Erreur: La spriteMap n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }

    if ( map == NULL ) {
       printf("Erreur: La Map n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }

    if ( listeType == NULL ) {
       printf("Erreur: La listeType n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }
    
    if ( view == NULL ) {
       printf("Erreur: La View n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur: Le Renderer n'est pas chargé dans Afficher_SpriteMap()\n");
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur: WinInfo Incorrecte dans Afficher_SpriteMap()\n");
        return 1;
    }
    
    // initialisation variable
    int ymin, ymax;
    int xmin, xmax;

    // calcule de xmin et ymax tout en verifiant/corrigeant les sorties de map
    if ( view->y < 0 ) {
        ymin = view->y = 0;
        ymax = view->y + view->h;
    }
    else if (view->y + view->h >= map->height ) {
        view->y = ymin = map->height - view->h ;
        ymax = view->y + view->h;
    }
    else {
        ymin = view->y;
        ymax = view->y + view->h;
    }

    if ( view->x < 0 ) {
        xmin = view->x = 0;
        xmax = view->x + view->w;
    }
    else if ( view->x + view->w >= map->width ) {
        view->x = xmin = map->width - view->w;
        xmax = view->x + view->w;
    }
    else {
        xmin = view->x;
        xmax = view->x + view->w;
    }

    // Affichage des sprites de la spriteMap qui correspond a la view
    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            if ( spriteMap[y][x] != NULL ) {
                // Recupération des informations lié au sprite ( pour la lisibilité du code )
                sprite_t * sprite = spriteMap[y][x];
                sprite_type_t * spriteType = listeType->typeListe[sprite->spriteTypeId];
                int spriteSize = spriteType->spriteSize;
                int frameNumber = spriteType->frameNumber;
                int spriteLine = spriteType->spriteLine;

                // Recupération de la texture du SpriteSheet
                SDL_Texture * texture = SpriteTextureListe->tab[spriteType->textureId]->spriteSheetTexture;
            
                // Rectangle Source ( SpriteSheet )
                SDL_Rect rectSrc;
                rectSrc.x = spriteSize * ( sprite->frame = sprite->frame % frameNumber);
                rectSrc.y = spriteSize * spriteLine;
                rectSrc.w = spriteSize;
                rectSrc.h = spriteSize;

                // Rectangle Destination ( Renderer )
                SDL_Rect rectDst;
                rectDst.x = ( dstCoef * (spriteSize * (sprite->x - view->x)) ) + xBorder;
                rectDst.y = ( dstCoef * (spriteSize * (sprite->y - view->y)) ) + yBorder; 
                rectDst.h = dstCoef * spriteSize;
                rectDst.w = dstCoef * spriteSize;

                // Affichage de la frame courante du sprite
                if ( SDL_RenderCopy(renderer, texture, &rectSrc, &rectDst) < 0 ) {
                    printf("Erreur: SDL_RenderCopy() à échoué dans Afficher_SpriteMap()\n");
                    return 1;
                }
            }
        }
    }

    return 0;
}

/**
 * \fn void Affichage_All(char * tileSet, map_t * map, sprite_t *** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view)
 * \brief Fonction externe qui affiche tout les ellements graphiques
 * 
 * \param texture Texture du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap à afficher
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return 0 success || 1 fail
 */
extern int Affichage_All(SDL_Texture * texture, map_t * map, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t *** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view) {
    // Initialisation des variables
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, map->tileSize, view, &dstCoef, &xBorder, &yBorder);

    // Affciher la Map
    if ( Afficher_TileMap(texture, map, view, renderer,dstCoef, xBorder, yBorder ) ) {
        printf("Errur: Afficher_map() à echoué dans Affichage_All().\n");
        return 1;
    }

    
    // Affciher la Map
    if ( Afficher_SpriteMap(SpriteTextureListe, spriteMap, map, listeType, view, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Errur: Afficher_SpriteMap() à echoué dans Affichage_All().\n");
        return 1;
    }
    

    return 0;
}

/**
 * \fn void AddFrame(sprite_t *** spriteMap, map_t * map, SDL_Rect * view)
 * \brief Fonction externe qui incrémente la frame des sprites présents dans la spriteMap qui correspondent à la view.
 * 
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap à afficher
 * \param FrameCat (int) La catégorie des sprites dont la frame sera augmenté
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return Aucun retour effectué en fin de fonction
*/
extern void AddFrame(sprite_t *** spriteMap, int FrameCat, sprite_type_liste_t * listeType, map_t * map, SDL_Rect * view) {
    // initialisation variable
    int ymin, ymax;
    int xmin, xmax;
    int frame_cat;

    // calcule de xmin et ymax tout en verifiant/corrigeant les sorties de map
    if ( view->y < 0 ) {
        ymin = view->y = 0;
        ymax = view->y + view->h;
    }
    else if (view->y + view->h >= map->height ) {
        view->y = ymin = map->height - view->h ;
        ymax = view->y + view->h;
    }
    else {
        ymin = view->y;
        ymax = view->y + view->h;
    }

    if ( view->x < 0 ) {
        xmin = view->x = 0;
        xmax = view->x + view->w;
    }
    else if ( view->x + view->w >= map->width ) {
        view->x = xmin = map->width - view->w;
        xmax = view->x + view->w;
    }
    else {
        xmin = view->x;
        xmax = view->x + view->w;
    }

    // Parcourt la spriteMap qui correspond a la view, et augmente la frame des sprite présent qui correspondent à la categorie donnée
    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            if ( spriteMap[y][x] != NULL ) {
                frame_cat = listeType->typeListe[(spriteMap[y][x]->spriteTypeId)]->frameCat;
                if ( frame_cat == FrameCat) {
                    (spriteMap[y][x])->frame += 1;
                }
            }
        }
    }
}

/**
 * \fn void Timer_Start( SDL_timer_t * timer )
 * \brief Initialise SDL_timer_t->start au temps courant
 * 
 * \fn timer Pointeur sur SDL_timer_t, le timer à lancer
 * \return Aucun retour effectué en fin de fonction
*/
extern void Timer_Start( SDL_timer_t * timer ) {
    timer->start = SDL_GetTicks();
}

/**
 * \fn void Timer_Update( SDL_timer_t * timer )
 * \brief Initialise SDL_timer_t->now au temps courant
 * 
 * \fn timer Pointeur sur SDL_timer_t, le timer à update
 * \return Aucun retour effectué en fin de fonction
*/
extern void Timer_Update( SDL_timer_t * timer ) {
    timer->now = SDL_GetTicks();
}

/**
 * \fn Uint32 Timer_Get_Time( SDL_timer_t * timer )
 * \brief Calcule le temps ecouler depuis le debut du timer
 * 
 * \fn timer Pointeur sur SDL_timer_t, le timer dont on doit calculer le temps
 * \return type Uint32 , Temps ecoulé debuit le debut du timer
*/
extern Uint32 Timer_Get_Time( SDL_timer_t * timer ) {
    Timer_Update(timer);
    return ( timer->now - timer->start );
}