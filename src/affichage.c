#include <stdio.h>
#include <stdlib.h>

#include <affichage.h>

/**
 * \file affichage.c
 * \brief Gestion affichage
 * \author Yamis MANFALOTI
 * \version 3.0
 * \date 20 février 2023
 *
 * Gestion de l'affichage:
 * \n Initialisation en mémoire
 * \n Afficher Une IMG
 * \n Afficher Une Tile (static)
 * \n Obtenir les informations relatives à la fenêtre  et/ou utile à l'affichage
 * \n Afficher une Map
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
 * \return SDL_TRUE = erreur, SDL_FALSE = success
 */
extern int Init_SDL(SDL_Window ** window, SDL_Renderer **renderer, int width, int height) {
    // Initialisation library SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    // Création d'une fenêtre
    if ( SDL_CreateWindowAndRenderer(width, height, 0, window, renderer) == -1 ) {
        printf("Une erreur s'est produite lors de la création de la fenêtre : %s",SDL_GetError());
        return SDL_TRUE;
    }
	// Initialisation de la library SDL_image
    if ( IMG_Init(IMG_INIT_PNG) == 0 ) {
        printf("Une erreur s'est produite lors du lancement de SDL_IMG: %s",IMG_GetError());
        return SDL_TRUE;
    }
    // Initialisation de la library SDL_ttf
    if (TTF_Init() == -1) {
        printf("Une erreur s'est produite lors du lancement de SDL_TTF : %s",TTF_GetError());
        return SDL_TRUE;
    }
    // return status
    return SDL_FALSE;
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
    // Destruction en mémoire du Renderer
    SDL_DestroyRenderer(renderer);
    // destruction en mémoire de la Fenètre
    SDL_DestroyWindow(window);
    // Fermeture la librairy SDL_image.
    SDL_Quit();
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
 * \return Aucun retour effectué en fin de fonction
 */
extern void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect ) {
    // Chargement d'une texture avec le Moteur de Rendu Graphique et le fichier de l'image
    if ( (*texture) == NULL ) {
        (*texture) = IMG_LoadTexture(renderer, IMG);
    }
    // Envoie de la texture vers le Moteur de Rendu Graphique
    SDL_RenderCopy(renderer, (*texture), srcrect, dstrect);
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
 * \fn int Afficher_Tile(char * tileSet, int tileSize, int dstCoef, int xBorder, int yBorder, int tileNumber, int ligne, int colonne, SDL_Rect * view, SDL_Renderer *renderer, SDL_Texture **texture)
 * \brief Fonction externe qui affiche une tile depuis un tileSet vers une zone du Renderer
 * 
 * \param tileSet Chemin du tileSet
 * \param tileSize Taille des tiles
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure a gauche dans la fenêtre
 * \param yBorder Bordure en haut dans la fenêtre
 * \param tileNumber Numéroe de la tile à afficher
 * \param ligne Ligne de la tile à afficher dans la matrice de map
 * \param colonne Colone de la tile à afficher dans la matrice de map
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param texture Pointeur de pointeur sur l'objet SDL_Texture
 * \return int qui correspond au succès de la fonction ( tileNumber correcte )
 */
extern int Afficher_Tile(char * tileSet, int tileSize, int dstCoef, int xBorder, int yBorder, int tileNumber, int ligne, int colonne, SDL_Rect * view, SDL_Renderer *renderer, SDL_Texture **texture) {
    if ( tileNumber-1 >= -1 ) {
        /* Rectangle Source */ 
        SDL_Rect srcrect;
        srcrect.y = tileSize * ( (tileNumber-1) / 120);
        srcrect.x = tileSize * ( (tileNumber-1) % 120);
        srcrect.h = tileSize;
        srcrect.w = tileSize;

        /* Rectangle Destination */ 
        SDL_Rect dstrect;
        dstrect.x = ( dstCoef * (tileSize * (colonne - view->x)) ) + xBorder;
        dstrect.y = ( dstCoef * (tileSize * (ligne - view->y)) ) + yBorder; 
        dstrect.h = dstCoef * tileSize;
        dstrect.w = dstCoef * tileSize;

        /* Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer */
        Afficher_IMG(tileSet, renderer, texture, &srcrect, &dstrect );
        return 0;
    }
    else {
        return 1;
    }

}

/**
 * \fn void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view)
 * \brief Fonction externe qui affiche une map composée des tiles d'un tileSet
 * 
 * \param tileSet Chemin du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return Aucun retour effectué en fin de fonction
 */
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view, int dstCoef, int xBorder, int yBorder) {
    // Initialisation des variables propre à la fonction
    SDL_Texture *texture = NULL;
    (void)window;
    /*
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    */
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

    int layer = map->layer;
    if ( map->layer > 3 ) {
        layer = 3;
    }

    /*
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, map->tileSize, view, &dstCoef, &xBorder, &yBorder);
    */

    // Affichage des tiles de la carte
    for (int n = 0; n < layer; n++ ) {
        for (int y = ymin; y < ymax; y++) {
            for (int x = xmin; x < xmax; x++) {
                Afficher_Tile(tileSet, map->tileSize, dstCoef, xBorder, yBorder,map->matrice[n][y][x], y, x, view, renderer, &texture);
            }
        }
    }


    // destruction en mémoire de la texture crée dans la fonction
    Detruire_Texture(texture);
}

