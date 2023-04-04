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

int loot_monstre(int niv_monstre){
    int drop = rand() % 3; 
    if(drop==1){
        int item_drop = rand() % 8;
        int niv_obj= rand() % 100;
        if(niv_monstre>=1 && niv_monstre<=10){
            if(niv_obj>=0 && niv_obj<=4){
                return item_drop*3+2;
            }
            else if(item_drop>=5 && item_drop<=15){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
        else if(niv_monstre>=11 && niv_monstre<=20){
            if(niv_obj>=0 && niv_obj<=9){
                return item_drop*3+2;
            }
            else if(item_drop>=10 && item_drop<=70){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
        else if(niv_monstre>=21 && niv_monstre<=25){
            if(niv_obj>=0 && niv_obj<=49){
                return item_drop*3+2;
            }
            else if(item_drop>=50 && item_drop<=80){
                return item_drop*3+1;
            }
            else{
                return item_drop*3;
            }
        }
    }
}

int loot_coffre(int niv_coffre){
    int item_drop = rand() % 8;
    return item_drop*3+niv_coffre;

}