#ifndef SAVE_H
#define SAVE_H

#include <hud.h>
#include <time.h>

extern int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet) ;

extern int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet, char * nomFichier) ;

#endif