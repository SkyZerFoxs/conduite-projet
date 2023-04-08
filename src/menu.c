#include <stdio.h>

#include <menu.h>

#define MS_FRAMERATE_GIF 8

/**
 * \file menu.c
 * \brief Affichage du menu
 * \author Cody Theard
 * \version 1.0
 * \date 06 Avril 2023
 *
 * Affichage du menu: 
 * \n Initialisation d'un timer
 * \n Fonction qui permet de calculer la hauteur d'un bouton
 * \n GetwinInfo permet d'avoir les informations de la taille de fenetre
 * \n Change la resolution de la fenetre
 * \n Initialition SDL
 * \n Fonction Quit SDL
 * \n Permet l'affichage d'image
 * \n Detruit la texture
 * \n Affichage d'un bouton
 * \n Detruire un bouton
 * \n Affichage d'un texte sans contour
 * \n Affichage d'un texte avec contour
 * \n Detruire le texte
 * \n Fonction echap 
 * \n Fonction Commande
 * \n Fonction Jouer
 * \n Fonction options
 * \n Fonction menu
 */



int option(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *tabTextGif[9]);
int menu(SDL_Window *window, SDL_Renderer *renderer);
int commande(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *tabTextGif[9]);



/**
 * \fn CalculBoutonHauteur(float x, float y)
 * \brief Fonction qui permet de calculer la hauteur d'un bouton
 * 
 * \param x float, la hauteur de la fenetre
 * \param y float, la largeur de la fenetre
 * \return float, la hauteur du bouton
 */
float CalculBoutonHauteur(float x, float y){
	int i=900;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}
/**
 * \fn CalculBoutonLargeur(float x, float y)
 * \brief Fonction qui permet de calculer la largeur d'un bouton
 * 
 * \param x float, la hauteur de la fenetre
 * \param y float, la largeur de la fenetre
 * \return float, la largeur du bouton
 */
float CalculBoutonLargeur(float x, float y){
	float i=1600;
    float bouton=i/y;
    bouton=x/bouton;
	return bouton;
}

/**
 * \fn bool Afficher_IMG(char * image, SDL_Renderer *renderer, SDL_Texture ** texture, SDL_Rect * srcrect, SDL_Rect * dstrect)
 * \brief Fonction externe qui permet d'afficher une image
 * 
 * \param image Chemin de l'image
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param texture Pointeur sur l'objet SDL_Texture
 * \param srcrect Pointeur sur l'objet SDL_Rect
 * \param dstrect Pointeur sur l'objet SDL_Rect
 * \return 1 si une erreur est survenue, 0 sinon
 */
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


/**
 * \fn SDL_Rect * Fonction_Button(char * image, int width, int height, SDL_Renderer *renderer, int y, int x)
 * \brief Fonction externe qui permet de créer un bouton
 * 
 * \param image Chemin de l'image
 * \param width Largeur de la fenêtre
 * \param height Hauteur de la fenêtre
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param y Position en y du bouton
 * \param x Position en x du bouton
 * \return Pointeur sur l'objet SDL_Rect
 */
SDL_Rect * Fonction_Button(char * image, int width, int height, SDL_Renderer *renderer, int y, int x, SDL_Rect * dstrect ) {
    if ( dstrect == NULL ) {
		dstrect = malloc(sizeof(SDL_Rect));
	}
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

	Detruire_Texture(&texture);
    return dstrect;
}

/**
 * \fn void Detruire_Button(SDL_Rect *dstrect)
 * \brief Fonction externe qui permet de détruire un bouton
 * 
 * \param dstrect Pointeur sur l'objet SDL_Rect
 * \return Aucun retour effectué en fin de fonction
 */
void Detruire_Button(SDL_Rect *dstrect){
    if( dstrect != NULL ){
		free( dstrect );
		dstrect=NULL;
	}
}

/**
 * \fn SDL_Rect * Affichage_texte_CommandeSansContour(char * texte, int width, int height, SDL_Renderer *renderer, int y, int x, float taille)
 * \brief Fonction externe qui permet d'afficher du texte sans contour
 * 
 * \param texte Texte à afficher
 * \param width Largeur de la fenêtre
 * \param height Hauteur de la fenêtre
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param y Position en y du texte
 * \param x Position en x du texte
 * \param taille Taille du texte
 * \return Pointeur sur l'objet SDL_Rect
 */
