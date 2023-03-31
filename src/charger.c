#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <personnage.h>

int load_game(int * pos_x, int * pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet,int lastQuest, char * nom_fichier){
    FILE * filename = fopen(nom_fichier, "r");   
    fscanf(filename, "%d:%d:\n",pos_x,pos_y);
    fscanf(filename, "%d:%d:%d:\n",perso->niveau,perso->exp,perso->pts_upgrade);
    fscanf(filename, "%d:%d:%d:\n",perso->caract->pv,perso->caract->maxPv,perso->caract->def,perso->caract->atk);
    fscanf(filename, "%d:%d:\n",inventaire->invHeight,inventaire->invWidth);
    for (int i=0;i<inventaire->invHeight;i++){
        for (int j=0;j<inventaire->invWidth;j++){
            fscanf(filename, "%d:",inventaire[i][j]);
        }
    }
    fscanf(filename,"%:d\n",inventaire->equipementHeight,inventaire->equipementWidth);
    for (int i=0;i<inventaire->equipementHeight;i++){
        for (int j=0;j<inventaire->equipementWidth;j++){
            fscanf(filename, "%d:",inventaire[i][j]);
        }
    }
    fscanf(filename,"%d\n",liste_objet->nbElem);
    for(int i=0;i<liste_objet->nbElem;i++){
        fscanf(filename,"%d:",liste_objet->tab[i]->n);
    }
    fscanf(filename,"\n");
    fscanf(filename,"%d:",lastQuest);
}