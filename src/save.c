#include <stdio.h>
#include <stdlib.h>

#include <save.h>


extern int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet,int lastQuest) {
    // Verification parametre
    if ( pos_x < 0  || pos_y < 0  ) {
        printf("Erreur : Position joueur en paramètre non valide dans save_game()\n");
        return 1;
    }

    if ( perso == NULL ) {
        printf("Erreur : perso en paramètre non valide dans save_game()\n");
        return 1;
    }

    if ( inventaire == NULL ) {
        printf("Erreur : inventaire en paramètre non valide dans save_game()\n");
        return 1;
    }

    if ( liste_objet == NULL ) {
        printf("Erreur : liste_objet en paramètre non valide dans save_game()\n");
        return 1;
    }
    
    if ( lastQuest < 0 ) {
        printf("Erreur : lastQuest en paramètre non valide dans save_game()\n");
        return 1;
    }

    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    char* time_str = malloc(sizeof(char)*20); // allocate memory for string
    strftime(time_str, 20, "%d-%m-%Y %Hh%M", tm_struct);

    char nomFichier[50];
    sprintf(nomFichier, "asset/save/auto-save-%s", time_str);

    if ( strlen(nomFichier) == 0 ) {
        printf("Erreur : nomFichier non valide dans save_game()\n");
        return 1;
    }


    FILE * filename = fopen(nomFichier, "w");
    if ( filename == NULL ) {
        printf("Erreur : Echec fopen(%s) dans save_game()\n",nomFichier);
        return 1;
    }

    fprintf(filename, "%d:%d:\n",pos_x,pos_y);
    fprintf(filename, "%d:%d:%d:\n",perso->niveau,perso->exp,perso->pts_upgrade);
    fprintf(filename, "%d:%d:%d:%d:\n",perso->caract->pv, perso->caract->maxPv, perso->caract->def, perso->caract->atk);
    fprintf(filename, "%d:%d:\n",inventaire->invHeight,inventaire->invWidth);
    for (int i=0;i<inventaire->invHeight;i++){
        for (int j=0;j<inventaire->invWidth;j++){
            fprintf(filename, "%d:",inventaire->inventaire[i][j]);
        }
        fprintf(filename, "\n");
    }
    fprintf(filename,"%d:%d:\n",inventaire->equipementHeight,inventaire->equipementWidth);
    for (int i=0;i<inventaire->equipementHeight;i++){
        for (int j=0;j<inventaire->equipementWidth;j++){
            fprintf(filename, "%d:",inventaire->equipement[i][j]);
        }
        fprintf(filename, "\n");
    }
    fprintf(filename,"%d:\n",liste_objet->nbElem);
    for(int i=0;i<liste_objet->nbElem;i++){
        fprintf(filename,"%d:",liste_objet->tab[i]->nb);
    }
    fprintf(filename,"\n");
    fprintf(filename,"%d:",lastQuest);

    fclose(filename);

    return 0;
}

extern int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet,int * lastQuest, char * nomFichier) {
    // Verification parametre
    if ( pos_x == NULL || pos_y == NULL ) {
        printf("Erreur : Position joueur en paramètre non valide dans load_game()\n");
        return 1;
    }

    if ( perso == NULL ) {
        printf("Erreur : perso en paramètre non valide dans load_game()\n");
        return 1;
    }

    if ( inventaire == NULL ) {
        printf("Erreur : inventaire en paramètre non valide dans load_game()\n");
        return 1;
    }

    if ( liste_objet == NULL ) {
        printf("Erreur : liste_objet en paramètre non valide dans load_game()\n");
        return 1;
    }
    
    if ( lastQuest == NULL ) {
        printf("Erreur : lastQuest en paramètre non valide dans load_game()\n");
        return 1;
    }

    if ( strlen(nomFichier) == 0 ) {
        printf("Erreur : nomFichier en paramètre non valide dans load_game()\n");
        return 1;
    }

    FILE * filename = fopen(nomFichier, "r");
    if ( filename == NULL ) {
        printf("Erreur : Echec fopen(%s) dans load_game()\n",nomFichier);
        return 1;
    }

    fscanf(filename, "%d:%d:\n",pos_x,pos_y);
    fscanf(filename, "%d:%d:%d:\n",&(perso->niveau),&(perso->exp),&(perso->pts_upgrade));
    fscanf(filename, "%d:%d:%d:%d:\n",&(perso->caract->pv), &(perso->caract->maxPv), &(perso->caract->def), &(perso->caract->atk));
    fscanf(filename, "%d:%d:\n",&(inventaire->invHeight),(&inventaire->invWidth));
    for (int i=0;i<inventaire->invHeight;i++){
        for (int j=0;j<inventaire->invWidth;j++){
            fscanf(filename, "%d:",&(inventaire->inventaire[i][j]));
        }
        fscanf(filename, "\n");
    }
    fscanf(filename,"%d:%d:\n",&(inventaire->equipementHeight),&(inventaire->equipementWidth));
    for (int i=0;i<inventaire->equipementHeight;i++){
        for (int j=0;j<inventaire->equipementWidth;j++){
            fscanf(filename, "%d:",&(inventaire->equipement[i][j]));
        }
        fscanf(filename, "\n");
    }
    fscanf(filename,"%d:\n",&(liste_objet->nbElem));
    for(int i=0;i<liste_objet->nbElem;i++){
        fscanf(filename,"%d:",&(liste_objet->tab[i]->nb));
    }
    fscanf(filename,"\n");
    fscanf(filename,"%d:",lastQuest);

    fclose(filename);

    return 0;
}

