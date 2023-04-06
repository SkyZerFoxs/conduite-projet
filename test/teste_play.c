#include <stdio.h>

#include <play.h>

/**
 * \brief Fonction principale qui init SDL et appelle la fonction Play
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la réussite de la fonction
 */
int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int erreur = 0;

    // initalisation de SDL
    if ( Init_SDL(&window,&renderer, 1280, 720) ) {
        printf("Erreur : Init_SDL() à échoué\n");
        erreur = 1;
        goto quit;
    }
    printf("Init SDL ................ OK\n");

    // fonction principal du jeu (play) 
    printf("Debut Play .............. OK\n");
    if ( play(window,renderer,1,tabTextGif) ) {
        printf("Erreur : play() à échoué\n");
        erreur = 1;
        goto quit;
    }

    printf("Fin Play ................ OK\n");

    // Fin de SDL + destruction allocation mémoire
    quit:
    
    Quit_SDL(window,renderer);
    printf("Quit SDL ................ OK\n");

    return erreur;
}