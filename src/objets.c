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
    objet->stats->force=force;
    return objet;
}



int main(){
    printf("fneuzifhez\n");
    objet_t * plastron=creer_objet("plastron",1,50,0,100,0,0,0,50,25);
    objet_t * epee=creer_objet("epee",2,0,20,0,0,0,50,10,5);
    objet_t * sceptre=creer_objet("sceptre",3,0,0,0,50,20,0,20,10);
    printf("%s\n",plastron->nom);
}

