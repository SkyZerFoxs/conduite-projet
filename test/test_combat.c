/**
 * \fn main()
 * \brief Test de toutes les fonctions de combat
 * \param sans paramètre
 * \return 0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <personnage.h>
#include <combat.h>
#include <monstre.h>

int main(){
    personnage_t * perso=creer_personnage("jean",1);
    monstre_t * monstre=creer_monstre("slime",1);
    char atk;
    printf("Combat niveau 1");
    while(perso->caract->pv>0 && monstre->pv>0){
        printf("PV perso : %d\n",perso->caract->pv);
        printf("PV monstre : %d\n",monstre->pv);
        printf("Choisir action\n");
        scanf(" %c",&atk); 
        combat_joueur(perso,monstre,atk);
        combat_monstre(monstre,perso);
    }
    perso->pts_upgrade=5;
    upgrade_perso(perso);
    monstre_t * monstre2=creer_monstre("slime",2);
    printf("Combat niveau 2");
    while(perso->caract->pv>0 && monstre2->pv>0){
        printf("PV perso : %d\n",perso->caract->pv);
        printf("PV monstre : %d\n",monstre2->pv);
        printf("Choisir action\n");
        scanf(" %c",&atk); 
        combat_joueur(perso,monstre2,atk);
        combat_monstre(monstre2,perso);
    }
}