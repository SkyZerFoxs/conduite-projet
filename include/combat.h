#ifndef COMBAT_H
#define COMBAT_H

#include <personnage.h>
#include <monstre.h>

/**
 *	\file combat.h
 *  \brief Header pour gérer les combats
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

extern void combat_joueur(personnage_t * perso, monstre_t * monstre, int atk);
extern void combat_monstre(monstre_t * monstre, personnage_t * perso);

#endif