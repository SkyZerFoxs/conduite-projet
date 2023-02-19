#include <stdlib.h>
#include <affichage.h>


/**
 * \file test_affichage.c
 * \brief Test des fonctionnalité d'affichage
 * \author Yamis MANFALOTI
 * \version 2.0
 * \date 14 février 2023
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
    if ( Init_SDL(&window,&renderer, 1600, 900) ) {
        Quit_SDL(window,renderer);
        return 1;
    }

    // initialisation de la map continent
    map_t * continent = initialiser_map( "asset/map/cp_tmx.txt");

    // vue du joueur
    SDL_Rect vue;
    vue.x = 0;
    vue.y = 0;
    vue.w = 25;
    vue.h = 15;
    
    // variable utile à la boucle principal
    int down = 1;

    // boucle qui s'arrete a la fermeture de la fenetre 
    int quit = SDL_FALSE;
    while( quit == SDL_FALSE ) {

        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) != 0 ) {
            //L'utilisateur demande la fermeture de la fenètres
            if( event.type == SDL_QUIT )
            {
                quit = SDL_TRUE;
                break;
            }
        }
    
        // remise à 0 du renderer ( fond noir )
        SDL_RenderClear(renderer);
        // affichage de la map
        Afficher_Map("asset/tileset_with_shadow_v3.png",continent, window, renderer,&vue);
        
        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        // delay pour le test de deplacement
        SDL_Delay(200);
        // scrolling de haut en bas
        if ( vue.y <= 15 && down) {
            vue.y = vue.y + 1;
            if ( vue.y == 15) {
                down = 0;
            }
        }
        else if ( vue.y >= 0 && !down ) {
            vue.y = vue.y - 1;
            if ( vue.y == 0) {
                down = 1;
            }
        }
        
        
    
    }

    // destruction en mémoire de la map en paramètre
    detruire_map(continent);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return 0;
}

