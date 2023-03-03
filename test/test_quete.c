/**
 *	\file quete.c
 *  \brief fonctions de test des quetes
 *  \author Sardon William
 *  \version 2.1
 *  \date 28/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quete.h"

int main(){

    printf("TEST AJOUT QUETE ---\n");
    quete_t * q1 = creer_quete(0);
    printf("TEST AFFICHAGE QUETE 1 ---\n");
    afficher_quete(q1);

    printf("TEST VALIDATION QUETE ---\n");
    valider_quete(&q1);

    printf("TEST AFFICHAGE QUETE 2 ---\n");
    afficher_quete(q1);

    printf("TEST SUPPRESSION QUETE ---\n");
    supprimer_quete(&q1);

    printf("TEST AFFICHAGE QUETE 3 ---\n");
    afficher_quete(q1);

    return 0;
}