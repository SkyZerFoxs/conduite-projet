#ifndef PERSO_H
#define PERSO_H

#include <caract.h>
#include <string.h>

/**
 *	\file personnage.h
 *  \brief Header pour gérer les personnage
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/


/*
typedef struct capacity_s{
    int apprise;
    int portee_x;
    int portee_y;
    int degats;
}capacity_t;
*/

typedef struct personnage_s{
    char * nom;

    int niveau;
    int exp;
    int palierExp;

    int pts_upgrade;
    
    // Stats du perso
    caract_t * caract;

    // Stat bonus de l'equipement
    caract_t * equipement[6];

}personnage_t;


extern personnage_t * creer_personnage(char * nom);
extern void supprimer_perso(personnage_t ** perso);

extern void afficher_perso(personnage_t * perso);

extern int calculer_stats_perso(personnage_t * perso, caract_t * caractSortie ) ;

extern int upgrade_perso(personnage_t * perso, int stats) ;

#endif