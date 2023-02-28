/**
 *	\file pnj.c
 *  \brief fonctions de gestion des personnages non joueurs
 *  \author Sardon William
 *  \version 2.0
 *  \date 10/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pnj.h"


/**
 * \fn int existe_pnj(pnj_t ** pnj)
 * \brief verifie si un pnj existe deja
 * \param pnj
 * \return FAUX si le pnj n'existe pas, VRAI sinon
**/
int existe_pnj(pnj_t * pnj){
    if(pnj == NULL)
        return(0);
    else
        return(1);
}

/**
 * \fn pnj_t * creer_pnj(char * nom, int num_role)
 * \brief création d'un pnj
 * \param id_pnj
 * \return Pointeur sur le pnj créer
**/
pnj_t * creer_pnj(int id_pnj){
    pnj_t * pnj = malloc(sizeof(pnj_t));

    pnj->id_pnj=id_pnj;

    return pnj;
}

/**
 * \fn void supprimer_pnj(pnj_t ** pnj)
 * \brief suppression d'un pnj
 * \param pnj Pointeur de pointeur sur le pnj à supprimer
**/
void supprimer_pnj(pnj_t * pnj){
    free(pnj);
    pnj = NULL; 
}

/**
 * \fn void afficher_pnj(pnj_t * pnj)
 * \brief affiche les caractéristiques du pnj
 * \param pnj Pointeur sur le pnj à afficher
**/
void afficher_pnj(pnj_t * pnj){
    if(existe_pnj(pnj)){
        FILE * fpnj;
        //ouvre pnj.txt ou est indiqué les informations à afficher
        fpnj = fopen("pnj.txt","r");

        if(fpnj == NULL)
            printf("Erreur à l'ouverture du fichier 'pnj.txt'.\n");
        
        else{
            //ligne courante
            char line[50];

            fscanf(fpnj,"%s",line);
            while(!feof(fpnj)){
                if (line[0] == pnj->id_pnj){
                    printf("Pnj --\n"); 
                    printf("id : %d\n",pnj->id_pnj);

                    fscanf(fpnj,"%s",line);
                    printf("Nom : %s\n",line);

                    fscanf(fpnj,"%s",line);
                    printf("Role : %s\n",line);
                }
                else{
                    fscanf(fpnj,"%s",line);
                    fscanf(fpnj,"%s",line);
                }
                fscanf(fpnj,"%s",line);
            }
        }
        fclose(fpnj);
    }
    else{
        printf("Ce pnj n'existe pas\n");
    }
}

