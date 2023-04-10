#ifndef COMBAT_H
#define COMBAT_H

#include <personnage.h>
#include <monstre.h>

/**
 *	\file combat.h
 *  \brief Header pour gérer les combats
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 27/02/2023
**/

/* ------------------------------------------------------ */
/* ------------------ Fonction Extern  ------------------ */
/* ------------------------------------------------------ */

extern int combat_joueur(personnage_t* personnage, monstre_t* monstre, int typeAttaque) ;
extern int combat_monstre(monstre_t* monstre, personnage_t* personnage, int distance) ;

#endif