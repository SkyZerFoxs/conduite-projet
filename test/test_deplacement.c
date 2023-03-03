/**
 *	\file pnj.c
 *  \brief Tests de deplacement avec matrice
 *  \author Sardon William
 *  \version 1.0
 *  \date 3/03/2023
**/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "deplacement.h"

int main(){
    int collision[N][M];
    int dir = 1;
    int x = 10;
    int y = 10;

    init_matrice(collision);

    system("clear");

    while(dir!=0){
        affiche_matrice(collision, x, y);

        printf("haut : %d\tdroite : %d\tbas : %d\tgauche : %d\n", H, D, B, G);
        printf("QUIT : 0\n");
        scanf("%d",&dir);

        deplacement_perso(collision, &x, &y, dir);

        system("clear");
    }
    
}