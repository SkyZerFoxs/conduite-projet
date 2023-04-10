#include <stdio.h>

#include <objets.h>

/**
 * \file test_objet.c
 * \brief Test des fonctionnalités des objets
 * \author Yamis MANFALOTI
 * \version 1.0
 * \date 04 mars 2023
 *
 * Test objets: 
 * \n Chargement de la liste d'objets
 * \n Affichage des informations de la liste d'objets 
 * \n Destruction de la liste d'objets
 * \n 
 */

/**
 * \brief Fonction principale du test
 * 
 * \param void Aucun paramètre en entrée 
 * \return Int qui caractérise le statut de la fonction main
 */
int main(){ 
    // chargement liste objet
    liste_objet_t * listeObjets = load_liste_objets("asset/objet/objet.txt");

    afficher_liste_objet(listeObjets);

    // Detruire liste objets
    detruire_liste_objet(&listeObjets);

    return 0;
}