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


/*

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

*/

// Fonction d'attaque du joueur sur un monstre
extern
int combat_joueur(personnage_t * personnage, monstre_t * monstre, int typeAttaque) {
    if ( personnage == NULL ) {
        printf("Erreur : personnage en parametre invalide dans combat_joueur()\n");
        return 1;
    }

    if ( monstre == NULL ) {
        printf("Erreur : monstre en parametre invalide dans combat_joueur()\n");
        return 1;
    }

    caract_t caractSortie = { 0, 0, 0, 0};
    if ( calculer_stats_perso(personnage, &caractSortie) ) {
        printf("Erreur : Echec calculer_stats_perso() dans combat_joueur()\n");
        return 1;
    }

    int damage = 0;

    switch (typeAttaque ) {
        case 1 :
            // Attaque normale
            damage = (caractSortie.atk + 10) - (monstre->caract->def / 2);
            break;
        case 2:
            // Attaque spéciale
            damage = (caractSortie.atk * 2 + 20) - (monstre->caract->def / 2);
            break;
        case 3:
            // Attaque ultime
            damage = (caractSortie.atk * 5 + 50) - (monstre->caract->def / 2);
            break;
        default :
            printf("Erreur : typeAttaque en parametre invalide dans combat_joueur()\n");
            return 1;
            break;
    }

    if ( damage > 0 ) {
        monstre->caract->pv -= damage;
    }

    return 0;
    
}   


// Fonction d'attaque du monstre sur le joueur
extern
int combat_monstre(monstre_t* monstre, personnage_t* personnage, int distance) {

    if ( monstre == NULL ) {
        printf("Erreur : monstre en parametre invalide dans combat_monstre()\n");
        return -1;
    }

    if ( personnage == NULL ) {
        printf("Erreur : personnage en parametre invalide dans combat_monstre()\n");
        return -1;
    }

    if ( distance < 1 || distance > 3 ) {
        printf("Erreur : distance en parametre invalide dans combat_monstre()\n");
        return -1;
    }

    int damage = 0;

    // Calcule la chance de toucher le joueur
    int hitChance = 60 - distance * 10;
    if (rand() % 100 < hitChance) {
        // Le monstre a touché le joueur
        damage = (monstre->caract->atk + 5) - (personnage->caract->def / 2);

        if ( damage > 0 ) {
            personnage->caract->pv -= damage;
            return 1;
        }
    }
    
    return 0;
}


