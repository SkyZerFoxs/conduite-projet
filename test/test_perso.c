#include <stdio.h>
#include <personnage.h>

int main() {
    personnage_t * perso = creer_personnage("Cody");

    afficher_perso(perso);

    supprimer_perso(&perso);

    return 0;
}