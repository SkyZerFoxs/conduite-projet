/**
 *	\file personnage.c
 *  \brief fonction pour gérer les personnages
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <personnage.h>

/**
 * \fn personnage_t * creer_personnage(char * nom, int num_classe)
 * \brief création d'un personnage
 * \param nom du personnage et le numéro de sa classe
 * \return personnage_t perso
**/
extern
personnage_t * creer_personnage(char * nom, int num_classe){
    personnage_t * perso=NULL;
    perso=malloc(sizeof(personnage_t)+1);
    perso->nom=malloc(sizeof(char)*strlen(nom)+1);
    strcpy(perso->nom,nom);
    perso->caract=malloc(sizeof(caract_t)+1);
    perso->base=malloc(sizeof(capacity_t)+1);
    perso->speciale=malloc(sizeof(capacity_t)+1);
    perso->ulti=malloc(sizeof(capacity_t)+1);
    perso->pos_x=10;
    perso->pos_y=10;
    perso->niveau=1;
    perso->pts_upgrade=0;
    perso->classe=num_classe;
    perso->base->apprise=1;
    if(num_classe==1){
        perso->caract->pv=100;
        perso->caract->atk=20;
        perso->caract->def=10;
        perso->caract->puissance=0;
        perso->caract->mana=50;
        perso->base->portee_x=2;
        perso->base->portee_y=2;
        perso->base->degats=100;
    }
    else if(num_classe==2){
        perso->caract->pv=500;
        perso->caract->atk=20;
        perso->caract->def=150;
        perso->caract->puissance=100;
        perso->caract->mana=50;
        perso->base->portee_x=4;
        perso->base->portee_y=4;
        perso->base->degats=100;
    }
    else if(num_classe==3){
        perso->caract->pv=750;
        perso->caract->atk=200;
        perso->caract->def=100;
        perso->caract->puissance=10;
        perso->caract->mana=10;
        perso->base->portee_x=2;
        perso->base->portee_y=2;
        perso->base->degats=200;
    }
    else if(num_classe==4){
        perso->caract->pv=1000;
        perso->caract->atk=100;
        perso->caract->def=350;
        perso->caract->puissance=50;
        perso->caract->mana=10;
        perso->base->portee_x=1;
        perso->base->portee_y=1;
        perso->base->degats=50;
    }
    else if(num_classe==5){
        perso->caract->pv=200;
        perso->caract->atk=100;
        perso->caract->def=100;
        perso->caract->puissance=50;
        perso->caract->mana=20;
        perso->base->portee_x=5;
        perso->base->portee_y=5;
        perso->base->degats=100;
    }
    perso->speciale->apprise=0;
    perso->speciale->portee_x=5;
    perso->speciale->portee_y=5;
    perso->speciale->degats=100;
    perso->ulti->apprise=0;
    perso->ulti->portee_x=5;
    perso->ulti->portee_y=5;
    perso->ulti->degats=100;
    return perso;
}
/**
 * \fn void afficher_perso(personnage_t * perso)
 * \brief Affiche les caractéristiques du personnage (nom, pv,...)
 * \param le personnage à afficher
**/
extern
void afficher_perso(personnage_t * perso){ 
    printf("Nom : %s",perso->nom);
    printf("Points de vie : %d\n", perso->caract->pv);
    printf("Attaque : %d\n", perso->caract->atk);
    printf("Defense : %d\n", perso->caract->def);
    printf("Mana : %d\n", perso->caract->mana);
    printf("Puissance : %d\n", perso->caract->puissance);
    printf("Capacité de base [portée][degats] : %d %d %d\n", perso->base->portee_x,perso->base->portee_x,perso->base->degats);
    if((perso->speciale->apprise)==0){
        printf("Capacité spéciale non apprise\n");
    }
    else{
        printf("Capacité speciale [portée][degats] : %d %d %d\n", perso->speciale->portee_x,perso->speciale->portee_x,perso->speciale->degats);
    }
    if((perso->speciale->apprise)==0){
        printf("Capacité ultime non apprise\n");
    }
    else{
        printf("Capacité ultime [portée][degats] : %d %d %d\n", perso->ulti->portee_x,perso->ulti->portee_x,perso->ulti->degats);
    }
}
/**
 * \fn void supprimer_perso(personnage_t ** perso)
 * \brief suppression d'un personnage de la base de donnée
 * \param nom du personnage à supprimer
**/
extern
void supprimer_perso(personnage_t ** perso){
    free((*perso)->nom);
    (*perso)->nom=NULL;
    free((*perso)->caract);
    (*perso)->caract=NULL;
    free((*perso)->base);
    (*perso)->base=NULL;
    free((*perso)->speciale);
    (*perso)->speciale=NULL;
    free((*perso)->ulti);
    (*perso)->ulti=NULL;
    free(*perso);
    (*perso)=NULL;  
}
/**
 * \fn void upgrade_perso(personnage_t * perso)
 * \brief amélioration d'un personnage
 * \param personnage à améliorer
**/
extern
void upgrade_perso(personnage_t * perso){
    int stat_upgrade;
    printf("Choix amélioration\n");
    printf("1 : Points de vie : %d\n",perso->caract->pv);
    printf("2 : Attaque : %d\n",perso->caract->atk);
    printf("3 : Defense : %d\n",perso->caract->def);
    printf("4 : Puissance : %d\n",perso->caract->puissance);
    printf("5 : Mana : %d\n",perso->caract->mana);
    do{
        printf("Vous avez %d points d'amélioration \n",perso->pts_upgrade);
        printf("Choisir une stat à améliorer \n");
        scanf("%d",&stat_upgrade);
        if(stat_upgrade==1){
            perso->caract->pv+=(perso->caract->pv/10);
            printf("Points de vie : %d\n", perso->caract->pv);
        }
        else if(stat_upgrade==2){
            perso->caract->atk+=(perso->caract->atk/10);
            printf("Attaque : %d\n", perso->caract->atk);
        }
        else if(stat_upgrade==3){
            perso->caract->def+=(perso->caract->def/10);
            printf("Defense : %d\n", perso->caract->def);
        }
        else if(stat_upgrade==4){
            perso->caract->puissance+=(perso->caract->puissance/10);
            printf("Puissance : %d\n", perso->caract->puissance);
        }
        else if(stat_upgrade==5){
            perso->caract->mana+=(perso->caract->mana/10);
            printf("Mana : %d\n", perso->caract->mana);
        }
        perso->pts_upgrade--;
    }while(stat_upgrade!=0 && perso->pts_upgrade!=0);
}



