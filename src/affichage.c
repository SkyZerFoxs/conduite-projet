#include <stdio.h>
#include <stdlib.h>

#include <affichage.h>

/**
 * \file affichage.c
 * \brief Gestion affichage
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 11 février 2023
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
 * \fn void Init_SDL(SDL_Window **window, SDL_Renderer **renderer)
 * \brief Fonction externe qui initialise les composants de SDL
 * 
 * \param window Pointeur de pointeur sur l'objet SDL_Window
 * \param renderer Pointeur de pointeur sur l'objet SDL_Renderer
 * \param width  Largeur de la fenêtre
 * \param height  Hauteur de la fenêtre
 * \return Aucun retour effectué en fin de fonction
 */
extern void Init_SDL(SDL_Window **window, SDL_Renderer **renderer, int width, int height) {
    // Initialisation library SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	// Création d'une fenêtre et d'un rendu par défaut
    SDL_CreateWindowAndRenderer(width, height, 0, window, renderer);
	// Initialisation library SDL_image
    IMG_Init(IMG_INIT_PNG);
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
 * \fn int Afficher_Tile(char * tileSet, int tileSize, float dstCoef, int xBorder, int tileNumber, int colonne, int ligne, SDL_Renderer *renderer, SDL_Texture **texture)
 * \brief Fonction externe qui affiche une tile depuis un tileSet vers une zone du Renderer
 * 
 * \param tileSet Chemin du tileSet
 * \param tileSize Taille des tiles
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure a gauche dans la fenêtre
 * \param tileNumber Numéroe de la tile à afficher
 * \param colonne Colone de la tile à afficher dans la matrice de map
 * \param ligne Ligne de la tile à afficher dans la matrice de map
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param texture Pointeur de pointeur sur l'objet SDL_Texture
 * \return int qui correspond au succès de la fonction ( tileNumber correcte )
 */
extern int Afficher_Tile(char * tileSet, int tileSize, float dstCoef, int xBorder, int tileNumber, int colonne, int ligne, SDL_Renderer *renderer, SDL_Texture **texture) {
    if ( tileNumber-1 >= -1 ) {
        /* Rectangle Source */ 
        SDL_Rect srcrect;
        srcrect.x = tileSize * ( (tileNumber-1) % 120);
        srcrect.y = tileSize * ( (tileNumber-1) / 120);
        srcrect.w = tileSize;
        srcrect.h = tileSize;

        /* Rectangle Destination */ 
        SDL_Rect dstrect;
        dstrect.x = (tileSize*dstCoef * ligne) + xBorder; 
        dstrect.y = tileSize*dstCoef * colonne; 
        dstrect.w = tileSize*dstCoef;
        dstrect.h = tileSize*dstCoef;

        /* Affiche La Tile Obtenue Grace Au Rectangle Source Vers Le Rectangle Destination Dans Le Renderer */
        Afficher_IMG(tileSet, renderer, texture, &srcrect, &dstrect );
        return 0;
    }
    else {
        return 1;
    }

}

/**
 * \fn void getWinInfo(SDL_Window *window, map_t * map, int * width, int * height, float * dstCoef, int * xBorder)
 * \brief Fonction externe qui permet d'obtenir les informations de la fenêtre
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param map Pointeur sur l'objet map_t, structure map nécéssaire à certain calcul
 * \param width  Pointeur sur un int, largeur de la fenêtre
 * \param height  Pointeur sur un int, hauteur de la fenêtre
 * \param dstCoef Coeficient qui permet d'apdater l'affichage de sorties à plusieur dimensions
 * \param xBorder Bordure a gauche dans la fenêtre
 * \return Aucun retour effectué en fin de fonction
 */
extern void getWinInfo(SDL_Window *window, map_t * map, int * width, int * height, float * dstCoef, int * xBorder ) {
    SDL_GetWindowSize(window, width, height);
    // Vérification de map et l'utilisation de dstcCoef et xBorder
    if ( map != NULL ) {
        if ( dstCoef != NULL ) {
            (*dstCoef) = (float)((*height)) / ((float)(map->height) * (float)(map->tileSize));
            if ( xBorder != NULL ) {
                (*xBorder) = ( (*width) - (map->width * map->tileSize * (*dstCoef))  ) / 2;
            }
        }
    }
    else if (dstCoef != NULL || xBorder != NULL ) {
        printf("getWinInfo Aucune Map Pour Les Calculs\n");
    }
} 

/**
 * \fn void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer)
 * \brief Fonction externe qui affiche une map composée des tiles d'un tileSet
 * 
 * \param tileSet Chemin du tileSet
 * \param map Pointeur sur l'objet map_t, map à afficher
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \return Aucun retour effectué en fin de fonction
 */
extern void Afficher_Map(char * tileSet, map_t * map, SDL_Window *window, SDL_Renderer *renderer) {
    // Initialisation des variables propre à la fonction qui sont nécessaires plus tard
    SDL_Texture *texture = NULL;
    int win_width,win_height;
    float dstCoef;
    int xBorder;

    // Récupération des informations de la fenêtre utile à l'affichage
    getWinInfo(window, map, &win_width, &win_height, &dstCoef, &xBorder);

    // Affichage des tiles de la carte du sol (matriceMap)
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if ( Afficher_Tile(tileSet, map->tileSize, dstCoef, xBorder,map->matriceMap[i][j], i, j, renderer, &texture) ) {
                printf("Erreur nb tile\n");
            }
        }
    }

    // Affichage par-dessus avec transparence des tiles de la carte des décors (matriceDecor)
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if ( Afficher_Tile(tileSet, map->tileSize, dstCoef, xBorder, map->matriceDecor[i][j], i, j, renderer, &texture) ) {
                printf("Erreur nb tile\n");
            }
        }
    }

    // destruction en mémoire de la texture crée dans la fonction
    Detruire_Texture(texture);
}