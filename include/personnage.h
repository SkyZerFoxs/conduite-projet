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
    int palierExp;

    int pts_upgrade;

    // Stats du perso
    caract_t * caract;

    // Stat bonus de l'equipement
    caract_t * equipement[6];

}personnage_t;


typedef struct inventaire_s {
    // Données inventaire
    int **inventaire;
    int nbPlaceInventaire;
    int invHeight;
    int invWidth;

    // Données inventaire equipement
    int **equipement;
    int nbPlaceEquipement;
    int equipementHeight;
    int equipementWidth;
}inventaire_t;

extern personnage_t * creer_personnage(char * nom);
extern void afficher_perso(personnage_t * perso);
extern void supprimer_perso(personnage_t ** perso);
extern void upgrade_perso(personnage_t * perso);

#endif