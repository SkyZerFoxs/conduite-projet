#ifndef OBJETS_H
#define OBJETS_H

#include <caract.h>
#include <stdlib.h>
#include <string.h>

/**
 *	\file objets.h
 *  \brief Header pour gérer les objets
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 28/02/2023
**/

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef objet_s objet_t Structure objet
 * \struct objet_t
 * \brief Structure d'un objet ( Data )
 * 
 * Contient:
 * \n id, id de l'objet
 * \n char * nom, nom de l'objet
 * \n int niveau, niveau de l'objet
 * \n int typeID, id du type de l'objet
 * \n caract_t * stats, strcuture caract_t qui contient les statistique bonus de l'objet
 * \n int nb, quantité de l'objet
 * \n int prix_achat, prix d'achat de l'objet
 * \n int prix_vente, prix de vente de l'objet
*/
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

/**
 * \typedef liste_objet_s liste_objet_t Structure liste objets
 * \struct liste_objet_t
 * \brief Structure de la liste des objets ( Data )
 * 
 * Contient:
 * \n objet_t ** tab, tableau / liste des différents objets
 * \n int nbElem, nombre d'élements dans le tableau
*/
typedef struct liste_objet_s {
	objet_t ** tab;
	int nbElem;
} liste_objet_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Extern  ------------------ */
/* ------------------------------------------------------ */

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