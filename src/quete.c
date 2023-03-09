/**
 *	\file quete.c
 *  \brief fonctions de gestion des quetes
 *  \author Sardon William
 *  \version 2.2
 *  \date 03/03/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quete.h"

/**
 * \fn int existe_quete(quete_t * quete)
 * \brief Vérifie si une quete existe
 * \param quete Pointeur sur la quete à téster
 * \return Un 0 si la quete n'existe pas ou 1 sinon
**/
int existe_quete(quete_t * quete){
    if(quete == NULL)
        return(0);
    else
        return(1);
}

/**
 * \fn quete_t * creer_quete(int id_quete)
 * \brief Création d'une quete avec l'id donné en paramètre et initialisée à un etat de 0 (En cours)
 * \param id_quete Id référant à la quete à créer
 * \return Un pointeur sur la quete créée
**/
quete_t * creer_quete(int id_quete){
    quete_t * quete = malloc(sizeof(quete_t));
    quete->id_quete = id_quete;
    quete->etat = 0;

    return quete;
}

/**
 * \fn void supprimer_quete(quete_t ** quete)
 * \brief Suppression d'une quete
 * \param quete Pointeur de pointeur sur la quete a supprimer
**/
void supprimer_quete(quete_t ** quete){
    free((*quete));
    (*quete) = NULL; 
}

/**
 * \fn quete_t * valider_quete(quete_t ** quete)
 * \brief Modifie l'entier 'etat' dans la structure quete sur 1 pour indiquer qu'elle est terminée
 * \param quete Pointeur sur la quete à valider
**/
void valider_quete(quete_t ** quete){
    (*quete)->etat = 1;
}

/**
 * \fn void afficher_quete(quete_t * quete)
 * \brief Affiche les caractéristiques de la quete
 * \param quete Pointeur sur la quete à afficher
**/
void afficher_quete(quete_t * quete){
    //Test si la quete existe
    if(existe_quete(quete)){ 
        //Condition d'arret
        int stop=0;

        //Ouvre quete.txt ou est indiqué les informations à afficher
        FILE * fquete;
        fquete = fopen("asset/quete.txt","r"); 

        //Test si le fichier quete.txt existe
        if(fquete == NULL)
            printf("Erreur à l'ouverture du fichier 'quete.txt'.\n");

        else{
            //Ligne de lecture courante
            char line[MAX_CHAR];

            //Lecture de la premiere ligne
            fgets(line, MAX_CHAR, fquete);
            while(!feof(fquete) && (stop==0)){
                //Verification de l'id de la quete pour la lecture
                if (atoi(&line[0]) == quete->id_quete){

                    printf("Quete -- \n");
                    printf("Id : %s",line);

                    fgets(line, MAX_CHAR, fquete);
                    printf("Nom : %s",line);

                    fgets(line, MAX_CHAR, fquete);
                    printf("Client : %s",line);

                    if(quete->etat == 0)
                        printf("Etat :  En cours\n");
                    else
                        printf("Etat :  Finis\n");
                    
                    fgets(line, MAX_CHAR, fquete);
                    printf("Description : %s",line);

                    fgets(line, MAX_CHAR, fquete);
                    printf("Recompense : %s",line);

                    printf("\n");

                    stop = 1;
                }
                fgets(line, MAX_CHAR, fquete);
            }
        }
        fclose(fquete);
        fquete = NULL;
    }
    else{
        printf("Cette quete n'existe pas\n");
    }
}
