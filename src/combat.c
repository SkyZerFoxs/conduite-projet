/**
 *	\file combat.c
 *  \brief fonction pour gérer les combats
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 10/03/2023
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <personnage.h>
#include <monstre.h>
#include <combat.h>


extern
void combat_joueur(personnage_t * perso, monstre_t * monstre, char atk){
    monstre->pv=monstre->pv-(perso->caract->atk/(0.3*monstre->def));
}

extern 
void combat_monstre(monstre_t * monstre, personnage_t * perso){
    perso->caract->pv-=(monstre->atk/(0.2*perso->caract->def));
}