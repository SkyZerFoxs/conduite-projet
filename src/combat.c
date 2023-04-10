#include <stdio.h>
#include <stdlib.h>

#include <combat.h>

/**
 *	\file combat.c
 *  \brief fonction pour gérer les combats
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 27/02/2023
**/


/**
 * \fn int combat_joueur(personnage_t * personnage, monstre_t * monstre, int typeAttaque)
 * \brief Calcul les dégats reçus par le monstre et enlève les points de vie correpondants
 * 
 * \param perso pointeur sur personnage du jeu
 * \param monstre pointeur sur monstre qu'on combat
 * \param typeAttaque atk choisie par le joueur
 * \return 0 Success || 1 Fail ( statut fonction )
**/
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
            damage = (caractSortie.atk * 1.5 + 20) - (monstre->caract->def / 2);
            break;
        case 3:
            // Attaque ultime
            damage = (caractSortie.atk * 2.5 + 30) - (monstre->caract->def / 2);
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


/**
 * \fn int combat_monstre(monstre_t* monstre, personnage_t* personnage, int distance)
 * \brief Calcul les dégats reçus par le joueur et enlève les points de vie correspondants
 * 
 * \param monstre pointeur sur monstre qui combat
 * \param perso pointeur sur personnage du jeu
 * \param distance distance entre le joueur et le monstre
 * \return 0 Success || 1 Fail ( statut fonction )
**/
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