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
personnage_t * creer_personnage(char * nom){
    personnage_t * perso=NULL;
    perso=malloc(sizeof(personnage_t)+1);
    perso->nom=malloc(sizeof(char)*strlen(nom)+1);
    strcpy(perso->nom,nom);
    perso->caract=malloc(sizeof(caract_t)+1);
    perso->niveau=1;
    perso->pts_upgrade=0;
    perso->caract->pv=100;
    perso->caract->atk=20;
    perso->caract->def=10;

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
        perso->pts_upgrade--;
    }while(stat_upgrade!=0 && perso->pts_upgrade!=0);
}



