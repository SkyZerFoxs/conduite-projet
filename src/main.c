#include <stdio.h>

#include <menu.h>

/**
 * \brief Fonction principale qui init SDL et appelle la fonction Play
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise la réussite de la fonction ( statut fonction )
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


    // Fonction menu
    printf("Debut Menu .............. OK\n");
    menu(window,renderer);
    printf("Fin Menu ................ OK\n");


    // Fin de SDL + destruction allocation mémoire
    quit:
    
    Quit_SDL(window,renderer);
    printf("Quit SDL ................ OK\n");

    return erreur;
}