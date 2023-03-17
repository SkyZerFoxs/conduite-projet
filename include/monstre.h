#ifndef MONSTRE_H
#define MONSTRE_H

typedef struct monstre_s{
    char * nom;
    int niveau;
    int pos_y;
    int pos_x;
    int pv;
    int atk;
    int def;
}monstre_t;

extern monstre_t * creer_monstre(char * nom,int niv);
extern void afficher_monstre(monstre_t * monstre);
extern void supprimer_monstre(monstre_t ** monstre);

#endif