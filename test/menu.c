#include<stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


int option(SDL_Window *window, SDL_Renderer *renderer);
int menu(SDL_Window *window, SDL_Renderer *renderer);
int jeu(SDL_Window *window, SDL_Renderer *renderer);
int commande(SDL_Window *window, SDL_Renderer *renderer);
int credit(SDL_Window *window, SDL_Renderer *renderer);
int quitter(SDL_Window *window, SDL_Renderer *renderer);

//fonction qui permet de calculer la position du bouton en fonction de la taille de la fenetre
float CalculBoutonHauteur(float x, float y){
	int i=900;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}
//fonction qui permet de calculer la position du bouton en fonction de la taille de la fenetre
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


int Init_SDL(SDL_Window** window, SDL_Renderer** renderer, int width, int height)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_Image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        fprintf(stderr, "SDL_Image initialization failed: %s\n", IMG_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        return 1;
    }

    // Create window
    *window = SDL_CreateWindow("Slime Hunter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void Quit_SDL(SDL_Window** window, SDL_Renderer** renderer)
{
    // Destroy renderer
    if (*renderer != NULL) {
        SDL_DestroyRenderer(*renderer);
        *renderer = NULL;
    }

    // Destroy window
    if (*window != NULL) {
        SDL_DestroyWindow(*window);
        *window = NULL;
    }

    // Quit SDL_ttf
    TTF_Quit();

    // Quit SDL_Image
    IMG_Quit();

    // Quit SDL
    SDL_Quit();
}

bool Afficher_IMG(char * image, SDL_Renderer *renderer, SDL_Texture ** texture, SDL_Rect * srcrect, SDL_Rect * dstrect) {
    SDL_Surface *surface = IMG_Load(image);
    if (surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        return false;
    }
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (*texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return false;
    }
    SDL_FreeSurface(surface);
    if (srcrect == NULL) {
        SDL_RenderCopy(renderer, *texture, NULL, dstrect);
        SDL_DestroyTexture(*texture);
    }
    else {
        SDL_RenderCopy(renderer, *texture, srcrect, dstrect);
        SDL_DestroyTexture(*texture);
    }
    return true;
}
void Detruire_Texture(SDL_Texture *texture) {
	if(texture!=NULL){
    SDL_DestroyTexture(texture);
	texture=NULL;
	}
}


/*--------------------------------------Fonction qui permet d'afficher un bouton--------------------------------------*/
SDL_Rect * Fonction_Button(char * image, int width, int height, SDL_Renderer *renderer, int y, int x) {
    SDL_Rect *dstrect = malloc(sizeof(SDL_Rect));
    y = CalculBoutonHauteur(height, y);
    x = CalculBoutonLargeur(width, x);
    SDL_Texture *texture = NULL;
    dstrect->x = width/2 + x;
    dstrect->y = height - height/1.2 + y;
    dstrect->w = width/(1600/300);
    dstrect->h = height/(900/150);
    bool success = Afficher_IMG(image, renderer, &texture, NULL, dstrect);
    if (!success) {
        fprintf(stderr, "Failed to create button texture.\n");
        free(dstrect);
        return NULL;
    }
    return dstrect;
}

/*--------------------------------------Fonction qui permet de detruire un bouton--------------------------------------*/
void Detruire_Button(SDL_Rect *dstrect){
    if(dstrect!=NULL){
		free(dstrect);
		dstrect=NULL;
	}
}

/*--------------------------------------Fonction qui permet d'afficher du texte--------------------------------------*/
SDL_Rect * Affichage_texte_Commande(char * texte, int width, int height, SDL_Renderer *renderer, int y, int x, float taille){
	// Déclaration et initialisation des variables
	TTF_Font* police = NULL;
	SDL_Color blanche = {255, 255, 255, 255};
	SDL_Surface* surface_texte = NULL;
	SDL_Texture* texture_texte = NULL;
	SDL_Rect * dest_rect = NULL;
	
	// Ouverture de la police d'écriture
	police = TTF_OpenFont("asset/font/Minecraft.ttf", taille);
	if(police == NULL){
		// Gestion d'erreur en cas d'échec de l'ouverture de la police
		printf("Erreur lors de l'ouverture de la police : %s\n", TTF_GetError());
		return NULL;
	}
	
	// Rendu du texte dans une surface
	surface_texte = TTF_RenderText_Solid(police, texte, blanche);
	if(surface_texte == NULL){
		// Gestion d'erreur en cas d'échec de rendu de la surface
		printf("Erreur lors du rendu de la surface texte : %s\n", TTF_GetError());
		TTF_CloseFont(police);
		return NULL;
	}
	
	// Création de la texture à partir de la surface
	texture_texte = SDL_CreateTextureFromSurface(renderer, surface_texte);
	if(texture_texte == NULL){
		// Gestion d'erreur en cas d'échec de création de la texture
		printf("Erreur lors de la création de la texture texte : %s\n", SDL_GetError());
		SDL_FreeSurface(surface_texte);
		TTF_CloseFont(police);
		return NULL;
	}
	
	// Allocation dynamique de la mémoire pour le rectangle de destination
	dest_rect = malloc(sizeof(SDL_Rect));
	if(dest_rect == NULL){
		// Gestion d'erreur en cas d'échec de l'allocation dynamique
		printf("Erreur lors de l'allocation de mémoire pour dest_rect\n");
		SDL_DestroyTexture(texture_texte);
		SDL_FreeSurface(surface_texte);
		TTF_CloseFont(police);
		return NULL;
	}
	
	// Assignation des valeurs au rectangle de destination
	dest_rect->x = width/2-width/5+x;
	dest_rect->y = height-height+y;
	dest_rect->w = surface_texte->w;
	dest_rect->h = surface_texte->h;
	
	// Affichage de la texture sur le renderer
	SDL_RenderCopy(renderer, texture_texte, NULL, dest_rect);
	
	// Libération de la mémoire
	SDL_DestroyTexture(texture_texte);
	SDL_FreeSurface(surface_texte);
	TTF_CloseFont(police);
	
	// Retourne le rectangle de destination alloué dynamiquement
	return dest_rect;
}
/*--------------------------------------Fonction qui permet de detruire du texte--------------------------------------*/
void Detruire_texte(SDL_Rect * rect) {
    // Vérification que le rectangle n'est pas NULL
    if(rect != NULL){
		// Libération de la mémoire allouée dynamiquement
		free(rect);
		rect = NULL;
	}
}

/*--------------------------------------Fonction quand on appui sur echap en jeu--------------------------------------*/
int echap(SDL_Window *window, SDL_Renderer *renderer){
	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	int BPretour=0,BPoptions=0,BPquitter=0;
	while(Isrunning){
		//affichage de l'image de fond
		Afficher_IMG("asset/flou.png",renderer, &texture, NULL, NULL);
		Affichage_texte_Commande("Game Paused",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,0,100);
		SDL_Rect * Retour,* Options,* Quitter;
		//affichage du bouton quitter
		Retour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,150,-160);
		//affichage du bouton retour
		Options=Fonction_Button("asset/menu/Bouton/options.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,300,-160);
		//affichage du bouton options
		Quitter=Fonction_Button("asset/menu/Bouton/quitter.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,450,-160);
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if(event.button.x > Quitter->x && event.button.x < Quitter->x + Quitter->w && event.button.y > Quitter->y && event.button.y < Quitter->y + Quitter->h && BPquitter==0){
						BPquitter=1;
						return Isrunning;
						break;
					}
				
					if(event.button.x > Retour->x && event.button.x < Retour->x + Retour->w && event.button.y > Retour->y && event.button.y < Retour->y + Retour->h && BPretour==0){
						BPretour=1;
						break;
					}
					if(event.button.x > Options->x && event.button.x < Options->x + Options->w && event.button.y > Options->y && event.button.y < Options->y + Options->h && BPoptions==0){
						BPoptions=1;
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if(event.button.x > Quitter->x && event.button.x < Quitter->x + Quitter->w && event.button.y > Quitter->y && event.button.y < Quitter->y + Quitter->h && BPquitter==1){
						BPquitter=0;
						Detruire_Button(Quitter);
						Detruire_Button(Retour);
						Detruire_Button(Options);
						//Detruire_Texture(texture);
						return Isrunning;
						break;
					}
					if(event.button.x > Retour->x && event.button.x < Retour->x + Retour->w && event.button.y > Retour->y && event.button.y < Retour->y + Retour->h){
						BPretour=0;
						Isrunning=0;
						break;
					}
					if(event.button.x > Options->x && event.button.x < Options->x + Options->w && event.button.y > Options->y && event.button.y < Options->y + Options->h){
						Isrunning=option(window,renderer);
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
			Detruire_Button(Quitter);
			Detruire_Button(Retour);
			Detruire_Button(Options);
			printf("Fermeture de la fenetre\n");
			//Detruire_Texture(texture);
            Isrunning = 0;
			return(Isrunning);
			break;
		}

		Detruire_Button(Quitter);
		Detruire_Button(Retour);
		Detruire_Button(Options);
		//Detruire_Texture(texture);
		SDL_RenderPresent(renderer);
		
	}
	return (1);
}
/*--------------------------------------Fonction quand on appuis sur le bouton commande--------------------------------------*/
int commande(SDL_Window *window,SDL_Renderer *renderer){


	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	int BPretour=0;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_RenderClear(renderer);
		/*--------------------------------------Affichage du texte/commande--------------------------------------*/
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		SDL_Rect * Avancer, * Reculer, * Gauche, * Droite, * Interagir, * AttaqueB, * AttaqueS, * AttaqueU, * Inventaire, * Pause, * BtRetour;
		Avancer=Affichage_texte_Commande("Avancer   =   Z",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*100/1600,0,WINDOWS_WIDTH/32);
		Reculer=Affichage_texte_Commande("Reculer   =   S",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*175/1600,0,WINDOWS_WIDTH/32);
		Gauche=Affichage_texte_Commande("Gauche   =   Q",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*250/1600,0,WINDOWS_WIDTH/32);
		Droite=Affichage_texte_Commande("Droite   =   D",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*325/1600,0,WINDOWS_WIDTH/32);
		Interagir=Affichage_texte_Commande("Interagir   =   F",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*400/1600,0,WINDOWS_WIDTH/32);
		AttaqueB=Affichage_texte_Commande("Attaque de base   =   A",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*475/1600,0,WINDOWS_WIDTH/32);
		AttaqueS=Affichage_texte_Commande("Attaque Special   =   B",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*550/1600,0,WINDOWS_WIDTH/32);
		AttaqueU=Affichage_texte_Commande("Attaque Ultime   =   C",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*625/1600,0,WINDOWS_WIDTH/32);
		Inventaire=Affichage_texte_Commande("Inventaire   =   E",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*700/1600,0,WINDOWS_WIDTH/32);
		Pause=Affichage_texte_Commande("Pause   =   Echap",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*775/1600,0,WINDOWS_WIDTH/32);
		
		if(BPretour==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h && BPretour==0) {
						BPretour=1;
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h) {
						BPretour=0;
						Isrunning = 0;
						break;
					}
				}break;
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
			Detruire_Button(BtRetour);
			Detruire_texte(Avancer);
			Detruire_texte(Reculer);
			Detruire_texte(Gauche);
			Detruire_texte(Droite);
			Detruire_texte(Interagir);
			Detruire_texte(AttaqueB);
			Detruire_texte(AttaqueS);
			Detruire_texte(AttaqueU);
			Detruire_texte(Inventaire);
			Detruire_texte(Pause);
			//Detruire_Texture(texture);
            Isrunning = 0;
			return(Isrunning);
			break;
		}
		Detruire_Button(BtRetour);
		Detruire_texte(Avancer);
		Detruire_texte(Reculer);
		Detruire_texte(Gauche);
		Detruire_texte(Droite);
		Detruire_texte(Interagir);
		Detruire_texte(AttaqueB);
		Detruire_texte(AttaqueS);
		Detruire_texte(AttaqueU);
		Detruire_texte(Inventaire);
		Detruire_texte(Pause);
		//Detruire_Texture(texture);
		SDL_RenderPresent(renderer);
		
	}
	return (1);
}

