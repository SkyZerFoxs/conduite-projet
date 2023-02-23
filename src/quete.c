/**
 *	\file quete.c
 *  \brief fonctions de gestion des quetes
 *  \author Sardon William
 *  \version 1.0
 *  \date 9/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module.h"


/**
 * \fn int existe_quete(quete_t ** quete)
 * \brief verifie si une quete existe deja
 * \param quete à verifier
 * \return FAUX si la quete n'existe pas, VRAI sinon
**/
int existe_quete(quete_t * quete){
    if(quete == NULL)
        return(FAUX);
    else
        return(VRAI);
}

/**
 * \fn quete_t * creer_quete(pnj_t * pnj, char * description, char * recompense)
 * \brief création d'un quete
 * \param pnj associé à la quete, sa description et sa récompense
 * \return quete_t quete
**/
quete_t * creer_quete(pnj_t * pnj, char * description, char * recompense){
    quete_t * quete = NULL;

    quete = malloc(sizeof(quete_t));
    quete->pnj = malloc(sizeof(pnj_t));
    quete->description = malloc(sizeof(char)*strlen(description)+1);
    quete->recompense = malloc(sizeof(char)*strlen(recompense)+1);

    quete->pnj = pnj;
    quete->pnj->nb_quete++; //Augmente le nombre de quete du pnj associé
    strcpy(quete->description, description);
    strcpy(quete->recompense, recompense);
    quete->etat = FAUX; //La quete est En cours par defaut a sa creation

    return quete;
}

/**
 * \fn void supprimer_quete(quete_t ** quete)
 * \brief suppression d'une quete
 * \param quete à supprimer
**/
void supprimer_quete(quete_t ** quete){
    free((*quete)->pnj); 
    ((*quete)->pnj) = NULL;

    free((*quete)->description); 
    ((*quete)->description) = NULL;

    free((*quete)->recompense); 
    ((*quete)->recompense) = NULL;

    free((*quete));
    (*quete) = NULL; 
}

/**
 * \fn quete_t * valider_quete(quete_t * quete)
 * \brief validation d'une quete
 * \param quete à valider
**/
quete_t * valider_quete(quete_t * quete){
    quete->etat = VRAI;
}

/**
 * \fn void afficher_quete(quete_t * quete)
 * \brief affiche les caractéristiques de la quete
 * \param quete à afficher
**/
void afficher_quete(quete_t * quete){
    printf("Quete --\n"); 
    if(existe_quete(quete)) 
        printf("Client : %s\n",quete->pnj->nom);
        if(quete->etat==FAUX)
            printf("Etat :  En cours\n");
        else
            printf("Etat :  Finis\n");
        printf("Description : %s\n",quete->description);
        printf("Recompense : %s\n",quete->recompense);

    printf("\n");
}
