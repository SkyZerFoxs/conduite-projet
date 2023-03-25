#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objets.h>

int main(){
    objet_t ** tab=malloc(sizeof(objet_t)*25);  
    ecrire_objet_tab("objet.txt", tab);
    afficher_objet(tab);
    detruire_tab_objet(tab);
}
