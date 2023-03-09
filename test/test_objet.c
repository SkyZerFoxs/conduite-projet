#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objet.h>

int main(){
    objet_t * plastron=creer_objet(1,50,0,100,0,0,0,50,25);
    objet_t * epee=creer_objet(2,0,20,0,0,0,50,10,5);
    objet_t * sceptre=creer_objet(3,0,0,0,50,20,0,20,10);
    afficher_objet(plastron);
    afficher_objet(epee);
    afficher_objet(sceptre);
    supprimer_objet(&plastron);
    supprimer_objet(&epee);
    supprimer_objet(&sceptre);
}
