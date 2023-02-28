#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#define WINDOWS_WIDTH 1600
#define WINDOWS_HEIGHT 900
// initalisation variable et pointeur
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *boutonjouer=NULL;
SDL_Texture *boutonOptions=NULL;
SDL_Texture *boutonQuit=NULL;



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

SDL_Rect * Fonction_Button(char * image,int witdh,int height,SDL_Renderer *renderer, SDL_Texture **texture, int y){
	SDL_Rect * dstrect=malloc(sizeof(SDL_Rect));
	dstrect->x =witdh/2-150;
    dstrect->y= height- height/1.2+y;
	dstrect->w = 300;
	dstrect->h = 150;
	Afficher_IMG(image,renderer,texture,NULL,dstrect);
	return dstrect;
}

void option(SDL_Renderer *renderer){
	SDL_bool Isrunning=1;
	SDL_Event event;
	while(Isrunning){

		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						Isrunning = 0;
						break;
					default:
						break;
				}
				break;
		}
			// detection touche clavier
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT){
			printf("Fermeture de la fenetre\n");
            break;
			Isrunning=0;
		}
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
	}
	
}



void menu(){
	SDL_Event event;
	SDL_bool Isrunning=1;
	Init_SDL(&window,&renderer);

	while(Isrunning) {
		 /* Affichage de l'image de fond */
        Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
        // Affichage du bouton Jouer
		/*SDL_Rect Btjouer;
        Btjouer.x= WINDOWS_WIDTH/2-150;
        Btjouer.y= WINDOWS_HEIGHT- WINDOWS_HEIGHT/1.2;
		Btjouer.w= 300;
		Btjouer.h= 150;
        Afficher_IMG("asset/menu/Bouton/BoutonJouerNp.png", renderer, &boutonjouer, NULL, &Btjouer);
		*/
		SDL_Rect * Btjouer=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,&boutonjouer,0);
		// Affichage du bouton Options
		SDL_Rect * BtOptions=Fonction_Button("asset/menu/Bouton/BoutonOptionsNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,&boutonOptions,200);
		// Affichage du bouton Quit
		SDL_Rect * BtQuit=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,&boutonQuit,450);
		/* Boucle d'evenement */
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*Btjouer).x && event.button.x <= (*Btjouer).x + (*Btjouer).w && event.button.y >= (*Btjouer).y && event.button.y <= (*Btjouer).y + (*Btjouer).h) {
						printf("Bouton Jouer\n");
						break;
					}
					if (event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h) {
						option(renderer);

						break;
						
					}
					if (event.button.x >= (*BtQuit).x && event.button.x <= (*BtQuit).x + (*BtQuit).w && event.button.y >= (*BtQuit).y && event.button.y <= (*BtQuit).y + (*BtQuit).h) {
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

}

int main(void) {
	menu();
	return EXIT_SUCCESS;
}