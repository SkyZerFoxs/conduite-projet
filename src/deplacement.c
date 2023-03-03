/**
 *	\file pnj.c
 *  \brief fonctions de deplacement avec matrice
 *  \author Sardon William
 *  \version 1.0
 *  \date 3/03/2023
**/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "deplacement.h"


/**
 * \fn init_matrice (int mat[N][M])
 * \brief Initialise une matrice donnée en paramètre avec des 0 et des 1 pour les obstacles a partir d'un fichier texte
 * \param collision Une matrice de int représentant les cases contenant des obstacles(1) ou libres(0)
**/
void init_matrice (int collision[N][M]){
	FILE * fic;
	int i,j;
	int x,y;
	
	fic = fopen("src/collision.txt","r");
	
	for (i = 0; i < N; i++){
		for (j = 0; j < M; j++){
			collision[i][j] = 0;
		}
	}
	
	while (!feof(fic)){
		fscanf(fic,"%d",&x);
		fscanf(fic,"%d",&y);
		collision[x][y] = 1;
	}
	
	fclose(fic);
}

/**
 * \fn affiche_matrice (int mat[N][M], int x, int y)
 * \brief Affiche la matrice de collision pour test sur terminal
 * \param collision Une matrice de int représentant les cases contenant des obstacles(1) ou libres(0)
 * \param x Entier de position en ligne du personnage
 * \param y Entier de position en colonne du personnage
**/
void affiche_matrice (int collision[N][M], int x, int y){
	int i,j;
	
	for (i = 0; i < N; i++){
		for (j = 0; j < M; j++){
            if(i == x && j == y)
                printf("2 ");
            else{
                if(collision[i][j]==0)
			        printf("\u25A1 ");
                else
                    printf("\u25A0 ");
            }
		}
		printf("\n");
	}
}


/**
 * \fn deplacement_perso (int mat[N][M], int x, int y, int dir)
 * \brief Deplace le joueur si il n'y a pas d'obstacle à sa destination
 * \param collision Une matrice de int représentant les cases contenant des obstacles(1) ou libres(0)
 * \param x Entier de position en ligne du personnage
 * \param y Entier de position en colonne du personnage
 * \param dir Entier représentant la direction du deplacement (haut, droite, bas, gauche)
 * \return FAUX si deplacement impossible, VRAI sinon
**/
int deplacement_perso (int collision[N][M], int * x, int * y, int dir){
    if (dir == H){
        if (collision[(*x)-1][*y] == 0){
            (*x)--;
            return(1);
        }
        else{
            return(0);
        }
    }
	else if(dir == D){
        if (collision[*x][(*y)+1] == 0){
            (*y)++;
            return(1);
        }
        else{
            return(0);
        }
    }
    else if(dir == B){
        if (collision[(*x)+1][*y] == 0){
            (*x)++;
            return(1);
        }
        else{
            return(0);
        }
    }
    else if(dir == G){
        if(collision[*x][(*y)-1] == 0){
            (*y)--;
            return(1);
        }
        else{
            return(0);
        }
    }
}

