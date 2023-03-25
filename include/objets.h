#ifndef OBJETS_H
#define OBJETS_H

#include <caract.h>
#include <stdio.h>

#define NB_ITEMS 25

typedef struct objet_s{
    int id;
    char * nom;
    int niv;
    int nb;
    caract_t * stats;
    int prix_achat;
    int prix_vente;
}objet_t;


extern void ecrire_objet_tab(char * nom_fich, objet_t ** tab);
extern void afficher_objet(objet_t ** tab);
extern void detruire_tab_objet(objet_t ** tab);
extern void detruire_objet(objet_t ** objet);

#endif