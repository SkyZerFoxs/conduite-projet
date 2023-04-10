#include <stdio.h>
#include <personnage.h>

/**
 * \file test_perso.c
 * \brief Test des fonctionnalités du personnage
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 04 mars 2023
 *
 * Test personnage: 
 * \n Chargement du personnage
 * \n Affichage des informations du personnage
 * \n Destruction du personnage
 * \n 
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise le statut de la fonction main
 */
int main() {
    personnage_t * perso = creer_personnage("Cody");

    afficher_perso(perso);

    supprimer_perso(&perso);

    return 0;
}