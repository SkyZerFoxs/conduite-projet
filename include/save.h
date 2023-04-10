#ifndef SAVE_H
#define SAVE_H

#include <hud.h>
#include <time.h>

/**
 *	\file save.h
 *  \brief Header Gestion Sauvegarder & Chargement
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 29/02/2023
**/

/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

//extern int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet) ;
extern int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map) ;

//extern int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, char * nomFichier) ;
extern int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, map_t * map, char * nomFichier);

#endif