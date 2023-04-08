#ifndef CARACT_H
#define CARACT_H

/**
 *	\file caract.h
 *  \brief Header pour gérer les caractéristique
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 9/02/2023
**/

/* ------------------------------------------------ */
/* ------------------ Structure  ------------------ */
/* ------------------------------------------------ */

/**
 * \typedef caract_s caract_t Structure caracteristique
 * \struct caract_t
 * \brief Structure des caracteristiques ( Data )
 * 
 * Contient:
 * \n int pv, statistique des points de vie
 * \n int maxPv, statistique des points de vie maximum
 * \n int atk, statistique des points d'attaque
 * \n int def, statistique des points de defense
*/
typedef struct caract_s{
    int pv;
    int maxPv;
    int atk;
    int def;
}caract_t;

#endif