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
    char * nom;
    int niveau;
    int pos_y;
    int pos_x;
    /*
    int pv;
    int atk;
    int def;
    */
    caract_t * caract;
}monstre_t;

extern monstre_t * creer_monstre(char * nom, int niv, int y, int x) ;
extern void afficher_monstre(monstre_t * monstre);
extern void supprimer_monstre(monstre_t ** monstre);

#endif