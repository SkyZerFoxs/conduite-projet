#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <personnage.h>

int save_game(int pos_x, int pos_y, personnage_t * perso, inventaire_t * inventaire,liste_objet_t * liste_objet,int lastQuest){
    FILE * filename = fopen("jean.txt", "w");
    fprintf(filename, "%d:%d:\n",pos_x,pos_y);
    fprintf(filename, "%d:%d:%d:\n",perso->niveau,perso->exp,perso->pts_upgrade);
    fprintf(filename, "%d:%d:%d:\n",perso->caract->pv,perso->caract->maxPv,perso->caract->def,perso->caract->atk);
    fprintf(filename, "%d:%d:\n",inventaire->invHeight,inventaire->invWidth);
    for (int i=0;i<inventaire->invHeight;i++){
        for (int j=0;j<inventaire->invWidth;j++){
            fprintf(filename, "%d:",inventaire[i][j]);
        }
        fprintf(filename, "\n");
    }
    fprintf(filename,"%d:\n",inventaire->equipementHeight,inventaire->equipementWidth);
    for (int i=0;i<inventaire->equipementHeight;i++){
        for (int j=0;j<inventaire->equipementWidth;j++){
            fprintf(filename, "%d:",inventaire[i][j]);
        }
        fprintf(filename, "\n");
    }
    fprintf(filename,"%d:\n",liste_objet->nbElem);
    for(int i=0;i<liste_objet->nbElem;i++){
        fprintf(filename,"%d:",liste_objet->tab[i]->n);
    }
    fprintf(filename,"%d:",lastQuest);
}