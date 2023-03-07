#include <stdio.h>
#include <stdlib.h>

#include <affichage.h>

/**
 * \file affichage.c
 * \brief Gestion affichage
 * \author Yamis MANFALOTI
 * \version 4.5
 * \date 07 février 2023
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
extern int Afficher_Map(SDL_Texture * texture, map_t * map, SDL_Rect * view, SDL_Renderer *renderer, int dstCoef, int xBorder, int yBorder ) {


     // Chargement d'une texture avec le Moteur de Rendu Graphique et le fichier de l'image
    if ( texture == NULL ) {
       printf("Erreur: La texture n'est pas chargé dans afficher_map()\n");
       return 1;
    }

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
    // Affichage des tiles de la carte
    for (int n = 0; n < layer; n++ ) {
        for (int y = ymin; y < ymax; y++) {
            for (int x = xmin; x < xmax; x++) {
                int tileNumber = map->matrice[n][y][x];
                if ( tileNumber-1 >= -1 ) {
                    /* Rectangle Source */ 
                    SDL_Rect srcrect;
                    srcrect.y = map->tileSize * ( (tileNumber-1) / 120);
                    srcrect.x = map->tileSize * ( (tileNumber-1) % 120);
                    srcrect.h = map->tileSize;
                    srcrect.w = map->tileSize;

                    /* Rectangle Destination */ 
                    SDL_Rect dstrect;
                    dstrect.x = ( dstCoef * (map->tileSize * (x - view->x)) ) + xBorder;
                    dstrect.y = ( dstCoef * (map->tileSize * (y - view->y)) ) + yBorder; 
                    dstrect.h = dstCoef * map->tileSize;
                    dstrect.w = dstCoef * map->tileSize;

                    /* Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer */
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
 * \fn void Afficher_Sprite(sprite_t * sprite, sprite_type_liste_t *listeType, SDL_Renderer * renderer, SDL_Rect * view, int dstCoef, int xBorder, int yBorder)
 * \brief Fonction externe qui affiche un sprite
 * 
 * \param sprite Pointeur sur la structure sprite_t, correspond au sprite à afficher
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \return Aucun retour effectué en fin de fonction
 * 
*/
extern void Afficher_Sprite(sprite_t * sprite, sprite_type_liste_t *listeType, SDL_Renderer * renderer, SDL_Rect * view, int dstCoef, int xBorder, int yBorder) {
    // Initialisation des variables
    SDL_Texture * textureSprite = NULL;
    SDL_Rect rectSrc;
    SDL_Rect rectDst;

    // Recupération des informations lié au type du sprite
    sprite_type_t * spriteType = listeType->typeListe[sprite->spriteTypeId];
    int spriteSize = spriteType->spriteSize;
    int frameNumber = spriteType->frameNumber;
    int spriteLine = spriteType->spriteLine;
    char * spriteSheet = spriteType->spriteSheet;
   
    // Rectangle Source
    rectSrc.x = spriteSize * ( sprite->frame = sprite->frame % frameNumber);
    rectSrc.y = spriteSize * spriteLine;
    rectSrc.w = spriteSize;
    rectSrc.h = spriteSize;

    // Rectangle Destination
    rectDst.x = ( dstCoef * (spriteSize * (sprite->x - view->x)) ) + xBorder;
    rectDst.y = ( dstCoef * (spriteSize * (sprite->y - view->y)) ) + yBorder; 
    rectDst.h = dstCoef * spriteSize;
    rectDst.w = dstCoef * spriteSize;

    // Affichage de la frame courante du sprite
    Afficher_IMG(spriteSheet, renderer, &textureSprite, &rectSrc, &rectDst);

    // Destruction en mémoire de la texture
    Detruire_Texture(textureSprite);
}

/**
 * \fn void Afficher_SpriteMap( sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder)
 * \brief Fonction externe qui affiche les sprites de la spriteMap qui correspondent à la view sur le renderer.
 * 
 * \param spriteMap Quadruple pointeur sur sprite_t, la spriteMap à afficher.
 * \param map Pointeur sur l'objet map_t, map à afficher.
 * \param listeType Pointeur sur sprite_type_liste_t, La liste des types de sprite.
 * \param view Pointeur sur l'objet SDL_Rect correspondant à la vue du joueur.
 * \param renderer Pointeur sur l'objet SDL_Renderer.
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions.
 * \param xBorder Bordure à gauche dans la fenêtre.
 * \param yBorder Bordure en haut dans la fenêtre.
 * \return Aucun retour effectué en fin de fonction.
 */
extern void Afficher_SpriteMap( sprite_t *** spriteMap, map_t * map, sprite_type_liste_t * listeType, SDL_Rect * view, SDL_Renderer * renderer, int dstCoef, int xBorder, int yBorder) {
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
                Afficher_Sprite(spriteMap[y][x], listeType, renderer, view, dstCoef, xBorder, yBorder);
            }
        }
    }

    
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
extern int Affichage_All(SDL_Texture * texture, map_t * map, sprite_t *** spriteMap, sprite_type_liste_t * listeType, SDL_Window * window, SDL_Renderer *renderer, SDL_Rect * view) {
    // Initialisation des variables
    int win_width,win_height;
    int dstCoef, xBorder, yBorder;
    
    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, &win_width, &win_height, map->tileSize, view, &dstCoef, &xBorder, &yBorder);

    // Affciher la Map
    if ( Afficher_Map(texture, map, view, renderer,dstCoef, xBorder, yBorder ) ) {
        printf("Errur: Afficher_map() à echoué dans Affichage_All\n");
        return 1;
    }
    
    // Afficher la SpriteMap
    Afficher_SpriteMap(spriteMap, map, listeType, view, renderer, dstCoef, xBorder, yBorder);

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