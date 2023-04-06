#ifndef OBJETS_H
#define OBJETS_H

#include <caract.h>
#include <stdlib.h>
#include <string.h>

/**
 *	\file objets.h
 *  \brief Header pour gérer les objets
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 10/02/2023
**/

typedef struct objet_s{
    // Données Objet 
    int id;
    char * nom;

    // Statistique en jeu
    int niv;
    int typeID;
    caract_t * stats;
    int nb;

    // Données trading
    int prix_achat;
    int prix_vente;
}objet_t;

typedef struct liste_objet_s {
	objet_t ** tab;
	int nbElem;
} liste_objet_t;

// Gestion Liste Objet
liste_objet_t * load_liste_objets(char * nom_fich) ;

// Gestion Objet
extern void detruire_objet(objet_t ** objet);
extern void detruire_liste_objet(liste_objet_t ** liste);

extern int afficher_liste_objet(liste_objet_t * liste);

// Gestion Loot
extern int loot_monstre(int niv_monstre) ;
extern int loot_coffre(int niv_coffre) ;

#endif