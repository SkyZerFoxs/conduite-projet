/**
 *	\file quete.c
 *  \brief fonctions de test des quetes
 *  \author Sardon William
 *  \version 1.0
 *  \date 9/02/2023
**/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module.h"


int main(){
    pnj_t * pnj1 = creer_pnj("nom1",0);
    pnj_t * pnj2 = creer_pnj("nom2",1);

    afficher_pnj(pnj1);
    quete_t * q1 = creer_quete(pnj1, "description de la premiere quete", "recompense de la premiere quete");
    afficher_quete(q1);
    afficher_pnj(pnj1);
    
    return 0;
}