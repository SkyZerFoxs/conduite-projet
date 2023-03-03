#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define WINDOWS_WIDTH 1600
#define WINDOWS_HEIGHT 900




// initialisation des touches de clavier
typedef struct touche_s{
	int haut;
	int bas;
	int gauche;
	int droite;
	int espace;
	int echap;
} touche_t;





extern int Init_SDL(SDL_Window ** window, SDL_Renderer **renderer, int width, int height) {
    // Initialisation library SDL
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    // Création d'une fenêtre
    if ( SDL_CreateWindowAndRenderer(width, height, 0, window, renderer) == -1 ) {
        printf("Une erreur s'est produite lors de la création de la fenêtre : %s",SDL_GetError());
        return SDL_TRUE;
    }
	// Initialisation de la library SDL_image
    if ( IMG_Init(IMG_INIT_PNG) == 0 ) {
        printf("Une erreur s'est produite lors du lancement de SDL_IMG: %s",IMG_GetError());
        return SDL_TRUE;
    }
    // Initialisation de la library SDL_ttf
    if (TTF_Init() == -1) {
        printf("Une erreur s'est produite lors du lancement de SDL_TTF : %s",TTF_GetError());
        return SDL_TRUE;
    }
    // return status
    return SDL_FALSE;
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

SDL_Rect * Fonction_Button(char * image,int witdh,int height,SDL_Renderer *renderer, int y, int x){
	SDL_Rect * dstrect=malloc(sizeof(SDL_Rect));
	SDL_Texture *texture=NULL;
	dstrect->x =witdh/2-150+x;
    dstrect->y= height- height/1.2+y;
	dstrect->w = 300;
	dstrect->h = 150;
	Afficher_IMG(image,renderer,&texture,NULL,dstrect);
	return dstrect;
}
//fonction des qu'on appuis sur le bouton commande
int commande(SDL_Renderer *renderer){
	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	while(Isrunning){
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-650);
		switch(event.type){
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h) {
						Isrunning = 0;
						break;
					}
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						Isrunning = 0;
						break;
				}
				break;
		}
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT){
			printf("Fermeture de la fenetre\n");
            Isrunning = 0;
			return(0);
			break;
		}
		
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
	}
	return (1);
}

// fonction des qu'on appuis sur le bouton jouer
int jouer(SDL_Window *window,SDL_Renderer *renderer){
	(void) window;
	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	while(Isrunning){
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtNew=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,0);
		SDL_Rect * BtCon=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,400,0);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-650);
		switch(event.type){
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtNew).x && event.button.x <= (*BtNew).x + (*BtNew).w && event.button.y >= (*BtNew).y && event.button.y <= (*BtNew).y + (*BtNew).h) {
						break;
					}
					if (event.button.x >= (*BtCon).x && event.button.x <= (*BtCon).x + (*BtCon).w && event.button.y >= (*BtCon).y && event.button.y <= (*BtCon).y + (*BtCon).h) {
						break;
					}
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h) {
						Isrunning = 0;
						break;
					}
				}
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
            Isrunning = 0;
			return(0);
			break;
		}
		
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
	}
	return (1);
}

// fonction des qu'on appuis sur le bouton option
int option(SDL_Renderer *renderer){
	SDL_bool Isrunning=1;
	SDL_Event event;
	SDL_Texture *texture = NULL;
	while(Isrunning){
		/*long newKey = NULL;
		long  * commandToModify = NULL;
		*/
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-650);
		SDL_Rect * Bt1600=Fonction_Button("asset/menu/Bouton/Bouton1600.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-400);
		SDL_Rect * Bt1900=Fonction_Button("asset/menu/Bouton/Bouton1900.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,0);
		SDL_Rect * Bt1280=Fonction_Button("asset/menu/Bouton/Bouton1280.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,400);
		SDL_Rect * btFullScreen=Fonction_Button("asset/menu/Bouton/BoutonFullScreen.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-175);
		SDL_Rect * Commande=Fonction_Button("asset/menu/Bouton/BoutonCommande.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,175);
		/*1600*900
		1900*1080
		1280*720
		*/
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h) {
						Isrunning=0;
						break;
					}
					if(event.button.x >= (*Commande).x && event.button.x <= (*Commande).x + (*Commande).w && event.button.y >= (*Commande).y && event.button.y <= (*Commande).y + (*Commande).h){
						commande(renderer);
						break;
					}
				}
				
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						Isrunning = 0;
						return(1);
						break;
					/*case SDLK_a:
						newKey = SDLK_a;
					case SDLK_z:
						newKey = SDLK_z;
						*/
					default:
						break;
				}
				break;
		}
			// detection touche clavier
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT){
			printf("Fermeture de la fenetre\n");
			Isrunning=0;
			return(0);
			break;
		}
		
		/*if ( clickSurToucheAvancer ) {
			commandToModify = &commande.haut
		}

		if ( commandToModify != NULL ) {
			commandToModify = newKey;
		}
		*/
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
	}
	return(1);
}



void menu(){
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event event;
	SDL_bool Isrunning=1;
	Init_SDL(&window,&renderer,WINDOWS_WIDTH,WINDOWS_HEIGHT);
	SDL_Texture *texture = NULL;
	while(Isrunning) {
		 /* Affichage de l'image de fond */
        Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
        // Affichage du bouton Jouer
		SDL_Rect * Btjouer=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,0);
		// Affichage du bouton Options
		SDL_Rect * BtOptions=Fonction_Button("asset/menu/Bouton/BoutonOptionNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,225,0);
		// Affichage du bouton Quit
		SDL_Rect * BtQuit=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,450,0);
		/* Boucle d'evenement */
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*Btjouer).x && event.button.x <= (*Btjouer).x + (*Btjouer).w && event.button.y >= (*Btjouer).y && event.button.y <= (*Btjouer).y + (*Btjouer).h) {
						//Permet la fermeture des qu'on appui sur le bouton en haut a gauche de la fenetre
						Isrunning=jouer(window,renderer);
						break;
					}
					if (event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h) {
						//Permet la fermeture des qu'on appuis sur le bouton en haut a gauche de la fenetre
						Isrunning=option(renderer);
						
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