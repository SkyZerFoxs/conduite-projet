/**
 *	\file pnj.c
 *  \brief fonctions de gestion des personnages non joueurs
 *  \author Sardon William
 *  \version 1.0
 *  \date 9/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module.h"


/**
 * \fn int existe_pnj(pnj_t ** pnj)
 * \brief verifie si un pnj existe deja
 * \param pnj à verifier
 * \return FAUX si le pnj n'existe pas, VRAI sinon
**/
int existe_pnj(pnj_t * pnj){
    if(pnj == NULL)
        return(FAUX);
    else
        return(VRAI);
}

/**
 * \fn pnj_t * creer_pnj(char * nom, int num_role)
 * \brief création d'un pnj
 * \param nom du pnj et numéro de son role
 * \return pnj_t pnj
**/
pnj_t * creer_pnj(char * nom, int num_role){
    pnj_t * pnj = NULL;

    pnj = malloc(sizeof(pnj_t));
    pnj->nom = malloc(sizeof(char)*strlen(nom)+1);

    strcpy(pnj->nom, nom);
    pnj->num_role=num_role;

    pnj->nb_quete=0; //Initialisation du nb de quete active à 0

    return pnj;
}

/**
 * \fn void supprimer_pnj(pnj_t ** pnj)
 * \brief suppression d'un pnj
 * \param pnj à supprimer
**/
void supprimer_pnj(pnj_t ** pnj){
    free((*pnj)->nom); 
    ((*pnj)->nom) = NULL;

    free((*pnj));
    (*pnj) = NULL; 
}

/**
 * \fn void afficher_pnj(pnj_t * pnj)
 * \brief affiche les caractéristiques du pnj
 * \param pnj à afficher
**/
void afficher_pnj(pnj_t * pnj){
    printf("Pnj --\n"); 
    if(existe_pnj(pnj)) 
        printf("Nom : %s\n",pnj->nom);
        printf("Role : %d\n",pnj->num_role);
        printf("Quete Actives : %i\n",pnj->nb_quete);

    printf("\n");
}

