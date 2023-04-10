#include <stdio.h>
#include <stdlib.h>

#include <save.h>

/**
 * \fn int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map, int finJeu)
 * \brief Fonction externe qui sauvegarde l'avancé de la partie dans un fichier de sauvegarde
 * 
 * \param pos_x Position en x de la camera du joueur ( implique la postion x du joueur )
 * \param pos_y Position en y de la camera du joueur ( implique la postion y du joueur )
 * \param perso Pointeur sur le personnage_t
 * \param inventaire Pointeur sur l' inventaire_t
 * \param liste_objet Pointeur sur la liste_objet_t
 * \param map Pointeur sur map_t, la map
 * \param finJeu Indice de fin du jeu ( Mort du boss final )
 * \return 0 Success || 1 Fail ( statut fonction )
 * 
*/
extern int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map, int finJeu) {
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

    if ( finJeu < 0 || finJeu > 1) {
        printf("Erreur : finJeu en paramètre non valide dans load_game()\n");
        return 1;
    }


    FILE * filename = fopen("asset/save/auto-save", "w");
    if ( filename == NULL ) {
        printf("Erreur : Echec fopen(asset/save/auto-save) dans save_game()\n");
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
    // Compter nombre de coffre sur la carte
    int nbCoffre = 0;
    for (int y = 0; y < map->height; y++ ) {
        for (int x = 0; x < map->width; x++ ) {
            if ( map->matrice[2][y][x] == ID_COFFRE_TILE || map->matrice[2][y][x] == (ID_COFFRE_TILE+1) ) {
                nbCoffre++;
            }
        }
    }
    fprintf(filename,"%d:\n",nbCoffre);
    for (int y = 0; y < map->height; y++ ) {
        for (int x = 0; x < map->width; x++ ) {
            if ( map->matrice[2][y][x] == ID_COFFRE_TILE ) {
                fprintf(filename,"%d:",0);
            }
            if ( map->matrice[2][y][x] == (ID_COFFRE_TILE+1) ) {
                fprintf(filename,"%d:",1);
            }
        }
    }
    fprintf(filename,"\n");
    // Charger Indice Fin De Jeu ( Boss Mort )
    fprintf(filename,"%d:\n",finJeu);
    fprintf(filename,"\n");

    fclose(filename);

    return 0;
}

/**
 * \fn int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map, int * finJeu, char * nomFichier)
 * \brief Fonction externe qui sauvegarde l'avancé de la partie dans un fichier de sauvegarde
 * 
 * \param pos_x Pointeur vers le int de la position en x de la camera du joueur ( implique la postion x du joueur )
 * \param pos_y Pointeur vers le int de la position en y de la camera du joueur ( implique la postion y du joueur )
 * \param perso Pointeur sur le personnage_t
 * \param inventaire Pointeur sur l' inventaire_t
 * \param liste_objet Pointeur sur la liste_objet_t
 * \param map Pointeur sur map_t, la map
 * \param nomFichier Chemin vers le fichier de sauvegarde
 * \param finJeu Indice de fin du jeu ( Mort du boss final )
 * \return 0 Success || 1 Fail ( statut fonction )
 * 
*/
extern int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map, int * finJeu, char * nomFichier) {
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

    if ( (*finJeu) < 0 || (*finJeu) > 1) {
        printf("Erreur : finJeu en paramètre non valide dans load_game()\n");
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
    // Compter nombre de coffre sur la carte
    int nbCoffre;
    fscanf(filename,"%d:\n",&nbCoffre);
    int * tabCoffre = malloc( sizeof(int) * nbCoffre );
    for (int n = 0; n < nbCoffre; n++ ) {
        fscanf(filename,"%d:",&(tabCoffre[n]));
    }
    fscanf(filename,"\n");
    // Charger coffre
    int i = 0;
    for (int y = 0; y < map->height; y++ ) {
        for (int x = 0; x < map->width; x++ ) {
            if ( map->matrice[2][y][x] == ID_COFFRE_TILE && i < nbCoffre ) {
                map->matrice[2][y][x] += tabCoffre[i++];
            }
        }
    }
    free(tabCoffre);
    tabCoffre = NULL;
    // Charger Indice Fin De Jeu ( Boss Mort )
    fscanf(filename,"%d:\n",finJeu);
    fscanf(filename,"\n");

    fclose(filename);

    return 0;
}

