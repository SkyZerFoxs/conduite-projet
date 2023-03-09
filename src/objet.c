/**
 *	\file objets.c
 *  \brief fonction pour gérer les objets
 *  \author Raccouard Nathan et William Sardon Arraz
 *  \version 1.1
 *  \date 09/03/2023
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objet.h>

objet_t * creer_objet(int id,int pv, int atk,int def,int puissance,int mana,int force, int prix_achat,int prix_vente){
    objet_t * objet=NULL;
    objet=malloc(sizeof(objet_t)+1);
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

extern
void afficher_objet(objet_t * objet){ 
    if(objet!=NULL){
        FILE * fobjet;
        //Ouvre objet.txt ou est indiqué les informations à afficher
        fobjet = fopen("asset/objet.txt","r");
        if(fobjet == NULL)
            printf("Erreur à l'ouverture du fichier 'objet.txt'.\n");
        else{
            //Ligne de lecture courante
            char line[MAX_CHAR];
            fgets(line, MAX_CHAR, fobjet);
            while(!feof(fobjet)){
                if (atoi(&line[0]) == objet->id){
                    
                    fgets(line, MAX_CHAR, fobjet);
                    printf("\nNom : %s",line);
                }
                else{
                    fgets(line, MAX_CHAR, fobjet);
                }
                fgets(line, MAX_CHAR, fobjet);
            }
        }
        printf("Id : %d\n", objet->id);
        printf("Prix achat: %d\n", objet->prix_achat);
        printf("Prix vente : %d\n", objet->prix_vente);
        fclose(fobjet);
    }
}

extern
void supprimer_objet(objet_t ** objet){
    free((*objet)->stats);
    (*objet)->stats=NULL;
    free(*objet);
    (*objet)=NULL;  
}