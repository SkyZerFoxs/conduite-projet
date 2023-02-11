#include <stdlib.h>
#include <affichage.h>

/**
 * \file test_affichage.c
 * \brief Test des fonctionnalité d'affichage
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 11 février 2023
 *
 * Test des fonctionnalités d'affichage: 
 * \n Chargement et Affichage d'une image
 * \n Affichage d'une map
 * \n Obtention des informations de la fenetres
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la réussite de la fonction
 */

int main(void) {
	// initalisation variable et pointeur
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    
	// initalisation de SDL
    Init_SDL(&window,&renderer, 1600, 900);

    // initialisation de la map continent
    map_t * continent = initialiser_map("asset/map/map53.txt");

    // teste getWinInfo()
    int win_width,win_height;
    float dstCoef;
    int xBorder;
    getWinInfo(window, NULL, &win_width, &win_height, NULL, NULL);
    printf("getWinInfo(No Map, No dstCoef, No xBorder): {window width:%d , window height:%d , dstCoef:%f , xBorder:%d} \n",win_width,win_height,dstCoef,xBorder);
    getWinInfo(window, continent, &win_width, &win_height, &dstCoef, &xBorder);
    printf("getWinInfo(Full): {window width:%d , window height:%d , dstCoef:%f , xBorder:%d} \n",win_width,win_height,dstCoef,xBorder);

    // boucle qui s'arrete a la fermeture de la fenetre 
    int continuer = 1;
    while ( continuer ) {
    
        Afficher_Map("asset/tileset_with_shadow_v3.png",continent, window, renderer);

        SDL_RenderPresent(renderer);

		// detection fermeture fenetre qui stop la boucle
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
            continuer = 0;
        }
    }
    
    // destruction en mémoire de la map en paramètre
    detruire_map(continent);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return EXIT_SUCCESS;
}

