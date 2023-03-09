#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>




// initialisation des touches de clavier
typedef struct touche_s{
	int haut;
	int bas;
	int gauche;
	int droite;
	int espace;
	int echap;
} touche_t;

float CalculBoutonHauteur(float x, float y){
	int i=900;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}
float CalculBoutonLargeur(float x, float y){
	float i=1600;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}
extern
void getWinInfo(SDL_Window *window, int * width, int * height, int tileSize, SDL_Rect * view, int * dstCoef, int * xBorder, int * yBorder) {
    SDL_GetWindowSize(window, width, height);
    // Vérification de map et l'utilisation de dstcCoef et xBorder
    if ( tileSize != 0 ) {
        // calcule du coeficient d'affichage
        if ( dstCoef != NULL && view != NULL ) {
            (*dstCoef) = (*width) / ( (view->w) * (tileSize) ) ;
            // calcule des bordures
            if ( xBorder != NULL && yBorder != NULL ) {
                (*xBorder) = ( (*width) - ( view->w * tileSize * (*dstCoef) )  ) / 2;
                (*yBorder) = ( (*height) - ( view->h * tileSize * (*dstCoef) )  ) / 2;
            }
        }
    }
} 

extern
void changeResolution(int indiceResolution, int indiceFullscreen, SDL_Window *window) {
    switch (indiceResolution) {
    case 1:
        SDL_SetWindowSize(window, 1280, 720);
        break;
    case 2:
        SDL_SetWindowSize(window, 1600, 900);
        break;
    case 3:
        SDL_SetWindowSize(window, 1920, 1080);
        break;
    default:
        break;
    }
    switch (indiceFullscreen)
    {
    case 0:
        SDL_SetWindowFullscreen(window, 0);
        break;
    case 1:
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        break;
    case 2:
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
    default:
        break;
    }
}



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

SDL_Rect * Fonction_Button(char * image, int witdh, int height,SDL_Renderer *renderer, int y, int x){
	SDL_Rect * dstrect=malloc(sizeof(SDL_Rect));
	y=CalculBoutonHauteur(height,y);
	x=CalculBoutonLargeur(witdh,x);
	SDL_Texture *texture=NULL;
	dstrect->x =witdh/2+x;
    dstrect->y= height- height/1.2+y;
	dstrect->w = witdh/(1600/300);
	dstrect->h = height/(900/150);
	Afficher_IMG(image,renderer,&texture,NULL,dstrect);
	return dstrect;
}
//fonction des qu'on appuis sur le bouton commande
int commande(SDL_Window *window,SDL_Renderer *renderer){
	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
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
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtNew=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,-160);
		SDL_Rect * BtCon=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,400,-160);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
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
int option(SDL_Window * window,SDL_Renderer *renderer){
	SDL_bool Isrunning=1;
	SDL_Event event;
	SDL_Texture *texture = NULL;
	int Fullscreen=0;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * BtRetour=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		SDL_Rect * Bt1600=Fonction_Button("asset/menu/Bouton/Bouton1600.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-560);
		SDL_Rect * Bt1900=Fonction_Button("asset/menu/Bouton/Bouton1900.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		SDL_Rect * Bt1280=Fonction_Button("asset/menu/Bouton/Bouton1280.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,240);
		SDL_Rect * btFullScreen=Fonction_Button("asset/menu/Bouton/BoutonFullScreen.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-335);
		SDL_Rect * Commande=Fonction_Button("asset/menu/Bouton/BoutonCommande.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,15);
		/*1600*900
		1900*1080
		1280*720
		*/
		SDL_PollEvent(&event);
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
						commande(window,renderer);
						
						break;
					}
					if(event.button.x >= (*Bt1600).x && event.button.x <= (*Bt1600).x + (*Bt1600).w && event.button.y >= (*Bt1600).y && event.button.y <= (*Bt1600).y + (*Bt1600).h){
						changeResolution(2,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*Bt1900).x && event.button.x <= (*Bt1900).x + (*Bt1900).w && event.button.y >= (*Bt1900).y && event.button.y <= (*Bt1900).y + (*Bt1900).h){
						changeResolution(3,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*Bt1280).x && event.button.x <= (*Bt1280).x + (*Bt1280).w && event.button.y >= (*Bt1280).y && event.button.y <= (*Bt1280).y + (*Bt1280).h){
						changeResolution(1,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*btFullScreen).x && event.button.x <= (*btFullScreen).x + (*btFullScreen).w && event.button.y >= (*btFullScreen).y && event.button.y <= (*btFullScreen).y + (*btFullScreen).h){
						if(!Fullscreen){
						changeResolution(-1,1,window);
						Fullscreen=1;
						}
						else{
						changeResolution(-1,0,window);
						Fullscreen=0;
						}
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
		
		SDL_RenderPresent(renderer);
		SDL_Delay(10);	
	}
	return(1);
}



void menu(SDL_Window *window,SDL_Renderer *renderer){
	SDL_Event event;
	SDL_bool Isrunning=1;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	SDL_Texture *texture = NULL;
	while(Isrunning) {
		 /* Affichage de l'image de fond */
        Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
        // Affichage du bouton Jouer
		SDL_Rect * Btjouer=Fonction_Button("asset/menu/Bouton/BoutonJouerNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,-160);
		// Affichage du bouton Options
		SDL_Rect * BtOptions=Fonction_Button("asset/menu/Bouton/BoutonOptionNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,225,-160);
		// Affichage du bouton Quit
		SDL_Rect * BtQuit=Fonction_Button("asset/menu/Bouton/BoutonQuitNp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,450,-160);
		/* Boucle d'evenement */
		SDL_PollEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*Btjouer).x && event.button.x <= (*Btjouer).x + (*Btjouer).w && event.button.y >= (*Btjouer).y && event.button.y <= (*Btjouer).y + (*Btjouer).h) {
						//Permet la fermeture des qu'on appui sur le bouton en haut a gauche de la fenetre
						Isrunning=jouer(window,renderer);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

						break;
					}
					if (event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h) {
						//Permet la fermeture des qu'on appuis sur le bouton en haut a gauche de la fenetre
						Isrunning=option(window,renderer);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

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
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	Init_SDL(&window,&renderer,1600,900);
	menu(window,renderer);
	return EXIT_SUCCESS;
}