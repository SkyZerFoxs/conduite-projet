#include <stdio.h>
#include <stdlib.h>

#include <combat.h>

/**
 *	\file combat.c
 *  \brief fonction pour gérer les combats
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

extern
void combat_joueur(personnage_t * perso, monstre_t * monstre, int atk) {
    if ( perso == NULL ) {
        printf("Erreur : perso Inexistant dans combat_joueur()\n");
        return;
    }

    if ( monstre == NULL ) {
        printf("Erreur : monstre Inexistant dans combat_joueur()\n");
        return;
    }

    int degats = 0;
    switch ( atk ) {
    case 0: 
        degats = perso->caract->atk - monstre->caract->def;
        break;
    case 1: 
        degats = ( 3 * perso->caract->atk ) - monstre->caract->def;
        break;
    case 2: 
        degats = ( 5 * perso->caract->atk ) - monstre->caract->def;
        break;
    default:
        printf("Erreur : Atk incorrecte dans combat_joueur()\n");
        return;
        break;
    }

    if ( degats > 0 ) {
        monstre->caract->pv -= degats;
    }

}

extern 
void combat_monstre(monstre_t * monstre, personnage_t * perso) {
    if ( perso == NULL ) {
        printf("Erreur : perso Inexistant dans combat_monstre()\n");
        return;
    }

    if ( monstre == NULL ) {
        printf("Erreur : monstre Inexistant dans combat_monstre()\n");
        return;
    }

    int degats = monstre->caract->atk - perso->caract->def ;
    if ( degats > 0 ) {
        perso->caract->pv -= degats;
    }

}