/**
 * \fn void Afficher_Sprite(sprite_t * sprite, SDL_Renderer * renderer)
 * \brief Fonction externe qui affiche un sprite
 * 
 * \param sprite Pointeur sur la structure sprite_t, correspond au sprite à afficher
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return Aucun retour effectué en fin de fonction
 * 
*/
extern void Afficher_Sprite(sprite_t * sprite, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
    // Initialisation des variables
    SDL_Texture * textureSprite = NULL;
    SDL_Rect rectSrc;
    SDL_Rect rectDst;

    // Rectangle Source
    rectSrc.x = sprite->spriteSize * ( sprite->frame % sprite->frameNumber);
    rectSrc.y = sprite->spriteSize * sprite->spriteLine;
    rectSrc.w = sprite->spriteSize;
    rectSrc.h = sprite->spriteSize;
    
    // Rectangle Destination
    rectDst.x = ( dstCoef * (sprite->spriteSize * sprite->x )) + xBorder ;
    rectDst.y = ( dstCoef * (sprite->spriteSize * sprite->y )) + yBorder ;
    rectDst.w = dstCoef * sprite->spriteSize;
    rectDst.h = dstCoef * sprite->spriteSize;

    /*
    dstrect.x = ( dstCoef * (tileSize * (colonne - view->x)) ) + xBorder;
    dstrect.y = ( dstCoef * (tileSize * (ligne - view->y)) ) + (1 * yBorder); 
    dstrect.h = dstCoef * tileSize;
    dstrect.w = dstCoef * tileSize;
    */

    // Affichage de la frame courante du sprite
    Afficher_IMG(sprite->spriteSheet, renderer, &textureSprite, &rectSrc, &rectDst);

    // Destruction en mémoire de la texture
    Detruire_Texture(textureSprite);
}

/**
 * \fn void Affichage_all(char * tileSet, map_t * map, sprite_t * sprite, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view)
 * \brief Fonction externe qui affiche tout les ellements graphiques
 * 
 * \param tileSet Chemin du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param sprite Pointeur sur la structure sprite_t, correspond au sprite à afficher
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \return Aucun retour effectué en fin de fonction
 */
extern void Affichage_all(char * tileSet, map_t * map, sprite_t * sprite, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect * view) {
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, map->tileSize, view, &dstCoef, &xBorder, &yBorder);

    Afficher_Map(tileSet,  map, window, renderer, view, dstCoef, xBorder, yBorder);

    Afficher_Sprite(sprite, renderer, dstCoef, xBorder, yBorder);
}