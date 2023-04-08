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
#include <quete.h>

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef type_pnj_s type_pnj_t Structure type de pnj
 * \struct type_pnj_t
 * \brief Structure du type de pnj ( Data )
 * 
 * Contient:
 * \n char * nom, nom ( standard ) du pnj
 * \n char * imageDialogue, chemin vers l'image de dialogue du pnj
 * \n char * dialogue, dialogue ( standard ) du pnj
*/
typedef struct type_pnj_s {
    char * nom;
    char * imageDialogue;
    char * dialogue;

}type_pnj_t;

/**
 * \typedef liste_type_pnj_s liste_type_pnj_t Structure liste type de pnj
 * \struct liste_type_pnj_t
 * \brief Structure de la liste des types de pnj ( Data )
 * 
 * Contient:
 * \n type_pnj_t ** liste,  liste des différents type de pnj
 * \n int nbElem, nombre d'élements dans la liste
*/
typedef struct liste_type_pnj_s {
    type_pnj_t ** liste;
    int nbElem;
}liste_type_pnj_t;

/**
 * \typedef pnj_s pnj_t Structure pnj
 * \struct pnj_t
 * \brief Structure d'un pnj ( Data )
 * 
 * Contient:
 * \n int pnjTypeID, ID du type de pnj
 * \n quete_t * quete, pointeur sur la quete assigné au pnj ( si une quete lui est attribué )
 * \n int pos_y, position y du pnj dans la map
 * \n int pos_x, position x du pnj dans la map
*/
typedef struct pnj_s{
    // Definition pnj
    int pnjTypeID;

    // Quete ( si quete ratachee a ce pnj )
    quete_t * quete;

    // position haut gauche du pnj dans la matrice
    int pos_y;
    int pos_x;
}pnj_t;


/* ------------------------------------------------------ */
/* ------------------ Fonction Extern  ------------------ */
/* ------------------------------------------------------ */

extern liste_type_pnj_t * Load_Liste_Type_Pnj(char * chemin_fichier) ;
extern void Detruire_Liste_Type_Pnj( liste_type_pnj_t ** liste_type  ) ;

extern int Afficher_Liste_Type_Pnj(liste_type_pnj_t * liste_type);

extern pnj_t * creer_pnj(int ID, int y, int x, liste_type_pnj_t * liste_type ) ;

extern void supprimer_pnj(pnj_t ** pnj);

extern void afficher_pnj(pnj_t * pnj);

#endif