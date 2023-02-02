#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void Init_SDL(SDL_Window **window, SDL_Renderer **renderer) {
    // initialisaiton librairy SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	// Création d'une fenètre et d'un rendu par défaut
    SDL_CreateWindowAndRenderer(500, 500,0, window, renderer);
	// initialisaiton librairy SDL_image
    IMG_Init(IMG_INIT_PNG);
}

void Quit_SDL(SDL_Window *window, SDL_Renderer *renderer) {
    // Deinitialise la librairy SDL_image.
    IMG_Quit();
	// destruction en mémoire du Renderer
    SDL_DestroyRenderer(renderer);
	// destruction en mémoire de la Fenètre
    SDL_DestroyWindow(window);
	// Deinitialise la librairy SDL_image.
    SDL_Quit();
}

void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture) {
    // chargement d'une texture avec le Moteur de Rendu Graphique et le fichier de l'image
    if ( (*texture) == NULL ) {
        (*texture) = IMG_LoadTexture(renderer, IMG);
    }
    // envoie de la texture vers le Moteur de Rendu Graphique
    SDL_RenderCopy(renderer, (*texture), NULL, NULL);
}

void Detruire_Texture(SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
}

int main(void) {
	// initalisation variable et pointeur
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Event event;
    
	// initalisation de SDL
    Init_SDL(&window,&renderer);

    while (1) {
        // afficher image
        Afficher_IMG("asset/test.png",renderer, &texture);

        SDL_RenderPresent(renderer);
		// detection fermeture fenetre qui stop la boucle
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
	// destruction en mémoire de la texture préciser
	Detruire_Texture(texture);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return EXIT_SUCCESS;
}

