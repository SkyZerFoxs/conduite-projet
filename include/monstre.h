#ifndef MONSTRE_H
#define MONSTRE_H

#include <caract.h>
#include <string.h>

/**
 *	\file monstre.h
 *  \brief Header pour gérer les monstres
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

typedef struct monstre_s{
    // Nom monstre
    char * nom;

    // Data monstre
    int niveau;
    caract_t * caract;

    // Position Monstre
    int pos_y;
    int pos_x;

    // Taile monstre ( 1 x 1 || 2 x 1)
    int monstreSize;
    
}monstre_t;

extern monstre_t * creer_monstre(char * nom, int niv, int y, int x) ;
extern int afficher_monstre(monstre_t * monstre);
extern void supprimer_monstre(monstre_t ** monstre);

#endif