SDL_Rect * Affichage_texte_CommandeSansContour(char * texte, int width, int height, SDL_Renderer *renderer, int y, int x, float taille, SDL_Rect * dest_rect){

	// Déclaration et initialisation des variables
	TTF_Font* police = NULL;
	SDL_Color couleur = { 255, 140, 0, 255};
	SDL_Surface* surface_texte = NULL;
	SDL_Texture* texture_texte = NULL;
	
	// Ouverture de la police d'écriture 
	police = TTF_OpenFont("asset/font/Minecraft.ttf", taille);
	if(police == NULL){
		// Gestion d'erreur en cas d'échec de l'ouverture de la police
		printf("Erreur lors de l'ouverture de la police : %s\n", TTF_GetError());
		return NULL;
	}
	
	// Rendu du texte dans une surface
	surface_texte = TTF_RenderText_Solid(police, texte, couleur);
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
	if ( dest_rect == NULL ) {
		dest_rect = malloc(sizeof(SDL_Rect));
	}
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

/**
 * \fn SDL_Rect * Affichage_texte_Commande(char * texte, int width, int height, SDL_Renderer *renderer, int y, int x, float taille)
 * \brief Fonction externe qui permet d'afficher du texte avec contour
 * 
 * \param texte Texte à afficher
 * \param width Largeur de la fenêtre
 * \param height Hauteur de la fenêtre
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param y Position en y du texte
 * \param x Position en x du texte
 * \param taille Taille du texte
 * \return Pointeur sur l'objet SDL_Rect
 */
SDL_Rect * Affichage_texte_Commande(char * texte, int width, int height, SDL_Renderer *renderer, int y, int x, float taille, SDL_Rect * dest_rect){
	// Déclaration et initialisation des variables
	TTF_Font* police = NULL;
	SDL_Color couleur = { 255,255,255, 255};
	SDL_Surface* surface_texte = NULL;
	SDL_Texture* texture_texte = NULL;

	// Ouverture de la police d'écriture
	police = TTF_OpenFont("asset/font/Minecraft.ttf", taille);
	if(police == NULL){
		// Gestion d'erreur en cas d'échec de l'ouverture de la police
		printf("Erreur lors de l'ouverture de la police : %s\n", TTF_GetError());
		return NULL;
	}

	// Rendu du texte dans une surface transparente avec alpha
	surface_texte = TTF_RenderText_Blended(police, texte, couleur);
	if(surface_texte == NULL){
		// Gestion d'erreur en cas d'échec de rendu de la surface
		printf("Erreur lors du rendu de la surface texte : %s\n", TTF_GetError());
		TTF_CloseFont(police);
		return NULL;
	}

	// Création d'une surface noire avec le même taille que la surface de texte
	SDL_Surface* surface_noire = SDL_CreateRGBSurface(0, surface_texte->w, surface_texte->h, 32, 0, 0, 0, 0);
	if(surface_noire == NULL){
		// Gestion d'erreur en cas d'échec de création de la surface noire
		printf("Erreur lors de la création de la surface noire : %s\n", SDL_GetError());
		SDL_FreeSurface(surface_texte);
		TTF_CloseFont(police);
		return NULL;
	}

	// Remplissage de la surface noire avec un rectangle noir
	SDL_Rect rect_noir = {0, 0, surface_texte->w, surface_texte->h};
	SDL_FillRect(surface_noire, &rect_noir, SDL_MapRGB(surface_noire->format, 0, 0, 0));

	// Collage de la surface de texte transparente sur la surface noire
	SDL_BlitSurface(surface_texte, NULL, surface_noire, NULL);

	// Création de la texture à partir de la surface noire avec le texte transparent
	texture_texte = SDL_CreateTextureFromSurface(renderer, surface_noire);
	if(texture_texte == NULL){
		// Gestion d'erreur en cas d'échec de création de la texture
		printf("Erreur lors de la création de la texture texte : %s\n", SDL_GetError());
		SDL_FreeSurface(surface_noire);
		SDL_FreeSurface(surface_texte);
		TTF_CloseFont(police);
		return NULL;
	}

	// Allocation dynamique de la mémoire pour le rectangle de destination
	if ( dest_rect == NULL ) {
		dest_rect = malloc(sizeof(SDL_Rect));
	}
	if(dest_rect == NULL){
		// Gestion d'erreur en cas d'échec de l'allocation dynamique
		printf("Erreur lors de l'allocation de mémoire pour dest_rect\n");
		SDL_DestroyTexture(texture_texte);
		SDL_FreeSurface(surface_noire);
		SDL_FreeSurface(surface_texte);
		TTF_CloseFont(police);
		return NULL;
	}

	// Assignation des valeurs au rectangle de destination
	dest_rect->x = width/2-width/5+x;
	dest_rect->y = height-height+y;
	dest_rect->w = surface_noire->w;
	dest_rect->h = surface_noire->h;

	// Affichage de la texture sur le renderer
	SDL_RenderCopy(renderer, texture_texte, NULL, dest_rect);

	// Libération de la mémoire
	SDL_DestroyTexture(texture_texte);
	SDL_FreeSurface(surface_noire);
	SDL_FreeSurface(surface_texte);
	TTF_CloseFont(police);

// Retourne le rectangle de destination alloué dynamiquement
return dest_rect;
}

/**
 * \fn void Detruire_texte(SDL_Rect * rect)
 * \brief Fonction externe qui permet de libérer la mémoire allouée dynamiquement pour le rectangle de destination
 * 
 * \param rect Pointeur sur l'objet SDL_Rect
 */
void Detruire_texte(SDL_Rect * rect) {
    // Vérification que le rectangle n'est pas NULL
    if(rect != NULL){
		// Libération de la mémoire allouée dynamiquement
		free(rect);
		rect = NULL;
	}
}

/**
 * \fn echap(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *tabTextGif[9])
 * \brief Fonction externe qui permet d'afficher le menu pause
 * 
 * \param window Pointeur sur l'objet SDL_Window
 * \param renderer Pointeur sur l'objet SDL_Renderer
 * \param background_texture Pointeur sur la texture de l'image d'arrière plan
 * \param tabTextGif Tableau de pointeurs sur les objets SDL_Texture
 * \return -1 Si on ferme le jeux || 0 Si on quite seulement le menu echap
 */
extern int echap(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture * background_texture, SDL_Texture * tabTextGif[9]) {
	/* Variable */

	// Variable event
	SDL_Event event;

	// Variable boucle
	SDL_bool Isrunning = 1;

	// Variable taille fenetre
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;

	// Variable timer
	SDL_timer_t timerFrame;


	// Variable Texture Bouton
	SDL_Rect * Retour = NULL;
	SDL_Rect * Options = NULL;
	SDL_Rect * Quitter = NULL;
	SDL_Rect * TextePause = NULL;

		
	// Variable Bouton
	int BPretour=0,BPoptions=0,BPquitter=0;


	/* Variable Boucle */
	Timer_Start(&timerFrame);


	while( Isrunning == 1 ) {
		// Recuperation information fenetre
		getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

		// Renderer Clear 
		SDL_RenderClear(renderer);
		
		// Texture d'Arrière Plan
		SDL_RenderCopy(renderer,background_texture,NULL,NULL);
		
		/* Affichage Bouton & Texte */

		// Afichage Texte GamePaused
		TextePause = Affichage_texte_Commande("Game Paused",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,0,0,100,TextePause);

		//affichage du bouton quitter
		Retour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,150,-160,Retour);

		//affichage du bouton retour
		Options=Fonction_Button("asset/menu/Bouton/options.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,300,-160,Options);

		//affichage du bouton options
		Quitter=Fonction_Button("asset/menu/Bouton/quitter.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,450,-160,Quitter);

		/* Detection d'Evenement */

		while (SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT:
					Isrunning = -1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						if(event.button.x > Quitter->x && event.button.x < Quitter->x + Quitter->w && event.button.y > Quitter->y && event.button.y < Quitter->y + Quitter->h && BPquitter==0){
							BPquitter=1;
							Isrunning = -2;
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
							Isrunning = -2;
							break;
						}
						if(event.button.x > Retour->x && event.button.x < Retour->x + Retour->w && event.button.y > Retour->y && event.button.y < Retour->y + Retour->h){
							BPretour=0;
							Isrunning=0;
							break;
						}
						if(event.button.x > Options->x && event.button.x < Options->x + Options->w && event.button.y > Options->y && event.button.y < Options->y + Options->h){
							if ( option(window,renderer,tabTextGif) == -1 ) {
								Isrunning = -1;
							}
							else {
								Isrunning = 1;
							}
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
		}

		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		
	}

	// Destruction en mémoire du texte
	if ( TextePause != NULL ) {
		Detruire_texte(TextePause);
	}

	// Destruction en mémoire des boutons
	if ( Quitter != NULL ) {
		Detruire_Button(Quitter);
	}
	if ( Retour != NULL ) {
		Detruire_Button(Retour);
	}
	if ( Options != NULL ) {
		Detruire_Button(Options);
	}
	
	return (Isrunning);
}

/**
 * \fn int commande(SDL_Window *window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9])
 * \brief Fonction qui affiche la page de commande
 * \param window la fenetre
 * \param renderer le renderer
 * \param tabTextGif le tableau de texture
 * \return -1 Si on ferme le jeux || 0 Si on quite seulement le menu commande
 */
int commande(SDL_Window *window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9]){

	/* Variable */

	// Variable event
	SDL_Event event;

	// Variable boucle
	SDL_bool Isrunning = 1;

	// Variable taille fenetre
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;

	// Variable timer
	SDL_timer_t timerFrame;


	// Variable Frame Fond
	int frameGif = 0;

	// Variable Texture Bouton
	SDL_Rect * Avancer = NULL;
	SDL_Rect * Entre = NULL;
	SDL_Rect * Reculer = NULL;
	SDL_Rect * Gauche = NULL;
	SDL_Rect * Droite = NULL;
	SDL_Rect * Interagir = NULL;
	SDL_Rect * AttaqueB = NULL;
	SDL_Rect * AttaqueS = NULL;
	SDL_Rect * AttaqueU = NULL;
	SDL_Rect * Inventaire = NULL;
	SDL_Rect * Pause = NULL;
	SDL_Rect * BtRetour = NULL;
	SDL_Rect * Suivant = NULL;
	SDL_Rect * Information = NULL;
		
	// Variable Bouton
	int BPretour=0;

	// Variable d'affichage
	int affichage=1,n=0;

	/* Variable Boucle */
	Timer_Start(&timerFrame);


	while( Isrunning == 1 ) {
		// Recuperation information fenetre
		getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

		// Gestion Renderer Clear 
		SDL_RenderClear(renderer);
		if(n==4){
			n=2;
		}
		else if(n==3){
			n=1;
		}

		// Affichage de l'image de fond
		SDL_RenderCopy(renderer, tabTextGif[frameGif], NULL, NULL);

		// Gestion frame fond
		if((int)Timer_Get_Time(&timerFrame)>=(1000/MS_FRAMERATE_GIF)){
			frameGif++;
			if(frameGif==8){
				frameGif=0;
			}
			Timer_Start(&timerFrame);
		}

		/*--------------------------------------Affichage du texte/commande--------------------------------------*/
		
		if(affichage==1){
			Avancer=Affichage_texte_Commande("Haut   =   Z",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*100/1600,0,WINDOWS_WIDTH/32,Avancer);
			Gauche=Affichage_texte_Commande("Gauche   =   Q",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*175/1600,0,WINDOWS_WIDTH/32,Gauche);
			Reculer=Affichage_texte_Commande("Bas   =   S",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*250/1600,0,WINDOWS_WIDTH/32,Reculer);
			Droite=Affichage_texte_Commande("Droite   =   D",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*325/1600,0,WINDOWS_WIDTH/32,Droite);
			Interagir=Affichage_texte_Commande("Interagir   =   E",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*400/1600,0,WINDOWS_WIDTH/32,Interagir);
			AttaqueB=Affichage_texte_Commande("Attaque de base   =   Clic gauche",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*475/1600,0,WINDOWS_WIDTH/32,AttaqueB);

			Suivant=Affichage_texte_Commande("Appuyez sur N pour changer de page",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*775/1600,-WINDOWS_HEIGHT/5,WINDOWS_WIDTH/32,Suivant);	
		}
		if(affichage==0){
			AttaqueS=Affichage_texte_Commande("Attaque Special   =   A",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*100/1600,0,WINDOWS_WIDTH/32,AttaqueS);
			AttaqueU=Affichage_texte_Commande("Attaque Ultime   =   R",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*175/1600,0,WINDOWS_WIDTH/32,AttaqueU);
			Information=Affichage_texte_Commande("Informations dans l'inventaire   =   I",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*250/1600,0,WINDOWS_WIDTH/32,Information);
			Entre=Affichage_texte_Commande("Valider   =   Entree",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*325/1600,0,WINDOWS_WIDTH/32,Entre);
			Pause=Affichage_texte_Commande("Pause   =   Echap",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*400/1600,0,WINDOWS_WIDTH/32,Pause);
			Inventaire=Affichage_texte_Commande("Inventaire   =   Tab",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*475/1600,0,WINDOWS_WIDTH/32,Inventaire);
			
			Suivant=Affichage_texte_Commande("Appuyez sur N pour changer de page",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*775/1600,-WINDOWS_HEIGHT/5,WINDOWS_WIDTH/32,Suivant);
		}
		if(BPretour==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}

		/* Detection d'Evenement */
		while (SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT:
					Isrunning = -1;
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
						case SDLK_n:
							if(affichage==1){
								affichage=0;
								n=3;
							}
							else if(affichage==0){
								affichage=1;
								n=4;
							}
							break;
					}
					break;
			}
		}
		
		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		
	}

	// destruction en mémoire des textes de la page n°1
	if ( Avancer != NULL ) {
		Detruire_texte(Avancer);
	}
	if ( Reculer != NULL ) {
		Detruire_texte(Reculer);
	}
	if ( Gauche != NULL ) {
		Detruire_texte(Gauche);
	}
	if ( Droite != NULL ) {
		Detruire_texte(Droite);
	}
	if ( Interagir != NULL ) {
		Detruire_texte(Interagir);
	}
	if ( AttaqueB != NULL ) {
		Detruire_texte(AttaqueB);
	}
	if ( Suivant != NULL ) {
		Detruire_texte(Suivant);
	}
	
	// destruction en mémoire des textes de la page n°2
	if ( Pause != NULL ) {
		Detruire_texte(Pause);
	}
	if ( Information != NULL ) {
		Detruire_texte(Information);
	}
	if ( Entre != NULL ) {
		Detruire_texte(Entre);
	}
	if ( AttaqueU != NULL ) {
		Detruire_texte(AttaqueU);
	}
	if ( Inventaire != NULL ) {
		Detruire_texte(Inventaire);
	}
	if ( AttaqueS != NULL ) {
		Detruire_texte(AttaqueS);
	}

	// Destruction en mémoire des bouton
	if ( BtRetour != NULL ) {
		Detruire_Button(BtRetour);
	}

	return ( Isrunning );
}

