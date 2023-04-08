#ifndef QUETE_H
#define QUETE_H

/**
 *	\file quete.h
 *  \brief Header Gestion Quete
 *  \author Yamis MANFALOTI
 *  \version 1.0
 *  \date 25/02/2023
**/

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */


typedef struct quete_s quete_t;

/**
 * \typedef quete_s quete_t Structure quete
 * \struct quete_t
 * \brief Structure des quetes ( Data )
 * 
 * Contient:
 * \n id, id de la quete
 * \n char * objectifQuete, texte description objectif de la quete
 * \n char * dialogueQuetePnj, texte du dialogue d'un pnj associé a la quete
 * \n quete_t * avant, pointeur sur la quete_t qui est avant
 * \n quete_t * apres, pointeur sur la quete_t qui est après
*/
typedef struct quete_s {
    // Information quete
    int id;
    char * objectifQuete;

    // Dialogue du pnj durant la quete
    char * dialogueQuetePnj;

    // Element Liste Quete
    quete_t * avant;
    quete_t * apres;

}quete_t;

/* ------------------------------------------------------ */
/* ------------------ Fonction Externe ------------------ */
/* ------------------------------------------------------ */

extern quete_t * Load_Quete(const char * nom_fichier) ;

#endif