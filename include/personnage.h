#ifndef PERSO_H
#define PERSO_H

#include <caract.h>
#include <string.h>

#define UPGRADE_MAX_PV 10
#define UPGRADE_ATK 1
#define UPGRADE_DEF 1

/**
 *	\file personnage.h
 *  \brief Header pour gérer les personnage
 *  \author Raccouard Nathan
 *  \version 2.0
 *  \date 27/02/2023
**/

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef personnage_s personnage_t Structure personnage
 * \struct personnage_t
 * \brief Structure du personnage ( Data )
 * 
 * Contient:
 * \n char * nom, nom du personnage
 * \n int niveau, niveau du personnage
 * \n int exp, points d'experience du personnage
 * \n int palierExp, palier pour atteindre le prochain niveau
 * \n int pts_upgrade, points disponible pour améliorer une statistique
 * \n caract_t * caract, strcuture caract_t qui contient les statistique du personnage
 * \n caract_t * equipement[6], tableau de strcuture caract_t qui contient les statistique de chaque piece d'equipements
*/
typedef struct personnage_s{
    char * nom;

    int niveau;
    int exp;
    int palierExp;

    int pts_upgrade;
    
    // Stats du perso
    caract_t * caract;

    // Stat bonus de l'equipement
    caract_t * equipement[6];

}personnage_t;

/* ------------------------------------------------------ */
/* ------------------ Fonction Extern  ------------------ */
/* ------------------------------------------------------ */

extern personnage_t * creer_personnage(char * nom);
extern void supprimer_perso(personnage_t ** perso);

extern void afficher_perso(personnage_t * perso);

extern int calculer_stats_perso(personnage_t * perso, caract_t * caractSortie ) ;

extern int upgrade_perso(personnage_t * perso, int stats) ;

#endif