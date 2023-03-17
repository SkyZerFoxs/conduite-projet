#ifndef PERSO_H
#define PERSO_H

#include <caract.h>

typedef struct capacity_s{
    int apprise;
    int portee_x;
    int portee_y;
    int degats;
}capacity_t;

typedef struct personnage_s{
    char * nom;
    int niveau;
    int exp;
    int pts_upgrade;
    int inventaire[10];
    int pos_x;
    int pos_y;
    capacity_t * base;
    capacity_t * speciale;
    capacity_t * ulti;
    caract_t * caract;
}personnage_t;


extern personnage_t * creer_personnage(char * nom);
extern void afficher_perso(personnage_t * perso);
extern void supprimer_perso(personnage_t ** perso);
extern void upgrade_perso(personnage_t * perso);

#endif