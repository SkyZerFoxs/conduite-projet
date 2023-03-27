#include <stdio.h>

#include <objets.h>

int main(){ 
    // chargement liste objet
    liste_objet_t * listeObjets = load_liste_objets("asset/objet/objet.txt");

    afficher_liste_objet(listeObjets);

    // Detruire liste objets
    detruire_liste_objet(&listeObjets);
}