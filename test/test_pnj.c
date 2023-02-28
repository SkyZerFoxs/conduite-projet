/**
 *	\file pnj.c
 *  \brief fonctions de test des pnj
 *  \author Sardon William
 *  \version 2.0
 *  \date 28/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pnj.h"

int main(){

    printf("TEST AJOUT PNJ ---\n");
    pnj_t * pnj1 = creer_pnj(0);
    printf("TEST AFFICHAGE PNJ 1 ---\n");
    afficher_pnj(pnj1);

    printf("TEST SUPPRESSION PNJ ---\n");
    supprimer_pnj(pnj1);

    printf("TEST AFFICHAGE PNJ 3 ---\n");
    afficher_pnj(pnj1);

    return 0;
}