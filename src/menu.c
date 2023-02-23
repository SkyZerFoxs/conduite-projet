#include<stdio.h>

void options(){
	int choix;
	// Affichage du menu options
	// A VOIR ->> gestion des paramètres sons, bind touches, etc...
	printf("OPTIONS ------\n");
	printf(" 1 - Son\n");
	printf(" 2 - Video\n");
    printf(" 3 - Controles\n");
    printf(" 4 - Quitter\n");
	scanf("%i",&choix);

	switch(choix){
        case 1:
			printf("    ->> Son\n");
		 	break;
		case 2:
			printf("    ->> Vidéo\n"); 
			break;
        case 3:
			printf("    ->> Controles\n"); 
			break;
		case 4:
			printf("    ->> Quitter\n"); 
			break;
		default: printf("Erreur: votre choix doit etre compris entre 1 et 4\n");
	}

}

void menuDemarrage(){
	int choix;
	// Affichage du menu de démarrage
	printf("MENU DEMARRAGE ------\n");
	printf(" 1 - Nouvelle partie\n");
	printf(" 2 - Charger partie\n");
    printf(" 3 - Options\n");
    printf(" 4 - Quitter\n");
	scanf("%i",&choix);

	switch(choix){
        case 1: 
			printf("    ->> Nouvelle partie\n"); 
			break;
		case 2: 
			printf("    ->> Charger Partie\n"); 
			break;
        case 3: 
			printf("    ->> Options\n"); 
			options();
			break;
		case 4: 
			printf("    ->> Quitter\n"); 
			break;
		default: printf("Erreur: votre choix doit etre compris entre 1 et 4\n");
	}
}


void menuInGame(){
	int choix;
	// Affichage du menu in game
	printf("MENU IN GAME------\n");
	printf(" 1 - Continuer\n");
	printf(" 2 - Sauvegarder\n");
	printf(" 3 - Options\n");
	printf(" 4 - Revenir au menu\n");
	scanf("%i",&choix);

	switch(choix){
        case 1: 
			printf("    ->> Continuer\n"); 
			break;
		case 2: 
			printf("    ->> Sauvegarder\n"); 
			break;
		case 3: 
			printf("    ->> Options\n"); 
			options();
			break;
		case 4: 
			printf("    ->> Revenir au menu\n"); 
			break;
		default: printf("Erreur: votre choix doit etre compris entre 1 et 4\n");
	}
}


int main(){
	menuDemarrage();
	menuInGame();
	return 0;
}