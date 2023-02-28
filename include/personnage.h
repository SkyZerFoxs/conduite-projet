#ifndef CARACT_H
#define CARACT_H

#include <caract.h>

typedef struct personnage_s{
    char * nom;
    int classe;
    int niveau;
    int exp;
    int pts_upgrade;
    int inventaire[10];
    caract_t * caract;
}personnage_t;

//static char * tab_classe[5]={"Guerrier","Mage","Ninja","Tank","Archer"};

extern personnage_t * creer_personnage(char * nom, int num_class);
extern void afficher_perso(personnage_t * perso);
extern void supprimer_perso(personnage_t ** perso);
extern void upgrade_perso(personnage_t * perso);

#endif