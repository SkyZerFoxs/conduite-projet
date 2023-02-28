#include <stdlib.h>
#include <affichage.h>


/**
 * \file test_affichage.c
 * \brief Test des fonctionnalité d'affichage
 * \author Yamis MANFALOTI
 * \version 3.0
 * \date 20 février 2023
 *
 * Test des fonctionnalités d'affichage: 
 * \n Obtention des informations de la fenetres
 * \n Chargement et Affichage d'une image
 * \n Affichage d'une map selon la vue du joueur
 * \n Affichage de l'animation d'un sprite
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
    map_t * continent = initialiser_map( "asset/map/mapWithColisionTeste.txt");

    // vue du joueur
    SDL_Rect vue;
    vue.x = 0;
    vue.y = 0;
    vue.w = 20;
    vue.h = 11;

    // initialisation du sprite slime
    sprite_t * slime = NULL;
    slime = Load_Sprite(0,0,0,2,"asset/Characters/SlimeV1.png",16,0);
    
    // variable utile à la boucle principal
    int FRAME_PER_SECONDE = 30;
    int testAffichageMap = 1;
    int testMouvementCamera = 0;
    int down = 1;
    int testAffichageSprite = 1;
    

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
        if ( testAffichageMap ) {
            Afficher_Map("asset/tileset.png",continent, window, renderer,&vue);
        }

        // test affiche sprite
        if ( testAffichageSprite ) {
            Afficher_Sprite(slime, renderer);
            SDL_Delay(200);
            slime->frame += 1;
        }

        // test du mouvement de la caméra
        if ( testMouvementCamera ) {
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

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);

        // gestion fps
        SDL_Delay( (1000 / FRAME_PER_SECONDE ));

    }

    // destruction en mémoire du sprite en paramètre
    Detruire_Sprite(&slime);
    
    // destruction en mémoire de la map en paramètre
    detruire_map(continent);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return 0;
}

