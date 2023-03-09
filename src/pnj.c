/**
 *	\file pnj.c
 *  \brief fonctions de gestion des personnages non joueurs
 *  \author Sardon William
 *  \version 2.2
 *  \date 3/03/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pnj.h"


/**
 * \fn int existe_pnj(pnj_t * pnj)
 * \brief Vérifie si un pnj existe deja
 * \param pnj Pointeur sur le pnj à téster
 * \return FAUX si le pnj n'existe pas, VRAI sinon
**/
int existe_pnj(pnj_t * pnj){
    if(pnj == NULL)
        return(0);
    else
        return(1);
}

/**
 * \fn pnj_t * creer_pnj(int id_pnj)
 * \brief Création d'un pnj avec l'id donné en paramètre
 * \param id_pnj Id référant au pnj à créer
 * \return Pointeur sur le pnj créer
**/
pnj_t * creer_pnj(int id_pnj){
    pnj_t * pnj = malloc(sizeof(pnj_t));

    pnj->id_pnj = id_pnj;

    return pnj;
}

/**
 * \fn void supprimer_pnj(pnj_t ** pnj)
 * \brief Suppression d'un pnj
 * \param pnj Pointeur de pointeur sur le pnj à supprimer
**/
void supprimer_pnj(pnj_t ** pnj){
    free((*pnj));
    (*pnj) = NULL; 
}

/**
 * \fn void afficher_pnj(pnj_t * pnj)
 * \brief Affiche les caractéristiques du pnj grace au fichier pnj.txt
 * \param pnj Pointeur sur le pnj à afficher
**/
void afficher_pnj(pnj_t * pnj){
    if(existe_pnj(pnj)){
        FILE * fpnj;

        //Ouvre pnj.txt ou est indiqué les informations à afficher
        fpnj = fopen("asset/pnj.txt","r");

        if(fpnj == NULL)
            printf("Erreur à l'ouverture du fichier 'pnj.txt'.\n");
        
        else{

            //Ligne de lecture courante
            char line[MAX_CHAR];

            fgets(line, MAX_CHAR, fpnj);
            while(!feof(fpnj)){
                if (atoi(&line[0]) == pnj->id_pnj){
                    printf("Pnj --"); 
                    printf("id : %s",line);

                    fgets(line, MAX_CHAR, fpnj);
                    printf("Nom : %s",line);

                    fgets(line, MAX_CHAR, fpnj);
                    printf("Role : %s",line);

                    printf("\n");
                }
                else{
                    fgets(line, MAX_CHAR, fpnj);
                    fgets(line, MAX_CHAR, fpnj);
                }
                fgets(line, MAX_CHAR, fpnj);
            }
        }
        fclose(fpnj);
    }
    else{
        printf("Ce pnj n'existe pas\n");
    }
}

