#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#define WINDOWS_WIDTH 1660
#define WINDOWS_HEIGHT 900



void Init_SDL(SDL_Window **window, SDL_Renderer **renderer) {
    // initialisaiton librairy SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	// Création d'une fenètre et d'un rendu par défaut
    SDL_CreateWindowAndRenderer(WINDOWS_WIDTH, WINDOWS_HEIGHT,0, window, renderer);
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

void Afficher_IMG(char * IMG, SDL_Renderer *renderer, SDL_Texture **texture, const SDL_Rect * srcrect, const SDL_Rect* dstrect) {
    // chargement d'une texture avec le Moteur de Rendu Graphique et le fichier de l'image
    if ( (*texture) == NULL ) {
        (*texture) = IMG_LoadTexture(renderer, IMG);
    }
    // envoie de la texture vers le Moteur de Rendu Graphique
    SDL_RenderCopy(renderer, (*texture), srcrect, dstrect);
}

void Detruire_Texture(SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
}

int main(void) {
	// initalisation variable et pointeur
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *boutonjouer=NULL;
	SDL_Texture *boutonOptions=NULL;
	SDL_Texture *boutonQuit=NULL;
    SDL_Event event;
	SDL_bool Isrunning=1;
	// initalisation de SDL
    Init_SDL(&window,&renderer);

    while(Isrunning) {


        /* Affichage de l'image de fond */
        Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
        // Affichage du bouton Jouer
		SDL_Rect Btjouer;
        Btjouer.x= WINDOWS_WIDTH/2-150;
        Btjouer.y= WINDOWS_HEIGHT- WINDOWS_HEIGHT/1.2;
		Btjouer.w= 300;
		Btjouer.h= 150;
        Afficher_IMG("asset/menu/Bouton/BoutonJouerNp.png", renderer, &boutonjouer, NULL, &Btjouer);
		// Affichage du bouton Options
		SDL_Rect BtOptions;
		BtOptions.x= WINDOWS_WIDTH/2-150;
		BtOptions.y= WINDOWS_HEIGHT- WINDOWS_HEIGHT/1.2+200;
		BtOptions.w= 300;
		BtOptions.h= 150;
		Afficher_IMG("asset/menu/Bouton/BoutonOptionNp.png", renderer, &boutonQuit, NULL, &BtOptions);
		// Affichage du bouton Quit
		SDL_Rect BtQuit;
		BtQuit.x= WINDOWS_WIDTH/2-150;
		BtQuit.y= WINDOWS_HEIGHT- WINDOWS_HEIGHT/1.2+400;
		BtQuit.w= 300;
		BtQuit.h= 150;
		Afficher_IMG("asset/menu/Bouton/BoutonQuitNp.png", renderer, &boutonOptions, NULL, &BtQuit);

		/* Boucle d'evenement */
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= Btjouer.x && event.button.x <= Btjouer.x + Btjouer.w && event.button.y >= Btjouer.y && event.button.y <= Btjouer.y + Btjouer.h) {
						if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
							SDL_RenderPresent(renderer);
							boutonjouer=NULL;
							
							Afficher_IMG("asset/menu/Bouton/BoutonJouerP.png", renderer, &boutonjouer, NULL, &Btjouer);
							boutonjouer=NULL;
							Afficher_IMG("asset/menu/Bouton/BoutonJouerNp.png", renderer, &boutonjouer, NULL, &Btjouer);
						}
						break;
					}
					if (event.button.x >= BtOptions.x && event.button.x <= BtOptions.x + BtOptions.w && event.button.y >= BtOptions.y && event.button.y <= BtOptions.y + BtOptions.h) {
						while(event.button.button == SDL_BUTTON_LEFT){
							Afficher_IMG("asset/menu/Bouton/BoutonOptionP.png", renderer, &boutonQuit, NULL, &BtOptions);
							SDL_WaitEvent(&event);
							if (event.button.x >= BtOptions.x && event.button.x <= BtOptions.x + BtOptions.w && event.button.y >= BtOptions.y && event.button.y <= BtOptions.y + BtOptions.h) {
								printf("Bouton Options\n");
								break;
							}
						}
						printf("Bouton Options\n");
						break;
						
					}
					if (event.button.x >= BtQuit.x && event.button.x <= BtQuit.x + BtQuit.w && event.button.y >= BtQuit.y && event.button.y <= BtQuit.y + BtQuit.h) {
						printf("Bouton Quit\n");
						Isrunning=0;
						break;
					}
				}
				break;
			// detection touche clavier
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_z:
						printf("Appui sur la touche Z\n");
						break;
					case SDLK_q:
						printf("Appui sur la touche Q\n");
						break;
					case SDLK_s:
						printf("Appui sur la touche S\n");
						break;
					case SDLK_d:
						printf("Appui sur la touche D\n");
						break;
					default:
						break;
				}
				break;
		}
		// detection fermeture fenetre qui stop la boucle
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT){
			printf("Fermeture de la fenetre\n");
            break;
			Isrunning=0;
		}

		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
    }
	// destruction en mémoire de la texture préciser
	Detruire_Texture(texture);

    // Fin de SDL + destruction allocation mémoire
    Quit_SDL(window,renderer);

    return EXIT_SUCCESS;
}

