#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <monstre.h>

int main(){
    monstre_t * slime=creer_monstre("slime",1);
    printf("Stats de base\n");
    afficher_monstre(slime);
    for (int niv=1;niv<=5;niv++){
        printf("Au niveau %d, les slimes auront\n",niv);
        afficher_monstre(slime);
    }
    supprimer_monstre(&slime);
}