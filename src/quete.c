/**
 *	\file quete.c
 *  \brief fonctions de gestion des quetes
 *  \author Sardon William
 *  \version 2.0
 *  \date 28/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quete.h"

/**
 * \fn int existe_quete(quete_t ** quete)
 * \brief verifie si une quete existe deja
 * \param quete Pointeur sur quete
 * \return Un entier égal à 0 si la quete n'existe pas ou 1 sinon
**/
int existe_quete(quete_t * quete){
    if(quete == NULL)
        return(0);
    else
        return(1);
}

/**
 * \fn quete_t * creer_quete(char * nom, int * id_pnj, char * description, int recompense)
 * \brief création d'un quete
 * \param id_quete Entier référant à la quete à créer
 * \return Un pointeur sur la quete créée
**/
quete_t * creer_quete(int id_quete){
    quete_t * quete = malloc(sizeof(quete_t));
    quete->id_quete = id_quete;
    quete->etat = 0; //La quete est En cours par defaut a sa creation

    return quete;
}

/**
 * \fn void supprimer_quete(quete_t ** quete)
 * \brief suppression d'une quete
 * \param quete Pointeur de pointeur sur la quete a supprimer
**/
void supprimer_quete(quete_t * quete){
    free(quete);
    quete = NULL; 
}

/**
 * \fn quete_t * valider_quete(quete_t * quete)
 * \brief validation d'une quete
 * \param quete Pointeuir sur la quete à supprimer
**/
void valider_quete(quete_t ** quete){
    (*quete)->etat = 1;
}

/**
 * \fn void afficher_quete(quete_t * quete)
 * \brief affiche les caractéristiques de la quete
 * \param quete Pointeur sur la quete à afficher
**/
void afficher_quete(quete_t * quete){
    //test si la quete existe
    if(existe_quete(quete)){ 
        //condition d'arret
        int stop=0;

        //ouvre quete.txt ou est indiqué les informations à afficher
        FILE * fquete;
        fquete = fopen("quete.txt","r"); 

        //test si le fichier quete.txt existe
        if(fquete == NULL)
            printf("Erreur à l'ouverture du fichier 'quete.txt'.\n");

        else{
            //ligne courante
            char line[50];

            fscanf(fquete, "%s", line);
            while(!feof(fquete) && (stop==0)){
                if (line[0] == quete->id_quete){

                    printf("Quete -- \n");
                    printf("Id : %s\n",line);

                    fscanf(fquete, "%s", line);
                    printf("Nom : %s\n",line);

                    fscanf(fquete, "%s", line);
                    printf("Client : %s\n",line);

                    if(quete->etat == 0)
                        printf("Etat :  En cours\n");
                    else
                        printf("Etat :  Finis\n");
                    
                    fscanf(fquete, "%s", line);
                    printf("Description : %s\n",line);

                    fscanf(fquete, "%s", line);
                    printf("Recompense : %s\n",line);

                    printf("\n");

                    stop = 1;
                }
                fscanf(fquete, "%s", line);
            }
        }
        fclose(fquete);
        fquete = NULL;
    }
    else{
        printf("Cette quete n'existe pas\n");
    }
}
