#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <objets.h>

/**
 * \fn main()
 * \brief Test de toutes les fonctions des objets
 * \param sans paramètre
 * \return 0
*/

void main(){ 
    objet_t ** tab=load_objets("objet.txt");
    afficher_objet(tab);
    detruire_tab_objet(tab);
}
