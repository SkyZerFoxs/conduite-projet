#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <monstre.h>

extern
void afficher_monstre(monstre_t * monstre){ 
    printf("PV: %d\n", monstre->pv);
    printf("Attaque : %d\n", monstre->atk);
    printf("Defense : %d\n", monstre->def);
}

void supprimer_monstre(monstre_t ** monstre){
    free((*monstre)->nom);
    (*monstre)->nom=NULL;
    free(*monstre);
    (*monstre)=NULL;

}

extern
monstre_t * creer_monstre(char * nom, int niv){
    monstre_t * monstre=NULL;
    monstre=malloc(sizeof(monstre_t)+1);
    monstre->nom=malloc(sizeof(strlen(nom))+1);
    strcpy(monstre->nom,nom);
    monstre->pos_x=0;
    monstre->pos_y=0;
    monstre->pv=niv*50;
    monstre->atk=niv*5;
    monstre->def=niv*5;
    return monstre;
}