#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objets.h>

int main(){ 
    objet_t ** tab=load_objets("objet.txt");
    afficher_objet(tab);
    detruire_tab_objet(tab);
}
