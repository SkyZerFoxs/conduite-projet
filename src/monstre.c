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
void evolution_monstre(monstre_t * monstre, int niv){
    monstre->pv=200*niv;
    monstre->atk=100*niv;
    monstre->def=10*niv;
}

extern
monstre_t * creer_monstre(char * nom){
    monstre_t * monstre=NULL;
    monstre=malloc(sizeof(monstre_t)+1);
    monstre->nom=malloc(sizeof(strlen(nom))+1);
    strcpy(monstre->nom,nom);
    monstre->pv=200;
    monstre->atk=100;
    monstre->def=10;
    return monstre;
}