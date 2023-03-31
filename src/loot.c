/**
 *	\file combat.c
 *  \brief fonction pour gérer les loots
 *  \author Raccouard Nathan
 *  \version 1.0
 *  \date 31/03/23
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

objet_t * loot_monstre(liste_t ** liste, int niv_monstre){
    srand( time( NULL ) );
    int drop = rand() % 3; 
    if(drop==1){
        int item_drop = rand() % 8;
        if(item_drop=0){
            return (*liste)->liste[item_drop+niv_monstre];
        }
        else{
            return (*liste)->liste[item_drop*3+niv_monstre];
        }
    else{
        
    }
    }
}

objet_t * loot_coffre(liste_t ** liste, int niv_monstre){
    srand( time( NULL ) );
    int drop = rand() % 3; 
    if(drop==1){
        int item_drop = rand() % 8;
        if(item_drop=0){
            return (*liste)->liste[item_drop+niv_monstre];
        }
        else{
            return (*liste)->liste[item_drop*3+niv_monstre];
        }
    else{
        
    }
    }
}