/**
 * \fn int jouer(SDL_Window *window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9])
 * \brief Fonction qui permet de lancer le jeu
 * 
 * \param window la fenetre
 * \param renderer le renderer
 * \param tabTextGif le tableau de texture
 * \return -1 Si on ferme le jeux || 0 Si on quite seulement le menu joueur
 */
int jouer(SDL_Window *window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9]){

	/* Variable */

	// Variable event
	SDL_Event event;

	// Variable boucle
	SDL_bool Isrunning = 1;

	// Varaible de sortie de la fonction play
	int sortiePlay = 0;

	// Variable taille fenetre
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;

	// Variable timer
	SDL_timer_t timerFrame;
	SDL_timer_t fps;

	// Variable Texture IMG Chargement
	SDL_Texture * textChargement = NULL;

	// Variable Frame Fond
	int frameGif = 0;

	// Variable Texture Bouton
	SDL_Rect * BtNew = NULL;
	SDL_Rect * BtCon = NULL;
	SDL_Rect * BtRetour = NULL;

	// Variable Bouton
	int BNpress=0,BCpress=0,BRpress=0;

	/* Initialisation */

	textChargement = IMG_LoadTexture(renderer,"asset/hud/chargement.png");
	if ( textChargement == NULL ) {
		printf("Erreur : Echec  IMG_Load(textChargement) dans jouer()\n");
		Isrunning = -1;
	}


	/* Variable Boucle */
	Timer_Start(&timerFrame);
	
	while( Isrunning == 1 ){
		// Debut Timer FPS
		Timer_Start(&fps);
		
		// Recuperation information fenetre
		getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

		// Clear renderer
		SDL_RenderClear(renderer);
		
		// Affichage de l'image de fond
		SDL_RenderCopy(renderer, tabTextGif[frameGif], NULL, NULL);

		// Gestion frame fond
		if((int)Timer_Get_Time(&timerFrame)>=(1000/MS_FRAMERATE_GIF)){
			frameGif++;
			if(frameGif==8){
				frameGif=0;
			}
			Timer_Start(&timerFrame);
		}

		/* Gestion Bouton */

		if(BNpress==0){
			BtNew=Fonction_Button("asset/menu/Bouton/demarrer.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,BtNew);
		}
		else {
			BtNew=Fonction_Button("asset/menu/Bouton/demarrerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,BtNew);	
		}
		if(BCpress==0){
			BtCon=Fonction_Button("asset/menu/Bouton/charger.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,350,-160,BtCon);
		}
		else{
			BtCon=Fonction_Button("asset/menu/Bouton/chargerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,350,-160,BtCon);
		}
		if(BRpress==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}
		
		/* Detection d'Evenement */
		while (SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT:
					Isrunning = -1;
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
							// Affichage IMG Chargement
							SDL_RenderCopy(renderer,textChargement,NULL,NULL);
							SDL_RenderPresent(renderer);
							// Apelle Play
							sortiePlay = play(window,renderer,0,tabTextGif);
							if ( sortiePlay == -1 ) {
								Isrunning = -1;
							}
							else if ( sortiePlay == 1 ) {
								printf("Erreur : Echec play() dans jouer()\n");
								Isrunning = 1;
							}
							else {
								Isrunning = 0;
							}
							
							break;
						}
						if(event.button.x >= (*BtCon).x && event.button.x <= (*BtCon).x + (*BtCon).w && event.button.y >= (*BtCon).y && event.button.y <= (*BtCon).y + (*BtCon).h && BCpress==1){
							BCpress=0;
							// Affichage IMG Chargement
							SDL_RenderCopy(renderer,textChargement,NULL,NULL);
							SDL_RenderPresent(renderer);
							// Apelle Play
							sortiePlay = play(window,renderer,1,tabTextGif);
							if ( sortiePlay == -1 ) {
								Isrunning = -1;
							}
							else if ( sortiePlay == 1 ) {
								printf("Erreur : Echec play() dans jouer()\n");
								Isrunning = 1;
							}
							else {
								Isrunning = 0;
							}
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
		}

		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		
	}

	// Destruction en mémoire texture IMG Chargement
	if ( textChargement != NULL ) {
		Detruire_Texture(&textChargement);
	}

	// Destruction en mémoire des boutons
	if ( BtCon != NULL ) {
		Detruire_Button(BtCon);
	}
	if ( BtRetour != NULL ) {
		Detruire_Button(BtRetour);
	}
	if ( BtNew != NULL ) {
		Detruire_Button(BtNew);
	}

	return ( Isrunning );
}

/**
 * \fn int option(SDL_Window * window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9])
 * \brief Fonction qui permet d'afficher le menu option
 * 
 * \param window la fenetre
 * \param renderer le rendu
 * \param tabTextGif tableau de texture contenant les images du gif
 * \return -1 Si on ferme le jeux || 0 Si on quite seulement le menu
 */
int option(SDL_Window * window,SDL_Renderer *renderer, SDL_Texture *tabTextGif[9]){
	/* Variable */

	// Variable event
	SDL_Event event;

	// Variable boucle
	SDL_bool Isrunning = 1;

	// Variable taille fenetre
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;

	// Variable timer
	SDL_timer_t timerFrame;


	// Variable Frame Fond
	int frameGif = 0;

	// Variable Fullscreen
	int Fullscreen=0;

	// Variable Texture Bouton
	SDL_Rect * BtRetour = NULL;
	SDL_Rect * Bt1600 = NULL;
	SDL_Rect * Bt1920 = NULL;
	SDL_Rect * Bt1280 = NULL;
	SDL_Rect * btFullScreen = NULL;
	SDL_Rect * Commande = NULL;

	// Variable Bouton
	int BPretour=0, BP1600=0, BP1920=0, BP1280=0, BPfullscreen=0, BPcommande=0;


	/* Initialisation */

	// Recuperation information fenetre
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

	/* Variable Boucle */
	Timer_Start(&timerFrame);


	while( Isrunning == 1 ){
		// Recuperation information fenetre
		getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

		// Clear renderer
		SDL_RenderClear(renderer);
		
		// Affichage de l'image de fond
		SDL_RenderCopy(renderer, tabTextGif[frameGif], NULL, NULL);

		// Gestion frame fond
		if((int)Timer_Get_Time(&timerFrame)>=(1000/MS_FRAMERATE_GIF)){
			frameGif++;
			if(frameGif==8){
				frameGif=0;
			}
			Timer_Start(&timerFrame);
		}
		
		/*--------------gestion des boutons----------------*/
		
		if(BPretour==0){
			BtRetour=Fonction_Button("asset/menu/Bouton/retour.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}
		else{
			BtRetour=Fonction_Button("asset/menu/Bouton/retourp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,-150,-800,BtRetour);
		}
		if(BP1600==0){
			Bt1600=Fonction_Button("asset/menu/Bouton/1600.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,Bt1600);
		}
		else{
			Bt1600=Fonction_Button("asset/menu/Bouton/1600p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,Bt1600);
		}
		if(BP1920==0){
			Bt1920=Fonction_Button("asset/menu/Bouton/1920.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,240,Bt1920);
		}
		else{
			Bt1920=Fonction_Button("asset/menu/Bouton/1920p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,240,Bt1920);
		}
		if(BP1280==0){
			Bt1280=Fonction_Button("asset/menu/Bouton/1280.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-560,Bt1280);
		}
		else{
			Bt1280=Fonction_Button("asset/menu/Bouton/1280p.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-560,Bt1280);
		}
		if(BPfullscreen==0){
			btFullScreen=Fonction_Button("asset/menu/Bouton/plein_ecran.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-360,btFullScreen);
		}
		else{
			btFullScreen=Fonction_Button("asset/menu/Bouton/plein_ecranp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,-360,btFullScreen);
		}
		if(BPcommande==0){
			Commande=Fonction_Button("asset/menu/Bouton/commande.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,40,Commande);
		}
		else{
			Commande=Fonction_Button("asset/menu/Bouton/commandes.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,250,40,Commande);
		}

		/*-----------------gestion des evenements-----------------*/
        while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					Isrunning = -1;
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
							if ( commande(window,renderer,tabTextGif) == -1 ) {
								Isrunning = -1;
							}
							else {
								Isrunning = 1;
							}
							break;
						}
						if(event.button.x >= (*Bt1600).x && event.button.x <= (*Bt1600).x + (*Bt1600).w && event.button.y >= (*Bt1600).y && event.button.y <= (*Bt1600).y + (*Bt1600).h && BP1600==1){
							BP1600=0;
							changeResolution(2,-1,window);
							break;
						}
						if(event.button.x >= (*Bt1920).x && event.button.x <= (*Bt1920).x + (*Bt1920).w && event.button.y >= (*Bt1920).y && event.button.y <= (*Bt1920).y + (*Bt1920).h && BP1920==1){
							BP1920=0;
							changeResolution(3,-1,window);
							break;
						}
						if(event.button.x >= (*Bt1280).x && event.button.x <= (*Bt1280).x + (*Bt1280).w && event.button.y >= (*Bt1280).y && event.button.y <= (*Bt1280).y + (*Bt1280).h && BP1280==1){
							BP1280=0;
							changeResolution(1,-1,window);
							break;
						}
						if(event.button.x >= (*btFullScreen).x && event.button.x <= (*btFullScreen).x + (*btFullScreen).w && event.button.y >= (*btFullScreen).y && event.button.y <= (*btFullScreen).y + (*btFullScreen).h && BPfullscreen==1){
							BPfullscreen=0;
							if(!Fullscreen){
								changeResolution(-1,2,window);
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
							break;
						default:
							break;
					}
					break;
			}
		}

		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
			
	}

	// destruction en mémoire des boutons
	if ( BtRetour != NULL ) {
		Detruire_Button(BtRetour);
	}
	if ( Bt1600 != NULL ) {
		Detruire_Button(Bt1600);
	}
	if ( Bt1920 != NULL ) {
		Detruire_Button(Bt1920);
	}
	if ( Bt1280 != NULL ) {
		Detruire_Button(Bt1280);
	}
	if ( Commande != NULL ) {
		Detruire_Button(Commande);
	}
	if ( btFullScreen != NULL ) {
		Detruire_Button(btFullScreen);
	}

	return( Isrunning );
}

/**
 * \fn int menu(SDL_Window *window,SDL_Renderer *renderer)
 * \brief Fonction qui affiche le menu
 * \param window la fenetre
 * \param renderer le renderer
 * \return 1 si le menu est fermé, 0 sinon
 */
extern
int menu(SDL_Window *window,SDL_Renderer *renderer){
	
	/* Variable */

	// Variable event
	SDL_Event event;

	// Variable boucle
	SDL_bool Isrunning = 1;

	// Variable taille fenetre
	int WINDOWS_WIDTH, WINDOWS_HEIGHT;

	// Variable taille
	float taille = 1600/150;

	// Variable timer
	SDL_timer_t timerFrame;

	// Variable sprintf
	char string[256];

	// Variable Texture Fond
	SDL_Texture * tabTextGif[9];

	// Variable Frame Fond
	int nbFrame = 9;
	int frameGif = 0;
	
	// Variable Texture Bouton
	SDL_Rect * Btjouer = NULL;
	SDL_Rect * BtOptions = NULL;
	SDL_Rect * BtQuit = NULL;
	SDL_Rect * TexteTitre = NULL;

	// Variable Bouton
	int BPjouer = 0, BPoptions = 0, BPquit = 0;

		
	/* Initialisation */

	// Recuperation information fenetre
	getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

	// Initialisation image fond
	for(int i=1 ; i< nbFrame ; i++){
		sprintf(string,"asset/fond/frame-%d.gif",i);
		tabTextGif[i-1]=IMG_LoadTexture(renderer,string);
	}

	/* Variable Boucle */
	Timer_Start(&timerFrame);

	while(Isrunning) {
		// Recuperation information fenetre
		getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);

		// Clear renderer
		SDL_RenderClear(renderer);
		
		// Affichage de l'image de fond
		SDL_RenderCopy(renderer, tabTextGif[frameGif], NULL, NULL);

		// Gestion frame fond
		if((int)Timer_Get_Time(&timerFrame)>=(1000/MS_FRAMERATE_GIF)){
			frameGif++;
			if(frameGif==8){
				frameGif=0;
			}
			Timer_Start(&timerFrame);
		}

		//Affichage du nom du jeu
		TexteTitre = Affichage_texte_CommandeSansContour("Hero's Quest", WINDOWS_WIDTH, WINDOWS_HEIGHT,renderer,WINDOWS_WIDTH*50/1600,-100,WINDOWS_WIDTH/taille,TexteTitre) ;

		// Gestion bouton
		if(BPjouer==0){
			Btjouer=Fonction_Button("asset/menu/Bouton/jouer.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,Btjouer);
		}
		else{
			Btjouer=Fonction_Button("asset/menu/Bouton/jouerp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,50,-160,Btjouer);
		}
		if(BPoptions==0){
			BtOptions=Fonction_Button("asset/menu/Bouton/options.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,275,-160,BtOptions);
		}
		else{
			BtOptions=Fonction_Button("asset/menu/Bouton/optionsp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,275,-160,BtOptions);
		}
		if(BPquit==0){
			BtQuit=Fonction_Button("asset/menu/Bouton/quitter.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,500,-160,BtQuit);
		}
		else{
			BtQuit=Fonction_Button("asset/menu/Bouton/quitterp.png",WINDOWS_WIDTH,WINDOWS_HEIGHT,renderer,500,-160,BtQuit);
		}

		/* Boucle d'evenement */
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					Isrunning=0;
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
							if ( jouer(window,renderer,tabTextGif) == -1 ) {
								Isrunning = 0;
							}
							else {
								Isrunning = 1;
							}
							getWinInfo(window,&WINDOWS_WIDTH,&WINDOWS_HEIGHT,0,NULL,NULL,NULL,NULL);
							break;
						}
						if(event.button.x >= (*BtOptions).x && event.button.x <= (*BtOptions).x + (*BtOptions).w && event.button.y >= (*BtOptions).y && event.button.y <= (*BtOptions).y + (*BtOptions).h && BPoptions==1){
							BPoptions=0;
							if ( option(window,renderer,tabTextGif) == -1) {
								Isrunning = 0;
							}
							else {
								Isrunning = 1;
							}
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
		}
		
		// Mise a jour de l'affichage
		SDL_RenderPresent(renderer);
		
		
    }

	// destruction en mémoire des boutons
	if ( Btjouer != NULL ) {
		Detruire_Button(Btjouer);
	}
	if ( BtOptions != NULL ) {
		Detruire_Button(BtOptions);
	}
	if ( BtQuit != NULL ) {
		Detruire_Button(BtQuit);
	}

	if ( TexteTitre != NULL ) {
		Detruire_texte(TexteTitre);
	}
	
	// destruction en mémoire des textures de fond
    for(int i=1 ; i< nbFrame ; i++){
		if ( tabTextGif[i-1] != NULL ) {
			Detruire_Texture(&(tabTextGif[i-1]));
		}
	}

	return(0);
}
