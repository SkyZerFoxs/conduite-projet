#ifndef MONSTRE_H
#define MONSTRE_H

#include <caract.h>
#include <string.h>

/**
 *	\file monstre.h
 *  \brief Header pour gérer les monstres
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 27/02/2023
**/

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef monstre_s monstre_t Structure Monstre
 * \struct monstre_t
 * \brief Structure des monstres ( Data )
 * 
 * Contient:
 * \n char * nom, nom du monstre
 * \n int niveau, niveau du monstre
 * \n caract_t * caract, strcuture caract_t qui contient les statistique du monstre
 * \n int pos_y, position y du monstre dans la map
 * \n int pos_x, position x du monstre dans la map
 * \n int monstreSize, taille du monstre en nombre de sprite utilisé pour le former
*/
typedef struct monstre_s{
    // Nom monstre
    char * nom;

    // Data monstre
    int niveau;
    caract_t * caract;

    // Position Monstre
    int pos_y;
    int pos_x;

    // Taile monstre
    int monstreSize;
    
}monstre_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Extern  ------------------ */
/* ------------------------------------------------------ */

extern monstre_t * creer_monstre(char * nom, int niv, int y, int x) ;
extern int afficher_monstre(monstre_t * monstre);
extern void supprimer_monstre(monstre_t ** monstre);


#endif