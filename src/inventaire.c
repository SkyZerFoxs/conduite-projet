#include <stdlib.h>

#include <inventaire.h>

extern int inventaire(SDL_Texture * textInventaire, SDL_Rect * CameraJoueur, SDL_Window *window, SDL_Renderer *renderer) {
    
    int erreur = 0;

    // Nombre De Ms Par Frame Produite
    int msPerFrame;

    int FRAME_PER_SECONDE = 30;
    SDL_timer_t fps;

    int quit = SDL_FALSE;
    int keyPressed = 0;

    // Variable SDL_Event Pour Detecter Les Actions
    SDL_Event event;

    while( quit == SDL_FALSE ) {
        /* --------- Variable Boucle --------- */
        // Lancement timer temps d'execution
        Timer_Start( &fps );

        // Reset keyPressed
        keyPressed = 0;

        /* ------- Detection Evenement -------*/
        while (SDL_PollEvent(&event)) {
            // Switch Event
            switch (event.type) {
                // Evenement QUIT
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    erreur = -1;
                    break;
                // Evenement Touche Clavier
                case SDL_KEYDOWN:
                    if (  !keyPressed ) {
                            // Gestion Touche Clavier
                            switch (event.key.keysym.sym) {
                                case SDLK_ESCAPE:
                                    quit = SDL_TRUE;
                                    break;
                                case SDLK_TAB:
                                    quit = SDL_TRUE;
                                    break;
                            }
                    }
                    break;
            }
        }

        if ( Afficher_Inventaire(textInventaire,CameraJoueur,renderer,window) ) {
            printf("Erreur : Echec Affichage_All() dans play()\n");
            erreur = 1;
        }
        
        // Gestion fps
        if ( ( msPerFrame = (int)Timer_Get_Time( &fps ) ) < (1000 / FRAME_PER_SECONDE) ) {
            SDL_Delay( (1000 / FRAME_PER_SECONDE)  - msPerFrame );
        }

        // mise à jour du renderer ( update affichage)
        SDL_RenderPresent(renderer);
        

    }         
        
    return erreur;
}