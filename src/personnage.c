#include <stdio.h>
#include <stdlib.h>

#include <personnage.h>

/**
 *	\file personnage.c
 *  \brief fonction pour gérer les personnages
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

/**
 * \fn personnage_t * creer_personnage(char * nom, int num_classe)
 * \brief création d'un personnage
 * \param nom du personnage et le numéro de sa classe
 * \return personnage_t perso
**/
extern
personnage_t * creer_personnage(char * nom){
    personnage_t * perso=malloc(sizeof(personnage_t)+1);
    if ( perso == NULL ) {
        printf("Erreur : Echec malloc(perso) dans creer_personnage()");
        return NULL;
    }

    perso->nom=malloc(sizeof(char)*strlen(nom)+1);
    if ( perso->nom == NULL ) {
        printf("Erreur : Echec malloc(perso->nom) dans creer_personnage()");
        return NULL;
    }
    strcpy(perso->nom,nom);

    perso->caract=malloc(sizeof(caract_t)+1);
    if ( perso->caract == NULL ) {
        printf("Erreur : Echec malloc(perso->caract) dans creer_personnage()");
        return NULL;
    }

    perso->niveau=1;
    perso->pts_upgrade=0;

    perso->caract->pv = 100 + ( perso->niveau - 1 ) * 10;
    perso->caract->atk = 15 + ( perso->niveau - 1 ) * 5;
    perso->caract->def = 5 + ( perso->niveau - 1 ) * 2;

    return perso;
}
/**
 * \fn void afficher_perso(personnage_t * perso)
 * \brief Affiche les caractéristiques du personnage (nom, pv,...)
 * \param le personnage à afficher
**/
extern
void afficher_perso(personnage_t * perso){ 
    if ( perso == NULL ) {
        printf("Erreur : perso inexistant dans afficher_perso()");
        return;
    }

    printf("Nom : %s\n",perso->nom);
    printf("lvl: %d\n", perso->niveau);
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
    if ( perso == NULL || (*perso) == NULL ) {
        printf("Erreur : perso inexistant dans supprimer_perso()");
        return;
    }

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
 * \return 0 Success || 1 Fail
**/
extern
int upgrade_perso(personnage_t * perso, int stats) { 
    switch ( stats ) {
        case 0:
            perso->caract->pv += 15;
            break;
        case 1:
            perso->caract->atk += 5;
            break;
        case 2:
            perso->caract->def += 2;
            break;
        default:
            printf("Erreur : Mauvaise stats augmenté dans upgrade_perso()\n");
            return 1;
            break;
    }

    return 0;
}



