/**
 * \fn main()
 * \brief Test de toutes les fonctions pour personnage
 * \param sans paramètre
 * \return 0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <personnage.h>
#include <objets.h>


int main(){
    printf("Bienvenue dans le jeu\n");
    printf("Création de personnage\n");
    printf("Saisir le nom : ");
    char nom_perso[50];
    fgets(nom_perso, 50, stdin);
    printf("Choisissez la classe du personnage : \n");
    printf("1 : Guerrier\n");
    printf("2 : Mage\n");
    printf("3 : Ninja\n");
    printf("4 : Tank\n");
    printf("5 : Archer\n");
    int num_classe;
    printf("Choix : ");
    scanf("%d", &num_classe);
    personnage_t * p1=creer_personnage(nom_perso,num_classe);
    afficher_perso(p1);
    p1->exp=101;
    upgrade_perso(p1);
    return (0);
}