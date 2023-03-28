#ifndef PNJ_H
#define PNJ_H


/**
 *	\file pnj.h
 *  \brief Header gestion des personnages non joueurs
 *  \author Sardon William
 *  \version 2.0
 *  \date 3/03/2023
 */

#include <string.h>

typedef struct type_pnj_s {
    char * nom;
    char * imageDialogue;
    char * dialogue;

}type_pnj_t;

typedef struct liste_type_pnj_t {
    type_pnj_t ** liste;
    int nbElem;
}liste_type_pnj_t;

typedef struct pnj_s{
    // Definition pnj
    int pnjTypeID;

    // Quete ( si quete ratachee a ce pnj )
    //quete_t

    // position haut gauche du pnj dans la matrice
    int pos_y;
    int pos_x;
}pnj_t;


extern liste_type_pnj_t * Load_Liste_Type_Pnj(char * chemin_fichier) ;
extern void Detruire_Liste_Type_Pnj( liste_type_pnj_t ** liste_type  ) ;

extern int Afficher_Liste_Type_Pnj(liste_type_pnj_t * liste_type);

extern pnj_t * creer_pnj(int ID, int y, int x, liste_type_pnj_t * liste_type ) ;

extern void supprimer_pnj(pnj_t ** pnj);

extern void afficher_pnj(pnj_t * pnj);

#endif