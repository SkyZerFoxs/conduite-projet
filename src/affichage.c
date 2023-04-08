#include <stdio.h>
#include <stdlib.h>

#include <affichage.h>

/**
 * \file affichage.c
 * \brief Gestion affichage
 * \author Yamis MANFALOTI
 * \version 7.0
 * \date 05 avril 2023
 *
 * Gestion de l'affichage:
 * \n Initialisation en mémoire
 * \n Afficher Des IMG
 * \n Obtenir les informations relatives à la fenêtre et/ou utile à l'affichage
 * \n Gestion texture sprite
 * \n Afficher une TileMap
 * \n Afficher une SpriteMap
 * \n Gestion timer
 * \n Destruction en mémoire
 * \n Autres fonction d'affichage ...
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
 * \return 0 success || 1 fail ( statut fonction )
 */
extern int Init_SDL(SDL_Window ** window, SDL_Renderer **renderer, int width, int height) {
    // Initialisation library SDL
    if ( SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0 ) {
        printf("Erreur : SDL_Init() à échoué dans Init_SDL : %s\n",SDL_GetError());
        return 1;
    }

    // Création d'une fenêtre
    *window = SDL_CreateWindow("Ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Erreur : SDL_CreateWindow() à échoué dans Init_SDL : %s\n",SDL_GetError());
        return 1;
    }
    // Création d'un renderer | SDL_RENDERER_PRESENTVSYNC
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Erreur : SDL_CreateRenderer() à échoué dans Init_SDL : %s\n",SDL_GetError());
        return 1;
    }

	// Initialisation de la library SDL_image
    if ( IMG_Init(IMG_INIT_PNG) == 0 ) {
        printf("Erreur : IMG_Init() à échoué dans Init_SDL : %s\n",IMG_GetError());
        return 1;
    }
    // Initialisation de la library SDL_ttf
    if (TTF_Init() < 0 ) {
        printf("Erreur : TTF_Init() à échoué dans Init_SDL : %s\n",TTF_GetError());
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
void Quit_SDL(SDL_Window* window, SDL_Renderer* renderer) {
    // On quitte les libraries SDL_image et SDL_ttf
    IMG_Quit();
    TTF_Quit();

    // On détruit le renderer
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    // On détruit la fenêtre
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    // On quitte la library SDL
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
extern void Detruire_Texture(SDL_Texture ** texture) {
    if ( texture == NULL || *texture == NULL ) {
        printf("Erreur : Texture vide dans Detruire_Texture()\n");
        return;
    }

    SDL_DestroyTexture((*texture));
    (*texture) = NULL;
}

/**
 * \fn void getWinInfo(SDL_Window *window, map_t * map, SDL_Rect * view, int * width, int * height, int * dstCoef, int * xBorder, int * yBorder)
 * \brief Fonction externe qui permet d'obtenir les informations de la fenêtre
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param width  Pointeur sur un int, largeur de la fenêtre
 * \param height  Pointeur sur un int, hauteur de la fenêtre
 * \param tileSize Taille en pixel des tiles
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
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
 * \fn void changeResolution(int coef1920, int indiceFullscreen, SDL_Window *window)
 * \brief Fonction externe qui permet de changer la résolution de la fenêtre
 * 
 * \param coef1920 [ 1: 1280x720 | 2: 1600x900 | 3: 1920x1080 | default: 1600x900 ]
 * \param indiceFullscreen [ 0: fenêtré | 1: plein écran | 2: plein écran fenêtré | default: fenêtré ]
 * \param window Pointeur sur l'objet SDL_Window
 * \return Aucun retour effectué en fin de fonction
 */
extern void changeResolution(int coef1920, int indiceFullscreen, SDL_Window *window) {
    switch (coef1920) {
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
        break;
    }
}

/**
 * \fn Sprite_Texture_Liste_t * Init_Sprite_Texture_Liste()
 * \brief Fonction externe qui initialise la liste des textures des sprites
 * 
 * \param void Aucun paramètre
 * \return Un pointeur sur la structure Sprite_Texture_Liste_t, la liste des textures de sprite || NULL si echec ( statut fonction )
*/
extern Sprite_Texture_Liste_t * Init_Sprite_Texture_Liste() {
    // Malloc Sprite_Texture_Liste_t
    Sprite_Texture_Liste_t * sprite_texture_liste = malloc(sizeof(Sprite_Texture_Liste_t));
    if (sprite_texture_liste == NULL) {
        printf("Erreur : echec malloc( sprite_texture_liste ) dans Init_Sprite_Texture_Liste.\n");
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
 * \return (int) L'indice i de la texture dans la liste || -1 Echec de la fonction ( statut fonction )
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
           printf("Erreur : echec malloc( Sprite_Texture_t ) dans Ajouter_Texture()\n");
            return -1;
        }

        // Initialisation de la nouvelle texture
        newTexture->spriteSheet = (char*) malloc(strlen(spriteSheet) + 1); // +1 pour le caractère null
        if (newTexture->spriteSheet == NULL) {
            printf("Erreur : echec malloc( newTexture->spriteSheet ) dans Ajouter_Texture()\n");
            free(newTexture);
            return -1;
        }
        strcpy(newTexture->spriteSheet, spriteSheet);

        newTexture->spriteSheetTexture = texture;

        // Ajout de la nouvelle texture dans la liste
        liste->tab[liste->nbElem] = newTexture;
        liste->nbElem++;

        return liste->nbElem - 1;
    } else {
        printf("Erreur : Max Sprite Texture Atteinte dans Ajouter_Texture()\n");
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
 * \return 0 success || 1 Echec de la fonction ( statut fonction )
*/
extern int Load_Sprite_Texture_Liste(Sprite_Texture_Liste_t *SpriteTexteListe, sprite_type_liste_t * listeType, SDL_Renderer *renderer ) {
    if ( SpriteTexteListe == NULL || listeType == NULL ) {
        printf("Erreur : SpriteTexteListe Ou listeType Inexistante Dans Load_Sprite_Texture_Liste().\n");
        return 1;
    }

    if ( renderer == NULL ) {
        printf("Erreur : renderer Invalide Dans Load_Sprite_Texture_Liste().\n");
        return 1;
    }

    int id;
    SDL_Texture * newText = NULL;
    for (int i = 0; i < listeType->nbElem; i++) {
        newText = IMG_LoadTexture(renderer,listeType->typeListe[i]->spriteSheet);
        if (  newText == NULL ) {
            printf("Erreur : Echec IMG_LoadTexture(newText) dans Load_Sprite_Texture_Liste().\n");
            return 1;
        }
        id = Ajouter_Texture(SpriteTexteListe,listeType->typeListe[i]->spriteSheet,newText);
        if (  id < 0 ) {
           printf("Erreur : Echec Ajouter_Texture() dans Load_Sprite_Texture_Liste().\n");
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
    if ( liste == NULL || (*liste) == NULL ) {
        printf("Erreur : Liste des textures vide dans Detruire_Sprite_Texture_Liste()\n");
        return;
    }

    if ((*liste)->tab == NULL) {
        printf("Erreur : Liste vide dans Detruire_Sprite_Texture_Liste()\n");
        return;
    }

    if ( (*liste)->nbElem <= 0 ) {
        printf("Erreur : Nombre element <= 0 dans Detruire_Sprite_Texture_Liste()\n");
        return;
    }
 
    // Parcours de la liste
    for (int i = 0; i < (*liste)->nbElem; i++) {
        if ( (*liste)->tab[i] != NULL ) {
            // Libération de la mémoire allouée pour chaque texture
            if ( (*liste)->tab[i]->spriteSheet != NULL ) {
                free((*liste)->tab[i]->spriteSheet);
                (*liste)->tab[i]->spriteSheet = NULL;
            }
            if ( (*liste)->tab[i]->spriteSheetTexture != NULL ) {
                Detruire_Texture(&((*liste)->tab[i]->spriteSheetTexture));
                (*liste)->tab[i]->spriteSheetTexture = NULL;
            }
            free((*liste)->tab[i]);
            (*liste)->tab[i] = NULL;
        }
        
    }

    // Libération de la mémoire allouée pour la liste
    free((*liste));
    (*liste) = NULL;
    
}


/**
 * \fn int Afficher_Map(SDL_Texture * texture, map_t * map, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder )
 * \brief Fonction externe qui affiche une map composée des tiles d'un tileSet
 * 
 * \param texture Texture du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param minLayer Premier layer de la tileMap à afficher
 * \param maxLayer Dernier layer de la tileMap à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure à gauche dans la fenêtre
 * \param yBorder Bordure en haut dans la fenêtre
 * \return 0 success || 1 fail ( statut fonction )
 */
static int Afficher_TileMap(SDL_Texture * texture, map_t * map, int minLayer, int maxLayer, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder ) {


    // Verification paramètres
    if ( texture == NULL ) {
       printf("Erreur : La texture n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( map == NULL ) {
       printf("Erreur : La Map n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( view == NULL ) {
       printf("Erreur : La View n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans afficher_map()\n");
       return 1;
    }

    if ( dstCoef == 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : Le WinInfo Incorrecte dans afficher_map()\n");
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

    // Affichage des tiles de la carte
    for (int n = minLayer; n < maxLayer; n++ ) {
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
                        printf("Erreur : SDL_RenderCopy() à échoué dans afficher_map\n");
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * \fn int Afficher_SpriteMap(Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t **** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
 * \brief Fonction externe qui affiche les sprites de la spriteMap qui correspondent à la view sur le renderer.
 * 
 * \param SpriteTextureListe Liste des textures des sprites
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param layer Layer dans la spriteMap a afficher
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param renderer Pointeur sur l'objet SDL_Renderer.
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \return 0 success || 1 fail ( statut fonction )
 */
static int Afficher_SpriteMap(Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t **** spriteMap, int layer, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Verification paramètres
    if ( SpriteTextureListe == NULL ) {
       printf("Erreur : La SpriteTextureListe n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }

    if ( spriteMap[layer] == NULL ) {
       printf("Erreur : La spriteMap[layer] n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }

    if ( map == NULL ) {
       printf("Erreur : La Map n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }

    if ( listeType == NULL ) {
       printf("Erreur : La listeType n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }
    
    if ( view == NULL ) {
       printf("Erreur : La View n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_spriteMap[layer = %d]()\n",layer);
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : WinInfo Incorrecte dans Afficher_spriteMap[layer = %d]()\n",layer);
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

    // Affichage des sprites de la spriteMap[layer] qui correspond a la view
    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            if ( spriteMap[layer][y][x] != NULL ) {
                if (spriteMap[layer][y][x]->monstre != NULL && spriteMap[layer][y][x]->monstre->caract->pv <= 0) {
                    continue;
                }
                else {
                    // Recupération des informations lié au sprite ( pour la lisibilité du code )
                    sprite_t * sprite = spriteMap[layer][y][x];
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
                        printf("Erreur : SDL_RenderCopy() à échoué dans Afficher_spriteMap[layer = %d]()\n",layer);
                        return 1;
                    }

                    if ( sprite != NULL && sprite->monstre != NULL ) {
                        sprite_t * sprite2 = spriteMap[0][y+1][x];
                        if ( sprite2 != NULL && sprite2->monstre == sprite->monstre ) 
                        {
                            sprite2->frame = sprite->frame;
                        }
                        
                    }
                    if (    (y + 1) < map->height && (x + 1) < map->width      
                            && spriteMap[layer][y    ][x    ] != NULL && spriteMap[layer][y    ][x    ]->pnj != NULL && spriteMap[layer][y    ][x    ]->pnj == sprite->pnj
                            && spriteMap[layer][y + 1][x    ] != NULL && spriteMap[layer][y + 1][x    ]->pnj != NULL && spriteMap[layer][y    ][x    ]->pnj == sprite->pnj
                            && spriteMap[layer][y    ][x + 1] != NULL && spriteMap[layer][y    ][x + 1]->pnj != NULL && spriteMap[layer][y    ][x    ]->pnj == sprite->pnj
                            && spriteMap[layer][y + 1][x + 1] != NULL && spriteMap[layer][y + 1][x + 1]->pnj != NULL && spriteMap[layer][y    ][x    ]->pnj == sprite->pnj
                        ) 
                    {   
                        spriteMap[layer][y + 1][x    ]->frame = sprite->frame;
                        spriteMap[layer][y    ][x + 1]->frame = sprite->frame;
                        spriteMap[layer][y + 1][x + 1]->frame = sprite->frame;
                    }

                }
            }
        }
    }

    return 0;
}

/**
 * \fn int Afficher_Stats_Monstre(sprite_t **** spriteMap, map_t * map, SDL_Rect * view, TTF_Font * font, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
 * \brief Fonction externe qui affiche les statistiques des monstres.
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param font Police d'écriture pour afficher le texte 
 * \param renderer Pointeur sur l'objet SDL_Renderer.
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \return 0 success || 1 fail ( statut fonction )
 */
static int  Afficher_Stats_Monstre(sprite_t **** spriteMap, map_t * map, SDL_Rect * view, TTF_Font * font, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    if ( spriteMap[0] == NULL ) {
       printf("Erreur : La spriteMap[layer] n'est pas chargé dans Afficher_Stats_Monstre()\n");
       return 1;
    }

    if ( map == NULL ) {
       printf("Erreur : La Map n'est pas chargé dans Afficher_Stats_Monstre()\n");
       return 1;
    }
    
    if ( view == NULL ) {
       printf("Erreur : La View n'est pas chargé dans Afficher_Stats_Monstre()\n");
       return 1;
    }

    if ( font == NULL ) {
       printf("Erreur : La font n'est pas chargé dans Afficher_Stats_Monstre()\n");
       return 1;
    }

    if ( renderer == NULL ) {
       printf("Erreur : Le Renderer n'est pas chargé dans Afficher_Stats_Monstre()\n");
       return 1;
    }

    if ( dstCoef < 0 || xBorder < 0 || yBorder < 0) {
        printf("Erreur : WinInfo Incorrecte dans Afficher_Stats_Monstre()\n");
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

    SDL_Color gris = { 128, 128, 128, 255 };
    int palierY = ( dstCoef * 16 * 0.7 ) / 2;
    char string[256];

    // Lecture des sprites de la spriteMap[0] qui correspond a la view
    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            if ( spriteMap[0][y][x] != NULL ) {
                if (spriteMap[0][y][x]->monstre != NULL && spriteMap[0][y][x]->monstre->caract->pv <= 0) {
                    continue;
                }
                else {
                    // Recupération des informations lié au sprite ( pour la lisibilité du code )
                    sprite_t * sprite = spriteMap[0][y][x];

                    if ( sprite->monstre != NULL && y == sprite->monstre->pos_y && x == sprite->monstre->pos_x ) {
                        // Affichage des stats du monstre
                            for (int n = 0; n < 2; n++) {
                                if ( n == 0 ) {
                                    sprintf(string,"%-3s:%-4d","niv",sprite->monstre->niveau);
                                }
                                if ( n == 1 ) {
                                    sprintf(string,"%-3s:%-4d","vie",sprite->monstre->caract->pv);
                                }
                                if ( Afficher_Texte_Zone(renderer, font, string, 
                                ( dstCoef * (16 * (sprite->y - view->y - 0.8)) ) + yBorder + ( palierY * n ), 
                                ( dstCoef * (16 * (sprite->x - view->x - 0.10)) ) + xBorder, 
                                dstCoef * (16 * 2), &gris) ) {
                                    printf("Erreur : Echec Afficher_Texte_Zone() dans Afficher_Stats_Monstre()\n");
                                    return 1;
                                }
                            }
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * \fn int Afficher_Skill_Bar(personnage_t * perso, int tabSkill[3], SDL_Texture * textSkillBar[4], TTF_Font * font, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
 * \brief Fonction externe qui affiche l'interface de la barre de skill.
 * 
 * \param perso pointeur sur le personnage_t
 * \param tabSkill tableau de 3 entier qui correspondent a l'état des attaques ( disponible / indisponible )
 * \param textSkillBar tableau de 4 textire qui contient l'ensemble des textures nécessaire pour afficher la SkillBar
 * \param font Police d'écriture pour afficher le texte 
 * \param renderer Pointeur sur l'objet SDL_Renderer.
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \param win_width Largeur de la fenêtre.
 * \return 0 success || 1 fail ( statut fonction )
 */
static int Afficher_Skill_Bar(personnage_t * perso, int tabSkill[3], SDL_Texture * textSkillBar[4], TTF_Font * font, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder, int win_width) {
    if ( perso == NULL ) {
        printf("Erreur : perso en parametre invalide dans Afficher_Skill_Bar()\n");
        return 1;
    }

    if ( font == NULL ) {
        printf("Erreur : font en parametre invalide dans Afficher_Skill_Bar()\n");
        return 1;
    }

    if ( renderer == NULL ) {
        printf("Erreur : renderer en parametre invalide dans Afficher_Skill_Bar()\n");
        return 1;
    }
    
    // Rect Destination ( renderer )
    SDL_Rect dest;
    dest.h = dstCoef * 16 * 11;
    dest.w = dstCoef * 16 * 20;
    dest.x = xBorder;
    dest.y = yBorder;

    // Affichage HUD Skill Bar
    SDL_RenderCopy(renderer, textSkillBar[0], NULL, &dest);

    // AFfichage Locked Skill
    for (int i = 0; i < 3; i++) {
        if ( tabSkill[i] == 1 ) {
            SDL_RenderCopy(renderer, textSkillBar[i+1], NULL, &dest);
        }
    }

    // Afichage pv & maxPV
    char string[256];
    SDL_Color blanc = {253, 56, 56, 255};

    caract_t caractSortie = { 0, 0, 0, 0 };
    calculer_stats_perso(perso,&caractSortie);
    int pv = perso->caract->pv;
    int maxPv = caractSortie.maxPv;

    // Calcule indice resolution 
    int coef1280, coef1920;
    if ( win_width > 1200 && win_width < 1300) {
        coef1920 = 1;
        coef1280 = 0;
    }
    else if ( win_width > 1500 && win_width < 1700) {
        coef1920 = 0;
        coef1280 = 1;
    }
    else if ( win_width > 1900 && win_width < 2000) {
        coef1920 = 1;
        coef1280 = 1;
    }
    else {
        printf("Erreur : win_width invalide dans Afficher_Skill_Bar()\n");
        return 1;
    }
    
    if ( pv / 1000 > 0 || maxPv / 1000 > 0 ) {
        sprintf(string,"%-4d",pv);
        Afficher_Texte_Zone(renderer, font, string, dstCoef * 16 * ( 10.17 + ( coef1280 * coef1920 * 0.1 ) ), dstCoef * 16 * ( 7.24 + ( coef1280 * coef1920 * 0.1 ) ), dstCoef * 16 * 2, &blanc);

        int yLine = dstCoef * 16 * (  10.6 - ( coef1280 * coef1920 * 0.00 ) - ( coef1280 * !coef1920 * 0.02 ) );
        int xLine = dstCoef * 16 * ( 7.20 - ( coef1280 * !coef1920 * 0.05 ) );
        int hLine = 0;
        int wLine = dstCoef * 16 * 0.88;
        drawLine(renderer,yLine,xLine,yLine+hLine,xLine+wLine, 3, blanc);

        sprintf(string,"%-4d",maxPv);
        Afficher_Texte_Zone(renderer, font, string, dstCoef * 16 * ( 10.55 + ( coef1280 * coef1920 * 0.1 ) ), dstCoef * 16 *( 7.24 + ( coef1280 * coef1920 * 0.1 ) ), dstCoef * 16 * 2, &blanc);
    }
    else {
        sprintf(string," %03d",pv);
        Afficher_Texte_Zone(renderer, font, string, dstCoef * 16 * ( 10.17 + ( coef1280 * coef1920 * 0.08 ) ), dstCoef * 16 * ( 7.16 + ( coef1280 * coef1920 * 0.1 ) ), dstCoef * 16 * 2, &blanc);

        int yLine = dstCoef * 16 *( 10.6 -  ( coef1280 * !coef1920 * 0.02 ) );
        int xLine = dstCoef * 16 * ( 7.25 - ( coef1280 * !coef1920 * 0.08 ) );
        int hLine = 0;
        int wLine = dstCoef * 16 * 0.78;
        drawLine(renderer,yLine,xLine,yLine+hLine,xLine+wLine, 3, blanc);

        sprintf(string," %03d",maxPv);
        Afficher_Texte_Zone(renderer, font, string, dstCoef * 16 * ( 10.55 + ( coef1280 * coef1920 * 0.08) ), dstCoef * 16 *  ( 7.16 + ( coef1280 * coef1920 * 0. ) ), dstCoef * 16 * 2, &blanc);
    }

    return 0;
}

/**
 * \fn int Affichage_All(personnage_t * perso, int tabSkill[3], SDL_Texture * textSkillBar[4], SDL_Texture * texture, map_t * map, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t **** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, TTF_Font * font, SDL_Renderer *renderer, SDL_Rect * view) {   // Initialisation des variables
 * \brief Fonction externe qui affiche Les elements graphiques
 * 
 * \param perso pointeur sur le personnage_t
 * \param tabSkill tableau de 3 entier qui correspondent a l'état des attaques ( disponible / indisponible )
 * \param textSkillBar tableau de 4 textire qui contient l'ensemble des textures nécessaire pour afficher la SkillBar
 * \param texture Texture du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param SpriteTextureListe Liste des textures des sprites
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite
 * \param window Pointeur sur l'objet SDL_Window
 * \param font Police d'écriture pour afficher le texte 
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return 0 success || 1 fail  ( statut fonction )
 */
extern int Affichage_All(personnage_t * perso, int tabSkill[3], SDL_Texture * textSkillBar[4], SDL_Texture * texture, map_t * map, Sprite_Texture_Liste_t *SpriteTextureListe, sprite_t **** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, TTF_Font * font, SDL_Renderer *renderer, SDL_Rect * view) {   // Initialisation des variables
    if ( perso == NULL ) {
        printf("Erreur : perso en parametre invalide dans Affichage_All().\n");
        return 1;
    }

    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, map->tileSize, view, &dstCoef, &xBorder, &yBorder);

    // Afficher la Map
    if ( Afficher_TileMap(texture, map, 0, LAST_TILEMAP_LAYER, view, renderer,dstCoef, xBorder, yBorder ) ) {
        printf("Erreur: Afficher_map() à echoué dans Affichage_All().\n");
        return 1;
    }
    
    // Afficher la Map Monstre
    if ( Afficher_SpriteMap(SpriteTextureListe, spriteMap, 0 ,map, listeType, view, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Erreur : Afficher_SpriteMap() à echoué dans Affichage_All().\n");
        return 1;
    }

    // Afficher la Map PNJ
    if ( Afficher_SpriteMap(SpriteTextureListe, spriteMap, 2 ,map, listeType, view, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Erreur : Afficher_SpriteMap() à echoué dans Affichage_All().\n");
        return 1;
    }

    // Afficher la Map
    if ( Afficher_SpriteMap(SpriteTextureListe, spriteMap, 1 ,map, listeType, view, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Erreur : Afficher_SpriteMap() à echoué dans Affichage_All().\n");
        return 1;
    }

    // Afficher Les Zone Qui Sont Par Dessus le Joueur
    if ( Afficher_TileMap(texture, map, LAST_TILEMAP_LAYER, LAST_TILEMAP_LAYER+1, view, renderer,dstCoef, xBorder, yBorder ) ) {
        printf("Erreur : Afficher_map() à echoué dans Affichage_All().\n");
        return 1;
    }

    // Afficher les stats des monstres
    if ( Afficher_Stats_Monstre(spriteMap ,map, view, font, renderer, dstCoef, xBorder, yBorder) ) {
        printf("Erreur : Afficher_Stats_Monstre() à echoué dans Affichage_All().\n");
        return 1;
    }

    // Afficher la skill bar
    if ( Afficher_Skill_Bar(perso, tabSkill, textSkillBar, font, renderer, dstCoef, xBorder, yBorder, win_width) ) {
        printf("Erreur : Afficher_Skill_Bar() à echoué dans Affichage_All().\n");
        return 1;
    }

    return 0;
}

/**
 * \fn void AddFrame(sprite_t **** spriteMap, map_t * map, SDL_Rect * view)
 * \brief Fonction externe qui incrémente la frame des sprites présents dans la spriteMap qui correspondent à la view.
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher
 * \param FrameCat (int) La catégorie des sprites dont la frame sera augmenté
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return Aucun retour effectué en fin de fonction
*/
extern void AddFrame(sprite_t **** spriteMap, int FrameCat, sprite_type_liste_t * listeType, map_t * map, SDL_Rect * view) {
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
    for (int i = 0; i < 3; i++ ) {
        for (int y = ymin; y < ymax; y++) {
            for (int x = xmin; x < xmax; x++) {
                if ( spriteMap[i][y][x] != NULL ) {
                    frame_cat = listeType->typeListe[(spriteMap[i][y][x]->spriteTypeId)]->frameCat;
                    if ( frame_cat == FrameCat) {
                        (spriteMap[i][y][x])->frame += 1;
                    }
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

/**
 * \fn int Clean_Remanent_Sprite( map_t * map, sprite_t **** spriteMap, SDL_Rect * view, int all)
 * \brief Fonction externe qui nettoie les anciens sprites présents dans la spriteMap[layerPersonnage] qui correspondent à la view.
 * 
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param all Entier qui correspond au type d'affichage ( 1 == tous || 0 = sprite 2 x 1 du joueur exclu )
 * \return 0 success || 1 fail  ( statut fonction )
*/
extern int Clean_Remanent_Sprite( map_t * map, sprite_t **** spriteMap, SDL_Rect * view, int all) {

    for (int y = view->y ; y < view->y+view->h; y++) {
        for (int x = view->x ; x < view->x + view->w; x++ ) {
            if (y < 0 || x < 0 || y >= map->height || x >= map->width) {
                printf("Erreur : Hors map dans Clean_Remanent_Sprite()\n");
                return 1;
            }
            else if ( !all && ( (y == view->y + 5 && x == view->x + 9) || (y == view->y + 6 && x == view->x + 9) ) ) {
                // Ne rien faire
            }
            else if (spriteMap[1][y][x] != NULL) {
                spriteMap[1][y][x] = NULL;
            }
        }
    }

    return 0;
}

/**
 * \fn int Synchronise_Sprite( map_t * map, sprite_t **** spriteMap, SDL_Rect * view)
 * \brief Fonction externe qui synchronise les sprites présents dans la spriteMap[layerPersonnage] qui correspondent à la view.
 * 
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return 0 success || 1 fail  ( statut fonction )
*/
extern int Synchronise_Sprite( map_t * map, sprite_t **** spriteMap, SDL_Rect * view) {

    for (int y = view->y ; y < view->y+view->h; y++) {
        for (int x = view->x ; x < view->x + view->w; x++ ) {
            if (y < 0 || x < 0 || y >= map->height || x >= map->width) {
                printf("Erreur : En Dehors de la map dans Deplacement_PersoSprite()\n");
                return 1;
            }
            else if (spriteMap[1][y][x] != NULL) {
                spriteMap[1][y][x]->y = y;
                spriteMap[1][y][x]->x = x;
                spriteMap[1][y][x]->frame = spriteMap[1][view->y+5+1][view->x+9]->frame;
            }
        }
    }
    
    return 0;
}

/**
 * \fn int Deplacement_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action )
 * \brief Fonction externe qui gere le deplacement du sprite et de la camera du joueur ( Changement Affichage )
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param spritePerso Tableau de sprite_t, Liste des sprites pour le personnage.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param Action Char qui correspond a l'action a effectuer
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern int Deplacement_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) {

    // Netoyage Ancien Sprite
    Clean_Remanent_Sprite(map,spriteMap,view,0);

    // Initialisation variable de colision
    int col;
    switch (Action) {
        // Action Mouvement
        case 'z':
            // Si on ne sort pas de la carte
            if ( (view->y - 1) >= 0) {
                // Verification colision
                col = Colision(map,spriteMap,'z',view->y+5,view->x+9);
                if ( col == -1 ) {
                    printf("Erreur : Echec Colision('z') dans Deplacement_PersoSprite()\n");
                    return 1;
                }
                // Si deplacement autorisé
                if ( col == 0 ) {
                    // Changement vers le sprite qui correspond a la bonne animation
                    if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[10],view->y + 5,view->x + 9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[11],view->y + 5 + 1,view->x + 9)  ) {
                        printf("Erreur : Echec Copy_Sprite('z') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    }
                    // Deplacement du sprite sur la matrice
                    if (Deplacement_Sprite(spriteMap,map,view->y+5,view->x+9,view->y+5-1,view->x+9) || Deplacement_Sprite(spriteMap,map,view->y+5+1,view->x+9,view->y+5,view->x+9) ) {
                        printf("Erreur : Echec Deplacement_Sprite('z') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    }
                    // Modification / Deplacement de la camera du joeur
                    view->y -= 1;
                }
            }
            break;
        case 'q':
            if ( (view->x - 1) >= 0) {
                col = Colision(map,spriteMap,'q',view->y+5+1,view->x+9-1);
                if ( col == -1 ) {
                    printf("Erreur : Echec Colision('q') dans Deplacement_PersoSprite()\n");
                    return 1;
                }
                if ( col == 0 ) {
                    if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[14],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[15],view->y+5+1,view->x+9) ) {
                        printf("Erreur : Echec Copy_Sprite('q') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    } 
                    if (Deplacement_Sprite(spriteMap,map,view->y+5,view->x+9,view->y+5,view->x+9-1) || Deplacement_Sprite(spriteMap,map,view->y+5+1,view->x+9,view->y+5+1,view->x+9-1) ) {
                        printf("Erreur : Echec Deplacement_Sprite('q') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    }
                    view->x -= 1;

                }
            }
            break;
        case 's':
            if ( (view->y + view->h + 1) < map->height) {
                col = Colision(map,spriteMap,'s',view->y+5+2,view->x+9);
                if ( col == -1 ) {
                    printf("Erreur : Echec Colision('s') dans Deplacement_PersoSprite()\n");
                    return 1;
                }
                if (  col == 0 ) {
                    if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[8],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[9],view->y+5+1,view->x+9) ) {
                        printf("Erreur : Echec Copy_Sprite('s') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    }
                    if (Deplacement_Sprite(spriteMap,map,view->y+5+1,view->x+9,view->y+5+2,view->x+9) || Deplacement_Sprite(spriteMap,map,view->y+5,view->x+9,view->y+5+1,view->x+9) ) {
                        printf("Erreur : Echec Deplacement_Sprite('s') dans Deplacement_PersoSprite()\n");
                        return 1;   
                    }
                    view->y += 1;
                }
            }
            break;
        case 'd':
            if ( (view->x + view->w + 1) < map->width ) {
                col = Colision(map,spriteMap,'d',view->y+5+1,view->x+9+1);
                if ( col == -1 ) {
                    printf("Erreur : Echec Colision('d') dans Deplacement_PersoSprite()\n");
                    return 1;
                }
                if ( col == 0 ) {
                    if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[12],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[13],view->y+5+1,view->x+9) ) {
                        printf("Erreur : Echec Copy_Sprite('d') dans Deplacement_PersoSprite()\n");
                        return 1;    
                    } 
                    if (Deplacement_Sprite(spriteMap,map,view->y+5,view->x+9,view->y+5,view->x+9+1) || Deplacement_Sprite(spriteMap,map,view->y+5+1,view->x+9,view->y+5+1,view->x+9+1) ) {
                        printf("Erreur : Echec Deplacement_Sprite('d') dans Deplacement_PersoSprite()\n");
                        return 1;   
                    }
                    view->x += 1;
                }
            }
            break;
        // Action Idle ( Sur place )
        case 'Z':
            // Changement vers le sprite qui correspond a la bonne animation
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[2],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[3],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('Z') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case 'Q':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[6],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[7],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('Q') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case 'S':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[0],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[1],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('S') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case 'D':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[4],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[5],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('D') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        // Animation Degat ( Sur place )
        case '1':
            // Changement vers le sprite qui correspond a la bonne animation
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[126],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[127],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('deg Z (1)') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case '2':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[130],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[131],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('deg Q (2)') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case '3':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[124],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[125],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('deg S (3)') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        case '4':
            if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[128],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[129],view->y+5+1,view->x+9) ) {
                printf("Erreur : Echec Copy_Sprite('deg D (4)') dans Deplacement_PersoSprite()\n");
                return 1;    
            } 
            break;
        // Action invalide
        default:
            printf("Erreur : Action Invalide\n");
            return 1;
            break;
    }

    // Synchronise les sprites
    Synchronise_Sprite(map,spriteMap,view);

    return 0;
}

/**
 * \fn int Attack_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) 
 * \brief Fonction externe qui gere l'animation de l'attaque de base 
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param spritePerso Tableau de sprite_t, Liste des sprites pour le personnage.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param Action Char qui correspond a la direction de l'attaque
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern int Attack_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) {

    // Netoyage Ancien Sprite
    Clean_Remanent_Sprite(map,spriteMap,view,1);

    switch (Action)
    {
    case 'Z':
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[18],view->y+5,view->x+8) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[19],view->y+5+1,view->x+8) ) {
            printf("Erreur : Echec Change_Sprite('Z') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[26],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[27],view->y+5+1,view->x+9) ) {
            printf("Erreur : Echec Change_Sprite('Z') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[34],view->y+5,view->x+10) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[35],view->y+5+1,view->x+10) ) {
            printf("Erreur : Echec Change_Sprite('Z') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        break;
    case 'Q':
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[22],view->y+5,view->x+8) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[23],view->y+5+1,view->x+8) ) {
            printf("Erreur : Echec Change_Sprite('Q') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[30],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[31],view->y+5+1,view->x+9) ) {
            printf("Erreur : Echec Change_Sprite('Q') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[38],view->y+5,view->x+10) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[39],view->y+5+1,view->x+10) ) {
            printf("Erreur : Echec Change_Sprite('Q') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        break;
    case 'S':
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[16],view->y+5,view->x+8) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[17],view->y+5+1,view->x+8) ) {
            printf("Erreur : Echec Change_Sprite('S') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[24],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[25],view->y+5+1,view->x+9) ) {
            printf("Erreur : Echec Change_Sprite('S') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[32],view->y+5,view->x+10) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[33],view->y+5+1,view->x+10) ) {
            printf("Erreur : Echec Change_Sprite('S') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        break;
    case 'D':
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[20],view->y+5,view->x+8) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[21],view->y+5+1,view->x+8) ) {
            printf("Erreur : Echec Change_Sprite('D') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[28],view->y+5,view->x+9) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[29],view->y+5+1,view->x+9) ) {
            printf("Erreur : Echec Change_Sprite('D') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[36],view->y+5,view->x+10) || Change_Sprite(spriteMap,map,spritePersoList->spriteListe[37],view->y+5+1,view->x+10) ) {
            printf("Erreur : Echec Change_Sprite('D') dans Attack_PersoSprite()\n");
            return 1;    
        } 
        break;
    default:
        printf("Erreur : Action incorrect dans Attack_PersoSprite()\n");
        return 1;
        break;
    }

    // Synchronise les sprites
    spriteMap[1][view->y+5+1][view->x+9]->frame = 3;
    Synchronise_Sprite(map,spriteMap,view);

    return 0;
}

/**
 * \fn int Special_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action )
 * \brief Fonction externe qui gere l'animation de l'attaque special 
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param spritePerso Tableau de sprite_t, Liste des sprites pour le personnage.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param Action Char qui correspond a la direction de l'attaque
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern int Special_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) {

    // Netoyage Ancien Sprite
    Clean_Remanent_Sprite(map,spriteMap,view,1);

    int index;
    switch (Action) {
    case 'Z':
        index = 52;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+4+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('D') dans Attack_PersoSprite()\n");
                    return 1;
                }
                spriteMap[1][view->y+4+i][view->x+8+j]->y = view->y+4+i;
                spriteMap[1][view->y+4+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+4+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'Q':
        index = 69;
        for (int j = 0; j < 4; j++ ) {
            for (int i = 0; i < 2; i++ ) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+7+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+7+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+7+j]->x = view->x+7+j;
                spriteMap[1][view->y+5+i][view->x+7+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'S':
        index = 40;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 4; i++) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+8+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+5+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'D':
        index = 61;
        for (int j = 0; j < 4; j++ ) {
            for (int i = 0; i < 2; i++ ) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+8+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+5+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    default:
        printf("Erreur : Action incorrect dans Special_PersoSprite()\n");
        return 1;
        break;
    }

    // Synchronise les sprites
    spriteMap[1][view->y+5+1][view->x+9]->frame = 5;
    Synchronise_Sprite(map,spriteMap,view);

    return 0;
}


/**
 * \fn int Ultime_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action )
 * \brief Fonction externe qui gere l'animation de l'attaque ultime 
 * 
 * \param spriteMap Matrice[layer][y][x] de pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param spritePerso Tableau de sprite_t, Liste des sprites pour le personnage.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param Action Char qui correspond a la direction de l'attaque
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern int Ultime_PersoSprite(sprite_t **** spriteMap, map_t * map, sprite_liste_t * spritePersoList , SDL_Rect * view, char Action ) {

    // Netoyage Ancien Sprite
    Clean_Remanent_Sprite(map,spriteMap,view,1);

    int index;
    switch (Action) {
    case 'Z':
        index = 92;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 4; i++) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+3+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('D') dans Ultime_PersoSprite()\n");
                    return 1;
                }
                spriteMap[1][view->y+3+i][view->x+8+j]->y = view->y+3+i;
                spriteMap[1][view->y+3+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+3+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'Q':
        index = 114;
        for (int j = 0; j < 5; j++ ) {
            for (int i = 0; i < 2; i++ ) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+6+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+6+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+6+j]->x = view->x+6+j;
                spriteMap[1][view->y+5+i][view->x+6+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'S':
        index = 77;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 5; i++) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+8+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+5+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    case 'D':
        index = 104;
        for (int j = 0; j < 5; j++ ) {
            for (int i = 0; i < 2; i++ ) {
                if ( Change_Sprite(spriteMap,map,spritePersoList->spriteListe[index],view->y+5+i,view->x+8+j) ) {
                    printf("Erreur : Echec Change_Sprite('%c') dans Special_PersoSprite()\n",Action);
                    return 1;   
                }
                spriteMap[1][view->y+5+i][view->x+8+j]->y = view->y+5+i;
                spriteMap[1][view->y+5+i][view->x+8+j]->x = view->x+8+j;
                spriteMap[1][view->y+5+i][view->x+8+j]->frame = 5;
                index++;
            }
        }
        break;
    default:
        printf("Erreur : Action incorrect dans Special_PersoSprite()\n");
        return 1;
        break;
    }

    // Synchronise les sprites
    spriteMap[1][view->y+5+1][view->x+9]->frame = 5;
    Synchronise_Sprite(map,spriteMap,view);

    return 0;
}

/**
 * \fn int Afficher_Texte_Zone(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, int x, int w, SDL_Color * textColor)
 * \brief Fonction externe qui affiche du texte dans une zone
 * 
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param font Police d'écriture pour afficher le texte 
 * \param texte Le texte a afficher
 * \param y La position y du texte
 * \param x La position x du texte
 * \param w La longueur de la zone de texte
 * \param textColor La couleur a utiliser pour le texte
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern int Afficher_Texte_Zone(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, int x, int w, SDL_Color * textColor)
{
    // Rectangle Destination ( Renderer )
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = 0;
    rect.w = w;
    
    // Surface pour le text
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text, (*textColor), rect.w);
    if ( textSurface == NULL ) {
        printf("Erreur : Echec TTF_RenderText_Blended_Wrapped() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Texture depuis la surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if ( textTexture == NULL ) {
        printf("Erreur : Echec TTF_RenderText_Blended_Wrapped() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Rectangle de destination
    SDL_Rect destRect = { rect.x, rect.y, textSurface->w, textSurface->h };

    // Ajoute de la texture au renderer
    if (  SDL_RenderCopy(renderer, textTexture, NULL, &destRect) < 0 ) {
        printf("Erreur : Echec SDL_RenderCopy() dans Afficher_Texte_Zone()\n");
        return 1;
    }

    // Netoyage
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    return 0;
}

/**
 * \fn void drawLine(SDL_Renderer *renderer, int y1, int x1, int y2, int x2, int thickness, SDL_Color color)
 * \brief Fonction externe qui affiche / dessine une ligne
 * 
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param y1 La position y1 du texte
 * \param x1 La position x1 du texte
 * \param y2 La position y2 du texte
 * \param x2 La position x2 du texte
 * \param thickness L'epaisseur de la ligne
 * \param color La couleur a utiliser pour la ligne
 * \return 0 Success || 1 Fail ( statut fonction )
 */
extern void drawLine(SDL_Renderer *renderer, int y1, int x1, int y2, int x2, int thickness, SDL_Color color) {
    // Calculate the slope and direction of the line
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    // Create a texture with the desired color and thickness
    SDL_Surface *lineSurface = SDL_CreateRGBSurface(0, thickness, thickness, 32, 0, 0, 0, 0);
    SDL_FillRect(lineSurface, NULL, SDL_MapRGB(lineSurface->format, color.r, color.g, color.b));
    SDL_Texture *lineTexture = SDL_CreateTextureFromSurface(renderer, lineSurface);

    // Draw the line by rendering small squares along its path
    while (x1 != x2 || y1 != y2) {
        SDL_Rect rect = {x1 - thickness/2, y1 - thickness/2, thickness, thickness};
        SDL_RenderCopy(renderer, lineTexture, NULL, &rect);
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    // Free the texture and surface
    SDL_DestroyTexture(lineTexture);
    SDL_FreeSurface(lineSurface);
}

/**
 * \fn int Respawn_Joueur( map_t * map, personnage_t * perso, SDL_Rect * view, int tabRespawnJoueur[6][2] )
 * \brief Fonction externe qui fait reapparaitre le joueur apres sa mort
 *
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param perso pointeur sur le personnage_t
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param tabRespawnJoueur Tableau d'entier qui correspond au couple de coordonnee ou reapparait le joueur selon la zone ou il est mort
 * \return 0 success || 1 fail  ( statut fonction )
 */
extern int Respawn_Joueur( map_t * map, personnage_t * perso, SDL_Rect * view, int tabRespawnJoueur[6][2] ) {

    if ( map == NULL ) {
        printf("Erreur : map en parametre invalide dans Respawn_Joueur()\n");
        return 1;
    }

    int ** matZoneLevel = map->matrice[6];
    if (matZoneLevel == NULL) {
        printf("Erreur : matZoneLevel Inexistante dans Respawn_Joueur()\n");
        return 1;
    }
    
    if ( perso == NULL ) {
        printf("Erreur : perso en parametre invalide dans Respawn_Joueur()\n");
        return 1;
    }

    if ( view == NULL ) {
        printf("Erreur : view en parametre invalide dans Respawn_Joueur()\n");
        return 1;
    }

    int zoneID;

    int y = view->y+5+1;
    int x = view->x+9;

    if ( perso->caract->pv <= 0 ) {
        perso->caract->pv = 0;
        if ( matZoneLevel[y][x] % 5 != 0 ) {
            printf("Erreur : level de zone invalide dans Respawn_Joueur()\n");
            return 1;
        }
        zoneID = ( matZoneLevel[y][x] / 5 ) - 1;
        if ( zoneID < 0 || zoneID > 5 ) {
            printf("Erreur : zoneID calculee invalide dans Respawn_Joueur()\n");
            return 1;
        }
        view->y = tabRespawnJoueur[zoneID][0];
        view->x = tabRespawnJoueur[zoneID][1];
    }
    
    return 0;
}