/*--------------------------------------Fonction du menu jouer--------------------------------------*/
int jouer(SDL_Window *window,SDL_Renderer *renderer){
	
	SDL_Event event;
	SDL_bool Isrunning=1;
	SDL_Texture *texture = NULL;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	int BNpress=0,BCpress=0,BRpress=0;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_Rect * BtNew,*BtCon,*BtRetour;
		SDL_RenderClear(renderer);
		//affichage de l'image de fond
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		if(BNpress==0){
			BtNew=Fonction_Button("asset/menu/Bouton/demarrer.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		}
		else {
			BtNew=Fonction_Button("asset/menu/Bouton/demarrerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);	
		}
		if(BCpress==0){
			BtCon=Fonction_Button("asset/menu/Bouton/charger.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,350,-160);
		}
		else{
			BtCon=Fonction_Button("asset/menu/Bouton/chargerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,350,-160);
		}
		if(BRpress==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtNew).x && event.button.x <= (*BtNew).x + (*BtNew).w && event.button.y >= (*BtNew).y && event.button.y <= (*BtNew).y + (*BtNew).h && BNpress==0) {
						BNpress=1;
						break;
					}
					
					if (event.button.x >= (*BtCon).x && event.button.x <= (*BtCon).x + (*BtCon).w && event.button.y >= (*BtCon).y && event.button.y <= (*BtCon).y + (*BtCon).h && BCpress==0 ){
						BCpress=1;
						break;
					}
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h && BRpress==0 ){
						BRpress=1;
						break;
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if (event.button.x >= (*BtNew).x && event.button.x <= (*BtNew).x + (*BtNew).w && event.button.y >= (*BtNew).y && event.button.y <= (*BtNew).y + (*BtNew).h && BNpress==1) {
						BNpress=0;
						break;
					}
					if(event.button.x >= (*BtCon).x && event.button.x <= (*BtCon).x + (*BtCon).w && event.button.y >= (*BtCon).y && event.button.y <= (*BtCon).y + (*BtCon).h && BCpress==1){
						BCpress=0;
						break;
					}
					if(event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h && BRpress==1){
						BRpress=0;
						Isrunning=0;
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
			Detruire_Button(BtNew);
			Detruire_Button(BtCon);
			Detruire_Button(BtRetour);
			//Detruire_Texture(texture);
			printf("Fermeture de la fenetre\n");
            Isrunning = 0;
			return(Isrunning);
			break;
		}
		
		Detruire_Button(BtCon);
		Detruire_Button(BtRetour);
		Detruire_Button(BtNew);
		//Detruire_Texture(texture);
		SDL_RenderPresent(renderer);
		
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
	int BPretour=0, BP1600=0, BP1920=0, BP1280=0, BPfullscreen=0, BPcommande=0;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	while(Isrunning){
		SDL_RenderClear(renderer);
		SDL_Rect *BtRetour, *Bt1600, *Bt1920, *Bt1280, *btFullScreen, *Commande;
		/*--------------gestion des boutons----------------*/
		Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		if(BPretour==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800);
		}
		if(BP1600==0){
			Bt1600=Fonction_Button("asset/menu/Bouton/1600.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		}
		else{
			Bt1600=Fonction_Button("asset/menu/Bouton/1600p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		}
		if(BP1920==0){
			Bt1920=Fonction_Button("asset/menu/Bouton/1920.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,240);
		}
		else{
			Bt1920=Fonction_Button("asset/menu/Bouton/1920p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,240);
		}
		if(BP1280==0){
			Bt1280=Fonction_Button("asset/menu/Bouton/1280.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-560);
		}
		else{
			Bt1280=Fonction_Button("asset/menu/Bouton/1280p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-560);
		}
		if(BPfullscreen==0){
			btFullScreen=Fonction_Button("asset/menu/Bouton/plein_ecran.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-360);
		}
		else{
			btFullScreen=Fonction_Button("asset/menu/Bouton/plein_ecranp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-360);
		}
		if(BPcommande==0){
			Commande=Fonction_Button("asset/menu/Bouton/commande.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,40);
		}
		else{
			Commande=Fonction_Button("asset/menu/Bouton/commandes.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,40);
		}
		/*-----------------gestion des evenements-----------------*/
		SDL_PollEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h && BPretour==0) {
						BPretour=1;
						break;
					}
					if(event.button.x >= (*Commande).x && event.button.x <= (*Commande).x + (*Commande).w && event.button.y >= (*Commande).y && event.button.y <= (*Commande).y + (*Commande).h && BPcommande==0){
						BPcommande=1;
						break;
					}
					if(event.button.x >= (*Bt1600).x && event.button.x <= (*Bt1600).x + (*Bt1600).w && event.button.y >= (*Bt1600).y && event.button.y <= (*Bt1600).y + (*Bt1600).h && BP1600==0){
						BP1600=1;
						break;
					}
					if(event.button.x >= (*Bt1920).x && event.button.x <= (*Bt1920).x + (*Bt1920).w && event.button.y >= (*Bt1920).y && event.button.y <= (*Bt1920).y + (*Bt1920).h && BP1920==0){
						BP1920=1;
						break;
					}
					if(event.button.x >= (*Bt1280).x && event.button.x <= (*Bt1280).x + (*Bt1280).w && event.button.y >= (*Bt1280).y && event.button.y <= (*Bt1280).y + (*Bt1280).h && BP1280==0){
						BP1280=1;
						break;
					}
					if(event.button.x >= (*btFullScreen).x && event.button.x <= (*btFullScreen).x + (*btFullScreen).w && event.button.y >= (*btFullScreen).y && event.button.y <= (*btFullScreen).y + (*btFullScreen).h && BPfullscreen==0){
						BPfullscreen=1;
						break;
					}
				}
				
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if (event.button.x >= (*BtRetour).x && event.button.x <= (*BtRetour).x + (*BtRetour).w && event.button.y >= (*BtRetour).y && event.button.y <= (*BtRetour).y + (*BtRetour).h && BPretour==1) {
						BPretour=0;
						Isrunning=0;
						break;
					}
					if(event.button.x >= (*Commande).x && event.button.x <= (*Commande).x + (*Commande).w && event.button.y >= (*Commande).y && event.button.y <= (*Commande).y + (*Commande).h && BPcommande==1){
						BPcommande=0;
						Isrunning=commande(window,renderer);
						break;
					}
					if(event.button.x >= (*Bt1600).x && event.button.x <= (*Bt1600).x + (*Bt1600).w && event.button.y >= (*Bt1600).y && event.button.y <= (*Bt1600).y + (*Bt1600).h && BP1600==1){
						BP1600=0;
						changeResolution(2,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*Bt1920).x && event.button.x <= (*Bt1920).x + (*Bt1920).w && event.button.y >= (*Bt1920).y && event.button.y <= (*Bt1920).y + (*Bt1920).h && BP1920==1){
						BP1920=0;
						changeResolution(3,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*Bt1280).x && event.button.x <= (*Bt1280).x + (*Bt1280).w && event.button.y >= (*Bt1280).y && event.button.y <= (*Bt1280).y + (*Bt1280).h && BP1280==1){
						BP1280=0;
						changeResolution(1,-1,window);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*btFullScreen).x && event.button.x <= (*btFullScreen).x + (*btFullScreen).w && event.button.y >= (*btFullScreen).y && event.button.y <= (*btFullScreen).y + (*btFullScreen).h && BPfullscreen==1){
						BPfullscreen=0;
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
						Detruire_Button(BtRetour);
						Detruire_Button(Bt1600);
						Detruire_Button(Bt1920);
						Detruire_Button(Bt1280);
						Detruire_Button(Commande);
						Detruire_Button(btFullScreen);
						//Detruire_Texture(texture);
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
			Detruire_Button(BtRetour);
			Detruire_Button(Bt1600);
			Detruire_Button(Bt1920);
			Detruire_Button(Bt1280);
			Detruire_Button(Commande);
			Detruire_Button(btFullScreen);
			//Detruire_Texture(texture);
			printf("Fermeture de la fenetre\n");
			Isrunning=0;
			return(Isrunning);
			break;
		}
		Detruire_Button(BtRetour);
		Detruire_Button(Bt1600);
		Detruire_Button(Bt1920);
		Detruire_Button(Bt1280);
		Detruire_Button(Commande);
		Detruire_Button(btFullScreen);
		//Detruire_Texture(texture);

		SDL_RenderPresent(renderer);
			
	}
	return(1);
}

/*-----------------Fonction de l'ecran de jeu-----------------*/
int menu(SDL_Window *window,SDL_Renderer *renderer){

	SDL_Event event;
	SDL_bool Isrunning=1;
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
	int BPjouer=0,BPoptions=0,BPquit=0;
	float taille=1600/150;
	SDL_Texture *texture = NULL;
	while(Isrunning) {
		 /* Affichage de l'image de fond */
        Afficher_IMG("asset/menu/menu.png",renderer, &texture, NULL, NULL);
		//Affichage du nom du jeu
		Affichage_texte_Commande("Slime Hunter", WINDOWS_WIDTH, WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*50/1600,-100,WINDOWS_WIDTH/taille) ;
        SDL_Rect * Btjouer,*BtOptions,*BtQuit;
		if(BPjouer==0){
			Btjouer=Fonction_Button("asset/menu/Bouton/jouer.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		}
		else{
			Btjouer=Fonction_Button("asset/menu/Bouton/jouerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160);
		}
		if(BPoptions==0){
			BtOptions=Fonction_Button("asset/menu/Bouton/options.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,275,-160);
		}
		else{
			BtOptions=Fonction_Button("asset/menu/Bouton/optionsp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,275,-160);
		}
		if(BPquit==0){
			BtQuit=Fonction_Button("asset/menu/Bouton/quitter.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,500,-160);
		}
		else{
			BtQuit=Fonction_Button("asset/menu/Bouton/quitterp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,500,-160);
		}
		/* Boucle d'evenement */
		SDL_PollEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				Isrunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (event.button.x >= (*Btjouer).x && event.button.x <= (*Btjouer).x + (*Btjouer).w && event.button.y >= (*Btjouer).y && event.button.y <= (*Btjouer).y + (*Btjouer).h && BPjouer==0) {
						BPjouer=1;
						break;
					}
					if (event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h && BPoptions==0) {
						BPoptions=1;
						break;
						
					}
					if (event.button.x >= (*BtQuit).x && event.button.x <= (*BtQuit).x + (*BtQuit).w && event.button.y >= (*BtQuit).y && event.button.y <= (*BtQuit).y + (*BtQuit).h && BPquit==0) {
						BPquit=1;
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if(event.button.x >= (*Btjouer).x && event.button.x <= (*Btjouer).x + (*Btjouer).w && event.button.y >= (*Btjouer).y && event.button.y <= (*Btjouer).y + (*Btjouer).h && BPjouer==1){
						BPjouer=0;
						Isrunning=jouer(window,renderer);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h && BPoptions==1){
						BPoptions=0;
						Isrunning=option(window,renderer);
						getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
						break;
					}
					if(event.button.x >= (*BtQuit).x && event.button.x <= (*BtQuit).x + (*BtQuit).w && event.button.y >= (*BtQuit).y && event.button.y <= (*BtQuit).y + (*BtQuit).h && BPquit==1){
						BPquit=0;
						Isrunning=0;
						break;
					}
				}break;
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
			Detruire_Button(Btjouer);
			Detruire_Button(BtOptions);
			Detruire_Button(BtQuit);
			//Detruire_Texture(texture);
			printf("Fermeture de la fenetre\n");
            break;
			Isrunning=0;
			return(Isrunning);
		}
		
		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		// destruction en mémoire des boutons
		Detruire_Button(Btjouer);
		//Detruire_Texture(texture);
		Detruire_Button(BtOptions);
		Detruire_Button(BtQuit);
		
    }
    //Detruire_Texture(texture);
	return(0);
}

int main(void) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	
	// Initialize SDL
    if (Init_SDL(&window, &renderer, 1600, 900) != 0) {
        fprintf(stderr, "SDL initialization failed\n");
        return 1;
    }
	menu(window,renderer);
	// Quit SDL
    Quit_SDL(&window,&renderer);
	exit (0);
}