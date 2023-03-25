/**
 *	\file objets.c
 *  \brief fonction pour gérer les objets
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 10/02/2023
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objets.h>
extern 
void ecrire_objet_tab(char * nom_fich, objet_t ** tab){
    FILE * filename = fopen(nom_fich, "r");
    if (!filename) {
        printf("Erreur d'ouverture du fichier %s\n", nom_fich);
        return;
    }
    for (int i = 0; i < NB_ITEMS; i++) {
        tab[i] = malloc(sizeof(objet_t));
        tab[i]->nom = malloc(sizeof(char) * 25);
        tab[i]->stats = malloc(sizeof(caract_t));
         fscanf(filename, "%d:%[^:]:%d:%d:%d:%d:%d:%d:%d:\n",&tab[i]->id, tab[i]->nom, &tab[i]->niv, &tab[i]->stats->pv,&tab[i]->stats->def, &tab[i]->stats->atk, &tab[i]->prix_achat, &tab[i]->prix_vente, &tab[i]->nb);
    }
    fclose(filename);
}


extern
void afficher_objet(objet_t ** tab){ 
    for (int i=0; i<NB_ITEMS;i++){
        printf("Id : %d\n", tab[i]->id);
        printf("Nom : %s\n",tab[i]->nom);
        printf("Niveau : %d\n", tab[i]->niv);
        printf("PV : %d\n", tab[i]->stats->pv);
        printf("Defense : %d\n", tab[i]->stats->def);
        printf("Attaque : %d\n", tab[i]->stats->atk);
        printf("Prix achat: %d\n", tab[i]->prix_achat);
        printf("Prix vente : %d\n", tab[i]->prix_vente);
        printf("Nombre : %d\n", tab[i]->nb);
    }
}

extern
void detruire_objet(objet_t ** objet){
    if(*objet){
        free((*objet)->nom);
        (*objet)->nom=NULL;
        free((*objet)->stats);
        (*objet)->stats=NULL;

    }
}

void detruire_tab_objet(objet_t **tab) {
    for (int i = 0; i < NB_ITEMS; i++) {
        if (tab[i]) {
            free(tab[i]->nom);
            tab[i]->nom = NULL;
            free(tab[i]->stats);
            tab[i]->stats = NULL;
            free(tab[i]);
            tab[i] = NULL;
        }
    }
    free(tab);
    tab = NULL;
}