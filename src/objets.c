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

objet_t * creer_objet(char * nom, int id,int pv, int atk,int def,int puissance,int mana,int force, int prix_achat,int prix_vente){
    objet_t * objet=NULL;
    objet=malloc(sizeof(objet_t)+1);
    objet->nom=malloc(sizeof(char)*strlen(nom)+1);
    strcpy(objet->nom,nom);
    objet->stats=malloc(sizeof(caract_t)+1);
    objet->prix_achat=prix_achat;
    objet->prix_vente=prix_vente;
    objet->id=id;
    objet->stats->pv=pv;
    objet->stats->atk=atk;
    objet->stats->def=def;
    objet->stats->puissance=puissance;
    objet->stats->mana=mana;
    return objet;
}

extern
void afficher_objet(objet_t * objet){ 
    printf("Nom : %s\n",objet->nom);
    printf("Id : %d\n", objet->id);
    printf("Prix achat: %d\n", objet->prix_achat);
    printf("Prix vente : %d\n", objet->prix_vente);
}

extern
void supprimer_objet(objet_t ** objet){
    free((*objet)->nom);
    (*objet)->nom=NULL;
    free((*objet)->stats);
    (*objet)->stats=NULL;
    free(*objet);
    (*objet)=NULL;  
}